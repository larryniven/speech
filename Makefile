CXXFLAGS += -std=c++11 -I .. -L ../ebt

.PHONY: all clean

all: libspeech.a gen343

clean:
	-rm *.o
	-rm libspeech.a gen343

gen343: gen343.o libspeech.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -lebt

libspeech.a: speech.o
	$(AR) rcs $@ $^

speech.o: speech.h

