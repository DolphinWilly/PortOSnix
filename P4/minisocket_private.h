#ifndef __MINISOCKETS_PRIVATE_H_
#define __MINISOCKETS_PRIVATE_H_

struct minisocket
{
    int num;
    int seq;
    int ack;
    queue_t data;
    semaphore_t socket_mutex;
    semaphore_t receive;
    network_address_t addr;
    int remote;
    enum socket_status {
		LISTEN,
		SYNSENT,
		SYNRECEIVED,
		ESTABLISHED,
		CLOSEWAIT,
		LASTACK,
		CLOSED,
		FINWAIT1,
		FINWAIT2,
		CLOSING,
		TIMEWAIT
    } state;
};

#endif /* __MINISOCKETS_PRIVATE_H_ */
