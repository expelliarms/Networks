#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#define TRUE   1
#define FALSE  0
#define SIZE 100000
int n;
char buffer[SIZE];
char buffer1[SIZE];
FILE *fp,*fp1,*fpr,*fpr1;

void error(const char *msg)
{
  perror(msg);
  exit(0);
}

int process(char array[])
{
    return 0;
}

int allocator(char * request, char * returnmsg)
{
  int status = 0,i,j,k;
  /*---------------------- Parse the input string ----------------------*/
  char *pt, *pt1, *pt2;
  char *trainClass, *ages, *preferences;
  trainClass = (char *)malloc(10*sizeof(char));
  ages = (char *)malloc(50*sizeof(char));
  preferences = (char *)malloc(50*sizeof(char));
  int personid, trainNum, numPass;
  pt = strtok (request,",");
  for (i = 0;pt != NULL; i++)
  {
    printf("%s\n", pt);

    if(i == 0)
      personid = atoi(pt);
    if(i == 1)
      trainNum = atoi(pt);
    if(i == 2)
      trainClass = pt;
    if(i == 3)
      numPass = atoi(pt);
    if(i == 4)
      preferences = pt;
    if(i == 5)
      ages = pt;
    pt = strtok (NULL, ",");
  }
  char **eachprefs;
  eachprefs = (char **)malloc(numPass*sizeof(char *));
  int *eachages;
  eachages = (int *)malloc(numPass*sizeof(int));
  printf("personid = %d\t trainNum = %d\t trainClass = %s\t numPass = %d\t preferences = %s\t ages = %s\n", personid, trainNum, trainClass, numPass, preferences, ages);
  pt1 = strtok(preferences, "-");
  for(j = 0;pt1 != NULL; j++)
  {
    eachprefs[j] = (char *)malloc(3*sizeof(char));
    sprintf(eachprefs[j],"%s", pt1);
    printf("%s\n", eachprefs[j]);
    pt1 = strtok(NULL, "-");
  }
  pt2 = strtok(ages, "-");
  for(j = 0;pt2 != NULL; j++)
  {
    eachages[j] =  atoi(pt2);
    printf("%d\n", eachages[j]);
    pt2 = strtok(NULL, "-");
  }
  /*---------------------- ENd of parser ----------------------*/

  /*--------------- Check and update availability file ---------------*/
  char *filename1 = (char *)malloc(10*sizeof(char));
  sprintf(filename1, "a%d",trainNum);
  int totalAC, bookedAC, avlAC, totalSL, bookedSL, avlSL;
  FILE *fp1 = fopen(filename1, "r");
  if(trainNum == 12321)
  {
    fscanf(fp1,"%d%d%d%d%d%d", &totalAC, &bookedAC, &avlAC, &totalSL, &bookedSL, &avlSL);
    fclose(fp1);
    printf("Before booking: totalAC = %d\t bookedAC = %d\t avlAC = %d\t totalSL = %d\t bookedSL = %d\t avlSL = %d\n", totalAC, bookedAC, avlAC, totalSL, bookedSL, avlSL);
    if(!strcmp(trainClass,"AC"))
    {
      if(numPass > avlAC)
      {
        returnmsg = "Requested number of seats Not Available";
        status = 0;
      }
      else
      {
        FILE *fp2 = fopen(filename1,"w");
        {
          fprintf(fp2,"%d %d %d %d %d %d", totalAC, bookedAC + numPass, avlAC - numPass, totalSL, bookedSL, avlSL);
        }
        fclose(fp2);
        status = 1;
      }
    }
    else if(!strcmp(trainClass,"Sleeper"))
    {
      if(numPass > avlSL)
      {
        returnmsg = "Requested number of seats Not Available";
        status = 0;
      }
      else
      {
        FILE *fp2 = fopen(filename1,"w");
        {
          fprintf(fp2,"%d %d %d %d %d %d", totalAC, bookedAC , avlAC , totalSL, bookedSL + numPass, avlSL - numPass);
        }
        fclose(fp2);
        status = 1;
      }
    }
    else
    {
      returnmsg = "Invalid class";
      status = -1;
    }
    printf("After booking: totalAC = %d\t bookedAC = %d\t avlAC = %d\t totalSL = %d\t bookedSL = %d\t avlSL = %d\n",totalAC, bookedAC , avlAC , totalSL, bookedSL + numPass, avlSL - numPass);
  }
  else if(trainNum == 12301)
  {
    fscanf(fp1,"%d%d%d", &totalAC, &bookedAC, &avlAC);
    fclose(fp1);
    printf("Before booking: totalAC = %d\t bookedAC = %d\t avlAC = %d\n", totalAC, bookedAC, avlAC);
    if(!strcmp(trainClass,"AC"))
    {
      if(numPass > avlAC)
      {
        returnmsg = "Requested number of seats Not Available";
        status = 0;
      }
      else
      {
        FILE *fp2 = fopen(filename1,"w");
        {
          fprintf(fp2,"%d %d %d", totalAC, bookedAC + numPass, avlAC - numPass);
        }
        fclose(fp2);
        status = 1;
      }
    }
    else
    {
      returnmsg = "Invalid class";
      status = -1;
    }
    printf("After booking: totalAC = %d\t bookedAC = %d\t avlAC = %d\n",  totalAC, bookedAC + numPass, avlAC - numPass);
  }
  else
  {
    returnmsg = "Invalid Train number";
    status = -1;
  }

  /*----------- Done Check and update of availability file -----------*/

  /* If availability test is passed with status 1 do the actual seat allotment */
  if(status == 1)
  {
    char *filename = (char *)malloc(10*sizeof(char));
    sprintf(filename, "t%d", trainNum);
    char **allotmentmatrix;
    int totalSeats;
    if(trainNum == 12321)
    {
      totalSeats = totalAC + totalSL;
    }
    else if(trainNum == 12301)
    {
      totalSeats = totalAC;
    }
    allotmentmatrix = (char **)malloc(totalSeats*sizeof(char*));
    for(i = 0; i<totalSeats; i++)
    {
      allotmentmatrix[i] = (char *)malloc(10*sizeof(char));
    }
    char *pt3;
    i = 0;
    FILE *fp = fopen(filename,"r");
    while(fgets(allotmentmatrix[i],10,fp) != NULL)
    {
      //printf("%s",allotmentmatrix[i]);
      i++;
    }
    fclose(fp);
    for(i = 0; i< totalSeats; i++)
    {
      //printf("%s",allotmentmatrix[i]);
    }
    int* availability = (int*)malloc(totalSeats*sizeof(int));
    for(j = 0; j < totalSeats; j++)
    {
      availability[j] = allotmentmatrix[j][7] - '0';
    }
    for(i = 0; i < numPass; i++)
    {
      while (1) {
        int seatalloted = 0;
        if(trainNum == 12321)
        {
          if(!strcmp(trainClass,"AC"))
          {
            if(!strcmp(eachprefs[i],"LB"))
            {
              j = 0;
              k = 0;
              while(j<totalSeats && j<72*3)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/72 + 1,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/72 + 1,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                if(k == 0)
                {
                  k = 1;
                  j += 3;
                }
                else
                {
                  k = 0;
                  j += 5;
                }
              }
            }
            if(!strcmp(eachprefs[i],"MB"))
            {
              j = 1;
              k = 0;
              while(j<totalSeats && j<72*3)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/72 + 1,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/72 + 1,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                if(k == 0)
                {
                  k = 1;
                  j += 3;
                }
                else
                {
                  k = 0;
                  j += 5;
                }
              }
            }
            if(!strcmp(eachprefs[i],"UB"))
            {
              j = 2;
              k = 0;
              while(j<totalSeats && j<72*3)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/72 + 1,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/72 + 1,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                if(k == 0)
                {
                  k = 1;
                  j += 3;
                }
                else
                {
                  k = 0;
                  j += 5;
                }
              }
            }
            if(!strcmp(eachprefs[i],"SL"))
            {
              j = 6;
              while(j<totalSeats && j<72*3)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/72 + 1,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/72 + 1,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                j += 8;
              }
            }
            if(!strcmp(eachprefs[i],"SU"))
            {
              j = 7;
              while(j<totalSeats && j<72*3)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/72 + 1,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/72 + 1,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                j += 8;
              }
            }
            if(!strcmp(eachprefs[i],"NA"))
            {
              j = 0;
              while(j<totalSeats && j<72*3)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/72 + 1,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/72 + 1,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                j++;
              }
            }
          }
          else if(!strcmp(trainClass,"Sleeper"))
          {
            if(!strcmp(eachprefs[i],"LB"))
            {
              j = 72*3;
              k = 0;
              while(j<totalSeats)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"S%02d,%02d,%d\n",j/72 + 1 - 3,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: S%d, %d\n",i + 1,j/72 + 1 - 3,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                if(k == 0)
                {
                  k = 1;
                  j += 3;
                }
                else
                {
                  k = 0;
                  j += 5;
                }
              }
            }
            if(!strcmp(eachprefs[i],"MB"))
            {
              j = 72*3 + 1;
              k = 0;
              while(j<totalSeats)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"S%02d,%02d,%d\n",j/72 + 1 - 3,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: S%d, %d\n",i + 1,j/72 + 1 - 3,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                if(k == 0)
                {
                  k = 1;
                  j += 3;
                }
                else
                {
                  k = 0;
                  j += 5;
                }
              }
            }
            if(!strcmp(eachprefs[i],"UB"))
            {
              j = 72*3 + 2;
              k = 0;
              while(j<totalSeats)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"S%02d,%02d,%d\n",j/72 + 1 - 3,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: S%d, %d\n",i + 1,j/72 + 1 - 3,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                if(k == 0)
                {
                  k = 1;
                  j += 3;
                }
                else
                {
                  k = 0;
                  j += 5;
                }
              }
            }
            if(!strcmp(eachprefs[i],"SL"))
            {
              j = 72*3 + 6;
              while(j<totalSeats)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"S%02d,%02d,%d\n",j/72 + 1 - 3,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: S%d, %d\n",i + 1,j/72 + 1 - 3,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                j += 8;
              }
            }
            if(!strcmp(eachprefs[i],"SU"))
            {
              j = 72*3 + 7;
              while(j<totalSeats)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"S%02d,%02d,%d\n",j/72 + 1 - 3,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: S%d, %d\n",i + 1,j/72 + 1 - 3,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                j += 8;
              }
            }
            if(!strcmp(eachprefs[i],"NA"))
            {
              j = 72*3;
              while(j<totalSeats)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"S%02d,%02d,%d\n",j/72 + 1 - 3,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: S%d, %d\n",i + 1,j/72 + 1 - 3,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                j++;
              }
            }
          }
        }
        else if(trainNum == 12301)
        {
          if(!strcmp(eachprefs[i],"LB"))
          {
            j = 0;
            k = 0;
            while(j<totalSeats)
            {
              if(availability[j] == 0)
              {
                availability[j] = 1;
                sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/54 + 1,j%54 + 1,1);
                sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/54 + 1,j%54 + 1);
                seatalloted = 1;
                break;
              }
              if(k == 0)
              {
                k = 1;
                j += 2;
              }
              else
              {
                k = 0;
                j += 4;
              }
            }
          }
          if(!strcmp(eachprefs[i],"UB"))
          {
            j = 1;
            k = 0;
            while(j<totalSeats)
            {
              if(availability[j] == 0)
              {
                availability[j] = 1;
                sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/54 + 1,j%54 + 1,1);
                sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/54 + 1,j%54 + 1);
                seatalloted = 1;
                break;
              }
              if(k == 0)
              {
                k = 1;
                j += 2;
              }
              else
              {
                k = 0;
                j += 4;
              }
            }
          }
          if(!strcmp(eachprefs[i],"SL"))
          {
            j = 4;
            while(j<totalSeats)
            {
              if(availability[j] == 0)
              {
                availability[j] = 1;
                sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/54 + 1,j%54 + 1,1);
                sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/54 + 1,j%54 + 1);
                seatalloted = 1;
                break;
              }
              j += 6;
            }
          }
          if(!strcmp(eachprefs[i],"SU"))
          {
            j = 5;
            while(j<totalSeats)
            {
              if(availability[j] == 0)
              {
                availability[j] = 1;
                sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/54 + 1,j%54 + 1,1);
                sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/54 + 1,j%54 + 1);
                seatalloted = 1;
                break;
              }
              j += 6;
            }
          }
          if(!strcmp(eachprefs[i],"NA"))
          {
            j = 0;
            while(j<totalSeats)
            {
              if(availability[j] == 0)
              {
                availability[j] = 1;
                sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/54 + 1,j%54 + 1,1);
                sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/54 + 1,j%54 + 1);
                seatalloted = 1;
                break;
              }
              j++;
            }
          }
        }
        if(seatalloted == 1)
          break;
        else{
          eachprefs[i] = "NA";
          continue;
        }
      }
    }
    FILE *fpw = fopen(filename,"w");
    for(i = 0; i < totalSeats; i++)
    {
      fprintf(fpw,"%s", allotmentmatrix[i]);
    }
    fclose(fpw);
  }
  /*------------- End of allotment ---------- */
  //printf("%s", returnmsg);
  return status;
}

