/**
 * @file   Wifi.h
 *
 * @brief  Wi-Fi�̓d�g�̋����E�F�؂ƈÍ��̃Z�L�����e�B���x���𓾂�w�b�_�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */

 // ���d�C���N���[�h�̖h�~ =====================================================
#pragma once
#include "Game/CommonResources.h"
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Wifi/Output/Output.h"
#include "Game/Wifi/ReleaseMemory/ReleaseMemory.h"
#include "Game/Wifi/UpdateInfo/UpdateInfo.h"
#include "Game/Wifi/Wifi.h"
#include "Libraries/MyLib/DebugString.h"



// �N���X�̒�` ===============================================================
/**
  * @brief �Q�[��
  */
class CommonResources;
namespace mylib
{
	class DebugCamera;
	class GridFloor;
	class DebugString;
}

class Wifi
{

	// �f�[�^�����o�̐錾
private:

	// wi-fi�֘A�̕ϐ�
		// �N���X���낢��
	std::unique_ptr<UpdateInfo>				 m_updateInfo;
	std::unique_ptr<Output>					 m_output;
	std::unique_ptr<ReleaseMemory>			 m_memory;
	// Wi-Fi�֘A�̐錾
	DWORD  m_dwMaxClient; // �ő�̃N���C�A���g��
	HANDLE m_hClient;
	DWORD  m_dwCurVersion;
	DWORD  m_dwResult;
	// �C���^�[�t�F�[�X�̃��X�g�擾
	PWLAN_INTERFACE_INFO_LIST m_pInterfaceList;
	// �X�L�������ʂ̎擾
	PWLAN_AVAILABLE_NETWORK_LIST m_pNetworkList;
	// ���łɕ\������SSID���Ǘ����邽�߂�set
	std::set<std::string> m_displayedSSIDs;
	// �\��������
	int m_count;
	// �l�b�g���[�N�����i�[����vector
	std::vector<NetworkInfo> m_networkInfos;
	// �X�L�������ʂ̏����Ɏg���ϐ�
	WLAN_AVAILABLE_NETWORK m_network;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> m_converter;
	std::string m_ssid;



	// �ŏ��̌ܕb�͂��̔z���Wi-Fi�̋������i�[����
	std::vector<int> m_preWifilevels;// Wi-Fi�̋���
	std::vector<int> m_preSSIDLengths;// ssid�̕�����
	std::vector<int> m_preSSIDValues;// ssid�̕�����ASCII�R�[�h�̍��v

	// �ܕb��Ɉȉ��̔z���Wi-Fi�̏����i�[����i��x���ꂽ����͍X�V���Ȃ��j
	std::vector<int> m_wifilevels;// Wi-Fi�̋���
	std::vector<int> m_ssidLengths;// ssid�̕����̍��v�l
	std::vector<int> m_ssidValues;// ssid�̕�����ASCII�R�[�h�̍��v

	float m_time;				  // Wi-Fi�擾���ԁi�T�b�߂�����T�b�ŌŒ�j

public:
	// �R���X�g���N�^
	Wifi();
	// �f�X�g���N�^
	~Wifi();
	// ����
public:
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// �N���A
	void Clear();
public:
	// �擾�������𑗂�
	std::vector<int> GetWifiLevels()const { return m_wifilevels; }//�d�g�̋�����n��
	std::vector<int> GetSSIDLengths()const { return m_ssidLengths; }//SSID�̕�������n��
	std::vector<int> GetSSIDValues()const { return m_ssidValues; }//SSID�̕����̍��v�l��n��
	// �X�V���I��������ǂ����𒲂ׂ邽�߂̃Q�b�^�[
	std::vector<int> GetPreWifiLevels()const { return m_preWifilevels; }
	std::vector<int> GetPreSSIDLengths()const { return m_preSSIDLengths; }
	std::vector<int> GetPreSSIDValues()const { return m_preSSIDValues; }
};
