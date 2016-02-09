#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <time.h>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;
int portno = 23466;
#define BUF_SIZE 1000
#define lld long int 

int type;
int main(){
	int rst,cfd,i,j;
	int sfd,n,udpfd;
	sfd = socket( AF_INET, SOCK_STREAM, 0);
	if(sfd == -1){
		perror ("Client: socket error");
		exit(1);
	}
	printf("Socket fd = %d\n", sfd);

    struct sockaddr_in srv_addr; // Addresses of the server and the client.
    socklen_t addrlen = sizeof (struct sockaddr_in);
	memset(&srv_addr, 0, addrlen);

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(portno);


	rst = inet_pton(AF_INET,"127.0.0.1", &srv_addr.sin_addr);

	if (rst <= 0)
    {
        perror ("Client Presentation to network address conversion.\n");
        exit (1);
    } 
    rst = connect(sfd, (struct sockaddr *) &srv_addr,addrlen);
    if(rst == -1){
        perror ("Client: Connect failed.");
        exit (1);
    }
    printf("success\n");

    while(1){
        printf("1) Current Status of trains\n");
        printf("2) Book Ticket\n");
        printf("3) Exit\n");
        scanf("%d",&i);
        if(i==3){
            char typ[BUF_SIZE]="#3";
            write(sfd,typ,2);
            rst = close (sfd);
            if(rst == -1){
                perror("Client close failed\n");
                exit (1);
            }
            return 0;
        }
        if(i==1){
            char typ[BUF_SIZE]="#1";
            write(sfd,typ,2);
            /*read(sfd,typ,BUF_SIZE);
            cout << typ << endl;*/
        }
        if(i==2)
            break;
    }

    ifstream file("Booking.csv");
    string line;
    while(getline(file, line)) 
    {
        std::size_t prev = 0, pos;
        char typ[BUF_SIZE];//="#2";
        //write(sfd,typ,2);
        int no=0;
        string typical="#2";
        while ((pos = line.find_first_of(" ',", prev)) != std::string::npos)
        {
            if (pos > prev)
            {
                if(no==0)
                    typical=typical+line.substr(prev, pos-prev);
                    //temp.pasid=atoi(line.substr(prev, pos-prev).c_str());
                else if(no==1)
                    typical=typical+"#"+line.substr(prev, pos-prev);
                    //temp.trano=atoi(line.substr(prev, pos-prev).c_str());
                else if(no==2)
                    typical=typical+"#"+line.substr(prev, pos-prev);
                    //temp.cotype=line.substr(prev, pos-prev);
                else if(no==3)
                    typical=typical+"#"+line.substr(prev, pos-prev);
                    //temp.numberth=atoi(line.substr(prev, pos-prev).c_str());
                else if(no==4)
                    typical=typical+"#"+line.substr(prev, pos-prev);
                    //temp.pref=line.substr(prev, pos-prev);
                no++;
            }
            prev = pos+1;
        }
        if (prev < line.length())
            typical=typical+"#"+line.substr(prev, std::string::npos);
            //temp.ages=line.substr(prev, std::string::npos);
        struct timeval ts;
        gettimeofday(&ts, NULL);
        lld l1 = ts.tv_sec ;
        //string srt = "#21#12321#AC#4#SL­-SU-­LB-­LB#17­-43-­22-­78";
        //srt = srt+"#";
        std::ostringstream stm ;
        stm << l1;
        typical = typical+"#"+stm.str();
        //srt = srt+stm.str();
        //string srt = ((("1"+"#")+(("12321"+"#")+(("AC"+"#")+(("4"+"#")+("SL­-SU-­LB-­LB"+"#")))))+"17­-43-­22-­78");
        cout << typical << " yoho\n";
        strcpy(typ,typical.c_str());
        rst = write(sfd,typ,sizeof(typ));
        if(rst == 0){
            cout << " error" << endl;
        }
        cout << "Meow" << endl;

        rst = read(sfd,typ,sizeof(typ));
        printf("%s\n", typ);
    }
    rst = close (sfd);
    if(rst == -1){
    	perror("Client close failed\n");
    	exit (1);
    }
    return 0;
}