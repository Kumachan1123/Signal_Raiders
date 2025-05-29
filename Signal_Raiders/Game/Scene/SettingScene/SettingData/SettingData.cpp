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
	using json = nlohmann::json;
	using namespace std;
	string filename = "Resources/Jsons/Setting.json";//読み込むファイルの名前を作成
	ifstream ifs(filename.c_str());//ファイルを開く
	if (ifs.good())// ファイルが正常に開けたら
	{
		json j;							//jsonオブジェクト
		ifs >> j;						//ファイルから読み込む
		m_bgmVolume = j["BGM"];			// BGMの音量
		m_seVolume = j["SE"];			// SEの音量
		m_mouseSensitivity = j["Mouse"];// マウス感度
		ifs.close();					//ファイルを閉じる
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
	using json = nlohmann::json;
	using namespace std;
	string filename = "Resources/Setting/Setting.json";//書き込むファイルの名前を作成
	ofstream ofs(filename.c_str());//ファイルを開く
	if (ofs.good())// ファイルが正常に開けたら
	{
		json j;					//jsonオブジェクト
		j["BGM"] = BGM;			// BGMの音量
		j["SE"] = SE;			// SEの音量
		j["Mouse"] = Mouse;		// マウス感度
		ofs << j;				//ファイルに書き込む
		ofs.close();			//ファイルを閉じる
	}
}