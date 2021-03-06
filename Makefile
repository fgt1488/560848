all: mine yours test

yours: yours.c
	gcc -O3 -march=native -fprofile-generate yours.c -o yours
	make testyours
	gcc -O3 -march=native -fprofile-use yours.c -o yours
	gcc -O3 -march=native yours.c -S

mine: mine.c 
	gcc -O3 -march=native -fprofile-generate mine.c -o mine
	make testmine
	gcc -O3 -march=native -fprofile-use mine.c -o mine
	gcc -O3 -march=native mine.c -S

foo_end.txt:
	python -c 'print("f" + "b" + "a"*0x20000000 + "kbf")' > foo_end.txt

foo_begin.txt:
	python -c 'print("f" + "b" + "a"*0x20000000 + "kba")' > foo_begin.txt

foo_middle.txt:
	python -c 'print("fkb" + "c"*0x10000000 + "d" + "a"*0x10000000 + "bkf")' > foo_middle.txt


mktest: foo_end.txt foo_begin.txt foo_middle.txt

test: testmine testyours

testyours: mktest
	time ./yours < foo_begin.txt
	time ./yours < foo_middle.txt
	time ./yours < foo_end.txt

testmine: mktest
	time ./mine < foo_begin.txt
	time ./mine < foo_middle.txt
	time ./mine < foo_end.txt

clean:
	rm -f yours mine

distclean: clean
	rm -f foo_end.txt foo_begin.txt
