// jpg_common.cpp
#include "../../stdafx.h"
#include "jpg_common.h"


// �G���[�n���h��
void error_handler(j_common_ptr cinfo)
{
	throw IR_INCORRECTHEADER;
}
