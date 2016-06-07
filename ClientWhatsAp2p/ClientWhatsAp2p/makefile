
CC=g++
OBJ=obj
FLAGS=-Wall
OBJLIST=$(OBJ)/Decoders.o $(OBJ)/Peer.o $(OBJ)/PeerConnection.o $(OBJ)/ClientManager.o $(OBJ)/ClientInterface.o $(OBJ)/ContactDAO.o
all: cliente

clean:
	rm $(OBJ)/*.o cliente

cliente: main.c $(OBJLIST)
	$(CC) -o cliente main.c $(OBJLIST) -lpthread $(FLAGS)

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

$(OBJ)/ContactDAO.o: ContactDAO.c ContactDAO.h
	g++ -o $(OBJ)/ContactDAO.o ContactDAO.c -c $(FLAGS)
