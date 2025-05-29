/*
*	@file Game.cpp
*	@brief �Q�[���̃��C���N���X
*/
#include <pch.h>
#include "Game.h"
/*
*	@brief �Q�[�����I������
*	@detail �Q�[�����I������֐�(Main.cpp����Ăяo�����)
*	@param �Ȃ�
*	@return �Ȃ�
*/
extern void ExitGame() noexcept;



using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
	: m_deviceResources{}
	, m_timer{}
	, m_commonStates{}
	, m_pCommonResources{}
	, m_debugString{}
	, m_inputManager{}
	, m_sceneManager{}
	, m_audioManager{}
	, m_modelManager{}
	, m_textureManager{}
	, m_fullscreen{ FALSE }
{
	m_deviceResources = std::make_unique<DX::DeviceResources>();// �f�o�C�X���\�[�X���쐬����
	m_deviceResources->RegisterDeviceNotify(this);// �f�o�C�X�ʒm��o�^����
}

// Initialize the Direct3D resources required to run.
/*
*	@brief �Q�[���̏�����
*	@detail �Q�[���̎��s�ɕK�v��Direct3D���\�[�X��������
*	@param window �E�B���h�E�n���h��
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void Game::Initialize(HWND window, int width, int height)
{
	using namespace DirectX;
	// �f�o�C�X���\�[�X�֘A��ݒ肷��
	m_deviceResources->SetWindow(window, width, height);// �E�B���h�E��ݒ肷��
	m_deviceResources->CreateDeviceResources();// �f�o�C�X���\�[�X���쐬����
	CreateDeviceDependentResources();// �f�o�C�X�ˑ����\�[�X���쐬����
	m_deviceResources->CreateWindowSizeDependentResources();// �E�B���h�E�T�C�Y�ˑ����\�[�X���쐬����
	CreateWindowSizeDependentResources();// �E�B���h�E�T�C�Y�ˑ����\�[�X���쐬����
	auto device = m_deviceResources->GetD3DDevice();// �f�o�C�X���擾����
	auto context = m_deviceResources->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g���擾����
	m_inputManager = std::make_unique<mylib::InputManager>(window);// ���̓}�l�[�W�����쐬����
	m_commonStates = std::make_unique<CommonStates>(device);// �R�����X�e�[�g���쐬����
	m_debugString = std::make_unique<mylib::DebugString>(// �f�o�b�O��������쐬����
		device,		// �f�o�C�X
		context,	// �f�o�C�X�R���e�L�X�g
		L"Resources/Fonts/SegoeUI_18.spritefont"// �t�H���g�t�@�C���̃p�X
	);
	m_audioManager = std::make_unique<AudioManager>();// �I�[�f�B�I�}�l�[�W���[���쐬����
	m_pCommonResources = std::make_unique<CommonResources>();// ���ʃ��\�[�X���쐬����
	m_modelManager = std::make_unique<ModelManager>();// ���f���}�l�[�W�����쐬����
	m_textureManager = std::make_unique<TextureManager>();// �e�N�X�`���}�l�[�W�����쐬����
	m_pCommonResources->Initialize(// �V�[���֓n�����ʃ��\�[�X��ݒ肷��
		&m_timer,				// �^�C�}�[
		m_deviceResources.get(),// �f�o�C�X���\�[�X
		m_commonStates.get(),	// �R�����X�e�[�g
		m_debugString.get(),	// �f�o�b�O������
		m_inputManager.get(),	// ���̓}�l�[�W��
		m_audioManager.get(),	// �I�[�f�B�I�}�l�[�W��
		m_modelManager.get(),	// ���f���}�l�[�W��
		m_textureManager.get()	// �e�N�X�`���}�l�[�W��
	);

	m_modelManager->SetCommonResources(m_pCommonResources.get());// ���f���}�l�[�W���ɋ��ʃ��\�[�X��ݒ肷��
	m_modelManager->Initialize();// ���f���}�l�[�W��������������
	m_textureManager->Initialize(m_pCommonResources.get());
	m_sceneManager = std::make_unique<SceneManager>();	// �V�[���}�l�[�W�����쐬����
	m_sceneManager->Initialize(m_pCommonResources.get());// �V�[���}�l�[�W��������������
	ShowCursor(FALSE);//�J�[�\����������悤�ɂ���
}

#pragma region Frame Update
/*
*	@brief �Q�[�����[�v�����s����
*	@detail �Q�[�����[�v�����s����֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::Tick()
{
	m_timer.Tick([&]() { Update(m_timer); });// �^�C�}�[���X�V���AUpdate�֐����Ăяo��
	Render();// �`�揈�����Ăяo��
}
/*
*	@brief �Q�[���̍X�V����
*	@detail �Q�[���̍X�V�������s���֐�
*	@param timer �^�C�}�[
*	@return �Ȃ�
*/
void Game::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());// �o�ߎ��Ԃ��擾����
	m_inputManager->Update();// ���̓}�l�[�W�����X�V����
	const auto& keyboardState = m_inputManager->GetKeyboardState();// �L�[�{�[�h�X�e�[�g���擾����
	if (keyboardState.Escape) ExitGame();// �uECS�v�L�[�ŏI������
	m_sceneManager->Update(elapsedTime);// �V�[���}�l�[�W�����X�V����
}
#pragma endregion

