include Makefile.include

SUBDIRS		= examples lib test
CLEANDIRS	= $(SUBDIRS:%=clean-%)
BUILDDIRS	= $(SUBDIRS:%=build-%)

all: $(BUILDDIRS)
	@echo -e "\n $(Y_COLOR)### All done.$(NO_COLOR)\n"

$(DIRS): $(BUILDDIRS)

$(BUILDDIRS):
	$(MAKE) -C $(@:build-%=%)

# Build examples only after lib
build-examples: build-lib
build-test: build-lib build-examples

lib: 
	$(MAKE) -C lib

test:
	$(MAKE) -C test

doc:
	$(MAKE) -C doc

examples:
	$(MAKE) -C examples

run: $(BUILDDIRS)
	LD_LIBRARY_PATH=lib/ examples/cpp_example

run_test: $(BUILDDIRS)
	$(MAKE) -C test run

clean:
	$(MAKE) -C lib clean
	$(MAKE) -C test clean
	$(MAKE) -C doc clean
	$(MAKE) -C examples clean
	rm -f include/elapsd/*.mod

.PHONY: subdirs $(SUBDIRS)
.PHONY: all lib test examples clean

