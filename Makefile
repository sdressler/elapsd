include Makefile.include

SUBDIRS		= examples lib
CLEANDIRS	= $(SUBDIRS:%=clean-%)
BUILDDIRS	= $(SUBDIRS:%=build-%)

all: $(BUILDDIRS)

$(DIRS): $(BUILDDIRS)

$(BUILDDIRS):
	$(MAKE) -C $(@:build-%=%)

# Build examples only after lib
build-examples: build-lib

lib: 
	$(MAKE) -C lib

doc:
	$(MAKE) -C doc

examples:
	$(MAKE) -C examples

run: $(BUILDDIRS)
	LD_LIBRARY_PATH=lib/ examples/cpp_example

clean:
	$(MAKE) -C lib clean
	$(MAKE) -C doc clean
	$(MAKE) -C examples clean
	rm -f include/elapsd/*.mod

.PHONY: subdirs $(SUBDIRS)
.PHONY: all lib examples clean

