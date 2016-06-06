//
//  ClientManager.h
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/23/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#ifndef ClientManager_h
#define ClientManager_h

#include <stdio.h>
#include <stdlib.h>

// Bibliotecas para socket
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>

// Bibliotecas proprias
#include "PeerConnection.h"
#include "ClientInterface.h"
#include "Decoders.h"

void requestLoop(short port, char *hostName, short listenPort);



#endif /* ClientManager_h */
