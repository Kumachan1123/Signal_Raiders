#include <pch.h>
#include "CreateShader.h"
#include <utility>
using namespace KumachiLib;
// ���_�V�F�[�_�[���쐬
void CreateShader::CreateVertexShader(ID3D11Device1* device,
	const wchar_t* fileName,
	Microsoft::WRL::ComPtr<ID3D11VertexShader>& vs,
	const D3D11_INPUT_ELEMENT_DESC* pIDE, UINT NumElements,
	ID3D11InputLayout** ppInputLauout)
{

	BinaryFile VS = BinaryFile::LoadFile(fileName);
	//	���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VS.GetData(), VS.GetSize(), NULL, vs.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	device->CreateInputLayout(&pIDE[0], NumElements, VS.GetData(), VS.GetSize(), ppInputLauout);
}




void CreateShader::CreatePixelShader(ID3D11Device1* device, const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& ps)
{
	BinaryFile PS = BinaryFile::LoadFile(fileName);
	//	�s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PS.GetData(), PS.GetSize(), NULL, ps.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
}

void CreateShader::CreateGeometryShader(ID3D11Device1* device, const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11GeometryShader>& gs)
{
	BinaryFile GS = BinaryFile::LoadFile(fileName);
	//	�W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GS.GetData(), GS.GetSize(), NULL, gs.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}

}

