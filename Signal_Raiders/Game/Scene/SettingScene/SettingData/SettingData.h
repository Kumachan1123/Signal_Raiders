/*
*	@file	SettingData.h
*	@brief	設定データの読み書きを管理するクラス
*/
#pragma once
#ifndef SETTING_DATA_DEFINED
#define SETTING_DATA_DEFINED
// 標準ライブラリ
#include <string>
#include <fstream>
// 外部ライブラリ
#include "Libraries/nlohmann/json.hpp"

// 設定データクラス
class SettingData
{
public:
	// public関数
	// コンストラクタ
	SettingData();
	// デストラクタ
	~SettingData();
	// 設定ファイルを読み込む
	void Load();
	// 設定ファイルに書き込む
	void Save(int BGM, int SE, int Mouse);
	// BGM音量を取得
	int GetBGMVolume() const { return m_bgmVolume; }
	// BGM音量を設定
	void SetBGMVolume(int volume) { m_bgmVolume = volume; }
	// SE音量を取得
	int GetSEVolume() const { return m_seVolume; }
	// SE音量を設定
	void SetSEVolume(int volume) { m_seVolume = volume; }
	// マウス感度を取得
	int GetMouseSensitivity() const { return m_mouseSensitivity; }
	// マウス感度を設定
	void SetMouseSensitivity(int sensitivity) { m_mouseSensitivity = sensitivity; }
private:
	//	private変数
	// BGMの音量
	int m_bgmVolume;
	// SEの音量
	int m_seVolume;
	// マウス感度
	int m_mouseSensitivity;
};
#endif // SETTING_DATA_DEFINED