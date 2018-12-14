/*************************************************************************
	> File Name: sn.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2018年02月01日 星期四 03时25分31秒
 ************************************************************************/

#include"sn.h"

void sn::init()
{

}

void sn::connect(int sid,char* pip)
{
	m_vecPlayer.push_back(sid);
}

void sn::disconnect(int sid)
{

		vector<int>::iterator it = m_vecPlayer.begin();
		for(;it != m_vecPlayer.end();++it)
		{
			if(*it == sid)
			{
				m_vecPlayer.erase(it);

				return;
			}
		}
}


void sn::recv_msg(int id,void* msg,int size,int sid)
{
	printf("%s\n",msg);
	return;
	int Lid = id;
	char* Lmsg = (char*)msg;
	int Lsize = size;
	int Lsid = sid;

	vector<int>::iterator it = m_vecPlayer.begin();
	for(;it != m_vecPlayer.end();++it)
	{
		send(*it,Lmsg,Lsize,0);
	}
}
