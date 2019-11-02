// ReBar.h …リバーのサブクラス化
#ifndef __WGC_REBAR__
#define __WGC_REBAR__

#include "Control.h"


_WGC_BEGIN                              // namespace wgc {

class rebar : public control
{
public:
	////////////////////////////////////////////////////////////////
	// コンストラクタ／デストラクタ
	explicit rebar(void);
	virtual ~rebar(void);

	BOOL Create(
		window *parent,
		HINSTANCE hInstance,
		const DWORD dwStyle      = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_BORDER,
		const DWORD dwCtlStyle   = CCS_NODIVIDER | CCS_NOPARENTALIGN,
		const DWORD dwReBarStyle = RBS_BANDBORDERS | RBS_VARHEIGHT,
		const UINT  nID          = IDW_STD_REBAR);


	BOOL AddBar(
		HWND hWnd,
		const SIZE &size,
		const DWORD dwStyle = RBBS_GRIPPERALWAYS | RBBS_CHILDEDGE,
		LPCTSTR lpszText    = NULL,
		HBITMAP hBitmap     = NULL);


	// アトリビュート
	UINT GetBandCount(void) const;
	BOOL GetBarInfo(REBARINFO &rbi) const;
	BOOL GetBandInfo(const UINT uBand, REBARBANDINFO &rbbi) const;

	// オペレーション
	BOOL SetBarInfo(const REBARINFO &rbi);
	BOOL SetBandInfo(const UINT uBand, const REBARBANDINFO &rbbi);
	BOOL InsertBand(const UINT uBand, const REBARBANDINFO &rbbi);
	BOOL DeleteBand(const UINT uBand);
};

_WGC_END                                // }

#endif // __WGC_REBAR__
