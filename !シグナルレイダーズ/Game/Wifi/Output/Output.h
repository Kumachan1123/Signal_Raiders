/**
 * @file   Output.h
 *
 * @brief  ���\���Ɋւ���w�b�_�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */

 // ���d�C���N���[�h�̖h�~ =====================================================
#pragma once


#include "Game/Wifi/Interface/IWifiParts.h"

// �N���X�̒�` ===============================================================
/**
  * @brief �Q�[��
  */
  // �l�b�g���[�N�����i�[����\����

class Output
{
private:
	
	// Wi-Fi�����i�[����ϒ��z��
	std::vector<NetworkInfo> m_networkInfos;
	// �\����
	int m_count;
	// �Í��A���S���Y���]��
	SecurityLevel m_cipherSecurityLevel;
	// �F�؃A���S���Y���]��
	SecurityLevel m_authSecurityLevel;
	// ���l�������f�[�^���i�[����ϐ�
	Datas m_data;
	std::vector<Datas> m_datas;
	// �����o�֐��̐錾
public:
	// �R���X�g���N�^
	Output();
	// �f�X�g���N�^
	~Output();
	// ����\��
	void DisplayInformation(std::vector<NetworkInfo>& networkInfos,
							int& count,
							SecurityLevel& cipherSecurityLevel,
							SecurityLevel& authSecurityLevel  );
	// �����o��
	std::vector<NetworkInfo> GetInformation() { return m_networkInfos; };
	// �������
	void SetInformation(std::vector<NetworkInfo>& networkInfos) { m_networkInfos = networkInfos; };
	// �\���񐔂��o��
	int GetCount() { return m_count; };
	// �\���񐔂����
	void SetCount(int count) { m_count = count; };
	// �Í��A���S���Y���]���o��
	SecurityLevel GetCipherSecurityLevel() { return m_cipherSecurityLevel; };
	// �Í��A���S���Y���]������
	void SetCipherSecurityLevel(SecurityLevel& cipherSecurityLevel) { m_cipherSecurityLevel = cipherSecurityLevel; };
	// �F�؃A���S���Y���]���o��
	SecurityLevel GetAuthSecurityLevel() {return m_authSecurityLevel;};
	// �Í��A���S���Y���]������
	void SetAuthSecurityLevel(SecurityLevel& authSecurityLevel) { m_authSecurityLevel = authSecurityLevel; };
	// ���l��
	void SetValue();
	std::vector<Datas> GetDatas()const { return m_datas; }
	// Cipher Algorithm��]�����ăZ�L�����e�B���x����Ԃ��֐�
	SecurityLevel EvaluateSecurityLevel(DWORD cipherAlgorithm);
	// �Z�L�����e�B���x���𕶎���ɕϊ�����֐�
	std::string ConvertSecurityLevelToJapanese(SecurityLevel securityLevel);
	std::string ConvertSecurityLevelToJapanese(DWORD cipherAlgorithm);
	int ConvertSecurityLevelToValue(SecurityLevel securityLevel);
	// �F�؃A���S���Y����]�����ăZ�L�����e�B���x����Ԃ��֐�
	SecurityLevel EvaluateAuthAlgorithmSecurity(DOT11_AUTH_ALGORITHM authAlgorithm);
	// �Z�L�����e�B���x���𕶎���ɕϊ�����֐�
	std::string ConvertAuthSecurityLevelToJapanese(SecurityLevel securityLevel);
	int ConvertAuthSecurityLevelToValue(SecurityLevel securityLevel);
};
