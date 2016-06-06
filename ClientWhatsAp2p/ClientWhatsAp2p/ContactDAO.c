//
//  ContactDAO.c
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 6/5/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//


#include <stdlib.h>
#include <string.h>

#include "ContactDAO.h"


#define succRead(x) if ( (x) == NULL) {     \
printf("Erro. Final inesperado do arquivo de dados\n"); \
exit(1); \
}

void removeNewLine(char buff[]) {
    while (*buff != '\n' && *buff != '\0') buff++;
    if (*buff == '\n') *buff = '\0';
}

//recebe o numero de telefone do peer e uma lista vazia de contatos
//preenche a lista com os contatos carregados do peer. retorna o numero de contatos carregados
int readData(long int mynum, contactDTO contatos[MAX_DATABASE_LENGTH]) {
    FILE *arq;
    
    char buff[40];
    snprintf(buff,16,"%ld",mynum);
    strcat(buff,".txt");
    
    arq = fopen(buff, "r");
    printf("Abrindo arquivo\n");
    if(arq == NULL) {
        printf("Nenhum arquivo salvo encontrado para este usuario\n");
        return 0;
    }
    /*  Um arquivo binario seria mais eficiente e seguro
     mas um arquivo texto ASC eh mais claro para o proposito academico desse programa
     O arquivo tem a seguinte estrutura:
     linha 1: Numero de telefone do usuario deste cliente,
     linhas seguintes: Grupos e contatos. Um contato é um grupo de um unico membro
     Grupos têm a seguinte estrutura: nome do grupo na primeira linha
     um numero de telefone por linha. lista terminada por uma linha com apenas #
     */
    succRead(fgets(buff,HEADER_PARAM_MESSAGE,arq));
    long int readnum = atol(buff);
    printf("Lido do arquivo: Telefone deste peer: %ld\n",readnum);
    
    int grupos, membro;
    for (grupos=0;grupos<MAX_DATABASE_LENGTH;grupos++) {
        if (fgets(buff,HEADER_PARAM_MESSAGE,arq) == NULL)   //fim do arquivo
            break;
        
        //se o arquivo n acabou, a linha lida é um nome de grupo
        removeNewLine(buff);
        strcpy(contatos[grupos].group,buff);
        printf("grupo %d: nome: %s\n",grupos+1,contatos[grupos].group);
        
        //extrai os membro do grupo
        for (membro = 0; membro < MAX_GROUP_LENGTH;membro++) {
            succRead(fgets(buff,HEADER_PARAM_MESSAGE,arq));
            if (buff[0] == '#') { //fim da descricao do grupo
                break;
            }
            contatos[grupos].numbers[membro] = (int)atol(buff);
            printf("Lido telefone %d\n",contatos[grupos].numbers[membro]);
        }
        contatos[grupos].totalNumbers = membro;
        contatos[grupos].isGroup = (membro != 1);
        printf("Fim do grupo. O grupo tem %d membros\n",membro);
    }
    
    
    fclose(arq);
    return grupos;
}

//Recebe o numero do peer e sua lista de contatos, e quantos contatos tal lista contem
//grava os contatos em um arquivo
int writeData(long int mynum, const contactDTO contatos[MAX_DATABASE_LENGTH], int numGroups) {
    FILE *arq;
    char buff[40];
    snprintf(buff,16,"%ld",mynum);
    strcat(buff,".txt");
    arq = fopen(buff, "w");
    
    printf("Abrindo arquivo %s para escrita\n",buff);
    if(arq == NULL) {
        printf("Erro ao criar o arquivo\n");
        return -1;
    }
    /*  Um arquivo binario seria mais eficiente e seguro
     mas um arquivo texto ASC eh mais claro para o proposito academico desse programa
     O arquivo tem a seguinte estrutura:
     linha 1: Numero de telefone do usuario deste cliente,
     linhas seguintes: Grupos e contatos. Um contato é um grupo de um unico membro
     Grupos têm a seguinte estrutura: nome do grupo na primeira linha
     um numero de telefone por linha. lista terminada por uma linha com apenas#
     */
    
    snprintf(buff,16,"%ld",mynum);
    fputs(buff,arq);
    fputs("\n",arq);
    int grupos, membro;
    for (grupos=0;grupos<numGroups;grupos++) {
        fputs(contatos[grupos].group,arq);
        fputs("\n",arq);
        //extrai os membro do grupo
        for (membro = 0; membro < contatos[grupos].totalNumbers ;membro++) {
            snprintf(buff,16,"%d",contatos[grupos].numbers[membro]);
            fputs(buff,arq);
            fputs("\n",arq);
        }
        fputs("#\n",arq);
    }
    
    
    fclose(arq);
    return 1;
    
}