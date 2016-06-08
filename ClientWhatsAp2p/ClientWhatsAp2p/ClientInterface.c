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
    printf("Grupo: %s\n",(dat.isGroup == 1) ? dat.group : "");
    if(dat.type == TEXT)
        printf("\n%s\n",dat.data);
    else if(dat.type == IMAGE) {
        printf("Imagem recebida:  size: %d... nome %s\n",dat.size-50,dat.data);

        //nos 50 primeiros bytes de data temos o nome da imagem
        FILE * img = fopen(dat.data, "w");
        if (img == NULL) { printf("Img is null!\n"); return; }

        int read = 0, size = dat.size - 50;
        while (read < size) {
            read += (int)fwrite( dat.data+read+50, 1,1024, img);
            printf("Gravando imagem...%d bytes\n",read);
        }
        printf("Done. Escritos %d...tamanho %d\n",read,size);
    }
    printf("\n======================================================================\n");

}

// Retorna uma struct que identifica um grupo ou pessoa
contactDTO findContactMenu(contactDTO contacts[MAX_DATABASE_LENGTH], int numContatos){
    int answ;


    while(1){
        printf("\n======================================================================\n");
        printf("Para quem a mensagem sera mandada ?\n");
        printf("Digite o numero correspondente ao contato\n");

        // Tamanho do loop sera o mesmo que o retorno da funcao que faz o load dos contatos
        for (int i=0; i<numContatos; i++) {
            printf("INDEX: %d. ",i);
            showContact(contacts[i]);
        }

        scanf("%d",&answ);

        while(!checkOption(answ,numContatos+1)){ //tamanho maximo sera igual ao retorno da funcao
            printf("Opcao invalida\n");
            scanf("%d",&answ);
        }

        printf("Tem certeza que quer escolher esse numero\n1 - Sim\n2 - Nao\n");
        showContact(contacts[answ]);

        int confirm;
        do{
            scanf("%d",&confirm);
        }while(confirm != 1 && confirm != 2);

        if(confirm == 1)
            break;
    }

    return contacts[answ];

}

contactDTO addContact() {
    contactDTO cont;
    printf("Qual o nome do seu amiguinho? (ate 16 caracteres\n");
    getchar();
    fgets(cont.group,16,stdin);
    removeNewLine(cont.group);
    printf("E o numero de telefone?\n");
    scanf("%ld",&cont.numbers[0]);
    cont.isGroup = 0; // Verifica se struct eh um grupo ou uma pessoa
    cont.totalNumbers = 1;
    return cont;
}

contactDTO createGroup(const contactDTO contactList[], const int numContatos) {
    contactDTO cont;
    cont.totalNumbers = 0;
    printf("Qual o nome da nova panelinha? (ate 16 caracteres\n");
    getchar();
    fgets(cont.group,16,stdin);
    removeNewLine(cont.group);

    int opt;
    printf("OK. selecione os contatos que serao adicionados ao grupo.\n");
    printf("Se um grupo for selecionado todos os seus membros serao adicionados ao novo grupo\n");
    do {
        printf("INDEX: 0. Finalizar\n");
        for (int i=0; i<numContatos; i++) {
            printf("INDEX: %d. ",i+1);
            showContact(contactList[i]);
        }
        scanf("%d",&opt);
        if (opt > 0 && opt <= numContatos) {
            for (int j=0;j<contactList[opt-1].totalNumbers ; j++) { //copia todos os numeros do contato escolhido
                //printf("adicionando contato num %d tel %ld\n",cont.totalNumbers,contactList[opt-1].numbers[j]);
                cont.numbers[cont.totalNumbers++] = contactList[opt-1].numbers[j];
            }
        }
    } while (opt != 0);
    if (cont.totalNumbers >= 2)
        cont.isGroup = 1;
    else cont.isGroup = 0;
    return cont;
}
//