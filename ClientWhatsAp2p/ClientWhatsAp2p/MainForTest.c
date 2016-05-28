//
//  MainForTest.c
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/27/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#include <stdio.h>

#include "Containers.h"
#include "Decoders.h"
#include <string.h>

datagram encodeMessageToPeerTest(char *myNumber, char* groupName, MessageType type, char *message){
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


int main(int argc, const char * argv[]){
    
    datagram dat = encodeMessageToPeerTest("999-9", "grupoT", TEXT, "MENSAGEM PARA O GRUPO");
    
    // Teste
    printf("\nTESTE main\n");
    char t[100];
    memcpy(t,dat.data,100);
    for (int i=0; i<100; i++) {
        printf("%c ",t[i]);
    }
    
    return 0;

}