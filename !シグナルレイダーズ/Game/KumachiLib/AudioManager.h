#pragma once
// FMODのインクルード
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"
#include <string>
#include <unordered_map>
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
	// シングルトンインスタンスを取得
	static AudioManager* const GetInstance();

public:
	// デストラクタ
	~AudioManager();

	// 初期化
	void Initialize();

	// 音声データのロード
	bool LoadSound(const std::string& filePath, const std::string& key);

	// 音声データの取得
	FMOD::Sound* GetSound(const std::string& key);

	// 音を再生する
	void PlaySound(const std::string& soundKey, float volume);


	// 音声が再生中かどうかをチェック
	bool IsSoundPlaying(const std::string& soundName) const;

	// 更新（FMODシステムの更新が必要）
	void Update();

	// 解放
	void Shutdown();

private:
	// コンストラクタとデストラクタをプライベートにする
	AudioManager();


	// コピーコンストラクタと代入演算子の禁止
	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

	// FMOD関連のオブジェクト
	FMOD::System* m_system;
	FMOD::Sound* m_sound;
	FMOD::Channel* m_channel;
	std::unordered_map<std::string, FMOD::Channel*> m_channels;
	std::unordered_map<std::string, FMOD::Sound*> m_sounds;
	// シングルトンインスタンス

	static std::unique_ptr<AudioManager> m_instance;
};
