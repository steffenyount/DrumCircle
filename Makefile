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

LDFLAGS=-lstdc++ -lbe -lmidi -ltracker -ltranslation

%.o: %.cpp
	gcc -c -o $@ $<

DrumCircle: $(OBJS)
	gcc -o $@ $(OBJS) $(LDFLAGS)
