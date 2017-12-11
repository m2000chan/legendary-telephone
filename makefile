all: control.c main.c
	gcc -o control control.c
	gcc -o main main.c

run: control main

clean: control main
	rm control
	rm main
