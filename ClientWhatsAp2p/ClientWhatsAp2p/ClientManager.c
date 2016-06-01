//
//  ClientManager.c
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/23/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#include "ClientManager.h"
#include <arpa/inet.h> //inet_ntoa()

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

//Cria imagem e texto
void createTextMessage(char **data){
    printf("Digite o texto\n");
    char *text; //TODO: Não esta faltando Malloc?
    getchar();
    gets(text); //TODO: trocar por fgets ou outra função segura
    *data = text;
}

void createImageMessage(char **data){
    printf("IMPOSSIVEL CRIAR IMAGE, ENCERRANDO O PROGRAMA\n");
    exit(0);
}


// TODO: Fazer as 3 funcoes muthafocka do cliente

void addContact(){
    
    
}

void createGroup(){
    
}

void sendDataToPeer(){
    contactDTO contact;
    contact = findContactMenu();
    
    int answ;
    printf("Qual o tipo da mensagem?\n1 - Texto\n2 - Imagem\n");
    do{
        scanf("%d",&answ);
    }while(answ != 1 && answ != 2);
    
    char *messageData;
    MessageType type;
    
    if(answ == 1){
        createTextMessage(&messageData);
        type = TEXT;
    }else{
        createImageMessage(&messageData);
        type = IMAGE;
    }
    
    // TODO: Pegar o meu numero de algum lugar salvo.
    char myNumber[HEADER_PARAM_MESSAGE];
    
    datagram encMessage = encodeMessageToPeer(myNumber, contact.group, type, messageData);
    
    //Envia mensagem para o contato
    short port;
    char *host;
    for (int i=0; i<contact.totalNumbers; i++) {
        
        if(1){ // TODO: Verifica se contato esta online e recebe porta e IP
            int newSocket;
            newSocket = makeClientSocket(port, host);
            
            Connection peerConection;
            newConnection(&peerConection, newSocket);
            
            peerConection.sendData(&peerConection,encMessage);
            printf("Mensagem enviada com sucesso para o numero: %d\n",contact.numbers[i]);
            
        }else{
            printf("Contato: %d esta offline\n",contact.numbers[i]);
        }
    }
    
}




void requestLoop(short port, char *hostName){
    printf("Entrou no Request Loop\n");
    
    int servSocket;
    Connection servConnection;
    OptionsMainMenu opt;
    
    // Se conecta com o servidor
    servSocket = makeClientSocket(port, hostName);
    newConnection(&servConnection, servSocket);
    
    // TODO: montar mensagem para o servidor
    
//    //Cria estrutura para comunicacao com o servidor
//    struct sockaddr_in localAddress;
//    unsigned int addrSize = sizeof(localAddress);
//
//    getsockname(servSocket, (struct sockaddr*)&localAddress, &addrSize);
//    printf("IP address is: %s\n", inet_ntoa(localAddress.sin_addr));
//    printf("port is: %d\n", (int) ntohs(localAddress.sin_port));
    
    do{
        
        opt = mainMenu();
        
        switch (opt) {
            case SEND_MESSAGE:
                printf("Opcao Mandar mensagem\n");
                sendDataToPeer();
                break;
            case QUIT:
                printf("Sair da aplicacao");
                break;
            default:
                printf("Opcao default");
                break;
        }
        
    }while((opt != QUIT));
}





