#pragma once
#include "Libraries/nlohmann/json.hpp"
#include <vector>
#include <fstream>
using json = nlohmann::json;
using namespace std;

class SettingData
{
private:
	//	�ϐ�
	int m_bgmVolume;// BGM�̉���
	int m_seVolume;// SE�̉���
	int m_mouseSensitivity;// �}�E�X���x
	//	�֐�
public:// �֐�
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