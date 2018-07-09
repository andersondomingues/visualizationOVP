#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulatorIntercepts.h"
#include "apps.h"

#define NUM_VALUES 40

int fibo(int i) {
    return (i>1) ? fibo(i-1) + fibo(i-2) : i;
}


int main(int argc, char **argv) {

    static int resultadofib, i, j, id ;

    //id = impProcessorId();
    id = 1;


    printf("CPU %d starting fibonacci...\n", id);

    for(i=0; i<NUM_VALUES; i++)  {

        if(i==(NUM_VALUES-1)){
        	FLAG = 2;
        	goto exit;
        }else{
        	FLAG = 1;
        }

        resultadofib = fibo(i);

        RESULTADO = resultadofib;

        printf("CPU %d: fib(%3d) = %d [flag:%d] \n", id, i, RESULTADO, FLAG);

        while(FLAG == 1) {}

    }
exit:
    printf("terminei CPU fibonacci...\n", id);

    return 1;
}
