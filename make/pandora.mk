include $(FAUR_PATH)/make/global/defs-first.mk
include $(FAUR_PATH)/make/global/defs-sdk.mk

F_CONFIG_BUILD_OPT := 3
F_CONFIG_LIB_PNG := 1
F_CONFIG_LIB_SDL := 1
F_CONFIG_LIB_SDL_CURSOR := 0
F_CONFIG_SCREEN_FORMAT := F_COLOR_FORMAT_RGB_565
F_CONFIG_SCREEN_FULLSCREEN := 1
F_CONFIG_SCREEN_RENDER := F_SCREEN_RENDER_SOFTWARE
F_CONFIG_SCREEN_SIZE_HEIGHT_HW := 240
F_CONFIG_SCREEN_SIZE_WIDTH_HW := 400
F_CONFIG_SYSTEM_LINUX := 1
F_CONFIG_SYSTEM_PANDORA := 1
F_CONFIG_TRAIT_KEYBOARD := 1

F_CONFIG_BUILD_LIBS += \
    -L$(F_SDK_PANDORA_TOOLCHAIN)/lib \
    -L$(F_SDK_PANDORA_TOOLCHAIN)/usr/lib \
    -L$(F_SDK_PANDORA_TOOLCHAIN)/arm-none-linux-gnueabi/lib \
    -lpng12 \
    -lz \
    -lSDL_mixer \
    -lmad \
    -lSDL \
    -lts \
    -lpthread \
    -lm \
    -ldl

F_CONFIG_BUILD_FLAGS_SHARED_C_AND_CPP += \
    -march=armv7-a \
    -mcpu=cortex-a8 \
    -mtune=cortex-a8 \
    -mfpu=neon \
    -mfloat-abi=softfp \
    -ffast-math \
    -fsingle-precision-constant \
    -ftree-vectorize \
    -fomit-frame-pointer \
    -isystem$(F_SDK_PANDORA_TOOLCHAIN)/include \
    -isystem$(F_SDK_PANDORA_TOOLCHAIN)/usr/include \
    -isystem$(F_SDK_PANDORA_TOOLCHAIN)/arm-none-linux-gnueabi/include

F_CONFIG_BUILD_TOOL_CC := $(F_SDK_PANDORA_TOOLCHAIN)/bin/arm-none-linux-gnueabi-gcc
F_CONFIG_BUILD_TOOL_CPP := $(F_SDK_PANDORA_TOOLCHAIN)/bin/arm-none-linux-gnueabi-g++

include $(FAUR_PATH)/make/global/defs-config.mk
include $(FAUR_PATH)/make/global/rules.mk

F_PND_DIR_BASE := $(F_DIR_ROOT_FROM_MAKE)/$(F_CONFIG_DIR_BUILD)/static/pnd
F_PND_DIR_STAGE := $(F_BUILD_DIR)/pnd
F_PND_FILE := $(F_CONFIG_APP_AUTHOR).$(F_CONFIG_APP_NAME).pnd
F_PND_FILE_PATH := $(PWD)/$(F_BUILD_DIR_BIN)/$(F_PND_FILE)

ifeq ($(shell test -d $(F_PND_DIR_BASE) ; echo $$?), 0)
    f__target_post : $(F_PND_FILE_PATH)
endif

$(F_PND_FILE_PATH) : $(F_BUILD_DIR_BIN)/$(F_BUILD_FILE_BIN)
	@ mkdir -p $(@D) $(F_PND_DIR_STAGE)
	rsync --archive --delete --progress --human-readable $(F_PND_DIR_BASE)/ $(F_PND_DIR_STAGE)
	rsync --archive --delete --progress --human-readable $(F_DIR_ROOT_FROM_MAKE)/$(F_CONFIG_DIR_MEDIA) $(F_PND_DIR_STAGE)
	cp $(F_BUILD_DIR_BIN)/$(F_BUILD_FILE_BIN) $(F_PND_DIR_STAGE)
	$(F_SDK_PANDORA_UTILS)/pnd_make.sh -c \
		-d $(PWD)/$(F_PND_DIR_STAGE) \
		-p $(PWD)/$(F_BUILD_DIR_BIN)/$(F_PND_FILE) \
		-x $(PWD)/$(F_PND_DIR_STAGE)/doc/PXML.xml \
		-i $(PWD)/$(F_PND_DIR_STAGE)/img/icon.png
