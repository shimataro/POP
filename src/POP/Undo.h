// Undo.h …アンドゥ／リドゥ
#ifndef __UNDO__
#define __UNDO__

#include <winbase.h>                    // HGLOBAL
#include <deque>                        // std::deque


// アンドゥ情報
typedef struct tagUNDOINFO
{
	IMAGE_INFO info;
	IMAGE_DATA data;
} UNDOINFO;

// アンドゥクラス
class CUndo
{
public:
	CUndo(void);                            // コンストラクタ
	~CUndo(void);                           // デストラクタ

	void SetModifiedFlag(BOOL bModified = TRUE) // ダーティフラグのセット（セーブ/ロード時に呼び出す）
	{m_saved = bModified ? -1 : m_now;}
	bool IsModified(void) const             // 変更されたか？
	{return m_now != m_saved;}                 // 現在位置≠保存位置なら変更された
	bool IsAbleToUndo(void) const           // アンドゥできるか？
	{return m_now > 0;}                       // 現在位置＞先頭位置ならアンドゥできる
	bool IsAbleToRedo(void) const           // リドゥできるか？
	{return m_now < m_tail;}                  // 現在位置＜後尾位置ならリドゥできる

	void Undo(UNDOINFO &info);              // アンドゥ
	void Redo(UNDOINFO &info);              // リドゥ

	bool AddUndoInfo(const UNDOINFO &info); // 現在位置にアンドゥ情報を加える

	void ClearFrontMemory(void);            // 現在位置より前のアンドゥバッファをクリア
	void ClearBackMemory(void);             // 現在位置より後のアンドゥバッファをクリア
	void ClearMemory(void);                 // 現在位置以外のアンドゥバッファをクリア

private:
	std::deque<UNDOINFO> m_buffer;          // アンドゥバッファ（デキュー）
	int m_now, m_tail, m_saved;             // 現在位置、最後尾位置、保存位置
};

#endif
