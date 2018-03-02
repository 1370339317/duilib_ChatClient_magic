#include "Task.h"
#include "LoginWnd.h"
#include "AddFriendWnd.h"

int Task::Connect(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL)
		return 0;
	LoginWnd *loginWnd = (LoginWnd*)wParam;
	
	while (WaitForSingleObject(loginWnd->stopEvent, 0))
	{
		loginWnd->Connect();
		if (!loginWnd->client->IsConnect())
		{
			loginWnd->ShowTip(L"��������ʧ�ܣ������������ӣ�", TRUE);
			Sleep(1000);
			continue;
		}
		else
		{
			loginWnd->ShowTip(L"");
			break;
		}
	}
	return 1;
}

int Task::SignInWait(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL)
		return 0;
	LoginWnd *loginWnd = (LoginWnd*)wParam;
	int i = 0;
	while (true)
	{
		DWORD ret = WaitForSingleObject(loginWnd->signInEvent, 500);
		if (ret == WAIT_TIMEOUT)
		{
			i++;
			if (i%3 == 1)
			{
				loginWnd->ShowTip(L"���ڵ�¼.");
			}
			else if (i % 3 == 2)
			{
				loginWnd->ShowTip(L"���ڵ�¼..");
			}
			else if (i % 3 == 0)
			{
				loginWnd->ShowTip(L"���ڵ�¼...");
			}
			if (i > 20)
			{
				loginWnd->ShowTip(L"��¼��ʱ", TRUE);
				loginWnd->SetSignInBtnEnable();
				break;
			}
		}
		else
		{
			break;
		}

	}
	return 1;
}

int Task::SignUpWait(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL)
		return 0;
	LoginWnd *loginWnd = (LoginWnd*)wParam;
	int i = 0;
	while (true)
	{
		DWORD ret = WaitForSingleObject(loginWnd->signUpEvent, 500);
		if (ret == WAIT_TIMEOUT)
		{
			i++;
			if (i % 3 == 1)
			{
				loginWnd->ShowTip(L"����ע��.");
			}
			else if (i % 3 == 2)
			{
				loginWnd->ShowTip(L"����ע��..");
			}
			else if (i % 3 == 0)
			{
				loginWnd->ShowTip(L"����ע��...");
			}
			if (i > 20)
			{
				loginWnd->ShowTip(L"ע�ᳬʱ", TRUE);
				loginWnd->SetSignUpBtnEnable();
				break;
			}
		}
		else
		{
			break;
		}

	}
	return 1;
}

int Task::ProcessRecvDate(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL || lParam == NULL)
		return 0;

	BYTE *data = (BYTE *)wParam;
	WindowImplBase *wnd = (WindowImplBase *)lParam;
	Document document;
	if (document.ParseInsitu((char *)data).HasParseError())
	{
		::HeapFree(::GetProcessHeap(), 0, data);
		return 0;
	}
	if (!(document.IsObject() && document.HasMember("protocol")))
	{
		::HeapFree(::GetProcessHeap(), 0, data);
		return 0;
	}

	int protocol = document["protocol"].GetInt();
	switch (protocol)
	{
	case SIGNIN_SECCUSS: // ��¼�ɹ�
	{
		UserAndFriend *user = new UserAndFriend;
		user->userID = document["userID"].GetInt();
		user->account = AToW(document["account"].GetString());
		user->headerImg = document["headerImg"].GetInt();
		user->nickName = AToW(document["nickName"].GetString());
		user->area = AToW(document["area"].GetString());
		user->phone = AToW(document["phone"].GetString());
		user->sexulity = document["sexulity"].GetInt();
		user->signature = AToW(document["signature"].GetString());
		SendMessage(wnd->GetHWND(), WM_USER_SIGNIN_SUCESS, (WPARAM)user, 0);
		break;
	}
	case SIGNIN_FAILED: // ��¼ʧ��
		SendMessage(wnd->GetHWND(), WM_USER_SIGNIN_FAIL, 0, 0);
		break;
	case SIGNIN_ALREADY: // ��ĵط���¼
		break;
	case SignUp_SECCUSS: // ע��ɹ�
		SendMessage(wnd->GetHWND(), WM_USER_SIGNUP_SUCESS, 0, 0);
		break; 
	case SignUp_FAILED: // ע��ʧ��
		SendMessage(wnd->GetHWND(), WM_USER_SIGNUP_FAIL, 0, 0);
			break;
	case GET_FRIENDS: // ��ȡ����
	{
		const Value& friends = document["Friends"];
		if (!friends.IsArray())
		{
			break;
		}
		for (SizeType i = 0; i < friends.Size(); i++)
		{
			UserAndFriend * user = new UserAndFriend;
			user->userID = friends[i]["userID"].GetInt();
			user->account = AToW(friends[i]["account"].GetString());
			user->headerImg = friends[i]["headerImg"].GetInt();
			user->nickName = AToW(friends[i]["nickName"].GetString());
			user->area = AToW(friends[i]["area"].GetString());
			user->phone = AToW(friends[i]["phone"].GetString());
			user->sexulity = friends[i]["sexulity"].GetInt();
			user->signature = AToW(friends[i]["signature"].GetString());
			SendMessage(wnd->GetHWND(), WM_USER_GET_FRIENDS, (WPARAM)user, 0);
		}
		SendMessage(wnd->GetHWND(), WM_USER_GET_FRIENDS, 0, 1);
		break;
	}
	case SEARCH_FRIENDS: // ��¼
		if (document["result"].GetBool())
		{
			UserAndFriend * user = new UserAndFriend;
			user->userID = document["userID"].GetInt();
			user->account = AToW(document["account"].GetString());
			user->headerImg = document["headerImg"].GetInt();
			user->nickName = AToW(document["nickName"].GetString());
			user->area = AToW(document["area"].GetString());
			user->phone = AToW(document["phone"].GetString());
			user->sexulity = document["sexulity"].GetInt();
			user->signature = AToW(document["signature"].GetString());
			SendMessage(wnd->GetHWND(), WM_USER_SEARCH_FRIEND, (WPARAM)user, 0);
		}
		else
		{
			SendMessage(wnd->GetHWND(), WM_USER_SEARCH_FRIEND, 0, 0);
		}
		break;
	case 111: // ��¼

		break;
	case 1111: // ��¼

		break;
	case 11111: // ��¼

		break;
	case 111111: // ��¼

		break;
	case 111112: // ��¼

		break;
	default:
		break;
	}

	::HeapFree(::GetProcessHeap(), 0, data);
	return 1;
}

int Task::SearchWait(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL)
		return 0;
	AddFriendWnd *wnd = (AddFriendWnd*)wParam;

	int i = 0;
	while (true)
	{
		DWORD ret = WaitForSingleObject(wnd->searchEvent, 500);
		if (ret == WAIT_TIMEOUT)
		{
			i++;
			if (i % 3 == 1)
			{
				wnd->ShowTips(L"��������.");
			}
			else if (i % 3 == 2)
			{
				wnd->ShowTips(L"��������..");
			}
			else if (i % 3 == 0)
			{
				wnd->ShowTips(L"��������...");
			}
			if (i > 20)
			{
				wnd->ShowTips(L"������ʱ", TRUE);
				wnd->SetSearchBtnEnable();
				break;
			}
		}
		else
		{
			break;
		}

	}
	return 1;
}
