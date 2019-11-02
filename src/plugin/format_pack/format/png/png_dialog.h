// png_dialog.h
#ifndef __PNG_DIALOG__
#define __PNG_DIALOG__


typedef struct tagPNGPARAM
{
	union CHECK
	{
		uint32_t checkflags;                    // チェックフラグ
		struct ELEMENTS
		{
			unsigned compress_text : 1;             // 画像ファイルにコメントを埋め込むか
			unsigned save_gamma    : 1;             // ウインドウのサイズと位置を保存するか
			unsigned save_time     : 1;             // ウインドウのサイズと位置を保存するか
		} elements;
	} check;
	uint_t interlace_type;                  // インターレースタイプ（0～1）
	uint_t filter_type;                     // フィルタタイプ(0～4）
	uint_t compression_level;               // 圧縮レベル（0～9）
} PNGPARAM;


class png_dialog : public wgc::dialog
{
public:
	png_dialog(const PNGPARAM &param, HWND hWndParent = NULL);

	PNGPARAM m_param;

protected:
	enum { IDD = IDD_SETTING_PNG };

	// コントロールオブジェクト
	wgc::checkbox_ctrl m_ctrlCompressText;  // テキストデータを圧縮するか？
	wgc::checkbox_ctrl m_ctrlSaveGamma;     // ガンマ値を保存するか？
	wgc::checkbox_ctrl m_ctrlSaveTime;      // 更新時刻を保存するか？
	wgc::slider_ctrl   m_ctrlCompressLevel; // 圧縮レベル

	void AssociateDialogControls(void);
	void UpdateData(const BOOL bUpdate = TRUE);

	BOOL OnInitDialog(HWND hWndFocus, const LPARAM dwInitParam);
};

#endif // __OPTIONDIALOGPNG__
