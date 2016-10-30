#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>

#include <signal.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <pcap/pcap.h>

#include "init.h"
#include "dumper.h"


static volatile int loopFlag = 1;

void handler()
{
	loopFlag = 0;
}
	

int dump(int soc, char *outFile)
{
	char buf[BUFSIZE];
	struct sockaddr_ll data;
	struct timeval tv;
	struct timezone tz;
	//socklen_t len;
	socklen_t len;
	int size;
	int fd;
	int pktnum = 0;


	
	fd = open(outFile, O_CREAT | O_WRONLY, 0644);
	if (fd < 0 ) {
		perror("open");
		return -1;
	}
	
	initPcapFile(fd);
	memset(&data, 0, sizeof(struct sockaddr_ll));
	len = sizeof(struct sockaddr_ll);

	memset(&tv, 0, sizeof(struct timeval));
	memset(&tz, 0, sizeof(struct timezone));

	signal(SIGINT, handler);

	while (loopFlag) {
		memset(&data, 0, sizeof(struct sockaddr_ll));
		len = sizeof(data);
		//if ((size=recvfrom(soc, buf, BUFSIZE, 0, (struct sockaddr *)&data, &len)) <= 0) {
		if ((size=read(soc, buf, BUFSIZE)) <= 0) {
			perror("recvfrom");
		} else if (size > sizeof(struct ether_header)) {
			pktnum++;

			gettimeofday(&tv, &tz);

			write(fd, &tv.tv_sec, sizeof(uint32_t));
			write(fd, &tv.tv_sec, sizeof(uint32_t));
			write(fd, &size, sizeof(size));
			write(fd, &size, sizeof(size));
			write(fd, &buf, size);

		}
	}
	fprintf(stderr, "%d packets captured.\n", pktnum);

	close(fd);
	return 0;
}
	
