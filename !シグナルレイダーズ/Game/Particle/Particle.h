#pragma once
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
class CommonResources;

class Particle
{
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;

	DirectX::SimpleMath::Vector3 m_position;// �G�t�F�N�g���Đ�������W
	float m_scale;
	DirectX::SimpleMath::Vector3 m_rotation;
	DirectX::SimpleMath::Matrix m_Billboard;
	DirectX::SimpleMath::Vector3 m_cameraPosition;
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//	���̓��C�A�E�g 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;

	//	���ʃX�e�[�g�I�u�W�F�N�g�ւ̃|�C���^
	std::unique_ptr<DirectX::CommonStates> m_States;

	//	�e�N�X�`���n���h�� 
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture;

	//	�G�t�F�N�g 
	std::unique_ptr<DirectX::AlphaTestEffect> m_BatchEffect;

	//	�v���~�e�B�u�o�b�` 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;
	// �A�j���[�V�����p
	// ���A�G�t�F�N�g���Đ����Ă邩
	bool m_isPlaying;
	// �t���[����
	int anim;
	// �A�j���[�V�����X�s�[�h
	float m_animSpeed;
	// �A�j���[�V�����̌o�ߎ���
	float m_elapsedTime;

	// �t���[���̍s���Ɨ�
	int m_frameRows;
	int m_frameCols;



public:
	static const DirectX::VertexPositionTexture Vertices[4];
public:

	Particle(CommonResources* resources, DirectX::SimpleMath::Vector3 PlayPos, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Matrix world);// �f�o�C�X�ƍĐ��ʒu���󂯎��
	~Particle();
	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext1* context, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();
	void CreateBillboard(DirectX::SimpleMath::Vector3 target,
						 DirectX::SimpleMath::Vector3 eye,
						 DirectX::SimpleMath::Vector3 up);

public:
	bool IsPlaying() { return m_isPlaying; };
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; };

};