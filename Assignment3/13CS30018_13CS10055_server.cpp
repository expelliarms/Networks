/*** IPPROTO_RAW receiver ***/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define SEST "10.146.104.49"
#define SPORT 3333
#define DPORT 4444


struct rtlphdr {
	unsigned short srcport;
	unsigned short destport;
	int seq_num;
	int ack_num;
	int checksum;
};

int csum(struct rtlphdr *rt,char *data,int len){
	unsigned long sum = 0;
	sum += rt->srcport;
	sum += rt->destport;
	sum += rt->seq_num;
	sum += rt->ack_num;
	int i=0;
	while(i<len){
		sum += data[i];
		i++;
	}
	sum = (sum>>16) + (sum &0xffff);
	sum += (sum >>16);
	return (unsigned short)(~sum);
}

int main(void)
{
	int s;
	struct sockaddr_in saddr,daddr;
	char packet[100],packet1[100],packet2[100];
	struct iphdr *ip1 = (struct iphdr *)packet;
	struct rtlphdr *rt1 = (struct rtlphdr *)(packet+sizeof (struct iphdr));
	daddr.sin_family = AF_INET;
	daddr.sin_port = 0; /* not needed in SOCK_RAW */
	inet_pton(AF_INET, SEST, (struct in_addr *)&daddr.sin_addr.s_addr);
	memset(packet, 0, sizeof(packet));
	socklen_t *len = (socklen_t *)sizeof(saddr);
	unsigned int fromlen = sizeof(saddr);

	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
		perror("error:");
		exit(EXIT_FAILURE);
	}

	struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

	if (setsockopt (s, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0)
        perror("setsockopt failed\n");

    if (setsockopt (s, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0)
        perror("setsockopt failed\n");

	if (recvfrom(s, (char *)&packet, sizeof(packet), 0,
		(struct sockaddr *)&saddr, &fromlen) < 0)
		perror("packet receive error:");


		printf("Client seq_num: %d\n",rt1->seq_num);
		int i;

	struct iphdr *ip = (struct iphdr *)packet1;
	struct rtlphdr *rt = (struct rtlphdr *)(packet1+sizeof (struct iphdr));
	memset(packet1, 'P', sizeof(packet1));
	ip->ihl = 5;
	ip->version = 4;
	ip->tos = 16;
	ip->tot_len = htons(40);	/* 16 byte value */
	ip->frag_off = 0;		/* no fragment */
	ip->ttl = 64;			/* default value */
	ip->protocol = IPPROTO_RAW;	/* protocol at L4 */
	ip->check = 0;			/* not needed in iphdr */
	ip->saddr = daddr.sin_addr.s_addr;
	ip->daddr = saddr.sin_addr.s_addr;

	rt->ack_num = rt1->seq_num;
	srand(time(NULL));
	rt->seq_num = rand()%999;
	rt->srcport = SPORT;
	rt->destport = DPORT;
	printf("Seq_num at server is %d\n",rt->seq_num);
	if (sendto(s, (char *)packet1, sizeof(packet1), 0,
			(struct sockaddr *)&saddr, (socklen_t)sizeof(saddr)) < 0)
			perror("packet send error:");

	if (recvfrom(s, (char *)&packet2, sizeof(packet2), 0,
		(struct sockaddr *)&saddr, &fromlen) < 0)
		perror("packet receive error:");

	ip1 = (struct iphdr *)packet2;
	rt1 = (struct rtlphdr *)(packet2+sizeof (struct iphdr));

	printf("Acknowlegdement number recieved%d\n",rt1->ack_num);

	printf("Connection Established\n");
	while(1)
	{
		char packet4[100];
		while(1){
			if (recvfrom(s, (char *)&packet4, sizeof(packet4), 0,
			(struct sockaddr *)&saddr, &fromlen) < 0)
			perror("packet receive error:");
			ip1 = (struct iphdr *)packet4;
			rt1 = (struct rtlphdr *)(packet4+sizeof (struct iphdr));

			char *data = (char *)(packet4+sizeof(struct iphdr)+sizeof(struct rtlphdr));
			int len = strlen(data);
			int checksumval = csum(rt1,data,len);
			if(checksumval == rt1->checksum){
				i=sizeof(struct iphdr)+sizeof(struct rtlphdr);
				ip1->ihl = 5;
				ip1->version = 4;
				ip1->tos = 0;
				ip1->tot_len = htons(40);	 //16 byte value
				ip1->frag_off = 0;		/* no fragment */
				ip1->ttl = 64;			/* default value */
				ip1->protocol = IPPROTO_RAW;	/* protocol at L4 */
				ip1->check = 0;			/* not needed in iphdr */
				ip1->saddr = daddr.sin_addr.s_addr;
				ip1->daddr = saddr.sin_addr.s_addr;
				rt1->ack_num=rt1->seq_num;
				rt1->seq_num=0;
				printf("%s\n",packet4+i);
				rt1->srcport = SPORT;
				rt1->destport = DPORT;
				if (sendto(s, (char *)packet4, sizeof(packet4), 0,
					(struct sockaddr *)&saddr, (socklen_t)sizeof(daddr)) < 0)
					perror("packet send error:");
				break;
			}
		}
	}
	exit(EXIT_SUCCESS);
}
