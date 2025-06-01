/*
*	@file Game.h
*	@brief �Q�[���̃��C���N���X
*/
#pragma once
// �W�����C�u����
#include <thread>
// DirectX
#include <DeviceResources.h>
#include "StepTimer.h"
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/nlohmann/json.hpp>
// �Q�[���֘A
#include "Game/CommonResources.h"
#include "Game/Scene/IScene.h"
#include "Game/Scene/SceneManager.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/KumachiLib/ModelManager/ModelManager.h"
#include "Game/KumachiLib/TextureManager/TextureManager.h"
#include "Game/Screen.h"

class Game final : public DX::IDeviceNotify
{
public:// public�֐�
	Game() noexcept(false);// �R���X�g���N�^
	~Game() = default;// �f�X�g���N�^

	Game(Game&&) = default;// ���[�u�R���X�g���N�^
	Game& operator= (Game&&) = default;// ���[�u������Z�q

	Game(Game const&) = delete;// �R�s�[�R���X�g���N�^
	Game& operator= (Game const&) = delete;// �R�s�[������Z�q

	void Initialize(HWND window, int width, int height);// ������
	void Tick();// �Q�[�����[�v�����s
	void OnDeviceLost() override;// �f�o�C�X������ꂽ�Ƃ��̏���
	void OnDeviceRestored() override;// �f�o�C�X���������ꂽ�Ƃ��̏���
	// ���b�Z�[�W
	void OnActivated();// �A�N�e�B�u�����ꂽ�Ƃ��̏���
	void OnDeactivated();// ��A�N�e�B�u�����ꂽ�Ƃ��̏���
	void OnSuspending();// �ꎞ��~���ꂽ�Ƃ��̏���
	void OnResuming();// �ĊJ���ꂽ�Ƃ��̏���
	void OnWindowMoved();// �E�B���h�E���ړ����ꂽ�Ƃ��̏���
	void OnDisplayChange();// �f�B�X�v���C�̕ύX���������Ƃ��̏���
	void OnWindowSizeChanged(int width, int height);// �E�B���h�E�T�C�Y���ύX���ꂽ�Ƃ��̏���
	void GetDefaultSize(int& width, int& height) const noexcept; // �f�t�H���g�̃E�B���h�E�T�C�Y���擾
	void SetFullscreenState(BOOL value);// �t���X�N���[����Ԃ�ݒ�
private:// private�֐�
	void Update(DX::StepTimer const& timer);// �X�V����
	void Render();// �`�揈��
	void Clear();// ��ʂ��N���A
	void CreateDeviceDependentResources();// �f�o�C�X�ˑ����\�[�X�̍쐬
	void CreateWindowSizeDependentResources(); // �E�B���h�E�T�C�Y�ˑ����\�[�X�̍쐬
private:// private�ϐ�
	// �f�o�C�X���\�[�X
	std::unique_ptr<DX::DeviceResources>    m_deviceResources;
	// �^�C�}�[
	DX::StepTimer                           m_timer;
	// �t���X�N���[�����
	BOOL m_fullscreen;
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>  m_commonStates;
	// �R�������\�[�X
	std::unique_ptr<CommonResources>        m_pCommonResources;
	// �f�o�b�O������
	std::unique_ptr<mylib::DebugString>     m_debugString;
	// ���̓}�l�[�W��
	std::unique_ptr<mylib::InputManager>    m_inputManager;
	// �V�[���}�l�[�W��
	std::unique_ptr<SceneManager>           m_sceneManager;
	// �I�[�f�B�I�}�l�[�W��
	std::unique_ptr<AudioManager>           m_audioManager;
	// ���f���}�l�[�W��
	std::unique_ptr<ModelManager>           m_modelManager;
	// �e�N�X�`���}�l�[�W��
	std::unique_ptr<TextureManager>         m_textureManager;
};
