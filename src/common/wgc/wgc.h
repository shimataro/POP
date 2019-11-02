// wgc.h Åc Windows Generalized Component
#ifndef __WGC__
#define __WGC__

#include <windows.h>
#include <tchar.h>

#include <string>


// SIZE_OF_ARRAY
#ifndef SIZE_OF_ARRAY
	#define SIZE_OF_ARRAY(array)    (sizeof(array) / sizeof(array[0]))
#endif // SIZE_OF_ARRAY


// wgc ñºëOãÛä‘
#define _WGC_BEGIN  namespace wgc {
#define _WGC_END    }
#define _WGC_USING  using namespace wgc;


_WGC_BEGIN

// å^ÇÃíËã`
typedef std::string  string_t;
typedef std::wstring wstring_t;
typedef std::basic_string<TCHAR> tstring_t;


class devicecontext;
class imm;
class ole_drop_source;
class registry;
class window;
	class dialog;
	class control;
		class checkbox_ctrl;
		class scrollbar_ctrl;
		class spinbutton_ctrl;
		class static_ctrl;
		class tooltip_ctrl;

_WGC_END

#endif // __WGC__
