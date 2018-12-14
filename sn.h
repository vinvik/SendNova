/*************************************************************************
	> File Name: sn.h
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2018年02月01日 星期四 03时21分01秒
 ************************************************************************/
#include<stdio.h>
#include<vector>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
using namespace std;

class sn
{
public:
	void init();
	void connect(int sid,char* pip);
	void disconnect(int sid);
	void recv_msg(int id,void* msg,int size,int sid); 
private:
	vector<int> m_vecPlayer;
};
