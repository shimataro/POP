// bitmap.cpp …インプリメンテーションファイル
#include "stdafx.h"
#include "bitmap.h"
#include "mem.h"                        // auto_lock

#include <math.h>                       // pow()

#include <vector>                       // std::vector<>


/////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	// アルファブレンディング
	inline unsigned char alpha_blend(const unsigned char fgc, const unsigned char bgc, const unsigned char alpha)
	{
		// 1. 以下の変形で乗算1回分の最適化
		// 　(fgc * alpha + bgc * (255 - alpha)) / 255 ＝ (fgc - bgc) * alpha / 255 + bgc
		// 2. 255での除算を256での除算で近似
		// 　コンパイラの最適化により除算が算術シフトに置き換えられることを期待

		const int tmp = (fgc - bgc) * alpha;
		return tmp / 256 + bgc;
	}

	void _BITMAP_SetDataFromImage_gray8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, const_uint8_t gamma_table[256], RGBQUAD *buffer);
	void _BITMAP_SetDataFromImage_index(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, const_uint8_t gamma_table[256], RGBQUAD *buffer);
	void _BITMAP_SetDataFromImage_rgb8 (const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, const_uint8_t gamma_table[256], RGBQUAD *buffer);
	void _BITMAP_SetDataFromImage_alphablend(const int width, const int height, const int grid_size, const COLORREF bgc1, const COLORREF bgc2, RGBQUAD *argb);
END_NAMESPACE()


/////////////////////////////////////////////////////////////////////////////////
// BITMAPINFOから32ビットDIBセクションを作成
EXTERN_C
HBITMAP DIBSECTION_Create32bits(const int width, const int height, BITMAPINFO *bitmapinfo_ptr, void **buffer_ptr)
{
	// デスクトップのデバイスコンテキストを取得
	HDC hDesktopDC = ::GetDC(HWND_DESKTOP);

	// BITMAPINFO構造体を作成
	BITMAPINFO bitmapinfo;
	bitmapinfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biWidth         = width;      // 幅
	bitmapinfo.bmiHeader.biHeight        = height;     // 高さ
	bitmapinfo.bmiHeader.biPlanes        = 1;          // プレーン数（must be 1）
	bitmapinfo.bmiHeader.biBitCount      = 32;         // ビット数（32ビットに固定）
	bitmapinfo.bmiHeader.biCompression   = BI_RGB;     // 圧縮形式（無圧縮）
	bitmapinfo.bmiHeader.biSizeImage     = 0;          // イメージサイズ（無圧縮なので0）
	bitmapinfo.bmiHeader.biXPelsPerMeter = 0;          // 水平解像度（0x0b13）
	bitmapinfo.bmiHeader.biYPelsPerMeter = 0;          // 垂直解像度（0x0b13）
	bitmapinfo.bmiHeader.biClrUsed       = 0;          // 使用されている色
	bitmapinfo.bmiHeader.biClrImportant  = 0;          // 重要な色の数（？）

	// DIBセクションを作成
	HBITMAP hDIBSection = ::CreateDIBSection(
		hDesktopDC,
		&bitmapinfo,
		DIB_RGB_COLORS,
		buffer_ptr,
		NULL,
		0);

	if(bitmapinfo_ptr != NULL)
	{
		*bitmapinfo_ptr = bitmapinfo;
	}

	// デバイスコンテキストを開放
	::ReleaseDC(HWND_DESKTOP, hDesktopDC);

	return hDIBSection;
}

// IMAGE_DATAのデータをバッファに格納
EXTERN_C
void BITMAP_SetDataFromImage(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, const_uint8_t gamma_table[256], const int grid_size, const COLORREF bgc1, const COLORREF bgc2, RGBQUAD *argb)
{
	switch(info_ptr->mode)
	{
	case COLORMODE_GRAYSCALE:
		_BITMAP_SetDataFromImage_gray8(info_ptr, data_ptr, gamma_table, argb);
		break;

	case COLORMODE_INDEX:
		_BITMAP_SetDataFromImage_index(info_ptr, data_ptr, gamma_table, argb);
		break;

	case COLORMODE_RGB:
		_BITMAP_SetDataFromImage_rgb8 (info_ptr, data_ptr, gamma_table, argb);
		break;
	}

	// アルファブレンディング
	if(info_ptr->alpha)
	{
		_BITMAP_SetDataFromImage_alphablend(
			info_ptr->width, info_ptr->height,
			grid_size,
			bgc1, bgc2,
			argb);
	}
}

