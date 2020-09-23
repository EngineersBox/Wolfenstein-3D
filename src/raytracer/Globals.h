#pragma once

#include "../configuration/mappers/LoggingCfg.h"
#include "../configuration/mappers/MinimapCfg.h"
#include "../configuration/mappers/PlayerCfg.h"
#include "../configuration/mappers/RenderCfg.h"
#include "../logging/GLDebug.h"

static PlayerCfg playerCfg = PlayerCfg();
static MinimapCfg minimapCfg = MinimapCfg();
static LoggingCfg loggingCfg = LoggingCfg();
static RenderCfg renderCfg = RenderCfg();

static GLDebugContext debugContext = GLDebugContext();