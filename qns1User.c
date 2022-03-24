#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<stdbool.h>
 
#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM
 
int main(){
   int ret, fd;
   char stringToSend[BUFFER_LENGTH], contProgramme;
   printf("Starting Device...\n");
   fd = open("/dev/CSC1007OS", O_RDWR);             // Open the device with read/write access
   if (fd < 0){
      perror("Failed to open the Device.");
      return errno;
   }
   
   while (1){
      printf("Write a message to send to the kernel module:\n");
      scanf("%[^\n]%*c", stringToSend);                // Read in a string (with spaces)
      printf("\nWriting message to the Device \"%s\"...\n", stringToSend);
      ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
      if (ret < 0){
         perror("Failed to write the message to the Device.");
         return errno;
      }
   
      printf("Press ENTER to read back from the Device\n");
      getchar();
   
      printf("Reading from the Device...\n");
      ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM
      if (ret < 0){
         perror("Failed to read the message from the Device.");
         return errno;
      }
      printf("The received message is: \"%s\"\n\n", receive);
      printf("Would you like to write another message?[Y/N]:");
      contProgramme = getchar();
      if (contProgramme == 'N' || contProgramme == 'n'){
         break;
      }
      getchar();
   }
   printf("Closing Device...\n");
   return 0;
}