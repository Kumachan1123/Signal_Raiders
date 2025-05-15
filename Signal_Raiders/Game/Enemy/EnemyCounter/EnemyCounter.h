/*
	@file	EnemyCounter.h
	@brief	�G�J�E���g�N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <memory>
// �O�����C�u����
#include "DeviceResources.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <Libraries/Microsoft/DebugDraw.h>
// ����w�b�_�[�t�@�C��
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
// �O���錾
class CommonResources;
class EnemyCounter
{
private:
	// private�\����
	struct ConstBuffer// �R���X�^���g�o�b�t�@
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 count;     // �J�E���g
		DirectX::SimpleMath::Vector4 height;    // ����
		DirectX::SimpleMath::Vector4 width;     // ��

	};
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
	void LoadTexture(const wchar_t* path, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& tex);
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
	static const int VERTEX_COUNT = 4; // ���_��
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;// �C���v�b�g���C�A�E�g

	// �e�摜�̕`��ʒu�ƃT�C�Y
	// �u�c��F�v�̕`��ʒu�E�T�C�Y
	const float REMAINING_POS_X = 0.4f;
	const float REMAINING_POS_Y = 1.0f;
	const float REMAINING_SIZE_X = 0.25f;
	const float REMAINING_SIZE_Y = 0.18f;

	// �u/�v�̕`��ʒu�E�T�C�Y
	const float SLASH_POS_X = 0.8f;
	const float SLASH_POS_Y = 1.0f;
	const float SLASH_SIZE_X = 0.05f;
	const float SLASH_SIZE_Y = 0.18f;

	// ���݂̓G�̐���1�̈ʁE10�̈ʂ̕`��ʒu�E�T�C�Y
	const float NOW_ENEMY_10_POS_X = 0.66f;
	const float NOW_ENEMY_10_POS_Y = 1.0f;
	const float NOW_ENEMY_1_POS_X = 0.72f;
	const float NOW_ENEMY_1_POS_Y = 1.0f;
	const float NOW_ENEMY_SIZE_X = 0.08f;
	const float NOW_ENEMY_SIZE_Y = 0.18f;

	// �G�̑�����1�̈ʁE10�̈ʂ̕`��ʒu�E�T�C�Y
	const float ENEMY_INDEX_10_POS_X = 0.86f;
	const float ENEMY_INDEX_10_POS_Y = 1.0f;
	const float ENEMY_INDEX_1_POS_X = 0.92f;
	const float ENEMY_INDEX_1_POS_Y = 1.0f;
	const float ENEMY_INDEX_SIZE_X = 0.08f;
	const float ENEMY_INDEX_SIZE_Y = 0.18f;

private:
	// private�ϐ�
	// ���ʃ��\�[�X�ւ̃|�C���^
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
	// �摜�̍s��
	int m_frameRows;
	// �摜�̗�
	int m_frameCols;
	// �����e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
	// �u�c��F�v�e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_remaining;
	// �u/�v�e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_slash;
	// �R���X�^���g�o�b�t�@
	ConstBuffer m_constBuffer;
	//	���̓��C�A�E�g 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
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
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;



};