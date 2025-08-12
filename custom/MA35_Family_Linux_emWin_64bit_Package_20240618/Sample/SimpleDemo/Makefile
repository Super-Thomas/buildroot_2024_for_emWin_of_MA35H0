.SUFFIXES : .x .o .c .s

CC=../../../../output/host/bin/aarch64-nuvoton-linux-gnu-gcc
LINK=$(CC)
STRIP=../../../../output/host/bin/aarch64-nuvoton-linux-gnu-strip
CFLAGS=-MD -O2
LDFLAGS=-L../../Lib -lNUemWin -lm -lts -ldl -pthread

APPDIR=.
CONFHEADER=../../Config
GUIHEADER=../../Include
CONFDIR=$(CONFHEADER)
TSLIBDIR=tslib
TSLIBHEADER=$(TSLIBDIR)
GUIX=GUI_X

BINARY=SimpleDemo

APPSOURCES := $(shell find $(APPDIR) -name '*.c')
APPOBJECTS := $(addprefix ,$(APPSOURCES:%.c=%.o))
APPDFILES := $(addprefix ,$(APPSOURCES:%.c=%.d))

RM=rm -rf

all: $(BINARY)

$(APPDIR)/%.o: $(APPDIR)/%.c
	$(CC) $(CFLAGS) -I. -I$(CONFHEADER) -I$(GUIHEADER) -I$(TSLIBHEADER) -c $< -o $@

$(CONFDIR)/$(GUIX).o: $(CONFDIR)/$(GUIX).c
	$(CC) $(CFLAGS) -I. -I$(CONFHEADER) -I$(GUIHEADER) -c $< -o $@

$(BINARY): $(APPOBJECTS) $(CONFDIR)/$(GUIX).o
	$(LINK) $(APPOBJECTS) $(CONFDIR)/$(GUIX).o $(LDFLAGS) -o $(BINARY)
	$(STRIP) $(BINARY)

clean:
	$(RM) $(BINARY) $(APPOBJECTS) $(APPDFILES) $(CONFDIR)/$(GUIX).o $(CONFDIR)/$(GUIX).d
