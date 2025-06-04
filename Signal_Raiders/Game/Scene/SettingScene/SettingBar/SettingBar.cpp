/*
*	@file	SettingBar.cpp
*	@brief	�ݒ�o�[�N���X�i���ʂ⊴�x�Ȃǂ̃X���C�_�[�j
*/
#include <pch.h>
#include "SettingBar.h"
// BGM�̈ʒu
const DirectX::SimpleMath::Vector2 SettingBar::BGM_POSITION = { Screen::CENTER_X + 300, Screen::CENTER_Y - 300 };
// SE�̈ʒu
const DirectX::SimpleMath::Vector2 SettingBar::SE_POSITION = { Screen::CENTER_X + 300, Screen::CENTER_Y - 150 };
// ���x�̈ʒu
const DirectX::SimpleMath::Vector2 SettingBar::SENSITIVITY_POSITION = { Screen::CENTER_X + 300, Screen::CENTER_Y };
// �o�[�̕�
const float SettingBar::BAR_WIDTH = 300.0f;
// �ʂ̈ʒu�ɏ��Z����l
const float SettingBar::BAR_POINTER_DIVISION = 60.0f;
/*
*	@brief �R���X�g���N�^
*	@details �ݒ�o�[�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
SettingBar::SettingBar()
	: m_pCommonResources{}// ���ʃ��\�[�X
	, m_pDR{}// �f�o�C�X���\�[�X
	, m_pSettingMenu{   }// �ݒ胁�j���[
	, m_pBar{}// UI�I�u�W�F�N�g���X�g
	, m_windowWidth{ 0 }// �E�B���h�E��
	, m_windowHeight{ 0 }// �E�B���h�E����
	, m_time{ 0.0f }// ����
	, m_num{ SettingMenu::SettingID::BGM }// ���ID
	, m_selectNum{ SettingMenu::SelectID::NONE }// �I��ԍ�
	, m_setting{ 0,0,0 }// �ݒ�f�[�^(BGM����,SE����,�}�E�X���x)
{
}
/*
*	@brief �f�X�g���N�^
*	@details �ݒ�o�[�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
SettingBar::~SettingBar() {/*do nothing*/ }
/*
*	@brief ���j���[������������
*	@details ���j���[�A�C�e���̉摜��ǂݍ��݁A�ʒu�E�X�P�[���E�A���J�[��ݒ肷��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void SettingBar::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X���Z�b�g
	m_pCommonResources = resources;
	// �f�o�C�X���\�[�X�擾
	m_pDR = m_pCommonResources->GetDeviceResources();
	// �E�B���h�E��
	m_windowWidth = width;
	// �E�B���h�E����
	m_windowHeight = height;
	// �ݒ�f�[�^�̏�����
	m_pSettingData = std::make_unique<SettingData>();
	// �uBGM�v�̏ꏊ�ɔz�u
	Add("SettingBar"
		, Vector2(BGM_POSITION)
		, Vector2(1, 1)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	// �uSE�v�̏ꏊ�ɔz�u
	Add("SettingBar"
		, Vector2(SE_POSITION)
		, Vector2(1, 1)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	// �u�}�E�X����ǁv�̏ꏊ�ɔz�u
	Add("SettingBar"
		, Vector2(SENSITIVITY_POSITION)
		, Vector2(1, 1)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	// Json�t�@�C������ݒ��ǂݍ���
	m_pSettingData->Load();
	// BGM����
	m_setting[0] = m_pSettingData->GetBGMVolume();
	// SE����
	m_setting[1] = m_pSettingData->GetSEVolume();
	// �}�E�X���x
	m_setting[2] = m_pSettingData->GetMouseSensitivity();
	// �ݒ�o�[�̋ʂ̐���������
	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
	{
		// �ݒ�o�[�̋ʂ�ǂݍ��񂾐ݒ�ɍ��킹�Ĉړ�
		m_pBarPointer[i]->SetPosition(
			Vector2(static_cast<float>(Screen::CENTER_X + BAR_POINTER_DIVISION + m_setting[i] * BAR_POINTER_DIVISION - BAR_POINTER_DIVISION)// X���W
				, m_pBarPointer[i]->GetPosition().y));// Y���W
	}
}
/*
*	@brief ���j���[���X�V����
*	@details �}�E�X�ʒu�ɉ��������j���[�̑I����ԍX�V�ƃN���b�N������s��
*	@param elapsedTime �O�t���[������̌o�ߎ���(�b)
*	@return �Ȃ�
*/
void SettingBar::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �}�E�X�̏�Ԃ��擾
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// �}�E�X�̍��W���擾
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// �}�E�X�{�^����Ԃ��擾
	auto& mouseTracker = m_pCommonResources->GetInputManager()->GetMouseTracker();
	// �h���b�O��Ԃ�ێ�
	bool isDragging = false;
	// �h���b�O���̃A�C�e���̃C���f�b�N�X(-1�ł̓h���b�O���Ă��Ȃ����)
	int dragIndex = -1;
	// ���Ԃ����Z
	m_time += elapsedTime;
	// �ݒ�o�[�̒e�̐��J��Ԃ�
	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
	{
		// �|�C���^�[�̓����蔻��`�F�b�N�i�~�`�j
		if (m_pBarPointer[i]->IsHit(mousePos))
		{
			// ���{�^����������Ă���ꍇ�A�h���b�O��Ԃɂ���
			if (mouseTracker->GetLastState().leftButton) isDragging = true;
			// �h���b�O���̃C���f�b�N�X��ۑ�
			dragIndex = i;
			// ���[�v�𔲂���
			break;
		}
	}
	// �h���b�O���Ȃ�}�E�X���W�ɒǏ]
	if (isDragging && dragIndex != -1)
	{
		// �}�E�X��X���W���擾
		float newX = mousePos.x;
		// �͈͐���(X���W�������Ȃ̂�BGM�̈ʒu����ɂ���)
		// ���[
		float minX = BGM_POSITION.x - BAR_WIDTH;
		// �E�[
		float maxX = BGM_POSITION.x + BAR_WIDTH;
		// �͈͐���
		newX = std::max(minX, std::min(maxX, newX));
		// �|�C���^�[�̈ʒu�X�V
		m_pBarPointer[dragIndex]->SetPosition(Vector2(newX, m_pBarPointer[dragIndex]->GetPosition().y));
		// �ݒ�l�̍X�V�i0�`5�ɕϊ��j
		m_setting[dragIndex] = static_cast<int>((newX - minX) / BAR_POINTER_DIVISION);
	}
	//�u�ւ񂱂��v�������ꂽ��
	if (m_pSettingMenu->GetSelectIDNum() == SettingMenu::APPLY &&
		mouseTracker->GetLastState().leftButton)
	{
		// �ݒ��ۑ�����
		m_pSettingData->Save(m_setting[0], m_setting[1], m_setting[2]);
	}
