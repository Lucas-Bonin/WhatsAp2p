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

enum OptionsMainMenu{
    CREATE_NEW_GROUP,
    SEND_MESSAGE,
    QUIT
};

enum OptionsMainMenu mainMenu();

#endif /* ClientInterface_h */
