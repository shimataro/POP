/*--------------------------------------------------------------------------------------*/
/*	非LZW理論GIFデコーダー本体 実装ファイル	Ver 1.20									*/
/*	for VisualStudio6.0 only															*/
/*	※実際の展開処理を行い、DIBとして返却します。										*/
/*	Copyright (C) 2000 DJ☆Uchi [H.Uchida]												*/
/*--------------------------------------------------------------------------------------*/
#include "GIFDecode.h"
// #pragma inline_depth(255)
// #pragma inline_recursion(on)


// from "image.h"
#define CALC_HSIZE(width, bit)              (((width) * (bit) + 7) / 8)
#define CALC_PACK_OFFSET(pos, log2_depth)   ((pos) >> (~(log2_depth) & 0x03))
#define CALC_PACK_SHIFTS(pos, log2_depth)   ((~(pos) << (log2_depth)) & 0x07)

#define BEGIN_NAMESPACE(name)       namespace name {
#define BEGIN_NAMESPACE_NONAME()    namespace {
#define END_NAMESPACE()             }


/*--------------------------------------------------------------------------------------*/
/*	RGBDECODESTATUS構造体																*/
/*	DIBデータ書き込み用構造体															*/
/*	※DIBに画像データを書き込む際に必要な情報をまとめた物。								*/
/*--------------------------------------------------------------------------------------*/
typedef struct RGBDECODESTATUS
{
	DWORD rgb_pt;                           // 展開データ書き込み位置（同一ライン上でのオフセット値）
	DWORD rgb_offset;                       // 展開データ書き込みオフセット（ライン数×アラインメント）
	DWORD rgb_width;                        // DIB横pixel数
	DWORD rgb_height;                       // DIB縦pixel数
	DWORD rgb_hsize;                        // DIBアラインメント値（１ライン分のバイト数）
	DWORD rgb_depth;                        // DIBカラービット値
	DWORD rgb_line;                         // 展開データ書き込みライン（インタレース時に使用）
	DWORD image_left;                       // 画像のロジカルスクリーン上における左上の座標
	DWORD image_top;                        // 〃
	DWORD image_width;                      // 画像の幅
	DWORD image_height;                     // 画像の高さ
	DWORD image_offset;
	DWORD interlace_offset;                 // インタレースオフセット（ライン数）
	BOOL  interlace_flag;                   // インタレースフラグ
} RGBDECODESTATUS;


/*--------------------------------------------------------------------------------------*/
/*	GIFDECODESTATUS構造体																*/
/*	GIFデータ解析用構造体																*/
/*	※GIFの符号化コード取得に必要な情報をまとめた物。									*/
/*--------------------------------------------------------------------------------------*/
typedef struct GIFDECODESTATUS
{
	RGBDECODESTATUS *rs;                    // RGBDECODESTATUS構造体参照用ポインタ
	DWORD code_size;                        // コードサイズ（CS）
	DWORD bit_size;                         // ビットサイズ（CBL）
	DWORD clear_code;                       // クリアコード
	DWORD end_code;                         // エンドコード
	DWORD entry;                            // 現エントリ数
	DWORD bit_pt;                           // 現ビット位置
	DWORD next_block;                       // 次ブロック開始位置
	DWORD data_size;                        // データサイズ
} GIFDECODESTATUS;


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()
	DWORD GetCode(const BYTE data[], GIFDECODESTATUS *gs);
	void lzw_decode(BYTE buffer[], const WORD lzw[], GIFDECODESTATUS *gs, const int x, const int y);
	void lzw_string(BYTE buffer[], const WORD lzw[], GIFDECODESTATUS *gs, int x);
	void lzw_char  (BYTE buffer[], const WORD lzw[], GIFDECODESTATUS *gs, int x);
	void rgb_write (BYTE buffer[], RGBDECODESTATUS *rs, const WORD code);
END_NAMESPACE()


