/**
 * @file   UpdateInfo.h
 *
 * @brief  Wi-Fi�̏����X�V����\�[�X�t�@�C��
 *
 * @author ����Җ��F���܂�
 *
 * @date   2024/04/22
 */
 // �w�b�_�t�@�C���̓ǂݍ��� ===================================================
#include "Game/Screen.h"
#include <sstream>
#include "pch.h"
#include "UpdateInfo.h"
#include "Game/Wifi/Wifi.h"
// �����o�֐��̒�` ===========================================================
/**
 * @brief �f�t�H���g�R���X�g���N�^
 *
 * @param[in] �Ȃ�
 */
UpdateInfo::UpdateInfo(Wifi* pWifi)
	: m_InterfacesList{ nullptr }
	, m_Checkversion{ nullptr }
	, m_StartScan{ nullptr }
	, m_GetResults{ nullptr }
	, m_handle{ nullptr }
	, m_Preparation{ nullptr }
	, m_ProcessingScanResults{ nullptr }
	, m_pWifi{ pWifi }
{
}
/**
 * @brief �f�X�g���N�^
 */
UpdateInfo::~UpdateInfo()
{
}
void UpdateInfo::Initialize()
{
	m_InterfacesList = std::make_unique<GetListOfInterfaces>();
	m_Checkversion = std::make_unique<CheckVersion>();
	m_StartScan = std::make_unique<StartScan>();
	m_GetResults = std::make_unique<GetScanResults>();
	m_Preparation = std::make_unique<Preparation>();
	m_ProcessingScanResults = std::make_unique< ProcessingScanResults>();
}


void UpdateInfo::Set()
{
	// Wi-Fi�n���h���̏�����
	m_handle->Initialize(m_pWifi);
	// �o�[�W�����̊m�F
	m_Checkversion->Check(m_pWifi);
	// �C���^�[�t�F�[�X�̃��X�g�擾
	m_InterfacesList->GetList(m_pWifi);
	// �X�L�����̊J�n
	m_StartScan->Scan(m_pWifi);
	// �X�L�������ʂ̎擾
	m_GetResults->GetResults(m_pWifi);
	if (m_pWifi->GetResult() != ERROR_SUCCESS)
	{
		return;
	}
	// �\������
	m_Preparation->SetUp(m_pWifi);
	// �X�L�������ʂ̏���
	m_ProcessingScanResults->GetResults(m_pWifi);
}

