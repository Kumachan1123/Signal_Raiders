/*
*	@file	TitleMenu.cpp
*	@brief	�^�C�g�����j���[�N���X
*/
#include <pch.h>
#include "TitleMenu.h"
// �����ȃ��j���[�C���f�b�N�X
const int TitleMenu::INVALID_MENU_INDEX = 6;// �����ȃ��j���[�C���f�b�N�X
/*
*	@brief �R���X�g���N�^
*	@details �^�C�g�����j���[�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
TitleMenu::TitleMenu()
	: m_menuIndex{ 0 }// ���ݑI�𒆂̃��j���[�C���f�b�N�X
	, m_pDR{ nullptr }// �f�o�C�X���\�[�X�ւ̃|�C���^
	, m_pCommonResources{ nullptr }// ���ʃ��\�[�X�ւ̃|�C���^
	, m_pUI{}// UI�̃|�C���^�[
	, m_pSelect{}// �I�����ꂽ���ɕ\������w�iUI�̃|�C���^�[
	, m_pSelectTexturePath{ nullptr }// �I��w�i�̃e�N�X�`���p�X
	, m_windowWidth{ 0 }// �E�B���h�E��
	, m_windowHeight{ 0 }// �E�B���h�E����
	, m_time{ 0.0f }// �o�ߎ���
	, m_hit{ false }// �q�b�g�t���O
	, m_num{ SceneID::STAGESELECT }// �V�[��ID
	, m_SEVolume{ 0.0f }// SE����
	, m_isSEPlay{ false }// SE�Đ��t���O
{
}
/*
*	@brief �f�X�g���N�^
*	@details �^�C�g�����j���[�N���X�̃f�X�g���N�^(�����ł͉������Ȃ�)
*	@param �Ȃ�
*	@return �Ȃ�
*/
TitleMenu::~TitleMenu() {/*do nothing*/ }
/*
*	@brief ���j���[������������
*	@details ���j���[�A�C�e���̉摜��ǂݍ��݁A�ʒu�E�X�P�[���E�A���J�[��ݒ肷��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void TitleMenu::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pCommonResources = resources;// ���ʃ��\�[�X���Z�b�g
	m_pDR = m_pCommonResources->GetDeviceResources();// �f�o�C�X���\�[�X�擾
	m_windowWidth = width;// �E�B���h�E��
	m_windowHeight = height;// �E�B���h�E����
	//m_pSelectTexturePath = L"Resources/Textures/Select.png";// �I��g�̃e�N�X�`���p�X�ݒ�
	//  �u�v���C�v��ǂݍ���
	Add("Play"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 250)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  �u�����Ă��v��ǂݍ���
	Add("Setting"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 350)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  �u�����v��ǂݍ���
	Add("Cancel"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 450)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  �u��������v��ǂݍ���
	Add("Guide"
		, Vector2(Screen::RIGHT, Screen::BOTTOM)
		, Vector2(1, 1)
		, KumachiLib::ANCHOR::BOTTOM_RIGHT
		, UIType::NON_SELECT);

}
/*
*	@brief	�X�V
*	@details �^�C�g�����j���[�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void TitleMenu::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();// �}�E�X�̃g���b�J�[���擾����
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();// �}�E�X�̏�Ԃ��擾
	m_hit = false;// �����Ƀq�b�g������
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));// �}�E�X�̍��W���擾
	for (int i = 0; i < m_pUI.size(); i++)// ���j���[�A�C�e���̐������J��Ԃ�
	{
		if (m_pUI[i]->IsHit(mousePos))// �}�E�X�̍��W���A�C�e���͈͓̔��ɂ��邩�ǂ����𔻒�
		{
			m_hit = true;	// �q�b�g�t���O�𗧂Ă�
			if ((int(m_menuIndex)) != i) m_isSEPlay = false;// �O��I���������j���[�ƈႤ�ꍇ��SE���Đ�����t���O�𗧂Ă�
			if (!m_isSEPlay)// SE���Đ�����Ă��Ȃ��ꍇ
			{
				m_pCommonResources->GetAudioManager()->PlaySound("Select", m_SEVolume);// SE�̍Đ�
				m_isSEPlay = true;// �Đ��t���O�𗧂Ă�
			}
			m_menuIndex = i;// �q�b�g�������j���[�̃C���f�b�N�X��ۑ�
			break;	// �q�b�g�����烋�[�v�𔲂���
		}
	}
	m_time += elapsedTime;// ���Ԃ����Z����
	if (!m_hit) { m_menuIndex = INVALID_MENU_INDEX; }// ��������q�b�g���ĂȂ�������A�I���Ȃ��ɂ���
	if (mtracker->GetLastState().leftButton)m_num = static_cast<SceneID>(m_menuIndex);// ���N���b�N���ꂽ��I�����j���[�̃V�[��ID���X�V
	for (int i = 0; i < m_pUI.size(); i++)// ���j���[�A�C�e���̑I�����X�V
	{
		//  �A�C�e���̑I����Ԃ��X�V
		m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());// �I����Ԃ̃X�P�[�����擾
		m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);// �A�C�e���̑I����Ԃ��X�V
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());// �I����Ԃ̃X�P�[�����擾
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);// �A�C�e���̑I����Ԃ��X�V
	}
	for (int i = 0; i < m_pGuide.size(); i++)// �I��s�\�ȃA�C�e���̑I����Ԃ��X�V
	{
		m_pGuide[i]->SetScale(m_pGuide[i]->GetSelectScale());// �X�P�[�����擾
		m_pGuide[i]->SetTime(m_pGuide[i]->GetTime() + elapsedTime);// ���Ԃ����Z
	}
	if (m_menuIndex == INVALID_MENU_INDEX)return;// �Ȃɂ��I������Ă��Ȃ��ꍇ�͉������Ȃ�
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();// �I�𒆂̏����T�C�Y���擾����
	Vector2 selectScale = Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), Vector2::One, 1);// �I����ԂƂ��邽�߂̕ω��p�T�C�Y���Z�o����
	select = Vector2((sin(m_time) * 0.1f) + 1.0f);// �I����Ԃ͏�����Ԃ��傫������
	m_pUI[m_menuIndex]->SetScale(select);// �Z�o��̃T�C�Y�����݂̃T�C�Y�Ƃ��Đݒ肷��
	m_pSelect[m_menuIndex]->SetScale(Vector2::One);// �w�i�p�̃E�B���h�E�摜�ɂ����������̒l��ݒ肷��
}
/*
*	@brief	�`��
*	@detail �^�C�g����UI��`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void TitleMenu::Render()
{
	for (unsigned int i = 0; i < m_pUI.size(); i++)// �o�^����UI�̐����[�v
	{
		if (i == m_menuIndex) m_pSelect[i]->Render();//  �A�C�e���p�E�B���h�E�w�i��\��
		m_pUI[i]->Render();	//  ���ۂɕ\���������A�C�e���摜��\��
	}
	for (unsigned int i = 0; i < m_pGuide.size(); i++)m_pGuide[i]->Render();// �I��s�\�ȃA�C�e����\��
#ifdef _DEBUG// �f�o�b�O���[�h�̎��̂ݎ��s����
	auto debugString = m_pCommonResources->GetDebugString(); // �f�o�b�O����\������
	for (int i = 0; i < m_transforms.size(); i++)// �o�^����UI�̐����[�v
		debugString->AddString("Transform.Pos:%f,%f  Scale:%f,%f",//  UI�̍��W��\��
			m_transforms[i].position.x,// X���W
			m_transforms[i].position.y,// Y���W
			m_transforms[i].scale.x,	// X�X�P�[��
			m_transforms[i].scale.y);	// Y�X�P�[��
#endif
}
/*
	@brief ���j���[�A�C�e����ǉ�����
	@details �w��̉摜��UI���쐬���A�������X�g�ɒǉ����� (�I���Ȃ�w�i�g���쐬)
	@param key �摜�t�@�C���̃L�[
	@param position �`��ʒu
	@param scale �`��X�P�[��
	@param anchor �A���J�[�w��
	@param type ���j���[�̎�ށi�I����/�s�j
	@return �Ȃ�
*/
void TitleMenu::Add(const std::string& key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	KumachiLib::ANCHOR anchor, UIType type)
{
	std::unique_ptr<UI> userInterface = std::make_unique<UI>(m_pCommonResources);// ���j���[�Ƃ��ăA�C�e����ǉ�����
	userInterface->Create(m_pDR, key, position, scale, anchor);// �w�肳�ꂽ�摜��\�����邽�߂̃A�C�e�����쐬����
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);// �E�B���h�E�̃T�C�Y��ݒ肷��
	if (type == UIType::SELECT)// �I���\�ȃA�C�e���Ȃ�
	{
		m_pUI.push_back(std::move(userInterface));//  �I���\�A�C�e����V�����ǉ�

		std::unique_ptr<UI> base = std::make_unique<UI>(m_pCommonResources);//  �w�i�p�̃E�B���h�E�摜���ǉ�����
		base->Create(m_pDR, "Select", position, scale, anchor);// �w�肳�ꂽ�摜��\�����邽�߂̃A�C�e�����쐬����
		base->SetWindowSize(m_windowWidth, m_windowHeight);// �E�B���h�E�̃T�C�Y��ݒ肷��
		m_pSelect.push_back(std::move(base));// �w�i�p�̃A�C�e�����V�����ǉ�����
		m_transforms.push_back({ position, scale });// UI�̏���z��ɓo�^
	}
	else// �I��s�ȃA�C�e���Ȃ�
	{
		m_pGuide.push_back(std::move(userInterface));// �I��s�\�A�C�e����V�����ǉ�
		return;
	}
}
