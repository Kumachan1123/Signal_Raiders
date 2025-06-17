/*
*	@file	EnemyBullet.cpp
*	@brief	�G�̒e�N���X
*/
#include <pch.h>
#include "EnemyBullet.h"
/*
*	@brief �R���X�g���N�^
*	@details �G�̒e�N���X�̃R���X�g���N�^
*	@param size �e�̃T�C�Y
*	@return �Ȃ�
*/
EnemyBullet::EnemyBullet(float size)
	: m_position{}// �e�̍��W
	, m_velocity{}// �e�̑���
	, m_pCommonResources{}// ���ʃ��\�[�X
	, m_time{ 0.0f }// �o�ߎ���
	, m_elapsedTime{ 0.0f }// �X�V����
	, m_angle{ 0.0f }// �e�̎��]
	, m_size{ size }// �e�̃T�C�Y
	, m_spiralAngle{ 0.0f }// ����e�̉�]�p�x
	, m_bulletSpeed{ 0.0f }// �e�̑��x
	, m_distance{ 5.0f }// �e�̓G�Ƃ̋���
	, m_rotateDirection{ 1 }// ��]�����i�����e�p:+�Ȃ�E���A-�Ȃ獶���j
	, m_rotationSpeed{ 0.0f }// ��]���x
	, m_height{ 0.0f }// ����
	, m_isExpand{ false }// �W�J�����t���O
	, m_isShot{ false }// ���˃t���O
	, m_pShooter{ nullptr }// ���˂����G�̃|�C���^�[
	, m_pModel{ nullptr }// �e�̃��f��
	, m_pSpecialBullet{ nullptr }// ����e
	, m_pNormalBullet{ nullptr }// �ʏ�e
	, m_pSpeedBullet{ nullptr }// �����e
	, m_pEnemyBullet{ nullptr }// �e�̎�ނ��Ƃɏ�����ς��邽�߂̃|�C���^�[
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X�̃|�C���^�[
	, m_bulletType{ BulletType::NORMAL }// �e�̎��
	, m_seVolume{ 0.0f }// SE����
{
}
/*
*	@brief �f�X�g���N�^
*	@details �e��|�C���^�[��nullptr�ɐݒ�
*	@param �Ȃ�
*	@return �Ȃ�
*/
EnemyBullet::~EnemyBullet()
{
	// ���ʃ��\�[�X�̃|�C���^�[��nullptr�ɐݒ�
	m_pCommonResources = nullptr;
	// �e�̃��f����nullptr�ɐݒ�
	m_pModel = nullptr;
	// �s�N�Z���V�F�[�_�[�����Z�b�g
	m_pPixelShader.Reset();
	// �e�̋O�������Z�b�g
	m_bulletTrail.reset();
	// ���˂����G�̃|�C���^�[��nullptr�ɐݒ�
	m_pShooter = nullptr;
}
/*
*	@brief ������
*	@details �G�̒e�N���X�̏�����
*	@param pCmmonResources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void EnemyBullet::Initialize(CommonResources* pCmmonResources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X��ݒ�
	m_pCommonResources = pCmmonResources;
	// �f�o�C�X���擾
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// �V�F�[�_�[�쐬�N���X��������
	m_pCreateShader->Initialize(device);
	// �t�@�N�g���Ő���
	m_pEnemyBullet = EnemyBulletFactory::CreateBullet(m_bulletType);
	// �G�e�|�C���^�[��ݒ肷��
	m_pEnemyBullet->SetEnemyBullet(this);
	// �e�ɋ��ʃ��\�[�X��ݒ�
	m_pEnemyBullet->SetCommonResources(m_pCommonResources);
	// SE���ʂ�ݒ�
	m_pEnemyBullet->SetSEVolume(m_seVolume);
	// ���E���̏�����
	DrawCollision::Initialize(m_pCommonResources);
	// �e�p�V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Shadow/PS_Shadow.cso", m_pPixelShader);
	// �}�l�[�W���[���烂�f�����擾
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("EnemyBullet");
	// �e�̋O������
	m_bulletTrail = std::make_unique<Particle>(ParticleUtility::Type::ENEMYTRAIL, m_size);
	// �e�̋O��������
	m_bulletTrail->Initialize(pCmmonResources);
	// ������������
	m_direction = Vector3::Zero;
	// ���x��������
	m_velocity = Vector3::Zero;
	// �ʒu��������
	m_position = Vector3::Zero;
	// ���E���̈ʒu�ݒ�
	m_boundingSphere.Center = m_position;
	// ���E���̔��a�ݒ�
	m_boundingSphere.Radius = m_size * 2;

}
/*
*	@brief �e�̔���
*	@details �e�̔��ˏ������s��
*	@param pos �e�̍��W
*	@param dir �e�̕���
*	@param target �e���ˎ��̃^�[�Q�b�g�̈ʒu
*	@return �Ȃ�
*/
void EnemyBullet::MakeBall(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir, const DirectX::SimpleMath::Vector3& target)
{
	using namespace DirectX::SimpleMath;
	// �e�̍��W
	m_position = pos;
	// �e�̕���
	m_direction = dir;
	// �e���ˎ��̃^�[�Q�b�g�̈ʒu
	m_target = target;
	// �e�̏�����
	m_pEnemyBullet->Initialize();
}

/*
*	@brief �e����������Ă���̌o�ߎ��Ԃ������𒴂������ǂ����𔻒肷��
*	@details �e�̎�ނɂ���Ď����𔻒肷��
*	@param �Ȃ�
*	@return true:�����𒴂��� false:�����𒴂��Ă��Ȃ�
*/
bool EnemyBullet::IsExpired() const
{
	// �e�̎�ނ�����e�̏ꍇ
	if (m_bulletType == BulletType::SPECIAL)
	{
		// ����e�̎����𒴂������ǂ���
		return GetTime() >= BulletParameters::SPIRAL_BULLET_LIFETIME;
	}
	else// ����e�ȊO�̏ꍇ
	{
		// ����e�ȊO�̎����𒴂������ǂ���
		return GetTime() >= BulletParameters::ENEMY_BULLET_LIFETIME;
	}
}

/*
*	@brief �e�̍X�V
*	@details �e�̍X�V�������s��
*	@param elapsedTime �X�V����
*	@return �Ȃ�
*/
void EnemyBullet::Update(float elapsedTime)
{
	// �p�x�𑝉�������
	m_angle += BulletParameters::BULLET_ROTATION_SPEED;
	// �p�x��0�`360�x�ɐ�������
	Clamp(m_angle, BulletParameters::ANGLE_MIN, BulletParameters::ANGLE_MAX);
	// �e�̍X�V
	m_pEnemyBullet->Update(elapsedTime);
	// ���݂̒e�̈ʒu���O�Ճ��X�g�ɒǉ�
	m_bulletTrail->SetBulletPosition(m_position);
	// �O�Ղ̍X�V
	m_bulletTrail->Update(elapsedTime);
	// �o�ߎ��Ԃ��X�V
	m_time += elapsedTime;
}
/*
*	@brief �e�̕`��
*	@details �e�̕`�揈�����s��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void EnemyBullet::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// �f�o�C�X�R���e�L�X�g���擾
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �R�����X�e�[�g���擾
	auto states = m_pCommonResources->GetCommonStates();
	// �r���{�[�h���쐬
	m_bulletTrail->CreateBillboard(m_cameraTarget, m_cameraEye, m_cameraUp);
	// �O�Օ`��
	m_bulletTrail->Render(view, proj);
	// �e�`��
	m_pModel->Draw(context, *states, BulletWorldMatrix(), view, proj);
}
/*
*	@brief �e�̉e�`��
*	@details �e�̉e�`�揈�����s��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void EnemyBullet::RenderShadow(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// �f�o�C�X�R���e�L�X�g���擾
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �R�����X�e�[�g���擾
	auto states = m_pCommonResources->GetCommonStates();
	// ���C�g�̕���
	Vector3 lightDir = Vector3::UnitY;
	// ���K��
	lightDir.Normalize();
	// �e�s��̌������
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, BulletParameters::SHADOW_PLANE);
	// �e�s������[���h�s��ɓK�p����
	shadowMatrix = BulletWorldMatrix() * shadowMatrix;
	// �e�`��
	m_pModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()
		{
			// �u�����h�X�e�[�g��ݒ�
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
			// �[�x�X�e���V���X�e�[�g��ݒ�
			context->OMSetDepthStencilState(states->DepthNone(), 0);
			// �J�����O��ݒ�
			context->RSSetState(states->CullClockwise());
			// �s�N�Z���V�F�[�_�[��ݒ�
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
		});
}
/*
*	@brief �e�̋��E����`��(�f�o�b�O�p)
*	@details �e�̋��E����`�悷��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void EnemyBullet::RenderBoundingSphere(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// ���g�p�ϐ��̌x����}��
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(proj);
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
#ifdef _DEBUG// �f�o�b�O�r���h���̂ݎ��s
	// ���E���`��J�n
	DrawCollision::DrawStart(view, proj);
	// ���E���̃��[���h�s��ɃX�P�[����ݒ�
	Matrix boundingbulletWorld = Matrix::CreateScale(Vector3::One);
	// �e�̍��W��ݒ�
	boundingbulletWorld *= Matrix::CreateTranslation(m_position);
	// ���E���̃��[���h�s���ݒ�
	DirectX::BoundingSphere transformedBoundingSphere = m_boundingSphere;
	// ���E���̈ړ�
	m_boundingSphere.Transform(transformedBoundingSphere, boundingbulletWorld);
	// ���E���`��
	DrawCollision::DrawBoundingSphere(m_boundingSphere, Colors::Red);
	// ���E���`��I��
	DrawCollision::DrawEnd();
#endif
}


/*
*	@brief �e�̃��[���h�s����쐬
*	@details �e�̃��[���h�s����쐬����
*	@param �Ȃ�
*	@return DirectX::SimpleMath::Matrix �e�̃��[���h�s��
*/
DirectX::SimpleMath::Matrix EnemyBullet::BulletWorldMatrix()
{
	using namespace DirectX::SimpleMath;
	// �e�̃T�C�Y��ݒ�
	Matrix bulletWorld = Matrix::CreateScale(m_size);
	// �e�̎��]
	bulletWorld *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angle));
	// �e�̍��W��ݒ�
	bulletWorld *= Matrix::CreateTranslation(m_position);
	// �e�̃��[���h�s���Ԃ�
	return bulletWorld;
}

