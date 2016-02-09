/*** IPPROTO_RAW sender ***/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DEST "127.0.0.1"
#define SOUR "127.0.0.1"

#define SPORT 2222
#define DPORT 3333

struct rtlphdr{
unsigned short int rtlp_srcport;
unsigned short int rtlp_destport;
unsigned int rtlp_seq_num;
unsigned int rtlp_ack_num;
unsigned int rtlp_chksum;
};

int csum(unsigned short *buf,int numwords,struct rtlphdr *rtlphdr1)
{
	unsigned long sum = 0;
	sum += rtlphdr1->rtlp_srcport;
	sum += rtlphdr1->rtlp_destport;
	sum += rtlphdr1->rtlp_seq_num;
	sum += rtlphdr1->rtlp_ack_num;
	int i=0;
	while(i<numwords){
		sum += buf[i];
		i++;
	}
	sum = (sum>>16) + (sum &0xffff);
	sum += (sum >>16);
	return (unsigned short)(~sum);
 }


//total rtlphdr headersize = 16 bytes
int main(int argc,char *argv[])
{
	int s;
	struct sockaddr_in daddr,saddr;
	char sendpacket[50],recpacket[50];
	/* point the iphdr to the beginning of the sendpacket */
	struct iphdr *ip = (struct iphdr *)sendpacket;  
	struct rtlphdr *rtlp1 = (struct rtlphdr *) (sendpacket + sizeof(struct iphdr)) ;

	daddr.sin_family = AF_INET;
	daddr.sin_port = 0; /* not needed in SOCK_RAW */
	inet_pton(AF_INET, DEST, (struct in_addr *)&daddr.sin_addr.s_addr);
	inet_pton(AF_INET, SOUR, (struct in_addr *)&saddr.sin_addr.s_addr);
	memset(daddr.sin_zero, 0, sizeof(daddr.sin_zero));
	memset(sendpacket, 'P', sizeof(sendpacket));   /* payload will be all As */
	
	if(argc != 5)
	{
		printf("- Invalid parameters!!!\n");
		printf("- Usage %s <source hostname/IP> <source port> <destination hostname/IP> <destination port>\n",argv[0]);
		exit(-1);
	}

	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
		perror("error:");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Socket() - Using SOCK_RAW and RAW protocol OK.\n");
	}

	ip->ihl = 5;
	ip->version = 4;
	ip->tos = 0;
	ip->tot_len = sizeof(struct iphdr) + sizeof(struct rtlphdr);	/* 16 byte value */
	ip->frag_off = 0;		/* no fragment */
	ip->ttl = 64;			/* default value */
	ip->protocol = IPPROTO_RAW;	/* protocol at L4 */
	ip->check = 0;			/* not needed in iphdr */
	ip->saddr = saddr.sin_addr.s_addr;
	ip->daddr = daddr.sin_addr.s_addr;

	unsigned int fromlen = sizeof(saddr);

	srand(time(NULL));
	int seq_num = rand()%999;
	//Fabricate the rtlphdr header
	//rtlphdr source address
	rtlp1->rtlp_srcport = htons(atoi(argv[2]));
	//rtlphdr destination address
	rtlp1->rtlp_destport = htons(atoi(argv[4]));
	//sequence number
	rtlp1->rtlp_seq_num = seq_num;


	printf("sequence number in client %d\n",seq_num);
	//rtlp1->rtlp_chksum = csum((unsigned short *)sendpacket,sizeof(struct iphdr)+sizeof(struct rtlphdr),);
	//Connection establishment 
	if (sendto(s, (char *)sendpacket, sizeof(sendpacket), 0, 
			(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr)) < 0)
			perror("sendpacket send error:");

	if (recvfrom(s, (char *)&recpacket, sizeof(recpacket), 0,
			(struct sockaddr *)&daddr, &fromlen) < 0)
			perror("packet receive error:");

		struct iphdr *ip1 = (struct iphdr *)recpacket;
		struct rtlphdr *rtlp2 = (struct rtlphdr *)(recpacket+sizeof (struct iphdr));
		printf("Recieved acknowlegdement number %d\n",rtlp2->rtlp_ack_num);
		printf("Recieved sequence number %d\n",rtlp2->rtlp_seq_num);
		int toack = rtlp2->rtlp_seq_num;

		rtlp1->rtlp_seq_num = rtlp2->rtlp_ack_num;
		rtlp1->rtlp_ack_num = toack;
		rtlp1->rtlp_srcport = SPORT;
		rtlp1->rtlp_destport = DPORT;

		printf("Sending back acknowledgement no %d:\n",toack);
		if (sendto(s, (char *)sendpacket, sizeof(sendpacket), 0, 
			(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr)) < 0)
			perror("packet send error:");
		
	printf("Connection Established !\nStarting part B\n");
	// while(1) {
	// 	sleep(1);
	// 	if (sendto(s, (char *)sendpacket, sizeof(sendpacket), 0, 
	// 		(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr)) < 0)
	// 		perror("sendpacket send error:");
	// }
	exit(EXIT_SUCCESS);
}