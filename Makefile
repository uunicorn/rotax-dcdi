CC=msp430-gcc
CFLAGS=-mmcu=msp430g2553
LDFLAGS=-mmcu=msp430g2553

all: dcdi-bench

dcdi-bench: dcdi-bench.o


install: dcdi-bench
	mspdebug rf2500 'prog dcdi-bench'
