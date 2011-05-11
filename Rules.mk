export PATH:= ${shell echo $$PATH}:/usr/local/amiga/bin

UNAME=${shell uname | tr A-Z a-z}

ifndef ARCH
ARCH=${shell uname -m}
endif

ifeq ($(ARCH),i686)
ARCH=i386
endif

ifndef OS
OS=$(UNAME)
endif

ifeq ($(UNAME),linux)
  ifneq ($(OS),linux)
    # Cross compilation
    ifeq ($(OS),amigaos)
      # Cross compiling for AmigaOS, without specifying an architecture - we assume m68k
      ifeq ($(ARCH),i386)
        ARCH=m68k
      endif

      CC=$(ARCH)-$(OS)-gcc
    else
      CC=$(ARCH)-linux-$(OS)-gcc
    endif
  endif
endif

ifeq ($(OS),aros)
LDFLAGS= -lintuition -lexec -ldos
endif

ifeq ($(OS),amigaos)
CFLAGS= -noixemul
LDFLAGS= -lamiga
endif

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS)  -o $@ $^

% : %.o ;  $(CC) $(CFLAGS) $(CPPFLAGS)  -o $@ $^ $(LDFLAGS) 

