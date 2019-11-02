// i_drop_source.h …ドロップソース
#ifndef __WGC_I_DROPSOURCE__
#define __WGC_I_DROPSOURCE__

#include "wgc.h"


_WGC_BEGIN                              // namespace wgc {

// IDropSource のデフォルトインプリメント
class i_dropsource : public IDropSource
{
public:
	static IDropSource *CreateInstance(void);

	// IUnknown メソッド
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppv);
	ULONG   STDMETHODCALLTYPE AddRef(void);
	ULONG   STDMETHODCALLTYPE Release(void);

	// IDropSource メソッド
	HRESULT STDMETHODCALLTYPE QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState);
	HRESULT STDMETHODCALLTYPE GiveFeedback     (DWORD dwEffect);

private:
	LONG m_lRefCount;                       // 参照カウント

	// CreateInstance() 以外でインスタンスを作成できないように、コンストラクタをprivateにする
	i_dropsource(void);
	~i_dropsource(void);
};

_WGC_END                                // }

#endif // __WGC_I_DROPSOURCE__
