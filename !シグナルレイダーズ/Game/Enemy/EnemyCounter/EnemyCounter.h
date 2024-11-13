/*
	@file	EnemyCounter.h
	@brief	�G�J�E���g�N���X
*/
#pragma once
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/KumachiLib/BinaryFile.h"
#include <memory>

class CommonResources;
class EnemyCounter
{
private:
	CommonResources* m_commonResources;
	// �G�̑���(�ۑ��p�j
	int m_enemyIndex;
	// ���݂̓G�̐�(�ۑ��p�j
	int m_nowEnemy;

	// �G�̑���(10�̈�)
	int m_enemyIndex10;
	// �G�̑���(1�̈�)
	int m_enemyIndex1;
	// ���݂̓G�̐�(10�̈�)
	int m_nowEnemy10;
	// ���݂̓G�̐�(1�̈�)
	int m_nowEnemy1;
	int m_frameRows = 1; // �摜�̍s��
	int m_frameCols = 10; // �摜�̗�
	// �����e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// �u�c��F�v�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_remaining;
	// �u/�v�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_slash;

	//	���̓��C�A�E�g 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//	���ʃX�e�[�g�I�u�W�F�N�g�ւ̃|�C���^
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	�G�t�F�N�g 
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;
	// ���_���
	// ������10�̈�
	DirectX::DX11::VertexPositionTexture m_verticesEnemyIndex10[4];
	// ������1�̈�
	DirectX::DX11::VertexPositionTexture m_verticesEnemyIndex1[4];
	// ���݂̓G�̐���10�̈�
	DirectX::DX11::VertexPositionTexture m_verticesNowEnemy10[4];
	// ���݂̓G�̐���1�̈�
	DirectX::DX11::VertexPositionTexture m_verticesNowEnemy1[4];
	// �u�̂���F�v
	DirectX::DX11::VertexPositionTexture m_verticesRemaining[4];
	// �u/�v
	DirectX::DX11::VertexPositionTexture m_verticesSlash[4];

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;

public:
	// �R���X�g���N�^
	EnemyCounter();
	// �f�X�g���N�^
	~EnemyCounter();
	// ����������
	void Initialize(CommonResources* commonResources);
	// �X�V����
	void Update(float elapsedTime);
	// �`�悷��
	void Render();
	void SetEnemyIndex(int enemyIndex) { m_enemyIndex = enemyIndex; }
	// ���݂̓G�̐���ݒ肷��
	void SetNowEnemy(int nowEnemy) { m_nowEnemy = nowEnemy; }
private:
	void LoadTexture(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& tex);
	// ������10�̈ʂ�`��
	void DrawEnemyIndex10();
	// ������1�̈ʂ�`��
	void DrawEnemyIndex1();
	// ���݂̓G�̐���10�̈ʂ�`��
	void DrawNowEnemy10();
	// ���݂̓G�̐���1�̈ʂ�`��
	void DrawNowEnemy1();
	// �u�̂���F�v��`��
	void DrawRemaining();
	// �u/�v��`��
	void DrawSlash();
};