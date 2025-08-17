EE_DIR_BIN  = bin/
EE_DIR_SRC  = src/
EE_DIR_OBJS = obj/

EE_BIN = $(EE_DIR_BIN)test.elf
#SLES_529
##EE_BIN = $(EE_DIR_BIN)SLES_529.89

# Recoger archivos C y C++
EE_CPP_SRC = $(wildcard $(EE_DIR_SRC)*.cpp)
EE_C_SRC = $(wildcard $(EE_DIR_SRC)*.c)

# Archivos objeto de C++ y C
EE_CPP_OBJS = $(subst .cpp,.o,$(EE_CPP_SRC))
EE_C_OBJS = $(subst .c,.o,$(EE_C_SRC))

EE_INCS = -I../../ -I$(PS2SDK)/ports/include
EE_INCS += -I./include/
EE_LDFLAGS = -L../../lib/ -L$(PS2SDK)/ports/lib -L$(PS2DEV)/gsKit/lib
EE_LIBS = -L. -lc -L$(PS2DEV)/gsKit/lib -L../lib -lsdlmixer -lSDL_image -lpng -ljpeg -lsdl -lgskit -ldmakit -lmad -laudsrv -lc -lm
EE_INCS += -I./src/
EE_INCS += -I$(PS2SDK)/ports/include -I$(PS2DEV)/gsKit/ee/gs/include -I$(PS2DEV)/gsKit/ee/dma/include

# Todos los objetos
EE_OBJS = $(EE_CPP_OBJS) $(EE_C_OBJS)
#EE_OBJS += src/audsrv_irx.o

all: $(EE_BIN)

# Compilar archivos C++
%.o: %.cpp
	$(EE_CXX) -o $@ -c $^ $(EE_INCS)

# Compilar archivos C
%.o: %.c
	$(EE_CC) -o $@ -c $^ $(EE_INCS)


#objet file----------------------------
src/audsrv_irx.o: $(PS2SDK)/iop/irx/audsrv.irx
	$(PS2SDK)/bin/bin2o $< $@ audsrv_irx

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

emu: $(EE_BIN)
	$(PSXEMU)/pcsx2 --elf $(PS2DEV)/test/bin/test.elf

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

reset:
	ps2client reset

line: et
	ee-addr2line -e $(EE_BIN)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal_cpp
