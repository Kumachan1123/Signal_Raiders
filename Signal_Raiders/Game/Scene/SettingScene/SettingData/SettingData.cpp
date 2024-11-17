#include "pch.h"
#include "SettingData.h"

SettingData::SettingData()
	: m_bgmVolume{}
	, m_seVolume{}
	, m_mouseSensitivity{}
{
}

SettingData::~SettingData()
{
}

void SettingData::Load()
{
	//読み込むファイルの名前を作成
	string filename = "Resources/Setting/Setting.json";

	//ファイルを開く
	ifstream ifs(filename.c_str());
	if (ifs.good())
	{
		json j;//jsonオブジェクト
		ifs >> j;//ファイルから読み込む
		m_bgmVolume = j["BGM"];// BGMの音量
		m_seVolume = j["SE"];// SEの音量
		m_mouseSensitivity = j["Mouse"];// マウス感度
		ifs.close();//ファイルを閉じる
	}
}

void SettingData::Save(int BGM, int SE, int Mouse)
{
	//書き込むファイルの名前を作成
	string filename = "Resources/Setting/Setting.json";

	//ファイルを開く
	ofstream ofs(filename.c_str());
	if (ofs.good())
	{
		json j;//jsonオブジェクト
		j["BGM"] = BGM;// BGMの音量
		j["SE"] = SE;// SEの音量
		j["Mouse"] = Mouse;// マウス感度
		ofs << j;//ファイルに書き込む
		ofs.close();//ファイルを閉じる
	}
}