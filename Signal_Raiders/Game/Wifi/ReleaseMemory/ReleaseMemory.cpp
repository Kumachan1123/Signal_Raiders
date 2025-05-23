/*
*	@file	ReleaseMemory.cpp
*	@brief	�������̉���ƃn���h���̃N���[�Y�Ɋւ���\�[�X�t�@�C��
*/
// �w�b�_�t�@�C���̓ǂݍ��� ===================================================
#include "pch.h"
#include "ReleaseMemory.h"
/*
*	@brief �R���X�g���N�^
*	@details �������̉���ƃn���h���̃N���[�Y�Ɋւ���N���X�̃R���X�g���N�^(�������Ȃ��j
*	@param �Ȃ�
*	@return �Ȃ�
*/
ReleaseMemory::ReleaseMemory() {/*do nothing*/ }
/*
 *	@brief �f�X�g���N�^
 *	@details �������̉���ƃn���h���̃N���[�Y�Ɋւ���N���X�̃f�X�g���N�^(�������Ȃ��j
 *	@param �Ȃ�
 *	@return �Ȃ�
 */
ReleaseMemory::~ReleaseMemory() {/*do nothing*/ }
/*
*	@brief �������̉���ƃn���h���̃N���[�Y
*	@details �������̉���ƃn���h���̃N���[�Y���s���֐�
*	@param pInterfaceList �C���^�[�t�F�[�X���X�g
*	@param hClient �n���h��
*	@param networkInfos Wi-Fi�����i�[����ϒ��z��
*	@param displayedSSIDs �\���ς�SSID�̃Z�b�g
*	@return �Ȃ�
*/
void ReleaseMemory::FreeMemoryAndCloseHandle(
	PWLAN_INTERFACE_INFO_LIST& pInterfaceList,
	HANDLE& hClient,
	std::vector<NetworkInfo>& networkInfos,
	std::set<std::string>& displayedSSIDs)
{
	WlanFreeMemory(pInterfaceList);// �C���^�[�t�F�[�X���X�g�̃����������
	WlanCloseHandle(hClient, NULL);// �n���h�����N���[�Y
	networkInfos.clear();// Wi-Fi�����i�[����ϒ��z����N���A
	displayedSSIDs.clear();// �\���ς�SSID�̃Z�b�g���N���A
}