include ../Makefile.include

TESTS        = lagrange predict sqlite

################
# DO NOT TOUCH #
################

TARGETS      = $(addprefix test_, $(TESTS))
TARGETS_RUN  = $(TARGETS:%=%-run)

CXXFLAGS	+= -I../include
LDFLAGS      = -L../lib -lelapsd -lelapsd_predict \
			   -lboost_unit_test_framework \
			   -lsqlite3

all: $(TARGETS)

$(TARGETS): $(addsuffix .cpp, $(TARGETS))
	$(CXX) $(CXXFLAGS) -o $@ $(addsuffix .cpp, $@) $(LDFLAGS)

run: $(TARGETS_RUN)

$(TARGETS_RUN): $(TARGETS)
	LD_LIBRARY_PATH=../lib ./$(@:%-run=%) --show_progress=yes

clean:
	rm -f $(TARGETS)

.PHONY: TARGETS_RUN TARGETS
