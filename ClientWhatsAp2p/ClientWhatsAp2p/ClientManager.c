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

// Codifica mensagem
datagram encodeMessageToPeer(char *myNumber, char* groupName, MessageType type, char *message){
    messageData mDat;
    strncpy(mDat.number,myNumber,sizeof(mDat.number));
    
    if (groupName != NULL) {
        strncpy(mDat.group, groupName, sizeof(mDat.group));
        mDat.isGroup = 1;
    }else{
        mDat.isGroup = 0;
    }
    
    mDat.type = type;
    
    mDat.data = message;
    
    switch (type) {
        case TEXT:
            mDat.size = (int)strlen(message);
            break;
        case IMAGE:
            printf("TAMANHO DE IMAGEM NAO DEFINIDO, ENCERRANDO PROGRAMA");
            exit(0);
            break;
        default:
            printf("TIPO DE DADO DA MENSAGEM DESCONHECIDO, ENCERRANDO PROGRAMA");
            exit(0);
            break;
    }
    
    // Transforma struct de mensagem em uma string
    char *encodedMessage;
    encodeMessage(mDat, &encodedMessage);
    
    //Cria datagrama
    datagram dat;
    
    dat.op = MESSAGE;
    dat.size = mDat.size + HEADER_MESSAGE_LENGHT;
    dat.data = encodedMessage;
    
    return dat;
}



// TODO: Fazer as 3 funcoes muthafocka do cliente

void addContact(){
    
    
}

void createGroup(){
    
}

void sendDataToPeer(){
    // TODO: Verificar se contato esta online
    //
}




void requestLoop(short port, char *hostName){
    printf("Entrou no Request Loop\n");
    
    int servSocket;
    Connection servConnection;
    OptionsMainMenu opt;
    
    // Se conecta com o servidor
    servSocket = makeClientSocket(port, hostName);
    newConnection(&servConnection, servSocket);
    
    do{

        
        // TODO: Fazer algum tipo de requisicao para o servidor sempre que tentar enviar uma mensagem para alguem
        
        
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





