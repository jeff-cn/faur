#
# App
#
F_CONFIG_APP_AUTHOR ?= <author>
F_CONFIG_APP_NAME ?= Untitled
F_CONFIG_APP_NAME_SUFFIX ?=
F_CONFIG_APP_VERSION_MAJOR ?= 1
F_CONFIG_APP_VERSION_MINOR ?= 0
F_CONFIG_APP_VERSION_MICRO ?= 0

#
# Project dirs
#
F_CONFIG_DIR_BUILD ?= build
F_CONFIG_DIR_MEDIA ?= media
F_CONFIG_DIR_SCREENSHOTS ?= screenshots
F_CONFIG_DIR_SRC ?= src

#
# Build and compilation options
#
F_CONFIG_BUILD_ATTRIBUTE_FORMAT_PRINTF ?= printf
F_CONFIG_BUILD_FLAGS_C ?=
F_CONFIG_BUILD_FLAGS_C_OVERRIDE ?=
F_CONFIG_BUILD_FLAGS_C_PEDANTIC ?= 1
F_CONFIG_BUILD_FLAGS_C_STANDARD ?= c99
F_CONFIG_BUILD_FLAGS_CPP ?=
F_CONFIG_BUILD_FLAGS_CPP_OVERRIDE ?=
F_CONFIG_BUILD_FLAGS_CPP_PEDANTIC ?= 1
F_CONFIG_BUILD_FLAGS_CPP_STANDARD ?= c++11
F_CONFIG_BUILD_FLAGS_SETTINGS :=
F_CONFIG_BUILD_FLAGS_SHARED_C_AND_CPP ?=
F_CONFIG_BUILD_FLAGS_SHARED_C_AND_CPP_OVERRIDE ?=
F_CONFIG_BUILD_GEN_LUTS ?= 0
F_CONFIG_BUILD_ID ?= default
F_CONFIG_BUILD_LIBS ?=
F_CONFIG_BUILD_OPT ?= 0
F_CONFIG_BUILD_UID := $(F_BUILD_PLATFORM)_$(F_CONFIG_BUILD_ID)

#
# Systems and platforms
#
F_CONFIG_SYSTEM_ARDUINO ?= 0
F_CONFIG_SYSTEM_CAANOO ?= 0
F_CONFIG_SYSTEM_EMSCRIPTEN ?= 0
F_CONFIG_SYSTEM_GAMEBUINO ?= 0
F_CONFIG_SYSTEM_GP2X ?= 0
F_CONFIG_SYSTEM_LINUX ?= 0
F_CONFIG_SYSTEM_MINGW64 ?= 0
F_CONFIG_SYSTEM_ODROID_GO ?= 0
F_CONFIG_SYSTEM_PANDORA ?= 0
F_CONFIG_SYSTEM_WIZ ?= 0

F_CONFIG_SYSTEM_ARDUINO_BOARD ?=
F_CONFIG_SYSTEM_ARDUINO_PORT ?=

F_CONFIG_SYSTEM_GAMEBUINO_NO_EXIT ?= 0

F_CONFIG_SYSTEM_GP2X_MENU ?= 1
F_CONFIG_SYSTEM_GP2X_MHZ ?= 0

F_CONFIG_SYSTEM_EMSCRIPTEN_INITIAL_MEMORY ?=
F_CONFIG_SYSTEM_EMSCRIPTEN_SHELL ?=

F_CONFIG_SYSTEM_WIZ_SCREEN_FIX ?= 0

#
# System traits
#
ifeq ($(F_CONFIG_TRAIT_DESKTOP), 1)
    F_CONFIG_TRAIT_KEYBOARD ?= 1
endif

F_CONFIG_TRAIT_CONSOLE ?= 1
F_CONFIG_TRAIT_CUSTOM_MAIN ?= 0
F_CONFIG_TRAIT_DESKTOP ?= 0
F_CONFIG_TRAIT_KEYBOARD ?= 0
F_CONFIG_TRAIT_LOW_MEM ?= 0
F_CONFIG_TRAIT_NO_SEEDING ?= 0
F_CONFIG_TRAIT_SCREENSHOTS ?= 1
F_CONFIG_TRAIT_SLOW_MUL ?= 0

