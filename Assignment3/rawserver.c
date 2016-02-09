/*** IPPROTO_RAW receiver ***/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SOUR "10.146.137.99"
#define SPORT 2222
#define DPORT 3333
struct rtlphdr{
unsigned short int rtlp_srcport;
unsigned short int rtlp_destport;
unsigned int rtlp_seq_num;
unsigned int rtlp_ack_num;
unsigned int rtlp_chksum;
};


int csum(unsigned short *buf,int numwords,struct rtlphdr *sendrtlphdr)
{
	unsigned long sum = 0;
	sum += sendrtlphdr->rtlp_srcport;
	sum += sendrtlphdr->rtlp_destport;
	sum += sendrtlphdr->rtlp_seq_num;
	sum += sendrtlphdr->rtlp_ack_num;
	int i=0;
	while(i<numwords){
		sum += buf[i];
		i++;
	}
	sum = (sum>>16) + (sum &0xffff);
	sum += (sum >>16);
	return (unsigned short)(~sum);
 }


int main(int argc,char *argv[])
{
	int s;
	struct sockaddr_in saddr,daddr;
	char recpacket[50],sendpacket[50];
	struct iphdr *ip = (struct iphdr *)sendpacket; 
	struct iphdr *recip = (struct iphdr *)recpacket;
	struct rtlphdr *sendrtlphdr = (struct rtlphdr *) (sendpacket + sizeof(struct iphdr));
	struct rtlphdr *recrtlphdr = (struct rtlphdr *) (recpacket + sizeof(struct iphdr));
	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
		perror("error:");
		exit(EXIT_FAILURE);
	}
	memset(sendpacket, 'S', sizeof(sendpacket));

	daddr.sin_family = AF_INET;
	memset(recpacket, 0, sizeof(recpacket));
	daddr.sin_port = 0;
	socklen_t *len = (socklen_t *)sizeof(saddr);
	inet_pton(AF_INET, SOUR, (struct in_addr *)&daddr.sin_addr.s_addr);
	unsigned int fromlen = sizeof(saddr);

if(recvfrom(s, (char *)&recpacket, sizeof(recpacket), 0,
			(struct sockaddr *)&saddr, &fromlen) < 0)
			perror("recpacket receive error:");

	printf("Sequence number of client:%d\n",sendrtlphdr->rtlp_seq_num);	
	ip->ihl = 5;
	ip->version = 4;
	ip->tos = 0;
	ip->tot_len = htons(40);	/* 16 byte value */
	ip->frag_off = 0;		/* no fragment */
	ip->ttl = 64;			/* default value */
	ip->protocol = IPPROTO_RAW;	/* protocol at L4 */
	ip->check = 0;			/* not needed in iphdr */
	ip->saddr = daddr.sin_addr.s_addr;
	ip->daddr = saddr.sin_addr.s_addr;	

	int seq_number = rand()%999;
	recrtlphdr->rtlp_ack_num = sendrtlphdr->rtlp_seq_num;
	recrtlphdr->rtlp_seq_num = seq_number;
	recrtlphdr->rtlp_srcport = SPORT;
	recrtlphdr->rtlp_destport = DPORT;

	printf("Sequence number in server = %d\n",seq_number);

	if (sendto(s, (char *)sendpacket , sizeof(sendpacket), 0, 
			(struct sockaddr *)&saddr, (socklen_t)sizeof(saddr)) < 0)
			perror("packet send error:");

	if (recvfrom(s, (char *)recpacket, sizeof(recpacket), 0,
		(struct sockaddr *)&saddr, &fromlen) < 0)
		perror("packet receive error:");
		
	printf("ACK no = %d\n",recrtlphdr->rtlp_ack_num);
	// while(1) {
	// 	if (recvfrom(s, (char *)&recpacket, sizeof(recpacket), 0,
	// 		(struct sockaddr *)&saddr, &fromlen) < 0)
	// 		perror("recpacket receive error:");

	// 	int i = sizeof(struct iphdr);	/* print the payload */
	// 	while (i < sizeof(recpacket)) {
	// 		fprintf(stderr, "%c", recpacket[i]);
	// 		i++;
	// 	}
	// 	printf("\n");
	// }
	exit(EXIT_SUCCESS);
}
