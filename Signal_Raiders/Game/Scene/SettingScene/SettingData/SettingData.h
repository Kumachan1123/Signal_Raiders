/*
*	@file	SettingData.h
*	@brief	設定データの読み書きを管理するクラス
*/
#pragma once
// 標準ライブラリ
#include <string>
#include <fstream>
// 外部ライブラリ
#include "Libraries/nlohmann/json.hpp"
class SettingData
{
public:// public関数
	SettingData();// コンストラクタ
	~SettingData();// デストラクタ
	void Load();// 設定ファイルを読み込む
	void Save(int BGM, int SE, int Mouse);// 設定ファイルに書き込む
	int GetBGMVolume() const { return m_bgmVolume; }// BGM音量を取得
	void SetBGMVolume(int volume) { m_bgmVolume = volume; }// BGM音量を設定
	int GetSEVolume() const { return m_seVolume; }// SE音量を取得
	void SetSEVolume(int volume) { m_seVolume = volume; }// SE音量を設定
	int GetMouseSensitivity() const { return m_mouseSensitivity; } // マウス感度を取得
	void SetMouseSensitivity(int sensitivity) { m_mouseSensitivity = sensitivity; } // マウス感度を設定
private://	private変数
	int m_bgmVolume;// BGMの音量
	int m_seVolume;// SEの音量
	int m_mouseSensitivity;// マウス感度
};