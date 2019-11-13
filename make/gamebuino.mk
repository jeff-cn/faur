include $(FAUR_PATH)/make/global/defs.mk

F_CONFIG_BUILD_DEBUG_FATAL_SPIN := 1
F_CONFIG_BUILD_MAIN := 0
F_CONFIG_BUILD_OPT := s
F_CONFIG_FEATURE_CONSOLE := 0
F_CONFIG_FEATURE_ECS := 0
F_CONFIG_FEATURE_EMBED := 0
F_CONFIG_FEATURE_MUSIC := 0
F_CONFIG_FEATURE_OUTPUT := 1
F_CONFIG_FEATURE_RANDOM_SEED := 0
F_CONFIG_FEATURE_SOUND := 1
F_CONFIG_FILES_LIB := GAMEBUINO
F_CONFIG_FIX_LUT := 1
F_CONFIG_FPS_HISTORY := 1
F_CONFIG_SCREEN_FORMAT := F_COLOR_FORMAT_RGB_565
F_CONFIG_SCREEN_HARDWARE_WIDTH := 80
F_CONFIG_SCREEN_HARDWARE_HEIGHT := 64
F_CONFIG_SOUND_VOLUME_ADJUSTABLE := 0
F_CONFIG_SYSTEM_GAMEBUINO := 1

include $(FAUR_PATH)/make/global/config.mk

ifndef F_DO_BUILD

F_MAKE_CMD := \
    $(MAKE) \
	-C $(F_DIR_ROOT)/$(F_CONFIG_DIR_SRC) \
	-f $(realpath $(firstword $(MAKEFILE_LIST))) \
	-j$(F_MAKE_PARALLEL_JOBS) \
	F_DIR_ROOT_ABS=$(F_DIR_ROOT_ABS) \
	F_DO_BUILD=1

all :
	bash -c "$(F_MAKE_CMD)"

% :
	bash -c "$(F_MAKE_CMD) $@"

else

ARDUINO_DIR = $(F_SDK_ARDUINO_DIR)
ARDMK_DIR = $(F_SDK_ARDUINO_MAKEFILE)

BOARD_TAG = gamebuino_meta_native
ALTERNATE_CORE_PATH = $(wildcard $(HOME)/.arduino15/packages/gamebuino/hardware/samd/1.*)

ARDUINO_LIBS = faur Gamebuino_META SPI
OPTIMIZATION_LEVEL = $(F_CONFIG_BUILD_OPT)
OBJDIR=$(F_DIR_ROOT_ABS)/$(F_CONFIG_DIR_BUILD)/builds/$(F_CONFIG_BUILD_UID)

include $(ARDMK_DIR)/Sam.mk

F_DIR_GEN := $(OBJDIR)/userlibs/faur/faur_gen

# Gamebuino_META needs __SKETCH_NAME__ (usually defined by Arduino IDE)
F_CONFIG_BUILD_FLAGS_SHARED += \
    -I$(F_DIR_GEN) \
    -D__SKETCH_NAME__=\"$(TARGET).ino\" \

CPPFLAGS := $(F_CONFIG_BUILD_FLAGS_SHARED) $(CPPFLAGS)

# Libraries don't build well with Link Time Optimization
CFLAGS += -fno-lto
CXXFLAGS += -fno-lto
LDFLAGS += -fno-lto

F_FILES_CONFIG := \
    $(F_DIR_GEN)/sketch/config-faur.h \
    $(F_DIR_GEN)/sketch/config-gamebuino.h \

$(USER_LIB_OBJS) $(LOCAL_OBJS) : $(F_FILES_CONFIG)

$(F_DIR_GEN)/sketch/%.h : $(F_DIR_ROOT_ABS)/$(F_CONFIG_DIR_SRC)/%.h
	@ mkdir -p $(@D)
	cp $< $@

endif
