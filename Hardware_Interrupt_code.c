#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
//Other pins are not designed for general digital output

#define filename1 "/sys/class/gpio/export" // telling the Linux kernel to make that GPIO pin accessible from user space via the sysfs interface.
#define filename2 "/sys/class/gpio/gpio45/direction"
#define filename3 "/sys/class/gpio/gpio45/value"


int begintransmission (char *filename , int flag){
    int fd;
    if ((fd = open(filename, flag)) < 0) 
        printf("file open ERROR");
    }
  
    return fd;
}

int main(){
    int the_fd1 = begintransmission (filename1 , O_WRONLY);
    sleep(2);
    


    if(write(the_fd1 ,"45" , 2 ) != 2){ //a new directory reppears in the sysfs interface
        // here you get files like direction, value, edge
        printf("error in reaching out to the gpio pin");
        close(the_fd1);
    }

    int the_fd2 = begintransmission (filename2, O_WRONLY);
    sleep(2);

    if(write(the_fd2 ,"out" , 3 ) != 3){
        printf("error in changing the direction of the gpio pin");
        close(the_fd2);
    }
    

    int the_fd3 = begintransmission (filename3 , O_RDWR);
    sleep(1);

    while(1){
       
       
            if (write(the_fd3, "1", 1) != 1) {
                printf("write 1 failed");
                break;
            }
            sleep(1);
            
            if (write(the_fd3, "0", 1) != 1) {
                printf("write 0 failed");
                break;
            }
            sleep(1);
        }

        close(the_fd3);
    
            
          
    }

return 0;
     

}