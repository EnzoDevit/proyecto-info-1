libs = BN.o client_funcs.o sdl_funcs.o server_funcs.o sock-lib.o

main = $(libs) main.o

server =  $(libs) mainServer.o 

cliente = $(libs) mainCliente.o 

Cflags = -Wall -lSDL2 -lpthread

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