/*--------------------------------------------------------------------------------------*/
/*	GifDecode()																			*/
/*	GIF画像（最初の一枚のみ）を展開する。												*/
/*	※入力はGIFデータ、出力はindex（トップダウン型アラインメントなしDIB）				*/
/*	※ここでは主にデコードの前処理を行います。											*/
/*--------------------------------------------------------------------------------------*/
DWORD GifDecode(
	BYTE buffer[], const BYTE data[],
	const DWORD width, const DWORD height, const WORD depth,
	const DWORD left, const DWORD top, const DWORD image_width, const DWORD image_height,
	const BOOL interlace, const DWORD size)
{
	const DWORD hsize = CALC_HSIZE(width, depth);

	// データ書き込み用構造体初期化
	RGBDECODESTATUS rs;
	rs.rgb_pt     = 0;                      // 展開データ書き込み位置初期化
	rs.rgb_offset = 0;                      // 書き込み位置オフセット
	rs.rgb_width  = width;                  // 横pixel数取得
	rs.rgb_height = height;                 // 縦pixel数取得
	rs.rgb_hsize  = hsize;                  // アラインメント値取得
	rs.rgb_depth  = depth;                  // カラービット値取得
	rs.rgb_line   = 0;                      // 展開データ書き込みライン初期化
	rs.image_left   = left;
	rs.image_top    = top;
	rs.image_width  = image_width;
	rs.image_height = image_height;
	rs.image_offset = hsize * top;
	rs.interlace_offset = 8;                // インタレースオフセット初期化
	rs.interlace_flag   = interlace;        // インタレースフラグ取得

	// GIFデータ解析用構造体初期化
	GIFDECODESTATUS gs;
	gs.rs         = &rs;                    // RGBDECODESTATUS構造体参照用ポインタ取得
	gs.code_size  = data[0];                // コードサイズ取得
	gs.bit_size   = gs.code_size + 1;       // CBL初期化（コードサイズ+1）
	gs.clear_code = 1 << gs.code_size;      // クリアコード設定（2^コードサイズ）
	gs.end_code   = gs.clear_code + 1;      // エンドコード設定（2^コードサイズ+1）
	gs.entry      = gs.end_code + 1;        // エントリ数初期化（2^コードサイズ+2）
	gs.bit_pt     = 8;                      // ビットポインタ初期化(8bit=1byte)
	gs.next_block = 1;                      // 次ブロック位置初期化（先頭ブロック）
	gs.data_size  = size;                   // GIFデータサイズ取得

	int x = 0;								// 展開回数初期化
	int y = 0;								// 展開開始位置初期化

	WORD lzw[8192];                         // 圧縮データ格納用配列（辞書テーブルではない！）
	DWORD code = 0;                         // 符号化コード初期化

	// エンドコードが現れるまでひたすらループ
	while((code = GetCode(data, &gs)) != gs.end_code)
	{
		// クリアコードが現れた場合
		if(code == gs.clear_code)
		{
			// エントリ数＆CBL初期化
			gs.entry = gs.end_code + 1;
			gs.bit_size = gs.code_size + 1;

			// 展開関数をコール。
			lzw_decode(buffer, lzw, &gs, x, y);
			x = 0;
			y = 0;
		}
		else
		{
			// WORDサイズ配列に取得した符号化コードを溜め込む。
			// これは展開作業をより円滑に行う為であり、辞書テーブルとは異なります。
			// 配列に溜め込まずに取得した符号化コードを直に展開することも出来ますが、
			// 速度的に問題がある為、この方法を採用しています。
			lzw[x++] = (WORD)code;

			// いつまでもクリアコードが現れないと配列が溢れるため、
			// 符号化コードが8192個貯まった時点で展開作業を行う。
			// 非圧縮GIF対策か？
			if(x == 8192)
			{
				lzw_decode(buffer, lzw, &gs, x, y);
				x = 4096;
				y = 4096;
			}
			if(gs.entry < 4095)
			{
				gs.entry++;
			}
		}
	}
	// 配列に残っている符号化コードを展開する。
	lzw_decode(buffer, lzw, &gs, x, y);

	// 次のデータ位置を返す
	return (gs.bit_pt + 7) / 8;
}


