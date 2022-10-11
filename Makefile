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

DrumCircle: $(OBJS)
	gcc -o $@ $(OBJS) $(LDFLAGS)

.PHONY: install
install: DrumCircle
	install -m 755 -d $(bindir)
	install -m 755 -d $(datadir)/
	install -m 755 -d $(settingsdir)/
	install -m 755 DrumCircle $(bindir)/
	install -m 644 Stuff/* $(datadir)/
	install -m 644 Profiles/* $(settingsdir)/
	settype -t application/x-vnd.SY.DrumCircle-Profile $(settingsdir)/*
