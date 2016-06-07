//
//  Decoders.c
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/26/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#include "Decoders.h"

messageData decodeMessage(char *rawData){


    messageData dat;


    // Decodifica header
    char cellNumber[HEADER_PARAM_MESSAGE];
    char groupName[HEADER_PARAM_MESSAGE];
    char size[HEADER_PARAM_MESSAGE];
    char fileType[HEADER_PARAM_MESSAGE];
    char flagGroup[FLAG_MESSAGE];

    long offset = 0;
    memcpy(flagGroup, rawData + offset, sizeof(flagGroup));

    offset += sizeof(flagGroup);
    memcpy(fileType, rawData + offset, sizeof(fileType));

    offset += sizeof(fileType);
    memcpy(cellNumber, rawData + offset, sizeof(cellNumber));

    offset += sizeof(cellNumber);
    memcpy(groupName, rawData + offset, sizeof(groupName));

    offset += sizeof(groupName);
    memcpy(size, rawData + offset, sizeof(size));

    // Faz o cast para numero
    int sizeNum = (int)strtol(size, (char **)NULL, 10);
    int flagNum = (int)strtol(flagGroup, (char **)NULL, 10);
    MessageType typeNum = (MessageType)strtol(fileType, (char **)NULL, 10);

    // Decodifica dados
    char *messageData = (char*) malloc(sizeNum * sizeof(char));

    offset += sizeof(size);
    memcpy(messageData, rawData + offset, sizeNum * sizeof(char));

    // Cria struct da mensagem decodificada

    strcpy(dat.number, cellNumber);
    strcpy(dat.group, groupName);
    dat.isGroup = flagNum;
    dat.type = typeNum;
    dat.size = sizeNum;
    dat.data = messageData;

    return dat;
}

void encodeMessage(messageData dat, char **messageEncoded){

    // Codificar header
    char size[HEADER_PARAM_MESSAGE];
    char fileType[HEADER_PARAM_MESSAGE];
    char flagGroup[FLAG_MESSAGE];

    sprintf(fileType, "%d", dat.type);
    sprintf(flagGroup, "%d", dat.isGroup);
    sprintf(size, "%d",dat.size);

    // Aloca memoria necessaria para criar a mensagem
    char *message = (char*) malloc((HEADER_MESSAGE_LENGHT + dat.size) * sizeof(char));

    long offset = 0;
    memcpy(message + offset, flagGroup, sizeof(flagGroup));

    offset += sizeof(flagGroup);
    memcpy(message + offset, fileType, sizeof(fileType));

    offset += sizeof(fileType);
    memcpy(message + offset, dat.number, sizeof(dat.number));

    offset += sizeof(dat.number);
    memcpy(message + offset, dat.group, sizeof(dat.group));

    offset += sizeof(dat.group);
    memcpy(message + offset, size, sizeof(size));

    offset += sizeof(size);
    memcpy(message + offset, dat.data, dat.size);

    // Teste
//    printf("\nTESTE encodeMessage\n");
//    char t[100];
//    memcpy(t,message,100);
//    for (int i=0; i<100; i++) {
//        printf("%c ",t[i]);
//    }

    *messageEncoded = message;

}

// Decodificadores para servidor

serverQuery decodeMessageServer(char *rawData){
    serverQuery dat;
    // Decodifica header
    char cellNumber[HEADER_PARAM_MESSAGE];
    char port[HEADER_PARAM_MESSAGE];
    char ip[HEADER_PARAM_MESSAGE];

    long offset = 0;
    memcpy(cellNumber, rawData + offset, sizeof(cellNumber));

    offset += sizeof(cellNumber);
    memcpy(port, rawData + offset, sizeof(port));

    offset += sizeof(port);
    memcpy(ip, rawData + offset, sizeof(ip));

    // Faz o cast para numero
    int portNum = (int)strtol(port, (char **)NULL, 10);
    //int ipNum = (int)strtol(ip, (char **)NULL, 10);

    // Cria struct da mensagem decodificada

    strcpy(dat.number, cellNumber);
    dat.port = portNum;
    //dat.ip = ipNum;
    strcpy(dat.ip,ip);


    return dat;
}

void encodeMessageServer(serverQuery dat, char **messageEncoded){

    // Codificar header
    char port[HEADER_PARAM_MESSAGE];
    //char ip[HEADER_PARAM_MESSAGE];

    sprintf(port, "%d", dat.port);
    //sprintf(ip, "%d", dat.ip);

    // Aloca memoria necessaria para criar a mensagem
    char *message = (char*) malloc(HEADER_PARAM_MESSAGE * 3);

    long offset = 0;
    memcpy(message + offset, dat.number, sizeof(dat.number));

    offset += sizeof(dat.number);
    memcpy(message + offset, port, sizeof(port));

    offset += sizeof(port);
    memcpy(message + offset, dat.ip, sizeof(dat.ip));

    *messageEncoded = message;

}