/*
*	@file	SettingData.h
*	@brief	�ݒ�f�[�^�̓ǂݏ������Ǘ�����N���X
*/
#pragma once
// �W�����C�u����
#include <string>
#include <fstream>
// �O�����C�u����
#include "Libraries/nlohmann/json.hpp"
class SettingData
{
public:// public�֐�
	SettingData();// �R���X�g���N�^
	~SettingData();// �f�X�g���N�^
	void Load();// �ݒ�t�@�C����ǂݍ���
	void Save(int BGM, int SE, int Mouse);// �ݒ�t�@�C���ɏ�������
	int GetBGMVolume() const { return m_bgmVolume; }// BGM���ʂ��擾
	void SetBGMVolume(int volume) { m_bgmVolume = volume; }// BGM���ʂ�ݒ�
	int GetSEVolume() const { return m_seVolume; }// SE���ʂ��擾
	void SetSEVolume(int volume) { m_seVolume = volume; }// SE���ʂ�ݒ�
	int GetMouseSensitivity() const { return m_mouseSensitivity; } // �}�E�X���x���擾
	void SetMouseSensitivity(int sensitivity) { m_mouseSensitivity = sensitivity; } // �}�E�X���x��ݒ�
private://	private�ϐ�
	int m_bgmVolume;// BGM�̉���
	int m_seVolume;// SE�̉���
	int m_mouseSensitivity;// �}�E�X���x
};