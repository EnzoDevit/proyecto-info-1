main = BN.o client_funcs.o main.o sdl_funcs.o server_funcs.o

server =  sock-lib.o mainServer.o

cliente =  sock-lib.o mainCliente.o

Cflags = -Wall -lSDL2

CC = gcc

main: $(main)
	$(CC) $(main) $(Cflags) -o $@

server: $(server)
	$(CC) $(server) $(Cflags) -o $@

cliente: $(cliente)
	$(CC) $(cliente) $(Cflags) -o $@


%.o: %.c
	$(CC) $< -c -g -o $@

run:
	./main

clean:
	rm main $(main) server $(server) cliente $(cliente)