/*
 * Original BMP.h provided by 'sol-prog' and 'cloudwiser' under:
 *   GNU General Public License v3.0
 * Repository with orginal copy can be found at:
 *   https://github.com/sol-prog/cpp-bmp-images
 * 
 * Modifications are by Jack Kilrain inline with license as specified above
*/

#pragma once

#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "../exceptions/image/BMPOriginPositionError.h"
#include "../exceptions/image/BPPSizeError.h"
#include "../exceptions/image/FileFormatError.h"
#include "../exceptions/image/ImageColourError.h"
#include "../exceptions/image/ImageDimensionsError.h"
#include "../exceptions/image/ImageFileStreamError.h"
#include "../exceptions/image/ImagePixelError.h"
#include "TextureColours.h"

using namespace std;

#define BMP_FILE_TYPE 0x4D42

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{BMP_FILE_TYPE};  // File type always BM which is BMP_FILE_TYPE (stored as hex uint16_t in little endian)
    uint32_t file_size{0};       // Size of the file (in bytes)
    uint16_t reserved1{0};       // Reserved, always 0
    uint16_t reserved2{0};       // Reserved, always 0
    uint32_t offset_data{0};     // Start position of pixel data (bytes from the beginning of the file)
};

struct BMPInfoHeader {
    uint32_t size{0};         // Size of this header (in bytes)
    int32_t width{0};         // width of bitmap in pixels
    int32_t height{0};        // width of bitmap in pixels
                              //       (if positive, bottom-up, with origin in lower left corner)
                              //       (if negative, top-down, with origin in upper left corner)
    uint16_t planes{1};       // No. of planes for the target device, this is always 1
    uint16_t bit_count{0};    // No. of bits per pixel
    uint32_t compression{0};  // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
    uint32_t size_image{0};   // 0 - for uncompressed images
    int32_t x_pixels_per_meter{0};
    int32_t y_pixels_per_meter{0};
    uint32_t colors_used{0};       // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
    uint32_t colors_important{0};  // No. of colors used for displaying the bitmap. If 0 all colors are required
};

struct BMPColorHeader {
    uint32_t red_mask{0x0ff0000};          // Bit mask for the red channel
    uint32_t green_mask{0x000ff00};        // Bit mask for the green channel
    uint32_t blue_mask{0x00000ff};         // Bit mask for the blue channel
    uint32_t alpha_mask{0xff000000};        // Bit mask for the alpha channel
    uint32_t color_space_type{0x73524742};  // Default "sRGB" (0x73524742)
    uint32_t unused[16]{0};                 // Unused data for sRGB color space
};
#pragma pack(pop)

struct BMP {
    BMPFileHeader file_header;
    BMPInfoHeader bmp_info_header;
    BMPColorHeader bmp_color_header;
    vector<uint8_t> data;

    BMP() {}

    BMP(const char *fname) {
        read(fname);
    }

    void read(const char *fname) {
        ifstream inp{fname, ios_base::binary};
        if (inp) {
            inp.read((char *)&file_header, sizeof(file_header));
            if (file_header.file_type != BMP_FILE_TYPE) {
                string format = string(fname);
                format = format.substr(format.find("."));
                throw FileFormatError(format);
            }
            inp.read((char *)&bmp_info_header, sizeof(bmp_info_header));

            // The BMPColorHeader is used only for transparent images
            if (bmp_info_header.bit_count == 32) {
                // Check if the file has bit mask color information
                if (bmp_info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
                    inp.read((char *)&bmp_color_header, sizeof(bmp_color_header));
                    // Check if the pixel data is stored as BGRA and if the color space type is sRGB
                    checkColorHeader(bmp_color_header);
                } else {
                    cerr << "Error! The file \"" << fname << "\" does not seem to contain bit mask information\n";
                    string format = string(fname);
                    format = format.substr(format.find("."));
                    throw FileFormatError(format);
                }
            }

            // Jump to the pixel data location
            inp.seekg(file_header.offset_data, inp.beg);

            // Adjust the header fields for output.
            // Some editors will put extra info in the image file, we only save the headers and the data.
            if (bmp_info_header.bit_count == 32) {
                bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
                file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
            } else {
                bmp_info_header.size = sizeof(BMPInfoHeader);
                file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
            }
            file_header.file_size = file_header.offset_data;

            if (bmp_info_header.height < 0) {
                throw BMPOriginPositionError(bmp_info_header.height);
            }

            data.resize(bmp_info_header.width * bmp_info_header.height * bmp_info_header.bit_count / 8);

            // Here we check if we need to take into account row padding
            if (bmp_info_header.width % 4 == 0) {
                inp.read((char *)data.data(), data.size());
                file_header.file_size += static_cast<uint32_t>(data.size());
            } else {
                row_stride = bmp_info_header.width * bmp_info_header.bit_count / 8;
                uint32_t new_stride = makeStrideAligned(4);
                vector<uint8_t> padding_row(new_stride - row_stride);

                for (int y = 0; y < bmp_info_header.height; ++y) {
                    inp.read((char *)(data.data() + row_stride * y), row_stride);
                    inp.read((char *)padding_row.data(), padding_row.size());
                }
                file_header.file_size += static_cast<uint32_t>(data.size()) + bmp_info_header.height * static_cast<uint32_t>(padding_row.size());
            }
        } else {
            throw ImageFileStreamError(fname);
        }
    }

