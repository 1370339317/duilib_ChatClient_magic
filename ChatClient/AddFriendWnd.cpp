#include "AddFriendWnd.h"
#include "Task.h"

AddFriendWnd::AddFriendWnd(WindowImplBase *wnd, Client *client, ThreadPool *threadPool)
{
	this->wnd = wnd;
	this->client = client;
	this->threadPool = threadPool;
	searchEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	friendID - 0;
}


AddFriendWnd::~AddFriendWnd()
{
	CloseHandle(searchEvent);
}

UILIB_RESOURCETYPE AddFriendWnd::GetResourceType() const
{
	//#ifdef _DEBUG
	//	return UILIB_FILE;
	//#else
	//	return UILIB_ZIP;
	//#endif // _DEBUG
	return UILIB_FILE;
}

CDuiString AddFriendWnd::GetSkinFolder()
{
	return L"Skin\\";
}

CDuiString AddFriendWnd::GetSkinFile()
{
	return L"AddFriendWnd.xml";
}

CDuiString AddFriendWnd::GetZIPFileName() const
{
	return L"Skin.zip";
}

LPCTSTR AddFriendWnd::GetWindowClassName() const
{
	return L"AddFriendWnd";
}

void AddFriendWnd::InitWindow()
{
	btnClose = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"btnclose"));
	btnSearch = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"BtnSearch"));
	btnAdd = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"Add"));
	edSearch = static_cast<CEditUI *>(m_PaintManager.FindControl(L"EdSearch"));
	edSearch->SetFocus();
	searchResult = static_cast<CContainerUI *>(m_PaintManager.FindControl(L"SearchResult"));
	lbTips = static_cast<CLabelUI *>(m_PaintManager.FindControl(L"Tips"));
	searchResult->SetVisible(false);
}

void AddFriendWnd::Notify(TNotifyUI & msg)
{
	if (msg.sType == L"click")
	{
		if (msg.pSender == btnClose)
		{
			ShowWindow(false);
			return;
		}
		if (msg.pSender == btnSearch)
		{
			CDuiString account = edSearch->GetText();
			if (account == L"")
			{
				ShowTips(L"��������ҵ��˺ţ�", true);
			}
			else
			{
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("protocol");
				writer.Int(SEARCH_FRIENDS);
				writer.Key("account");
				writer.String(WToA(account));
				writer.EndObject();
				const char *json = s.GetString();

				if (!client->Send((BYTE*)json, strlen(json) + 1))
				{
					ShowTips(L"����ʧ�ܣ������������ӣ�", true);
					return;
				}

				threadPool->QueueTaskItem(Task::SearchWait, (WPARAM)this, NULL);
			}
			return;
		}
		if (msg.pSender == btnAdd)
		{
			if (friendID != 10)
			{
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("protocol");
				writer.Int(FRIENDS_REQUEST);
				writer.Key("userID");
				writer.Int(client->user->userID);
				writer.Key("friedID");
				writer.Int(friendID);
				writer.EndObject();
				const char *json = s.GetString();

				if (!client->Send((BYTE*)json, strlen(json) + 1))
				{
					ShowTips(L"����ʧ�ܣ������������ӣ�", true);
					return;
				}
				ShowTips(L"���������ͳɹ���");
			}
			return;
		}
	}
	return WindowImplBase::Notify(msg);
}

LRESULT AddFriendWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCLBUTTONDBLCLK:   //��ֹ˫���Ŵ󴰿�
		return 0;
	case WM_USER_SEARCH_FRIEND:
		SetEvent(searchEvent);
		if (wParam == NULL)
		{
			searchResult->SetVisible(false);
			ShowTips(L"����ʧ�ܣ��޴��˺ŵ��û���", true);
		}
		else
		{
			UserAndFriend * user = (UserAndFriend *)wParam;
			CButtonUI *headerImg = static_cast<CButtonUI *>(m_PaintManager.FindControl(L"HeaderImg"));
			if (headerImg)
			{
				CString str;
				str.Format(L"HeaderImg%1d.png", user->headerImg);
				headerImg->SetBkImage(str);
			}

			CLabelUI *nickName = static_cast<CLabelUI *>(m_PaintManager.FindControl(L"NickName"));
			if (nickName)
			{
				nickName->SetText(user->nickName);
			}

			CLabelUI *signature = static_cast<CLabelUI *>(m_PaintManager.FindControl(L"Signture"));
			if (signature)
			{
				signature->SetText(user->signature);
			}
			searchResult->SetVisible();
			friendID = user->userID;
			ShowTips(L"���ҳɹ���");
			delete user;
		}
		return 0;
	default:
		break;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

void AddFriendWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void AddFriendWnd::ShowTips(CDuiString tips, BOOL bWarning)
{
	if (bWarning)
	{
		lbTips->SetText(tips);
		lbTips->SetTextColor(0xFFFF0000);
	}
	else
	{
		lbTips->SetText(tips);
		lbTips->SetTextColor(0xFF53CB4E);
	}
}

void AddFriendWnd::SetSearchBtnEnable(BOOL enable)
{
	btnSearch->SetEnabled(enable);
}
