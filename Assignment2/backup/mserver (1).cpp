#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h> // For the socket (), bind () etc. functions.
#include <netinet/in.h> // For IPv4 data struct..
#include <string.h> // For memset.
#include <arpa/inet.h> // For inet_pton (), inet_ntop ().
#include <signal.h>
#include <errno.h>
#include <bits/stdc++.h>
#include <string>
#include <sys/wait.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

int port_numtcp = 23466;
#define BUF_SIZE 1000
#define pasid first.first.first
#define trano first.first.second
#define cotype first.second.first
#define numberth first.second.second
#define pref second.first.first
#define ages second.first.second
#define timst second.second.first
#define socksen second.second.second
#define mp make_pair
#define pb push_back
#define lld long long int
#define bpair pair<pair<pair<int,int>,pair<string,int> >,pair<pair<string,string>,pair<lld,int> > >

int train[3][13][72];
int rajAC[13][54],supAC[3][72],supSleep[12][72];
vector<pair<int,int> > booked;
vector<pair<string,int> > vsend;
class compQueue{    
public:
    bool operator()(bpair &p1,bpair &p2){
        if(p1.timst > p2.timst)
            return true;
        else if(p1.timst < p2.timst)
            return false;
        else if(p1.numberth > p2.numberth)
            return false;
        else if(p1.numberth < p2.numberth)
            return true;
        else
            return true;
    }
};

priority_queue<bpair ,vector<bpair >,compQueue > pq;

int tot=0;

void sig_chld(int signo)
{
    pid_t    pid;
    int      stat;

    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
    printf("child %d terminated\n", pid);
    return;
}

void intitialise_seats(){
    char path[100],path1[100];
    getcwd(path,sizeof(path));
    strcpy(path1,path);
    strcat(path1,"/superfast_ac.csv");
    ifstream infile(path1);
    char line[1000];
    vector<string> v;
    int i=0,j=0;
    while(infile.getline(line,sizeof(line),',')){
        //cout << line << endl;
        // v.push_back(line);
        if(i<3){
            //cout << i << " kola "<< j <<"\n";
            supAC[i][j]=stoi(string(line));
            if(j>70){
                j=0;
                i++;
            }
            else
                j++;
        }
    }
    //cout << "jola\n";
    infile.close();
    strcpy(path1,path);
    strcat(path1,"/superfast_sleeper.csv");
    ifstream infile1(path1);
    i=0,j=0;
    while(infile1.getline(line,sizeof(line),',')){
        //cout << line << endl;
        // v.push_back(line);
        if(i<12){
            //cout << i << " meoto"<< j <<"\n";
            supSleep[i][j]=stoi(string(line));
            if(j>70){
                j=0;
                i++;
            }
            else
                j++;

        }
    }
    infile1.close();
    strcpy(path1,path);
    strcat(path1,"/rajdhani_ac.csv");
    ifstream infile2(path1);
    i=0,j=0;
    while(infile2.getline(line,sizeof(line),',')){
        //cout << line << endl;
        // v.push_back(line);
        if(i<13){    
            //cout << i << " kola " << j << endl;
            rajAC[i][j]=stoi(string(line));
            if(j>52){
                j=0;
                i++;
            }
            else
                j++;
        }
    }
    infile2.close();
    /*for(i=0;i<3;i++){
        for(j=0;j<72;j++){
            cout << supAC[i][j] << " ";
        }
        cout << endl;
    }*/
}

void display(){
    char path[100];
    getcwd(path,sizeof(path));
    strcat(path,"/traininfo.csv");
    ifstream infile(path);
    char line[1000];
    vector<string> v;
    while(infile.getline(line,sizeof(line),',')){
        //cout << line << endl;
        v.push_back(line);
    }
    infile.close();
    /*cout << v.size() << endl;
    for(vector<string>::iterator io=v.begin();io!=v.end();io++)
        cout << *io << endl;*/
    cout << "Train\t\t\t#seats\t\t\t#seats"<<endl;
    cout << "\t\tTotal[Booked/Available]\t   Total[Booked/Available]"<<endl;
    cout << "\t\t\t(AC)\t\t\t(Sleeper)"<<endl;
    cout << v[0] << " Exp\t   "<<v[1]<<"["<< v[2]<<"/"<<stoi(v[1])-stoi(v[2])<<"]\t\t\t" ;
    cout << v[3] << "["<< v[4]<<"/"<<stoi(v[3])-stoi(v[4])<<"]" << endl;
    cout << v[5] << " Exp\t   "<<v[6]<<"["<< v[7]<<"/"<<stoi(v[6])-stoi(v[7])<<"]\t\t\t" ;
    cout << "-" << endl;
}


