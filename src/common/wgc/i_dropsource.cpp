// i_dropsource.cpp …インプリメンテーションファイル
#include "i_dropsource.h"
#include <assert.h>                     // assert()


_WGC_BEGIN                              // namespace wgc {


////////////////////////////////////////////////////////////
// コンストラクション

i_dropsource::i_dropsource(void)
	: m_lRefCount(0)
{
}

i_dropsource::~i_dropsource(void)
{
}

// インスタンスを作成
IDropSource *i_dropsource::CreateInstance(void)
{
	IDropSource *pDropSource = new i_dropsource();
	pDropSource->AddRef();

	return pDropSource;
}

HRESULT STDMETHODCALLTYPE i_dropsource::QueryInterface(
	REFIID iid,
	void **ppv)
{
	if(IsEqualIID(iid, IID_IUnknown) || IsEqualIID(iid, IID_IDropSource))
	{
		*ppv = this;
		AddRef();
		return S_OK;
	}

	*ppv = NULL;
	return E_NOINTERFACE;
}


ULONG STDMETHODCALLTYPE i_dropsource::AddRef()
{
	// 参照カウントをインクリメント
	::InterlockedIncrement(&m_lRefCount);
	return m_lRefCount;
}


ULONG STDMETHODCALLTYPE i_dropsource::Release()
{
	assert(m_lRefCount > 0);

	// 参照カウントをデクリメント
	if(::InterlockedDecrement(&m_lRefCount) == 0)
	{
		// 0になったら自分自身を削除
		delete this;
		return 0;
	}
	return m_lRefCount;
}

// ドラッグを継続するかどうかを決める
HRESULT STDMETHODCALLTYPE i_dropsource::QueryContinueDrag(
	BOOL fEscapePressed,
	DWORD grfKeyState)
{
	// ESCが押されたら中止
	if(fEscapePressed)
	{
		return DRAGDROP_S_CANCEL;
	}

	// マウスのボタンが両方押されたら中止
	const DWORD dwCancelFlag = MK_LBUTTON | MK_RBUTTON;
	if((grfKeyState & dwCancelFlag) == dwCancelFlag)
	{
		return DRAGDROP_S_CANCEL;
	}

	// マウスボタンが離されたらドロップ
	const DWORD dwDropFlag = MK_LBUTTON | MK_RBUTTON;
	if((grfKeyState & dwDropFlag) == 0)
	{
		return DRAGDROP_S_DROP;
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE i_dropsource::GiveFeedback(
	DWORD /* dwEffect */)
{
	return DRAGDROP_S_USEDEFAULTCURSORS;
}

_WGC_END                                // }
