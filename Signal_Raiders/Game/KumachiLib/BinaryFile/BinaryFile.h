/*
*	@file BinaryFile.h
*	@brief ファイル読み込みクラス
*/

#pragma once

// 標準ライブラリ
#include <memory>
#include <fstream>
#include <assert.h>
namespace KumachiLib
{
	class BinaryFile
	{
	protected:
		//protectedメンバ変数
		std::unique_ptr<char[]> m_data;		// データ
		unsigned int m_size;		// サイズ
	public:
		//	アクセサ
		char* GetData() { return m_data.get(); }
		unsigned int GetSize() const { return m_size; }
		// publicメンバ関数
		static KumachiLib::BinaryFile LoadFile(const wchar_t* fileName);		// ファイル名を指定してロード
		BinaryFile();// コンストラクタ
		BinaryFile(BinaryFile&& in);// ムーブコンストラクタ

	};
}