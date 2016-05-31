//
//  DecoderServer.c
//  ServerWhatsAp2p
//
//  Created by HyagoHirai on 27/05/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#include "Decoders.h"
#include "Containers.h"
#include <string.h>

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
    int ipNum = (int)strtol(ip, (char **)NULL, 10);
    
    // Cria struct da mensagem decodificada
    
    strcpy(dat.number, cellNumber);
    dat.port = portNum;
    dat.ip = ipNum;
    
    return dat;
}

void encodeMessageServer(serverQuery dat, char **messageEncoded){
    
    // Codificar header
    char port[HEADER_PARAM_MESSAGE];
    char ip[HEADER_PARAM_MESSAGE];
    
    sprintf(port, "%d", dat.port);
    sprintf(ip, "%d", dat.ip);
    
    // Aloca memoria necessaria para criar a mensagem
    char *message = malloc(HEADER_PARAM_MESSAGE * 3);
    
    long offset = 0;
    memcpy(message + offset, dat.number, sizeof(dat.number));
    
    offset += sizeof(dat.number);
    memcpy(message + offset, port, sizeof(port));
    
    offset += sizeof(port);
    memcpy(message + offset, ip, sizeof(ip));
    
    *messageEncoded = message;
    
}