#pragma once
// / <summary>
// / シェーダーを作成するクラス
// / </summary>
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include <memory>
namespace KumachiLib
{
	class CreateShader
	{

	public:// 関数
		// 頂点シェーダを作成
		static void CreateVertexShader(ID3D11Device1* device, const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11VertexShader>& vs, const D3D11_INPUT_ELEMENT_DESC* pIDE, UINT NumElements, ID3D11InputLayout** ppInputLauout);
		// ピクセルシェーダーを作成
		static void CreatePixelShader(ID3D11Device1* device, const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& ps);
		// ジオメトリシェーダーを作成
		static void CreateGeometryShader(ID3D11Device1* device, const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11GeometryShader>& gs);



	};


};
