# Makefile for Compiler Theory Virtual Machine for Quads

CPP = clang++ -o $@

CPPFLAGS = -O3 -Wall -Wextra --std=gnu++14

vmq:	vmq.cpp storage.h quad.h
	$(CPP) $(CPPFLAGS) vmq.cpp

# pseudo-targets

clean:
	rm -f vmq *.o
