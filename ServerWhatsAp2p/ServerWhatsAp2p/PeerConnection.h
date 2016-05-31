//
//  PeerConnection.h
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/16/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#ifndef PeerConnection_h
#define PeerConnection_h

#include <stdio.h>
#include <netinet/in.h>

// Bibliotecas proprias
#include "Containers.h"


// Estrutura que simulará uma classe
typedef struct _Connection{
    // Propriedades
    int socket;
    
    // Funcoes
    datagram (*recvData)(struct _Connection *self);
    void (*sendData)(struct _Connection *self, datagram data);
    void (*closeConnection)(struct _Connection *self);
    
}Connection;

void newConnection(Connection *self, int socket);

#endif /* PeerConnection_h */
