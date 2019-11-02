// i_dataobject.cpp …インプリメンテーションファイル
#include "i_dataobject.h"
#include <assert.h>                     // assert()


_WGC_BEGIN                              // namespace wgc {


////////////////////////////////////////////////////////////
// コンストラクション

i_dataobject::i_dataobject(void)
	: m_lRefCount(0)
{
}

i_dataobject::~i_dataobject(void)
{
	for(object_list_t::iterator p = m_object_list.begin(); p != m_object_list.end(); p++)
	{
		// ストレージメディアの開放義務があれば開放
		if(p->release)
		{
			::ReleaseStgMedium(&p->medium);
		}
	}
}

// インスタンスを作成
IDataObject *i_dataobject::CreateInstance(void)
{
	IDataObject *pDataObject = new i_dataobject();
	pDataObject->AddRef();

	return pDataObject;
}

HRESULT STDMETHODCALLTYPE i_dataobject::QueryInterface(
	REFIID iid,
	void **ppv)
{
	if(IsEqualIID(iid, IID_IUnknown) || IsEqualIID(iid, IID_IDataObject))
	{
		*ppv = this;
		AddRef();
		return S_OK;
	}

	*ppv = NULL;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE i_dataobject::AddRef()
{
	// 参照カウントをインクリメント
	::InterlockedIncrement(&m_lRefCount);
	return m_lRefCount;
}

ULONG STDMETHODCALLTYPE i_dataobject::Release()
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

HRESULT STDMETHODCALLTYPE i_dataobject::GetData(
	FORMATETC *pFormatEtc,
	STGMEDIUM *pMedium)
{
	if(pFormatEtc == NULL || pMedium == NULL)
	{
		return E_INVALIDARG;
	}

	if((pFormatEtc->dwAspect & DVASPECT_CONTENT) == 0)
	{
		return DV_E_DVASPECT;
	}

	// オブジェクトのリストから条件に合致するデータを検索
	for(object_list_t::iterator p = m_object_list.begin(); p != m_object_list.end(); p++)
	{
		// 要求されたデータタイプが見つかったらコピーを作成
		if(p->format.cfFormat == pFormatEtc->cfFormat
		&&(p->format.tymed & pFormatEtc->tymed) != 0)
		{
			return _DuplicateMedium(pMedium, &p->medium, pFormatEtc->cfFormat);
		}
	}

	return DV_E_FORMATETC;
}

HRESULT STDMETHODCALLTYPE i_dataobject::GetDataHere(
	FORMATETC * /* pFormatEtc */,
	STGMEDIUM * /* pMedium */)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE i_dataobject::QueryGetData(FORMATETC *pFormatEtc)
{
	if(pFormatEtc == NULL)
	{
		return E_INVALIDARG;
	}

	if((pFormatEtc->dwAspect & DVASPECT_CONTENT) == 0)
	{
		return DV_E_DVASPECT;
	}

	for(object_list_t::iterator p = m_object_list.begin(); p != m_object_list.end(); p++)
	{
		if(p->format.cfFormat == pFormatEtc->cfFormat
		&&(p->format.tymed & pFormatEtc->tymed) != 0)
		{
			return S_OK;
		}
	}

	return DV_E_FORMATETC;
}

HRESULT STDMETHODCALLTYPE i_dataobject::EnumFormatEtc(
	DWORD             /* dwDirection */,
	IEnumFORMATETC ** /* ppEnumFormatEtc */)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE i_dataobject::SetData(
	FORMATETC *pFormatEtc,
	STGMEDIUM *pMedium,
	BOOL fRelease)
{
	if(pFormatEtc == NULL || pMedium == NULL)
	{
		return E_INVALIDARG;
	}

	// 新しいオブジェクトを作成
	object_t object;
	object.release = fRelease;
	object.format  = *pFormatEtc;
	object.medium  = *pMedium;

	// 作成したオブジェクトを挿入
	m_object_list.push_back(object);
	return S_OK;
}


HRESULT STDMETHODCALLTYPE i_dataobject::GetCanonicalFormatEtc(
	FORMATETC * /* pFormatetcIn */,
	FORMATETC * /* pFormatetcOut */)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE i_dataobject::DAdvise(
	FORMATETC   * /* pFormatEtc */,
	DWORD         /* advf */,
	IAdviseSink * /* pAdvSink */,
	DWORD       * /* pdwConnection */)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

HRESULT STDMETHODCALLTYPE i_dataobject::DUnadvise(
	DWORD /* dwConnection */)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

HRESULT STDMETHODCALLTYPE i_dataobject::EnumDAdvise(
	IEnumSTATDATA ** /* ppenumAdvise */)
{
	return OLE_E_ADVISENOTSUPPORTED;
}


// ストレージメディアの複製を作成
HRESULT i_dataobject::_DuplicateMedium(
	STGMEDIUM *dst,
	const STGMEDIUM *src,
	const CLIPFORMAT cfFormat)
{
	if(src->pUnkForRelease != NULL)
	{
		// リリース用オブジェクトがあれば、複製を作らずに参照カウントを増やす
		src->pUnkForRelease->AddRef();
		*dst = *src;
		return S_OK;
	}

	HANDLE hData = NULL;

	// データをコピー
	dst->tymed          = src->tymed;
	dst->pUnkForRelease = src->pUnkForRelease;
	switch(src->tymed)
	{
	case TYMED_HGLOBAL:
		hData = ::OleDuplicateData(src->hGlobal, cfFormat, 0);
		dst->hGlobal = reinterpret_cast<HGLOBAL>(hData);
		break;
	}

	if(hData == NULL)
	{
		return DV_E_FORMATETC;
	}
	return S_OK;
}

_WGC_END                                // }
