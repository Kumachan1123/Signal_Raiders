/*
*	@file CreateShaders.h
*	@brief シェーダーを作成するシングルトンクラス
*	@details 板ポリゴンを描画する際に使用するシェーダーを作成する
*/
#pragma once
// 標準ライブラリ
#include <memory>
#include <utility>
// 自作ヘッダーファイル
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

class CreateShader
{
public:
	static CreateShader* const GetInstance();	// シングルトンインスタンス
public:
	// public関数
	~CreateShader();	// デストラクタ
	void Initialize(ID3D11Device1* device, // 初期化
		const D3D11_INPUT_ELEMENT_DESC* pIDE,
		UINT NumElements,
		Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout);
	void CreateVertexShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11VertexShader>& vs);// 頂点シェーダを作成
	void CreatePixelShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& ps);// ピクセルシェーダーを作成
	void CreateGeometryShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11GeometryShader>& gs);// ジオメトリシェーダーを作成
	Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout() { return m_pInputLayout; }// 作ったインプットレイアウトを返す
	void CreateConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& cBuffer, UINT bufferSize);// コンスタントバッファを作成
private:
	// private関数
	CreateShader();// コンストラクタ
	CreateShader(const CreateShader&) = delete;// コピーコンストラクタ
	CreateShader& operator=(const CreateShader&) = delete;// 代入演算子の禁止
private:
	// シングルトンインスタンス
	static std::unique_ptr<CreateShader> m_pInstance;
	// デバイス
	ID3D11Device1* m_pDevice;
	// 入力レイアウト
	const D3D11_INPUT_ELEMENT_DESC* m_pIDE;
	// 要素数
	UINT m_NumElements;
	// レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
};



