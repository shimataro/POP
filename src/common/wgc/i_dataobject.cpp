// i_dataobject.cpp �c�C���v�������e�[�V�����t�@�C��
#include "i_dataobject.h"
#include <assert.h>                     // assert()


_WGC_BEGIN                              // namespace wgc {


////////////////////////////////////////////////////////////
// �R���X�g���N�V����

i_dataobject::i_dataobject(void)
	: m_lRefCount(0)
{
}

i_dataobject::~i_dataobject(void)
{
	for(object_list_t::iterator p = m_object_list.begin(); p != m_object_list.end(); p++)
	{
		// �X�g���[�W���f�B�A�̊J���`��������ΊJ��
		if(p->release)
		{
			::ReleaseStgMedium(&p->medium);
		}
	}
}

// �C���X�^���X���쐬
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
	// �Q�ƃJ�E���g���C���N�������g
	::InterlockedIncrement(&m_lRefCount);
	return m_lRefCount;
}

ULONG STDMETHODCALLTYPE i_dataobject::Release()
{
	assert(m_lRefCount > 0);

	// �Q�ƃJ�E���g���f�N�������g
	if(::InterlockedDecrement(&m_lRefCount) == 0)
	{
		// 0�ɂȂ����玩�����g���폜
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

	// �I�u�W�F�N�g�̃��X�g��������ɍ��v����f�[�^������
	for(object_list_t::iterator p = m_object_list.begin(); p != m_object_list.end(); p++)
	{
		// �v�����ꂽ�f�[�^�^�C�v������������R�s�[���쐬
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

	// �V�����I�u�W�F�N�g���쐬
	object_t object;
	object.release = fRelease;
	object.format  = *pFormatEtc;
	object.medium  = *pMedium;

	// �쐬�����I�u�W�F�N�g��}��
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


// �X�g���[�W���f�B�A�̕������쐬
HRESULT i_dataobject::_DuplicateMedium(
	STGMEDIUM *dst,
	const STGMEDIUM *src,
	const CLIPFORMAT cfFormat)
{
	if(src->pUnkForRelease != NULL)
	{
		// �����[�X�p�I�u�W�F�N�g������΁A��������炸�ɎQ�ƃJ�E���g�𑝂₷
		src->pUnkForRelease->AddRef();
		*dst = *src;
		return S_OK;
	}

	HANDLE hData = NULL;

	// �f�[�^���R�s�[
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
