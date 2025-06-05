/*
*	@file	WiFiParameters.h
*	@brief	Wi-Fi�Ɋւ���p�����[�^���`����w�b�_�[�t�@�C��
*/
#pragma once
#ifndef WIFI_PARAMETERS_DEFINED
#define WIFI_PARAMETERS_DEFINED

// Wi-Fi�Ɋւ���p�����[�^���`����
class WiFiParameters
{
public:// public�萔
	// Wi-Fi���擾�ł��Ȃ��ꍇ�̓G�̑���
	static constexpr int MAX_ENEMY = 40;
	// Wi-Fi���擾�ł��Ȃ��ꍇ�̓G�̗̑�
	static constexpr int DEFAULT_ENEMY_HP = 100;
	// Wi-Fi���擾�ł��Ȃ��ꍇ�̓G�̎��
	static constexpr int DEFAULT_ENEMY_TYPE = 0;
	// �X�V���I���鎞��
	static constexpr float MAX_TIME = 1.0f;
};
#endif // WIFI_PARAMETERS_DEFINED