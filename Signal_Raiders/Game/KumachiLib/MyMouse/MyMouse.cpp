/*
*	@file MyMouse.cpp
*	@brief InputManager�݂̂ł͂ł��Ȃ��}�E�X�̓��͂��g������N���X
*/
#include <pch.h>
#include "MyMouse.h"

// �O��̃}�E�X�{�^���̏�Ԃ�������
bool MyMouse::m_prevMouseButtonState = false;

/*
*	@brief ���N���b�N�������ꂽ�u�Ԃ��擾
*	@details �}�E�X�̏�Ԃ��󂯎��A���N���b�N�������ꂽ�u�Ԃ𔻒肷��
*	@param currentState �}�E�X�̌��݂̏��
*	@return ���N���b�N�������ꂽ�u�ԂȂ�true�A�����łȂ����false
*/
bool MyMouse::IsLeftMouseButtonPressed(const DirectX::Mouse::State& currentState)
{
	// �O��̃}�E�X�{�^���̏�Ԃƌ��݂̏�Ԃ��r���āA���N���b�N�������ꂽ�u�Ԃ𔻒肷��
	bool isPressed = false;
	// ���N���b�N��������Ă��āA�O��̏�Ԃł͉�����Ă��Ȃ������ꍇ
	if (currentState.leftButton && !m_prevMouseButtonState)
	{
		// ���N���b�N�������ꂽ�u�ԂƔ���
		isPressed = true;
	}
	// �O��̃}�E�X�{�^���̏�Ԃ��X�V
	m_prevMouseButtonState = currentState.leftButton;
	// ���茋�ʂ�Ԃ�
	return isPressed;
}
