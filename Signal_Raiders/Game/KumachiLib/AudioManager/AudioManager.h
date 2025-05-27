/*
*	@file	AudioManager.h
*	@brief	�����Ǘ��N���X
*/
#pragma once
// �W�����C�u����
#include <string>
#include <unordered_map>
#include <thread> 
#include <memory>
// FMOD(�O�����C�u����)
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"

namespace FMOD
{
	class System;  // FMOD �̃V�X�e���N���X�ւ̃t�H���[�h�錾
	class Sound;   // FMOD �̃T�E���h�N���X�ւ̃t�H���[�h�錾
	class Channel; // FMOD �̃`���l���N���X�ւ̃t�H���[�h�錾
}
class AudioManager
{
public:
	// public�֐�
	AudioManager();	// �R���X�g���N�^
	~AudioManager();	// �f�X�g���N�^
	void Initialize();	// ������
	bool LoadSound(const std::string& filePath, const std::string& key, bool allowMultiplePlay);	// �����f�[�^�̃��[�h
	FMOD::Sound* GetSound(const std::string& key);	// �����f�[�^�̎擾
	void PlaySound(const std::string& soundKey, float volume);	// �����Đ�����
	void StopSound(const std::string& soundKey);	// �����~����
	void Update();	// �X�V�iFMOD�V�X�e���̍X�V���K�v�j
	void Shutdown();	// ���
private:
	// private�ϐ�
	FMOD::System* m_pFMODSystem;	// FMOD�V�X�e��
	std::unordered_map<std::string, FMOD::Channel*> m_pChannels;	// �`�����l��
	std::unordered_map<std::string, FMOD::Sound*> m_pSounds;	// �T�E���h
	std::unordered_map<std::string, bool> m_pAllowMultiplePlayMap; // �L�[���Ƃ̓�d�Đ���
	float m_volume;	// �{�����[��
};
