//
//  Peer.h
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/23/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#ifndef Peer_h
#define Peer_h

#include <stdio.h>
#include "Containers.h"
#include "PeerConnection.h"
#include "Decoders.h"
#include "ClientInterface.h"

void connectionLoop(short serverPort);

#endif /* Peer_h */
