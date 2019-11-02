// stdafx.h …必要な共通ヘッダファイルをインクルード
#ifndef __STDAFX__
#define __STDAFX__


// 警告の無効化
#pragma warning(disable:4786)           // 「識別子が○○文字に切り捨てられました」


// 対応バージョン
#define WINVER        0x0400            // Windows 95 / NT 4.0 or later
#define _WIN32_IE     0x0000            // Internet Explorer なし


#include "../../common/image.h"
#include "../../common/bitmap.h"
#include "../../common/interpolate.h"
#include "../../common/thumbnail.h"
#include "../../common/mathematics.h"
#include "../../common/mem.h"
#include "../../common/template_utility.h"
#include "../../common/plugin/filter.h"

#include "../../common/wgc/wgc.h"
#include "../../common/wgc/wgfunc.h"
#include "../../common/wgc/DllMain.h"
#include "../../common/wgc/Window.h"
#include "../../common/wgc/Dialog.h"
#include "../../common/wgc/Control.h"
#include "../../common/wgc/CheckBoxCtrl.h"
#include "../../common/wgc/ScrollBarCtrl.h"
#include "../../common/wgc/SliderCtrl.h"
#include "../../common/wgc/SpinButtonCtrl.h"
#include "../../common/wgc/StaticCtrl.h"
#include "../../common/wgc/ToolTipCtrl.h"

#include "resource.h"

#endif // __STDAFX__
