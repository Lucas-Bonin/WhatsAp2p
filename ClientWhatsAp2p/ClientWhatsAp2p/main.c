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
#include "Decoders.h"
#include "Containers.h"
#include <string.h>

int main(int argc, const char * argv[]) {
   
    char *mensagemTeste = "Mensagem Teste";
    messageData teste;
    
    strcpy(teste.data,mensagemTeste);
    strcpy(teste.group,"nomeGrupoTeste");
    strcpy(teste.number,"643563456");
    teste.isGroup = 1;
    teste.type = TEXT;
    teste.size = (int)strlen(mensagemTeste + 1);
    
    char *resp;
    
    encodeMessage(teste, &resp);
    
    messageData teste2;
    
    teste2 = decodeMessage(resp);
    
    printf("%d",teste.size);
    
    return 0;
}