#pragma region Frame Render
/*
*	@brief �Q�[���̕`�揈��
*	@detail �Q�[���̕`�揈�����s���֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::Render()
{
	if (m_timer.GetFrameCount() == 0)return;// �ŏ���Update�O�͉����`�悵�Ȃ�
	Clear();// ��ʂ��N���A����
	m_deviceResources->PIXBeginEvent(L"Render");// PIX�C�x���g���J�n����
	auto context = m_deviceResources->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g���擾����
	UNREFERENCED_PARAMETER(context);// �f�o�C�X�R���e�L�X�g�̖��g�p�x����}������
	m_sceneManager->Render();// �V�[���}�l�[�W����`�悷��
#ifdef _DEBUG
	m_debugString->Render(m_commonStates.get());// �f�o�b�O�������`�悷��
	m_debugString->AddString("fps : %d", m_timer.GetFramesPerSecond());// �f�o�b�O��������쐬����FFPS
#endif
	m_deviceResources->PIXEndEvent();// PIX�C�x���g���I������
	m_deviceResources->Present();// �V�����t���[����\������
}

/*
*	@brief ��ʂ��N���A����
*	@detail ��ʂ��N���A����֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::Clear()
{
	using namespace DirectX;
	m_deviceResources->PIXBeginEvent(L"Clear");// PIX�C�x���g���J�n����
	auto context = m_deviceResources->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g���擾����
	auto renderTarget = m_deviceResources->GetRenderTargetView();// �����_�[�^�[�Q�b�g�r���[���擾����
	auto depthStencil = m_deviceResources->GetDepthStencilView();// �[�x�X�e���V���r���[���擾����
	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);// �����_�����O�^�[�Q�b�g�r���[���N���A����
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);// �[�x�X�e���V���r���[���N���A����
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);// �����_�[�^�[�Q�b�g�Ɛ[�x�X�e���V���r���[��ݒ肷��
	auto const viewport = m_deviceResources->GetScreenViewport();// �X�N���[���r���[�|�[�g���擾����
	context->RSSetViewports(1, &viewport); // �r���[�|�[�g��ݒ肷��
	m_deviceResources->PIXEndEvent(); // PIX�C�x���g���I������
}
#pragma endregion

#pragma region Message Handlers
/*
*	@brief �Q�[�����A�N�e�B�u�����ꂽ�Ƃ��̏���
*	@detail �Q�[�����A�N�e�B�u�E�B���h�E�ɂȂ����Ƃ��̏���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::OnActivated() {/*do nothing*/ }
/*
*	@brief �Q�[������A�N�e�B�u�����ꂽ�Ƃ��̏���
*	@detail �Q�[������A�N�e�B�u�E�B���h�E�ɂȂ����Ƃ��̏���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::OnDeactivated() {/*do nothing*/ }
/*
*	@brief �Q�[�����ꎞ��~���ꂽ�Ƃ��̏���
*	@detail �Q�[�����ꎞ��~���ꂽ�Ƃ��̏���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::OnSuspending() {/*do nothing*/ }
/*
*	@brief �Q�[�����ĊJ���ꂽ�Ƃ��̏���
*	@detail �Q�[�����ĊJ���ꂽ�Ƃ��̏���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::OnResuming() { m_timer.ResetElapsedTime(); }// �^�C�}�[�����Z�b�g����
/*
*	@brief �E�B���h�E���ړ����ꂽ�Ƃ��̏���
*	@detail �E�B���h�E���ړ����ꂽ�Ƃ��̏���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::OnWindowMoved()
{
	auto const r = m_deviceResources->GetOutputSize();// �E�B���h�E�̃T�C�Y���擾����
	m_deviceResources->WindowSizeChanged(r.right, r.bottom);// �E�B���h�E�T�C�Y���ύX���ꂽ���Ƃ�ʒm����
	BOOL fullscreen = FALSE;// �t���X�N���[�������ׂ�
	m_deviceResources->GetSwapChain()->GetFullscreenState(&fullscreen, nullptr);// �t���X�N���[����Ԃ��擾����
	if (m_fullscreen != fullscreen)// �t���X�N���[������������Ă��܂������̏���
	{
		m_fullscreen = fullscreen;// �t���X�N���[����Ԃ��X�V����
		m_deviceResources->CreateWindowSizeDependentResources();// �E�B���h�E�T�C�Y�ˑ����\�[�X���쐬����
	}
}
/*
*	@brief �f�B�X�v���C�̕ύX���������Ƃ��̏���
*	@detail �f�B�X�v���C�̕ύX���������Ƃ��̏���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::OnDisplayChange() { m_deviceResources->UpdateColorSpace(); }// �J���[�X�y�[�X���X�V����
/*
*	@brief �E�B���h�E�T�C�Y���ύX���ꂽ�Ƃ��̏���
*	@detail �E�B���h�E�T�C�Y���ύX���ꂽ�Ƃ��̏���
*	@param width �V�����E�B���h�E�̕�
*	@param height �V�����E�B���h�E�̍���
*	@return �Ȃ�
*/
void Game::OnWindowSizeChanged(int width, int height)
{
	if (!m_deviceResources->WindowSizeChanged(width, height))return;// �E�B���h�E�T�C�Y���ύX����Ă��Ȃ��ꍇ�͉������Ȃ�
	CreateWindowSizeDependentResources();// �E�B���h�E�T�C�Y�ˑ����\�[�X���쐬����
}

