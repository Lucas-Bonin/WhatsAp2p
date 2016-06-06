//
//  Decoders.h
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/26/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#ifndef Decoders_h
#define Decoders_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Bibliotecas proprias
#include "Containers.h"

messageData decodeMessage(char *rawData);

void encodeMessage(messageData dat, char **messageEncoded);

serverQuery decodeMessageServer(char *rawData);

void encodeMessageServer(serverQuery dat, char **messageEncoded);

#endif /* Decoders_h */
