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
	m_pCommonResources = nullptr; // ���ʃ��\�[�X�̃|�C���^�[��nullptr�ɐݒ�
	m_pEnemyBullet = nullptr; // �e�̎�ނ��Ƃɏ�����ς��邽�߂̃|�C���^�[��nullptr�ɐݒ�
	m_pNormalBullet = nullptr; // �ʏ�e�̃|�C���^�[��nullptr�ɐݒ�
	m_pSpeedBullet = nullptr; // �����e�̃|�C���^�[��nullptr�ɐݒ�
	m_pSpecialBullet = nullptr; // ����e�̃|�C���^�[��nullptr�ɐݒ�
	m_pModel = nullptr; // �e�̃��f����nullptr�ɐݒ�
	m_pPixelShader.Reset(); // �s�N�Z���V�F�[�_�[�����Z�b�g
	m_bulletTrail.reset(); // �e�̋O�������Z�b�g
	m_pShooter = nullptr; // ���˂����G�̃|�C���^�[��nullptr�ɐݒ�
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
	m_pCommonResources = pCmmonResources;// ���ʃ��\�[�X��ݒ�
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X���擾
	m_pEnemyBullet = EnemyBulletFactory::CreateBullet(m_bulletType);// �t�@�N�g���Ő���
	m_pEnemyBullet->SetEnemyBullet(this);// �G�e�|�C���^�[��ݒ肷��
	m_pEnemyBullet->SetCommonResources(m_pCommonResources);// �e�ɋ��ʃ��\�[�X��ݒ�
	m_pEnemyBullet->SetSEVolume(m_seVolume);// SE���ʂ�ݒ�
	DrawCollision::Initialize(m_pCommonResources);// ���E���̏�����
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");// �e�p�̃s�N�Z���V�F�[�_�[�ǂݍ���
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pPixelShader.ReleaseAndGetAddressOf()));// �s�N�Z���V�F�[�_�[�̍쐬
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("EnemyBullet");// �}�l�[�W���[���烂�f�����擾
	m_bulletTrail = std::make_unique<Particle>(ParticleUtility::Type::ENEMYTRAIL, m_size);	// �e�̋O������
	m_bulletTrail->Initialize(pCmmonResources);// �e�̋O��������
	m_direction = Vector3::Zero;// ������������
	m_velocity = Vector3::Zero;// ���x��������
	m_position = Vector3::Zero;// �ʒu��������
	m_boundingSphere.Center = m_position;	// ���E���̈ʒu�ݒ�
	m_boundingSphere.Radius = m_size * 2;	// ���E���̔��a�ݒ�

}
/*
*	@brief �e�̔���
*	@details �e�̔��ˏ������s��
*	@param pos �e�̍��W
*	@param dir �e�̕���
*	@param target �e���ˎ��̃^�[�Q�b�g�̈ʒu
*	@return �Ȃ�
*/
void EnemyBullet::MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir, DirectX::SimpleMath::Vector3& target)
{
	using namespace DirectX::SimpleMath;
	m_position = pos;// �e�̍��W
	m_direction = dir;// �e�̕���
	m_target = target;// �e���ˎ��̃^�[�Q�b�g�̈ʒu
	m_pEnemyBullet->Initialize();// �e�̏�����
}

/*
*	@brief �e����������Ă���̌o�ߎ��Ԃ������𒴂������ǂ����𔻒肷��
*	@details �e�̎�ނɂ���Ď����𔻒肷��
*	@param �Ȃ�
*	@return true:�����𒴂��� false:�����𒴂��Ă��Ȃ�
*/
bool EnemyBullet::IsExpired() const
{
	if (m_bulletType == BulletType::SPECIAL)// �e�̎�ނ�����e�̏ꍇ
		return GetTime() >= BulletParameters::SPIRAL_BULLET_LIFETIME;// ����e�̎����𒴂������ǂ���
	else
		return GetTime() >= BulletParameters::ENEMY_BULLET_LIFETIME;// ����e�ȊO�̎����𒴂������ǂ���
}

