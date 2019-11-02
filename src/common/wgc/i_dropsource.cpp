// i_dropsource.cpp �c�C���v�������e�[�V�����t�@�C��
#include "i_dropsource.h"
#include <assert.h>                     // assert()


_WGC_BEGIN                              // namespace wgc {


////////////////////////////////////////////////////////////
// �R���X�g���N�V����

i_dropsource::i_dropsource(void)
	: m_lRefCount(0)
{
}

i_dropsource::~i_dropsource(void)
{
}

// �C���X�^���X���쐬
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
	// �Q�ƃJ�E���g���C���N�������g
	::InterlockedIncrement(&m_lRefCount);
	return m_lRefCount;
}


ULONG STDMETHODCALLTYPE i_dropsource::Release()
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

// �h���b�O���p�����邩�ǂ��������߂�
HRESULT STDMETHODCALLTYPE i_dropsource::QueryContinueDrag(
	BOOL fEscapePressed,
	DWORD grfKeyState)
{
	// ESC�������ꂽ�璆�~
	if(fEscapePressed)
	{
		return DRAGDROP_S_CANCEL;
	}

	// �}�E�X�̃{�^�������������ꂽ�璆�~
	const DWORD dwCancelFlag = MK_LBUTTON | MK_RBUTTON;
	if((grfKeyState & dwCancelFlag) == dwCancelFlag)
	{
		return DRAGDROP_S_CANCEL;
	}

	// �}�E�X�{�^���������ꂽ��h���b�v
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
