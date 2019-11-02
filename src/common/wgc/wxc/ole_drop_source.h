// ole_drop_source.h …OLEドロップソース
#ifndef __WGC_OLE_DROP_SOURCE__
#define __WGC_OLE_DROP_SOURCE__

#include "wxc.h"


_WXC_BEGIN                              // namespace wxc {

class ole_drop_source
{
public:
	ole_drop_source(IUnknown *pUnknown = NULL);
	~ole_drop_source(void);

	void  CacheGlobalData(const CLIPFORMAT cfFormat, HGLOBAL hGlobal);
	DWORD DoDragDrop(const DWORD dwOKEffects = DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK);

private:
	IDropSource *m_pDropSource;
	IDataObject *m_pDataObject;
};

_WXC_END                                // }

#endif // __WGC_OLE_DROP_SOURCE__
