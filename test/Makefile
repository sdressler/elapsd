include ../Makefile.include

TARGET       = elapsd_test_suite

################
# DO NOT TOUCH #
################

TARGETS_SRC  = $(wildcard *.cpp) 
TARGETS_OBJ  = $(TARGETS_SRC:%.cpp=%.o)

CXXFLAGS	+= -I../include
LDFLAGS      = -L../lib -lelapsd -lelapsd_predict \
			   -lboost_unit_test_framework \
			   -lsqlite3 \
               $(if $(DEBUG),-fopenmp,)

all: $(TARGET)

$(TARGET): $(TARGETS_OBJ)
	$(CXX) -o $@ $(TARGETS_OBJ) $(LDFLAGS)

run: $(TARGET)
	LD_LIBRARY_PATH=../lib ./$(TARGET) --show_progress=yes --report_level=detailed

clean:
	rm -f $(TARGETS_OBJ) $(TARGET)

.PHONY: pre_build
pre_build:
	@echo -e "\n $(Y_COLOR)### Building tests.$(NO_COLOR)\n"
