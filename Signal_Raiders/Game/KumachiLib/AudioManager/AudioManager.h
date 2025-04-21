#pragma once
// FMODのインクルード
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
public:
	// コンストラクタ
	AudioManager();

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
	// 音を停止する
	void StopSound(const std::string& soundKey);
	// 更新（FMODシステムの更新が必要）
	void Update();
	// 解放
	void Shutdown();
private:

	// FMOD関連のオブジェクト
	FMOD::System* m_system;
	std::unordered_map<std::string, FMOD::Channel*> m_channels;
	std::unordered_map<std::string, FMOD::Sound*> m_sounds;
};
