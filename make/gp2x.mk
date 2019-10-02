include $(A2X_PATH)/make/global/defs.mk
include $(A_FILE_SDKCONFIG_MK)

A_CONFIG_BUILD_OPT := -O3 -s
A_CONFIG_INPUT_MOUSE_CURSOR := 0
A_CONFIG_LIB_SDL := 1
A_CONFIG_LIB_SDL_TIME := 1
A_CONFIG_SCREEN_FULLSCREEN := 1
A_CONFIG_SCREEN_HARDWARE_WIDTH := 320
A_CONFIG_SCREEN_HARDWARE_HEIGHT := 240
A_CONFIG_SYSTEM_GP2X := 1
A_CONFIG_SYSTEM_LINUX := 1

OPEN2X := $(A_SDK_OPEN2X_ROOT)/$(A_SDK_OPEN2X_TOOLCHAIN)

A_CONFIG_APP_NAME_SUFFIX := .gpe

A_CONFIG_BUILD_LIBS += \
    -static \
    -L$(OPEN2X)/lib \
    -L$(OPEN2X)/arm-open2x-linux/lib \
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
    -ldl \

A_CONFIG_BUILD_CFLAGS += \
    -mcpu=arm920t \
    -mtune=arm920t \
    -msoft-float \
    -ffast-math \
    -fomit-frame-pointer \
    -isystem$(OPEN2X)/include \
    -isystem$(OPEN2X)/arm-open2x-linux/include \
    -Wno-conversion \

PREFIX := arm-open2x-linux-

export PATH    := $(OPEN2X)/bin:$(PATH)
export CC      := $(PREFIX)gcc
export CXX     := $(PREFIX)g++
export AS      := $(PREFIX)as
export AR      := $(PREFIX)ar
export OBJCOPY := $(PREFIX)objcopy
export READELF := $(PREFIX)readelf
export STRIP   := $(PREFIX)strip

include $(A2X_PATH)/make/global/rules.mk
