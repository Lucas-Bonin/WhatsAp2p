//
//  ClientManager.c
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/23/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#include "ClientManager.h"
#include "ContactDAO.h"
#include <arpa/inet.h> //inet_ntoa()
#include <unistd.h>
#include <dirent.h>

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
datagram encodeMessageToPeer(char *myNumber, char* groupName, MessageType type, char *message, int tamImg){
    messageData mDat;
    strncpy(mDat.number,myNumber,sizeof(mDat.number));

    if (strcmp(groupName,"") != 0) {
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
            mDat.size = tamImg;
            break;
        default:
            printf("TIPO DE DADO DA MENSAGEM DESCONHECIDO, ENCERRANDO PROGRAMA");
            exit(0);
            break;
    }

    // Transforma struct de mensagem em uma string
    char *encodedMessage;
    int lengthEncMessage = encodeMessage(mDat, &encodedMessage);

//    printf("\nVOLTANDO DO ENCODE MESSAGE:\n\n");
//    for (int i=0; i<(mDat.size + HEADER_MESSAGE_LENGHT + 6); i++) {
//        printf("%c ",encodedMessage[i]);
//    }
//    printf("\n\n\n");


    //Cria datagrama
    datagram dat;

    dat.op = MESSAGE;
  //  dat.size = mDat.size + HEADER_MESSAGE_LENGHT;
    dat.size = lengthEncMessage;

    dat.data = encodedMessage;

    return dat;
}

//Cria imagem e texto
void createTextMessage(char **data){
    printf("Digite o texto\n");
    char *text = (char*) malloc(1200);
    getchar();
    fgets(text,1200,stdin); //TODO: trocar por fgets ou outra função segura
    printf("Mensagem: %s\n",text);
    *data = text;
}

void listFiles(char img[]){
    DIR *d;
    d = opendir("imagens");

    struct dirent *dir;
    int i = 1, size;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            size = strlen(dir->d_name);
            //if (strcmp(dir->d_name+size-3,"jpg") == 0 || strcmp(dir->d_name+size-3,"png") == 0)
                printf("%d - %s\n",i,dir->d_name);
        }
        closedir(d);
    }
    printf("ESCOLHA O ARQUIVO QUE DESEJA ENVIAR\n");
    char tmp[30];
    scanf("%s",tmp);
    strcpy(img,"imagens/");
    strcat(img,tmp);

}

int createImageMessage(char **data){
    char imgNome[50];
    listFiles(imgNome);
    printf("img nome %s\n",imgNome);

    FILE * img = fopen(imgNome, "r");
    if (img == NULL) {
        printf("nao deu pra carregar\n");
        return 0;
    }
    fseek(img, 0, SEEK_END);
    long int size = ftell(img);
    fseek(img, 0, SEEK_SET);

    *data = (char*) malloc( (size+50)*sizeof(char));
    if (!(*data)) { printf("erro ao iniciar o buffer\n"); return 0; }
    memcpy(*data, imgNome, 50);
    int read = 0;
    while (read < size) {
        read += (int)fread( (*data)+read+50, 1,1024, img);
        printf("Lendo imagens read...%d bytes\n",read);
    }
    printf("Done. lidos %d...tamanho %d\n",read,size);
    return size+50;
}


datagram queryToServer(const char number[]) { //, short *port, const unsigned int *ip) {
    datagram dat;
    char *messageEncoded;
    serverQuery query;
    dat.op = PING;
    query.port = 0;
    strcpy(query.number,number);
    encodeMessageServer(query, &messageEncoded);
    dat.size = HEADER_PARAM_MESSAGE * 3;
    dat.data = messageEncoded;
    return dat;
}

serverQuery checkPeerOnline(short serverPort, char *serverHostName, const char wantedNumber[]) {
    Connection servConnection;

    datagram dta = queryToServer(wantedNumber);
    int servSocket = makeClientSocket(serverPort, serverHostName);
    newConnection(&servConnection, servSocket);
    //envia para o servidor o ping
    servConnection.sendData(&servConnection,dta);
    //resposta do ping
    dta = servConnection.recvData(&servConnection);
    servConnection.closeConnection(&servConnection);

    if (dta.op == PEER_ONLINE) {
        serverQuery query = decodeMessageServer(dta.data);
        printf("Peer %s em %s : %d\n",query.number,query.ip,ntohs(query.port));
        return query;
    } else if (dta.op == PEER_OFFLINE)
        printf("O peer esta offline :(\n");
    else
        printf("RESPOSTA PARA: checkPeerOnline INVALIDA\n");
    serverQuery query;
    query.number[0] = '\0';
    return query;
}

