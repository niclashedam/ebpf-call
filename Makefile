CC = clang

.PHONY: clean

all:
	$(MAKE) ebpf-call
	$(MAKE) examples/basic.o
	$(MAKE) examples/read.o
	$(MAKE) examples/state.o

clean:
	rm -f ./ebpf-call
	rm -f ./examples/*.o

ebpf-call: src/loader.c src/exts.h src/modules/*.c src/modules/*.h
	$(CC) -O2 -g -o ebpf-call src/modules/*.c src/loader.c -lubpf

examples/%.o: examples/%.c
	$(CC) -target bpf -g -c -o $@ $<


.DEFAULT_GOAL := ebpf-call
