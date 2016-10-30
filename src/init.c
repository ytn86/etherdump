
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <netinet/if_ether.h>

#include <pcap/pcap.h>

#include "init.h"




int initSocket(char *ifName, int promMode)
{

	int soc;
	struct ifreq ifrq;
	struct sockaddr_ll ifAddr;
	struct packet_mreq mreq;
	
	if ((soc = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0 ) {
		perror("socket");
		return -1;
	}

	memset(&ifrq, 0, sizeof(struct ifreq));
	
	strncpy((char *)ifrq.ifr_name, ifName, sizeof(ifrq.ifr_name));

	
	if (ioctl (soc, SIOCGIFINDEX, &ifrq) < 0) {
		perror("ioctl");
		close(soc);
		return -1;
	}
	

	ifAddr.sll_ifindex = ifrq.ifr_ifindex;
	ifAddr.sll_family = PF_PACKET;
	ifAddr.sll_protocol = htons(ETH_P_ALL);

	if (bind(soc, (struct sockaddr *)&ifAddr, sizeof(struct sockaddr_ll)) < 0) {
		perror("bind");
		close(soc);
		return -1;
	}
	

	  //For promiscous mode
	if (promMode) {
		if (ioctl (soc, SIOCGIFINDEX, &ifrq) < 0) {
			perror("ioctl");
			close(soc);
			return -1;
		}
		
		ifrq.ifr_flags = ifrq.ifr_flags | IFF_PROMISC;

		if (ioctl (soc, SIOCSIFFLAGS, &ifrq) < 0) {
			perror("ioctl");
			close(soc);
			return -1;
		}
	}

		
	return soc;
	
}



void initPcapFile(int fd)
{

	struct pcap_file_header header;
	struct timezone tz;
	struct timeval tv;
	

	memset(&tz, 0, sizeof(struct timezone));
	memset(&tv, 0, sizeof(struct timeval));
	
	if (gettimeofday(&tv, &tz) == -1) {
		perror("gettimeofday");
	}
		       	
	
	memset(&header, 0, sizeof(struct pcap_file_header));
	
	header.magic = TCPDUMP_MAGIC;
	header.version_major = PCAP_VERSION_MAJOR;
	header.version_minor = PCAP_VERSION_MINOR;
	header.thiszone = tz.tz_dsttime;
	header.snaplen = 0xfffffff;
	header.sigfigs = 0;
	header.linktype = DLT_EN10MB;

	write(fd, &header, sizeof(struct pcap_file_header));
}
