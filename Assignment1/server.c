#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#define SIZE 1000000
void sig_chld(int signo)
{
     pid_t    pid;
     int      stat;
     while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
         printf("child %d terminated\n", pid);
     return;
 }

void dostuff(int); /* function prototype */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}
char* asctime(const struct tm *timeptr)
{
  static const char wday_name[][4] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
  };
  static const char mon_name[][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };
  static char result[26];
  sprintf(result, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n",
    wday_name[timeptr->tm_wday],
    mon_name[timeptr->tm_mon],
    timeptr->tm_mday, timeptr->tm_hour,
    timeptr->tm_min, timeptr->tm_sec,
    1900 + timeptr->tm_year);
  return result;
}

int main(int argc, char *argv[])
{
     int sockfd1,sockfd2, newsockfd1,newsockfd2, portno1,portno2, pid;
     socklen_t clilen;
     struct sockaddr_in serv_addr[2], cli_addr[100];

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd1 < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr[0], sizeof(serv_addr[0]));
     portno1 = atoi(argv[1]);
     serv_addr[0].sin_family = AF_INET;
     serv_addr[0].sin_addr.s_addr = INADDR_ANY;
     serv_addr[0].sin_port = htons(portno1);
     if (bind(sockfd1, (struct sockaddr *) &serv_addr[0],
              sizeof(serv_addr[0])) < 0) 
              error("ERROR on binding");
     listen(sockfd1,5);
     clilen = sizeof(cli_addr);
     signal(SIGCHLD,sig_chld);
     while (1) {
         newsockfd1 = accept(sockfd1, 
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd1 < 0) 
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd1);
             dostuff(newsockfd1);
             exit(0);
         }
         else close(newsockfd1);
     } /* end of while */
     close(sockfd1);
     //close(sockfd2);
     return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
  FILE *fp,*fp1;
  int n;
  char buffer[SIZE];
  bzero(buffer,SIZE);//Initialize buffer array to zero
  n = read(sock,buffer,SIZE);//Read the type of the user -- reader or reporter
  if (n < 0) error("ERROR reading from socket");
  printf("%s\n",buffer);
  if(strcmp(buffer,"Reporter") == 0)
  {
    printf("Entered Reporter\n");
    bzero(buffer,SIZE);//Reset buffer
    n = read(sock,buffer,SIZE);//Read the type of the article into the buffer
    printf("%s\n",buffer);
    if (n < 0) error("ERROR reading from socket");
    if(strcmp(buffer,"Academic") == 0)//Branch according to the type of buffer
    {
      printf("Entered Academic\n");
      const char mode = 'a';
      fp = fopen("Academic", "a");
      if(fp == NULL)
        printf("File not created\n");
      bzero(buffer,SIZE);
      n = read(sock,buffer,SIZE);//Read the title of the article from the socket into the buffer
      if (n < 0) 
        error("ERROR reading from socket");
      printf("%s\n",buffer);
      fprintf(fp,"%s",buffer);
      fp1 = fopen(buffer, "a");
      printf("To open file and store...................\n");
      bzero(buffer,SIZE);
      n = read(sock,buffer,SIZE);//
      fprintf(fp,"%s\n",buffer);
      if (n < 0) error("ERROR reading from socket");
      printf("%s\n",buffer);
      bzero(buffer,SIZE);
      n = read(sock,buffer,SIZE);
      if (n < 0) error("ERROR reading from socket");
      fprintf(fp1, "%s\n",buffer);
      printf("%s\n",buffer);
    }
    else if(strcmp(buffer,"Non-Academic") == 0)
    {
      printf("Entered Non-Academic\n");
      const char mode = 'a';
      fp = fopen("Non-Academic", "a");
      if(fp == NULL)
          printf("File not created\n");
      bzero(buffer,SIZE);
      n = read(sock,buffer,SIZE);
      if (n < 0) error("ERROR reading from socket");
      printf("%s\n",buffer);
      fprintf(fp,"%s---->",buffer);
      fp1 = fopen(buffer, "a");
      printf("To open file and store...................\n");
      bzero(buffer,SIZE);
      n = read(sock,buffer,SIZE);
      fprintf(fp,"%s\n",buffer);
      if (n < 0) error("ERROR reading from socket");
      printf("%s\n",buffer);
      bzero(buffer,SIZE);
      n = read(sock,buffer,SIZE);
      if (n < 0) error("ERROR reading from socket");
      fprintf(fp1, "%s\n",buffer);
      printf("%s\n",buffer);
    }
   }
   else
    if(strcmp(buffer,"Reader") == 0)
    {
    printf("Entered Reader\n");
    bzero(buffer,SIZE);
    n = read(sock,buffer,SIZE);
    printf("%s\n",buffer);
    if (n < 0) error("ERROR reading from socket");
    if(strcmp(buffer,"Academic") == 0)
    {
        printf("Entered Academic\n");
        printf("Print the list of news Item..............\n");
        fp = fopen("Academic", "r");
        bzero(buffer,SIZE);
        char buffer1[100],buffer2[20];
        do{
            fscanf(fp,"%s@%s\n",buffer1,buffer2);
            //printf("%s %s\n",buffer2,buffer1);
            strcat(buffer,buffer2);
            //strcat(buffer,"------->");
            strcat(buffer,buffer1);
            strcat(buffer,"\n");
            // n = write(sock,buffer2,sizeof(buffer2));
            // if (n < 0) error("ERROR writing to socket");
            // n = write(sock,"      ",sizeof("      "));
            // if (n < 0) error("ERROR writing to socket");
            // n = write(sock,buffer2,sizeof(buffer2));
            // if (n < 0) error("ERROR writing to socket");
            // n = write(sock,"\n",sizeof("\n"));
            // if (n < 0) error("ERROR writing to socket");
        }
        while(!feof(fp));
        //printf("%s----->%s\n",buffer2,buffer1);
        n = write(sock,buffer,sizeof(buffer));
        if (n < 0) error("ERROR writing to socket");
        n = read(sock,buffer,SIZE);
        if (n < 0) error("ERROR reading from socket");
        printf("%s\n",buffer);
    }
    else
        if(strcmp(buffer,"Non-Academic") == 0)
        {
        printf("Entered Non-Academic\n");
        printf("Print the list of news Item..............");
        bzero(buffer,SIZE);
        n = read(sock,buffer,SIZE);
        if (n < 0) error("ERROR reading from socket");
        printf("%s\n",buffer);
        }
    }
   n = write(sock,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");
}
