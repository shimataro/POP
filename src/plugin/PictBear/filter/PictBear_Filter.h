// PictBear_Filter.h
#ifndef __PICTBEAR_FILTER__
#define __PICTBEAR_FILTER__

#include "PbPlugin.h"


int get_PictBear_plugin_filter_func(HMODULE hModule, const int filter_plugin_no);
int get_filter_count(void);

// PictBearプラグインの関数をコール
void call_PBF_GetName_sub(int filter_no, LPSTR kind, int kind_len, LPSTR name, int name_len);
void call_PBF_Execute_sub(int filter_no, HWND hwnd, const Pixel32* src, Pixel32* dst, BYTE* mask, RECT* area, DWORD* maskflag, int width, int height, FARPROC proc, long data);
BOOL call_PBF_Setup_sub  (int filter_no, HWND hwnd, const Pixel32* src, int width, int height);

#endif // __PICTBEAR_FILTER__