int noempty(int a[])
{
    int no=0;
    for(int i=0;i<72;i++)
        if(a[i]==0)
            no++;
    return no;
}
string itos(int a)
{
    string result;
    stringstream convert; 
    convert << a;
    result = convert.str();
    return result;
}
int isther(int i,int seats,int coach[])
{
    
    int ind=0;
    if(seats==72)
    {
        if(i==0)
            ind=2;
        else if(i==1)
            ind=0;
        else if(i==4)
            ind=1;
        else if(i==3)
            ind=7;
        else if(i==2)
            ind=6;
        int s=0;
        while(s<72)
        {
            if(coach[s+ind]==0)
            {
                return (s+ind);
            }
            else if(coach[s+ind+3]==0)
            {
                return (s+ind+3);
            }
            s+=8;
        }
    }
    else
    {
        if(i==0)
            ind=1;
        else if(i==1)
            ind=0;
        else if(i==4)
            ind=1;
        else if(i==3)
            ind=5;
        else if(i==2)
            ind=4;
        int s=0;
        while(s<54)
        {
            if(coach[s+ind]==0)
            {
                return (s+ind);
            }
            else if(coach[s+ind+2]==0)
            {
                return (s+ind+2);
            }
            s+=6;
        }
    }
    return -1;
}
int traincheck(int trainn[][72],int coaches,int seats,int pr[],int total,int ttype)
{
    vector <pair<int,int> > modified;
    for(int i=0;i<5;i++)
    {
        int j=0;
            while(pr[i]>0 && j<coaches)
            {
                int c = isther(i,seats,trainn[j]);
                if(c!=-1)
                {
                    pr[i]--;
                    train[ttype][j][c]=1;
                    modified.pb(mp(j,c));
                }
                else
                {
                    j++;
                }
            }
            if(pr[i]!=0)
            {
                for(int p=0;p<modified.size();p++)
                    train[ttype][modified[i].first][modified[i].second]=0;
                return -1;
            }
    }
    for(int i=0;i<modified.size();i++)
        booked.pb(modified[i]);
    return 1;
}
int continuousbooking(int trainn[][72],int coaches,int seats,int total)
{
    for(int i=0;i<coaches;i++)
    {
        for(int j=0;j<seats-total+1;j++)
        {
            bool av=true;
            for(int k=j;k<j+total;k++)
            {
                if(trainn[i][k]==1)
                {
                    av=false;
                    break;
                }
            }
            if(av==true)
            {
                for(int p=0;p<total;p++)
                    booked.pb(mp(i,j+p));
                return 1;
            }
        }
    }
    return -1;
}
int randombooking(int trainn[][72],int coaches,int seats,int total)
{
    vector<pair<int,int> > tmp;
    for(int i=0;i<coaches;i++)
    {
        for(int j=0;j<seats;j++)
        {
            if(trainn[i][j]==0){
                trainn[i][j]=1;
                tmp.pb(mp(i,j));
            }
            if(tmp.size()==total)
            {
                for(int i=0;i<total;i++)
                    booked.pb(tmp[i]);
                return 1;
            }
        }
    }
    return -1;
}
int* coachcheck(int coach[],int seats,int pr[],int total)
{
    int *allotted=(int*)calloc(total,sizeof(int));
    int indarr[total];
    int tmp[5];
    int aind=0;
    for(int i=0;i<5;i++)
        tmp[i]=pr[i];
    if(seats==72)
    {
        for(int i=0;i<5;i++)
        {
            bool side=false;
            int s=0;
            int ind=0;
            if(i==0)
                ind=2;
            else if(i==1)
                ind=0;
            else if(i==4)
                ind=1;
            else if(i==3)
            {
                side=true;
                ind=7;
            }
            else if(i==2)
            {
                side=true;
                ind=6;
            }

            while(tmp[i]>0 && s<72)
            {
                if(coach[s+ind]==0)
                {
                    allotted[aind++]=s+ind;
                    tmp[i]--;
                }
                if(tmp[i]==0 || side==true)
                {
                    s+=8;
                    continue;
                }
                if(coach[s+3+ind]==0)
                {
                    allotted[aind++]=s+3+ind;
                    tmp[i]--;
                }
                s+=8;
            }

        }
    }
    else
    {
        for(int i=0;i<4;i++)
        {
            bool side=false;
            int s=0;
            int ind=0;
            if(i==0)
                ind=1;
            else if(i==1)
                ind=0;
            else if(i==3)
            {
                side=true;
                ind=5;
            }
            else if(i==2)
            {
                side=true;
                ind=4;
            }

            while(tmp[i]>0 && s<54)
            {
                if(coach[s+ind]==0)
                {
                    allotted[aind++]=s+ind;
                    tmp[i]--;
                }
                if(tmp[i]==0 || side==true)
                {
                    s+=6;
                    continue;
                }
                if(coach[s+3+ind]==0)
                {
                    allotted[aind++]=s+3+ind;
                    tmp[i]--;
                }
                s+=6;
            }
        }
    }
    if(aind<total)
    {
        allotted[0]=-1;
    }
    else
    {
        for(int i=0;i<total;i++)
            coach[allotted[i]]=1;
    }
    return allotted;
}
int booking(bpair book)
{
    int ttype,coaches,seats;
    if(book.trano==12301)
    {
        ttype=0;
        coaches=13;
        seats=54;
    }
    else if(book.trano==12321)
    {
        if(book.cotype=="AC")
        {
            ttype=1;
            coaches=3;
        }
        else
        {
            ttype=2;
            coaches=12;
        }
        seats=72;
    }
    else
    {
        printf("No such train found\n");
        return -1;
    }

    istringstream iss(book.pref);
    std::size_t found;
    std::string token;
    token=book.pref;
    found = token.find("NA");
    bool nopref=false;
    int bpr[5]; // up,lb,sl,su,mb;
    if(found!=std::string::npos)
    {
        nopref=true;
    }
    else
    {
        vector<string> preff;
        memset(bpr,0,sizeof(bpr));
        while (std::getline(iss, token, '-'))
        {   
            found = token.find("UB");
            if(found!=std::string::npos)
                bpr[0]++;
            found = token.find("LB");
            if(found!=std::string::npos)
                bpr[1]++;
            found = token.find("SL");
            if(found!=std::string::npos)
                bpr[2]++;
            found = token.find("SU");
            if(found!=std::string::npos)
                bpr[3]++;
            found = token.find("MB");
            if(found!=std::string::npos)
                bpr[4]++;
        }
    }
    int tcheck;
    if(!nopref)
    {
        int acoach=-1;
        for(int i=0;i<coaches;i++)
        {
            int *tickets = coachcheck(train[ttype][i],seats,bpr,book.numberth);
            if(tickets[0]!=-1)
            {
                acoach=i+1;
                for(int j=0;j<book.numberth;j++)
                    booked.pb(mp(i,tickets[j]));
                return 1;
            }
        }
        tcheck=traincheck(train[ttype],coaches,seats,bpr,book.numberth,ttype);
        if(tcheck)
            return 1;
    }
    tcheck=continuousbooking(train[ttype],coaches,seats,book.numberth);
    if(tcheck)
        return 1;
    tcheck=randombooking(train[ttype],coaches,seats,book.numberth);
    if(tcheck)
        return 1;
    return -1;
}

