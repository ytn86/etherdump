#ifndef INIT_H_
#define INIT_H_
#endif


#define TCPDUMP_MAGIC 0xa1b2c3d4


int initSocket(char *ifName, int promMode);
void initPcapFile(int fd);
