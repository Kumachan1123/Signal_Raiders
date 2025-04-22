#include "pch.h"
#include "AudioManager.h"



// コンストラクタ
AudioManager::AudioManager()
	:
	m_system(nullptr),
	m_sounds(),
	m_channels()
{
	Initialize();

}

// デストラクタ
AudioManager::~AudioManager()
{
	Shutdown();
}

// FMODシステムの初期化
void AudioManager::Initialize()
{
	FMOD_RESULT result = FMOD::System_Create(&m_system);
	if (result != FMOD_OK || !m_system)
	{
		// エラー処理
		m_system = nullptr;
		return;
	}
	result = m_system->init(512, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		// エラー処理
		//m_system->release();
		m_system = nullptr;
		//return;
	}
	LoadSound("Resources/Sounds/playerBullet.mp3", "Shoot");// プレイヤーの弾のSE
	LoadSound("Resources/Sounds/Hit.mp3", "Hit");// ヒットのSE
	LoadSound("Resources/Sounds/enemybullet.mp3", "EnemyBullet");// 弾発射音
	LoadSound("Resources/Sounds/Explosion.mp3", "EnemyDead");// 敵死亡音
	LoadSound("Resources/Sounds/damage.mp3", "Damage");// プレイヤーがダメージを食らう音
	LoadSound("Resources/Sounds/Barrier.mp3", "Barrier");// ボスのバリアが出現する音
	LoadSound("Resources/Sounds/BarrierBreak.mp3", "BarrierBreak");// ボスのバリアが破壊される音
	LoadSound("Resources/Sounds/playbgm.mp3", "PlayBGM");// プレイ中のBGM
	LoadSound("Resources/Sounds/select.mp3", "SE");// 選択音
	LoadSound("Resources/Sounds/result.mp3", "ResultBGM");// 結果画面のBGM
	LoadSound("Resources/Sounds/click.mp3", "Select"); // タイトル画面の選択音
	LoadSound("Resources/Sounds/title.mp3", "TitleBGM");// タイトル画面のBGM


}


// 音を再生する(音声の種類key->"BGM","SE") (音量指定:min = 0,max = 1)
void AudioManager::PlaySound(const std::string& soundKey, float volume)
{
	//if (!m_system) return; // m_system が null なら再生しない
	auto soundIt = m_sounds.find(soundKey);
	if (soundIt != m_sounds.end())
	{
		FMOD::Sound* sound = soundIt->second;

		// サウンドキーに「BGM」が含まれている場合、二重再生を防ぐ
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
					return; // 既に再生中のため、何もしない
				}
			}
		}


		// 音を再生する
		FMOD::Channel* channel = nullptr;
		m_system->playSound(sound, nullptr, false, &channel);

		// チャンネルを保存する
		m_channels[soundKey] = channel;

		// 音量を設定する
		if (channel)
		{
			channel->setVolume(volume);
		}
	}
}

// FMODシステムの更新
void AudioManager::Update()
{
	m_system->update();
}

// FMODシステムの解放
void AudioManager::Shutdown()
{
	if (!m_system) return; // m_system が null なら解放しない
	// チャンネル解放
	for (auto& pair : m_channels)
	{
		if (pair.second)
		{
			pair.second->stop();
			pair.second = nullptr;
		}
	}
	m_channels.clear();
	m_channels.rehash(0); // チャンネルのハッシュテーブルをクリア
	// すべてのサウンドを解放
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
		m_system->update(); // 最後に update してから release
		//m_system->close();
		m_system->release();
		m_system = nullptr;
		// release の後に少し待つ
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

// 音声ファイルを読み込む
bool AudioManager::LoadSound(const std::string& filePath, const std::string& key) {
	if (m_sounds.find(key) != m_sounds.end())
	{
		// 既にロード済み
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

// 音声ファイルを取得する
FMOD::Sound* AudioManager::GetSound(const std::string& key) {
	auto it = m_sounds.find(key);
	return (it != m_sounds.end()) ? it->second : nullptr;
}

// 音を停止する
void AudioManager::StopSound(const std::string& soundKey)
{
	auto channelIt = m_channels.find(soundKey);
	if (channelIt != m_channels.end())
	{
		FMOD::Channel* channel = channelIt->second;
		channel->stop();
	}
}