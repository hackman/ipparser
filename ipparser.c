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

char *int2bin(int a, char *buffer, int buf_size) {
	int i;
    buffer += (buf_size - 1);
    for (i = 31; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';
        a >>= 1;
    }
    return buffer;
}

int main() {
	char *p[] = IP_RANGES;
	char *s = NULL;
	char *f = NULL;
	char ipbuf[33];
	int i, m;
	struct myip ip;
	ipbuf[32] = '\0';

	for(i=0; i<IP_COUNT; i++) {
		printf("Net: %s\n", p[i]);
		// Check for netmask and parse it
		if ((s = strchr(p[i], '/'))) {
			// We have netmask
			*s++;
			if (strchr(s, '.')) {
			// it is in the a.b.c.d form
				printf("P: %s\n", s);
				if (inet_aton(s, &ip.mask) == 0) {
					printf("Unable to parse the netmask\n");
					return 1;
				}
			} else {
			// it is in the /xx form
				m = atoi(s);
				if (m > 32 || m <= 0) {
					printf("invalid mask in network/netmask\n");
					return 2;
				}
				ip.mask.s_addr = 0xFFFFFFFFUL << (32 - m);
				ip.mask.s_addr = htonl(ip.mask.s_addr);
			}
			printf("Mask: %d\n", ip.mask.s_addr);
			int2bin((int) ip.mask.s_addr, ipbuf, 32);
			printf("Mask(bin): %s\n", ipbuf);
		}
		// Parse the IP portion
		s = NULL;
		f = NULL;
		s = malloc(16);
		memset(s, '\0', 16); // clear the memory
		f = s;	// copy the pointer so we can print from the begining of the copied string
		
		// copy the IP string
		while (*p[i]) {
			if (*p[i] == '/' || *p[i] == '\0')
				break;
			if (isdigit(*p[i]) || *p[i] == '.') {
				*f = *p[i];
				*p[i]++;
				*f++;
			}
		}
		// convert the IP string into 32bit binary IP
		if (inet_aton(s, &ip.net) == 0) {
			printf("Unable to parse the IP\n");
			return 4;
		}
		printf("IP: %d\n", ip.net.s_addr);
// Print it in binary
//		ipbuf[32] = '\0';
//		int2bin((int) ip.net.s_addr, ipbuf, 32);
//		printf("Mask(bin): %s\n", ipbuf);
		printf("--------------------------------\n");
	}
	
	return 0;
}
