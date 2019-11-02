// i_drop_source.h �c�h���b�v�\�[�X
#ifndef __WGC_I_DROPSOURCE__
#define __WGC_I_DROPSOURCE__

#include "wgc.h"


_WGC_BEGIN                              // namespace wgc {

// IDropSource �̃f�t�H���g�C���v�������g
class i_dropsource : public IDropSource
{
public:
	static IDropSource *CreateInstance(void);

	// IUnknown ���\�b�h
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppv);
	ULONG   STDMETHODCALLTYPE AddRef(void);
	ULONG   STDMETHODCALLTYPE Release(void);

	// IDropSource ���\�b�h
	HRESULT STDMETHODCALLTYPE QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState);
	HRESULT STDMETHODCALLTYPE GiveFeedback     (DWORD dwEffect);

private:
	LONG m_lRefCount;                       // �Q�ƃJ�E���g

	// CreateInstance() �ȊO�ŃC���X�^���X���쐬�ł��Ȃ��悤�ɁA�R���X�g���N�^��private�ɂ���
	i_dropsource(void);
	~i_dropsource(void);
};

_WGC_END                                // }

#endif // __WGC_I_DROPSOURCE__