void bookticket(){
    while(!pq.empty())
    {
        bpair temp = pq.top();
        
        //cout<<temp.pasid<<" . "<<temp.trano<<" . "<<temp.cotype<<" . "<<temp.numberth<<" . "<<temp.pref<<" . "<<temp.ages<<"\n";
        booking(temp);
        string hashed=itos(temp.pasid)+"#"+itos(booked[0].first)+"#"+itos(booked[0].second)+"#";
        for(int i=1;i<booked.size();i++)
        {
            hashed+= itos(temp.pasid)+"#"+itos(booked[i].first)+"#"+itos(booked[i].second)+"#";
        }
        booked.clear();
        //cout<<hashed<<endl;
        //cout << "me hsh " << hashed << " " << temp.socksen << endl;

        vsend.push_back(mp(hashed,temp.socksen));
        pq.pop();
    }
}


int main ()
{
    int rst,i; // Return status of functions.
    int cfd[50],num=0;
    memset(cfd,0,sizeof(cfd));
    /**************** Create a socket. *******************************/
    int sfd; // Socket file descriptor ftp.
    void sig_chld(int);
    char buf[BUF_SIZE]={'\0'};
    int b_recv   = 0; // Number of bytes received.
    
    sfd = socket (AF_INET, SOCK_STREAM, 0); /* AF_INET --> IPv4,
                      * SOCK_STREAM --> TCP Protocol, 0 --> for the protocol. */
    if (sfd == -1) 
    {
      perror ("Server: socket error");
      exit (1);
    }
    printf ("Socket fd = %d\n", sfd);

    struct sockaddr_in srv_addr,cli_addr; // Addresses of the server and the client.
    socklen_t addrlen = sizeof (struct sockaddr_in);

    // Clear the two addresses.
    memset (&srv_addr, 0, addrlen);
    memset (&cli_addr, 0, addrlen);

    // Assign values to the server address.
    srv_addr.sin_family = AF_INET; // IPv4.
    srv_addr.sin_port   = htons (port_numtcp); // Port Number.

    rst = inet_pton (AF_INET, "127.0.0.1", &srv_addr.sin_addr); /* To 
                            * type conversion of the pointer here. */
    if (rst <= 0)
    {
        perror ("Server Presentation to network address conversion.\n");
        exit (1);
    }    
    /****************** Bind the server to an address. ***************/
    rst = bind (sfd, (struct sockaddr *) &srv_addr, addrlen); /* Note
      * the type casting of the pointer to the server's address. */
    if (rst == -1)
    {
      perror ("Server: Bind failed");
      exit (1);
    }
    /***************** listen (): Mark the socket as passive. *******/
    printf ("Max connections allowed to wait: %d\n", SOMAXCONN);
    rst = listen (sfd, SOMAXCONN);
    if (rst == -1)
    {
      perror ("Server: Listen failed");
      exit (1);
    }

    signal(SIGCHLD, sig_chld);     /* must call waitpid() */

    pid_t childpid;
    int kolak=0;
    for( ; ; ){
        kolak++;
        //printf("top\n");
        fd_set rd, wr, er;
        FD_ZERO(&rd);
        FD_ZERO(&wr);
        FD_ZERO(&er);
        FD_SET(sfd, &rd);
        int nmax=0;
        nmax = sfd;
        for(i=0;i<num;i++){
            int ccd = cfd[i];
            if(ccd > 0){
                //cout << ccd << endl;
                FD_SET(ccd, &rd);
                FD_SET(ccd, &wr);
            }
            if(nmax < ccd)
                nmax = ccd;
        }
        struct timeval tm;
        tm.tv_sec = 10;
        tm.tv_usec = 0;

        //printf ("Select block starts:...\n");
        rst = select (nmax+1, &rd, &wr, &er, &tm);
        if (rst < 0 )
        {
            if(errno == EINTR)
                continue;
            else
                perror ("Select failed: ");
        }
        //printf ("Select block ends:...\n");
        //printf ("Number of set fds obtained by select() = %d\n", rst);
        if (FD_ISSET (sfd, &rd)){ 
            printf("In accept\n");
            int ksd;
            ksd = accept (sfd, (struct sockaddr *) &cli_addr, &addrlen);   
            if (ksd == -1)
            {
                perror ("Server: Accept failed");
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
        for(i=0;i<num;i++){
            int ccd = cfd[i];
            if (ccd > 0 && FD_ISSET (ccd, &rd)){
                printf("kola in for %d\n",ccd);
                char buf[BUF_SIZE]={'\0'};
                printf("%d\n", ccd);
                rst = read(ccd,buf,BUF_SIZE);
                if(rst == 0){
                    cout << "Error readingn\n";
                    close(cfd[i]);
                    cfd[i] = 0;
                    continue;
                }
                printf ("Message obtained from child with fd = %d|%s| rst %d\n", cfd[i],buf,rst);
                if(buf[0]=='#' && buf[1]=='1' && (childpid = fork()) == 0){
                    close(sfd);
                    close(cfd[i]);
                    display();
                    exit (0);
                }   
                if(buf[0]=='#' && buf[1]=='2'){
                    char str[1000];
                    strcpy(str,buf+2);
                    strcpy(buf,str);

                    printf("%s\n", buf);
                    string delimiter = "#";
                    int pos=0;
                    char data[100][1000];
                    string token;
                    string total(buf);
                    int no=0;
                    while ((pos = total.find(delimiter)) != std::string::npos)
                    {
                        token = total.substr(0, pos);
                        strcpy(data[no++],token.c_str());
                        total.erase(0, pos + delimiter.length());
                    }
                    strcpy(data[no++],total.c_str());
                    /*for(i=0;i<no;i++)
                        cout << data[i] << endl;*/
                    pq.push(mp(mp(mp(stoi(data[0]),stoi(data[1])),mp(data[2],stoi(data[3]))),mp(mp(data[4],data[5]),mp(stoi(data[6]),i))));
                    continue;
                }
                if(buf[0]=='#' && buf[1]=='3'){
                    close(cfd[i]);
                    cfd[i] = 0;
                }
            }
        }
        bookticket();
        for(vector<pair<string,int> >::iterator io=vsend.begin();io!=vsend.end();io++){
            int ss=((io->first).length()) +1;
            cout << "sending to " << cfd[io->second] << endl;
            rst = write(cfd[io->second],io->first.c_str(),ss);
            if(rst == 0){
                cout << "Error sending booking data\n";
                close(cfd[io->second]);
                cfd[io->second] = 0;
            }
            cout << "Data sent " << io->first << endl;
            //close(cfd[io->second]);
            //cfd[io->second] = 0;
            //cout << cfd[io->second] << " cfd " << io->second << endl;
        }
        vsend.clear();
    }
    rst = close (sfd); // Close the socket file descriptor.
    if (rst == -1)
    {
        perror ("Server close failed");
        exit (1);
    }
    return 1;   
}