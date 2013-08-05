include ../Makefile.include

SUBDIRS		= elapsd predict
CLEANDIRS	= $(SUBDIRS:%=clean-%)
BUILDDIRS	= $(SUBDIRS:%=build-%)

all: $(BUILDDIRS)

clean: $(CLEANDIRS)

$(DIRS): $(BUILDDIRS)

build-predict: build-elapsd

$(BUILDDIRS):
	$(MAKE) -C $(@:build-%=%)

$(CLEANDIRS):
	$(MAKE) -C $(@:clean-%=%) clean

.PHONY: subdirs $(SUBDIRS)
.PHONY: all clean

