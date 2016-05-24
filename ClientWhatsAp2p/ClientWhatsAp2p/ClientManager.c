//
//  ClientManager.c
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/23/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#include "ClientManager.h"

// Cria um socket para se comunicar com o servidor
int makeClientSocket(short port, char *hostName){
    
    struct hostent *hostnm; // Representacao binaria do endereco de IP
    struct sockaddr_in server; // Endereco de um socket
    int s;
    
    // Obter IP do servidor via DNS
    hostnm = gethostbyname(hostName);
    if (hostnm == (struct hostent*) 0){ // Verifica se existe esse hostName
        fprintf(stderr, "Gethostbyname failed\n");
        exit(0);
    }
    
    // Define dados sobre o servidor
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = *((unsigned int *) hostnm->h_addr);
    
    // Cria socket
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket()");
        exit(3);
    }
    
    
    // Se conecta com o servidor
    if (connect(s, (struct sockaddr *) &server, sizeof(server)) < 0){
        perror("Connect()");
        exit(0);
    }
    
    return s;
}

// TODO: Fazer as 3 funcoes muthafocka do cliente

void addContact(){
    
}

void createGroup(){
    
}

void sendData(){
    // Verificar se contato esta online
}




void requestLoop(short port, char *hostName){
    int servSocket;
    int peerSocket;
    Connection servConnection;
    
    
    enum OptionsMainMenu opt;
    
    do{
        // Se conecta com o servidor
        servSocket = makeClientSocket(port, hostName);
        newConnection(&servConnection, servSocket);
        
        // TODO: Fazer algum tipo de requisicao para o servidor sempre que tentar enviar uma mensagem para alguem
        
        // TODO: Desacoplar verificacao e escolha da Opcao
        
         opt = mainMenu();
        
        switch (opt) {
            case SEND_MESSAGE:
                printf("Mandar mensagem\n");
                break;
            case QUIT:
                printf("Sair da aplicacao");
                break;
            default:
                printf("Opcao default");
                break;
        }
        
    }while((opt =! QUIT));
}





