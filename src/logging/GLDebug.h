#pragma once

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../configuration/mappers/LoggingCfg.h"
#include "../exceptions/debug/DebugLogDirCreateError.h"
#include "../exceptions/debug/DebugLogWriteError.h"
#include "../exceptions/debug/ExceededDebugMessageSize.h"
#include "../raytracer/Globals.h"

using namespace std;

#define GL_MAX_DEBUG_MSG_LENGTH 2048
#define LOGS_DIR string("logs/")

enum GL_DEBUG_TYPE : uint16_t {
    DEBUG_TYPE_ERROR = 0x001,
    DEBUG_TYPE_UNDEFINED_BEHAVIOR = 0x002,
    DEBUG_TYPE_PERFORMANCE = 0x003,
    DEBUG_TYPE_MARKER = 0x004,
    DEBUG_TYPE_PUSH_GROUP = 0x005,
    DEBUG_TYPE_POP_GOUP = 0x006,
    DEBUG_TYPE_OTHER = 0x007
};

static const string GL_DEBUG_TYPE_LUT[] = {
    "DEBUG_TYPE_ERROR",
    "DEBUG_TYPE_UNDEFINED_BEHAVIOR",
    "DEBUG_TYPE_PERFORMANCE",
    "DEBUG_TYPE_MARKER",
    "DEBUG_TYPE_PUSH_GROUP",
    "DEBUG_TYPE_POP_GOUP",
    "DEBUG_TYPE_OTHER"
};

#define GL_DEBUG_TYPE_STRING(gl_type) GL_DEBUG_TYPE_LUT[gl_type]

enum GL_DEBUG_SOURCE : uint16_t {
    DEBUG_SOURCE_API = 0x010,
    DEBUG_SOURCE_OS_X_SYSTEM = 0x020,
    DEBUG_SOURCE_SHADER_COMPILER = 0x030,
    DEBUG_SOURCE_THIRD_PARTY = 0x040,
    DEBUG_SOURCE_APPLICATION = 0x050,
    DEBUG_SOURCE_OTHER = 0x060
};

static const string GL_DEBUG_SOURCE_LUT[] = {
    "DEBUG_SOURCE_API",
    "DEBUG_SOURCE_OS_X_SYSTEM",
    "DEBUG_SOURCE_SHADER_COMPILER",
    "DEBUG_SOURCE_THIRD_PARTY",
    "DEBUG_SOURCE_APPLICATION",
    "DEBUG_SOURCE_OTHER"
};

#define GL_DEBUG_SOURCE_STRING(gl_source) GL_DEBUG_SOURCE_LUT[(gl_source >> 4) - 1]

enum GL_DEBUG_SEVERITY : uint16_t {
    DEBUG_SEVERITY_HIGH = 0x100,
    DEBUG_SEVERITY_MEDIUM = 0x200,
    DEBUG_SEVERITY_LOW = 0x300,
    DEBUG_SEVERITY_INFO = 0x400
};

static const string GL_DEBUG_SEVERITY_LUT[] = {
    "DEBUG_SEVERITY_HIGH",
    "DEBUG_SEVERITY_MEDIUM",
    "DEBUG_SEVERITY_LOW",
    "DEBUG_SEVERITY_INFO"
};

#define GL_DEBUG_SEVERITY_STRING(gl_severity) GL_DEBUG_SEVERITY_LUT[(gl_severity >> 8) - 1]

struct DEBUG_NAME_FORMAT {
    string prefix;
    string suffix;
};

enum DEBUG_LOG_FORMAT {
    DEFAULT,
    DEFAULT_TIME_ONLY,
    CUSTOM_PREFIX,
    CUSTOM_PREFIX_TIME_ONLY
};

static const DEBUG_NAME_FORMAT DEBUG_LOG_FORMAT_LUT[] = {
    DEBUG_NAME_FORMAT{"debug_log_", "%d-%m-%Y_%H:%M:%S"},
    DEBUG_NAME_FORMAT{"debug_log_", "%H:%M:%S"},
    DEBUG_NAME_FORMAT{"", "%d-%m-%Y_%H:%M:%S"},
    DEBUG_NAME_FORMAT{"", "%H:%M:%S"}
};

