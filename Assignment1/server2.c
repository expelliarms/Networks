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
#include <errno.h>
#define SIZE 100000
#define max(a,b) a>b?a:b
const char password[] = "123456";
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
  FILE *fpr1,*fpr2,*fprow1,*fprow2;
  fd_set rset;
  int maxfdp1,rst;
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
     ////////////////////////////////////////////////////////
    int sock, length, n;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[SIZE];

   if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }
   
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(atoi(argv[1]));
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
    FD_ZERO(&rset);
    maxfdp1 = max(sockfd1, sock) + 1;
   while (1) {
        FD_SET(sockfd1, &rset);
        FD_SET(sock, &rset);
        if((rst = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0){
            if(errno == EINTR)
                continue;
            else
                perror("select error");
        }

        if(FD_ISSET(sockfd1, &rset)){
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

      /* Handles UDP Connection from admin */
      if (FD_ISSET(sock, &rset)) 
      {
        n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);//Recieve password from the client
        if (n < 0) 
          error("recvfrom");
        write(1,"Received password as datagram: ",31);
        write(1,buf,n);
        int wrongpwdflag = 0;
        //Match passwords and set wrongpwdflag to 1 if passwords do not match
        if(strlen(password) == n - 1)
        {
          int i;
          for(i = 0; i<n-1; i++)
          {
            if(password[i] != buf[i])
            {
              wrongpwdflag = 1;
              break;
            }
          }
        }
        else
        {
          wrongpwdflag = 1;
        }
        //printf("flag = %d strlen = %d n = %d\n",wrongpwdflag,strlen(password),n);
        if(!wrongpwdflag)
        {
          //Password authentication successful send 1 as return message
          n = sendto(sock,"1\n",2,0,(struct sockaddr *)&from,fromlen);
          if (n  < 0) error("sendto");
          n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
          if (n < 0) 
            error("recvfrom");
          write(1,"Received datetime as datagram: ",31);
          write(1,buf,n);
          int i=0,j=0,k=0,deleteindexA = 0,deleteindexNA = 0;
          char refdate[20];
          for(i=0; buf[i]!= '\0'; i++)
          {
            refdate[i] = buf[i];
          }
          char new_array[1000][1000];//Full news headlines
          fpr1 = fopen("Academic","r");
          if(fpr1 == NULL)
            printf("File open error\n");
          while(fgets(new_array[i],1000,fpr1) != NULL)
          {
            for(j=0; new_array[i][j]!='@';j++);
            j++;
            char thisdate[20];
            for(k = 0; new_array[i][j+k] != '\0'; k++)
            {
              thisdate[k] = new_array[i][j+k];
            } 
            if(strcmp(thisdate,refdate)>0)
              deleteindexA++;
            //printf("date of this article ------------------- %s\n", thisdate);
              i++;
          }
          fclose(fpr1);
          fprow1 = fopen("Academic","w");
          for(j = deleteindexA; j<i; j++)
          {
            fprintf(fprow1, "%s\n", new_array[j]);
          }
          fclose(fprow1);

          fpr2 = fopen("Non-Academic","r");
          if(fpr2 == NULL)
            printf("File open error\n");
          while(fgets(new_array[i],1000,fpr2) != NULL)
          {
            for(j=0; new_array[i][j]!='@';j++);
            j++;
            char thisdate[20];
            for(k = 0; new_array[i][j+k] != '\0'; k++)
            {
              thisdate[k] = new_array[i][j+k];
            } 
            if(strcmp(thisdate,refdate)>0)
              deleteindexNA++;
              i++;
          }
          fclose(fpr2);
          fprow2 = fopen("Non-Academic","w");
          for(j = deleteindexNA; j<i; j++)
          {
            fprintf(fprow2, "%s\n", new_array[j]);
          }
          fclose(fprow2);
          n = sendto(sock,"Delete request submitted\n",15,0,(struct sockaddr *)&from,fromlen);
          if (n  < 0) error("sendto");
        }
        else
        {
          //Wrong password send zero to client
          n = sendto(sock,"0\n",2,0,(struct sockaddr *)&from,fromlen);
          if (n  < 0) error("sendto");
        }
      }
    }
    /****************** Close ****************************************/
    rst = close (sockfd1); // Close the socket file descriptor.
    if (rst == -1)
    {
        perror ("Server close failed");
        exit (1);
    }
    rst = close (sock); // Close the socket file descriptor.
    if (rst < 0)
    {
        perror ("Server close failed");
        exit (1);
    }
     return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
  FILE *fp,*fp1,*fpr,*fp2;
  int n;
  char buffer[SIZE],buffer1[SIZE];
  bzero(buffer,SIZE);
  n = read(sock,buffer,SIZE);
  if (n < 0) error("ERROR reading from socket");
  printf("%s\n",buffer);
  if(strcmp(buffer,"Reporter") == 0)
  {
    printf("Entered Reporter\n");
    bzero(buffer,SIZE);
    n = read(sock,buffer,SIZE);
    printf("%s\n",buffer);
    if (n < 0) error("ERROR reading from socket");
    if(strcmp(buffer,"Academic") == 0)
    {
      printf("Entered Academic\n");
      const char mode = 'a';
      bzero(buffer,SIZE);
      n = read(sock,buffer,SIZE);
      if (n < 0) error("ERROR reading from socket");
      bzero(buffer1,SIZE);
      n = read(sock,buffer1,SIZE);
      n = strlen(buffer);
      //printf("%s\n",buffer);
      int i = 0,j = 0;
      buffer[n-1]='@';
      for(i = n; i<n+11; j++,i++)
      {
        buffer[i] = buffer1[j];
      }
      buffer[i] = '\n';
      fp = fopen("Academic", "a");
      if(fp == NULL)
          printf("File not created\n");
      printf("Buffer Now : %s\n",buffer);
      fprintf(fp,"%s\n",buffer);
      fclose(fp);
      fp1 = fopen(buffer, "a");
      // printf("To open file and store...................\n");
      // fprintf(fp,"%s\n",buffer);
      // if (n < 0) error("ERROR reading from socket");
      //printf("%s\n",buffer);
      bzero(buffer,SIZE);
      n = read(sock,buffer,SIZE);
      if (n < 0) error("ERROR reading from socket");
      fprintf(fp1, "%s",buffer);
      fclose(fp1);
      //printf("%s",buffer);
    }
    else if(strcmp(buffer,"Non-Academic") == 0)
    {
      printf("Entered Non Academic\n");
      const char mode = 'a';
      bzero(buffer,SIZE);
      n = read(sock,buffer,SIZE);
      if (n < 0) error("ERROR reading from socket");
      bzero(buffer1,SIZE);
      n = read(sock,buffer1,SIZE);
      n = strlen(buffer);
      //printf("%s\n",buffer);
      int i = 0,j = 0;
      buffer[n-1]='@';
      for(i = n; i<n+11; j++,i++)
      {
        buffer[i] = buffer1[j];
      }
      buffer[i] = '\n';
      fp = fopen("Non-Academic", "a");
      if(fp == NULL)
          printf("File not created\n");
      printf("Buffer Now : %s\n",buffer);
      fprintf(fp,"%s\n",buffer);
      fclose(fp);
      fp1 = fopen(buffer, "a");
      // printf("To open file and store...................\n");
      // fprintf(fp,"%s\n",buffer);
      // if (n < 0) error("ERROR reading from socket");
      //printf("%s\n",buffer);
      bzero(buffer,SIZE);
      n = read(sock,buffer,SIZE);
      if (n < 0) error("ERROR reading from socket");
      fprintf(fp1, "%s",buffer);
      fclose(fp1);
      //printf("%s",buffer);
    }
  }
  else if(strcmp(buffer,"Reader") == 0)
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
        fpr = fopen("Academic", "r");//Open Academic file in read mode
        buffer[0] = '\0';
        char new_array[1000][1000];//Full news headlines
        int i=0,j=0;
        while(fgets(new_array[i],1000,fpr) != NULL)
        {
          char istr[10];
          sprintf(istr,"%d",i+1);
          strcat(buffer,istr);
            strcat(buffer,".");
            strcat(buffer,new_array[i]);
            i++;
        }
        strcat(buffer,"\n");
        fclose(fpr);
        for(i=0;i<5;++i)
        {
            printf("new_array[i] = %s\n",new_array[i]);
        }
        n = write(sock,buffer,sizeof(buffer));
        if (n < 0) error("ERROR writing to socket");
        bzero(buffer,SIZE);
        n = read(sock,buffer,SIZE);
        if (n < 0) error("ERROR reading from socket");
        //printf("%s\n",buffer);
        char to_open[1000];
        to_open[0] ='\0';
        for(i = 0; i<strlen(new_array[atoi(buffer)-1]); i++)
        {
          to_open[i] = new_array[atoi(buffer)-1][i];
        }
        to_open[i-1] = '\0';
        printf("%s\n", to_open);
        
        fp2 = fopen(to_open,"r");
        if(fp2 == NULL)
          printf("File read error\n");
        bzero(buffer,SIZE);
        buffer[0] = '\0';
        while(fgets(new_array[i],1000,fp2) != NULL)
        {
            strcat(buffer,new_array[i]);
            i++;
        }
        printf("buffere here --------------%s\n", buffer);
        fclose(fp2);
        n = write(sock,buffer,sizeof(buffer));
        if (n < 0) error("ERROR writing to socket");
    }
    else if(strcmp(buffer,"Non-Academic") == 0)
    {
      printf("Entered Non-Academic\n");
      printf("Print the list of news Item..............\n");
      fpr = fopen("Non-Academic", "r");//Open Academic file in read mode
      buffer[0] = '\0';
      char new_array[1000][1000];//Full news headlines
      int i=0,j=0;
      while(fgets(new_array[i],1000,fpr) != NULL)
      {
        char istr[10];
        sprintf(istr,"%d",i+1);
        strcat(buffer,istr);
          strcat(buffer,".");
          strcat(buffer,new_array[i]);
          i++;
      }
      strcat(buffer,"\n");
      fclose(fpr);
      for(i=0;i<5;++i)
      {
          printf("new_array[i] = %s\n",new_array[i]);
      }
      n = write(sock,buffer,sizeof(buffer));
      if (n < 0) error("ERROR writing to socket");
      bzero(buffer,SIZE);
      n = read(sock,buffer,SIZE);
      if (n < 0) error("ERROR reading from socket");
      //printf("%s\n",buffer);
      char to_open[1000];
      to_open[0] ='\0';
      for(i = 0; i<strlen(new_array[atoi(buffer)-1]); i++)
      {
        to_open[i] = new_array[atoi(buffer)-1][i];
      }
      to_open[i-1] = '\0';
      printf("%s\n", to_open);
      
      fp2 = fopen(to_open,"r");
      if(fp2 == NULL)
        printf("File read error\n");
      bzero(buffer,SIZE);
      buffer[0] = '\0';
      while(fgets(new_array[i],1000,fp2) != NULL)
      {
          strcat(buffer,new_array[i]);
          i++;
      }
      printf("buffere here --------------%s\n", buffer);
      fclose(fp2);
      n = write(sock,buffer,sizeof(buffer));
      if (n < 0) error("ERROR writing to socket");
    }
  }
   n = write(sock,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");
  
}