// 画像からビットマップハンドルを作成
EXTERN_C
HBITMAP HBITMAP_CreateFromImage(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr)
{
	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	// 画像データを格納
	std::vector<RGBQUAD> argb(width * height);
	if(TRUE)
	{
		IMAGE_INFO info = *info_ptr;
		info.alpha = FALSE;

		// ガンマテーブルを作成
		uint8_t gamma_table[256];
		for(int i = 0; i < SIZE_OF_ARRAY(gamma_table); i++)
		{
			gamma_table[i] = i;
		}

		BITMAP_SetDataFromImage(
			&info, data_ptr,
			gamma_table,
			0,
			0, 0,
			&argb[0]);
	}

	// ビットマップハンドルを作成
	HBITMAP hBitmap = NULL;
	if(TRUE)
	{
		HDC hDC = ::GetDC(HWND_DESKTOP);

		hBitmap = ::CreateCompatibleBitmap(hDC, width, height);
		if(hBitmap != NULL)
		{
			BITMAPINFO bitmapinfo;
			bitmapinfo.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
			bitmapinfo.bmiHeader.biWidth       = width;
			bitmapinfo.bmiHeader.biHeight      = height;
			bitmapinfo.bmiHeader.biPlanes      = 1;
			bitmapinfo.bmiHeader.biBitCount    = 32;
			bitmapinfo.bmiHeader.biCompression = 0;

			::SetDIBits(
				hDC, hBitmap,
				0, height,
				&argb[0], &bitmapinfo,
				DIB_RGB_COLORS);
		}

		::ReleaseDC(HWND_DESKTOP, hDC);
	}
	return hBitmap;
}

// ビットマップハンドルから画像を取得
EXTERN_C
IMAGERESULT HBITMAP_GetImage(HBITMAP hBitmap, IMAGE_INFO *info_ptr, IMAGE_DATA *data_ptr)
{
	// ビットマップのサイズを取得
	SIZE size;
	HBITMAP_GetSize(hBitmap, &size);

	const int width  = size.cx;
	const int height = size.cy;

	// IMAGE_INFO構造体の設定
	info_ptr->width      = width;
	info_ptr->height     = height;
	info_ptr->depth      = 8;
	info_ptr->mode       = COLORMODE_RGB;
	info_ptr->colors     = 0;
	info_ptr->alpha      = FALSE;
	info_ptr->density_x  = 0;
	info_ptr->density_y  = 0;
	info_ptr->gamma_file = GAMMA_DEFAULT_FILE;
	info_ptr->comment    = NULL;

	// メモリを確保
	if(!image_alloc(info_ptr, data_ptr))
	{
		return IR_NOMEMORY;
	}

	if(TRUE)
	{
		// メモリ領域をロック
		pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
		auto_lock lock_obj(data_ptr, pixel_rgb8_ptr);

		HBITMAP_GetData(hBitmap, &size, pixel_rgb8_ptr);
	}

	return IR_SUCCESS;
}

// ビットマップのサイズを取得
EXTERN_C
BOOL HBITMAP_GetSize(HBITMAP hBitmap, LPSIZE lpSize)
{
	BITMAP bitmap;
	::GetObject(hBitmap, sizeof(bitmap), &bitmap);

	lpSize->cx = bitmap.bmWidth;
	lpSize->cy = bitmap.bmHeight;
	return TRUE;
}

