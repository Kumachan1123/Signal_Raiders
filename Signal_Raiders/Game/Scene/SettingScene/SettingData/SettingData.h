#pragma once
#include "Libraries/nlohmann/json.hpp"
#include <vector>
#include <fstream>
using json = nlohmann::json;
using namespace std;

class SettingData
{
private:
	//	変数
	int m_bgmVolume;// BGMの音量
	int m_seVolume;// SEの音量
	int m_mouseSensitivity;// マウス感度
	//	関数
public:// 関数
	SettingData();
	~SettingData();
	void Load();
	void Save(int BGM, int SE, int Mouse);
	void SetBGMVolume(int volume) { m_bgmVolume = volume; }
	void SetSEVolume(int volume) { m_seVolume = volume; }
	void SetMouseSensitivity(int sensitivity) { m_mouseSensitivity = sensitivity; }
	int GetBGMVolume() const { return m_bgmVolume; }
	int GetSEVolume() const { return m_seVolume; }
	int GetMouseSensitivity() const { return m_mouseSensitivity; }
};