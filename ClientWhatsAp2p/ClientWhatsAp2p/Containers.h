//
//  Containers.h
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/23/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#ifndef Containers_h
#define Containers_h

#include <netinet/in.h>

// Constantes
#define PAYLOAD_MAX_LENGTH 100
#define HEADER_DATAGRAM_LENGTH 32
#define HEADER_PARAM_LENGTH 16

// Enums para tratamento de mensagens
typedef enum _OptionsMainMenu{
    CREATE_NEW_GROUP,
    SEND_MESSAGE,
    QUIT
}OptionsMainMenu;

typedef enum _ProtocolCommand{
    PROT1,
    PROT2,
    PROT3
}ProtocolCommand;

typedef struct _socketData{
    int socket;
    struct sockaddr_in socketAddr;
}socketData;

// Estruturas para o envio de mensagens
typedef struct _datagram{
    ProtocolCommand op;
    int size;
    char *data; // Tamanho da mensagem pode variar
}datagram;

#endif /* Containers_h */

