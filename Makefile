
export CC=i386-aros-gcc

SUBDIRS = cbm tests

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

