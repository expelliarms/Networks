#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define SIZE 100000
void error(const char *msg)
{
  perror(msg);
  exit(0);
}
int main(int argc, char *argv[])
{
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[SIZE],c,buffer1[SIZE];
  int i = 0;
  if (argc < 4) {
     fprintf(stderr,"usage %s hostname port filename\n", argv[0]);
     exit(0);
  }
  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");
  server = gethostbyname(argv[1]);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
  serv_addr.sin_port = htons(portno);

  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    error("ERROR connecting");
  //printf("Choose an Option\n1)Reporter\n2)Reader\n3)exit\n");
  printf("Please Choose an appropriate option : \n");
  printf("1)Show availability\n2)Book Tickets from files\n3)exit\n");
  bzero(buffer,SIZE);
  //fgets(buffer,255,stdin);
  scanf("%s",buffer);
  //n = write(sockfd,buffer,strlen(buffer));
  //if (n < 0)
    //   error("ERROR writing to socket");
  //bzero(buffer,256);
  //n = read(sockfd,buffer,255);
  ///if (n < 0)
    //   error("ERROR reading from socket");
   //else
  if(strcmp(buffer,"1") == 0)
  {
    write(sockfd,"1",SIZE);
    read(sockfd,buffer,SIZE);
    printf("%s",buffer);
  }
  else if(strcmp(buffer,"2") == 0)
  {
    /*n = write(sockfd, "2", SIZE);
    n = read(sockfd, buffer, SIZE);
    printf("%s",buffer);
    sleep(5);*/
    /*FILE *f = fopen(argv[3],"r");
    while(fgets(buffer,SIZE,f))
    {
      //n = write(sockfd, buffer, SIZE);
      //n = read(sockfd, buffer1, SIZE);
      printf("%s",buffer);
      //sleep(5);
    }*/
    char *file_contents;
    long input_file_size;
    FILE *input_file = fopen(argv[3], "rb");
    fseek(input_file, 0, SEEK_END);
    input_file_size = ftell(input_file);
    rewind(input_file);
    file_contents = malloc(input_file_size * (sizeof(char)));
    fread(file_contents, sizeof(char), input_file_size, input_file);
    fclose(input_file);
    printf("%s\n", file_contents);
    write(sockfd,file_contents,SIZE);
    read(sockfd,buffer,SIZE);
    printf("%s\n", buffer);
  }
  else
  {
   	printf("Exiting\n");
   	close(sockfd);
    exit(1);
  }
  //printf("%s\n",buffer);
  close(sockfd);
  return 0;
}
