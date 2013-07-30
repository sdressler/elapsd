include ../Makefile.include

TARGET		 = elapsd

CXXFLAGS	+= -fPIC -I../include
LDFLAGS		+= -shared

LLIB		+= 

SRC_LIB      = $(filter-out Callbacks%, $(wildcard *.cpp))
OBJ_LIB      = $(SRC_LIB:%.cpp=%.o)

ifeq ($(WITH_CAPI), YES)
    OBJ_CAPI = Callbacks.o CallbacksParams.o C.o cparams.o
else
    OBJ_CAPI =
endif

ifeq ($(WITH_FAPI), YES)
    OBJ_FAPI = F.o
else
    OBJ_FAPI =
endif

INC = $(wildcard ../include/elapsd/*.h)

all: $(OBJ_LIB) $(OBJ_CAPI) $(OBJ_FAPI)
	$(CXX) $(LDFLAGS) $(OBJ_LIB) $(OBJ_CAPI) $(OBJ_FAPI) -o lib$(TARGET).so $(LLIB)

clean:
	rm -f lib$(TARGET).so $(OBJ_LIB) $(OBJ_CAPI) $(OBJ_FAPI) ../include/elapsd/m_elapsd.mod