#ifdef _DEBUG// �f�o�b�O���[�h�̂ݗL��
	// �f�o�b�O����\������
	auto debugString = m_pCommonResources->GetDebugString();
	// �E�B���h�E��̃}�E�X���W���擾����
	Vector2 pos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// �ݒ�o�[�̋ʂ̐���������
	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
	{
		// �I�΂�Ă���UI�ԍ��Ɛݒ�l��\��
		debugString->AddString("SelectNum:%i Setting:%i", m_pBarPointer[i]->IsHit(mousePos), m_setting[i]);
	}
	// �h���b�O���̃C���f�b�N�X��\��
	debugString->AddString("DragIndex:%i ", dragIndex);
	// �}�E�X�̍��W��\��
	debugString->AddString("MousePos:%f %f", pos.x, pos.y);
	// ���N���b�N��Ԃ�\��
	debugString->AddString("ClickLeft:%i", mouseTracker->GetLastState().leftButton);
#endif
}
/*
*	@brief �`�悷��
*	@details �ݒ�o�[�̋ʂƃA�C�e���摜��`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SettingBar::Render()
{
	// �ݒ�o�[�̋ʂ̐���������
	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
	{
		// ���ۂɕ\���������A�C�e���摜��\��
		m_pBar[i]->Render();
		// �ݒ�o�[�̋ʂ�\��
		m_pBarPointer[i]->Render();
	}
}
/*
*	@brief ���j���[�A�C�e����ǉ�����
*	@details �w��̉摜��UI���쐬���A�������X�g�ɒǉ����� (�I���Ȃ�w�i�g���쐬)
*	@param key ���j���[�A�C�e���̃L�[
*	@param position ���j���[�A�C�e���̈ʒu
*	@param scale ���j���[�A�C�e���̃X�P�[��
*	@param anchor ���j���[�A�C�e���̃A���J�[
*	@param type ���j���[�A�C�e���̎��
*/
void SettingBar::Add(const std::string& key
	, const DirectX::SimpleMath::Vector2& position
	, const DirectX::SimpleMath::Vector2& scale
	, KumachiLib::ANCHOR anchor
	, UIType type)
{
	using namespace DirectX::SimpleMath;
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(type);
	// UI�I�u�W�F�N�g�̐���
	std::unique_ptr<UI> userInterface = std::make_unique<UI>(m_pCommonResources);
	// �w�肳�ꂽ�摜��\�����邽�߂̃A�C�e�����쐬����
	userInterface->Create(m_pDR, key, position, Vector2(scale.x / 1.45f, scale.y / 2.0f), anchor);
	// �E�B���h�E�T�C�Y��ݒ�
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);
	// �A�C�e����V�����ǉ�
	m_pBar.push_back(std::move(userInterface));
	// �w�i�p�̃E�B���h�E�摜���ǉ�����
	std::unique_ptr<UI> base = std::make_unique<UI>(m_pCommonResources);
	// �w�肳�ꂽ�摜��\�����邽�߂̃A�C�e�����쐬����
	base->Create(m_pDR, "SettingBarPointer", position, scale, anchor);
	// �E�B���h�E�T�C�Y��ݒ�
	base->SetWindowSize(m_windowWidth, m_windowHeight);
	// �w�i�p�̃A�C�e�����V�����ǉ�����
	m_pBarPointer.push_back(std::move(base));
}