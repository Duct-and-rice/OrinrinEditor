/*! @file
	@brief �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
	���̃t�@�C���� stdafx.h �ł��B
	@author	SikigamiHNQ
	@date	2011/00/00
*/

/*
Orinrin Editor : AsciiArt Story Editor for Japanese Only
Copyright (C) 2011 - 2014 Orinrin/SikigamiHNQ

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.
*/
//-------------------------------------------------------------------------------------------------

#pragma once

#define STRICT

//!	�����ɂ���x��
#pragma warning( disable : 4100 )	//!<	�����͔����̖{�̕��� 1 �x���Q�Ƃ���܂���B
//#pragma warning( disable : 4101 )	//!<	���[�J���ϐ��� 1 �x���g���Ă��܂���B
#pragma warning( disable : 4201 )	//!<	��W���̊g���@�\���g�p����Ă��܂�
#pragma warning( disable : 4244 )	//!<	�^�ϊ��ɂ�����A�f�[�^��������\���ɂ��āB
#pragma warning( disable : 4312 )	//!<	���傫���T�C�Y�ւ̌^�ϊ��ɂ���
//#pragma warning( disable : 4995 )	//!<	���O��������ꂽ #pragma �Ƃ��ċL�q����Ă��܂��B

#include "targetver.h"

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")


//#define WIN32_LEAN_AND_MEAN		//	Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
// Windows �w�b�_�[ �t�@�C��:
#include <windows.h>
#include <shlobj.h>
#include <shellapi.h>
#include <windowsX.h>

//	�V�F���Ƃ�
#pragma comment(lib, "shell32.lib")

//	�R�����_�C�����O
#include <commdlg.h>
#pragma comment(lib, "Comdlg32.lib")

#include <Commctrl.h>
#pragma comment(lib, "ComCtl32.lib")

#ifndef _ORCOLL

//	IME�̑���Ɏg��
#include <imm.h>
#pragma comment(lib, "imm32.lib")

//	SQLite3
#include <sqlite3.h>
#pragma comment(lib, "sqlite3.lib")


#endif

// C �����^�C�� �w�b�_�[ �t�@�C��
#include <assert.h>

#define _CRTDBG_MAP_ALLOC	//	���������[�N�`�F�N�p
#include <stdlib.h>
#include <malloc.h>
#ifdef _DEBUG
#include <crtdbg.h>
#endif
#include <memory.h>
#include <tchar.h>
#include <time.h>

#include <sstream>

#define STRSAFE_NO_CB_FUNCTIONS
#include <strsafe.h>

#pragma warning( disable : 4995 )
#include <shlwapi.h>
#pragma warning( default : 4995 )
#pragma comment(lib, "shlwapi.lib")

//-------------------------------------------------------------------------------------------------

#pragma warning( disable : 4995 )	//	���O��������ꂽ #pragma �Ƃ��ċL�q����Ă��܂��B
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#pragma warning( default : 4995 )

using namespace	std;	//	���̃X�R�[�v���ł�std::���ȗ��ł���
//-------------------------------------------------------------------------------------------------

//���̃A�v����GUID�A���ɈӖ��͂Ȃ�
//	{66D3E881-972B-458B-935E-9E78B926B415}
static CONST GUID gcstGUID = { 0x66D3E881, 0x972B, 0x458B, { 0x93, 0x5E, 0x9E, 0x78, 0xB9, 0x26, 0xB4, 0x15 } };
//-------------------------------------------------------------------------------------------------

//�@�\�n�j
//	�s�̕ێ������X�g�ɂ��Ă݂�
//	�t�@�C���J��������
//	�v���t�J��������
//	�g�p�[�c�����s

#define ACCELERATOR_EDIT//	�L�[�̃A���ҏW
#define USE_HOVERTIP
#define EDGE_BLANK_STYLE	//	���C����������Ƃ��A���k�L�̈��p�ӂ���
//	�t�@�C���ǂݍ��񂾎��͕œW�J���Ȃ��A�Ђ悤�ɂȂ�����J��

#define MAA_IADD_PLUS	//	MAA�ւ̃A�C�e���ǉ��@�\�g��
//	���\�[�X�� IDM_MAA_IADD_OPEN IDM_MAA_ITEM_INSERT �ɒ���

