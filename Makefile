.PHONY: all clean

all: wiidraw

wiidraw: main.o hid_osx.o
	g++ -o $@ $^ -framework IOKit -framework CoreFoundation

main.o: main.cpp
	g++ $< -c -o $@

hid.o: hid_osx.c
	g++ $< -c -o $@

clean:
	rm -f *.o wiidraw
