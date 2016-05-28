//
//  ClientInterface.c
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/23/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#include "ClientInterface.h"

// TODO: Caprichar mais nos menus (pesquisar sobre gtk.org)


// Verifica se usuario escolheu uma opcao valida
int checkOption(int input, int maxNum){
    
    if (input > maxNum || input <= 0)
        return 0;
    
    return 1;
}

//Retorna a opcao escolhida pelo usuário
OptionsMainMenu mainMenu(){
    
     OptionsMainMenu opt;
    
    int answ;
    
    while(1){
        printf ("1 - cadastrar mensagem\n2 - ler mensagem\n3 - apagar mensagem\n4 - sair da aplicacao\n" );
        scanf("%d", &answ);
        
        if (checkOption(answ, 4) == 0){
            printf("Opcao invalida\n\n");
        }else{
            break;
        }
    }
    
    switch(answ){
        case 1:
            opt = SEND_MESSAGE;
            break;
        default:
            opt = CREATE_NEW_GROUP;
            
    }
    
    return opt;
}

void showNewMessage(messageData dat){
    printf("\n======================================================================\n");
    printf("Nova Mensagem\n\n");
    printf("Numero: %s\n",dat.number);
    printf("Grupo: %s\n",(dat.isGroup == 1) ? dat.group : "SEM GRUPO");
    if(dat.type == TEXT)
        printf("\n%s\n",dat.data);
    else
        printf("Mensagem com tipo desconhecido: %d\n",dat.type);
    printf("\n======================================================================\n");
    
}

// Retorna uma struct que identifica um grupo ou pessoa
void sendMessageMenu(){
    int answ;
    printf("\n======================================================================\n");
    printf("Para quem a mensagem sera mandada ?\n");
    
    // TODO: Mostrar uma tabela com os contatos
    
    // TODO: Quando usuario escolher para quem ira a mensagem, perguntar para ele se aquela foi realmente a opcao que ele escolheu
    
    //showContact()
    
}

void showContact(){
    
}
