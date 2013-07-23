include ../Makefile.include

TARGET		 = elapsd

CXXFLAGS	+= -fPIC -I../include
LDFLAGS		+= -shared

LLIB		+= 

SRC_LIB     = $(filter-out Callbacks.cpp, $(wildcard *.cpp))
OBJ_LIB     = $(SRC_LIB:%.cpp=%.o)

ifeq ($(WITH_CAPI), YES)
    OBJ_CAPI = C.o Callbacks.o
else
    OBJ_CAPI =
endif

ifeq ($(WITH_FAPI), YES)
    OBJ_FAPI = F.o
else
    OBJ_FAPI =
endif

INC = $(wildcard ../include/elapsd/*.h)

%.o : %.cpp ${INC}
	$(CXX) $(CXXFLAGS) -c $<

%.o : %.c ${INC}
	$(CC) $(CXXFLAGS) -c $<

%.o : %.F90
	$(FC) $(FFLAGS) -c $<

all: $(OBJ_LIB) $(OBJ_CAPI) $(OBJ_FAPI)
	$(CXX) $(LDFLAGS) $(OBJ_LIB) $(OBJ_CAPI) $(OBJ_FAPI) -o lib$(TARGET).so $(LLIB)

clean:
	rm -f lib$(TARGET).so $(OBJ_LIB) $(OBJ_CAPI) $(OBJ_FAPI) ../include/elapsd/m_elapsd.mod