#define MULTIACT_RELAY	//	�A�C�R���ɂc�c�ő��d�N��������A�J���Ă郄�c�ɂ�����
#define DOT_SPLIT_MODE	//	�^�񒆂���L������k�߂���
#define SPLIT_BAR_POS_FIX	//	��ʃT�C�Y�ς��Ă��X�v���b�g�o�[�̈ʒu���ς��Ȃ����



//	�쐬���̋@�\
#define BIG_TEXT_SEPARATE	//	����e�L�X�g�̕��������Ă݂�

#define TODAY_HINT_STYLE	//	�����̃q���g�@�\
#define SPMOZI_ENCODE		//	�@��ˑ������𐔒l�Q�ƃR�s�[

#define FIND_STRINGS	//	�����񌟍��@�\
//#define SEARCH_HIGHLIGHT	//	�����q�b�g�ʒu���n�C���C�g����
//�ʓ|�Ȃ̂Ŗ�����

#define MAA_TEXT_FIND	//	�l�`�`�ŁA�J���Ă�t�@�C������P��Search
//#define HUKUTAB_DRAGMOVE	
//#define PAGE_MULTISELECT//	�ňꗗ�̕����I��
//#define PLUGIN_ENABLE	//	�v���O�C���@�\
//#define MINI_TEMPLATE	

//#define WORK_LOG_OUT	

#define DO_TRY_CATCH	//	��O�΍􂵂Ă݂�
#define USE_NOTIFYICON	//	�^�X�N�g���C�A�C�R����L��
//-------------------------------------------------------------------------------------------------

//	�P�Ȃ鎯�ʖ��@extern�ŊO���Q�Ƃ���Ă�ϐ��ɂ������Ă���
#define EXTERNED
//-------------------------------------------------------------------------------------------------


#if defined(_DEBUG) || defined(WORK_LOG_OUT)
	#define TRACE(str,...)	OutputDebugStringPlus( GetLastError(), _CRT_WIDE(__FILE__), __LINE__, _CRT_WIDE(__FUNCTION__), str, __VA_ARGS__ )
	VOID	OutputDebugStringPlus( DWORD, LPTSTR, INT, LPTSTR, LPTSTR, ... );	//!<	
	//#define TRACE(str,...)	OutputDebugStringPlus( GetLastError(), __FILE__, __LINE__, __FUNCTION__, str, __VA_ARGS__ )
	//VOID	OutputDebugStringPlus( DWORD, LPSTR, INT, LPSTR, LPTSTR, ... );	//!<	
#else
	#define TRACE(x,...)
#endif

#ifndef _ORCOLL
	#ifdef _DEBUG
		#define SQL_DEBUG(db)	SqlErrMsgView(db,__LINE__)
		VOID	SqlErrMsgView( sqlite3 *, DWORD );
	#else
		#define SQL_DEBUG(db)
	#endif
#endif

#ifdef DO_TRY_CATCH
#define ETC_MSG(str,ret)	ExceptionMessage( str, __FUNCTION__, __LINE__, ret )

LRESULT	ExceptionMessage( LPCSTR, LPCSTR, UINT, LPARAM );
#endif

#define FREE(pp)	{if(pp){free(pp);pp=NULL;}}

//�X�e�[�^�X�o�[������ǉ��}�N��
#define StatusBar_SetText(hwndSB,ipart,ptext)	(BOOLEAN)SNDMSG((hwndSB),SB_SETTEXT,ipart,(LPARAM)(LPCTSTR)(ptext))
//-------------------------------------------------------------------------------------------------

#define MIN_STRING	20
#define SUB_STRING	64
#define MAX_STRING	130
#define BIG_STRING	520

//	���C���E�C���h�E�T�C�Y
#define W_WIDTH		480
#define W_HEIGHT	400

//	�E�C���h�E�T�C�Y�E���g�p
//#define WCL_WIDTH	480
//#define WCL_HEIGHT	370

//	�h�b�L���O�T�C�Y
#define PLIST_DOCK	190
#define TMPL_DOCK	150
//-------------------------------------------------------------------------------------------------

#ifdef USE_NOTIFYICON
#define WMP_TRAYNOTIFYICON	(WM_APP+1)
#endif
#define WMP_BRUSH_TOGGLE	(WM_APP+2)
#define WMP_PREVIEW_CLOSE	(WM_APP+3)
//-------------------------------------------------------------------------------------------------

#define BASIC_COLOUR	RGB(0xF0,0xF0,0xF0)	//	Collector�p
//-------------------------------------------------------------------------------------------------

#define USER_ITEM_FILE	TEXT("UserItem.ast")
#define USER_ITEM_MAX	16

