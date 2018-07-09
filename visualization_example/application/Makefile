IMPERAS_HOME := $(shell getpath.exe "$(IMPERAS_HOME)")
include $(IMPERAS_HOME)/bin/Makefile.include

ifndef IMPERAS_HOME
  IMPERAS_ERROR := $(error "IMPERAS_HOME not defined")
endif

CROSS?=OR1K
all: application1.$(CROSS).elf  application2.$(CROSS).elf

-include $(IMPERAS_HOME)/lib/$(IMPERAS_ARCH)/CrossCompiler/$(CROSS).makefile.include
ifeq ($($(CROSS)_CC),)
    IMPERAS_ERROR := $(error "Please install the $(CROSS)_CC toolchain")
endif

application1.$(CROSS).elf: application1.$(CROSS).o
	$(V) echo "Linking Application $@"
	$(V) $(IMPERAS_LINK) -g -o $@  $^  $(IMPERAS_LDFLAGS)

application1.$(CROSS).o: application1.c
	$(V) echo "Compiling Application $@"
	$(V) $(IMPERAS_CC) -g -c  $(IMPERAS_CFLAGS) -D$(CROSS)  -o $@  $<

application2.$(CROSS).elf: application2.$(CROSS).o
	$(V) echo "Linking Application $@"
	$(V) $(IMPERAS_LINK) -g -o $@  $^  $(IMPERAS_LDFLAGS)

application2.$(CROSS).o: application2.c
	$(V) echo "Compiling Application $@"
	$(V) $(IMPERAS_CC) -g -c  $(IMPERAS_CFLAGS) -D$(CROSS)  -o $@  $<


clean:
	- rm -f application1.$(CROSS).elf application1.$(CROSS).o  application2.$(CROSS).elf application2.$(CROSS).o
