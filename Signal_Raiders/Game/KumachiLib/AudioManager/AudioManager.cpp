/*
*	@file AudioManager.cpp
*	@brief �����Ǘ��N���X
*/

#include "pch.h"
#include "AudioManager.h"
/*
*	@brief �R���X�g���N�^
*	@param �Ȃ�
*	@retrun �Ȃ�
*/
AudioManager::AudioManager()
	: m_system(nullptr)// FMOD�V�X�e�� 
	, m_sounds()// �T�E���h 
	, m_channels()// �`�����l�� 
{
	Initialize();// ������
}
/*
*	@brief �f�X�g���N�^
*	@param �Ȃ�
*	@retrun �Ȃ�
*/
AudioManager::~AudioManager() { Shutdown(); }// FMOD�V���b�g�_�E��

/*
*	@brief ������
*	@details �V�X�e���쐬����я��������s���A
			 �Q�[�����Ŏg�p����S�Ă̌��ʉ���BGM��ǂݍ���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void AudioManager::Initialize()
{
	FMOD_RESULT result = FMOD::System_Create(&m_system);// FMOD�V�X�e���̍쐬
	if (result != FMOD_OK || !m_system)// �G���[����
	{
		m_system = nullptr;// �V�X�e���̏������Ɏ��s
		return;// �I��
	}
	result = m_system->init(512, FMOD_INIT_NORMAL, nullptr);// FMOD�V�X�e���̏�����
	if (result != FMOD_OK)		// �G���[����
	{
		m_system = nullptr;// �V�X�e���̏������Ɏ��s
		return;// �I��
	}
	// �e����ʉ��EBGM�̓ǂݍ���
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
	LoadSound("Resources/Sounds/BulletCollision.mp3", "BulletCollision");// �e���m�̏Փˉ�


}
/*
*	@brief �����Đ�����
*	@details �����f�[�^���Đ�����(BGM�͓�d�Đ������Ȃ��j
*	@param soundKey �Đ����鉹���f�[�^�̃L�[
*	@param volume ����
*	@return �Ȃ�
*/
void AudioManager::PlaySound(const std::string& soundKey, float volume)
{
	auto soundIt = m_sounds.find(soundKey);// �w�肳�ꂽ�����f�[�^�̃L�[������
	if (soundIt != m_sounds.end())// �����f�[�^�����������ꍇ
	{
		FMOD::Sound* sound = soundIt->second;// �����f�[�^�̎擾
		if (soundKey.find("BGM") != std::string::npos)// �T�E���h�L�[�ɁuBGM�v���܂܂�Ă���ꍇ
		{
			auto channelIt = m_channels.find(soundKey);// �`�����l��������
			if (channelIt != m_channels.end())// �`�����l�����݂�������
			{
				FMOD::Channel* existingChannel = channelIt->second;// �`�����l���̎擾
				bool isPlaying = false;// �Đ����t���O��錾
				existingChannel->isPlaying(&isPlaying);// �`�����l���̍Đ���Ԃ��擾
				if (isPlaying)// �Đ����̏ꍇ
				{
					existingChannel->setVolume(volume);// ���ʂ�ݒ�
					return; // ���ɍĐ����̂��߁A�������Ȃ�
				}
			}
		}
		// �����Đ�����
		FMOD::Channel* channel = nullptr;// �`�����l����錾
		m_system->playSound(sound, nullptr, false, &channel); // �����f�[�^���Đ�
		m_channels[soundKey] = channel;		// �`�����l����ۑ�����
		if (channel)// �`�����l�������݂���ꍇ
			channel->setVolume(volume);// ���ʂ�ݒ肷��
	}
}
/*
*	@brief �����V�X�e���̍X�V
*	@details FMOD�V�X�e���̍X�V���s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void AudioManager::Update() { m_system->update(); }

/*
*	@brief �����֘A���\�[�X�̉������
*	@details FMOD�V�X�e���̉�����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void AudioManager::Shutdown()
{
	if (!m_system) return; // m_system �� null �Ȃ������Ȃ�
	for (auto& pair : m_channels)	// �`�����l���̒�~�Ɖ��
	{
		if (pair.second)
		{
			pair.second->stop();
			pair.second = nullptr;
		}
	}
	m_channels.clear();	// �`�����l�����N���A
	m_channels.rehash(0); // �����n�b�V�������
	for (auto& pair : m_sounds)// ���ׂẴT�E���h�����
	{
		if (pair.second)
		{
			pair.second->release();
			pair.second = nullptr;
		}
	}
	m_sounds.clear();// �T�E���h���N���A

	if (m_system)// FMOD�V�X�e�������݂���ꍇ
	{
		m_system->update(); // �ŏI�X�V
		m_system->release();// FMOD�V�X�e���̉��
		m_system = nullptr; // �V�X�e���� null �ɐݒ�
		std::this_thread::sleep_for(std::chrono::milliseconds(100));// ���S�̂��߁A�����ҋ@
	}
}
/*
*	@brief �����f�[�^�̃��[�h
*	@details �����f�[�^�����[�h����(���[�h�ς݂Ȃ牽�����Ȃ�)
*	@param filePath ���[�h���鉹���t�@�C���̃p�X
*	@param key �����f�[�^�̃L�[
*	@return ���������ꍇ�� true�A���s�����ꍇ�� false
*/
bool AudioManager::LoadSound(const std::string& filePath, const std::string& key)
{
	if (m_sounds.find(key) != m_sounds.end()) return false;// ���Ƀ��[�h�ς݂Ȃ�I��
	FMOD::Sound* sound = nullptr;// �����f�[�^��錾
	FMOD_RESULT result = m_system->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound);// �����f�[�^�̍쐬
	if (result != FMOD_OK || !sound) return false;// �G���[����
	m_sounds[key] = sound;// �����f�[�^��ۑ�
	return true;// �����܂ł��ꂽ�琬��
}
/*
*	@brief �����f�[�^�̎擾
*	@details �����f�[�^���擾����
*	@param key �����f�[�^�̃L�[
*	@return �����f�[�^�̃|�C���^(nullptr�̏ꍇ�͎擾���s)
*/
FMOD::Sound* AudioManager::GetSound(const std::string& key)
{
	auto it = m_sounds.find(key);// �w�肳�ꂽ�L�[������
	return (it != m_sounds.end()) ? it->second : nullptr;// �����f�[�^��������Ȃ��ꍇ�� nullptr ��Ԃ�
}

/*
*	@brief �����~����
*	@details �w�肳�ꂽ�����f�[�^���~����
*	@param soundKey ��~���鉹���f�[�^�̃L�[
*	@return �Ȃ�
*/
void AudioManager::StopSound(const std::string& soundKey)
{
	auto channelIt = m_channels.find(soundKey);// �w�肳�ꂽ�����f�[�^�̃L�[������
	if (channelIt != m_channels.end())// �����f�[�^�����������ꍇ
	{
		FMOD::Channel* channel = channelIt->second; // �`�����l�����擾
		channel->stop(); // �`�����l�����~
	}
}