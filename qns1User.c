/*
A simple C application program named chardevclient.c, to access the 
“character device” via the new read-only device driver, chardev.c in the kernel. 
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIZE 100

int main() {
    int fd, c, rtn;    
    //defined device name: CSC1007OS
    fd = open("/dev/CSC1007OS", O_RDWR);  // open the CSC1007OS driver 
    if (fd == -1)   // if fail to open the driver (the drive is not in kernel)
    {
        perror("open /dev/CSC1007OS");
        exit(EXIT_FAILURE);
    }    
    char str[MAX_SIZE];
    //Get user input to write
    printf("Writing to /dev/CSC1007OS: \n");
    fgets(str, sizeof str, stdin);
      bool x = true;
      while (x){          
        if(str[0] == '\n'){ //check if input empty where user only enter the "Enter" key
            if(rtn = read(fd, &str, 1)>0){
                printf("Reading from /dev/CSC1007OS: \n");
            } 
            if (rtn == -1)  // fail to ready info
            {
                perror("reading /dev/CSC1007OS");
            } 
            x= false; // end of user input, stop the loop
        }
        else{
            // call write
            rtn = write(fd, &str, strlen(str));
            if (rtn == -1) // fail to write info
            {
                perror("writing /dev/CSC1007OS");
            }else{ //successful write
                //The user space application will print out the received sentences/messages plus the length of the characters on the screen.
                printf("%s (%d characters)\n",str,rtn); 
                // Get for more user input to write
                printf("Writing to /dev/CSC1007OS: \n");
                fgets(str, sizeof str, stdin);
            }
        }
      }
      // call release?? idk how REMEBER to ASK THEM.
      //rtn = release("/dev/CSC1007OS" ,O_RDWR);
      printf("\n Program have ended.\n");// print the program ended and exit
    exit(EXIT_SUCCESS);
    //dmesg use to check driver output
}
