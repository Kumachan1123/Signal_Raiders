/*
*	@file AudioManager.cpp
*	@brief 音声管理クラス
*/

#include "pch.h"
#include "AudioManager.h"
/*
*	@brief コンストラクタ
*	@param なし
*	@retrun なし
*/
AudioManager::AudioManager()
	: m_system(nullptr)// FMODシステム 
	, m_sounds()// サウンド 
	, m_channels()// チャンネル 
{
	Initialize();// 初期化
}
/*
*	@brief デストラクタ
*	@param なし
*	@retrun なし
*/
AudioManager::~AudioManager() { Shutdown(); }// FMODシャットダウン

/*
*	@brief 初期化
*	@details システム作成および初期化を行い、
			 ゲーム内で使用する全ての効果音とBGMを読み込む
*	@param なし
*	@return なし
*/
void AudioManager::Initialize()
{
	FMOD_RESULT result = FMOD::System_Create(&m_system);// FMODシステムの作成
	if (result != FMOD_OK || !m_system)// エラー処理
	{
		m_system = nullptr;// システムの初期化に失敗
		return;// 終了
	}
	result = m_system->init(512, FMOD_INIT_NORMAL, nullptr);// FMODシステムの初期化
	if (result != FMOD_OK)		// エラー処理
	{
		m_system = nullptr;// システムの初期化に失敗
		return;// 終了
	}
	// 各種効果音・BGMの読み込み
	LoadSound("Resources/Sounds/playerBullet.mp3", "Shoot", true);// プレイヤーの弾のSE
	LoadSound("Resources/Sounds/Hit.mp3", "Hit", true);// ヒットのSE
	LoadSound("Resources/Sounds/enemybullet.mp3", "EnemyBullet", true);// 弾発射音
	LoadSound("Resources/Sounds/Explosion.mp3", "EnemyDead", true);// 敵死亡音
	LoadSound("Resources/Sounds/damage.mp3", "Damage", true);// プレイヤーがダメージを食らう音
	LoadSound("Resources/Sounds/Barrier.mp3", "Barrier", false);// ボスのバリアが出現する音
	LoadSound("Resources/Sounds/BarrierBreak.mp3", "BarrierBreak", true);// ボスのバリアが破壊される音
	LoadSound("Resources/Sounds/playbgm.mp3", "PlayBGM", false);// プレイ中のBGM
	LoadSound("Resources/Sounds/select.mp3", "SE", true);// 選択音
	LoadSound("Resources/Sounds/result.mp3", "ResultBGM", false);// 結果画面のBGM
	LoadSound("Resources/Sounds/click.mp3", "Select", true); // タイトル画面の選択音
	LoadSound("Resources/Sounds/title.mp3", "TitleBGM", false);// タイトル画面のBGM
	LoadSound("Resources/Sounds/BulletCollision.mp3", "BulletCollision", true);// 弾同士の衝突音
	LoadSound("Resources/Sounds/BossAppear.mp3", "BossAppear", true);// ボス登場演出音
	LoadSound("Resources/Sounds/SpecialAttack.mp3", "SpecialAttack", true);// ボスの特殊攻撃音
	LoadSound("Resources/Sounds/ChargeSpecial.mp3", "ChargeSpecial", true);// ボスの特殊攻撃待機音



}
/*
*	@brief 音を再生する
*	@details 音声データを再生する(BGMは二重再生させない）
*	@param soundKey 再生する音声データのキー
*	@param volume 音量
*	@return なし
*/
void AudioManager::PlaySound(const std::string& soundKey, float volume)
{

	auto soundIt = m_sounds.find(soundKey);// 指定された音声データのキーを検索
	if (soundIt != m_sounds.end())// 音声データが見つかった場合
	{
		FMOD::Sound* sound = soundIt->second;// 音声データの取得
		// 音声データが見つかった場合
		auto allowIt = m_allowMultiplePlayMap.find(soundKey); // 二重再生可否のフラグを取得
		bool allowMultiple = (allowIt != m_allowMultiplePlayMap.end()) ? allowIt->second : false; // 見つからなければデフォルトで false

		if (!allowMultiple) // 二重再生を許可していない場合
		{
			auto channelIt = m_channels.find(soundKey); // チャンネルを検索
			if (channelIt != m_channels.end()) // チャンネルが見つかった場合
			{
				FMOD::Channel* existingChannel = channelIt->second; // チャンネルの取得
				bool isPlaying = false; // 再生中かどうかを保持する変数
				existingChannel->isPlaying(&isPlaying); // 再生中か確認
				if (isPlaying) // もし再生中なら
				{
					existingChannel->setVolume(volume); // 音量だけ更新
					return; // 再生しない（重複再生防止）
				}
			}
		}
		// 音を再生する
		FMOD::Channel* channel = nullptr;// チャンネルを宣言
		m_system->playSound(sound, nullptr, false, &channel); // 音声データを再生
		m_channels[soundKey] = channel;		// チャンネルを保存する
		if (channel)// チャンネルが存在する場合
			channel->setVolume(volume);// 音量を設定する
	}
}
/*
*	@brief 音声システムの更新
*	@details FMODシステムの更新を行う
*	@param なし
*	@return なし
*/
void AudioManager::Update() { m_system->update(); }

