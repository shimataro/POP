// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#if !defined(AFX_STDAFX_H__BF5410AC_332D_11D5_8205_08004605D14C__INCLUDED_)
#define AFX_STDAFX_H__BF5410AC_332D_11D5_8205_08004605D14C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B

// �Ή��o�[�W����
#define WINVER        0x0400            // Windows 95 / NT or later
#define _WIN32_IE     0x0400            // Internet Explorer 4 or later

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������
#include <afxdisp.h>        // MFC �̃I�[�g���[�V���� �N���X
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
#endif // _AFX_NO_AFXCMN_SUPPORT


// �{�v���W�F�N�g�̃C���N���[�h�t�@�C��
#include <htmlhelp.h>                   // ::HtmlHelp()
#include <shlwapi.h>                    // ::PathRenameExtension()

// �R�����w�b�_�t�@�C�����C���N���[�h
#include "..\common\image.h"            // �摜������{�w�b�_�t�@�C��
#include "..\common\bitmap.h"           // �r�b�g�}�b�v����֐�
#include "..\common\interpolate.h"      // ��ԃA���S���Y��
#include "..\common\mathematics.h"      // ���x�D�搔�w�֐�
#include "..\common\mem.h"              // �������Ǘ��֐�
#include "..\common\template_utility.h" // �e���v���[�g���[�e�B���e�B�[�֐�
#include "..\common\plugin\plugin.h"
#include "..\common\plugin\filter.h"
#include "..\common\plugin\format.h"


// �x���̖�����
#pragma warning(disable:4786)           // �u���ʎq�����������ɐ؂�̂Ă��܂����v


// for���̐擪�Œ�`���ꂽ�ϐ��̃X�R�[�v��for�����Ɍ���i�ŐV��C++����d�l�j
#define for     if(0); else for


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__BF5410AC_332D_11D5_8205_08004605D14C__INCLUDED_)