/*
*	@brief �e�̍X�V
*	@details �e�̍X�V�������s��
*	@param elapsedTime �X�V����
*	@return �Ȃ�
*/
void EnemyBullet::Update(float elapsedTime)
{
	m_angle += BulletParameters::BULLET_ROTATION_SPEED;	// �p�x�𑝉�������
	Clamp(m_angle, BulletParameters::ANGLE_MIN, BulletParameters::ANGLE_MAX);// �p�x��0�`360�x�ɐ�������
	m_pEnemyBullet->Update(elapsedTime);	// �e�̍X�V
	m_bulletTrail->SetBulletPosition(m_position);	// ���݂̒e�̈ʒu���O�Ճ��X�g�ɒǉ�
	m_bulletTrail->Update(elapsedTime);	// �O�Ղ̍X�V
	m_time += elapsedTime;	// �o�ߎ��Ԃ��X�V
}
/*
*	@brief �e�̕`��
*	@details �e�̕`�揈�����s��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void EnemyBullet::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g���擾
	auto states = m_pCommonResources->GetCommonStates();// �R�����X�e�[�g���擾
	m_bulletTrail->CreateBillboard(m_cameraTarget, m_cameraEye, m_cameraUp);// �r���{�[�h���쐬
	m_bulletTrail->Render(view, proj);	// �O�Օ`��
	m_pModel->Draw(context, *states, BulletWorldMatrix(), view, proj);	// �e�`��
}
/*
*	@brief �e�̉e�`��
*	@details �e�̉e�`�揈�����s��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void EnemyBullet::RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g���擾
	auto states = m_pCommonResources->GetCommonStates();// �R�����X�e�[�g���擾
	Vector3 lightDir = Vector3::UnitY;	// ���C�g�̕���
	lightDir.Normalize();// ���K��
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, BulletParameters::SHADOW_PLANE);	// �e�s��̌������
	shadowMatrix = BulletWorldMatrix() * shadowMatrix;	// �e�s������[���h�s��ɓK�p����
	m_pModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()	// �e�`��
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);// �u�����h�X�e�[�g��ݒ�
			context->OMSetDepthStencilState(states->DepthNone(), 0);// �[�x�X�e���V���X�e�[�g��ݒ�
			context->RSSetState(states->CullClockwise());	// �J�����O��ݒ�
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);	// �s�N�Z���V�F�[�_�[��ݒ�
		});
}
/*
*	@brief �e�̋��E����`��(�f�o�b�O�p)
*	@details �e�̋��E����`�悷��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void EnemyBullet::RenderBoundingSphere(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);// ���g�p�ϐ��̌x����}��
	UNREFERENCED_PARAMETER(proj);// ���g�p�ϐ��̌x����}��
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
#ifdef _DEBUG// �f�o�b�O�r���h���̂ݎ��s
	DrawCollision::DrawStart(view, proj);// ���E���`��J�n
	Matrix boundingbulletWorld = Matrix::CreateScale(Vector3::One);// ���E���̃��[���h�s��ɃX�P�[����ݒ�
	boundingbulletWorld *= Matrix::CreateTranslation(m_position);// �e�̍��W��ݒ�
	DirectX::BoundingSphere transformedBoundingSphere = m_boundingSphere;// ���E���̃��[���h�s���ݒ�
	m_boundingSphere.Transform(transformedBoundingSphere, boundingbulletWorld);// ���E���̈ړ�
	DrawCollision::DrawBoundingSphere(m_boundingSphere, Colors::Red);// ���E���`��
	DrawCollision::DrawEnd();// ���E���`��I��
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
	Matrix bulletWorld = Matrix::CreateScale(m_size);	// �e�̃T�C�Y��ݒ�
	bulletWorld *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angle));	// �e�̎��]
	bulletWorld *= Matrix::CreateTranslation(m_position);	// �e�̍��W��ݒ�
	return bulletWorld;// �e�̃��[���h�s���Ԃ�
}

