#pragma once
#include "stdafx.h"
#include "Common.h"

class Client;
class ThreadPool;
class AddFriendWnd;
class ChatMainWnd : public WindowImplBase
{
public:
	ChatMainWnd(Client *client, ThreadPool *threadPool);
	~ChatMainWnd();

	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetZIPFileName() const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

	virtual LPCTSTR GetWindowClassName() const;
	virtual void InitWindow();
	void LoginSuccess();
	void BackToLogin();
	virtual void Notify(TNotifyUI &msg);

	virtual LRESULT OnSysCommand(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	virtual LRESULT OnMouseHover(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	virtual LRESULT OnChar(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	virtual void OnFinalMessage(HWND hWnd);

	void UpdateFriendList();					// ���º����б�
	void AddTopSessionItem(ULONG friendID);		// ��ӻỰ�б���ö�
	void ShowSesionItem();						// �Ự�б���
	void ViewSessionChatRoom(ULONG friendID);	// ��ʾ�����
	BOOL SendMsg();								// ������Ϣ
	void OnFriendRequest(FriendRequest *request);// ������Ϣ
	void RecvSingleMsg(ChatMsgItem * msg);		// ������Ϣ
	void OtherPlaceSignin();		// �����ط���¼
	void ConnectClose();

	Client * client;
	ThreadPool * threadPool;
	AddFriendWnd *addFriendWnd;	// ��Ӻ��Ѵ���
	BOOL initSuccess;
	BOOL bClose;

	DUI_DECLARE_MESSAGE_MAP()
	void OnClick(TNotifyUI& msg);
	void OnSelectChanged(TNotifyUI& msg);
	void OnItemClick(TNotifyUI& msg);

private:
	CListContainerElementUI * CreateSessionItem(SessionItem *item);
	CListContainerElementUI * CreateFriendItem();
	CListContainerElementUI * CreateFrientItem(UserAndFriend *user);
	CListContainerElementUI * CreateChatMsgItem(ChatMsgItem *item);
	CListContainerElementUI * CreateNewFriendItem(FriendRequest *request);
	CListContainerElementUI * CreateFriendDetail(UserAndFriend *user);
	CListUI * CreateChatMsgList();

	SessionItem *friendISession;
	CLabelUI *title;
	CButtonUI *btnClose;
	CButtonUI *btnMax;
	CButtonUI *btnRestore;
	CButtonUI *btnMin;

	CButtonUI *btnSession;		// �Ựtab
	CButtonUI *btnContact;		// ����tab
	CButtonUI *btnSearchFriend;	// ������Ӻ���

	CListUI *sessionList;		// �Ự�б�
	CContainerUI *chatRoom;			// �������
	CRichEditUI *edInput;		// �������
	CButtonUI *btnSend;

	CListUI *friendList;		// �����б�
	CListUI *friendDetailList;	// ��������
};

