//
//  ContactDAO.h
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 6/5/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#ifndef ContactDAO_h
#define ContactDAO_h

#include <stdio.h>

#include "Containers.h"

int readData(long int mynum, contactDTO contatos[MAX_DATABASE_LENGTH]);


int writeData(long int mynum, const contactDTO contatos[MAX_DATABASE_LENGTH], int numGroups);

void removeNewLine(char buff[]);

#endif /* ContactDAO_h */
