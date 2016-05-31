
CC=g++
OBJ=obj
FLAGS=-Wall

all: servidor

clean:
	rm $(OBJ)/*.o servidor

servidor: main.c $(OBJ)/Decoders.o $(OBJ)/Peer.o $(OBJ)/PeerConnection.o $(OBJ)/Decoders.o
	$(CC) main.c $(OBJ)/Peer.o $(OBJ)/PeerConnection.o $(OBJ)/Decoders.o -o servidor $(FLAGS) -lpthread

$(OBJ)/Decoders.o: Decoders.c Decoders.h
	$(CC) -c Decoders.c -o $(OBJ)/Decoders.o $(FLAGS)

$(OBJ)/Peer.o: Peer.c Peer.h
	$(CC) Peer.c -c -o $(OBJ)/Peer.o $(FLAGS)

$(OBJ)/PeerConnection.o: PeerConnection.c PeerConnection.h
	$(CC) PeerConnection.c -c -o $(OBJ)/PeerConnection.o $(FLAGS)
