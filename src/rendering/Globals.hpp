#pragma once

#include <iostream>
#include <sstream>

#include "../io/configuration/sections/PlayerCfg.hpp"
#include "../io/configuration/sections/LoggingCfg.hpp"
#include "../io/configuration/sections/MinimapCfg.hpp"
#include "../io/configuration/sections/RenderCfg.hpp"
#include "../io/logging/GLDebug.hpp"
#include "../environment/world/Coordinates.hpp"
#include "raycaster/Ray.hpp"

using namespace std;

#define IMUL_2(x) ((x) << 1)
#define IDIV_2(x) ((x) >> 1)
#define IMUL_4(x) ((x) << 2)
#define IDIV_4(x) ((x) >> 2)
#define IMUL_8(x) ((x) << 3)
#define IDIV_8(x) ((x) >> 3)

#define IMUL_16(x) ((x) << 4)
#define IDIV_16(x) ((x) >> 4)
#define IMUL_32(x) ((x) << 5)
#define IDIV_32(x) ((x) >> 5)
#define IMUL_64(x) ((x) << 6)
#define IDIV_64(x) ((x) >> 6)

#define IMUL_128(x) ((x) << 7)
#define IDIV_128(x) ((x) >> 7)
#define IMUL_256(x) ((x) << 8)
#define IDIV_256(x) ((x) >> 8)

#define radToCoord(r) IDIV_64((int) r)

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

static ConfigSection::PlayerCfg playerCfg = ConfigSection::PlayerCfg();
static ConfigSection::MinimapCfg minimapCfg = ConfigSection::MinimapCfg();
static ConfigSection::LoggingCfg loggingCfg = ConfigSection::LoggingCfg();
static ConfigSection::RenderCfg renderCfg = ConfigSection::RenderCfg();

static GLDebugContext debugContext = GLDebugContext();

typedef unsigned long int int_id;
typedef vector<uint32_t> PNGTex;
typedef uint64_t Tick;

namespace Rendering {
    typedef vector<Ray> RayBuffer;
    typedef vector<double> ZBuffer;
}

Tick global_tick;

#define __EMTPY_VOID_FUNC__() (void (func*)())