class GLDebugContext {
    private:
        string filename = LOGS_DIR;
        DEBUG_NAME_FORMAT format;
        string getCurrentTime(const char* date_format = "%d-%m-%Y_%H:%M:%S");
        bool checkFileExists(const string& filename);
        string toHex(int value) noexcept;
    public:
        GLDebugContext();
        GLDebugContext(LoggingCfg* l_cfg, const DEBUG_LOG_FORMAT format = DEBUG_LOG_FORMAT::DEFAULT, string optional_prefix = "");
        ~GLDebugContext();
        void glDebugMessageCallback(GL_DEBUG_SOURCE source, GL_DEBUG_TYPE type, GL_DEBUG_SEVERITY severity, const string& message);
        void logAppInfo(const string& message);
        LoggingCfg* l_cfg;
};

GLDebugContext::GLDebugContext(){};

GLDebugContext::GLDebugContext(LoggingCfg* l_cfg, const DEBUG_LOG_FORMAT format, string optional_prefix) {
    this->l_cfg = l_cfg;
    this->format = DEBUG_LOG_FORMAT_LUT[format];
    this->filename += this->format.prefix + optional_prefix + getCurrentTime(this->format.suffix.c_str()) + ".log";
    if (!checkFileExists(LOGS_DIR) && mkdir(LOGS_DIR.c_str(), 0777) == -1) {
        throw DebugLogDirCreateError();
    }

    ofstream logFile(this->filename);
    if (logFile.is_open()) {
        logFile << "#### START OF LOG ####\n";
        if (!this->l_cfg->gl_debug) {
            logFile << "[gl_debug = false] logging to file disabled.";
        }
        logFile.close();
    } else {
        throw DebugLogWriteError(this->filename);
    }
};

GLDebugContext::~GLDebugContext() {};

void GLDebugContext::glDebugMessageCallback(GL_DEBUG_SOURCE source, GL_DEBUG_TYPE type, GL_DEBUG_SEVERITY severity, const string& message) {
    if (message.size() > GL_MAX_DEBUG_MSG_LENGTH) {
        throw ExceededDebugMessageSize(message.size(), GL_MAX_DEBUG_MSG_LENGTH);
    }

    string timeFormat = this->format.suffix;
    size_t pos_ = timeFormat.find("_");
    string currentTime = getCurrentTime(string(timeFormat.substr(0, pos_) + timeFormat.substr(pos_, timeFormat.size() - 1)).c_str());

    if (l_cfg->gl_debug) {
        FILE* out_loc = type == DEBUG_TYPE_ERROR || type == DEBUG_TYPE_UNDEFINED_BEHAVIOR ? stderr : stdout;
        fprintf(out_loc, "[%s] {%x|%x|%x ~ %x}%s %s :: %s\n",
            currentTime.c_str(),
            type, source, severity,
            type | source | severity,
            (type == DEBUG_TYPE_ERROR ? " ** GL ERROR **" : ""),
            GL_DEBUG_SOURCE_STRING(source).c_str(), message.c_str()
        );
    }

    FILE *debugLog;
    debugLog = fopen(filename.c_str(), "a");

    fprintf(debugLog, "[%s] {%s|%s|%s ~ %x}:%s type = 0x%s, severity = 0x%s, message = %s\n",
        currentTime.c_str(),
        toHex(type).c_str(), toHex(source).c_str(), toHex(severity).c_str(),
        type | source | severity,
        (type == DEBUG_TYPE_ERROR ? " ** GL ERROR **" : ""),
        toHex(type).c_str(), toHex(severity).c_str(), message.c_str()
    );

    fclose(debugLog);
};

void GLDebugContext::logAppInfo(const string& message) {
    glDebugMessageCallback(
        DEBUG_SOURCE_APPLICATION,
        DEBUG_TYPE_OTHER,
        DEBUG_SEVERITY_INFO,
        message
    );
};

string GLDebugContext::getCurrentTime(const char* date_format) {
    time_t rawtime;
    struct tm* timeinfo;
    char date_buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(date_buffer, sizeof(date_buffer), date_format, timeinfo);
    return string(date_buffer);
};

bool GLDebugContext::checkFileExists(const string& filename) {
    struct stat st;
    return stat(filename.c_str(), &st) == 0;
};

string GLDebugContext::toHex(int value) noexcept {
    stringstream sstream;
    sstream << hex << value;
    return sstream.str();
};