#define AA_BRUSH_FILE	TEXT("aabrush.txt")		//	�h��Ԃ��p
#define AA_LIST_FILE	TEXT("aalist.txt")		//	��s�e���v���[�g

#define AA_MIRROR_FILE	TEXT("hantenX.txt")		//	���E���]�Q��
#define AA_UPSET_FILE	TEXT("hantenY.txt")		//	�㉺���]�Q��

#define MAA_FAVDB_FILE	TEXT("Favorite.qmt")	//	AA���X�g�p
#define MAA_TREE_CACHE	TEXT("TreeCache.qor")	//	�c���[�̒��g������Ă����E�g��Ȃ�

#define NAMELESS_DUMMY	TEXT("NoName0.txt")
#define NAME_DUMMY_NAME	TEXT("NoName")
#define NAME_DUMMY_EXT	TEXT("txt")

#define TEMPLATE_DIR	TEXT("Templates")
#define BACKUP_DIR		TEXT("BackUp")
#define PROFILE_DIR		TEXT("Profile")

#define DROP_OBJ_NAME	TEXT("[*DROP_OBJECT*]")
//-------------------------------------------------------------------------------------------------

//	�g�̐�	20110707	�g���P�O�ɑ��₵��
#define FRAME_MAX	20

//	�ݒ�ɂ���
#define INI_FILE		TEXT("Utuho.ini")
#define FRAME_INI_FILE	TEXT("Satori.ini")
#define MZCX_INI_FILE	TEXT("Koisi.ini")

#define HINT_FILE		TEXT("Today_Hint.txt")


#define INIT_LOAD		1
#define INIT_SAVE		0

#define WDP_MVIEW		1
#define WDP_PLIST		2
#define WDP_LNTMPL		3
#define WDP_BRTMPL		4
#define WDP_MAATPL		5
#define WDP_PREVIEW		6
#define WDP_MMAATPL		7

#define FONTSZ_NORMAL	16
#define FONTSZ_REDUCE	12

