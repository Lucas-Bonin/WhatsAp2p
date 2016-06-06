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
        printf ("1 - Enviar mensagem\n2 - Cadastrar novo grupo\n3 - Cadastrar novo contato\n4 - sair da aplicacao\n" );
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
        case 2:
            opt = CREATE_NEW_GROUP;
            break;
        case 3:
            opt = CREATE_NEW_CONTACT;
            break;
        default:
            opt = QUIT;

    }

    return opt;
}

//Mostra informacoes do contato na tela
void showContact(contactDTO contact){

    if(contact.isGroup == 1){
        printf("Grupo: %s\n",contact.group);
        printf("Numeros nesse grupo:\n");
        for (int i=0; i<contact.totalNumbers; i++) {
            printf("\t%ld\n",contact.numbers[i]);
        }
    }else{
        printf("Numero do Contato:\n%ld\n",contact.numbers[0]);
    }
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
contactDTO findContactMenu(){
    int answ;

    // TODO: Receber todos os contatos da base de dados
    contactDTO contacts[MAX_DATABASE_LENGTH];


    while(1){
        printf("\n======================================================================\n");
        printf("Para quem a mensagem sera mandada ?\n");
        printf("Digite o numero correspondente ao contato");

        // Tamanho do loop sera o mesmo que o retorno da funcao que faz o load dos contatos
        for (int i=0; i<1; i++) {
            printf("INDEX: %d\n",i+1);
            showContact(contacts[i]);
        }

        scanf("%d",&answ);

        while(checkOption(answ, 1)){ //tamanho maximo sera igual ao retorno da funcao
            printf("Opcao invalida\n");
            scanf("%d",&answ);
        }

        printf("Tem certeza que quer escolher esse numero\n1 - Sim\n2 - Nao\n");
        showContact(contacts[answ]);

        do{
            scanf("%d",&answ);
        }while(answ != 1 && answ != 0);

        if(answ == 1)
            break;
    }

    return contacts[answ];

}
//