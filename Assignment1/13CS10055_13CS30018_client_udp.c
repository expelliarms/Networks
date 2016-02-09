/* UDP client in the internet domain */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void error(const char *);
int main(int argc, char *argv[])
{
  int sock, n;
  unsigned int length;
  struct sockaddr_in server, from;
  struct hostent *hp;
  char buffer[256];
  if (argc != 3) 
  { 
    printf("Usage: server port\n");
    exit(1);
  }
  sock = socket(AF_INET, SOCK_DGRAM, 0);//Socket in datagram mode -- UDP
  if (sock < 0) 
    error("socket error");
  server.sin_family = AF_INET;//AF_INET for IPv4
  hp = gethostbyname(argv[1]);//Get the host object from the name entered
  if (hp == 0) error("Unknown host");
  bcopy((char *)hp->h_addr,(char *)&server.sin_addr,hp->h_length);
  server.sin_port = htons(atoi(argv[2]));//host to network 
  length = sizeof(struct sockaddr_in);//Size of sockaddr_in 
  printf("Please enter the password: ");
  bzero(buffer,256);//Initialize to zero
  fgets(buffer,255,stdin);//Get the input line

  n = sendto(sock,buffer,strlen(buffer),0,(const struct sockaddr *)&server,length);//Send the password string to the server
  if (n < 0) error("Sendto error");
  n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);//Recieve the response of authentication
  if (n < 0) error("recvfrom error");

  if(buffer[0] == '1')
  {
    //Authentiction successful
    int YYYY = 0,MM = 0,DD = 0,hh = 0,mm = 0,ss = 0;
    printf("Give a cutoff date time. Format: YYYY-MM-DD hh:mm:ss\n");
    bzero(buffer,256);//Clear buffer
    fgets(buffer,255,stdin);//Get date time input
    sscanf( buffer, "%d-%d-%d %d:%d:%d", &YYYY, &MM, &DD, &hh,&mm,&ss );
    //printf("%d-%d-%d %d:%d:%d\n", YYYY, MM, DD, hh,mm,ss);
    if(YYYY == 0 || MM == 0 || DD == 0)
    {
      printf("Invalid date format. Exiting...\n");
    }
    else//If date format is valid send it to the server
    {
      n = sendto(sock,buffer,strlen(buffer),0,(const struct sockaddr *)&server,length);//Send the password string to the server
      if (n < 0) error("Sendto error");
      bzero(buffer,256);//Clear buffer
      n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);//Recieve the response of authentication
      if (n < 0) error("recvfrom error");
      write(1,buffer,n);//Print the response message
    }
  }
  else
  {
    //Authentication failed
    printf("Authentication Failed. Exiting...\n");
  }
  close(sock);//Close socket
  return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
