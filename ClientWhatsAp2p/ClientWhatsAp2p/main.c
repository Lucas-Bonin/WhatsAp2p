//
//  main.c
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/16/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

// Bibliotecas proprias
#include "Peer.h"
#include "ClientManager.h"

// Bibliotecas para thread
#include <pthread.h>

// Thread para peer

void* interFunc(void *param){
    printf("Entrou no thread de interface\n");

    serverParam *sParam = (serverParam*) param;

    requestLoop(sParam->port, sParam->hostName, sParam->listenPort); // Quando servidor estiver funcionando descomentar essa linha
    return NULL;
}

void* peerFunc(void *param){
    printf("Entrou no thread de peer\n");

    short port = *(short*)param;

    connectionLoop(port);

    return NULL;
}

int main(int argc, const char * argv[]){
    /* initialize random seed: */
    srand (time(NULL));

    //Recebendo parametros
    unsigned short port;

    // O primeiro argumento (argv[1]) é o hostname do servidor.
    // O segundo argumento (argv[2]) é a porta do servidor.

    // Cheat
    argc = 3;
    argv[1] = "localhost";
    argv[2] = "5000";

    if (argc != 3)
    {
        fprintf(stderr, "Use: %s hostname porta\n", argv[0]);
        exit(1);
    }

    port = (unsigned short) atoi(argv[2]);

     // Porta de thread listen
    short portParam;
    portParam = PORT_PEER_CONNECTION + rand()%5000;
    printf("porta sorteada: %d\n",portParam);

    serverParam param;
    param.port = port;
    param.listenPort = portParam;
    param.hostName = (char*)argv[1];

    // Thread para peer
    pthread_t peerThread;
    pthread_create(&peerThread, NULL, &peerFunc, &portParam);

    // Thread para interface
    pthread_t interThread;
    pthread_create(&interThread, NULL, &interFunc, &param);


    while(1){
    }

    return 0;
}