int main(int argc , char *argv[])
{
   int PORT = atoi(argv[1]);
   int opt = TRUE;
    int master_socket , addrlen , new_socket , client_socket[300] , max_clients = 300 , activity, i , valread , sd;
    int max_sd;
    struct sockaddr_in address;


    //set of socket descriptors
    fd_set readfds;

    //a message
    char message[1024];

    strcpy(message,"Hi\n");

    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);

    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");

    while(TRUE)
    {
        //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }

        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

            //send new connection greeting message
            // //n = write(new_socket, message, strlen(message));
            // if(n<0)
            // {
            //     error("Error Writing to socket.");
            // }
            //puts("Welcome message sent successfully");

            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++)
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
                    break;
                }
            }
        }

        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds))
            {
                //Check if it was for closing , and also read the incoming message
                if ((valread = read( sd , buffer, SIZE)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[i] = 0;
                }
                else
                {
                    char status[SIZE] ;
                    //set the string terminating NULL byte on the end of the data read
                    buffer[valread] = '\0';
                    if(strcmp(buffer,"1") == 0)
                    {
                          pid_t pid = fork();
                         if(pid  == 0)
                         {
                            fpr = fopen("a12321", "r");//Open Availability file in read mode
                            fpr1 = fopen("a12301", "r");//Open Availability file in read mode
                            buffer[0] = '\0';
                            int i=0,j=0,totAC1,BookedAC1,AvaiAC1,totSL1,BookedSL1,AvaiSL1,totAC2,BookedAC2,AvaiAC2;
                            fscanf(fpr, "%d %d %d %d %d %d",&totAC1,&BookedAC1,&AvaiAC1,&totSL1,&BookedSL1,&AvaiSL1);
                            fscanf(fpr1, "%d %d %d",&totAC2,&BookedAC2,&AvaiAC2);
                            sprintf(buffer,"Train Number 12321:\nTotalAC = %d,BookedAC = %d,AvaiAC = %d\nTotalSL = %d,BookedSL = %d,AvaiSL= %d\nTrain Number 12301:\nTotalAC = %d,BookedAC = %d,AvaiAC = %d\n",totAC1,BookedAC1,AvaiAC1,totSL1,BookedSL1,AvaiSL1,totAC2,BookedAC2,AvaiAC2);
                            fclose(fpr);
                            fclose(fpr1);
                            n = write(sd,buffer,SIZE);
                            if (n < 0) error("ERROR writing to socket");
                         }
                         else
                         {
                           wait(&status);
                         }
                    }
                    else
                    {
                      printf("%s\n",buffer );
                      char * curLine = buffer;
                      buffer1[0] = '\0';
                      i = 1;
                      while(curLine)
                      {
                          char * nextLine = strchr(curLine, '\n');
                          if (nextLine) *nextLine = '\0';  // temporarily terminate the current line
                          printf("curLine=[%s]\n", curLine);
                          char returnmsg[SIZE] = "\0";
                          int seatstatus;
                          sleep(2);
                          if(strcmp(curLine,"") != 0)
                          {
                            sprintf(buffer1 + strlen(buffer1),"Ticket #%d\n", i);
                            seatstatus = allocator(curLine,returnmsg);
                          }
                          printf("%s\n", returnmsg);
                          curLine = nextLine ? (nextLine+1) : NULL;
                          sprintf(buffer1 + strlen(buffer1),returnmsg);
                          i++;
                      }
                      n = write(sd,buffer1,SIZE);
                      if (n < 0) error("ERROR writing to socket");
                      continue;
                    }
                    /*
                    else
                    {
                        buffer[valread] = '\0';
                        int n = write(sd, "Yoo", SIZE);
                        printf("%s\n",buffer);
                    }*/
                }
            }
        }
    }
    return 0;
}
