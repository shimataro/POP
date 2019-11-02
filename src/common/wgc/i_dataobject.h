// i_dataobject.h …データオブジェクト
#ifndef __WGC_I_DATAOBJECT__
#define __WGC_I_DATAOBJECT__

#include "wgc.h"
#include <list>


_WGC_BEGIN                              // namespace wgc {

// IDataObject のデフォルトインプリメント
class i_dataobject : public IDataObject
{
public:
	static IDataObject *CreateInstance(void);

	// IUnknown メソッド
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppv);
	ULONG   STDMETHODCALLTYPE AddRef(void);
	ULONG   STDMETHODCALLTYPE Release(void);

	// IDataObject メソッド
	HRESULT STDMETHODCALLTYPE GetData              (FORMATETC *pFormatEtc, STGMEDIUM *pMedium);
	HRESULT STDMETHODCALLTYPE GetDataHere          (FORMATETC *pFormatEtc, STGMEDIUM *pMedium);
	HRESULT STDMETHODCALLTYPE QueryGetData         (FORMATETC *pFormatEtc);
	HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc(FORMATETC *pFormatEtcIn, FORMATETC *pFormatEtcInOut);
	HRESULT STDMETHODCALLTYPE SetData              (FORMATETC *pFormatEtc, STGMEDIUM *pMedium, BOOL fRelease);
	HRESULT STDMETHODCALLTYPE EnumFormatEtc        (DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc);
	HRESULT STDMETHODCALLTYPE DAdvise              (FORMATETC *pFormatEtc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection);
	HRESULT STDMETHODCALLTYPE DUnadvise            (DWORD dwConnection);
	HRESULT STDMETHODCALLTYPE EnumDAdvise          (IEnumSTATDATA **ppenumAdvise);

private:
	typedef struct _object_t
	{
		BOOL      release;
		FORMATETC format;
		STGMEDIUM medium;
	} object_t;
	typedef std::list<object_t> object_list_t;

	object_list_t m_object_list;
	LONG m_lRefCount;                       // 参照カウント

	// CreateInstance() 以外でインスタンスを作成できないように、コンストラクタをprivateにする
	i_dataobject(void);
	~i_dataobject(void);

	static HRESULT _DuplicateMedium(STGMEDIUM *dst, const STGMEDIUM *src, const CLIPFORMAT cfFormat);
};

_WGC_END                                // }

#endif // __WGC_I_DATAOBJECT__
