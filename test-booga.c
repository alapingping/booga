#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define RIGHT 0
#define PARA_MISMATCH 1
#define DEVICE_NUMBER_MISMATCH 2
#define BUFFER_SIZE_MISMATCH 3
#define OPERATION_TYPE_MISMATCH 4

const char * tip_info[] = {
    "Attempting to write to booga device",
    "Usage: test-booga <minor number> <buffersize> <read|write>",
    "Error: device number should be a integer and between 0 and 3",
    "Error: buffer size should be between 0 and 1000000",
    "Error: opeartion should be \'read\' or \'write\'"};

int fd = 0;
int judge_digit(char *);

int main(int argc, char * argv[]){

    if( argc != 4 )
        printf("%s\n", tip_info[PARA_MISMATCH]);
    else if( strlen(argv[1]) != 1 ||  *argv[1] < '0' || *argv[1] > '3'){
        printf("%s\n", tip_info[DEVICE_NUMBER_MISMATCH]);
    }
    else if( *argv[1] > 51 || *argv[1] < 48 ){
        printf("%s\n", tip_info[DEVICE_NUMBER_MISMATCH]);
    }
    else if( judge_digit(argv[2]) ){
        printf("%s\n", tip_info[BUFFER_SIZE_MISMATCH]);
    }
    else if( 0 != strcmp(argv[3], "write") && 0 != strcmp(argv[3], "read") ){
        printf("%s\n", tip_info[OPERATION_TYPE_MISMATCH]);
    }
    else {
        
        char *dev_name = (char *) malloc( (strlen("/dev/booga") + strlen(argv[2])) * sizeof(char) );
        strcat(dev_name,"/dev/booga");
        strcat(dev_name,argv[1]);
        fd = open(dev_name, O_RDWR);
        if( fd < 0 ){
            printf("open failed\n");
            return 0;
        }

        if(0 == strcmp(argv[3], "read")){
            int len = atoi(argv[2]);
            char * buf = (char *)malloc(len * sizeof(char));
            read(fd, buf, len);
            printf("%s\n", buf);
        }
        else if(0 == strcmp(argv[3], "write")){
            printf("%s\n", tip_info[RIGHT]);
            
            int len = atoi(argv[2]);
            char * buf = (char *)malloc(len * sizeof(char));
            int i = 0;
            i = write(fd, buf, len);
/*
            if( *argv[1] == 51){
                printf("Terminated\n");
                // system("sh oops.sh&");
            }
                
            else
*/
                printf("Wrote %s bytes.\n", argv[2]);
  
      }
    }
    return 0;
}

int judge_digit(char * src){

    char * temp = src;
    while(*temp){
        if( *temp < '0' || *temp > '9' )
            return 1;
        temp++;
    }

    int src_size = atoi(src);
    if( src_size > 1000000 )
        return 1;
    return 0;
}
