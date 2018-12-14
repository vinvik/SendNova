/*************************************************************************
	> File Name: libev.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Fri 29 Dec 2017 12:09:37 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
//#include<malloc.h>
#include<ev.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<vector>
#include<signal.h>
#include"sn.h"

using namespace std;
#define PORT 10004
#define BUFFER_SIZE 1024*10
char Sbuff[BUFFER_SIZE] = {};
int playerArr[1024] = {};
int total_clients = 0;
sn g_sn;

void accept_cb(struct ev_loop* loop,struct ev_io* watcher,int revents);
void read_cb(struct ev_loop* loop,struct ev_io* watcher,int revents);

typedef void (*signal_handler)(int);

void signal_handler_fun(int v)
{
	printf("signal : %d\n",v);
}


int main()
{
	
	signal_handler p_signal = signal_handler_fun;
	//signal(,p_signal);
	//signal(SIGPIPE,SIG_IGN);
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigaction(SIGPIPE,&sa,0);

	struct ev_loop* loop = ev_default_loop(0);
	int sd;
	struct sockaddr_in addr;
	struct ev_io socket_accept;

	if((sd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		printf("socket error\n");
		return -1;
	}
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sd,(struct sockaddr*)&addr,sizeof(addr)) != 0)
	{
		printf("bind error\n");
	}
	if(listen(sd,0) < 0)
	{
		printf("listen error\n");
		return -1;
	}

	ev_io_init(&socket_accept,accept_cb,sd,EV_READ);
	ev_io_start(loop,&socket_accept);

	g_sn.init();
	while(1)
	{
		ev_loop(loop,0);
	}

	return 0;
}


void accept_cb(struct ev_loop* loop,struct ev_io* watcher,int revents)
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	int client_sd;
	struct ev_io* w_client = (struct ev_io*)malloc(sizeof(struct ev_io));
	if(EV_ERROR & revents)
	{
		printf("error event in accept\n");
		return;
	}

	client_sd = accept(watcher->fd,(struct sockaddr*)&client_addr,&client_len );
	//client_sd = accept(watcher->fd,NULL,NULL);
	//getpeername(watcher->fd,(sockaddr*)&client_addr,&client_len);
	char* pip = inet_ntoa(client_addr.sin_addr);
	if(client_sd < 0)
	{
		printf("accept error\n");
		return;
	}
	printf("successfully connected with client.\n");
	
	ev_io_init(w_client,read_cb,client_sd,EV_READ);
	ev_io_start(loop,w_client);
	g_sn.connect(client_sd,pip);
	total_clients++;
	printf("%d client connected.\n",total_clients);
}

void read_cb(struct ev_loop* loop,struct ev_io* watcher,int revents)
{
	//char buffer[BUFFER_SIZE];
	int read;
	int idx = 0;
	if(EV_ERROR & revents)
	{
		printf("error event in read\n");
		return;
	}
	
	while(1)
	{
		read = recv(watcher->fd,&Sbuff[idx],BUFFER_SIZE,0);
		if(read == 0)
		{
			ev_io_stop(loop,watcher);
			free(watcher);
			perror("peer might closing\n");
	
			g_sn.disconnect(watcher->fd);
	
			break;
		}
		else if(read < 0)
		{
			g_sn.disconnect(watcher->fd);
			printf("client close\n");
			return;
		}
		else
		{
			idx += read;
			//buffer[read] = '\0';
			//printf("readnum:%d msg:%s\n",read,buffer);
		}
	}
	g_sn.recv_msg(-99,Sbuff,idx,watcher->fd);


	//bzero(buffer,read);

}


