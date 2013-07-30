include ../Makefile.include

SUBDIRS		= elapsd
CLEANDIRS	= $(SUBDIRS:%=clean-%)
BUILDDIRS	= $(SUBDIRS:%=build-%)

all: $(BUILDDIRS)

clean: $(CLEANDIRS)

$(DIRS): $(BUILDDIRS)

$(BUILDDIRS):
	$(MAKE) -C $(@:build-%=%)

$(CLEANDIRS):
	$(MAKE) -C $(@:clean-%=%) clean

.PHONY: subdirs $(SUBDIRS)
.PHONY: all clean

