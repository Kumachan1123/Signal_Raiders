/*
	@file	CommonResources.h
	@brief	�V�[���֓n���A�Q�[�����Ŏg�p���鋤�ʃ��\�[�X
*/
#pragma once
// �W�����C�u����
#include <cassert>
// ����w�b�_�[�t�@�C��
#include "Game/KumachiLib/AudioManager/AudioManager.h"

// �O���錾
namespace DX
{
	class StepTimer;
	class DeviceResources;
}
namespace mylib
{
	class DebugString;
	class InputManager;
}

class CommonResources
{
public:// �A�N�Z�T
	DX::StepTimer* GetStepTimer() const { return m_stepTimer; }// �^�C�}�[���擾����
	DX::DeviceResources* GetDeviceResources() const { return m_deviceResources; }// �f�o�C�X���\�[�X���擾����
	DirectX::CommonStates* GetCommonStates() const { return m_commonStates; }// �R�����X�e�[�g���擾����
	mylib::DebugString* GetDebugString() const { return m_debugString; }// �f�o�b�O��������擾����
	mylib::InputManager* GetInputManager() const { return m_inputManager; }// ���̓}�l�[�W�����擾����
	AudioManager* GetAudioManager() const { return m_audioManager; }// �I�[�f�B�I�}�l�[�W�����擾����
public:// public���\�b�h
	CommonResources();// �R���X�g���N�^
	~CommonResources() = default;// �f�X�g���N�^ default�Œ�`
	void Initialize(// ����������
		DX::StepTimer* timer,// �󂯓n������^�C�}�[
		DX::DeviceResources* dr,// �󂯓n������f�o�C�X���\�[�X
		DirectX::CommonStates* commonStates,// �󂯓n������R�����X�e�[�g
		mylib::DebugString* debugString,// �󂯓n������f�o�b�O������
		mylib::InputManager* inputManager,// �󂯓n��������̓}�l�[�W��
		AudioManager* audioManager// �󂯓n������I�[�f�B�I�}�l�[�W��
	);
private:// private�����o�ϐ�
	// �󂯓n�����郊�\�[�X�ꗗ
	// �^�C�}�[
	DX::StepTimer* m_stepTimer;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_deviceResources;
	// �R�����X�e�[�g
	DirectX::CommonStates* m_commonStates;
	// �f�o�b�O������
	mylib::DebugString* m_debugString;
	// ���̓}�l�[�W��
	mylib::InputManager* m_inputManager;
	// �I�[�f�B�I�}�l�[�W��
	AudioManager* m_audioManager;
};
