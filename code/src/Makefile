main = BN.o client_funcs.o main.o sdl_funcs.o server_funcs.o

cliente = BN.o client_funcs.o client.o sdl_funcs.o

Cflags = -Wall -lSDL2

CC = gcc

main: $(main)
	$(CC) $< $(Cflags) -o $@
cliente: $(cliente)
	$(CC) $< $(Cflags) -o $@

%.o: %.c
	$(CC) $< -c -g -o $@

run:
	./main

clean:
	rm main $(main)