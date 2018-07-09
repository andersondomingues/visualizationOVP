#ifndef __APPS_H
#define __APPS_H

// ENDEREÇOS COMPARTILHADOS
//#define RESULTADO (*(volatile unsigned int*)0xef111111)
//#define FLAG      (*(volatile unsigned int*)0xef000000)

#define RESULTADO (*(volatile unsigned int*)0xe0000000)
#define FLAG      (*(volatile unsigned int*)0xe0001000)


#endif

