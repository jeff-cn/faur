include $(FAUR_PATH)/make/global/defs-first.mk
include $(FAUR_PATH)/make/global/defs-sdk.mk

F_CONFIG_APP_NAME_SUFFIX := .gpe
F_CONFIG_BUILD_FLAGS_CPP_PEDANTIC := 0
F_CONFIG_BUILD_FLAGS_CPP_STANDARD := c++98
F_CONFIG_BUILD_OPT := 3
F_CONFIG_LIB_PNG := 1
F_CONFIG_LIB_SDL := 1
F_CONFIG_LIB_SDL_CURSOR := 0
F_CONFIG_SCREEN_FORMAT := F_COLOR_FORMAT_RGB_565
F_CONFIG_SCREEN_FULLSCREEN := 1
F_CONFIG_SCREEN_RENDER := F_SCREEN_RENDER_SOFTWARE
F_CONFIG_SCREEN_SIZE_HEIGHT_HW := 240
F_CONFIG_SCREEN_SIZE_WIDTH_HW := 320
F_CONFIG_SOUND_VOLUME_ADJUSTABLE := 1
F_CONFIG_SOUND_VOLUME_BAR := 1
F_CONFIG_SYSTEM_GP2X := 1
F_CONFIG_SYSTEM_LINUX := 1
F_CONFIG_TRAIT_SLOW_MUL := 1

F_CONFIG_BUILD_LIBS += \
    -static \
    -L$(F_SDK_OPEN2X_TOOLCHAIN)/lib \
    -L$(F_SDK_OPEN2X_TOOLCHAIN)/arm-open2x-linux/lib \
    -lpng12 \
    -lz \
    -lSDL_mixer \
    -lsmpeg \
    -lmikmod \
    -logg \
    -lvorbisidec \
    -lSDL \
    -lpthread \
    -lm \
    -lstdc++ \
    -ldl

F_CONFIG_BUILD_FLAGS_SHARED_C_AND_CPP += \
    -mcpu=arm920t \
    -mtune=arm920t \
    -msoft-float \
    -ffast-math \
    -fomit-frame-pointer \
    -isystem$(F_SDK_OPEN2X_TOOLCHAIN)/include \
    -isystem$(F_SDK_OPEN2X_TOOLCHAIN)/arm-open2x-linux/include

F_CONFIG_BUILD_FLAGS_SHARED_C_AND_CPP_OVERRIDE += \
    -Wno-conversion

F_CONFIG_BUILD_TOOL_CC := $(F_SDK_OPEN2X_TOOLCHAIN)/bin/arm-open2x-linux-gcc
F_CONFIG_BUILD_TOOL_CPP := $(F_SDK_OPEN2X_TOOLCHAIN)/bin/arm-open2x-linux-g++

include $(FAUR_PATH)/make/global/defs-config.mk
include $(FAUR_PATH)/make/global/rules.mk