F_CONFIG_TRAIT_CONSOLE_TOGGLE ?= F_BUTTON_A,F_BUTTON_B,F_BUTTON_X,F_BUTTON_Y,F_BUTTON_L,F_BUTTON_R

#
# Colors
#
F_CONFIG_COLOR_SCREEN_BORDER ?= 0x1f0f0f
F_CONFIG_COLOR_SPRITE_BORDER ?= 0x00FF00
F_CONFIG_COLOR_SPRITE_KEY ?= 0xFF00FF

#
# Debug options
#
F_CONFIG_DEBUG ?= 0
F_CONFIG_DEBUG_FATAL ?= F_DEBUG_FATAL_DEFAULT
F_CONFIG_DEBUG_MEM_POOL ?= 0

#
# Files
#
F_CONFIG_FILES_EMBED_BLOB ?=
F_CONFIG_FILES_EMBED_OBJ_SAMPLE ?=
F_CONFIG_FILES_EMBED_OBJ_SPRITE ?=
F_CONFIG_FILES_EMBED_PATHS_BLOB ?=
F_CONFIG_FILES_EMBED_PATHS_C ?=
F_CONFIG_FILES_EMBED_PATHS_EMSCRIPTEN ?=
F_CONFIG_FILES_PREFIX ?=

ifdef F_CONFIG_FILES_EMBED_PATHS_BLOB
    ifndef F_CONFIG_FILES_EMBED_BLOB
        F_CONFIG_FILES_EMBED_BLOB := $(call F_MAKE_SPACE_DASH,$(F_CONFIG_APP_NAME)).blob
    endif
else
    F_CONFIG_FILES_EMBED_BLOB :=
endif

#
# FPS
#
F_CONFIG_FPS_CAP ?= F_FPS_CAP_DEFAULT
F_CONFIG_FPS_RATE_TICK ?= 30
F_CONFIG_FPS_RATE_DRAW ?= $(F_CONFIG_FPS_RATE_TICK)

ifeq ($(shell expr $(F_CONFIG_FPS_RATE_TICK) \< 1), 1)
    F_CONFIG_FPS_RATE_TICK := 1
endif

ifeq ($(shell expr $(F_CONFIG_FPS_RATE_DRAW) \< 1), 1)
    F_CONFIG_FPS_RATE_DRAW := 1
endif

ifeq ($(shell expr $(F_CONFIG_FPS_RATE_TICK) \< $(F_CONFIG_FPS_RATE_DRAW)), 1)
    F_CONFIG_FPS_RATE_DRAW := F_CONFIG_FPS_RATE_TICK
endif

#
# Terminal output
#
F_CONFIG_OUT_COLOR_TEXT ?= 0
F_CONFIG_OUT_STDERR ?= stderr
F_CONFIG_OUT_STDOUT ?= stdout

#
# Screen
#
ifdef F_CONFIG_SCREEN_SIZE_WIDTH_HW
    F_CONFIG_SCREEN_SIZE_HEIGHT ?= $(F_CONFIG_SCREEN_SIZE_HEIGHT_HW)
    F_CONFIG_SCREEN_SIZE_WIDTH ?= $(F_CONFIG_SCREEN_SIZE_WIDTH_HW)
endif

ifeq ($(F_CONFIG_LIB_SDL), 1)
    F_CONFIG_SCREEN_FORMAT ?= F_COLOR_FORMAT_ARGB_8888
    F_CONFIG_SCREEN_RENDER ?= F_SCREEN_RENDER_SOFTWARE
else ifeq ($(F_CONFIG_LIB_SDL), 2)
    F_CONFIG_SCREEN_FORMAT ?= F_COLOR_FORMAT_ARGB_8888
    F_CONFIG_SCREEN_RENDER ?= F_SCREEN_RENDER_SDL2
    F_CONFIG_SCREEN_VSYNC ?= 1
endif

F_CONFIG_SCREEN_FORMAT ?= F_COLOR_FORMAT_RGB_565
F_CONFIG_SCREEN_FULLSCREEN ?= 0
F_CONFIG_SCREEN_MAXIMIZED ?= 1
F_CONFIG_SCREEN_RENDER ?= F_SCREEN_RENDER_SOFTWARE
F_CONFIG_SCREEN_SIZE_HEIGHT ?= 240
F_CONFIG_SCREEN_SIZE_HEIGHT_HW ?= 0
F_CONFIG_SCREEN_SIZE_WIDTH ?= 320
F_CONFIG_SCREEN_SIZE_WIDTH_HW ?= 0
F_CONFIG_SCREEN_SIZE_ZOOM ?= 1
F_CONFIG_SCREEN_VSYNC ?= 0

