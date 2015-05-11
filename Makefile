CXXFLAGS += -std=c++11 -I ../

.PHONY: all clean

all: libspeech.a

clean:
	-rm *.o
	-rm libspeech.a

libspeech.a: speech.o
	$(AR) rcs $@ $^

speech.o: speech.h
