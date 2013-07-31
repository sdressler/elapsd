include ../Makefile.include

TESTS = predict

TARGETS = $(addprefix test_, $(TESTS))

CXXFLAGS	+= -I../include -DBOOST_TEST_DYN_LINK
LDFLAGS      = -L../lib -lelapsd -lelapsd_predict -lboost_unit_test_framework

all: $(TARGETS)

$(TARGETS): $(addsuffix .cpp, $(TARGETS))
	$(CXX) $(CXXFLAGS) -o $@ $(addsuffix .cpp, $@) $(LDFLAGS)

run: $(TARGETS)
	LD_LIBRARY_PATH=../lib ./$< --show_progress=yes

clean:
	rm -f $(TARGETS)
