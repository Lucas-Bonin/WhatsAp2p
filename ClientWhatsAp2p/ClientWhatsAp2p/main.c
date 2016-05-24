//
//  main.c
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/16/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#include <stdio.h>
#include <netinet/in.h>

#include "PeerConnection.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    struct sockaddr_in addr;
    int socket;
    Connection teste;
    newConnection(&teste,  socket);
    teste.closeConnection(&teste);
    
    return 0;
}
