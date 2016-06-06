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
#include "Decoders.h"
#include <netinet/in.h>
#include <string.h>
//Bibliotecas para thread
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h> //gethostbyname()

// Constantes
#define NUM_CONNECTIONS 5 // Numero maximo de conexoes pendentes

int numberOfPeers = 0;
serverQuery peerConnected[MAX_PEER_CONNECTED];

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

int makeClientSocket(short port, char *hostName) {

    struct hostent *hostnm; // Representacao binaria do endereco de IP
    struct sockaddr_in server; // Endereco de um socket
    int s;

    // Obter IP do servidor via DNS
    hostnm = gethostbyname(hostName);
    if (hostnm == (struct hostent*) 0){ // Verifica se existe esse hostName
        fprintf(stderr, "Gethostbyname failed\n");
        return -1;
    }

    // Define dados sobre o servidor
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = *((unsigned int *) hostnm->h_addr);
    // Cria socket
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }


    // Se conecta com o servidor
    if (connect(s, (struct sockaddr *) &server, sizeof(server)) < 0){
        return -1;
    }

    return s;
}

int findPeer(short *port, char ip[HEADER_PARAM_MESSAGE], int *index, char *number) {
    for(int i=0;i<numberOfPeers;i++) {
        if(strcmp(peerConnected[i].number, number) == 0){
            strcpy(ip,peerConnected[i].ip);
            *port = peerConnected[i].port;
            *index = i;
            return 1;
        }
    }
    return 0;
}

datagram sendPeerOnline(datagram dta, serverQuery query, short port, char ip[HEADER_PARAM_MESSAGE]) {
    printf("ONLINE\n");
    char *messageEncoded;
    dta.op = PEER_ONLINE;
    strcpy(query.ip, ip);
    query.port = htons(port);
    encodeMessageServer(query, &messageEncoded);
    dta.size = HEADER_PARAM_MESSAGE * 3;
    dta.data = messageEncoded;
    return dta;
}

datagram sendPeerOffline(datagram dta, int index, int op) {
    printf("OFFLINE\n");
    dta.op = PEER_OFFLINE;
    //dta.data = NULL;

    // Se o peer estiver no array, porem estiver offline, entao remove ele da lista
    if(op == 1){
        printf("removendo da lista de peers...\n");
        peerConnected[index] = peerConnected[numberOfPeers - 1];
        numberOfPeers--;
    }

    return dta;
}

datagram isPeerOnline(datagram dta, serverQuery query, short port, char ip[HEADER_PARAM_MESSAGE]) {
    int index = 0;
    if(findPeer(&port, ip, &index, query.number) == 0)
        return sendPeerOffline(dta, index, 0);

    printf("pingando %s:%d\n",ip,port);
    int s = makeClientSocket(port,ip);

    // Se nao conseguir criar socket, encerra o programa
    if(s < 1){
        //printf("Erro ao criar socket para verificacao de cliente/n");
        //exit(0);
        return sendPeerOffline(dta, index, 1);
    }
    
    // Por enquanto o cliente nao retorna nenhuma mensagem quando recebe um ping.
    
    // TODO: Verificacao do peer
    Connection cliConnection;
    newConnection(&cliConnection, s);
    dta.op = PING;
    dta.size = 0;
    cliConnection.sendData(&cliConnection,dta);
    //dta = servConnection.recvData(&servConnection);
    close(s);
    return sendPeerOnline(dta, query, port, ip);
}

void* peerHandler(void* param) {

    printf("Thread Criada\n");

    //faz o cast dos dados para um socketData
    socketData *sData = (socketData*) param;

    //cria um objeto para tratar das requisicoes
    Connection peerConnection;
    newConnection(&peerConnection, sData->socket);

    datagram dta = peerConnection.recvData(&peerConnection);
    printf("Datagram %d\n",dta.op);

    serverQuery query = decodeMessageServer(dta.data);

    printf("OPERACAO RECEBIDA: %d\nNUMERO TELEFONE: %s\n",dta.op,query.number);

//    unsigned int ip = 0;
//    short port = 0;

    switch (dta.op){
        case PING:
            
            dta = isPeerOnline(dta,query,query.port,query.ip);
            peerConnection.sendData(&peerConnection,dta);
            printf("DEU CERTO\n");
            break;
            
        case PEER_CONNECTED:
            // Completando as informacoes recebidas do cliente
            //printf("IP address is: %s\n", inet_ntoa(sData->socketAddr.sin_addr));
            //printf("port is: %d\n", (int) ntohs(sData->socketAddr.sin_port)); //a porta do cliente é de outra thread dele
            
            strcpy(query.ip,inet_ntoa(sData->socketAddr.sin_addr)); //podemos extrair o ip do proprio socket
            query.port = ntohs(query.port); //porta da thread de listen do cliente vinda da mensagem
            peerConnected[numberOfPeers++] = query;
            printf("CADASTRANDO PEER NUMERO: %s PORTA: %d IP: %s\n",query.number,query.port,query.ip);
            break;
        default:
            break;
    }
    peerConnection.closeConnection(&peerConnection);
    return NULL;
}


// Um loop que aceita conexoes de outros peers
void connectionLoop(short serverPort) {

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
