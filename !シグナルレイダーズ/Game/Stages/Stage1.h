#pragma once
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <PrimitiveBatch.h> 
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <SimpleMath.h>
#include <Effects.h>
#include <Libraries/Microsoft/DebugDraw.h>

class Stage1
{
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// ���f��
	std::unique_ptr<DirectX::Model> m_stage;
	//	���̓��C�A�E�g 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	//	�e�N�X�`���n���h�� 
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture;

	// �m�[�}���}�b�v�n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_NormalMap;

	std::unique_ptr<DirectX::NormalMapEffect> m_BatchEffect;

	//	���ʃX�e�[�g�I�u�W�F�N�g�ւ̃|�C���^
	std::unique_ptr<DirectX::CommonStates> m_States;

	//	�G�t�F�N�g 
	//std::unique_ptr<DirectX::AlphaTestEffect> m_BatchEffect;

	//	�v���~�e�B�u�o�b�` 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;

public:
	// �����X�e�[�^�X��ݒ�
	Stage1();
	~Stage1();
	void Initialize(CommonResources* resources);
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

};