    BMP(int32_t width, int32_t height, bool has_alpha = true) {
        if (width <= 0 || height <= 0) {
            throw ImageDimensionsError(width, height);
        }

        bmp_info_header.width = width;
        bmp_info_header.height = height;
        if (has_alpha) {
            bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
            file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);

            bmp_info_header.bit_count = 32;
            bmp_info_header.compression = 3;
            row_stride = width * 4;
            data.resize(row_stride * height);
            file_header.file_size = file_header.offset_data + data.size();
        } else {
            bmp_info_header.size = sizeof(BMPInfoHeader);
            file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

            bmp_info_header.bit_count = 24;
            bmp_info_header.compression = 0;
            row_stride = width * 3;
            data.resize(row_stride * height);

            uint32_t new_stride = makeStrideAligned(4);
            file_header.file_size = file_header.offset_data + static_cast<uint32_t>(data.size()) + bmp_info_header.height * (new_stride - row_stride);
        }
    }

    vector<Colour> getCol(float percentageDist) {
        u_int32_t col = bmp_info_header.width * percentageDist;
        vector<Colour> colData(bmp_info_header.height);
        uint32_t channels = bmp_info_header.bit_count / 8;
        for (u_int32_t i = 0; i < bmp_info_header.height; i++) {
            // uint32_t R, G, B, A = 1;
            // B = data[channels * (i * bmp_info_header.width + col) + 0];
            // G = data[channels * (i * bmp_info_header.width + col) + 1];
            // R = data[channels * (i * bmp_info_header.width + col) + 2];
            // if (channels == 4) {
            //     A = data[channels * (i * bmp_info_header.width + col) + 3];
            // }
            // colData.at(i) = {(GLdouble)R, (GLdouble)G, (GLdouble)B, (GLdouble)A};
            colData.at(i) = getPixel(col, i); // <-- Alternative using existing method
        }
        return colData;
    }

    Colour getPixel(u_int32_t x, u_int32_t y) {
        if (x > (uint32_t)bmp_info_header.width || y > (uint32_t)bmp_info_header.height) {
            throw ImagePixelError(x, y);
        }
        uint32_t R, G, B, A = 1;
        uint32_t channels = bmp_info_header.bit_count / 8;
        B = data[channels * (y * bmp_info_header.width + x) + 0];
        G = data[channels * (y * bmp_info_header.width + x) + 1];
        R = data[channels * (y * bmp_info_header.width + x) + 2];
        if (channels == 4) {
            A = data[channels * (y * bmp_info_header.width + x) + 3];
        }
        // printColour({(GLdouble)R, (GLdouble)G, (GLdouble)B, (GLdouble)A});
        return {(GLdouble)R, (GLdouble)G, (GLdouble)B, (GLdouble)A};
    }

    void setPixel(uint32_t x, uint32_t y, uint8_t B, uint8_t G, uint8_t R, uint8_t A) {
        if (x > (uint32_t)bmp_info_header.width || y > (uint32_t)bmp_info_header.height) {
            throw ImagePixelError(x, y);
        }

        uint32_t channels = bmp_info_header.bit_count / 8;
        data[channels * (y * bmp_info_header.width + x) + 0] = B;
        data[channels * (y * bmp_info_header.width + x) + 1] = G;
        data[channels * (y * bmp_info_header.width + x) + 2] = R;
        if (channels == 4) {
            data[channels * (y * bmp_info_header.width + x) + 3] = A;
        }
    }

   private:
    uint32_t row_stride{0};

    // Add 1 to the row_stride until it is divisible with align_stride
    uint32_t makeStrideAligned(uint32_t align_stride) {
        uint32_t new_stride = row_stride;
        while (new_stride % align_stride != 0) {
            new_stride++;
        }
        return new_stride;
    }

    // Check if the pixel data is stored as BGRA and if the color space type is sRGB
    void checkColorHeader(BMPColorHeader &bmp_color_header) {
        BMPColorHeader expected_color_header;
        if (expected_color_header.red_mask != bmp_color_header.red_mask ||
            expected_color_header.blue_mask != bmp_color_header.blue_mask ||
            expected_color_header.green_mask != bmp_color_header.green_mask ||
            expected_color_header.alpha_mask != bmp_color_header.alpha_mask) {
            string mask_val = to_string(bmp_color_header.red_mask);
            mask_val.append(to_string(bmp_color_header.blue_mask));
            mask_val.append(to_string(bmp_color_header.green_mask));
            mask_val.append(to_string(bmp_color_header.alpha_mask));
            throw ImageColourError("mask", "BGRA", mask_val);
        }
        if (expected_color_header.color_space_type != bmp_color_header.color_space_type) {
            throw ImageColourError("space", "sRGB", to_string(bmp_color_header.color_space_type));
        }
    }
};