ifneq ($(F_CONFIG_SCREEN_SIZE_ZOOM), 1)
    F_CONFIG_SYSTEM_WIZ_SCREEN_FIX := 0
endif

#
# Sound
#
F_CONFIG_SOUND_FORMAT ?= F_SOUND_FORMAT_DYNAMIC
F_CONFIG_SOUND_MUTE ?= 0
F_CONFIG_SOUND_SAMPLE_CHANNELS_RESERVED ?= 32
F_CONFIG_SOUND_SAMPLE_CHANNELS_TOTAL ?= 64
F_CONFIG_SOUND_SAMPLE_NEEDS_RUNTIME_OBJECT ?= 1
F_CONFIG_SOUND_VOLUME_ADJUSTABLE ?= 0
F_CONFIG_SOUND_VOLUME_BAR ?= 0
F_CONFIG_SOUND_VOLUME_SCALE_MUSIC ?= 100
F_CONFIG_SOUND_VOLUME_SCALE_SAMPLE ?= 100

#
# Libraries
#
F_CONFIG_LIB_PNG ?= 0
F_CONFIG_LIB_SDL ?= 0
F_CONFIG_LIB_SDL_CONFIG ?=
F_CONFIG_LIB_SDL_CURSOR ?= 1
F_CONFIG_LIB_SDL_GAMEPADMAP ?= gamecontrollerdb.txt
F_CONFIG_LIB_SDL_MIXER_CHUNK_SIZE ?= 256
F_CONFIG_LIB_SDL_MIXER_LIMITED_SUPPORT ?= 0
F_CONFIG_LIB_SDL_TIME ?= 1
F_CONFIG_LIB_STDLIB_FILES ?= 1
F_CONFIG_LIB_STDLIB_MEMORY ?= 1

#
# Build-time info
#
F_BUILD_CONFIG_GIT := $(shell cd $(FAUR_PATH) && git rev-parse --verify HEAD)
F_BUILD_CONFIG_TIME := $(shell date "+%Y-%m-%d~%H:%M:%S")

