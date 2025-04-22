#include "pch.h"
#include "AudioManager.h"



// �R���X�g���N�^
AudioManager::AudioManager()
	:
	m_system(nullptr),
	m_sounds(),
	m_channels()
{
	Initialize();

}

// �f�X�g���N�^
AudioManager::~AudioManager()
{
	Shutdown();
}

// FMOD�V�X�e���̏�����
void AudioManager::Initialize()
{
	FMOD_RESULT result = FMOD::System_Create(&m_system);
	if (result != FMOD_OK || !m_system)
	{
		// �G���[����
		m_system = nullptr;
		return;
	}
	result = m_system->init(512, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		// �G���[����
		//m_system->release();
		m_system = nullptr;
		//return;
	}
	LoadSound("Resources/Sounds/playerBullet.mp3", "Shoot");// �v���C���[�̒e��SE
	LoadSound("Resources/Sounds/Hit.mp3", "Hit");// �q�b�g��SE
	LoadSound("Resources/Sounds/enemybullet.mp3", "EnemyBullet");// �e���ˉ�
	LoadSound("Resources/Sounds/Explosion.mp3", "EnemyDead");// �G���S��
	LoadSound("Resources/Sounds/damage.mp3", "Damage");// �v���C���[���_���[�W��H�炤��
	LoadSound("Resources/Sounds/Barrier.mp3", "Barrier");// �{�X�̃o���A���o�����鉹
	LoadSound("Resources/Sounds/BarrierBreak.mp3", "BarrierBreak");// �{�X�̃o���A���j�󂳂�鉹
	LoadSound("Resources/Sounds/playbgm.mp3", "PlayBGM");// �v���C����BGM
	LoadSound("Resources/Sounds/select.mp3", "SE");// �I����
	LoadSound("Resources/Sounds/result.mp3", "ResultBGM");// ���ʉ�ʂ�BGM
	LoadSound("Resources/Sounds/click.mp3", "Select"); // �^�C�g����ʂ̑I����
	LoadSound("Resources/Sounds/title.mp3", "TitleBGM");// �^�C�g����ʂ�BGM


}


// �����Đ�����(�����̎��key->"BGM","SE") (���ʎw��:min = 0,max = 1)
void AudioManager::PlaySound(const std::string& soundKey, float volume)
{
	//if (!m_system) return; // m_system �� null �Ȃ�Đ����Ȃ�
	auto soundIt = m_sounds.find(soundKey);
	if (soundIt != m_sounds.end())
	{
		FMOD::Sound* sound = soundIt->second;

		// �T�E���h�L�[�ɁuBGM�v���܂܂�Ă���ꍇ�A��d�Đ���h��
		if (soundKey.find("BGM") != std::string::npos)
		{
			auto channelIt = m_channels.find(soundKey);
			if (channelIt != m_channels.end())
			{
				FMOD::Channel* existingChannel = channelIt->second;
				bool isPlaying = false;
				existingChannel->isPlaying(&isPlaying);
				if (isPlaying)
				{
					existingChannel->setVolume(volume);
					return; // ���ɍĐ����̂��߁A�������Ȃ�
				}
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
	if (!m_system) return; // m_system �� null �Ȃ������Ȃ�
	// �`�����l�����
	for (auto& pair : m_channels)
	{
		if (pair.second)
		{
			pair.second->stop();
			pair.second = nullptr;
		}
	}
	m_channels.clear();
	m_channels.rehash(0); // �`�����l���̃n�b�V���e�[�u�����N���A
	// ���ׂẴT�E���h�����
	for (auto& pair : m_sounds)
	{
		if (pair.second)
		{
			pair.second->release();
			pair.second = nullptr;
		}
	}
	m_sounds.clear();

	if (m_system)
	{
		m_system->update(); // �Ō�� update ���Ă��� release
		//m_system->close();
		m_system->release();
		m_system = nullptr;
		// release �̌�ɏ����҂�
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

// �����t�@�C����ǂݍ���
bool AudioManager::LoadSound(const std::string& filePath, const std::string& key) {
	if (m_sounds.find(key) != m_sounds.end())
	{
		// ���Ƀ��[�h�ς�
		return false;
	}

	FMOD::Sound* sound = nullptr;
	FMOD_RESULT result = m_system->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound);
	if (result != FMOD_OK || !sound)
	{
		return false;
	}

	m_sounds[key] = sound;
	return true;
}

// �����t�@�C�����擾����
FMOD::Sound* AudioManager::GetSound(const std::string& key) {
	auto it = m_sounds.find(key);
	return (it != m_sounds.end()) ? it->second : nullptr;
}

// �����~����
void AudioManager::StopSound(const std::string& soundKey)
{
	auto channelIt = m_channels.find(soundKey);
	if (channelIt != m_channels.end())
	{
		FMOD::Channel* channel = channelIt->second;
		channel->stop();
	}
}