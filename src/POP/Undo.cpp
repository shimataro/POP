// Undo.cpp …インプリメンテーションファイル
#include "stdafx.h"
#include "Undo.h"


////////////////////////////////////////////////////////////////////////////////
// コンストラクタ（変数の初期化）
CUndo::CUndo(void)
{
	m_now   = -1;                           // 現在位置＝-1
	m_tail  = -1;                           // 最後尾位置＝-1
	m_saved = 0;                            // 保存位置＝デキューの先頭
}

// デストラクタ（メモリの開放）
CUndo::~CUndo(void)
{
	if(m_tail != -1)
	{
		ClearFrontMemory();
		ClearBackMemory();
		image_free(&m_buffer[m_now].data);
	}
}

// アンドゥ
void CUndo::Undo(UNDOINFO &info)
{
	if(IsAbleToUndo())
	{
		m_now--;
		info = m_buffer[m_now];
	}
}

// リドゥ
void CUndo::Redo(UNDOINFO &info)
{
	if(IsAbleToRedo())
	{
		m_now++;
		info = m_buffer[m_now];
	}
}

// 現在位置にアンドゥ情報を加える
bool CUndo::AddUndoInfo(const UNDOINFO &info)
{
	// 現在位置より後のアンドゥメモリをクリア
	ClearBackMemory();
	ASSERT(m_now == m_tail);

	try
	{
		// バッファの最後にアンドゥ情報を追加
		m_buffer.push_back(info);
		m_now++;
		m_tail++;
		return true;
	}
	// 追加に失敗することはまずないけど…
	catch(CMemoryException *e)
	{
		e->Delete();
		return false;
	}
}

// 現在位置より前のアンドゥメモリをクリア
void CUndo::ClearFrontMemory(void)
{
	while(m_now > 0)
	{
		if(!binary_equal(m_buffer[0].data, m_buffer[1].data))
		{
			image_free(&m_buffer[0].data);
		}
		m_buffer.pop_front();
		m_now--;
		m_tail--;
		m_saved--;
	}
}

// 現在位置より後のアンドゥメモリをクリア
void CUndo::ClearBackMemory(void)
{
	while(m_tail > m_now)
	{
		if(!binary_equal(m_buffer[m_tail].data, m_buffer[m_tail - 1].data))
		{
			image_free(&m_buffer[m_tail].data);
		}
		m_buffer.pop_back();
		m_tail--;
	}

	if(m_saved > m_now)
	{
		m_saved = -1;
	}
}

// 現在位置以外のメモリをクリア
void CUndo::ClearMemory(void)
{
	ClearFrontMemory();
	ClearBackMemory();
	if(m_saved != m_now)
	{
		m_saved = -1;
	}
}
