// jpg_common.h …
#ifndef __JPG_COMMON__
#define __JPG_COMMON__

extern "C"
{
#include "jpeglib/jpeglib.h"
}


// エラーハンドラ
void error_handler(j_common_ptr cinfo);

#endif // __JPG_COMMON__
