#pragma once
// FMOD�̃C���N���[�h
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"
#include <string>
#include <unordered_map>
#include <thread> 
#include <memory>
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}
class AudioManager
{
public:
	// �V���O���g���C���X�^���X���擾
	static AudioManager* const GetInstance();
public:
	// �f�X�g���N�^
	~AudioManager();
	// ������
	void Initialize();
	// �����f�[�^�̃��[�h
	bool LoadSound(const std::string& filePath, const std::string& key);
	// �����f�[�^�̎擾
	FMOD::Sound* GetSound(const std::string& key);
	// �����Đ�����
	void PlaySound(const std::string& soundKey, float volume);
	// �����~����
	void StopSound(const std::string& soundKey);
	// �X�V�iFMOD�V�X�e���̍X�V���K�v�j
	void Update();
	// ���
	void Shutdown();
private:
	// �R���X�g���N�^�ƃf�X�g���N�^���v���C�x�[�g�ɂ���
	AudioManager();
	// �R�s�[�R���X�g���N�^�Ƒ�����Z�q�̋֎~
	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;
	// FMOD�֘A�̃I�u�W�F�N�g
	FMOD::System* m_system;
	std::unordered_map<std::string, FMOD::Channel*> m_channels;
	std::unordered_map<std::string, FMOD::Sound*> m_sounds;
	// �V���O���g���C���X�^���X
	static std::unique_ptr<AudioManager> m_instance;
};
