#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulatorIntercepts.h"
#include "apps.h"


int main(int argc, char **argv) {


    static int result, i, id ;

    //id = impProcessorId();
    id = 2;


    printf("CPU %d starting acumulação...\n", id);

    for(;;) {

        while(!FLAG) {}

        for(i=result=0; i<=RESULTADO; result += i, i++) ;    // acumula

        printf("CPU %d: sum(%3d) = %d [flag:%d  %x] \n", id, RESULTADO, result, FLAG, &RESULTADO);
        
        if(FLAG==2)  break;
        
        FLAG = 0;
    } 

	FLAG = 0;
    printf("terminei CPU acumulação...\n", id);

    return 1;
}
