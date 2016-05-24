//
//  PeerConnection.c
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/16/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#include "PeerConnection.h"

#include <stdlib.h>

//Bibliotecas para socket
#include <unistd.h>
#include <sys/socket.h>

//Funcoes da classe
rawData recvData(struct _Connection *self){ // TODO: Tratar dado recebido antes de mostrar para o usuario e ter um parametro que diga se e uma mensagem de texto ou outro tipo de arquivo.
    rawData teste;
    // Recebe uma mensagem via TCP
    
    if(recv(self->socket, &teste, sizeof(teste), 0) < 0){
        perror("Recv()");
        exit(0);
    }

    return teste;
}

void sendData(Connection *self){ //TODO: Mandar arquivos de maneira correta
    rawData teste;
    if (send(self->socket, &teste, sizeof(teste), 0) < 0){
        perror("Send()");
        exit(0);
    }
}

void closeConnection(Connection *self){
    close(self->socket);
}


// Construtor da classe
void newConnection(Connection *self, int socket){
    self->socket = socket;
    
    self->recvData = &recvData;
    self->sendData = &sendData;
    self->closeConnection = &closeConnection;
    
}