void sendDataToPeer(short serverPort, char *serverHostName, contactDTO contacts[MAX_DATABASE_LENGTH], int numContatos, char myNum[26]){
    contactDTO contact;
    contact = findContactMenu(contacts,numContatos);
    printf("Contato num: %ld\n",contact.numbers[0]);
    int answ, tamImg;
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
        tamImg = createImageMessage(&messageData);
        type = IMAGE;
    }

    // verifica se eh grupo ou nao
    char group[16] = "";
    if (contact.isGroup == 1){
        strncpy(group, contact.group,sizeof(group));
    }
    datagram encMessage = encodeMessageToPeer(myNum, group, type, messageData, tamImg);


    //Envia mensagem para o contato
    short port;
    char *host;
    for (int i=0; i<contact.totalNumbers; i++) {
        char strnum[33];
        snprintf(strnum,33,"%ld",contact.numbers[i]);
        serverQuery query = checkPeerOnline(serverPort,serverHostName,strnum);
        if(query.number[0] != '\0'){ // TODO: Verifica se contato esta online e recebe porta e IP
            port = ntohs(query.port);
            host = query.ip;
            int newSocket;
            newSocket = makeClientSocket(port, host);

            Connection peerConection;
            newConnection(&peerConection, newSocket);

            peerConection.sendData(&peerConection,encMessage);
            printf("Mensagem enviada com sucesso para o numero: %ld\n",contact.numbers[i]);

        }else{
            printf("Contato: %ld esta offline\n",contact.numbers[i]);
        }
    }

}


int sayHiToServer(short port, char *hostName, short listenPort, const char number[]) {
    int servSocket;
    Connection servConnection;

    // Se conecta com o servidor
    servSocket = makeClientSocket(port, hostName);
    newConnection(&servConnection, servSocket);

    //Cria um datagrama com as informações da thread de listen deste cliente
    datagram dta;
    dta.op = PEER_CONNECTED;
    serverQuery query;
    query.port = htons(listenPort);

    strcpy(query.number,number);

    //codifica a mensagem e a adiciona ao datagrama
    char *messageEncoded;
    encodeMessageServer(query, &messageEncoded);
    dta.size = HEADER_PARAM_MESSAGE * 3;
    dta.data = messageEncoded;

    //envia a mensagem para o servidor marcar este cliente como on
    servConnection.sendData(&servConnection,dta);
    servConnection.closeConnection(&servConnection);
    return 1;
}


void requestLoop(short port, char *hostName, short listenPort){
    printf("Entrou no Request Loop\n");

    char num[26];
    printf("Digite o numero de seu telefone\n");
    scanf("%s",num);

    //funcao que envia para o servidor dados para conexão neste peer
    sayHiToServer(port,hostName,listenPort,num);

    //Recupera os contatos do arquivo
    contactDTO contacts[MAX_DATABASE_LENGTH];
    int numContatos = readData(atol(num),contacts);

    OptionsMainMenu opt;

    do{

        opt = mainMenu();

        switch (opt) {
            case SEND_MESSAGE:
                printf("Opcao Mandar mensagem\n");
                sendDataToPeer(port,hostName,contacts,numContatos,num);
                break;
            case CREATE_NEW_CONTACT:
                printf("Opcao criar contato\n");
                contacts[numContatos] = addContact();
                printf("Contato %s de tel %ld cadastrado\n",contacts[numContatos].group,contacts[numContatos].numbers[0]);
                numContatos++;
                break;
            case CREATE_NEW_GROUP:
                printf("Opcao criar grupo\n");
                contacts[numContatos] = createGroup(contacts,numContatos);
                if (contacts[numContatos].totalNumbers > 0)  //a funcao retorna 0 contatos caso tenha sido cancelada
                    numContatos++;
                break;
            case QUIT:
                printf("Sair da aplicacao");
                break;
            default:
                printf("Opcao invalida");
                break;
        }

    }while((opt != QUIT));

    //salva os contatos antes de sair;
    writeData(atol(num),contacts,numContatos);
}
