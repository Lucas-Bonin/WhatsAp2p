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
#define HEADER_DATAGRAM_LENGTH 32
#define HEADER_PARAM_LENGTH 16

// Constantes para Mensagem
#define HEADER_MESSAGE_LENGHT 70
#define HEADER_PARAM_MESSAGE 16
#define FLAG_MESSAGE 16

// Constantes para porta
#define PORT_PEER_CONNECTION 5010

// Numero maximo de pessoas por grupo
#define MAX_GROUP_LENGTH 50

// Maximo de strucs armazenadas no banco de dados
#define MAX_DATABASE_LENGTH 100

// Estruturas para guradar dados do servidor
typedef struct _serverParam{
    short port, listenPort;
    char *hostName;
}serverParam;

// Enums para tratamento de mensagens
typedef enum _OptionsMainMenu{
    CREATE_NEW_GROUP,
    SEND_MESSAGE,
    QUIT
}OptionsMainMenu;

typedef enum _ProtocolCommand{
    MESSAGE,
    PING,
    SERVER_QUERY,
    PEER_CONNECTED,
    PEER_ONLINE,
    PEER_OFFLINE
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

// Estruturas para a decodificacao das mensagens
typedef enum _MessageType{
    TEXT,
    IMAGE
}MessageType;

typedef struct _messageData{
    char number[HEADER_PARAM_MESSAGE];
    char group[HEADER_PARAM_MESSAGE];
    int size;
    int isGroup;
    MessageType type;
    char *data;
}messageData;

// Estrutura para salvar pessoa ou grupo no banco de dados
typedef struct _contactDTO{
    char group[16];
    int isGroup; // Verifica se struct eh um grupo ou uma pessoa
    int totalNumbers;
    int numbers[MAX_GROUP_LENGTH];
}contactDTO;

// Estrutura para comunicacao com o servidor
typedef struct _serverQuery{
    char number[HEADER_PARAM_MESSAGE];
    unsigned short port;
    char ip[HEADER_PARAM_MESSAGE];
}serverQuery;

#endif /* Containers_h */

