// i_dataobject.h �c�f�[�^�I�u�W�F�N�g
#ifndef __WGC_I_DATAOBJECT__
#define __WGC_I_DATAOBJECT__

#include "wgc.h"
#include <list>


_WGC_BEGIN                              // namespace wgc {

// IDataObject �̃f�t�H���g�C���v�������g
class i_dataobject : public IDataObject
{
public:
	static IDataObject *CreateInstance(void);

	// IUnknown ���\�b�h
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppv);
	ULONG   STDMETHODCALLTYPE AddRef(void);
	ULONG   STDMETHODCALLTYPE Release(void);

	// IDataObject ���\�b�h
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
	LONG m_lRefCount;                       // �Q�ƃJ�E���g

	// CreateInstance() �ȊO�ŃC���X�^���X���쐬�ł��Ȃ��悤�ɁA�R���X�g���N�^��private�ɂ���
	i_dataobject(void);
	~i_dataobject(void);

	static HRESULT _DuplicateMedium(STGMEDIUM *dst, const STGMEDIUM *src, const CLIPFORMAT cfFormat);
};

_WGC_END                                // }

#endif // __WGC_I_DATAOBJECT__
