#pragma once

//ͨ��Э��ֵ
enum EM_PROTOCOL
{
	INVALID_PROTOCOL,			// ��Чֵ

	//��½							 
	SIGNIN,                     // ��½
	SIGNIN_SECCUSS,             // ��½�ɹ�
	SIGNIN_FAILED,              // ��½ʧ��
	SIGNIN_ALREADY,             // ��½ʧ��
	SignUp,						// ע��
	SignUp_SECCUSS,             // ע��ɹ�
	SignUp_FAILED,              // ע��ʧ��

	// ����
	GET_FRIENDS,				// ��ȡ����
	SEARCH_FRIENDS,				// ���Һ���
	FRIENDS_REQUEST,			// ��Ӻ�������
};