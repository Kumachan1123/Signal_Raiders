/*
*	@file	SettingData.h
*	@brief	�ݒ�f�[�^�̓ǂݏ������Ǘ�����N���X
*/
#pragma once
#ifndef SETTING_DATA_DEFINED
#define SETTING_DATA_DEFINED
// �W�����C�u����
#include <string>
#include <fstream>
// �O�����C�u����
#include "Libraries/nlohmann/json.hpp"

// �ݒ�f�[�^�N���X
class SettingData
{
public:
	// public�֐�
	// �R���X�g���N�^
	SettingData();
	// �f�X�g���N�^
	~SettingData();
	// �ݒ�t�@�C����ǂݍ���
	void Load();
	// �ݒ�t�@�C���ɏ�������
	void Save(int BGM, int SE, int Mouse);
	// BGM���ʂ��擾
	int GetBGMVolume() const { return m_bgmVolume; }
	// BGM���ʂ�ݒ�
	void SetBGMVolume(int volume) { m_bgmVolume = volume; }
	// SE���ʂ��擾
	int GetSEVolume() const { return m_seVolume; }
	// SE���ʂ�ݒ�
	void SetSEVolume(int volume) { m_seVolume = volume; }
	// �}�E�X���x���擾
	int GetMouseSensitivity() const { return m_mouseSensitivity; }
	// �}�E�X���x��ݒ�
	void SetMouseSensitivity(int sensitivity) { m_mouseSensitivity = sensitivity; }
private:
	//	private�ϐ�
	// BGM�̉���
	int m_bgmVolume;
	// SE�̉���
	int m_seVolume;
	// �}�E�X���x
	int m_mouseSensitivity;
};
#endif // SETTING_DATA_DEFINED