#
# Config values made available to code
#
F_CONFIG_BUILD_FLAGS_SETTINGS := \
    -DF_CONFIG_APP_AUTHOR=\"$(call F_MAKE_SPACE_ESCAPE,$(F_CONFIG_APP_AUTHOR))\" \
    -DF_CONFIG_APP_NAME=\"$(call F_MAKE_SPACE_ESCAPE,$(F_CONFIG_APP_NAME))\" \
    -DF_CONFIG_APP_VERSION_MAJOR=$(F_CONFIG_APP_VERSION_MAJOR) \
    -DF_CONFIG_APP_VERSION_MICRO=$(F_CONFIG_APP_VERSION_MICRO) \
    -DF_CONFIG_APP_VERSION_MINOR=$(F_CONFIG_APP_VERSION_MINOR) \
    -DF_CONFIG_BUILD_ATTRIBUTE_FORMAT_PRINTF=$(F_CONFIG_BUILD_ATTRIBUTE_FORMAT_PRINTF) \
    -DF_CONFIG_BUILD_FAUR_GIT=\"$(F_BUILD_CONFIG_GIT)\" \
    -DF_CONFIG_BUILD_FAUR_TIME=\"$(call F_MAKE_SPACE_ESCAPE,$(F_BUILD_CONFIG_TIME))\" \
    -DF_CONFIG_BUILD_GEN_LUTS=$(F_CONFIG_BUILD_GEN_LUTS) \
    -DF_CONFIG_BUILD_UID=\"$(F_CONFIG_BUILD_UID)\" \
    -DF_CONFIG_COLOR_SPRITE_BORDER=$(F_CONFIG_COLOR_SPRITE_BORDER) \
    -DF_CONFIG_COLOR_SPRITE_KEY=$(F_CONFIG_COLOR_SPRITE_KEY) \
    -DF_CONFIG_COLOR_SCREEN_BORDER=$(F_CONFIG_COLOR_SCREEN_BORDER) \
    -DF_CONFIG_DEBUG=$(F_CONFIG_DEBUG) \
    -DF_CONFIG_DEBUG_FATAL=$(F_CONFIG_DEBUG_FATAL) \
    -DF_CONFIG_DEBUG_MEM_POOL=$(F_CONFIG_DEBUG_MEM_POOL) \
    -DF_CONFIG_DIR_SCREENSHOTS=\"$(F_CONFIG_DIR_SCREENSHOTS)\" \
    -DF_CONFIG_FILES_EMBED_BLOB=\"$(F_CONFIG_FILES_EMBED_BLOB)\" \
    -DF_CONFIG_FILES_PREFIX=\"$(F_CONFIG_FILES_PREFIX)\" \
    -DF_CONFIG_FPS_CAP=$(F_CONFIG_FPS_CAP) \
    -DF_CONFIG_FPS_RATE_DRAW=$(F_CONFIG_FPS_RATE_DRAW) \
    -DF_CONFIG_FPS_RATE_TICK=$(F_CONFIG_FPS_RATE_TICK) \
    -DF_CONFIG_LIB_PNG=$(F_CONFIG_LIB_PNG) \
    -DF_CONFIG_LIB_SDL=$(F_CONFIG_LIB_SDL) \
    -DF_CONFIG_LIB_SDL_CURSOR=$(F_CONFIG_LIB_SDL_CURSOR) \
    -DF_CONFIG_LIB_SDL_GAMEPADMAP=\"$(F_CONFIG_LIB_SDL_GAMEPADMAP)\" \
    -DF_CONFIG_LIB_SDL_MIXER_CHUNK_SIZE=$(F_CONFIG_LIB_SDL_MIXER_CHUNK_SIZE) \
    -DF_CONFIG_LIB_SDL_MIXER_LIMITED_SUPPORT=$(F_CONFIG_LIB_SDL_MIXER_LIMITED_SUPPORT) \
    -DF_CONFIG_LIB_SDL_TIME=$(F_CONFIG_LIB_SDL_TIME) \
    -DF_CONFIG_LIB_STDLIB_FILES=$(F_CONFIG_LIB_STDLIB_FILES) \
    -DF_CONFIG_LIB_STDLIB_MEMORY=$(F_CONFIG_LIB_STDLIB_MEMORY) \
    -DF_CONFIG_OUT_COLOR_TEXT=$(F_CONFIG_OUT_COLOR_TEXT) \
    -DF_CONFIG_OUT_STDERR=$(F_CONFIG_OUT_STDERR) \
    -DF_CONFIG_OUT_STDOUT=$(F_CONFIG_OUT_STDOUT) \
    -DF_CONFIG_SCREEN_FORMAT=$(F_CONFIG_SCREEN_FORMAT) \
    -DF_CONFIG_SCREEN_FULLSCREEN=$(F_CONFIG_SCREEN_FULLSCREEN) \
    -DF_CONFIG_SCREEN_MAXIMIZED=$(F_CONFIG_SCREEN_MAXIMIZED) \
    -DF_CONFIG_SCREEN_RENDER=$(F_CONFIG_SCREEN_RENDER) \
    -DF_CONFIG_SCREEN_SIZE_HEIGHT=$(F_CONFIG_SCREEN_SIZE_HEIGHT) \
    -DF_CONFIG_SCREEN_SIZE_HEIGHT_HW=$(F_CONFIG_SCREEN_SIZE_HEIGHT_HW) \
    -DF_CONFIG_SCREEN_SIZE_WIDTH=$(F_CONFIG_SCREEN_SIZE_WIDTH) \
    -DF_CONFIG_SCREEN_SIZE_WIDTH_HW=$(F_CONFIG_SCREEN_SIZE_WIDTH_HW) \
    -DF_CONFIG_SCREEN_SIZE_ZOOM=$(F_CONFIG_SCREEN_SIZE_ZOOM) \
    -DF_CONFIG_SCREEN_VSYNC=$(F_CONFIG_SCREEN_VSYNC) \
    -DF_CONFIG_SOUND_FORMAT=$(F_CONFIG_SOUND_FORMAT) \
    -DF_CONFIG_SOUND_MUTE=$(F_CONFIG_SOUND_MUTE) \
    -DF_CONFIG_SOUND_SAMPLE_CHANNELS_RESERVED=$(F_CONFIG_SOUND_SAMPLE_CHANNELS_RESERVED) \
    -DF_CONFIG_SOUND_SAMPLE_CHANNELS_TOTAL=$(F_CONFIG_SOUND_SAMPLE_CHANNELS_TOTAL) \
    -DF_CONFIG_SOUND_SAMPLE_NEEDS_RUNTIME_OBJECT=$(F_CONFIG_SOUND_SAMPLE_NEEDS_RUNTIME_OBJECT) \
    -DF_CONFIG_SOUND_VOLUME_ADJUSTABLE=$(F_CONFIG_SOUND_VOLUME_ADJUSTABLE) \
    -DF_CONFIG_SOUND_VOLUME_BAR=$(F_CONFIG_SOUND_VOLUME_BAR) \
    -DF_CONFIG_SOUND_VOLUME_SCALE_MUSIC=$(F_CONFIG_SOUND_VOLUME_SCALE_MUSIC) \
    -DF_CONFIG_SOUND_VOLUME_SCALE_SAMPLE=$(F_CONFIG_SOUND_VOLUME_SCALE_SAMPLE) \
    -DF_CONFIG_SYSTEM_ARDUINO=$(F_CONFIG_SYSTEM_ARDUINO) \
    -DF_CONFIG_SYSTEM_CAANOO=$(F_CONFIG_SYSTEM_CAANOO) \
    -DF_CONFIG_SYSTEM_EMSCRIPTEN=$(F_CONFIG_SYSTEM_EMSCRIPTEN) \
    -DF_CONFIG_SYSTEM_GAMEBUINO=$(F_CONFIG_SYSTEM_GAMEBUINO) \
    -DF_CONFIG_SYSTEM_GP2X=$(F_CONFIG_SYSTEM_GP2X) \
    -DF_CONFIG_SYSTEM_GP2X_MENU=$(F_CONFIG_SYSTEM_GP2X_MENU) \
    -DF_CONFIG_SYSTEM_GP2X_MHZ=$(F_CONFIG_SYSTEM_GP2X_MHZ) \
    -DF_CONFIG_SYSTEM_LINUX=$(F_CONFIG_SYSTEM_LINUX) \
    -DF_CONFIG_SYSTEM_MINGW64=$(F_CONFIG_SYSTEM_MINGW64) \
    -DF_CONFIG_SYSTEM_ODROID_GO=$(F_CONFIG_SYSTEM_ODROID_GO) \
    -DF_CONFIG_SYSTEM_PANDORA=$(F_CONFIG_SYSTEM_PANDORA) \
    -DF_CONFIG_SYSTEM_WIZ=$(F_CONFIG_SYSTEM_WIZ) \
    -DF_CONFIG_SYSTEM_WIZ_SCREEN_FIX=$(F_CONFIG_SYSTEM_WIZ_SCREEN_FIX) \
    -DF_CONFIG_TRAIT_CONSOLE=$(F_CONFIG_TRAIT_CONSOLE) \
    -DF_CONFIG_TRAIT_CONSOLE_TOGGLE=$(F_CONFIG_TRAIT_CONSOLE_TOGGLE) \
    -DF_CONFIG_TRAIT_CUSTOM_MAIN=$(F_CONFIG_TRAIT_CUSTOM_MAIN) \
    -DF_CONFIG_TRAIT_DESKTOP=$(F_CONFIG_TRAIT_DESKTOP) \
    -DF_CONFIG_TRAIT_KEYBOARD=$(F_CONFIG_TRAIT_KEYBOARD) \
    -DF_CONFIG_TRAIT_LOW_MEM=$(F_CONFIG_TRAIT_LOW_MEM) \
    -DF_CONFIG_TRAIT_NO_SEEDING=$(F_CONFIG_TRAIT_NO_SEEDING) \
    -DF_CONFIG_TRAIT_SCREENSHOTS=$(F_CONFIG_TRAIT_SCREENSHOTS) \
    -DF_CONFIG_TRAIT_SLOW_MUL=$(F_CONFIG_TRAIT_SLOW_MUL) \
