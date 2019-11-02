// PictBear.h
#ifndef __PICTBEAR__
#define __PICTBEAR__

#include <windows.h>


BOOL load_PictBear_plugin(LPCTSTR lpDirectory);
void free_PictBear_plugin(void);

BOOL install_PictBear_plugin(HMODULE hModule, const int filter_plugin_no);

#endif // __PICTBEAR__
