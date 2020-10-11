#pragma once

#include "../configuration/mappers/LoggingCfg.hpp"
#include "../configuration/mappers/MinimapCfg.hpp"
#include "../configuration/mappers/PlayerCfg.hpp"
#include "../configuration/mappers/RenderCfg.hpp"
#include "../logging/GLDebug.hpp"
#include "../map/Coordinates.hpp"

#include <iostream>
#include <sstream>

using namespace std;

#define IMUL_2(x) (x << 1)
#define IDIV_2(x) (x >> 1)
#define IMUL_4(x) (x << 2)
#define IDIV_4(x) (x >> 2)
#define IMUL_8(x) (x << 3)
#define IDIV_8(x) (x >> 3)

#define IMUL_16(x) (x << 4)
#define IDIV_16(x) (x >> 4)
#define IMUL_32(x) (x << 5)
#define IDIV_32(x) (x >> 5)
#define IMUL_64(x) (x << 6)
#define IDIV_64(x) (x >> 6)

// ../~
#define RES_DIR string("resources/")
#define SRC_DIR string("src/")

// ../../~
#define MAPS_DIR RES_DIR + "maps/"
#define TEX_DIR RES_DIR + "textures/"
#define CFG_DIR RES_DIR + "configs/"

// ../../../~
#define DEFAULT_CONFIG CFG_DIR + "config.ini"

string toHex(int value) noexcept {
    stringstream sstream;
    sstream << hex << value;
    return sstream.str();
};

#define ADDR_OF(f) string("0x") + toHex((size_t)(&f))

constexpr double THREE_HALF_PI = 3 * M_PI / 2;

static PlayerCfg playerCfg = PlayerCfg();
static MinimapCfg minimapCfg = MinimapCfg();
static LoggingCfg loggingCfg = LoggingCfg();
static RenderCfg renderCfg = RenderCfg();

static GLDebugContext debugContext = GLDebugContext();