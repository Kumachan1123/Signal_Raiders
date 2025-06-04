/*
*	@file	SettingData.cpp
*	@brief	設定データの読み書きを管理するクラス
*/
#include <pch.h>
#include "SettingData.h"
/*
*	@brief コンストラクタ
*	@details 各設定項目の値を初期化する
*	@param なし
*	@return なし
*/
SettingData::SettingData()
	: m_bgmVolume{}			// BGM音量初期化
	, m_seVolume{}			// SE音量初期化
	, m_mouseSensitivity{}	// マウス感度初期化
{
}
/*
*	@brief デストラクタ
*	@details 特にやることはなし
*	@param なし
*	@return なし
*/
SettingData::~SettingData() {/*do nothing*/ }
/*
*	@brief 設定ファイルを読み込む
*	@details JSONファイルからBGM・SE音量、マウス感度を読み取って保存する
*	@param なし
*	@return なし
*/
void SettingData::Load()
{
	// nlohmann::jsonライブラリを使用してJSONファイルを読み込む
	using json = nlohmann::json;
	// 標準ライブラリを使用
	using namespace std;
	//読み込むファイルの名前を作成
	string filename = "Resources/Setting/Setting.json";
	//ファイルを開く
	ifstream ifs(filename.c_str());
	// ファイルが正常に開けたら
	if (ifs.good())
	{
		//jsonオブジェクト
		json j;
		//ファイルから読み込む
		ifs >> j;
		// BGMの音量
		m_bgmVolume = j["BGM"];
		// SEの音量
		m_seVolume = j["SE"];
		// マウス感度
		m_mouseSensitivity = j["Mouse"];
		//ファイルを閉じる
		ifs.close();
	}
}
/*
*	@brief 設定ファイルに書き込む
*	@details BGM・SE音量、マウス感度をJSONファイルに書き込む
*	@param BGM BGM音量
*	@param SE SE音量
*	@param Mouse マウス感度
*	@return なし
*/
void SettingData::Save(int BGM, int SE, int Mouse)
{
	// nlohmann::jsonライブラリを使用してJSONファイルに書き込む
	using json = nlohmann::json;
	// 標準ライブラリを使用
	using namespace std;
	//書き込むファイルの名前を作成
	string filename = "Resources/Setting/Setting.json";
	//ファイルを開く
	ofstream ofs(filename.c_str());
	// ファイルが正常に開けたら
	if (ofs.good())
	{
		//jsonオブジェクト
		json j;
		// BGMの音量
		j["BGM"] = BGM;
		// SEの音量
		j["SE"] = SE;
		// マウス感度
		j["Mouse"] = Mouse;
		//ファイルに書き込む
		ofs << j;
		//ファイルを閉じる
		ofs.close();
	}
}