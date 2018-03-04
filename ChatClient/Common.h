#pragma once
#include "stdafx.h"
#include <Windows.h>
#include <list>

using std::list;

const char* WcharToUtf8(const wchar_t *pwStr);
const wchar_t* Utf8ToWchar(const char *pStr);
CDuiString GetInstancePath();
CDuiString GetCurrentTimeString();
char *WToA(LPCTSTR str);
LPWSTR AToW(const char* str);

//�߳��ٽ�����
class CriticalSectionLock
{
private:
	CRITICAL_SECTION cs;//�ٽ���
public:
	CriticalSectionLock() { InitializeCriticalSection(&cs); }
	~CriticalSectionLock() { DeleteCriticalSection(&cs); }
	void Lock() { EnterCriticalSection(&cs); }
	void UnLock() { LeaveCriticalSection(&cs); }
};

// �û�/����
struct UserAndFriend
{
	ULONG		userID;
	LPCTSTR		account;
	int			headerImg;
	LPCTSTR		nickName;
	int			sexulity;
	LPCTSTR		signature;
	LPCTSTR		area;
	LPCTSTR		phone;
};

struct ChatMsgItem
{
	BOOL bMyself;
	ULONG from;
	int headerImg;
	CDuiString msg;
	CDuiString time;
};

class SessionItem
{
public:
	bool mute;
	bool newMsg;
	CDuiString msgTime;
	CDuiString latestMsg;
	UserAndFriend *friendUser;
	list<ChatMsgItem *> msgRecords;
	CListUI *msgListUI;
	//CriticalSectionLock msgRecordsLock;
};

// ��������
struct FriendRequest
{
	ULONG		userID;
	LPCTSTR		account;
	int			headerImg;
	LPCTSTR		nickName;
	LPCTSTR		signature;
	int			operation;
};