// quant.h Åcå∏êFèàóù
#ifndef __QUANT__
#define __QUANT__

#include "callback.h"
#include "quant_palette.h"
#include "quant_dither.h"


EXTERN_C
IMAGERESULT quantize(
	const IMAGE_INFO *in_info, const IMAGE_DATA *in_data,
	IMAGE_INFO *out_info, IMAGE_DATA *out_data,
	const uint_t new_colors, 
	const QUANT_PALETTE_METHOD palette_method, const QUANT_DITHER_METHOD dither_method,
	callback_t callback, long data);

#endif
