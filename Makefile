# 1. Tools
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# 2. Paths
TARGET  = firmware
BUILD_DIR = build
LDSCRIPT = linker/linker.ld  # Path to your linker script

# 3. CPU & Flags
MCU     = -mcpu=cortex-m4 -mthumb
INC     = -Idrivers/inc
# Added -T to specify the linker script and --specs=nosys.specs for bare metal
LDFLAGS = $(MCU) -T$(LDSCRIPT) -nostdlib

# 4. Source Files
SRCS = src/main.c \
       drivers/src/gpio_hal.c \
       startup/startup.c  

# 5. Build Rules
all: $(BUILD_DIR)/$(TARGET).bin

# Rule to create the build directory
$(BUILD_DIR):
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

# Prefix the output with $(BUILD_DIR)/
# The | $(BUILD_DIR) tells Make to ensure the folder exists first
$(BUILD_DIR)/$(TARGET).elf: $(SRCS) | $(BUILD_DIR)
	$(CC) $(MCU) $(INC) $(SRCS) $(LDFLAGS) -o $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rmdir /s /q $(BUILD_DIR)