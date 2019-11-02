// jpg_data_src.h
#ifndef __JPG_DATA_SRC__
#define __JPG_DATA_SRC__

#include <windows.h>
#include "jpg_common.h"


void jpeg_handle_src(j_decompress_ptr cinfo, HANDLE infile);

#endif // __JPG_DATA_SRC__