// ビットマップのビット配列を取得
EXTERN_C
BOOL HBITMAP_GetData(HBITMAP hBitmap, const SIZE *lpSize, LPVOID lpBuffer)
{
	// デスクトップのデバイスコンテキストを取得
	HDC hDesktopDC = ::GetDC(HWND_DESKTOP);

	// BITMAPINFO構造体を設定
	BITMAPINFO bitmapinfo;
	bitmapinfo.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biWidth       = lpSize->cx;
	bitmapinfo.bmiHeader.biHeight      =-lpSize->cy;
	bitmapinfo.bmiHeader.biPlanes      = 1;
	bitmapinfo.bmiHeader.biBitCount    = 32;
	bitmapinfo.bmiHeader.biCompression = 0;

	// ビット配列を取得
	const int result = ::GetDIBits(
		hDesktopDC,
		hBitmap,
		0, lpSize->cy,
		lpBuffer,
		&bitmapinfo,
		DIB_RGB_COLORS);

	// デバイスコンテキストを開放
	::ReleaseDC(HWND_DESKTOP, hDesktopDC);
	hDesktopDC = NULL;

	if(result == 0)
	{
		return FALSE;
	}
	return TRUE;
}

// ウインドウハンドルからビットマップハンドルを取得
EXTERN_C
HBITMAP GetWindowBitmap(HWND hWnd, BOOL bAltogether)
{
	RECT rect;                              // ウインドウの矩形
	HDC hDC;                                // デバイスコンテキスト

	// ウインドウ全体
	if(bAltogether)
	{
		::GetWindowRect(hWnd, &rect);
		hDC = ::GetWindowDC(hWnd);
	}
	// ウインドウの内側
	else
	{
		::GetClientRect(hWnd, &rect);
		hDC = ::GetDC(hWnd);
	}
	const LONG width  = rect.right - rect.left;
	const LONG height = rect.bottom - rect.top;

	// ビットマップハンドルを作成
	HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, width, height);

	// hBitmapにウインドウの指定領域をコピー
	HDC hDCMem         = ::CreateCompatibleDC(hDC);
	HGDIOBJ hOldObject = ::SelectObject(hDCMem, hBitmap);
	::BitBlt(hDCMem, 0, 0, width, height, hDC, 0, 0, SRCCOPY);


	////////////////////////////////////////
	// 後処理

	// 選択されたオブジェクトを元に戻す
	::SelectObject(hDCMem, hOldObject);
	hOldObject = NULL;

	// メモリデバイスコンテキストを削除
	::DeleteDC(hDCMem);
	hDCMem = NULL;

	// デバイスコンテキストを開放
	::ReleaseDC(hWnd, hDC);
	hDC = NULL;

	return hBitmap;
}

// ガンマテーブルの作成
EXTERN_C
void CreateGammaTable(const double gamma_file, const double gamma_decode, const double gamma_display, uint8_t gamma_table[256])
{
	const double power = 1 / (gamma_decode * gamma_file * gamma_display);

	for(int i = 0; i < 256; i++)
	{
		double p = i / 255.0;
		*gamma_table++ = static_cast<uint8_t>(pow(p, power) * 255);
	}
}

