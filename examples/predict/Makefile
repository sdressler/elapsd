include ../../Makefile.include
include ../Makefile.rules

TARGET_DIR = .
SRC        = $(wildcard *.cpp)

ifeq ($(WITH_FAPI), YES)
	SRC += $(wildcard *.F90)
endif

ifeq ($(WITH_CAPI), YES)
	SRC += $(wildcard *.c)
endif

TARGETS = $(basename $(SRC))
OBJ     = $(TARGETS:%=%.o)

CXXFLAGS  += -I../../include
LDFLAGS   = -L../../lib -lelapsd -lelapsd_predict

all: pre_build $(TARGETS)

clean:
	rm -f $(TARGETS)

.PHONY: all 

.PHONY: pre_build
pre_build:
	@echo -e "\n $(Y_COLOR)### Building prediction examples.$(NO_COLOR)\n"

