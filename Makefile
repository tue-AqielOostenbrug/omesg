SUBDIRS := $(shell find src -type f -name Makefile -exec dirname {} \;)

.PHONY: all $(SUBDIRS)

all: $(SUBDIRS)

clean: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)