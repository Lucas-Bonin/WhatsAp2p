//
//  DecoderServer.h
//  ServerWhatsAp2p
//
//  Created by HyagoHirai on 27/05/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#ifndef Decoders_h
#define Decoders_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Bibliotecas proprias
#include "Containers.h"

serverQuery decodeMessageServer(char *rawData);

void encodeMessageServer(serverQuery dat, char **messageEncoded);


#endif /* Decoders_h */
