/*
*	@file BulletGauge.cpp
*	@brief �e�Q�[�W�N���X
*/
#include <pch.h>
#include "BulletGauge.h"
/*
*	@brief �R���X�g���N�^
*	@details �e�Q�[�W�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
BulletGauge::BulletGauge()
	: m_windowHeight(0)// �E�B���h�E�̍���
	, m_windowWidth(0)// �E�B���h�E�̕�
	, m_pDR(nullptr) // �f�o�C�X���\�[�X�ւ̃|�C���^
	, m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_baseTexturePath(nullptr) // �x�[�X�e�N�X�`���̃p�X
	, m_pGauge(nullptr) // �Q�[�W�̃|�C���^
{
}
/*
*	@brief �f�X�g���N�^
*	@details �e�Q�[�W�N���X�̃f�X�g���N�^(�����ł͉������Ȃ�)
*	@param �Ȃ�
*	@return �Ȃ�
*/
BulletGauge::~BulletGauge() {/*do nothing*/ }
/*
*	@brief ���j���[������������
*	@details ���j���[�A�C�e���̉摜��ǂݍ��݁A�ʒu�E�X�P�[���E�A���J�[��ݒ肷��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void BulletGauge::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X��ݒ�
	m_pCommonResources = resources;
	// �f�o�C�X���\�[�X�擾
	m_pDR = resources->GetDeviceResources();
	// �E�B���h�E��
	m_windowWidth = width;
	// �E�B���h�E����
	m_windowHeight = height;
	// �Q�[�W�̒ǉ�
	Add(m_pGauge
		, "BulletGauge"
		, Vector2(640, 360)
		, Vector2(0.8f, 0.8f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);

}
/*
*	@brief �Q�[�W�̍X�V
*	@details �Q�[�W�̒l���X�V����
*	@param bulletPoint �e�̃|�C���g(0.0f����1.0f�͈̔�)
*	@return �Ȃ�
*/
void BulletGauge::Update(float bulletPoint)
{
	// �e�̃|�C���g��0.02�{����
	float bp = bulletPoint * 0.02f;
	// 0�����ɂȂ�Ȃ��悤�ɂ���
	bp = std::max(0.f, bp);
	// �Q�[�W�̃����_�����O���ݒ�
	m_pGauge->SetRenderRatio(bp);
}
/*
*	@brief �Q�[�W�̕`��
*	@details �Q�[�W��`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void BulletGauge::Render()
{
	// �V�F�[�_�[�^�C�v��ݒ肵�������ŕ`��
	m_pGauge->SetShaderType(PlayerUI::ShaderType::CIRCLE), m_pGauge->Render();
}
/*
*	@brief �v���C���[UI��ǉ�����
*	@details �v���C���[UI��ǉ�����
*	@param pPlayerUI �v���C���[UI�̃��j�[�N�|�C���^
*	@param key �e�N�X�`���̃L�[
*	@param position �v���C���[UI�̈ʒu
*	@param scale �v���C���[UI�̃X�P�[��
*	@param anchor �A���J�[�̎��
*/
void BulletGauge::Add(std::unique_ptr<PlayerUI>& pPlayerUI
	, const std::string& key
	, const DirectX::SimpleMath::Vector2& position
	, const DirectX::SimpleMath::Vector2& scale
	, KumachiLib::ANCHOR anchor)
{
	// �v���C���[UI�̃��j�[�N�|�C���^�𐶐�
	pPlayerUI = std::make_unique<PlayerUI>(m_pCommonResources);
	// �v���C���[UI�̍쐬
	pPlayerUI->Create(m_pDR	// �f�o�C�X���\�[�X
		, key				// �e�N�X�`���̃p�X
		, position			// �v���C���[UI�̈ʒu
		, scale				// �v���C���[UI�̃X�P�[��
		, anchor);			// �A���J�[�̎��
	// �E�B���h�E�T�C�Y��ݒ�
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);
}