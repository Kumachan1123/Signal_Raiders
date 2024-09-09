#include "pch.h"
#include "Game/KumachiLib/AudioManager.h"

// �V���O���g���C���X�^���X�̏�����
std::unique_ptr<AudioManager> AudioManager::m_instance = nullptr;

// �C���X�^���X���擾����
AudioManager* const AudioManager::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance.reset(new AudioManager());
	}
	return m_instance.get();
}

// �R���X�g���N�^
AudioManager::AudioManager()
	: m_system(nullptr)
{
}

// �f�X�g���N�^
AudioManager::~AudioManager()
{
	Shutdown();
}

// FMOD�V�X�e���̏�����
void AudioManager::Initialize()
{
	FMOD::System_Create(&m_system);
	m_system->init(512, FMOD_INIT_NORMAL, nullptr);
}

// �����Đ�����
// �����Đ�����
void AudioManager::PlaySound(const std::string& soundKey, float volume)
{
	auto soundIt = m_sounds.find(soundKey);
	if (soundIt != m_sounds.end())
	{
		FMOD::Sound* sound = soundIt->second;

		// ���ɍĐ����̃`�����l�������݂���ꍇ�͍Đ����Ȃ�
		auto channelIt = m_channels.find(soundKey);
		if (channelIt != m_channels.end())
		{
			FMOD::Channel* existingChannel = channelIt->second;
			bool isPlaying = false;
			existingChannel->isPlaying(&isPlaying);
			if (isPlaying)
			{
				return; // ���ɍĐ����̂��߁A�������Ȃ�
			}
		}

		// �����Đ�����
		FMOD::Channel* channel = nullptr;
		m_system->playSound(sound, nullptr, false, &channel);

		// �`�����l����ۑ�����
		m_channels[soundKey] = channel;

		// ���ʂ�ݒ肷��
		if (channel)
		{
			channel->setVolume(volume);
		}
	}
}

// FMOD�V�X�e���̍X�V
void AudioManager::Update()
{
	m_system->update();
}

// FMOD�V�X�e���̉��
void AudioManager::Shutdown()
{
	for (auto& pair : m_sounds)
	{
		if (pair.second)
		{
			pair.second->release();
		}
	}
	m_sounds.clear();

	if (m_system)
	{
		m_system->close();
		m_system->release();
		m_system = nullptr;
	}
}

bool AudioManager::LoadSound(const std::string& filePath, const std::string& key) {
	if (m_sounds.find(key) != m_sounds.end()) {
		// ���Ƀ��[�h�ς�
		return false;
	}

	FMOD::Sound* sound = nullptr;
	FMOD_RESULT result = m_system->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound);
	if (result != FMOD_OK || !sound) {
		return false;
	}

	m_sounds[key] = sound;
	return true;
}

FMOD::Sound* AudioManager::GetSound(const std::string& key) {
	auto it = m_sounds.find(key);
	return (it != m_sounds.end()) ? it->second : nullptr;
}

bool AudioManager::IsSoundPlaying(const std::string& soundName) const
{
	auto it = m_channels.find(soundName);
	if (it != m_channels.end())
	{
		FMOD::Channel* channel = it->second;
		bool isPlaying = false;
		channel->isPlaying(&isPlaying);
		return isPlaying;
	}
	return false;
}
