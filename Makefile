CC := gcc
CFLAGS := -Wall 
LDLIBS := -lm

.SUFFIXES:
.SUFFIXES: .o .c

.PHONY: all
all: keep-cpt-VCSEL tempcontrol

keep-cpt-VCSEL: keep-cpt-VCSEL.o ethernet.o
tempcontrol: tempcontrol.o ethernet.o

.c.o:

.PHONY: clean
clean:
	$(RM) -rf *.o

keep-cpt-VCSEL.o tempcotrol.o ethernet.o: ether_prog.h
