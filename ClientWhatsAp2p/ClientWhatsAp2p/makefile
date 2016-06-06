
CC=g++
OBJ=obj
FLAGS=-Wall

all: cliente

clean:
	rm $(OBJ)/*.o cliente

cliente: main.c $(OBJ)/Decoders.o $(OBJ)/Peer.o $(OBJ)/PeerConnection.o $(OBJ)/Decoders.o $(OBJ)/ClientManager.o $(OBJ)/ClientInterface.o
	$(CC) -o cliente main.c $(OBJ)/Peer.o $(OBJ)/PeerConnection.o $(OBJ)/Decoders.o $(OBJ)/ClientManager.o $(OBJ)/ClientInterface.o -lpthread $(FLAGS)

$(OBJ)/Decoders.o: Decoders.c Decoders.h
	$(CC) -o $(OBJ)/Decoders.o -c Decoders.c $(FLAGS)

$(OBJ)/Peer.o: Peer.c Peer.h
	$(CC) -o $(OBJ)/Peer.o Peer.c -c $(FLAGS)

$(OBJ)/PeerConnection.o: PeerConnection.c PeerConnection.h
	$(CC) -o $(OBJ)/PeerConnection.o PeerConnection.c -c $(FLAGS)

$(OBJ)/ClientInterface.o: ClientInterface.c ClientInterface.h
	$(CC) -o $(OBJ)/ClientInterface.o ClientInterface.c -c $(FLAGS)

$(OBJ)/ClientManager.o: ClientManager.c ClientManager.h
	g++ -o $(OBJ)/ClientManager.o ClientManager.c -c $(FLAGS)
