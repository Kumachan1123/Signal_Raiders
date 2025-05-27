/*
*	@file	PlayerController.cpp
*	@brief	�v���C���[�R���g���[���[�N���X
*/
#include "pch.h"
#include "PlayerController.h"
const float PlayerController::MOVE_SPEED = 5.0f;// �ʏ�ړ����x
const float PlayerController::DASH_SPEED = 15.0f;// �_�b�V���ړ����x
const float PlayerController::DASH_TIME = 10.0f;// �_�b�V������
const float PlayerController::GRAVITY = 0.75f;// �d��
const float PlayerController::PITCH_LIMIT = 80.0f;// �s�b�`�̐���
const float PlayerController::MOUSE_SENSITIVE = 0.0025f;// �}�E�X���x
const float PlayerController::MOUSE_X_SENSITIVE = 10.0f;// �}�E�XX�����x
const float PlayerController::MOUSE_Y_SENSITIVE = 100.0f;// �}�E�XY�����x
/*
*	@brief �R���X�g���N�^
*	@details �v���C���[�R���g���[���[�N���X�̃R���X�g���N�^
*	@param pPlayer �v���C���[�̃|�C���^�[
*	@return �Ȃ�
*/
PlayerController::PlayerController(Player* pPlayer)
	: m_pPlayer{ pPlayer }// �v���C���[�̃|�C���^�[
	, m_position{}// �v���C���[�̈ʒu
	, m_velocity{}// �v���C���[�̈ړ��x�N�g��
	, m_yaw{}// �J������X����]
	, m_pitch{}// �J������Y����]
	, m_pitchLimit{ PlayerController::PITCH_LIMIT }// �s�b�`�̐���
	, m_dash{}// �_�b�V�����x
	, m_pCommonResources{}// ���ʃ��\�[�X
	, m_hWnd{ nullptr }// �E�B���h�E�n���h��
	, m_sensitive{ pPlayer->GetMouseSensitive() + 1.0f }// �}�E�X���x:Player->GetMouseSensitive()�̂܂܂��ƍŏ��l��0.0f�ɂȂ�̂� + 1.0f����
	, m_rotate{}// �v���C���[�̉�]
	, m_dashTime{ PlayerController::DASH_TIME }// �_�b�V������
{
	RECT desktopRect;// �X�N���[���̉𑜓x���擾
	const HWND hDesktop = GetDesktopWindow();// �f�X�N�g�b�v�E�B���h�E�̃n���h�����擾
	GetWindowRect(hDesktop, &desktopRect);// �f�X�N�g�b�v�E�B���h�E�̃T�C�Y���擾
	// �X�N���[���̒������W���v�Z
	int centerX = (desktopRect.right - desktopRect.left) / 2;// ����X���W
	int centerY = (desktopRect.bottom - desktopRect.top) / 2;// ����Y���W
	// �}�E�X�ʒu�𒆉��ɐݒ�
	m_point = { centerX, centerY };// �}�E�X���W
	SetCursorPos(m_point.x, m_point.y);// �}�E�X�ʒu�𒆉��ɐݒ�
	// SetCursorPos(m_point.x, m_point.y)��int�^�̂��߁Afloat�^�ɃL���X�g���ĕۑ�
	m_lastMouseX = (float)(m_point.x);// �}�E�X��X���W
	m_lastMouseY = (float)(m_point.y);// �}�E�X��Y���W
}
/*
*	@brief �f�X�g���N�^
*	@details �v���C���[�R���g���[���[�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
PlayerController::~PlayerController() {/* do nothing. */ }
/*
*	@brief �v���C���[�̈ړ����~�߂�
*	@details �v���C���[�̈ړ����~�߂�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayerController::MoveStop()
{
	using namespace DirectX::SimpleMath;
	m_velocity = Vector3::Zero;// �ړ��x�N�g�����[���ɂ���
	m_velocity.y -= PlayerController::GRAVITY; // �d�͂�������
	m_yaw = 0.0f;// yaw��������
	m_pitch = 0.0f;	// pitch��������
	m_dash = PlayerController::MOVE_SPEED;// �ʏ푬�x
}
/*
*	@brief �v���C���[�R���g���[���[�̏�����
*	@details �v���C���[�R���g���[���[�̏�����(�R�������\�[�X��n��)
*	@param resources �R�������\�[�X
*	@return �Ȃ�
*/
void PlayerController::Initialize(CommonResources* resources) { m_pCommonResources = resources; }// ������
/*
*	@brief �v���C���[�̈ړ����X�V
*	@details WASD�ňړ��A�}�E�X�Ŏ��_��ύX����
*	@param elapsedTime �O��̃t���[������̌o�ߎ���
*	@return �Ȃ�
*/
void PlayerController::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	const auto& kbTracker = m_pCommonResources->GetInputManager()->GetKeyboardTracker();// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	MoveStop();// �ړ����~�߂�
	POINT point;// �}�E�X�̍��W
	GetCursorPos(&point);// �}�E�X�̈ʒu���擾
	ScreenToClient(m_hWnd, &point);// �E�B���h�E���W�ɕϊ�
	int mouseX = point.x;// �}�E�X��X���W
	int mouseY = point.y;// �}�E�X��Y���W
	// �}�E�X�̑��Έړ����v�Z
	float deltaX = static_cast<float>(mouseX - m_lastMouseX);// �}�E�X��X���W�̍���
	float deltaY = static_cast<float>(mouseY - m_lastMouseY);// �}�E�X��Y���W�̍���
	// �}�E�X���x�̓K�p
	const float mouseSensitivity = PlayerController::MOUSE_SENSITIVE * m_sensitive;// �}�E�X���x
	float x = deltaX * mouseSensitivity * PlayerController::MOUSE_X_SENSITIVE * elapsedTime;// �}�E�X��X���W�̍���
	float y = deltaY * mouseSensitivity * PlayerController::MOUSE_Y_SENSITIVE * elapsedTime;// �}�E�X��Y���W�̍���
	// ��]
	m_yaw -= x;// ���E��]�iyaw�j
	m_pitch -= y;// �㉺��]�ipitch�j
	float pitchLimit = XMConvertToRadians(m_pitchLimit);// �s�b�`�̐��������W�A���ɕϊ�
	m_pitch = Clamp(m_pitch, -pitchLimit, pitchLimit);// �㉺�̉�]����
	// �O��ړ�
	if (kbTracker->lastState.W)// W�L�[��������Ă���Ƃ�
	{
		// �v���C���[�̕����Ɉړ�����
		m_velocity.x += m_pPlayer->GetPlayerDir().x;// X������
		m_velocity.z += m_pPlayer->GetPlayerDir().z;// Z������
	}
	if (kbTracker->lastState.S)// S�L�[��������Ă���Ƃ�
	{
		// �J�����̌����Ƌt�̕����Ɉړ�����
		m_velocity.x -= m_pPlayer->GetPlayerDir().x;// X������
		m_velocity.z -= m_pPlayer->GetPlayerDir().z;// Z������
	}
	// �J�����̉E�����x�N�g�����v�Z����(�E����Ƃ���j
	Vector3 rightDirection;// �E�����x�N�g��
	rightDirection.x = m_pPlayer->GetPlayerDir().z;// X������
	rightDirection.z = -m_pPlayer->GetPlayerDir().x;// Z������
	// ���E�ړ�
	if (kbTracker->lastState.A)// A�L�[��������Ă���Ƃ�
	{
		// �J�����̍������Ɉړ����� 
		m_velocity.x += rightDirection.x;// X������
		m_velocity.z += rightDirection.z;// Z������
	}
	if (kbTracker->lastState.D)// D�L�[��������Ă���Ƃ�
	{
		// �J�����̉E�����Ɉړ�����
		m_velocity.x -= rightDirection.x;// X������
		m_velocity.z -= rightDirection.z;// Z������
	}
	// �_�b�V��
	if (kbTracker->lastState.LeftShift)// ��shift�L�[��������Ă���Ƃ�
	{
		if (m_dashTime > 0.0f)// �_�b�V�����Ԃ��c���Ă���Ƃ�
		{
			m_dash = PlayerController::DASH_SPEED;// �_�b�V�����x
			m_dashTime -= elapsedTime;// �_�b�V�����Ԃ����炷
		}
	}
	else// ��shift�L�[��������Ă��Ȃ��Ƃ�
	{
		m_dash = PlayerController::MOVE_SPEED;// �ʏ푬�x
		m_dashTime += elapsedTime / 2;// �_�b�V�����Ԃ𑝂₷
		if (m_dashTime >= PlayerController::DASH_TIME)// �_�b�V�����Ԃ�����𒴂����Ƃ�
			m_dashTime = PlayerController::DASH_TIME;// �_�b�V�����Ԃ̏��
	}
	if (m_velocity.LengthSquared() > 0)// �ړ��ʂ�����Ƃ�
		m_velocity.Normalize();// �ړ��ʂ𐳋K������
	m_velocity *= m_dash * elapsedTime;// �ړ��ʂ�␳����
	m_position += m_velocity;// ���ۂɈړ�����
	m_rotate = x;// ����]��ۑ�
	if (m_position.y <= Stage::STAGE_HEIGHT)m_position.y = Stage::STAGE_HEIGHT;// �n�ʂ̍����ɐ���
	// �n�ʂ̖ʐςɐ���
	if (m_position.x <= -Stage::STAGE_SIZE) m_position.x = -Stage::STAGE_SIZE;// ���[
	if (m_position.x >= Stage::STAGE_SIZE) m_position.x = Stage::STAGE_SIZE;// �E�[
	if (m_position.z <= -Stage::STAGE_SIZE) m_position.z = -Stage::STAGE_SIZE;// ��[
	if (m_position.z >= Stage::STAGE_SIZE) m_position.z = Stage::STAGE_SIZE;// ���[
	SetCursorPos(m_point.x, m_point.y);	// �}�E�X�ʒu�𒆉��ɖ߂�
}
/*
*	@brief �f�o�b�O�R�}���h
*	@details �f�o�b�O���̂ݎg�p�\�ȃ`�[�g�R�}���h
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayerController::DebugCommand()
{
	const auto& kbTracker = m_pCommonResources->GetInputManager()->GetKeyboardTracker();// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	auto& mTracker = m_pCommonResources->GetInputManager()->GetMouseTracker();// �}�E�X�̃g���b�J�[���擾����
	if (mTracker->GetLastState().rightButton && m_pPlayer->GetisCheat() == false)// �E�N���b�N�œG����|
	{
		m_pPlayer->SetisCheat(true);// �`�[�g�t���O�𗧂Ă�
		for (auto& enemy : m_pPlayer->GetEnemies()->GetEnemies())enemy->ApplyDamageToEnemy(1000);// �G��HP��1000���炷�i�����j
	}
	if (!mTracker->GetLastState().rightButton)// �E�N���b�N����ĂȂ��Ƃ�
		m_pPlayer->SetisCheat(false);	// �`�[�g�R�}���h�𖳌��ɂ���
	// �X�y�[�X�L�[�Ńv���C���[��HP��0�ɂ���
	if (kbTracker->pressed.Space)m_pPlayer->SetPlayerHP(0.0f);
}
/*
*	@brief �v���C���[���e������
*	@details ���N���b�N�Œe�𔭎�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayerController::Shoot()
{
	auto& mTracker = m_pCommonResources->GetInputManager()->GetMouseTracker();// �}�E�X�̃g���b�J�[���擾����
	if (mTracker->GetLastState().leftButton &&// ���N���b�N���ꂽ�Ƃ�
		m_pPlayer->GetBulletManager()->GetIsPlayerShoot() == false &&// �e�����˂���Ă��Ȃ��Ƃ�
		m_pPlayer->GetBulletManager()->GetPlayerBulletCount() > 0 && // �e���c���Ă���Ƃ�
		m_pPlayer->GetBulletManager()->GetIsReloading() == false)// �����[�h���łȂ��Ƃ�
	{
		m_pPlayer->CreateBullet();// �e�𐶐�����
		m_pPlayer->GetBulletManager()->ConsumePlayerBullet();// �e�������
	}
	if (!mTracker->GetLastState().leftButton)// ���N���b�N����Ă��Ȃ��Ƃ�
		m_pPlayer->GetBulletManager()->SetIsPlayerShoot(false);// �e���������t���O�����낷
}
/*
*	@brief �v���C���[�̒e�������[�h����
*	@details R�L�[�Œe�������[�h����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayerController::Reload()
{
	const auto& kbState = m_pCommonResources->GetInputManager()->GetKeyboardState();// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	if (kbState.R)m_pPlayer->GetBulletManager()->SetIsReloading(true);// R�L�[�Ń����[�h�t���O�𗧂Ă�
	if (m_pPlayer->GetBulletManager()->GetIsReloading())// �����[�h���̂Ƃ� 
		m_pPlayer->GetBulletManager()->ReLoadPlayerBullet();// �e���[����
}