////////////////////////////////////////////////////////////////////////////////
// anonymous namespace
BEGIN_NAMESPACE_NONAME()

/*--------------------------------------------------------------------------------------*/
/*	GetCode()																			*/
/*	可変ビット長入力関数。																*/
/*	※符号化コードを一つ取り出して、ビット位置をインクリメントする。					*/
/*	※取り出したデータはWORDサイズの配列に一定量貯めてからデコードする。				*/
/*--------------------------------------------------------------------------------------*/
DWORD GetCode(const BYTE data[], GIFDECODESTATUS *gs)
{
	DWORD code = 0;					// 符号化コード初期化
	DWORD pt = gs->bit_pt >> 3;		// 読み込み位置（バイト単位）取得

	// サイズオーバーフローの場合、強制的にエンドコードを返す（破損ファイル対策）
	if(pt + 2 > gs->data_size)
	{
		return gs->end_code;
	}

	// 符号化コード取得
	int i = 0;	// 読み込みバイト数初期化
	while((gs->bit_pt + gs->bit_size - 1) >> 3 >= pt)
	{
		// 読み込み中にブロックが終了した場合
		if(pt == gs->next_block)
		{
			gs->next_block += data[pt++] + 1;   // 次ブロック位置更新
			gs->bit_pt += 8;                    // ビットポインタを１バイト分加算
		}
		code += data[pt++] << i;            // コード取得
		i += 8;
	}

	// 得られたコードの余分なビットを切りとばす。（マスキング処理）
	code = (code >> (gs->bit_pt & 0x07)) & ((1 << gs->bit_size) - 1);

	// ビットポインタ更新
	gs->bit_pt += gs->bit_size;

	// CBLをインクリメントする必要があるかどうか確認する。
	if(gs->entry > (DWORD)((1 << gs->bit_size) - 1))
	{
		gs->bit_size++;
	}

	return code;
}


/*--------------------------------------------------------------------------------------*/
/*	lzw_decode()																		*/
/*	非LZW理論展開関数（メインループ）													*/
/*	※WORDサイズの配列に格納された符号化コードをデコードします。						*/
/*--------------------------------------------------------------------------------------*/
void lzw_decode(BYTE buffer[], const WORD lzw[], GIFDECODESTATUS *gs, const int x, const int y)
{
	// 単にループを回して展開関数を呼んでるだけ。
	for (int i = y; i < x; i++)
	{
		lzw_string(buffer, lzw, gs, i);
	}
}


/*--------------------------------------------------------------------------------------*/
/*	lzw_string()																		*/
/*	非LZW理論展開関数（コア）															*/
/*	※非LZW理論の核。																	*/
/*	※指定された符号化コードに対する展開データを返します。								*/
/*--------------------------------------------------------------------------------------*/
void lzw_string(BYTE buffer[], const WORD lzw[], GIFDECODESTATUS *gs, int x)
{
	// 配列から符号化コードを一つ取り出します。
	WORD code = lzw[x];

	// 符号化コードが”色数”より小さい場合
	if(code < gs->clear_code)
	{
		// 符号化コードをそのままDIBに書き込む。
		rgb_write(buffer, gs->rs, code);
	}
	// 符号化コードが未知のものである場合
	else if(code > gs->end_code + x--)
	{
		// 一つ前の展開データ
		lzw_string(buffer, lzw, gs, x);

		// 一つ前の展開データの先頭一個
		lzw_char(buffer, lzw, gs, x);
	}
	// 符号化コードが”色数+1”より大きい場合
	// ちなみに、この関数に入ってくるコードにエンドコードやクリアコードは
	// 絶対に現れませんので、その場合の処理は考慮されていません。
	else
	{
		// （符号化コード-色数+1）の展開データ
		lzw_string(buffer, lzw, gs, code - gs->end_code - 1);

		// （符号化コード-色数+2）の展開データの先頭一個
		lzw_char(buffer, lzw, gs, code - gs->end_code);
	}
}


