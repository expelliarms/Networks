#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define DEST "10.146.137.99"
#define SOUR "10.146.104.49"

#define SPORT 3333
#define DPORT 4444

//RTLP HEADER
struct rtlphdr {
	unsigned short srcport;
	unsigned short destport;
	int seq_num;
	int ack_num;
	int checksum;
};
//Check sum calculator
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
	srand(time(NULL));  //seeding the rand function with time
	int s;
	struct sockaddr_in daddr,saddr;
	char packet[100],packet1[100],packet2[100];
	struct iphdr *ip = (struct iphdr *)packet;  //ip header pointed to the beginning of the packet
	struct rtlphdr *rt = (struct rtlphdr *)(packet+sizeof (struct iphdr)); //rtlp header after the iphdr

	daddr.sin_family = AF_INET;
	daddr.sin_port = 0; /* not needed in SOCK_RAW */
	inet_pton(AF_INET, DEST, (struct in_addr *)&daddr.sin_addr.s_addr);		//p to n conversion,result stored in dadder
	inet_pton(AF_INET, SOUR, (struct in_addr *)&saddr.sin_addr.s_addr);		//p to n conversion, result stored in sadder
	memset(daddr.sin_zero, 0, sizeof(daddr.sin_zero));
	memset(packet, 'C', sizeof(packet));   /* payload will be all Cs */
	memset(saddr.sin_zero, 0, sizeof(saddr.sin_zero));

	ip->ihl = 5;
	ip->version = 4;
	ip->tos = 16;
	ip->tot_len = htons(40);	/* 16 byte value */
	ip->frag_off = 0;		/* no fragment */
	ip->ttl = 64;			/* default value */
	ip->protocol = IPPROTO_RAW;	/* protocol at L4 */
	ip->check = 0;			/* not needed in iphdr */
	ip->saddr = saddr.sin_addr.s_addr;
	ip->daddr = daddr.sin_addr.s_addr;
	int seq_num = rand()%100;

	rt->seq_num = seq_num;
	rt->srcport = SPORT;
	rt->destport = DPORT;

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


	char *data;
	int csumval,len;
	printf("Sequence number generated at the client %d\n",rt->seq_num);
	if (sendto(s, (char *)packet, sizeof(packet), 0,
		(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr)) < 0)
		perror("packet send error:");
	unsigned int fromlen = sizeof(saddr);

	if (recvfrom(s, (char *)&packet1, sizeof(packet1), 0,
		(struct sockaddr *)&daddr, &fromlen) < 0)
			perror("packet receive error:");

		struct iphdr *ip1 = (struct iphdr *)packet1;
		struct rtlphdr *rt1 = (struct rtlphdr *)(packet1+sizeof (struct iphdr));
		printf("Acknowlegdement number recieved from server %d\n",rt1->ack_num);
		printf("Sequence number recieved from server %d\n",rt1->seq_num);
		int to_ack = rt1->seq_num;
		int i;

		memset(packet, 'C', sizeof(packet));
		rt->seq_num = rt1->ack_num;
		rt->ack_num = to_ack;
		rt->srcport = SPORT;
		rt->destport = DPORT;

		printf("Sending back acknowledgement no %d:\n",to_ack);
		if (sendto(s, (char *)packet, sizeof(packet), 0,
			(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr)) < 0)
			perror("packet send error:");

	printf("Connection Established \n");
	int g_sno=0;
    bool status=false;
	while(1)
	{
		if(!status)
		{
			ip1 = (struct iphdr *)packet1;
			rt1 = (struct rtlphdr *)(packet1+sizeof (struct iphdr));
			ip1->ihl = 5;
			ip1->version = 4;
			ip1->tos = 0;
			ip1->tot_len = htons(40);	/* 16 byte value */
			ip1->frag_off = 0;		/* no fragment */
			ip1->ttl = 64;			/* default value */
			ip1->protocol = IPPROTO_RAW;	/* protocol at L4 */
			ip1->check = 0;			/* not needed in iphdr */
			ip1->saddr = saddr.sin_addr.s_addr;
			ip1->daddr = daddr.sin_addr.s_addr;
			int l;
			i=sizeof(struct iphdr)+sizeof(struct rtlphdr);
			for(l=i;l<sizeof(packet1);l++)
				packet1[l] = 'C';
			scanf("%s",packet1+i);
			int n= strlen(packet1+i);
			rt1->seq_num = g_sno+n;
			rt1->ack_num=0;

			rt1->srcport = SPORT;
			rt1->destport = DPORT;

			data = (char *)(packet1+sizeof(struct iphdr)+sizeof(struct rtlphdr));
			len = strlen(data);
			csumval = csum(rt1,data,len);
			rt1->checksum = csumval;
		}
		int t=time(0);
		while(1){
			if (sendto(s, (char *)packet1, sizeof(packet), 0,
				(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr)) < 0)
				perror("packet send error:");
			fd_set socks;
		    struct timeval t;
		    FD_ZERO(&socks);
		    FD_SET(s, &socks);
		    t.tv_sec = 2;
			if (select(s + 1, &socks, NULL, NULL, &t) && recvfrom(s, (char *)&packet2, sizeof(packet2), 0,(struct sockaddr *)&daddr, &fromlen) != -1)
			{}
			ip = (struct iphdr *)packet2;
			rt = (struct rtlphdr *)(packet2+sizeof (struct iphdr));
			if(rt->ack_num==rt1->seq_num)
			{
				printf("Success sending and receiving packet\n");
				g_sno=rt->ack_num;
				status=false;
				break;
			}
			else{
				printf("trying again....\n");
				fflush(stdout);
			}
		}
	}
	exit(EXIT_SUCCESS);
}
