include ../../Makefile.include

TARGET_DIR   = ../
TARGET		 = elapsd_predict

CXXFLAGS	+= -fPIC -I../../include
LDFLAGS		+= -shared

LLIB		+= 

SRC_LIB      = $(filter-out Callbacks%, $(wildcard *.cpp))
OBJ_LIB      = $(SRC_LIB:%.cpp=%.o)

ifeq ($(WITH_CAPI), YES)
    OBJ_CAPI = Callbacks.o cpredict.o
else
    OBJ_CAPI =
endif

ifeq ($(WITH_FAPI), YES)
    OBJ_FAPI = 
else
    OBJ_FAPI =
endif

INC = $(wildcard ../../include/elapsd/*.h) $(wildcard ../../include/elapsd/predict/*.h)

all: pre_build $(OBJ_LIB) $(OBJ_CAPI) $(OBJ_FAPI)
	$(CXX) $(LDFLAGS) $(OBJ_LIB) $(OBJ_CAPI) $(OBJ_FAPI) -o $(TARGET_DIR)lib$(TARGET).so $(LLIB)

clean:
	rm -f $(TARGET_DIR)lib$(TARGET).so $(OBJ_LIB) $(OBJ_CAPI) $(OBJ_FAPI)

.PHONY: pre_build
pre_build:
	@echo -e "\n $(Y_COLOR)### Building $(TARGET).$(NO_COLOR)\n"
