#include "ipparser.h"

char int2bin(int a) {
	int i;
	char ipbuf[IPBUFSIZE];
	ipbuf[IPBUFSIZE] = '\0';
    for (i = IPBUFSIZE - 1; i >= 0; i--) {
        ipbuf[i] = (a & 1) + '0';
        a >>= 1;
    }
    return ipbuf[IPBUFSIZE];
}

int parse_ip(char *buf, struct myip *ip) {
		char *s = NULL;
		char *f = NULL;
		int ipsize=0;
		printf("Buf: %s\n", buf);
		// Check for netmask and parse it
		if ((s = strchr(buf, '/'))) {
			// We have netmask
			*s++;
			if (strchr(s, '.')) {
				// it is in the a.b.c.d form
				if (inet_aton(s, &ip->mask) == 0) {
					printf("Unable to parse the netmask\n");
					return 1;
				}
			} else {
				int m = 0;
				// it is in the /xx form
				m = atoi(s);
				if (m > 32 || m <= 0) {
					printf("invalid mask in network/netmask\n");
					return 2;
				}
				ip->mask.s_addr = 0xFFFFFFFFUL << (32 - m);
				ip->mask.s_addr = htonl(ip->mask.s_addr);
			}
			printf("Mask: %d\n", ip->mask.s_addr);
//			printf("Mask(bin): %s\n", int2bin((int) ip->mask.s_addr));
		}
		// Parse the IP portion
		s = NULL;
		s = malloc(16);
		memset(s, '\0', 16); // clear the memory
		f = s;
		
		// copy the IP string
		while (*buf) {
			if (*buf == '/' || *buf == '\0' || ipsize == 16)
				break;
			if (isdigit(*buf) || *buf == '.') {
				*s = *buf;
				*buf++;
				*s++;
				ipsize++;
			}
		}
		// convert the IP string into 32bit binary IP
		if (inet_aton(f, &ip->net) == 0) {
			printf("Unable to parse the IP\n");
			return 4;
		}
		printf("IP: %d\n", ip->net.s_addr);
// Print it in binary
//		ipbuf[32] = '\0';
//		int2bin((int) ip.net.s_addr, ipbuf, 32);
//		printf("Mask(bin): %s\n", ipbuf);
		printf("--------------------------------\n");
	
}

int parse_file() {
	FILE* fp;
	char buf[1024];
	char *a, *b;
	int count = 0;
	a = malloc(33);
	b = a;
	if ((fp = fopen(CONF,"rt")) == NULL) {
		fprintf(stderr,"Error opening file: " CONF "\n");
		return -1;
	}
	/* Read into the buffer contents within the file stream */
	while(fgets(buf, 1024, fp) != NULL) {
		b = buf;
		memset(a, '\0', 33);
		count=0;
		printf("Buf:          %sChar by char: ", buf);
		while(*b) {
			if (*b == '\0' || *b == '\r' || *b == '\n')
				break;
			if (isdigit(*b) || *b == '.' || *b == '/') {
				if (count == 32)
					break;
				printf("%c", *b);
				*a = *b;
				*a++;
				count++;
			}
			*b++;
		}
		printf("\nSt: %s\n-------------------------\n", b);
	}
}

int main() {
	int i;
	char *p[] = IP_RANGES;
	struct myip *return_ip;
	return_ip = malloc(sizeof(return_ip));
	for(i=0; i<IP_COUNT; i++) {
		parse_ip(p[i], return_ip);
	}
	parse_file();
	return 0;
}
