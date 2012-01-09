#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define IP_RANGES { "11.0.0.0/24", "12.0.0.0/255.255.255.0", "13.1.0.12" }
#define IP_COUNT 3
struct myip {
	struct in_addr net;
	struct in_addr mask;
};