/*--------------------------------------------------------------------------------------*/
/*	lzw_char()																			*/
/*	非LZW理論展開関数（サブ）															*/
/*	※非LZW理論の核。																	*/
/*	※指定された符号化コードに対する展開データの先頭１つを返します。					*/
/*--------------------------------------------------------------------------------------*/
void lzw_char(BYTE buffer[], const WORD lzw[], GIFDECODESTATUS *gs, int x)
{
	WORD code;
	// 配列から符号化コードを一つ取り出し、”色数”より小さかどうか確認。
	while((code = lzw[x]) >= gs->clear_code)
	{
		// 符号化コードが未知のものである場合
		if(code > gs->end_code + x)
		{
			// 一つ前の展開データの先頭一個
			x--;
		}
		// 符号化コードが”色数+1”より大きい場合
		else
		{
			// （符号化コード-色数+1）の展開データの先頭一個
			x = code - gs->end_code - 1;
		}
	}

	// 得られた展開データをDIBに書き込む。
	rgb_write(buffer, gs->rs, code);
}


/*--------------------------------------------------------------------------------------*/
/*	rgb_write()																			*/
/*	DIB画像データ書き込み関数。															*/
/*	※展開された画像データをDIBとして書き込みます。										*/
/*--------------------------------------------------------------------------------------*/
void rgb_write(BYTE buffer[], RGBDECODESTATUS *rs, const WORD code)
{
	// log2(depth)を計算
	const int log2_depth_array[] = {0, 1, 1, 2, 2, 2, 2, 3};
	const int log2_depth = log2_depth_array[rs->rgb_depth - 1];

	// オフセットとシフト量を求める
	const int x = rs->image_left + rs->rgb_pt++;
	const int offset = rs->image_offset + rs->rgb_offset + CALC_PACK_OFFSET(x, log2_depth);
	const int shifts = CALC_PACK_SHIFTS(x, log2_depth);

	// マスク値を求める
	const BYTE mask_array[] = {0x01, 0x03, 0x0f, 0xff}; // 00000001, 00000011, 00001111, 11111111
	const BYTE mask = ~(mask_array[log2_depth] << shifts);

	// codeをパッキングして書き込む
	buffer[offset] &= mask;
	buffer[offset] |= code << shifts;

	// 書き込み位置がラインの終端に達した場合
	if(rs->rgb_pt == rs->image_width)
	{
		// インタレースGIFの場合
		if(rs->interlace_flag)
		{
			// インタレースラインが画面下端に達した場合
			if(rs->rgb_line + rs->interlace_offset >= rs->image_height)
			{
				if((rs->rgb_line & 0x07) == 0)
				{
					rs->rgb_offset = rs->rgb_hsize * 4;
					rs->rgb_line   = 4;
				}
				else
				{
					switch(rs->interlace_offset)
					{
					case 8:
						rs->rgb_offset = rs->rgb_hsize * 2;
						rs->rgb_line   = 2;
						rs->interlace_offset = 4;
						break;

					case 4:
						rs->rgb_offset = rs->rgb_hsize * 1;
						rs->rgb_line   = 1;
						rs->interlace_offset = 2;
						break;
					}
				}
			}
			else
			{
				rs->rgb_offset += rs->rgb_hsize * rs->interlace_offset;
				rs->rgb_line   += rs->interlace_offset;
			}
		}
		// リニアGIFの場合
		else
		{
			rs->rgb_offset += rs->rgb_hsize;
		}
		rs->rgb_pt = 0;
	}
}

END_NAMESPACE()
