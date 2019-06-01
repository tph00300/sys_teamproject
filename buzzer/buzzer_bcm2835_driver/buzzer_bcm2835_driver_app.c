#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define  CL1  131
#define  CL2  147
#define  CL3  165
#define  CL4  175
#define  CL5  196
#define  CL6  221
#define  CL7  248

#define  CM1  262
#define  CM2  294
#define  CM3  330
#define  CM4  350
#define  CM5  393
#define  CM6  441
#define  CM7  495

#define  CH1  525
#define  CH2  589
#define  CH3  661
#define  CH4  700
#define  CH5  786
#define  CH6  882
#define  CH7  990

int song_1[] = {CM3,CM5,CM6,CM3,CM2,CM3,CM5,CM6,CH1,CM6,CM5,CM1,CM3,CM2,
                CM2,CM3,CM5,CM2,CM3,CM3,CL6,CL6,CL6,CM1,CM2,CM3,CM2,CL7,
                CL6,CM1,CL5};

int beat_1[] = {1,1,3,1,1,3,1,1,1,1,1,1,1,1,3,1,1,3,1,1,1,1,1,1,1,2,1,1,
                1,1,1,1,1,1,3};

int main(int argc, char **argv)
{
    int dev = 0;
    int data = 262;
    //char buffer[32];

    printf("Device driver test.\n");

    dev = open("/dev/buzzer_device", O_RDWR);
    if(dev == -1)
    {
        perror("failed open because");
        return 1;
    }

    printf("play music\n");
    for(i=0;i<sizeof(song_1)/4;i++)
    {
        write(dev, &song_1[i], 4);   
        delay(beat_1[i] * 500);
    }

    close(dev);

    exit(EXIT_SUCCESS);

    return 0;
}