/*
*	@brief 音声関連リソースの解放処理
*	@details FMODシステムの解放を行う
*	@param なし
*	@return なし
*/
void AudioManager::Shutdown()
{
	if (!m_system) return; // m_system が null なら解放しない
	for (auto& pair : m_channels)	// チャンネルの停止と解放
	{
		if (pair.second)
		{
			pair.second->stop();
			pair.second = nullptr;
		}
	}
	m_channels.clear();	// チャンネルをクリア
	m_channels.rehash(0); // 内部ハッシュを解放
	for (auto& pair : m_sounds)// すべてのサウンドを解放
	{
		if (pair.second)
		{
			pair.second->release();
			pair.second = nullptr;
		}
	}
	m_sounds.clear();// サウンドをクリア

	if (m_system)// FMODシステムが存在する場合
	{
		m_system->update(); // 最終更新
		m_system->release();// FMODシステムの解放
		m_system = nullptr; // システムを null に設定
		std::this_thread::sleep_for(std::chrono::milliseconds(100));// 安全のため、少し待機
	}
}
/*
*	@brief 音声データのロード
*	@details 音声データをロードする(ロード済みなら何もしない)
*	@param filePath ロードする音声ファイルのパス
*	@param key 音声データのキー
*	@param allowMultiplePlay 二重再生を許可するかどうか
*	@return 成功した場合は true、失敗した場合は false
*/
bool AudioManager::LoadSound(const std::string& filePath, const std::string& key, bool allowMultiplePlay)
{
	if (m_sounds.find(key) != m_sounds.end()) return false;// 既にロード済みなら終了
	FMOD::Sound* sound = nullptr;// 音声データを宣言
	FMOD_RESULT result = m_system->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound);// 音声データの作成
	if (result != FMOD_OK || !sound) return false;// エラー処理
	m_sounds[key] = sound;// 音声データを保存
	m_allowMultiplePlayMap[key] = allowMultiplePlay; // 二重再生の可否を保存
	return true;// ここまでこれたら成功
}
/*
*	@brief 音声データの取得
*	@details 音声データを取得する
*	@param key 音声データのキー
*	@return 音声データのポインタ(nullptrの場合は取得失敗)
*/
FMOD::Sound* AudioManager::GetSound(const std::string& key)
{
	auto it = m_sounds.find(key);// 指定されたキーを検索
	return (it != m_sounds.end()) ? it->second : nullptr;// 音声データが見つからない場合は nullptr を返す
}

/*
*	@brief 音を停止する
*	@details 指定された音声データを停止する
*	@param soundKey 停止する音声データのキー
*	@return なし
*/
void AudioManager::StopSound(const std::string& soundKey)
{
	auto channelIt = m_channels.find(soundKey);// 指定された音声データのキーを検索
	if (channelIt != m_channels.end())// 音声データが見つかった場合
	{
		FMOD::Channel* channel = channelIt->second; // チャンネルを取得
		channel->stop(); // チャンネルを停止
	}
}