#pragma once
#include "stdafx.h"
#include "ClientBase.h"
#include "Common.h"
#include <list>
#include "ThreadPool.h"

using std::list;

class LoginWnd;
class ChatMainWnd;
class Client : public ClientBase
{
public:
	Client();
	~Client();

	// ���ӹر�
	virtual void OnConnectionClosed();
	// �����Ϸ�������
	virtual void OnConnectionError();
	// ���������
	virtual void OnRecvCompleted(BYTE* data, int len);
	// д�������
	virtual void OnSendCompleted();

	LoginWnd *loginWnd;
	ChatMainWnd *chatMainWnd;
	UserAndFriend *user;

	ThreadPool *threadPool;

	list<SessionItem *> sessions;
	list<UserAndFriend *> friends;
	list<FriendRequest *> friendRequests;
};
