#  program name
	TARGET	= co2meter

# compile options
	MCU			= cortex-m3
	OPTIMIZE	= -O3
	USE_LTO		= YES
	CHIP		= STM32F103xB

#defines
	DEFS		= -D$(CHIP)
	DEFS		+= -DHL_STM32F1XX

# source directories (all *.c, *.cpp and *.S files included)

	DIRS	:= src
	DIRS	+= src/cmsis/Source
	DIRS	+= src/muil

	INCLUDE	:= src
	INCLUDE	+= src/cmsis/Include
	INCLUDE	+= src/stm32_lib
	INCLUDE	+= src/muil

# linker script (chip dependent)
	LD_SCRIPT	= src/CMSIS/Linker/$(CHIP)_FLASH.ld

###########################################################
	TOOL	= arm-none-eabi-

	BASE		= .
	CC			= $(TOOL)gcc
	CXX			= $(TOOL)g++
	LD			= $(TOOL)g++
	AS			= $(CC) -x assembler-with-cpp
	OBJCOPY		= $(TOOL)objcopy
	OBJDUMP		= $(TOOL)objdump
	SIZE		= $(TOOL)size -d
	RM			= rm -f
	CP			= cp
	MD			= mkdir

#  dirs
	OBJDIR		= $(BASE)/.obj
	DSTDIR		= $(BASE)/.dst

#files
	HEX		= $(DSTDIR)/$(TARGET).hex
	BIN		= $(DSTDIR)/$(TARGET).bin
	ELF		= $(DSTDIR)/$(TARGET).elf
	MAP		= $(DSTDIR)/$(TARGET).map
	LSS		= $(DSTDIR)/$(TARGET).lss
	OK		= $(DSTDIR)/$(TARGET).ok

# includes
	INCS	:= $(patsubst %, -I "%", $(INCLUDE))

#calc obj files list
	OBJS	:= $(wildcard $(addsuffix /*.cpp, $(DIRS)))
	OBJS	+= $(wildcard $(addsuffix /*.c, $(DIRS)))
	OBJS	+= $(wildcard $(addsuffix /startup_$(CHIP).S, $(DIRS)))
	OBJS	:= $(notdir $(OBJS))
	OBJS	:= $(OBJS:.cpp=.o)
	OBJS	:= $(OBJS:.c=.o)
	OBJS	:= $(OBJS:.S=.o)
	OBJS	:= $(patsubst %, $(OBJDIR)/%, $(OBJS))

# flags
	FLAGS	= -mcpu=$(MCU) -mthumb
	FLAGS	+= $(INCS)
	FLAGS	+= -MD
	FLAGS	+= $(DEFS)
	FLAGS	+= -Wa,-adhlns=$(addprefix $(DSTDIR)/, $(notdir $(addsuffix .lst, $(basename $<))))

	AFLAGS	= $(FLAGS)

	CFLAGS	= $(FLAGS)
	CFLAGS	+= $(OPTIMIZE)
	CFLAGS	+= -std=c99
	CFLAGS	+= -g
	CFLAGS	+= -ffunction-sections -fdata-sections
	CFLAGS  += -finput-charset=utf-8

	CXXFLAGS	= $(FLAGS)
	CXXFLAGS	+= $(OPTIMIZE)
	CXXFLAGS	+= -g
	CXXFLAGS	+= -fno-exceptions -fno-rtti -fno-unwind-tables
	CXXFLAGS	+= -ffunction-sections -fdata-sections
	CXXFLAGS	+= -fno-threadsafe-statics
	CXXFLAGS	+= -funsigned-bitfields
	CXXFLAGS	+= -std=c++17
	CXXFLAGS	+= -finput-charset=utf-8

	LD_FLAGS	= -mcpu=$(MCU)
	LD_FLAGS	+= -mthumb
	LD_FLAGS	+= $(OPTIMIZE)
	LD_FLAGS	+= -Xlinker --gc-sections
	LD_FLAGS	+= -Wl,-Map="$(MAP)",--cref
	LD_FLAGS	+= -ffunction-sections -fdata-sections
	LD_FLAGS	+= -T$(LD_SCRIPT)
	LD_FLAGS	+= --specs=nano.specs


ifeq ($(USE_LTO),YES)
	CFLAGS		+= -flto
	CXXFLAGS	+= -flto
	LD_FLAGS	+= -flto
endif

.SILENT :

.PHONY: all start dirs build clean program reset archive

############# targets

all : start dirs $(ELF) $(BIN) $(LSS) $(OK)

build:
	$(MAKE) clean
	$(MAKE)

start:
	@echo --- building $(TARGET)
	@echo OBJS=$(OBJS)

$(LSS): $(ELF) makefile
	@echo --- making asm-lst...
#	@$(OBJDUMP) -dStC $(ELF) > $(LSS)
	@$(OBJDUMP) -dC $(ELF) > $(LSS)

$(OK): $(ELF)
	@$(SIZE) $(ELF)
	@echo "Errors: none"

$(ELF):	$(OBJS) makefile
	@echo --- linking...
	$(LD) $(OBJS) $(LIBS) $(LD_FLAGS) -o "$(ELF)"

$(HEX): $(ELF)
	@echo --- make hex...
	@$(OBJCOPY) -O ihex $(ELF) $(HEX)

$(BIN): $(ELF)
	@echo --- make binary...
	@$(OBJCOPY) -O binary $(ELF) $(BIN)

VPATH := $(DIRS)

$(OBJDIR)/%.o: %.cpp makefile
	@echo --- compiling $<...
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(OBJDIR)/%.o: %.c makefile
	@echo --- compiling $<...
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/%.o: %.S makefile
	@echo --- assembling $<...
	$(AS) -c $(AFLAGS) -o $@ $<

dirs: $(OBJDIR) $(DSTDIR)

$(OBJDIR):
	-@$(MD) $(OBJDIR)

$(DSTDIR):
	-@$(MD) $(DSTDIR)

clean:
	-@$(RM) $(OBJDIR)/*.d 2>/dev/null
	-@$(RM) $(OBJDIR)/*.o 2>/dev/null
	-@$(RM) $(DSTDIR)/*.lst 2>/dev/null
	-@$(RM) $(ELF)
	-@$(RM) $(HEX)
	-@$(RM) $(LSS)
	-@$(RM) $(MAP)


# dependencies
ifeq (,$(findstring build,$(MAKECMDGOALS)))
 ifeq (,$(findstring clean,$(MAKECMDGOALS)))
  ifeq (,$(findstring dirs,$(MAKECMDGOALS)))
  -include $(wildcard $(OBJDIR)/*.d)
  endif
 endif
endif
