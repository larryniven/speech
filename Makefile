CXXFLAGS += -std=c++11 -I .. -L ../ebt
AR = gcc-ar

.PHONY: all clean

all: libspeech.a gen343 gen-edges-343 label-frames cat-frames shuffle-frames

clean:
	-rm *.o
	-rm libspeech.a gen343 gen-edges-343 label-frames

gen343: gen343.o libspeech.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -lebt

gen-edges-343: gen-edges-343.o libspeech.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -lebt

cat-frames: cat-frames.o libspeech.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -lebt

label-frames: label-frames.o libspeech.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -lebt

shuffle-frames: shuffle-frames.o
	$(CXX) $(CXXFLAGS) -o $@ $^ -lebt

libspeech.a: speech.o
	$(AR) rcs $@ $^

speech.o: speech.h

