/*
*	@file	GameEndChecker.cpp
*	@brief	�Q�[���I���O�ɍĊm�F����UI�N���X
*/
#include <pch.h>
#include "GameEndChecker.h"
// �����ȃ��j���[�C���f�b�N�X
const int GameEndChecker::INVALID_MENU_INDEX = 6;
/*
*	@brief �R���X�g���N�^
*	@details �Ċm�F�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
GameEndChecker::GameEndChecker()
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
	, m_isEndGame{ false }// �Q�[������߂邩�ǂ���
	, m_isStageSelect{ false }// �X�e�[�W��I�тȂ������ǂ����̃t���O
	, m_isGameEndCheck{ false }// ���̃N���X�̏������s�����̃t���O
	, m_SEVolume{ 0.0f }// SE����
	, m_isSEPlay{ false }// SE�Đ��t���O
{
}
/*
*	@brief �f�X�g���N�^
*	@details �Ċm�F�N���X�̃f�X�g���N�^(�����ł͉������Ȃ�)
*	@param �Ȃ�
*	@return �Ȃ�
*/
GameEndChecker::~GameEndChecker() {/*do nothing*/ }
/*
*	@brief ���j���[������������
*	@details ���j���[�A�C�e���̉摜��ǂݍ��݁A�ʒu�E�X�P�[���E�A���J�[��ݒ肷��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void GameEndChecker::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X���Z�b�g
	m_pCommonResources = resources;
	// �f�o�C�X���\�[�X�擾
	m_pDR = m_pCommonResources->GetDeviceResources();
	// �E�B���h�E��
	m_windowWidth = width;
	// �E�B���h�E����
	m_windowHeight = height;
	// �v���C�V�[���̏ꍇ�͔w�i��ς���
	std::string backgroundTexture = m_isStageSelect ? "ToStageSelectBack" : "GameEndBack";
	// �u�w�i�v��ǂݍ���
	Add(backgroundTexture
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y)
		, Vector2(1.0f, 1.0f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	// �u�Q�[���ɖ߂�v��ǂݍ���
	Add("BackGame"
		, Vector2(Screen::CENTER_X - 250, Screen::CENTER_Y + 250)
		, Vector2(1.0f, 1.0f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	// �v���C�V�[���̏ꍇ��UI�̕�����ς���
	std::string buttonText = m_isStageSelect ? "ReSelectStage" : "EndGame";
	// �u�Q�[�����I������v���u�X�e�[�W��I�тȂ����v��ǂݍ���
	Add(buttonText
		, Vector2(Screen::CENTER_X + 250, Screen::CENTER_Y + 250)
		, Vector2(1.0f, 1.0f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	// �u��������v��ǂݍ���
	Add("Guide"
		, Vector2(Screen::RIGHT, Screen::BOTTOM)
		, Vector2(1.0f, 1.0f)
		, KumachiLib::ANCHOR::BOTTOM_RIGHT
		, UIType::NON_SELECT);

}
/*
*	@brief	�X�V
*	@details �Ċm�F���j���[�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void GameEndChecker::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �}�E�X�̃g���b�J�[���擾����
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();
	// �}�E�X�̏�Ԃ��擾
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// �����Ƀq�b�g������
	m_hit = false;
	// �}�E�X�̍��W���擾
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// ���j���[�A�C�e���̐������J��Ԃ�
	for (int i = 0; i < m_pUI.size(); i++)
	{
		// �}�E�X�̍��W���A�C�e���͈͓̔��ɂ��邩�ǂ����𔻒�
		if (m_pUI[i]->IsHit(mousePos))
		{
			// �q�b�g�t���O�𗧂Ă�
			m_hit = true;
			// �O��I���������j���[�ƈႤ�ꍇ��SE���Đ�����t���O�𗧂Ă�
			if ((int(m_menuIndex)) != i) m_isSEPlay = false;
			// SE���Đ�����Ă��Ȃ��ꍇ
			if (!m_isSEPlay)
			{
				// SE�̍Đ�
				m_pCommonResources->GetAudioManager()->PlaySound("Select", m_SEVolume);
				// �Đ��t���O�𗧂Ă�
				m_isSEPlay = true;
			}
			// �q�b�g�������j���[�̃C���f�b�N�X��ۑ�
			m_menuIndex = i;
			// �q�b�g�����烋�[�v�𔲂���
			break;
		}
	}
	// ���Ԃ����Z����
	m_time += elapsedTime;
	// ��������q�b�g���ĂȂ�������A�I���Ȃ��ɂ���
	if (!m_hit) { m_menuIndex = INVALID_MENU_INDEX; }
	// ���N���b�N���ꂽ��I�����j���[�̃V�[��ID���X�V
	if (mtracker->GetLastState().leftButton && m_hit)
	{
		// �I�𒆂̃��j���[�A�C�e�����u�Q�[���ɖ߂�v�������ꍇ
		if (m_menuIndex == 0)
		{
			// �Q�[������߂Ȃ��t���O�𗧂Ă�
			m_isEndGame = false;
		}
		// �I�𒆂̃��j���[�A�C�e�����u�Q�[�����I������v�������ꍇ
		else
		{
			// �Q�[������߂�t���O�𗧂Ă�
			m_isEndGame = true;
		}

	}
	// �I��s�\�ȃA�C�e���̑I����Ԃ��X�V
	for (int i = 0; i < m_pNonSelectUI.size(); i++)
	{
		// �X�P�[�����擾
		m_pNonSelectUI[i]->SetScale(m_pNonSelectUI[i]->GetSelectScale());
		// ���Ԃ����Z
		m_pNonSelectUI[i]->SetTime(m_pNonSelectUI[i]->GetTime() + elapsedTime);
	}
	// ���j���[�A�C�e���̑I�����X�V
	for (int i = 0; i < m_pUI.size(); i++)
	{
		//  �A�C�e���̑I����Ԃ��X�V
		// �I����Ԃ̃X�P�[�����擾
		m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());
		// �A�C�e���̑I����Ԃ��X�V
		m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);
		// �I����Ԃ̃X�P�[�����擾
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());
		// �A�C�e���̑I����Ԃ��X�V
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);
	}

	// �Ȃɂ��I������Ă��Ȃ��ꍇ
	if (m_menuIndex == INVALID_MENU_INDEX)
	{
		// �I���\UI�̃T�C�Y��������Ԃɖ߂�
		for (unsigned int i = 0; i < m_pUI.size(); i++)
		{
			// �I����Ԃ̃X�P�[����������Ԃɖ߂�
			m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());
			// �w�i�p�̃E�B���h�E�摜��������Ԃɖ߂�
			m_pSelect[i]->SetScale(Vector2::One);
		}
		// ����ȍ~�̏��������Ȃ�
		return;
	}
	// �I�𒆂̏����T�C�Y���擾����
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();
	// �I����ԂƂ��邽�߂̕ω��p�T�C�Y���Z�o����
	Vector2 selectScale = Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), Vector2::One, 1);
	// �I����Ԃ͏�����Ԃ��傫������
	select = Vector2((sin(m_time) * 0.1f) + 1.0f);
	// �Z�o��̃T�C�Y�����݂̃T�C�Y�Ƃ��Đݒ肷��
	m_pUI[m_menuIndex]->SetScale(select);
	// �w�i�p�̃E�B���h�E�摜�ɂ����������̒l��ݒ肷��
	m_pSelect[m_menuIndex]->SetScale(Vector2::One);
}
/*
*	@brief	�`��
*	@detail �Ċm�FUI��`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void GameEndChecker::Render()
{
	// �I��s�\�ȃA�C�e����\��
	for (unsigned int i = 0; i < m_pNonSelectUI.size(); i++)m_pNonSelectUI[i]->Render();
	// �o�^����UI�̐����[�v
	for (unsigned int i = 0; i < m_pUI.size(); i++)
	{
		// �A�C�e���p�E�B���h�E�w�i��\��
		if (i == m_menuIndex) m_pSelect[i]->Render();
		// ���ۂɕ\���������A�C�e���摜��\��
		m_pUI[i]->Render();
	}

#ifdef _DEBUG// �f�o�b�O���[�h�̎��̂ݎ��s����
	// �f�o�b�O����\������
	auto debugString = m_pCommonResources->GetDebugString();
	// �o�^����UI�̐����[�v
	for (int i = 0; i < m_transforms.size(); i++)
	{
		//  UI�̍��W��\��
		debugString->AddString("Transform.Pos:%f,%f  Scale:%f,%f",
			m_transforms[i].position.x,// X���W
			m_transforms[i].position.y,// Y���W
			m_transforms[i].scale.x,	// X�X�P�[��
			m_transforms[i].scale.y);	// Y�X�P�[��
		// ���j���[�C���f�b�N�X��\��
		debugString->AddString("MenuIndex:%d", m_menuIndex);
	}
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
void GameEndChecker::Add(const std::string& key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	KumachiLib::ANCHOR anchor, UIType type)
{
	// �V����UI�A�C�e���𐶐�
	std::unique_ptr<UI> userInterface = std::make_unique<UI>(m_pCommonResources);
	// UI�A�C�e���ɑ΂��ĉ摜�E�ʒu�E�X�P�[���E�A���J�[����ݒ肷��
	userInterface->Create(m_pDR, key, position, scale, anchor);
	// �E�B���h�E�T�C�Y��UI�A�C�e���ɐݒ�
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);
	// UI�̎�ނɉ����ď����𕪊򂷂�
	if (type == UIType::SELECT)// UI���I���\�ȃ^�C�v�������ꍇ
	{
		// ���C����UI�A�C�e����I���\UI���X�g�ɒǉ�����
		m_pUI.push_back(std::move(userInterface));
		// �w�i�p�̃E�B���h�EUI�𐶐�
		std::unique_ptr<UI> base = std::make_unique<UI>(m_pCommonResources);
		// �w�i�p�E�B���h�E�ɉ摜�ƈʒu���Ȃǂ�ݒ肷��
		base->Create(m_pDR, "Select", position, scale, anchor);
		// �w�i�p�E�B���h�E�̃T�C�Y���ݒ肷��
		base->SetWindowSize(m_windowWidth, m_windowHeight);
		// �w�i�E�B���h�E��I��UI�w�i���X�g�ɒǉ�����
		m_pSelect.push_back(std::move(base));
		// ����UI�A�C�e���̈ʒu�ƃX�P�[�����g�����X�t�H�[�����Ƃ��ċL�^����
		m_transforms.push_back({ position, scale });
	}
	else // �I���ł��Ȃ�UI
	{
		// �I��s��UI���X�g�ɒǉ�����
		m_pNonSelectUI.push_back(std::move(userInterface));
		// �������I����
		return;
	}
}