#define VL_CLASHCOVER	0	//!<	�N��������P�A����Ƃ��ɂO�ɂ��āACLASH�����m
#define VL_GROUP_UNDO	1	//!<	�O���[�v�A���h�D
#define VL_USE_UNICODE	2	//!<	���j�R�[�h�p�f�B���O
#define VL_LAYER_TRANS	3	//!<	���C���{�b�N�X�����x
#define VL_RIGHT_SLIDE	4	//!<	�E�񂹂�ʒu
#define VL_MAA_SPLIT	5	//!<	�l�`�`�̃X�v���b�g�o�[�̈ʒu
#define VL_MAA_LCLICK	6	//!<	�l�`�`�ꗗ�ŃN���b�R�����Ƃ��̕W������
#define VL_UNILISTLAST	7	//!<	���j�R�[�h�ꗗ������Ƃ��̈ʒu
#define VL_MAATIP_VIEW	8	//!<	�l�`�`�̂`�`�c�[���`�b�v�̕\�����邩
#define VL_MAATIP_SIZE	9	//!<	�l�`�`�̂`�`�c�[���`�b�v�̕����T�C�Y�E16��12
#define VL_LINETMP_CLM	10	//!<	��s�e���v���̃J������
#define VL_BRUSHTMP_CLM	11	//!<	�u���V�e���v���̃J������
#define VL_UNIRADIX_HEX	12	//!<	���j�R�[�h���l�Q�Ƃ��P�U�i���ɂ��邩�ǂ���[COL][EDIT]
#define VL_BACKUP_INTVL	13	//!<	�o�b�N�A�b�v���o�E�f�t�H�T�����炢�H
#define VL_BACKUP_MSGON	14	//!<	�o�b�N�A�b�v�����Ƃ��̃��b�Z�[�W��\�����邩�H
#define VL_GRID_X_POS	15	//!<	�O���b�h���̂w�h�b�g��
#define VL_GRID_Y_POS	16	//!<	�O���b�h���̂x�h�b�g��
#define VL_MAA_TOPMOST	17	//!<	VIEWER�l�`�`���őO�ʁEEDITOR�l�`�`��ON/OFF
#define VL_R_RULER_POS	18	//!<	�E���[���̈ʒu
#define VL_CRLF_CODE	19	//!<	���s�R�[�h�F�O������΁E��O�x�x
#define VL_SPACE_VIEW	20	//!<	�󔒂�\�����邩�@��O�\��
#define VL_GRID_VIEW	21	//!<	�O���b�h�\�����邩
#define VL_R_RULER_VIEW	22	//!<	�E���[���\�����邩
#define VL_PAGETIP_VIEW	23	//!<	�ňꗗ�̃c�[���`�b�v�\�����邩
#define VL_PCOMBINE_NM	24	//!<	�P�Ȃ瓝��Message����
#define VL_PDIVIDE_NM	25	//!<	�P�Ȃ番��Message����
#define VL_PDELETE_NM	26	//!<	�P�Ȃ�폜Message����
#define VL_MAASEP_STYLE	27	//!<	�����e���v���E�P�Ȃ��؂���X�^�C��
#define VL_USE_BALLOON	28	//!<	[COL]�ۑ��m�F���b�Z�[�W�\�����邩�ǂ���
#define VL_CLIPFILECNT	29	//!<	[COL]�ۑ��t�@�C���̌�
#define VL_PLS_LN_DOCK	30	//!<	�ňꗗ���͂�������
//#define VL_BRUSH_DOCK	31	//!<	��s�EBrush�e���v�����͂�������
#define VS_PROFILE_NAME	32	//!<	
#define VS_PAGE_FORMAT	33	//!<	�Ŕԍ��}���̕�����e���v��
#define VL_SWAP_COPY	34	//!<	�R�s�[�W����SJIS�ɂ��邩
#define VL_MAIN_SPLIT	35	//!<	���C���̃X�v���b�g�o�[�̉E����̈ʒu
#define VL_MAXIMISED	36	//!<	�ő剻�ŏI��������H
#define VL_DRT_LCLICK	37	//!<	�h���t�g�{�[�h�N���b�N�̃f�t�H����
#define VL_FIRST_READED	38	//!<	�}�j���A���Ǎ�����
#define VL_LAST_OPEN	39	//!<	�Ō�ɊJ���Ă����t�@�C�����@�O�J���@�P�J���Ȃ��@�Q����I��
#define VL_MAA_MCLICK	40	//!<	�l�`�`�ꗗ�Ń~�h�D�N���b�R�����Ƃ��̕W������
#define VL_DRT_MCLICK	41	//!<	�h���t�g�{�[�h�N���b�N�̃f�t�H����
#define VS_FONT_NAME	42	//!<	���C���̃t�H���g���A�l�r �o�S�V�b�N
#define VL_WORKLOG		43	//!<	���샍�O���o�͂��邩
#define VL_PAGE_UNDER	44	//!<	�Ŕԍ����ŉ��s�ɑ}��
#define VL_PAGE_OVWRITE	45	//!<	�Y���s�̓��e���폜���ď㏑
#define VL_COLLECT_AON	46	//!<	[COL]�N�����ɃR�s�y�ۑ���ON�ɂ���
#define VL_COLHOT_MODY	47	//!<	[COL]�|�b�v�A�b�v�z�b�g�L�[�E�C���q
#define VL_COLHOT_VKEY	48	//!<	[COL]�|�b�v�A�b�v�z�b�g�L�[�E���z�L�[�R�[�h
#define VL_MAA_RETFCS	49	//!<	MAA���ŁA�I��������ҏW���Ƀt�H�[�J�X�߂�
#define VL_PGL_RETFCS	50	//!<	�ňꗗ�ŁA�I��������ҏW���Ƀt�H�[�J�X�߂�
#define VL_U_RULER_POS	51	//!<	�����[���̈ʒu�s��
#define VL_U_RULER_VIEW	52	//!<	�����[���\�����邩
#define VL_PAGEBYTE_MAX	53	//!<	�냌�X�̍ő�o�C�g��
#define VS_UNI_USE_LOG	54	//!<	���j�R�[�h�����̎g�p���O
#define VL_MAA_BKCOLOUR	55	//!<	�l�`�`���̔w�i�F�ύX
#define VS_RGUIDE_MOZI	56	//!<	�E�������̕����E�땶������
#define VL_THUMB_HORIZ	57	//!<	�h���t�g�E�T���l�̕�
#define VL_THUMB_VERTI	58	//!<	�h���t�g�E�T���l�̍���
#define VL_MULTI_ACT_E	59	//!<	���d�N���L��
#define VL_SAVE_MSGON	60	//!<	�ۑ������Ƃ��̃��b�Z�[�W��\�����邩�H
#define VL_MAATAB_SNGL	61	//!<	MAA���̃^�u�A��s�\���ɂ���
#define VL_HINT_ENABLE	62	//!<	�����̃q���g��\�����邩�H
#define VL_SPMOZI_ENC	63	//!<	�@��ˑ������𐔒l�Q�ƃR�s�[	SPMOZI_ENCODE
#define VS_EXT_M2H_PATH	64	//!<	MLT2HTML�̃p�X

