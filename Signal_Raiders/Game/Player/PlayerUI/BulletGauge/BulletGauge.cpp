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
	m_pDR = resources->GetDeviceResources();// �f�o�C�X���\�[�X�擾
	m_windowWidth = width;// �E�B���h�E��
	m_windowHeight = height;// �E�B���h�E����
	Add(m_pGauge,// �Q�[�W�̒ǉ�
		L"Resources/Textures/BulletGauge.png"
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
	float bp = bulletPoint * 0.02f;// �e�̃|�C���g��0.02�{����
	bp = std::max(0.f, bp); // 0�����ɂȂ�Ȃ��悤�ɂ���
	m_pGauge->SetRenderRatio(bp);// �Q�[�W�̃����_�����O���ݒ�
}
/*
*	@brief �Q�[�W�̕`��
*	@details �Q�[�W��`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void BulletGauge::Render()
{
	m_pGauge->SetShaderType(PlayerUI::ShaderType::CIRCLE), m_pGauge->Render();// �V�F�[�_�[�^�C�v��ݒ肵�������ŕ`��
}
/*
*	@brief �v���C���[UI��ǉ�����
*	@details �v���C���[UI��ǉ�����
*	@param pPlayerUI �v���C���[UI�̃��j�[�N�|�C���^
*	@param path �e�N�X�`���̃p�X
*	@param position �v���C���[UI�̈ʒu
*	@param scale �v���C���[UI�̃X�P�[��
*	@param anchor �A���J�[�̎��
*/
void BulletGauge::Add(std::unique_ptr<PlayerUI>& pPlayerUI
	, const wchar_t* path
	, DirectX::SimpleMath::Vector2 position
	, DirectX::SimpleMath::Vector2 scale
	, KumachiLib::ANCHOR anchor)
{
	pPlayerUI = std::make_unique<PlayerUI>();// �v���C���[UI�̃��j�[�N�|�C���^�𐶐�
	// �v���C���[UI�̍쐬
	pPlayerUI->Create(m_pDR	// �f�o�C�X���\�[�X
		, path				// �e�N�X�`���̃p�X
		, position			// �v���C���[UI�̈ʒu
		, scale				// �v���C���[UI�̃X�P�[��
		, anchor);			// �A���J�[�̎��
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);// �E�B���h�E�T�C�Y��ݒ�
}