CXXFLAGS += -std=c++11 -I .. -L ../ebt
AR = gcc-ar

.PHONY: all clean

all: libspeech.a gen343 gen-edges-343 gen-frames

clean:
	-rm *.o
	-rm libspeech.a gen343 gen-edges-343 gen-frames

gen343: gen343.o libspeech.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -lebt

gen-edges-343: gen-edges-343.o libspeech.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -lebt

gen-frames: gen-frames.o libspeech.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -lebt

libspeech.a: speech.o
	$(AR) rcs $@ $^

speech.o: speech.h

