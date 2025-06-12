/*
*	@file	EnemyCounter.h
*	@brief	�G�J�E���g�N���X
*/
#pragma once
#ifndef ENEMY_COUNTER_DEFINED
#define ENEMY_COUNTER_DEFINED
// �W�����C�u����
#include <vector>
#include <cassert>
#include <memory>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/DebugDraw.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"

// �O���錾
class CommonResources;

// �G�J�E���g�N���X
class EnemyCounter
{
private:
	// private�\����
	// �V�F�[�_�[�ɓn���萔�o�b�t�@
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 count;     // �J�E���g
		DirectX::SimpleMath::Vector4 height;    // ����
		DirectX::SimpleMath::Vector4 width;     // ��
	};
public:
	// public�֐�
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
	// �G�̑�����ݒ肷��
	void SetEnemyIndex(int enemyIndex) { m_enemyIndex = enemyIndex; }
	// ���݂̓G�̐���ݒ肷��
	void SetNowEnemy(int nowEnemy) { m_nowEnemy = nowEnemy; }
private:
	// private�֐�
	// ������10�̈ʂ�`��
	void DrawEnemyIndex10();
	// ������1�̈ʂ�`��
	void DrawEnemyIndex1();
	// ���݂̓G�̐���10�̈ʂ�`��
	void DrawNowEnemy10();
	// ���݂̓G�̐���1�̈ʂ�`��
	void DrawNowEnemy1();
	// �摜��\��
	void DrawQuad(std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture,
		DirectX::VertexPositionTexture* vertices,
		float startX, float startY, float width, float height,
		int frameIndex, int frameCols, int frameRows);
public:
	// public�萔
	// ���_��
	static const int VERTEX_COUNT = 4;
	// �C���v�b�g���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// �e�摜�̕`��ʒu�ƃT�C�Y
	// �u�c��F�v�̕`��ʒu�E�T�C�Y
	const float REMAINING_POS_X = 0.35f;// �c��̕\���ʒuX
	const float REMAINING_POS_Y = 1.0f;// �c��̕\���ʒuY
	const float REMAINING_SIZE_X = 0.25f;// �c��̕\���T�C�YX
	const float REMAINING_SIZE_Y = 0.18f;//	�c��̕\���T�C�YY
	// �u/�v�̕`��ʒu�E�T�C�Y
	const float SLASH_POS_X = 0.8f;// �u/�v�̕\���ʒuX
	const float SLASH_POS_Y = 1.0f;// �u/�v�̕\���ʒuY
	const float SLASH_SIZE_X = 0.05f;// �u/�v�̕\���T�C�YX
	const float SLASH_SIZE_Y = 0.18f;// �u/�v�̕\���T�C�YY
	// ���݂̓G�̐���1�̈ʁE10�̈ʂ̕`��ʒu�E�T�C�Y
	const float NOW_ENEMY_10_POS_X = 0.66f;// ���݂̓G�̐���10�̈ʂ̕\���ʒuX
	const float NOW_ENEMY_10_POS_Y = 1.0f;// ���݂̓G�̐���10�̈ʂ̕\���ʒuY
	const float NOW_ENEMY_1_POS_X = 0.72f;// ���݂̓G�̐���1�̈ʂ̕\���ʒuX
	const float NOW_ENEMY_1_POS_Y = 1.0f;// ���݂̓G�̐���1�̈ʂ̕\���ʒuY
	const float NOW_ENEMY_SIZE_X = 0.08f;// ���݂̓G�̐��̕\���T�C�YX
	const float NOW_ENEMY_SIZE_Y = 0.18f;// ���݂̓G�̐��̕\���T�C�YY
	// �G�̑�����1�̈ʁE10�̈ʂ̕`��ʒu�E�T�C�Y
	const float ENEMY_INDEX_10_POS_X = 0.86f;// �G�̑�����10�̈ʂ̕\���ʒuX
	const float ENEMY_INDEX_10_POS_Y = 1.0f;// �G�̑�����10�̈ʂ̕\���ʒuY
	const float ENEMY_INDEX_1_POS_X = 0.92f;// �G�̑�����1�̈ʂ̕\���ʒuX
	const float ENEMY_INDEX_1_POS_Y = 1.0f;// �G�̑�����1�̈ʂ̕\���ʒuY
	const float ENEMY_INDEX_SIZE_X = 0.08f;// �G�̑����̕\���T�C�YX
	const float ENEMY_INDEX_SIZE_Y = 0.18f;// �G�̑����̕\���T�C�YY
private:
	// private�ϐ�
	// ���ʃ��\�[�X�ւ̃|�C���^
	CommonResources* m_pCommonResources;
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
	// �摜�̍s��
	int m_frameRows;
	// �摜�̗�
	int m_frameCols;
	// �����e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTexture;
	// �u�c��F�v�e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pRemaining;
	// �u/�v�e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pSlash;
	// �R���X�^���g�o�b�t�@
	ConstBuffer m_constBuffer;
	//	���̓��C�A�E�g 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// ���_���
	// ������10�̈�
	DirectX::DX11::VertexPositionTexture m_verticesEnemyIndex10[VERTEX_COUNT];
	// ������1�̈�
	DirectX::DX11::VertexPositionTexture m_verticesEnemyIndex1[VERTEX_COUNT];
	// ���݂̓G�̐���10�̈�
	DirectX::DX11::VertexPositionTexture m_verticesNowEnemy10[VERTEX_COUNT];
	// ���݂̓G�̐���1�̈�
	DirectX::DX11::VertexPositionTexture m_verticesNowEnemy1[VERTEX_COUNT];
	// �u�̂���F�v
	DirectX::DX11::VertexPositionTexture m_verticesRemaining[VERTEX_COUNT];
	// �u/�v
	DirectX::DX11::VertexPositionTexture m_verticesSlash[VERTEX_COUNT];
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;
};
#endif //ENEMY_COUNTER_DEFINED