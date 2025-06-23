/*
*	@file	PlayerController.cpp
*	@brief	�v���C���[�R���g���[���[�N���X
*/
#include <pch.h>
#include "PlayerController.h"
// �ʏ�ړ����x
const float PlayerController::MOVE_SPEED = 5.0f;
// �_�b�V���ړ����x
const float PlayerController::DASH_SPEED = 15.0f;
// �_�b�V������
const float PlayerController::DASH_TIME = 10.0f;
// �d��
const float PlayerController::GRAVITY = 0.75f;
// �s�b�`�̐���
const float PlayerController::PITCH_LIMIT = 80.0f;
// �}�E�X���x
const float PlayerController::MOUSE_SENSITIVE = 0.0025f;
// �}�E�XX�����x
const float PlayerController::MOUSE_X_SENSITIVE = 10.0f;
// �}�E�XY�����x
const float PlayerController::MOUSE_Y_SENSITIVE = 100.0f;
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
	, m_point{}// �}�E�X�̈ʒu
	, m_lastMouseX{ 0.0f }// �}�E�X��X���W
	, m_lastMouseY{ 0.0f }// �}�E�X��Y���W
	, m_sensitive{ pPlayer->GetMouseSensitive() + 1.0f }// �}�E�X���x:Player->GetMouseSensitive()�̂܂܂��ƍŏ��l��0.0f�ɂȂ�̂� + 1.0f����
	, m_rotate{}// �v���C���[�̉�]
	, m_dashTime{ PlayerController::DASH_TIME }// �_�b�V������
{

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
	// �ړ��x�N�g�����[���ɂ���
	m_velocity = Vector3::Zero;
	// �d�͂�������
	m_velocity.y -= PlayerController::GRAVITY;
	// yaw��������
	m_yaw = 0.0f;
	// pitch��������
	m_pitch = 0.0f;
	// �ʏ푬�x
	m_dash = PlayerController::MOVE_SPEED;
}
/*
*	@brief �v���C���[�R���g���[���[�̏�����
*	@details �v���C���[�R���g���[���[�̏�����(�R�������\�[�X��n��)
*	@param resources �R�������\�[�X
*	@return �Ȃ�
*/
void PlayerController::Initialize(CommonResources* resources)
{
	// �R�������\�[�X��ۑ�
	m_pCommonResources = resources;
	// window�n���h�����擾
	HWND hWnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// �E�B���h�E�n���h����ۑ�
	m_hWnd = hWnd;
	// �E�B���h�E���\������Ă��郂�j�^�[���擾
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	// ���j�^�[�����擾
	MONITORINFO mi = {};
	// ���j�^�[���̃T�C�Y��ݒ�
	mi.cbSize = sizeof(MONITORINFO);
	// ���j�^�[�����擾
	if (GetMonitorInfo(hMonitor, &mi))
	{
		// ���j�^�[�̈�̒���
		// X���W�̒���
		int centerX = (mi.rcMonitor.left + mi.rcMonitor.right) / 2;
		// Y���W�̒���
		int centerY = (mi.rcMonitor.top + mi.rcMonitor.bottom) / 2;
		// �}�E�X�̈ʒu�𒆉��ɐݒ�
		m_point = { centerX, centerY };
		// �}�E�X�̈ʒu���E�B���h�E���W�ɕϊ�
		SetCursorPos(centerX, centerY);
		// �N���C�A���g���W�Œ������擾
		POINT clientCenter = { centerX, centerY };
		// �N���C�A���g���W���E�B���h�E���W�ɕϊ�
		ClientToScreen(hWnd, &clientCenter);
		// �}�E�X��X���W��ۑ�
		m_lastMouseX = (float)clientCenter.x;
		// �}�E�X��Y���W��ۑ�
		m_lastMouseY = (float)clientCenter.y;
	}
}
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
	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kbTracker = m_pCommonResources->GetInputManager()->GetKeyboardTracker();
	// �ړ����~�߂�
	MoveStop();
	// �}�E�X�̍��W
	POINT point;
	// �}�E�X�̈ʒu���擾
	GetCursorPos(&point);
	// �E�B���h�E���W�ɕϊ�
	ScreenToClient(m_hWnd, &point);
	// �}�E�X��X���W���擾
	int mouseX = point.x;
	// �}�E�X��Y���W���擾
	int mouseY = point.y;
	// �}�E�X�̑��Έړ����v�Z
	// �}�E�X��X���W�̍���
	float deltaX = static_cast<float>(mouseX - m_lastMouseX);
	// �}�E�X��Y���W�̍���
	float deltaY = static_cast<float>(mouseY - m_lastMouseY);
	// �}�E�X���x��K�p
	const float mouseSensitivity = PlayerController::MOUSE_SENSITIVE * m_sensitive;
	// �}�E�X��X���W�̍������v�Z
	float x = deltaX * mouseSensitivity * PlayerController::MOUSE_X_SENSITIVE * elapsedTime;
	// �}�E�X��Y���W�̍������v�Z
	float y = deltaY * mouseSensitivity * PlayerController::MOUSE_Y_SENSITIVE * elapsedTime;
	// ���E��]�iyaw�j
	m_yaw -= x;
	// �㉺��]�ipitch�j
	m_pitch -= y;
	// �s�b�`�̐��������W�A���ɕϊ�
	float pitchLimit = XMConvertToRadians(m_pitchLimit);
	// �㉺�̉�]����
	m_pitch = Clamp(m_pitch, -pitchLimit, pitchLimit);
	// �O��ړ�
	// W�L�[��������Ă���Ƃ��A�O���ɐi��
	if (kbTracker->lastState.W)
	{
		// X�������Ɉړ�����
		m_velocity.x += m_pPlayer->GetPlayerDir().x;
		// Z�������Ɉړ�����
		m_velocity.z += m_pPlayer->GetPlayerDir().z;
	}
	// S�L�[��������Ă���Ƃ��A����ɐi��
	if (kbTracker->lastState.S)
	{
		// X�������Ɉړ�����
		m_velocity.x -= m_pPlayer->GetPlayerDir().x;
		// Z�������Ɉړ�����
		m_velocity.z -= m_pPlayer->GetPlayerDir().z;
	}
	// �J�����̉E�����x�N�g�����v�Z����(�E����Ƃ���j
	// �E�����x�N�g��
	Vector3 rightDirection;
	// X�������Ɉړ�����
	rightDirection.x = m_pPlayer->GetPlayerDir().z;
	// Z�������Ɉړ�����
	rightDirection.z = -m_pPlayer->GetPlayerDir().x;
	// ���E�ړ�
	// A�L�[��������Ă���Ƃ��A�������Ɉړ�����
	if (kbTracker->lastState.A)
	{
		// X�������Ɉړ�����
		m_velocity.x += rightDirection.x;
		// Z�������Ɉړ�����
		m_velocity.z += rightDirection.z;
	}
	// D�L�[��������Ă���Ƃ�, �E�����Ɉړ�����
	if (kbTracker->lastState.D)
	{
		// X�������Ɉړ�����
		m_velocity.x -= rightDirection.x;
		// Z�������Ɉړ�����
		m_velocity.z -= rightDirection.z;
	}
	// ��shift�L�[��������Ă���Ƃ��_�b�V��
	if (kbTracker->lastState.LeftShift)
	{
		// �_�b�V�����Ԃ��c���Ă���Ƃ�
		if (m_dashTime > 0.0f)
		{
			// �_�b�V�����x
			m_dash = PlayerController::DASH_SPEED;
			// �_�b�V�����Ԃ����炷
			m_dashTime -= elapsedTime;
		}
	}
	else// ��shift�L�[��������Ă��Ȃ��Ƃ�
	{
		// �ʏ푬�x
		m_dash = PlayerController::MOVE_SPEED;
		// �_�b�V�����Ԃ𑝂₷
		m_dashTime += elapsedTime / 2;
		// �_�b�V�����Ԃ�����𒴂����Ƃ�����ȏ㑝�₳�Ȃ�
		if (m_dashTime >= PlayerController::DASH_TIME)m_dashTime = PlayerController::DASH_TIME;
	}
	// �ړ��ʂ�����Ƃ����K������
	if (m_velocity.LengthSquared() > 0)	m_velocity.Normalize();
	// �ړ��ʂ�␳����
	m_velocity *= m_dash * elapsedTime;
	// ���ۂɈړ�����
	m_position += m_velocity;
	// ����]��ۑ�
	m_rotate = x;
	// �n�ʂ̍����ɐ���
	if (m_position.y <= Stage::STAGE_HEIGHT)m_position.y = Stage::STAGE_HEIGHT;
	// �n�ʂ̖ʐςɐ���
	if (m_position.x <= -Stage::STAGE_SIZE) m_position.x = -Stage::STAGE_SIZE;// ���[
	if (m_position.x >= Stage::STAGE_SIZE) m_position.x = Stage::STAGE_SIZE;// �E�[
	if (m_position.z <= -Stage::STAGE_SIZE) m_position.z = -Stage::STAGE_SIZE;// ��[
	if (m_position.z >= Stage::STAGE_SIZE) m_position.z = Stage::STAGE_SIZE;// ���[
	// �}�E�X�ʒu�𒆉��ɖ߂�
	SetCursorPos(m_point.x, m_point.y);
	// ���̃t���[���p��lastMouseX, lastMouseY���N���C�A���g�����Ƀ��Z�b�g
	POINT clientCenter = { m_point.x,m_point.y };
	// �N���C�A���g���W���E�B���h�E���W�ɕϊ�
	ScreenToClient(m_hWnd, &clientCenter);
	// �}�E�X��X���W��ۑ�
	m_lastMouseX = static_cast<float>(clientCenter.x);
	// �}�E�X��Y���W��ۑ�
	m_lastMouseY = static_cast<float>(clientCenter.y);
}
/*
*	@brief �f�o�b�O�R�}���h
*	@details �f�o�b�O���̂ݎg�p�\�ȃ`�[�g�R�}���h
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayerController::DebugCommand()
{
	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kbTracker = m_pCommonResources->GetInputManager()->GetKeyboardTracker();
	// �}�E�X�̏�Ԃ��擾����
	const auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// �E�N���b�N�œG����|
	if (MyMouse::IsRightMouseButtonPressed(mouseState) && m_pPlayer->GetisCheat() == false)
	{
		// �`�[�g�t���O�𗧂Ă�
		m_pPlayer->SetisCheat(true);
		// �G��HP��1000���炷�i�����j
		for (auto& enemy : m_pPlayer->GetEnemies()->GetEnemies())enemy->ApplyDamageToEnemy(1000);
	}
	// �E�N���b�N����ĂȂ��Ƃ��`�[�g�R�}���h�𖳌��ɂ���
	if (!MyMouse::IsRightMouseButtonPressed(mouseState))m_pPlayer->SetisCheat(false);
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
	// �}�E�X�̏�Ԃ��擾����
	const auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	if (MyMouse::IsLeftMouseButtonPressed(mouseState) &&// ���N���b�N����Ă���
		m_pPlayer->GetBulletManager()->GetIsPlayerShoot() == false &&// �e�����˂���Ă��Ȃ���
		m_pPlayer->GetBulletManager()->GetPlayerBulletCount() > 0 && // �e���c���Ă���
		m_pPlayer->GetBulletManager()->GetIsReloading() == false)// �����[�h���łȂ��Ƃ�
	{
		// �e�𐶐�����
		m_pPlayer->CreateBullet();
		// �e�������
		m_pPlayer->GetBulletManager()->ConsumePlayerBullet();
	}
	// ���N���b�N����Ă��Ȃ��Ƃ��e���������t���O�����낷
	if (!MyMouse::IsLeftMouseButtonPressed(mouseState))m_pPlayer->GetBulletManager()->SetIsPlayerShoot(false);
}
/*
*	@brief �v���C���[�̒e�������[�h����
*	@details R�L�[�Œe�������[�h����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayerController::Reload()
{
	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kbState = m_pCommonResources->GetInputManager()->GetKeyboardState();
	// R�L�[�Ń����[�h�t���O�𗧂Ă�
	if (kbState.R)m_pPlayer->GetBulletManager()->SetIsReloading(true);
	// �����[�h���Ȃ�e���[����
	if (m_pPlayer->GetBulletManager()->GetIsReloading())m_pPlayer->GetBulletManager()->ReLoadPlayerBullet();
}
