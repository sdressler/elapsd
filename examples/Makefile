include ../Makefile.include

TARGET_CPP	= cpp_example
TARGET_MDEV	= multidevice
TARGET_THREADS = threads_example

ifeq ($(WITH_CAPI), YES)
TARGET_C	= c_example
TARGET_PREDICT_C = predict_example
else
TARGET_C	=
endif

ifeq ($(WITH_FAPI), YES)
TARGET_F90	= f_example
else
TARGET_F90	=
endif

LLIB		= -L../lib -lelapsd

FFLAGS		+= -I../include/elapsd

OBJ_CPP		= cpp_example.o
OBJ_MDEV	= multidevice.o
OBJ_THR		= $(TARGET_THREADS).o
OBJ_C		= c_example.o
OBJ_PREDICT = predict_example.o
OBJ_F90		= f_example.o

all: pre_build $(TARGET_CPP) $(TARGET_C) $(TARGET_F90) $(TARGET_MDEV) $(TARGET_THREADS) $(TARGET_PREDICT_C)

$(TARGET_CPP) : $(OBJ_CPP)
	$(CXX) $(LDFLAGS) $(OBJ_CPP) -o $(TARGET_CPP) $(LLIB) 

$(TARGET_C) : $(OBJ_C)
	$(CC) $(LDFLAGS) $(OBJ_C) -o $(TARGET_C) $(LLIB)

$(TARGET_THREADS) : $(OBJ_THR)
	$(CXX) $(LDFLAGS) $(OBJ_THR) -o $(TARGET_THREADS) $(LLIB) -fopenmp
	
$(TARGET_MDEV) : $(OBJ_MDEV)
	$(CXX) $(LDFALGS) $(OBJ_MDEV) -o $(TARGET_MDEV) $(LLIB)

$(TARGET_F90) : $(OBJ_F90)
	$(FC) $(FFLAGS) $(OBJ_F90) -o $(TARGET_F90) $(LLIB)

$(TARGET_PREDICT_C) : $(OBJ_PREDICT)
	$(CC) $(LDFLAGS) $(OBJ_PREDICT) -o $(TARGET_PREDICT_C) $(LLIB) -lelapsd_predict

clean:
	rm -f $(TARGET_CPP) $(TARGET_C) $(OBJ_CPP) $(OBJ_C) $(TARGET_F90) $(OBJ_F90) $(TARGET_MDEV) $(OBJ_MDEV) $(TARGET_THREADS) $(OBJ_THR) $(TARGET_PREDICT_C) $(OBJ_PREDICT)

.PHONY: pre_build
pre_build:
	@echo -e "\n $(Y_COLOR)### Building examples.$(NO_COLOR)\n"
