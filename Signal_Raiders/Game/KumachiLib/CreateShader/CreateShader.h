#pragma once
// / <summary>
// / �V�F�[�_�[���쐬����N���X
// / </summary>
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include <memory>
namespace KumachiLib
{
	class CreateShader
	{

	public:// �֐�
		// ���_�V�F�[�_���쐬
		static void CreateVertexShader(ID3D11Device1* device, const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11VertexShader>& vs, const D3D11_INPUT_ELEMENT_DESC* pIDE, UINT NumElements, ID3D11InputLayout** ppInputLauout);
		// �s�N�Z���V�F�[�_�[���쐬
		static void CreatePixelShader(ID3D11Device1* device, const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& ps);
		// �W�I���g���V�F�[�_�[���쐬
		static void CreateGeometryShader(ID3D11Device1* device, const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11GeometryShader>& gs);



	};


};
