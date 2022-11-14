SOURCES=\
	code/DrumCircleApp.cpp \
	code/DrumCircleData.cpp \
	code/DrumCircleMidi.cpp \
	code/DrumCircleView.cpp \
	code/DrumCircleWindow.cpp \
	code/DrumVarView.cpp \
	code/globals.cpp \
	code/main.cpp \
	code/stddlg.cpp \
	code/ValueButton.cpp \
	code/ValueButtonII.cpp

OBJS=$(SOURCES:.cpp=.o)

datadir?=Stuff
settingsdir?=Profiles
CXXFLAGS+=-DDATAPREFIX=\"$(datadir)\" -DSETTINGSPREFIX=\"$(settingsdir)\"

LDFLAGS=-lstdc++ -lbe -lmidi -ltracker -ltranslation

%.o: %.cpp
	gcc -c -o $@ $< $(CXXFLAGS)

DrumCircle: $(OBJS) code/DrumCircle.rsrc
	gcc -o $@ $(OBJS) $(LDFLAGS)
	xres -o $@ code/DrumCircle.rsrc
	mimeset -F $@

.PHONY: clean
clean:
	@rm -f DrumCircle $(OBJS)
	@echo "Clean done."

.PHONY: install
install: DrumCircle
	install -m 755 -d $(bindir)
	install -m 755 -d $(datadir)/
	install -m 755 -d $(settingsdir)/
	install -m 755 DrumCircle $(bindir)/
	install -m 644 Stuff/* $(datadir)/
	install -m 644 Profiles/* $(settingsdir)/
	settype -t application/x-vnd.SY.DrumCircle-Profile $(settingsdir)/*
