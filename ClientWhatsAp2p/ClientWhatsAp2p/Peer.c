//
//  Peer.c
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/23/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#include "Peer.h"

//Bibliotecas para conexao
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

//Bibliotecas para thread
#include <pthread.h>

// Constantes
#define NUM_CONNECTIONS 5 // Numero maximo de conexoes pendentes

// Cria um socket e faz ele aguardar conexoes
int makeServerSocket(short port, int numConnections){
    
    int serverSocket;
    struct sockaddr_in serverAddr;
    
    // Cria um novo socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket()");
        exit(0);
    }
    
    // Define porta e IP do servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port); // Conversao necessaria para passar informacoes pela rede
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Socket se liga a todos os enderecos de IP
    
    // Faz o bind do socket com o IP e porta indicados
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
        perror("Bind()");
        exit(0);
    }
    
    // Fica aguardando conexoes
    if (listen(serverSocket, numConnections)){ // numConnections é o numero de conexoes pendentes
        perror("Listen()");
        exit(0);
    }
    
    return serverSocket;
}

// Trata da mensagem recebida
void messageHandler(char *data){
    // Deodifica mensagem
    messageData dat = decodeMessage(data);
    
    // Mostra a mensagem para o usuario
    showNewMessage(dat);
    
    //TODO: Melhor local para desalocar memoria ja usada e se necessario, armazenar a informacao se usuario nao quiser ver a mensagem naquele momento.
    
}

void* peerHandler(void* param){
    
    //faz o cast dos dados para um socketData
    socketData *sData = (socketData*) param;
    
    //cria um objeto para tratar das requisicoes
    Connection peerConnection;
    newConnection(&peerConnection, sData->socket);
    
    // Espera mensagem
    datagram dat = peerConnection.recvData(&peerConnection);
    
    // Trata o tipo da mensagem
    
    switch (dat.op) {
        case MESSAGE:
            messageHandler(dat.data);
            break;
        case PING:
            printf("Funcao para mandar um ping de resposta para o servidor");
            break;
        default:
            printf("Tipo de mensagem invalido: %d",dat.op);
            break;
    }
    
    // Fecha conexao
    peerConnection.closeConnection(&peerConnection);
    
    return NULL;
}

// Um loop que aceita conexoes de outros peers
void connectionLoop(short serverPort){
    printf("Entrou no connectionLoop\n");
    
    // Dados socket
    int clientSocket, serverSocket;
    struct sockaddr_in clientAddr;
    
    int clientLen = sizeof(clientAddr); // Inicializacao necessária para a chamada da funcao
    
    //Dados thread
    pthread_t newClientThread;
    
    serverSocket = makeServerSocket(serverPort, NUM_CONNECTIONS);
    
    while(1){
        
        // Aceita conexao e cria um socket para o cliente
        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, (socklen_t *)&clientLen)) == -1){
            perror("Accept()");
            exit(0);
        }
        
        socketData nSocket;
        nSocket.socket = clientSocket;
        nSocket.socketAddr = clientAddr;
        
        // Cria thread
        pthread_create(&newClientThread, NULL, &peerHandler, &nSocket);
    }
    
    //TODO: De alguma maneira executar server.close() quando sair do loop
}
