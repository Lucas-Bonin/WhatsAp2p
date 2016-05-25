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

// Enums para tratamento de mensagens
enum OptionsMainMenu{
    CREATE_NEW_GROUP,
    SEND_MESSAGE,
    QUIT
};

typedef struct _rawData{
    int teste;
}rawData;

typedef struct _socketData{
    int socket;
    struct sockaddr_in socketAddr;
}socketData;

#endif /* Containers_h */

