/*
*	@file	AudioManager.h
*	@brief	音声管理クラス
*/
#pragma once
// 標準ライブラリ
#include <string>
#include <unordered_map>
#include <thread> 
#include <memory>
// FMOD(外部ライブラリ)
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"

namespace FMOD
{
	class System;  // FMOD のシステムクラスへのフォワード宣言
	class Sound;   // FMOD のサウンドクラスへのフォワード宣言
	class Channel; // FMOD のチャネルクラスへのフォワード宣言
}
class AudioManager
{
public:
	// public関数
	AudioManager();	// コンストラクタ
	~AudioManager();	// デストラクタ
	void Initialize();	// 初期化
	bool LoadSound(const std::string& filePath, const std::string& key, bool allowMultiplePlay);	// 音声データのロード
	FMOD::Sound* GetSound(const std::string& key);	// 音声データの取得
	void PlaySound(const std::string& soundKey, float volume);	// 音を再生する
	void StopSound(const std::string& soundKey);	// 音を停止する
	void Update();	// 更新（FMODシステムの更新が必要）
	void Shutdown();	// 解放
private:
	// private変数
	FMOD::System* m_pFMODSystem;	// FMODシステム
	std::unordered_map<std::string, FMOD::Channel*> m_pChannels;	// チャンネル
	std::unordered_map<std::string, FMOD::Sound*> m_pSounds;	// サウンド
	std::unordered_map<std::string, bool> m_pAllowMultiplePlayMap; // キーごとの二重再生可否
	float m_volume;	// ボリューム
};
