all:
	gcc -o platypusser src/*.c -Iinclude -lraylib -lm

run:
	./platypusser

clean:
	rm -f platypusser