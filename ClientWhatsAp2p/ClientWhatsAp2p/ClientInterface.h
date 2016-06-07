//
//  ClientInterface.h
//  ClientWhatsAp2p
//
//  Created by Lucas Bonin on 5/23/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#ifndef ClientInterface_h
#define ClientInterface_h

#include <stdio.h>

//Bibliotecas proprias
#include "Containers.h"

OptionsMainMenu mainMenu();
void showNewMessage(messageData dat);
contactDTO findContactMenu(contactDTO contacts[MAX_DATABASE_LENGTH], int numContatos);


#endif /* ClientInterface_h */
