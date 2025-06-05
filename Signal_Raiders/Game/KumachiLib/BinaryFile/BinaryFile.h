/*
*	@file BinaryFile.h
*	@brief ファイル読み込みクラス
*/
#pragma once
#ifndef BINARYFILE_DEFINED
#define BINARYFILE_DEFINED
// 標準ライブラリ
#include <memory>
#include <fstream>
#include <assert.h>

namespace KumachiLib
{
	// ファイル読み込みクラス
	class BinaryFile
	{
	public:
		//	アクセサ
		// データを取得
		char* GetData() { return m_pData.get(); }
		// ファイルのサイズを取得
		unsigned int GetSize() const { return m_size; }
	public:
		// publicメンバ関数
		// ファイル名を指定してロード
		static KumachiLib::BinaryFile LoadFile(const wchar_t* fileName);
		// コンストラクタ
		BinaryFile();
		// ムーブコンストラクタ
		BinaryFile(BinaryFile&& in)noexcept;
	protected:
		//protectedメンバ変数
		// データ
		std::unique_ptr<char[]> m_pData;
		// サイズ
		unsigned int m_size;
	};
}
#endif // BINARYFILE_DEFINED