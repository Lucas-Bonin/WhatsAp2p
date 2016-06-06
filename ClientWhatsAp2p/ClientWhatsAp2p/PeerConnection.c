//
//  PeerConnection.c
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/16/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#include "PeerConnection.h"

#include <stdlib.h>

#include <string.h> // memcpy

//Bibliotecas para socket
#include <unistd.h>
#include <sys/socket.h>

void encodeData(datagram dat, char **dataEncoded){
    char size[HEADER_PARAM_LENGTH];
    char command[HEADER_PARAM_LENGTH];

    // Aloca memoria necessaria para criar o datagrama
    char *datagram = (char*) malloc((HEADER_DATAGRAM_LENGTH + dat.size) * sizeof(char));

    // Transforma o parametros em string
    sprintf(command, "%d", dat.op);
    sprintf(size, "%d",dat.size);

    //Concatena tudo em um unico datagrama
    memcpy(datagram, size, sizeof(size));
    memcpy((datagram + sizeof(size)), command, sizeof(command));
    memcpy((datagram + HEADER_DATAGRAM_LENGTH), dat.data, dat.size);

    *dataEncoded = datagram;
}


//Funcoes da classe
datagram recvData(struct _Connection *self){

    // Recebe header
    long partialData = 0;
    long currentData = 0;
    char *rawData = (char*) malloc(HEADER_DATAGRAM_LENGTH); // Aloca memoria suficiente para receber o header

    // Loop para receber o header inteiro
    while(partialData < HEADER_DATAGRAM_LENGTH){

        currentData = recv(self->socket, (rawData + partialData), (HEADER_DATAGRAM_LENGTH - partialData), 0);
        if(currentData < 0){
            perror("Recv()");
            exit(0);
        }

        partialData += currentData;
    }

    // Decodifica header
    char size[HEADER_PARAM_LENGTH];
    char command[HEADER_PARAM_LENGTH];

    memcpy(size, rawData, sizeof(size));
    memcpy(command, (rawData + sizeof(size)), sizeof(command));

    int sizeNum = (int)strtol(size, (char **)NULL, 10);
    ProtocolCommand cmdNum = (ProtocolCommand)strtol(command, (char **)NULL, 10);

    // Recebe a mensagem
    rawData = (char*) realloc(rawData, sizeNum * sizeof(char)); // Realoca memoria com o tamanho da mensagem

    partialData = currentData = 0;

    // Loop para receber a mensagem inteira
    while(partialData < sizeNum){

        currentData = recv(self->socket, (rawData + partialData), sizeNum - partialData, 0);
        if(currentData < 0){
            perror("Recv()");
            exit(0);
        }

        partialData += currentData;
    }

    // Verifica o que veio na mensagem
    //for(int i=0; i<sizeNum; i++){
    //    printf("%c ",rawData[i]);
    //}

    //Monta a struct para enviar como retorno da funcao
    datagram dat;

    dat.op = cmdNum;
    dat.size = sizeNum;
    dat.data = rawData;

    return dat;
}

void sendData(Connection *self, datagram data){

    char *dataEncoded;

    // Codifica a mensagem
    encodeData(data, &dataEncoded);

    // Envia a mensagem
    long totalDataSize = (data.size + HEADER_DATAGRAM_LENGTH);
    long currentDataSent = 0;
    long totalDataSent = 0;

    while(totalDataSent < totalDataSize){
        currentDataSent = send(self->socket, (dataEncoded + totalDataSent), (totalDataSize - totalDataSent), 0);
        if (currentDataSent < 0){
            perror("Send()");
            exit(0);
        }

        totalDataSent += currentDataSent;
    }

    //Desacola memoria ja utilizada
    free(dataEncoded);
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
