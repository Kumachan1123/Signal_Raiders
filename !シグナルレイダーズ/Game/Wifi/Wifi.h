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
#include "Game/Wifi/Interface/IWifiParts.h"
#include "Game/Wifi/UpdateInfo/UpdateInfo.h"
#include "Libraries/MyLib/DebugString.h"
#include "Game/Wifi/ReleaseMemory/ReleaseMemory.h"
#include "Game/Wifi/Output/Output.h"
#include "Game/Wifi/Wifi.h"
#include "Game/CommonResources.h"



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
	// �N���X�萔�̐錾
public:


	// �f�[�^�����o�̐錾
private:

	// wi-fi�֘A�̕ϐ�
		// �N���X���낢��
	std::unique_ptr<UpdateInfo>				 m_updateInfo;
	std::unique_ptr<Output>					 m_output;
	std::unique_ptr<ReleaseMemory>			 m_memory;
	// Wi-Fi�֘A�̐錾
	DWORD  dwMaxClient; // �ő�̃N���C�A���g��
	HANDLE hClient;
	DWORD  dwCurVersion;
	DWORD  dwResult;
	// �C���^�[�t�F�[�X�̃��X�g�擾
	PWLAN_INTERFACE_INFO_LIST pInterfaceList;
	// �X�L�������ʂ̎擾
	PWLAN_AVAILABLE_NETWORK_LIST pNetworkList;
	// ���łɕ\������SSID���Ǘ����邽�߂�set
	std::set<std::string> displayedSSIDs;
	// �\��������
	int count;
	// �l�b�g���[�N�����i�[����vector
	std::vector<NetworkInfo> m_networkInfos;
	// �X�L�������ʂ̏����Ɏg���ϐ�
	WLAN_AVAILABLE_NETWORK network;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string ssid;
	// �Í��A���S���Y���̃Z�L�����e�B���x����\��
	SecurityLevel cipherSecurityLevel;
	// �F�؃A���S���Y���̃Z�L�����e�B���x����\��
	SecurityLevel authSecurityLevel;
	// �����o�֐��̐錾

	// �ŏ��̌ܕb�͂��̔z���Wi-Fi�̋������i�[����
	std::vector<int> m_preWifilevels;
	// �ܕb��ɂ��̔z���Wi-Fi�̋������i�[����i��x���ꂽ����͍X�V���Ȃ��j
	std::vector<int> m_wifilevels;
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
	// �`�揈��
	void Render(mylib::DebugString* debugString);
	// �N���A
	void Clear();
public:
	// �擾����Wi-Fi���󂯎��
	std::vector<int> GetWifiLevels()const { return m_wifilevels; }
	// �X�V���I��������ǂ����𒲂ׂ邽�߂̃Q�b�^�[
	std::vector<int> GetPreWifiLevels()const { return m_preWifilevels; }
};
