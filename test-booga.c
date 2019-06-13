#include <stdio.h>
#include <stdlib.h>

char * tip_info = "Usage: test-booga <minor number> <buffersize> <read|write>\n";

int main(int argc, char * argv[]){

    if(argc < 4){
        printf("%s", tip_info);
    }

    int i;
    for (i = 0; i < argc; i++){
        printf("%s\n", argv[i]);
    }


    return 0;
}