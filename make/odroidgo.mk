include $(FAUR_PATH)/make/global/defs-first.mk
include $(FAUR_PATH)/make/global/defs-sdk.mk

F_CONFIG_APP_NAME_SUFFIX := .bin
F_CONFIG_DEBUG_FATAL := F_DEBUG_FATAL_SPIN
F_CONFIG_FILES_STORAGE_PREFIX := "/"
F_CONFIG_LIB_STDLIB_FILES := 0
F_CONFIG_LIB_STDLIB_MEMORY := 0
F_CONFIG_SCREEN_FORMAT := F_COLOR_FORMAT_RGB_565_B
F_CONFIG_SCREEN_FULLSCREEN := 1
F_CONFIG_SCREEN_RENDER := F_SCREEN_RENDER_SOFTWARE
F_CONFIG_SCREEN_SIZE_HEIGHT_HW := 240
F_CONFIG_SCREEN_SIZE_WIDTH_HW := 320
F_CONFIG_SYSTEM_ARDUINO := 1
F_CONFIG_SYSTEM_ARDUINO_BOARD ?= esp32:esp32:odroid_esp32:FlashMode=qio,FlashFreq=80,PartitionScheme=default,UploadSpeed=921600,DebugLevel=none
F_CONFIG_SYSTEM_ARDUINO_PORT ?= /dev/ttyUSB0
F_CONFIG_SYSTEM_ODROID_GO := 1
F_CONFIG_TRAIT_CONSOLE_TOGGLE := F_BUTTON_A,F_BUTTON_B,F_BUTTON_START,F_BUTTON_SELECT
F_CONFIG_TRAIT_CUSTOM_MAIN := 1
F_CONFIG_TRAIT_SCREENSHOTS := 0

include $(FAUR_PATH)/make/global/defs-config.mk
include $(FAUR_PATH)/make/global/rules.mk

f__target_run :
	$(F_SDK_ODROID_GO_ESPTOOL)/esptool.py \
		--chip esp32 \
		--port $(F_CONFIG_SYSTEM_ARDUINO_PORT) \
		--baud 921600 \
		--before default_reset \
		--after hard_reset \
		write_flash \
		-z \
		--flash_mode dio \
		--flash_freq 80m \
		--flash_size detect \
		0xe000 $(F_SDK_ODROID_GO_HARDWARE_TOOLS)/partitions/boot_app0.bin \
		0x1000 $(F_SDK_ODROID_GO_HARDWARE_TOOLS)/sdk/bin/bootloader_qio_80m.bin \
		0x10000 $(F_BUILD_DIR_ARDUINO_BUILD)/$(notdir $(F_BUILD_FILE_INO)).bin \
		0x8000 $(F_BUILD_DIR_ARDUINO_BUILD)/$(notdir $(F_BUILD_FILE_INO)).partitions.bin
