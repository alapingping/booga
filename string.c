#include <stdio.h>
#include <linux/random.h>

static char * name[] = {"booga1", "booga2"};
static int device_file_major_number[] = {0,0,0,0};
void get_random_bytes(void *buf, int nbytes);

int main(){
    int i = 0;
    // for(i;i<4;i++){
    //     printf("%d\n", device_file_major_number[i]);
    // }
    char val;
    get_random_bytes(&val, 1);
    char cho = ( val & 0x7F ) % 4;
    printf("%c, %c", val, cho);
    return 0;
}