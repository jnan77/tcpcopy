#include "../core/xcopy.h"

/* initiate for netlink firewall*/
int nl_firewall_init(){
	int sock = nl_init(NETLINK_FIREWALL, FIREWALL_GROUP);
	nl_set_mode(sock, IPQ_COPY_PACKET, 65536);
	return sock;
}

static char buffer[65536];

/* receive message from netlink firewall */
struct iphdr *nl_firewall_recv(int sock, unsigned long *packet_id){
	ssize_t len = nl_recv(sock, buffer, sizeof(buffer));
	ssize_t exp_len = sizeof(struct ipq_packet_msg) + NLMSG_LENGTH(0);
	if(len < 0)
	{
		logInfo(LOG_WARN,"nl recv length is less than zero:%ld", len);
		return NULL;
	}
	if(len < exp_len)
	{
		logInfo(LOG_WARN,"nl recv error:%ld", len);
		logInfo(LOG_WARN,"privilage problems or not the object of tcpcopy");
		return NULL;
	}else
	{
		struct ipq_packet_msg *msg = nl_get_payload(buffer);
		*packet_id = msg->packet_id;
		return (struct iphdr *)msg->payload;
	}
}


