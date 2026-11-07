####################
# COSMOS GBA build
####################
PROGRAM    := cosmos
TITLE      := COSMOS
GAME_CODE  := CSMS
MAKER_CODE := 00
REVISION   := 0

INC_DIR   := include
SRC_DIR   := src
BUILD_DIR := build

CFLAGS   := -O2 -Wall -Wextra -Werror -I$(INC_DIR) -mcpu=arm7tdmi -mthumb -mthumb-interwork
# libgba ships legacy headers that use the removed C++ register keyword.
# Keep -Werror for project code while not treating that third-party warning as fatal.
CXXFLAGS := -std=gnu++17 -O2 -Wall -Wextra -Werror -Wno-error=register -I$(INC_DIR) -mcpu=arm7tdmi -mthumb -mthumb-interwork -fno-exceptions -fno-rtti
LDFLAGS  := -Wl,-Map=$(PROGRAM).map --specs=nosys.specs

LIBGBA          := libgba
LIBGBA_INC      := $(LIBGBA)/include
LIBGBA_FLAGS    := -I$(LIBGBA_INC)
LIBGBA_LDFLAGS  := -L$(LIBGBA)/lib -lgba -nostartfiles $(LIBGBA)/gba_crt0.o -Wl,-T,$(LIBGBA)/gba_cart.ld
CFLAGS   += $(LIBGBA_FLAGS)
CXXFLAGS += $(LIBGBA_FLAGS)

C_BUILD_DIR   := $(BUILD_DIR)/c
CXX_BUILD_DIR := $(BUILD_DIR)/cxx
ASM_BUILD_DIR := $(BUILD_DIR)/asm

C_SRCS   := $(shell find $(SRC_DIR)/ -type f -name '*.c')
CXX_SRCS := $(shell find $(SRC_DIR)/ -type f -name '*.cpp')
ASM_SRCS := $(shell find $(SRC_DIR)/ -type f -name '*.s')

C_OBJS   := $(patsubst %.c,$(C_BUILD_DIR)/%.o,$(C_SRCS))
CXX_OBJS := $(patsubst %.cpp,$(CXX_BUILD_DIR)/%.o,$(CXX_SRCS))
ASM_OBJS := $(patsubst %.s,$(ASM_BUILD_DIR)/%.o,$(ASM_SRCS))
OBJS     := $(C_OBJS) $(CXX_OBJS) $(ASM_OBJS)

PREF    := arm-none-eabi-
AS      := $(PREF)as
CC      := $(PREF)gcc
CXX     := $(PREF)g++
LD      := $(CXX)
OBJCOPY := $(PREF)objcopy
HOST_CXX ?= c++

CORE_SOURCES := src/core/portal.cpp src/core/world_clock.cpp src/core/ecology.cpp
HOST_TEST    := $(BUILD_DIR)/host/core_tests
ECOLOGY_TEST := $(BUILD_DIR)/host/ecology_tests

.PHONY: all clean test
all: $(PROGRAM).gba

clean:
	rm -rf $(PROGRAM).gba $(PROGRAM).elf $(PROGRAM).map $(BUILD_DIR)

$(PROGRAM).gba: $(PROGRAM).elf
	$(OBJCOPY) -O binary $< $@
	gbafix $@ -t"$(TITLE)" -c$(GAME_CODE) -m$(MAKER_CODE) -r$(REVISION) -p

$(PROGRAM).elf: $(OBJS)
	$(LD) -o $@ $^ $(CXXFLAGS) $(LIBGBA_LDFLAGS) $(LDFLAGS)

$(C_BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

$(CXX_BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(ASM_BUILD_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) -o $@ $<

$(HOST_TEST): $(CORE_SOURCES) tests/core_tests.cpp include/cosmos/isometric.hpp include/cosmos/portal.hpp include/cosmos/world_clock.hpp
	@mkdir -p $(dir $@)
	$(HOST_CXX) -std=c++17 -O2 -Wall -Wextra -Werror -pedantic -I$(INC_DIR) $(CORE_SOURCES) tests/core_tests.cpp -o $@

$(ECOLOGY_TEST): src/core/ecology.cpp tests/ecology_tests.cpp include/cosmos/ecology.hpp
	@mkdir -p $(dir $@)
	$(HOST_CXX) -std=c++17 -O2 -Wall -Wextra -Werror -pedantic -I$(INC_DIR) src/core/ecology.cpp tests/ecology_tests.cpp -o $@

test: $(HOST_TEST) $(ECOLOGY_TEST)
	./$(HOST_TEST)
	./$(ECOLOGY_TEST)
