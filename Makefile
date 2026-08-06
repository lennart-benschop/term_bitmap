CFLAGS=-O3 -Wall -I ./incl

default: lib tests

lib: libtbm.a

libtbm.a: src/tbm_main.o src/tbm_plot.o src/tbm_text.o src/tbm_out_sixel.o src/tbm_out_kitty.o src/tbm_out_octant.o src/tbm_out_sextant.o src/tbm_out_braille.o src/tbm_out_fb.o src/tbm_probe.o
	$(AR) r $@ src/*.o
	ranlib libtbm.a

tests: test_simple

test_simple: tests/test_simple.o libtbm.a
	$(CC) -o $@ $^ -L . -ltbm -lm

clean:
	rm test_simple libtbm.a src/*.o tests/*.o

