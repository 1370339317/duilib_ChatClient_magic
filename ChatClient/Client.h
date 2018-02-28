#pragma once
#include "stdafx.h"
#include "ClientBase.h"

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

	void AssociateWnd(WindowImplBase *wnd);
	WindowImplBase *wnd;
};
