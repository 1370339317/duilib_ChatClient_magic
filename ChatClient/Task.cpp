#include "Task.h"
#include "LoginWnd.h"

int Task::Connect(PVOID p)
{
	if (p == NULL)
		return 0;
	LoginWnd *loginWnd = (LoginWnd*)p;
	
	while (!loginWnd->client->IsConnect())
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
		}
	}
	return 1;
}

int Task::SignInWait(PVOID p)
{
	if (p == NULL)
		return 0;
	LoginWnd *loginWnd = (LoginWnd*)p;
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

int Task::SignUpWait(PVOID p)
{
	if (p == NULL)
		return 0;
	LoginWnd *loginWnd = (LoginWnd*)p;
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

int Task::ProcessRecvDate(PVOID p)
{
	if (p == NULL)
		return 0;

	RecvDateParam *param = (RecvDateParam*)p;
	Document document;
	if (document.ParseInsitu((char *)param->data).HasParseError())
	{
		::HeapFree(::GetProcessHeap(), 0, param->data);
		delete param;
		return 0;
	}
	if (!(document.IsObject() && document.HasMember("protocol")))
	{
		::HeapFree(::GetProcessHeap(), 0, param->data);
		delete param;
		return 0;
	}

	int protocol = document["protocol"].GetInt();
	switch (protocol)
	{
	case SIGNIN_SECCUSS: // ��¼�ɹ�
		SendMessage(param->wnd->GetHWND(), WM_USER_SIGNIN_SUCESS, 0, 0);
		break;
	case SIGNIN_FAILED: // ��¼ʧ��
		SendMessage(param->wnd->GetHWND(), WM_USER_SIGNIN_FAIL, 0, 0);
		break;
	case SIGNIN_ALREADY: // ��ĵط���¼
		break;
	case SignUp_SECCUSS: // ע��ɹ�
		SendMessage(param->wnd->GetHWND(), WM_USER_SIGNUP_SUCESS, 0, 0);
		break; 
	case SignUp_FAILED: // ע��ʧ��
		SendMessage(param->wnd->GetHWND(), WM_USER_SIGNUP_FAIL, 0, 0);
			break;
	case 100: // ��¼

		break;
	case 101: // ��¼

		break;
	default:
		break;
	}

	::HeapFree(::GetProcessHeap(), 0, param->data);
	delete param;
	return 1;
}
