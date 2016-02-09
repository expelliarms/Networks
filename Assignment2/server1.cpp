#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h> // For the socket (), bind () etc. functions.
#include <netinet/in.h> // For IPv4 data struct..
#include <arpa/inet.h> // For inet_pton (), inet_ntop ().
#include <signal.h>
#include <errno.h>
#include <bits/stdc++.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

#define BUFFER_CAPACITY 1024
#define PORT 10798

void sig_chld(int signo)
{
    pid_t    pid;
    int      status;
    while ( (pid = waitpid(-1, &status, WNOHANG)) > 0)
    printf("child %d terminated\n", pid);
    return;
}

int main ()
{
    int sfd,status;
    void sig_chld(int);
    char buffer[BUFFER_CAPACITY] = {'\0'};
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd == -1)
    {
      perror("SFD is -1");
      exit(EXIT_FAILURE);
    }
    printf("sfd = %d\n", sfd);
    struct sockaddr_in Saddress;
    struct sockaddr_in Caddress;
    socklen_t address_length = sizeof(struct sockaddr_in);
    memset(&Saddress, 0, address_length);
    memset(&Caddress, 0, address_length);
    Saddress.sin_family = AF_INET;
    Saddress.sin_port = htons(PORT);//Conver the port number from Host byte order to network byte order
    int status_pton;
    status_pton = inet_pton (AF_INET, "127.0.0.1", &Saddress.sin_addr);//Type conversion --form text to binary form
    if(status_pton == -1)
    {
      perror("inte_pton returned -1");
      exit(EXIT_FAILURE);
    }
    int status_bind;
    status_bind = bind (sfd, (struct sockaddr *) &Saddress, address_length);//Bind the server to an address.
    if(status_bind == -1)
    {
      perror("bind failed: returned -1");
      exit(EXIT_FAILURE);
    }
    printf ("Max connections allowed: %d\n", SOMAXCONN);
    int status_listen;
    status_listen = listen (sfd, SOMAXCONN);//Mark sfd as passive
    if (status_listen == -1)
    {
      perror ("Server: Listen failed");
      exit (1);
    }
    signal(SIGCHLD, sig_chld);
    pid_t chPID;
    int numClients = 0;
    int cfdArray[50];
    while(1)
    {
      fd_set rd, wr, er;//Set of socket descriptors
      FD_ZERO(&rd);
      FD_ZERO(&wr);
      FD_ZERO(&er);
      FD_SET(sfd, &rd);
      int N = 0;
      N = sfd;
      for(int i = 0; i < numClients; i++)
      {
        int current = cfdArray[i];
        if(current > 0)
        {
          FD_SET(current, &rd);
          FD_SET(current, &wr);
        }
        if(N < current)
          N = current;
      }
      struct timeval t;
      t.tv_sec = 10;
      t.tv_usec = 0;
      int status_select;
      status_select = select(N + 1, &rd, &wr, &er, &t);
      if(status_select < 0)
      {
        if(errno == EINTR)
          continue;
        else
          perror ("Select failed");
      }
      if (FD_ISSET (sfd, &rd))
      {
        printf("In accept\n");
        int ksd;
        ksd = accept (sfd, (struct sockaddr *) &cli_addr, &addrlen);
        if (ksd == -1)
        {
          perror ("Accept failed");
          exit (1);
        }
        //num++;
        printf("chile: %d\n", ksd);
        int flag=0;
        for(i=0;i<num;i++){
          int ccd = cfd[i];
          if(ccd == 0){
            cfd[i] = ksd;
            flag=1;
            break;
          }
        }
        if(flag==0){
          flag=1;
          cfd[num++] = ksd;
        }
        continue;
      }
    }
}