//���₵����A�������Ɏ戵�����Ă�������

#define CLRV_BASICPEN	101
#define CLRV_BASICBK	102
#define CLRV_GRIDLINE	103
#define CLRV_CRLFMARK	104
#define CLRV_CANTSJIS	105
//-------------------------------------------------------------------------------------------------

//	�X�e�[�^�X�o�[�̓�
#define SB_MODIFY	0	//!<	�ύX
#define SB_OP_STYLE	1	//!<	�I�y��
#define SB_MOUSEPOS	2	//!<	�}�E�X�J�[�\���ʒu
#define SB_CURSOR	3	//!<	�J�[�\���ʒu�̃h�b�g�l�Ƃ�
#define SB_LAYER	4	//!<	���C���{�b�N�X�̈ʒu
#define SB_BYTECNT	5	//!<	�o�C�g��
#define SB_SELBYTE	6	//!<	�I��͈͂̃o�C�g��
//-------------------------------------------------------------------------------------------------

//	���ԍ�
#define WND_MAIN	1
#define WND_MAAT	2
#define WND_PAGE	3
#define WND_LINE	4
#define WND_BRUSH	5
#define WND_TAIL	5	//	���[�_�~�[

//	���[�h
#define M_DESTROY	0
#define M_CREATE	1
#define M_EXISTENCE	2

#define MAA_DEFAULT		0xFF
#define MAA_SUBDEFAULT	0xFE
#define MAA_INSERT		0
#define MAA_INTERRUPT	1
#define MAA_LAYERED		2
#define MAA_UNICLIP		3
#define MAA_SJISCLIP	4
#define MAA_DRAUGHT		5

#define LASTOPEN_DO		0
#define LASTOPEN_NON	1
#define LASTOPEN_ASK	2

//	�w���R�[�h�E���Ԃ�Ȃ��悤��
#define D_SJIS		0x00	//	�V�t�gJIS
#define D_UNI		0x01	//	���j�R�[�h
#define D_SQUARE	0x02	//	��`�I��
#define D_INVISI	0x10	//	�s����Ԃ̓��ʏ���
#define D_RENAME	0x80	//	���O��t���ĕۑ�
//-------------------------------------------------------------------------------------------------

//	ImgCtl.dll�ɏ���
#define ISAVE_BMP	0x1
//#define ISAVE_JPEG	0x2
#define ISAVE_PNG	0x3
//-------------------------------------------------------------------------------------------------

#define CLIP_FORMAT	TEXT("ORINRIN_EDITOR_STYLE")	//!<	���ʗp�E���ɈӖ��͂Ȃ�
#define CLIP_SQUARE	TEXT("MSDEVColumnSelect")	//!<	��`�I�����ʎq�EVC�̂��
//-------------------------------------------------------------------------------------------------


//!	�L�[�R�[�h���蓖��
#define	VK_0	0x30
#define	VK_1	0x31
#define	VK_2	0x32
#define	VK_3	0x33
#define	VK_4	0x34
#define	VK_5	0x35
#define	VK_6	0x36
#define	VK_7	0x37
#define	VK_8	0x38
#define	VK_9	0x39

//	�K�v�ȃ{�^���ɖ��O�t�������Ă���
#define	VK_A	0x41
#define	VK_B	0x42
#define	VK_C	0x43
#define	VK_D	0x44
#define	VK_E	0x45
#define	VK_F	0x46
#define	VK_G	0x47
#define	VK_H	0x48
#define	VK_I	0x49
#define	VK_J	0x4A
#define	VK_K	0x4B
#define	VK_L	0x4C
#define	VK_M	0x4D
#define	VK_N	0x4E
#define	VK_O	0x4F
#define	VK_P	0x50
#define	VK_Q	0x51
#define	VK_R	0x52
#define	VK_S	0x53
#define	VK_T	0x54
#define	VK_U	0x55
#define	VK_V	0x56
#define	VK_W	0x57
#define	VK_X	0x58
#define	VK_Y	0x59
#define	VK_Z	0x5A
//-------------------------------------------------------------------------------------------------
