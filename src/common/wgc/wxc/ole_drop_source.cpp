// ole_drop_source.cpp �c�C���v�������e�[�V�����t�@�C��
#include "ole_drop_source.h"
#include "../i_dropsource.h"
#include "../i_dataobject.h"


_WXC_BEGIN                              // namespace wxc {


////////////////////////////////////////////////////////////
// �R���X�g���N�V����

ole_drop_source::ole_drop_source(IUnknown *pUnknown /* = NULL */)
	: m_pDropSource(NULL), m_pDataObject(NULL)
{
	// pUnknown ����C���^�[�t�F�[�X���擾
	if(pUnknown != NULL)
	{
		pUnknown->QueryInterface(
			IID_IDropSource,
			reinterpret_cast<void **>(&m_pDropSource));

		pUnknown->QueryInterface(
			IID_IDataObject,
			reinterpret_cast<void **>(&m_pDataObject));
	}

	// �C���^�[�t�F�[�X�̎擾�Ɏ��s������A�f�t�H���g�̃C���^�[�t�F�[�X���擾
	if(m_pDropSource == NULL) { m_pDropSource = wgc::i_dropsource::CreateInstance(); }
	if(m_pDataObject == NULL) { m_pDataObject = wgc::i_dataobject::CreateInstance(); }
}

ole_drop_source::~ole_drop_source()
{
	m_pDataObject->Release();
	m_pDropSource->Release();
}

void ole_drop_source::CacheGlobalData(
	const CLIPFORMAT cfFormat,
	HGLOBAL hGlobal)
{
	FORMATETC format;
	format.cfFormat = cfFormat;
	format.dwAspect = DVASPECT_CONTENT;
	format.lindex   = -1;
	format.ptd      = NULL;
	format.tymed    = TYMED_HGLOBAL;

	STGMEDIUM medium;
	medium.hGlobal        = hGlobal;
	medium.tymed          = TYMED_HGLOBAL;
	medium.pUnkForRelease = NULL;

	// �f�[�^���i�[�i����� m_pDataObject �ɔC����j
	m_pDataObject->SetData(&format, &medium, TRUE);
}

DWORD ole_drop_source::DoDragDrop(
	const DWORD dwOKEffects /* = DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK */)
{
	DWORD dwEffect = DROPEFFECT_NONE;
	::DoDragDrop(m_pDataObject, m_pDropSource, dwOKEffects, &dwEffect);
	return dwEffect;
}

_WXC_END                                // }
