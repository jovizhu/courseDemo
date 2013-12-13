#include "Getlocalip.h"



uint32_t  Getlocalip() {

	printf("Getlocalip() Enter: \n");
	struct sockaddr_in server_address;
	struct sockaddr_in local_address;
	int sock_fd = 0, pton_ret, sock_name_no;
	char server_ip[8096] = DEFAULT_DNS_IP;
	uint16_t server_port = DEFAULT_DNS_PORT;
	socklen_t len;
	uint32_t localip;

	printf("Define End: \n");

	// fill up server address structure
	memset((void *) &server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(server_port);
	pton_ret = inet_pton(AF_INET, server_ip, &server_address.sin_addr);

	printf(" Init End \n");

	if (pton_ret <= 0)
		printf("inet_pton() failed");

	// create a UDP socket
	printf("Create UDP socket: \n");
	if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("socket() failed");
	}

	if (connect(sock_fd, (struct sockaddr *) &server_address,
			sizeof(server_address)) == -1) {
		printf("connect() failed");
		return 0;
	}

	printf("get SockName : \n");
	len = sizeof(local_address);
	sock_name_no = getsockname(sock_fd, (struct sockaddr *) &local_address,
			&len);
	if (sock_name_no < 0)
		printf("getsockname() error");

    localip = ntohl(local_address.sin_addr.s_addr);

	printf("local ip is %s and num is %x \n", inet_ntoa(local_address.sin_addr), localip);

	return localip;
}