/*
*	@brief �f�t�H���g�̃E�B���h�E�T�C�Y���擾����
*	@detail �f�t�H���g�̃E�B���h�E�T�C�Y���擾����֐�
*	@param width �f�t�H���g�̃E�B���h�E�̕�
*	@param height �f�t�H���g�̃E�B���h�E�̍���
*	@return �Ȃ�
*/
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
	width = Screen::WIDTH;// �f�t�H���g�̃E�B���h�E�̕���ݒ�
	height = Screen::HEIGHT;// �f�t�H���g�̃E�B���h�E�̍�����ݒ�
}
#pragma endregion

#pragma region Direct3D Resources
/*
*	@brief �f�o�C�X�ˑ����\�[�X���쐬����
*	@detail �f�o�C�X�ˑ����\�[�X���쐬����֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();// �f�o�C�X���擾����
	UNREFERENCED_PARAMETER(device);// �f�o�C�X�̖��g�p�x����}������
}
/*
*	@brief �E�B���h�E�T�C�Y�ˑ����\�[�X���쐬����
*	@detail �E�B���h�E�T�C�Y�ˑ����\�[�X���쐬����֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::CreateWindowSizeDependentResources() {/*do nothing*/ }
/*
*	@brief �f�o�C�X������ꂽ�Ƃ��̏���
*	@detail �f�o�C�X������ꂽ�Ƃ��̏������s���֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::OnDeviceLost() {/*do nothing*/ }
/*
*	@brief �f�o�C�X���������ꂽ�Ƃ��̏���
*	@detail �f�o�C�X�E�E�B���h�E�T�C�Y�ˑ����\�[�X���č쐬����֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Game::OnDeviceRestored()
{
	CreateDeviceDependentResources();// �f�o�C�X�ˑ����\�[�X���쐬
	CreateWindowSizeDependentResources();// �E�B���h�E�T�C�Y�ˑ����\�[�X���쐬
}
/*
*	@brief �t���X�N���[����Ԃ�ݒ肷��
*	@detail �t���X�N���[����Ԃ�ݒ肷��֐�
*	@param value �t���X�N���[����ԁiTRUE: �t���X�N���[���AFALSE: �E�B���h�E���[�h�j
*	@return �Ȃ�
*/
void Game::SetFullscreenState(BOOL value)
{
	m_fullscreen = value;// �t���X�N���[����Ԃ��X�V
	m_deviceResources->GetSwapChain()->SetFullscreenState(m_fullscreen, nullptr);// �t���X�N���[����Ԃ�ݒ�
	if (value) m_deviceResources->CreateWindowSizeDependentResources();// �t���X�N���[����Ԃɉ����ăE�B���h�E�T�C�Y�ˑ����\�[�X���쐬
}
#pragma endregion