// スケールアップテーブルを作成
EXTERN_C
void create_scaleup_table(uint8_t table[], const_uint_t depth)
{
	assert(depth != 0);
	const int table_size = 1 << depth;

	for(int i = 0; i < 8; i += depth)
	{
		for(int j = 0; j < table_size; j++)
		{
			table[j] <<= depth;
			table[j]  |= j;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

void _BITMAP_SetDataFromImage_gray8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data_ptr, const_uint8_t gamma_table[256], RGBQUAD *argb)
{
	// 画像情報を取得
	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	// メモリ領域をロック
	const_pixel_gray8_ptr_t pixel_gray8_ptr = NULL;
	auto_lock lock_obj(data_ptr, pixel_gray8_ptr);

	// ガンマデコーディングをしながらバッファに格納
	int offset = width * (height - 1);
	for(int y = 0; y < height; y++)
	{
		RGBQUAD *line = &argb[offset];
		offset -= width;
		for(int x = 0; x < width; x++)
		{
			const_uint8_t gray_gamma = gamma_table[pixel_gray8_ptr->el.gray];

			line->rgbBlue     = gray_gamma;
			line->rgbGreen    = gray_gamma;
			line->rgbRed      = gray_gamma;
			line->rgbReserved = pixel_gray8_ptr->el.alpha;

			pixel_gray8_ptr++;
			line++;
		}
	}
}

void _BITMAP_SetDataFromImage_index(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data, const_uint8_t gamma_table[256], RGBQUAD *argb)
{
	// 画像情報を取得
	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	const_uint_t   depth_index = info_ptr->depth;
	const_uint32_t hsize_index = CALC_HSIZE(width, depth_index);

	// log2(depth)を計算
	const_uint_t log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const_uint_t log2_depth = log2_depth_array[depth_index - 1];

	// メモリ領域をロック
	const_uint8_ptr_t    index_ptr = NULL;
	const_palette8_ptr_t palette8_ptr = NULL;
	auto_lock lock_obj(data, index_ptr, palette8_ptr);

	// ガンマデコーディングをしながらバッファに格納
	int offset = width * (height - 1);
	for(int y = 0; y < height; y++)
	{
		RGBQUAD *line = &argb[offset];
		offset -= width;
		for(int x = 0; x < width; x++)
		{
			// オフセットとシフト量を求める
			const_uint32_t offset_index = CALC_PACK_OFFSET(x, log2_depth);
			const int      shifts_index = CALC_PACK_SHIFTS(x, log2_depth);

			// インデックス値を取得
			const_uint8_t index = PACK_EXTRACT(index_ptr[offset_index], depth_index, shifts_index);

			// 前景色を取得
			const_palette8_t palette = palette8_ptr[index];

			// ガンマデコーディング
			line->rgbBlue     = gamma_table[palette.b];
			line->rgbGreen    = gamma_table[palette.g];
			line->rgbRed      = gamma_table[palette.r];
			line->rgbReserved = palette.a;
			line++;
		}
		index_ptr += hsize_index;
	}
}

void _BITMAP_SetDataFromImage_rgb8(const IMAGE_INFO *info_ptr, const IMAGE_DATA *data, const_uint8_t gamma_table[256], RGBQUAD *argb)
{
	// 画像情報を取得
	const int width  = info_ptr->width;
	const int height = info_ptr->height;

	// メモリ領域をロック
	const_pixel_rgb8_ptr_t pixel_rgb8_ptr = NULL;
	auto_lock lock_obj(data, pixel_rgb8_ptr);

	// ガンマデコーディングをしながらバッファに格納
	int offset = width * (height - 1);
	for(int y = 0; y < height; y++)
	{
		RGBQUAD *line = &argb[offset];
		offset -= width;
		for(int x = 0; x < width; x++)
		{
			line->rgbBlue     = gamma_table[pixel_rgb8_ptr->el.b];
			line->rgbGreen    = gamma_table[pixel_rgb8_ptr->el.g];
			line->rgbRed      = gamma_table[pixel_rgb8_ptr->el.r];
			line->rgbReserved = pixel_rgb8_ptr->el.a;

			pixel_rgb8_ptr++;
			line++;
		}
	}
}

void _BITMAP_SetDataFromImage_alphablend(const int width, const int height, const int grid_size, const COLORREF bgc1, const COLORREF bgc2, RGBQUAD *argb)
{
	const int bgc_r[] = {GetRValue(bgc1), GetRValue(bgc2)};
	const int bgc_g[] = {GetGValue(bgc1), GetGValue(bgc2)};
	const int bgc_b[] = {GetBValue(bgc1), GetBValue(bgc2)};

	for(int y = height - 1; y >= 0; y--)
	{
		for(int x = 0; x < width; x++)
		{
			// 前景色とアルファチャネルを取得
			const_uint8_t b     = argb->rgbBlue;
			const_uint8_t g     = argb->rgbGreen;
			const_uint8_t r     = argb->rgbRed;
			const_uint8_t alpha = argb->rgbReserved;

			// 背景の格子番号（0 or 1）を計算
			// 以下の式を最適化
			// 　((x >> grid_size) & 1) ^ ((y >> grid_size) & 1)
			const int area = ((x ^ y) >> grid_size) & 1;

			// アルファブレンディングしてバッファに格納
			argb->rgbBlue  = alpha_blend(b, bgc_b[area], alpha);
			argb->rgbGreen = alpha_blend(g, bgc_g[area], alpha);
			argb->rgbRed   = alpha_blend(r, bgc_r[area], alpha);
			argb++;
		}
	}
}

END_NAMESPACE()
