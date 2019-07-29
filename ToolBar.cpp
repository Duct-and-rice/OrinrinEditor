/*! @file
	@brief �c�[���o�[�����܂�
	���̃t�@�C���� ToolBar.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/09/11
*/

/*
Orinrin Editor : AsciiArt Story Editor for Japanese Only
Copyright (C) 2011 - 2013 Orinrin/SikigamiHNQ

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.
*/
//-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "OrinrinEditor.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

static  HWND	ghRebarWnd;			//!<	���o�[

static  HWND	ghMainTBWnd;		//!<	���C���c�[���o�[�̃E�C���h�E�n���h��
static  HWND	ghEditTBWnd;		//!<	�ҏW�c�[���o�[�̃E�C���h�E�n���h��
static  HWND	ghInsertTBWnd;		//!<	�}���c�[���o�[�̃E�C���h�E�n���h��
static  HWND	ghLayoutTBWnd;		//!<	���`�c�[���o�[�̃E�C���h�E�n���h��
static  HWND	ghViewTBWnd;		//!<	�\���c�[���o�[�̃E�C���h�E�n���h��

static HIMAGELIST	ghMainImgLst;	//!<	
static HIMAGELIST	ghEditImgLst;	//!<	
static HIMAGELIST	ghInsertImgLst;	//!<	
static HIMAGELIST	ghLayoutImgLst;	//!<	
static HIMAGELIST	ghViewImgLst;	//!<	

static WNDPROC	gpfOrigTBProc;		//!<	

extern HMENU	ghHistyMenu;		//	����\�����镔���E���I�ɓ��e�쐬���Ȃ�����
//-------------------------------------------------------------------------------------------------

static LRESULT	CALLBACK gpfToolbarProc( HWND, UINT, WPARAM, LPARAM );
//-------------------------------------------------------------------------------------------------

#define TBT_STRING	32

//	�t�@�C��
#define TB_MAIN_ITEMS	5
static  TBBUTTON	gstMainTBInfo[] = {
	{  0,	IDM_NEWFILE,		TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,						{0, 0}, 0, 0  },	//	
	{  1,	IDM_OPEN,			TBSTATE_ENABLED,	TBSTYLE_DROPDOWN | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  2,	IDM_OVERWRITESAVE,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,						{0, 0}, 0, 0  },	//	
	{  0,	0,					TBSTATE_ENABLED,	TBSTYLE_SEP,							{0, 0}, 0, 0  },
	{  3,	IDM_GENERAL_OPTION,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,						{0, 0}, 0, 0  } 	//	
};	//	���e�ύX������A�c�[���o�[������̐ݒ�Ƃ����ύX�Z��

static CONST TCHAR	gatMainTBtext[TB_MAIN_ITEMS][TBT_STRING] = { 
	{ TEXT("�V�K�쐬") },	{ TEXT("�t�@�C���J��") },	{ TEXT("�㏑���ۑ�") },	{ TEXT("") },
	{ TEXT("��ʐݒ�") }
};


//	�ҏW
#define TB_EDIT_ITEMS	21
static  TBBUTTON	gstEditTBInfo[] = {
	{  0,	IDM_UNDO,			TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	�A���h�D
	{  1,	IDM_REDO,			TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	���h�D
	{  0,	0,					TBSTATE_ENABLED,	TBSTYLE_SEP,						{0, 0}, 0, 0  },
	{  2,	IDM_CUT,			TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	�؂���
	{  3,	IDM_COPY,			TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	�R�s�[
	{  4,	IDM_PASTE,			TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	�\�t
	{  5,	IDM_DELETE,			TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	�폜
	{  0,	0,					TBSTATE_ENABLED,	TBSTYLE_SEP,						{0, 0}, 0, 0  },
	{  6,	IDM_SJISCOPY,		TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	�r�i�h�r�R�s�[
	{  7,	IDM_SJISCOPY_ALL,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	�őS��SJIS�R�s�[
	{  0,	0,					TBSTATE_ENABLED,	TBSTYLE_SEP,						{0, 0}, 0, 0  },
	{  8,	IDM_ALLSEL,			TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	�S�I��
	{  9,	IDM_SQSELECT,		TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	��`�I��
	{  0,	0,					TBSTATE_ENABLED,	TBSTYLE_SEP,						{0, 0}, 0, 0  },
	{ 10,	IDM_LAYERBOX,		TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	���C���{�b�N�X�N��
	{  0,	0,					TBSTATE_ENABLED,	TBSTYLE_SEP,						{0, 0}, 0, 0  },
	{ 11,	IDM_PAGEL_DIVIDE,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	�y�[�W����
	{  0,	0,					TBSTATE_ENABLED,	TBSTYLE_SEP,						{0, 0}, 0, 0  },
	{ 12,	IDM_EXTRACTION_MODE,TBSTATE_ENABLED,	TBSTYLE_CHECK | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	���o���[�h
	{  0,	0,					TBSTATE_ENABLED,	TBSTYLE_SEP,						{0, 0}, 0, 0  },
	{ 13,	IDM_UNICODE_TOGGLE,	TBSTATE_ENABLED,	TBSTYLE_CHECK | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	���j�R�[�h�g�pON/OFF
};

static CONST TCHAR	gatEditTBtext[TB_EDIT_ITEMS][TBT_STRING] = { 
	{ TEXT("���ɖ߂�") },	{ TEXT("��蒼��") },	{ TEXT("") },
	{ TEXT("�؂���") },	{ TEXT("�R�s�[") },	{ TEXT("�\�t") },	{ TEXT("�폜") },	{ TEXT("") },
	{ TEXT("SJIS�R�s�[") },	{ TEXT("�S�̂�SJIS�R�s�[") },	{ TEXT("") },
	{ TEXT("�S�I��") },	{ TEXT("��`�I��") },	{ TEXT("") },
	{ TEXT("���C���{�b�N�X") },	{ TEXT("") },
	{ TEXT("���̍s�ȍ~��V�łɕ���") },	{ TEXT("") },
	{ TEXT("�������o���[�h") },	{ TEXT("") },
	{ TEXT("���j�R�[�h�󔒂��g��") }
};


//	�}��
#define  TB_INSERT_ITEMS	6
static  TBBUTTON	gstInsertTBInfo[] = {
	{  0, IDM_IN_UNI_SPACE,		TBSTATE_ENABLED,	TBSTYLE_DROPDOWN | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  1, IDM_INSTAG_COLOUR,	TBSTATE_ENABLED,	TBSTYLE_DROPDOWN | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  2, IDM_FRMINSBOX_OPEN,	TBSTATE_ENABLED,	TBSTYLE_DROPDOWN | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  3, IDM_USERINS_NA,		TBSTATE_ENABLED,	TBSTYLE_DROPDOWN | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  0, 0,					TBSTATE_ENABLED,	TBSTYLE_SEP,							{0, 0}, 0, 0  },
	{  4, IDM_MOZI_SCR_OPEN,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,						{0, 0}, 0, 0  } 	//	
};

static CONST TCHAR	gatInsertTBtext[TB_INSERT_ITEMS][TBT_STRING] = { 
	{ TEXT("���j�R�[�h��") },	{ TEXT("�F�ύX�^�O") },	{ TEXT("�g") },	{ TEXT("���[�U��`") },	{ TEXT("") },
	{ TEXT("�����`�`�ϊ��{�b�N�X") }
};


//	���`
#define  TB_LAYOUT_ITEMS	19
static  TBBUTTON	gstLayoutTBInfo[] = {
	{  0, IDM_RIGHT_GUIDE_SET,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	�E������
	{  1, IDM_INS_TOPSPACE,		TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	�s���ɋ󔒑}��
	{  0, 0,					TBSTATE_ENABLED,	TBSTYLE_SEP,		{0, 0}, 0, 0  },
	{  2, IDM_DEL_TOPSPACE,		TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	�s���󔒍폜
	{  3, IDM_DEL_LASTSPACE,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	�s���󔒍폜
	{  4, IDM_DEL_LASTLETTER,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	�s�������폜
	{  0, 0,					TBSTATE_ENABLED,	TBSTYLE_SEP,		{0, 0}, 0, 0  },
	{ 10, IDM_MIRROR_INVERSE,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{ 11, IDM_UPSET_INVERSE,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  0, 0,					TBSTATE_ENABLED,	TBSTYLE_SEP,		{0, 0}, 0, 0  },
	{  5, IDM_RIGHT_SLIDE,		TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	�E�Ɋ񂹂�
	{  6, IDM_INCREMENT_DOT,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	�P�h�b�g���₷
	{  7, IDM_DECREMENT_DOT,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	�P�h�b�g���炷
	{  0, 0,					TBSTATE_ENABLED,	TBSTYLE_SEP,		{0, 0}, 0, 0  },
	{  8, IDM_INCR_DOT_LINES,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	�P�h�b�g�E��
	{  9, IDM_DECR_DOT_LINES,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	�P�h�b�g����
	{  0, 0,					TBSTATE_ENABLED,	TBSTYLE_SEP,		{0, 0}, 0, 0  },
	{ 12, IDM_DOT_SPLIT_LEFT,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	�^�񒆂���P�h�b�g�E��
	{ 13, IDM_DOT_SPLIT_RIGHT,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  } 	//	�^�񒆂���P�h�b�g����
};

static CONST TCHAR	gatLayoutTBtext[TB_LAYOUT_ITEMS][TBT_STRING] = { 
	{ TEXT("�E������") },	{ TEXT("�s���ɑS�p�󔒒ǉ�") },	{ TEXT("") },
	{ TEXT("�s���󔒍폜") },	{ TEXT("�s���󔒍폜") },	{ TEXT("�s�������폜") },	{ TEXT("") },
	{ TEXT("���E���]") },	{ TEXT("�㉺���]") },	{ TEXT("") },
	{ TEXT("�E�Ɋ񂹂�") },	{ TEXT("�P�h�b�g���₷") },	{ TEXT("�P�h�b�g���炷") },	{ TEXT("") },
	{ TEXT("�S�̂��P�h�b�g�E��") },	{ TEXT("�S�̂��P�h�b�g����") },	{ TEXT("") },
	{ TEXT("�w��ʒu���獶�ɋ��߂�") },	{ TEXT("�w��ʒu����E�֍L����") }
};

//	�\��
#define TB_VIEW_ITEMS	5
static  TBBUTTON	gstViewTBInfo[] = {
	{  0, IDM_UNI_PALETTE,			TBSTATE_ENABLED,	TBSTYLE_CHECK | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  1, IDM_TRACE_MODE_ON,		TBSTATE_ENABLED,	TBSTYLE_CHECK | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  2, IDM_ON_PREVIEW,			TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	
	{  3, IDM_DRAUGHT_OPEN,			TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	
	{  4, IDM_MAA_THUMBNAIL_OPEN,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  } 	//	
};

static CONST TCHAR	gatViewTBtext[TB_VIEW_ITEMS][TBT_STRING] = { 
	{ TEXT("���j�R�[�h�\") },	{ TEXT("�g���X���[�h") },	{ TEXT("�v���r���[") },	{ TEXT("�h���t�g�{�[�h") },	{ TEXT("�����s�T���l�C��") }
};

//�S��
#define TB_BAND_COUNT	5
static CONST REBARLAYOUTINFO	gcstReBarDef[] = {
	{ IDTB_MAIN_TOOLBAR,   180, RBBS_GRIPPERALWAYS | RBBS_CHILDEDGE               },	//	���C��
	{ IDTB_EDIT_TOOLBAR,   450, RBBS_GRIPPERALWAYS | RBBS_CHILDEDGE               },	//	�ҏW
	{ IDTB_INSERT_TOOLBAR, 280, RBBS_GRIPPERALWAYS | RBBS_CHILDEDGE | RBBS_BREAK  },	//	�}��
	{ IDTB_LAYOUT_TOOLBAR, 310, RBBS_GRIPPERALWAYS | RBBS_CHILDEDGE               },	//	���`
	{ IDTB_VIEW_TOOLBAR,   140, RBBS_GRIPPERALWAYS | RBBS_CHILDEDGE               } 	//	�\��
};	//	�����\��
//-------------------------------------------------------------------------------------------------

/*!
	�c�[���o�[�T�u�N���X
	WindowsXP�ŁA�c�[���o�[�̃{�^����Ń}�E�X�̍��{�^�����������܂܉E�{�^���������ƁA
	����ȍ~�̃}�E�X����𐳏�Ɏ󂯕t���Ȃ��Ȃ�B����̑΍�
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	msg		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@return	�����������ʂƂ�
*/
LRESULT CALLBACK gpfToolbarProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_CONTEXTMENU:
			TRACE( TEXT("TOOLBAR CONTEXT[%X]"), hWnd );
			break;

		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			if( SendMessage(hWnd, TB_GETHOTITEM, 0, 0) >= 0 ){	ReleaseCapture(   );	}
			return 0;
	}

	return CallWindowProc( gpfOrigTBProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------


/*!
	�c�[���o�[������E���o�[����
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	lcInst	�A�v���̎���
*/
VOID ToolBarCreate( HWND hWnd, HINSTANCE lcInst )
{
//	TBADDBITMAP	stToolBmp;
	TCHAR	atBuff[MAX_STRING];

	UINT			ici, resnum, d;
	REBARINFO		stRbrInfo;
	REBARBANDINFO	stRbandInfo;
	REBARLAYOUTINFO	stInfo[TB_BAND_COUNT];

	HBITMAP	hImg, hMsq;

//���o�[
	ghRebarWnd = CreateWindowEx( WS_EX_TOOLWINDOW, REBARCLASSNAME, NULL,	//	RBS_VARHEIGHT
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | RBS_BANDBORDERS | RBS_DBLCLKTOGGLE | CCS_NODIVIDER | CCS_TOP,
		0, 0, 0, 0, hWnd, (HMENU)IDRB_REBAR, lcInst, NULL);

	ZeroMemory( &stRbrInfo, sizeof(REBARINFO) );
	stRbrInfo.cbSize = sizeof(REBARINFO);
	SendMessage( ghRebarWnd, RB_SETBARINFO, 0, (LPARAM)&stRbrInfo );

	//	�����l
	ZeroMemory( stInfo, sizeof(stInfo) );
	for( d = 0; TB_BAND_COUNT > d; d++ )
	{
		stInfo[d].wID    = gcstReBarDef[d].wID;
		stInfo[d].cx     = gcstReBarDef[d].cx;
		stInfo[d].fStyle = gcstReBarDef[d].fStyle;
	}
	//	�ۑ���ԓǍ�
	InitToolBarLayout( INIT_LOAD, TB_BAND_COUNT, stInfo );
	//	�f�[�^�L��Ώ��������A�Ȃ������珉���l���̂܂�

//���C���c�[���o�[
	ghMainTBWnd = CreateWindowEx( 0, TOOLBARCLASSNAME, TEXT("maintb"),
		WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TOOLTIPS | CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER
		, 0, 0, 0, 0, ghRebarWnd, (HMENU)IDTB_MAIN_TOOLBAR, lcInst, NULL);
	//	�����c�[���`�b�v�X�^�C����ǉ�
	SendMessage( ghMainTBWnd, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS | TBSTYLE_EX_DRAWDDARROWS );

	ghMainImgLst = ImageList_Create( 16, 16, ILC_COLOR24 | ILC_MASK, 4, 1 );
	resnum = IDBMPQ_MAIN_TB_FIRST;
	for( ici = 0; 4 > ici; ici++ )
	{
		hImg = LoadBitmap( lcInst, MAKEINTRESOURCE( (resnum++) ) );
		hMsq = LoadBitmap( lcInst, MAKEINTRESOURCE( (resnum++) ) );
		ImageList_Add( ghMainImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ�
		DeleteBitmap( hImg );	DeleteBitmap( hMsq );
	}
	SendMessage( ghMainTBWnd, TB_SETIMAGELIST, 0, (LPARAM)ghMainImgLst );
	SendMessage( ghMainTBWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(16,16) );

	SendMessage( ghMainTBWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0 );
	//	�c�[���`�b�v�������ݒ�E�{�^���e�L�X�g���c�[���`�b�v�ɂȂ�
	StringCchCopy( atBuff, MAX_STRING, gatMainTBtext[0] );	gstMainTBInfo[0].iString = SendMessage( ghMainTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatMainTBtext[1] );	gstMainTBInfo[1].iString = SendMessage( ghMainTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatMainTBtext[2] );	gstMainTBInfo[2].iString = SendMessage( ghMainTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatMainTBtext[4] );	gstMainTBInfo[4].iString = SendMessage( ghMainTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );

	SendMessage( ghMainTBWnd, TB_ADDBUTTONS, (WPARAM)TB_MAIN_ITEMS, (LPARAM)&gstMainTBInfo );	//	�c�[���o�[�Ƀ{�^����}��

	SendMessage( ghMainTBWnd, TB_AUTOSIZE, 0, 0 );	//	�{�^���̃T�C�Y�ɍ��킹�ăc�[���o�[�����T�C�Y
//	InvalidateRect( ghMainTBWnd , NULL, TRUE );	//	�N���C�����g�S�̂��ĕ`�悷��

	//	�c�[���o�[�T�u�N���X��
	gpfOrigTBProc = SubclassWindow( ghMainTBWnd, gpfToolbarProc );



//�ҏW�c�[���o�[
	ghEditTBWnd = CreateWindowEx( 0, TOOLBARCLASSNAME, TEXT("edittb"),
		WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TOOLTIPS | CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER
		, 0, 0, 0, 0, ghRebarWnd, (HMENU)IDTB_EDIT_TOOLBAR, lcInst, NULL);
	//	�����c�[���`�b�v�X�^�C����ǉ�
	SendMessage( ghEditTBWnd, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS );

	//stToolBmp.hInst = HINST_COMMCTRL;
	//stToolBmp.nID   = IDB_STD_SMALL_COLOR;
	//SendMessage( ghEditTBWnd, TB_ADDBITMAP, 0, (LPARAM)&stToolBmp );
	ghEditImgLst = ImageList_Create( 16, 16, ILC_COLOR24 | ILC_MASK, 14, 1 );
	resnum = IDBMPQ_EDIT_TB_FIRST;
	for( ici = 0; 14 > ici; ici++ )
	{
		hImg = LoadBitmap( lcInst, MAKEINTRESOURCE( (resnum++) ) );
		hMsq = LoadBitmap( lcInst, MAKEINTRESOURCE( (resnum++) ) );
		ImageList_Add( ghEditImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ�
		DeleteBitmap( hImg );	DeleteBitmap( hMsq );
	}
	SendMessage( ghEditTBWnd, TB_SETIMAGELIST, 0, (LPARAM)ghEditImgLst );

	SendMessage( ghEditTBWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(16,16) );

	SendMessage( ghEditTBWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0 );
	//	�c�[���`�b�v�������ݒ�E�{�^���e�L�X�g���c�[���`�b�v�ɂȂ�
	StringCchCopy( atBuff, MAX_STRING , gatEditTBtext[ 0] );	gstEditTBInfo[ 0].iString = SendMessage( ghEditTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatEditTBtext[ 1] );	gstEditTBInfo[ 1].iString = SendMessage( ghEditTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatEditTBtext[ 3] );	gstEditTBInfo[ 3].iString = SendMessage( ghEditTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatEditTBtext[ 4] );	gstEditTBInfo[ 4].iString = SendMessage( ghEditTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatEditTBtext[ 5] );	gstEditTBInfo[ 5].iString = SendMessage( ghEditTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatEditTBtext[ 6] );	gstEditTBInfo[ 6].iString = SendMessage( ghEditTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatEditTBtext[ 8] );	gstEditTBInfo[ 8].iString = SendMessage( ghEditTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatEditTBtext[ 9] );	gstEditTBInfo[ 9].iString = SendMessage( ghEditTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatEditTBtext[11] );	gstEditTBInfo[11].iString = SendMessage( ghEditTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatEditTBtext[12] );	gstEditTBInfo[12].iString = SendMessage( ghEditTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatEditTBtext[14] );	gstEditTBInfo[14].iString = SendMessage( ghEditTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatEditTBtext[16] );	gstEditTBInfo[16].iString = SendMessage( ghEditTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatEditTBtext[18] );	gstEditTBInfo[18].iString = SendMessage( ghEditTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatEditTBtext[20] );	gstEditTBInfo[20].iString = SendMessage( ghEditTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	SendMessage( ghEditTBWnd, TB_ADDBUTTONS, (WPARAM)TB_EDIT_ITEMS, (LPARAM)&gstEditTBInfo );	//	�c�[���o�[�Ƀ{�^����}��
	SendMessage( ghEditTBWnd, TB_AUTOSIZE, 0, 0 );	//	�{�^���̃T�C�Y�ɍ��킹�ăc�[���o�[�����T�C�Y

	//	�c�[���o�[�T�u�N���X��
	gpfOrigTBProc = SubclassWindow( ghEditTBWnd, gpfToolbarProc );


//�}���c�[���o�[
	ghInsertTBWnd = CreateWindowEx( 0, TOOLBARCLASSNAME, TEXT("inserttb"),
		WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TOOLTIPS | CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER,
		0, 0, 0, 0, ghRebarWnd, (HMENU)IDTB_INSERT_TOOLBAR, lcInst, NULL);
	//	�����c�[���`�b�v�X�^�C����ǉ�	�h���b�v�_�E�����j���[��L���ɂ���
	SendMessage( ghInsertTBWnd, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS | TBSTYLE_EX_DRAWDDARROWS );

	//stToolBmp.hInst = HINST_COMMCTRL;
	//stToolBmp.nID   = IDB_HIST_SMALL_COLOR;
	//SendMessage( ghInsertTBWnd, TB_ADDBITMAP, 0, (LPARAM)&stToolBmp );
	ghInsertImgLst = ImageList_Create( 16, 16, ILC_COLOR24 | ILC_MASK, 5, 1 );
	resnum = IDBMPQ_INSERT_TB_FIRST;
	for( ici = 0; 4 > ici; ici++ )
	{
		hImg = LoadBitmap( lcInst, MAKEINTRESOURCE( (resnum++) ) );
		hMsq = LoadBitmap( lcInst, MAKEINTRESOURCE( (resnum++) ) );
		ImageList_Add( ghInsertImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ�
		DeleteBitmap( hImg );	DeleteBitmap( hMsq );
	}
	hImg = LoadBitmap( lcInst, MAKEINTRESOURCE( IDBMP_MOZI_SCRIPT ) );
	hMsq = LoadBitmap( lcInst, MAKEINTRESOURCE( IDBMQ_MOZI_SCRIPT ) );
	ImageList_Add( ghInsertImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ�
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	SendMessage( ghInsertTBWnd, TB_SETIMAGELIST, 0, (LPARAM)ghInsertImgLst );

	SendMessage( ghInsertTBWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(16,16) );

	SendMessage( ghInsertTBWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0 );
	//	�c�[���`�b�v�������ݒ�E�{�^���e�L�X�g���c�[���`�b�v�ɂȂ�
	StringCchCopy( atBuff, MAX_STRING, gatInsertTBtext[ 0] );	gstInsertTBInfo[ 0].iString = SendMessage( ghInsertTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatInsertTBtext[ 1] );	gstInsertTBInfo[ 1].iString = SendMessage( ghInsertTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatInsertTBtext[ 2] );	gstInsertTBInfo[ 2].iString = SendMessage( ghInsertTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatInsertTBtext[ 3] );	gstInsertTBInfo[ 3].iString = SendMessage( ghInsertTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatInsertTBtext[ 5] );	gstInsertTBInfo[ 5].iString = SendMessage( ghInsertTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	SendMessage( ghInsertTBWnd , TB_ADDBUTTONS, (WPARAM)TB_INSERT_ITEMS, (LPARAM)&gstInsertTBInfo );	//	�c�[���o�[�Ƀ{�^����}��
	SendMessage( ghInsertTBWnd, TB_AUTOSIZE, 0, 0 );	//	�{�^���̃T�C�Y�ɍ��킹�ăc�[���o�[�����T�C�Y

	//	�c�[���o�[�T�u�N���X��
	gpfOrigTBProc = SubclassWindow( ghInsertTBWnd, gpfToolbarProc );


//���`�c�[���o�[
	ghLayoutTBWnd = CreateWindowEx( 0, TOOLBARCLASSNAME, TEXT("layouttb"),
		WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TOOLTIPS | CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER,
		0, 0, 0, 0, ghRebarWnd, (HMENU)IDTB_LAYOUT_TOOLBAR, lcInst, NULL);
	//	�����c�[���`�b�v�X�^�C����ǉ�
	SendMessage( ghLayoutTBWnd, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS );

	//stToolBmp.hInst = HINST_COMMCTRL;
	//stToolBmp.nID   = IDB_VIEW_SMALL_COLOR;
	//SendMessage( ghLayoutTBWnd, TB_ADDBITMAP, 0, (LPARAM)&stToolBmp );
	ghLayoutImgLst = ImageList_Create( 16, 16, ILC_COLOR24 | ILC_MASK, 14, 1 );
	resnum = IDBMPQ_LAYOUT_TB_FIRST;
	for( ici = 0; 12 > ici; ici++ )
	{
		hImg = LoadBitmap( lcInst, MAKEINTRESOURCE( (resnum++) ) );
		hMsq = LoadBitmap( lcInst, MAKEINTRESOURCE( (resnum++) ) );
		ImageList_Add( ghLayoutImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ�
		DeleteBitmap( hImg );	DeleteBitmap( hMsq );
	}
	hImg = LoadBitmap( lcInst, MAKEINTRESOURCE( IDBMP_SPLIT_LEFT ) );
	hMsq = LoadBitmap( lcInst, MAKEINTRESOURCE( IDBMQ_SPLIT_LEFT ) );
	ImageList_Add( ghLayoutImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ�
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	hImg = LoadBitmap( lcInst, MAKEINTRESOURCE( IDBMP_SPLIT_RIGHT ) );
	hMsq = LoadBitmap( lcInst, MAKEINTRESOURCE( IDBMQ_SPLIT_RIGHT ) );
	ImageList_Add( ghLayoutImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ�
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	SendMessage( ghLayoutTBWnd, TB_SETIMAGELIST, 0, (LPARAM)ghLayoutImgLst );


	SendMessage( ghLayoutTBWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(16,16) );

	SendMessage( ghLayoutTBWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0 );
	//	�c�[���`�b�v�������ݒ�E�{�^���e�L�X�g���c�[���`�b�v�ɂȂ�
	StringCchCopy( atBuff, MAX_STRING, gatLayoutTBtext[ 0] );	gstLayoutTBInfo[ 0].iString = SendMessage( ghLayoutTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatLayoutTBtext[ 1] );	gstLayoutTBInfo[ 1].iString = SendMessage( ghLayoutTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatLayoutTBtext[ 3] );	gstLayoutTBInfo[ 3].iString = SendMessage( ghLayoutTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatLayoutTBtext[ 4] );	gstLayoutTBInfo[ 4].iString = SendMessage( ghLayoutTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatLayoutTBtext[ 5] );	gstLayoutTBInfo[ 5].iString = SendMessage( ghLayoutTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatLayoutTBtext[ 7] );	gstLayoutTBInfo[ 7].iString = SendMessage( ghLayoutTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatLayoutTBtext[ 8] );	gstLayoutTBInfo[ 8].iString = SendMessage( ghLayoutTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatLayoutTBtext[10] );	gstLayoutTBInfo[10].iString = SendMessage( ghLayoutTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatLayoutTBtext[11] );	gstLayoutTBInfo[11].iString = SendMessage( ghLayoutTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatLayoutTBtext[12] );	gstLayoutTBInfo[12].iString = SendMessage( ghLayoutTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatLayoutTBtext[14] );	gstLayoutTBInfo[14].iString = SendMessage( ghLayoutTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatLayoutTBtext[15] );	gstLayoutTBInfo[15].iString = SendMessage( ghLayoutTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatLayoutTBtext[17] );	gstLayoutTBInfo[17].iString = SendMessage( ghLayoutTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, gatLayoutTBtext[18] );	gstLayoutTBInfo[18].iString = SendMessage( ghLayoutTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );

	SendMessage( ghLayoutTBWnd , TB_ADDBUTTONS, (WPARAM)TB_LAYOUT_ITEMS, (LPARAM)&gstLayoutTBInfo );	//	�c�[���o�[�Ƀ{�^����}��

	SendMessage( ghLayoutTBWnd, TB_AUTOSIZE, 0, 0 );	//	�{�^���̃T�C�Y�ɍ��킹�ăc�[���o�[�����T�C�Y

	//	�c�[���o�[�T�u�N���X��
	gpfOrigTBProc = SubclassWindow( ghLayoutTBWnd, gpfToolbarProc );


//�\���c�[���o�[
	ghViewTBWnd = CreateWindowEx( 0, TOOLBARCLASSNAME, TEXT("viewtb"),
		WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TOOLTIPS | CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER,
		0, 0, 0, 0, ghRebarWnd, (HMENU)IDTB_VIEW_TOOLBAR, lcInst, NULL);
	//	�����c�[���`�b�v�X�^�C����ǉ�
	SendMessage( ghViewTBWnd, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS );

//	hImg = ImageList_LoadBitmap( lcInst, MAKEINTRESOURCE(IDBMP_UNIPALETTE), 16, 0, RGB(0xFF,0x00,0xFF) );
//	���������MASK�܂ňꔭ�����ł���E�Q�T�U�F�p���ۂ�
	ghViewImgLst = ImageList_Create( 16, 16, ILC_COLOR24 | ILC_MASK, 5, 1 );
	resnum = IDBMPQ_VIEW_TB_FIRST;
	for( ici = 0; 5 > ici; ici++ )
	{
		hImg = LoadBitmap( lcInst, MAKEINTRESOURCE( (resnum++) ) );
		hMsq = LoadBitmap( lcInst, MAKEINTRESOURCE( (resnum++) ) );
		ImageList_Add( ghViewImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ�
		DeleteBitmap( hImg );	DeleteBitmap( hMsq );
	}
	SendMessage( ghViewTBWnd, TB_SETIMAGELIST, 0, (LPARAM)ghViewImgLst );

	SendMessage( ghViewTBWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(16,16) );

	SendMessage( ghViewTBWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0 );
	//	�c�[���`�b�v�������ݒ�E�{�^���e�L�X�g���c�[���`�b�v�ɂȂ�
	StringCchCopy( atBuff, MAX_STRING , gatViewTBtext[ 0] );	gstViewTBInfo[ 0].iString = SendMessage( ghViewTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatViewTBtext[ 1] );	gstViewTBInfo[ 1].iString = SendMessage( ghViewTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatViewTBtext[ 2] );	gstViewTBInfo[ 2].iString = SendMessage( ghViewTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatViewTBtext[ 3] );	gstViewTBInfo[ 3].iString = SendMessage( ghViewTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING , gatViewTBtext[ 4] );	gstViewTBInfo[ 4].iString = SendMessage( ghViewTBWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );

	SendMessage( ghViewTBWnd, TB_ADDBUTTONS, (WPARAM)TB_VIEW_ITEMS, (LPARAM)&gstViewTBInfo );	//	�c�[���o�[�Ƀ{�^����}��
	SendMessage( ghViewTBWnd, TB_AUTOSIZE, 0, 0 );	//	�{�^���̃T�C�Y�ɍ��킹�ăc�[���o�[�����T�C�Y

	//	�c�[���o�[�T�u�N���X��
	gpfOrigTBProc = SubclassWindow( ghViewTBWnd, gpfToolbarProc );


	//�z�u
	ZeroMemory( &stRbandInfo, sizeof(REBARBANDINFO) );
	stRbandInfo.cbSize     = sizeof(REBARBANDINFO);
	stRbandInfo.fMask      = RBBIM_TEXT | RBBIM_STYLE | RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_SIZE | RBBIM_ID;
	stRbandInfo.cxMinChild = 0;
	stRbandInfo.cyMinChild = 25;

	for( d = 0; TB_BAND_COUNT > d; d++ )
	{
		switch( stInfo[d].wID )
		{
			case IDTB_MAIN_TOOLBAR:
				stRbandInfo.lpText    = TEXT("�t�@�C��");
				stRbandInfo.hwndChild = ghMainTBWnd;
				break;

			case IDTB_EDIT_TOOLBAR:
				stRbandInfo.lpText    = TEXT("�ҏW");
				stRbandInfo.hwndChild = ghEditTBWnd;
				break;

			case IDTB_INSERT_TOOLBAR:
				stRbandInfo.lpText    = TEXT("�}��");
				stRbandInfo.hwndChild = ghInsertTBWnd;
				break;

			case IDTB_LAYOUT_TOOLBAR:
				stRbandInfo.lpText    = TEXT("���`");
				stRbandInfo.hwndChild = ghLayoutTBWnd;
				break;

			case IDTB_VIEW_TOOLBAR:
				stRbandInfo.lpText    = TEXT("�\��");
				stRbandInfo.hwndChild = ghViewTBWnd;
				break;

			default:	continue;	break;
		}

		stRbandInfo.wID       = stInfo[d].wID;
		stRbandInfo.cx        = stInfo[d].cx;
		stRbandInfo.fStyle    = stInfo[d].fStyle;

		SendMessage( ghRebarWnd, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&stRbandInfo );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�c�[���o�[�̃C���t�H�`�b�v�̃A�N�Z���L�[�e�L�X�g��ύX
	@param[in]	pstAccel	���e�e�[�u���E��������NULL
	@param[in]	iEntry		�e�[�u���̃G���g����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ToolBarInfoChange( LPACCEL pstAccel, INT iEntry )
{
	TCHAR	atText[MAX_STRING];
	TBBUTTONINFO	stButtonInfo;

	INT	i;

	ZeroMemory( &stButtonInfo, sizeof(TBBUTTONINFO) );
	stButtonInfo.cbSize  = sizeof(TBBUTTONINFO);
	stButtonInfo.dwMask  = TBIF_TEXT;
	stButtonInfo.pszText = atText;
	stButtonInfo.cchText = MAX_STRING;

	for( i = 0; TB_MAIN_ITEMS > i; i++ )
	{
		if( 0 == gstMainTBInfo[i].idCommand )	continue;

		StringCchCopy( atText, MAX_STRING, gatMainTBtext[i] );
		AccelKeyTextBuild( atText, MAX_STRING , gstMainTBInfo[i].idCommand, pstAccel, iEntry );
		SendMessage( ghMainTBWnd, TB_SETBUTTONINFO, (WPARAM)(gstMainTBInfo[i].idCommand), (LPARAM)&stButtonInfo );
	}

	for( i = 0; TB_EDIT_ITEMS > i; i++ )
	{
		if( 0 == gstEditTBInfo[i].idCommand )	continue;

		StringCchCopy( atText, MAX_STRING, gatEditTBtext[i] );
		AccelKeyTextBuild( atText, MAX_STRING , gstEditTBInfo[i].idCommand, pstAccel, iEntry );
		SendMessage( ghEditTBWnd, TB_SETBUTTONINFO, (WPARAM)(gstEditTBInfo[i].idCommand), (LPARAM)&stButtonInfo );
	}

	for( i = 0; TB_INSERT_ITEMS > i; i++ )
	{
		if( 0 == gstInsertTBInfo[i].idCommand )	continue;

		StringCchCopy( atText, MAX_STRING, gatInsertTBtext[i] );
		AccelKeyTextBuild( atText, MAX_STRING , gstInsertTBInfo[i].idCommand, pstAccel, iEntry );
		SendMessage( ghInsertTBWnd, TB_SETBUTTONINFO, (WPARAM)(gstInsertTBInfo[i].idCommand), (LPARAM)&stButtonInfo );
	}

	for( i = 0; TB_LAYOUT_ITEMS > i; i++ )
	{
		if( 0 == gstLayoutTBInfo[i].idCommand )	continue;

		StringCchCopy( atText, MAX_STRING, gatLayoutTBtext[i] );
		AccelKeyTextBuild( atText, MAX_STRING , gstLayoutTBInfo[i].idCommand, pstAccel, iEntry );
		SendMessage( ghLayoutTBWnd, TB_SETBUTTONINFO, (WPARAM)(gstLayoutTBInfo[i].idCommand), (LPARAM)&stButtonInfo );
	}

	for( i = 0; TB_VIEW_ITEMS > i; i++ )
	{
		if( 0 == gstViewTBInfo[i].idCommand )	continue;

		StringCchCopy( atText, MAX_STRING, gatViewTBtext[i] );
		AccelKeyTextBuild( atText, MAX_STRING , gstViewTBInfo[i].idCommand, pstAccel, iEntry );
		SendMessage( ghViewTBWnd, TB_SETBUTTONINFO, (WPARAM)(gstViewTBInfo[i].idCommand), (LPARAM)&stButtonInfo );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�c�[���o�[�̔j��
*/
VOID ToolBarDestroy( VOID )
{
	ImageList_Destroy( ghMainImgLst );
	ImageList_Destroy( ghEditImgLst );
	ImageList_Destroy( ghLayoutImgLst );
	ImageList_Destroy( ghInsertImgLst );
	ImageList_Destroy( ghViewImgLst );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�c�[���o�[�̑傫�����Ƃ�
	@param[in]	pstRect	�T�C�Y��߂��o�b�t�@
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ToolBarSizeGet( LPRECT pstRect )
{
	RECT	rect;

	GetWindowRect( ghRebarWnd, &rect );

	rect.right  -= rect.left;
	rect.bottom -= rect.top;
	rect.left    = 0;
	rect.top     = 0;

	SetRect( pstRect, rect.left, rect.top, rect.right, rect.bottom );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	��΍��ڂ��`�F�b�N������O������
	@param[in]	itemID	���삷�郁�j���[�̂h�c
	@param[in]	bCheck	��O�`�F�b�N�E�O�͂���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ToolBarCheckOnOff( UINT itemID, UINT bCheck )
{
	HWND	hTlBrWnd;

	switch( itemID )
	{
		default:	return S_OK;

		case IDM_EXTRACTION_MODE:	hTlBrWnd =  ghEditTBWnd;	break;
		case IDM_SQSELECT:			hTlBrWnd =  ghEditTBWnd;	break;
		case IDM_UNICODE_TOGGLE:	hTlBrWnd =  ghEditTBWnd;	break;

		case IDM_UNI_PALETTE:		hTlBrWnd =  ghViewTBWnd;	break;
		case IDM_TRACE_MODE_ON:		hTlBrWnd =  ghViewTBWnd;	break;
	}

	SendMessage( hTlBrWnd, TB_CHECKBUTTON, itemID, bCheck ? TRUE : FALSE );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E���T�C�Y�ύX���ꂽ�Ƃ�
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	state	�ύX�̏�ԁESIZE_MINIMIZED �Ƃ�
	@param[in]	cx		�N���C�����g�w�T�C�Y
	@param[in]	cy		�N���C�����g�x�T�C�Y
	@return		����
*/
HRESULT ToolBarOnSize( HWND hWnd, UINT state, INT cx, INT cy )
{
	FORWARD_WM_SIZE( ghRebarWnd, state, cx, cy, SendMessage );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�m�[�e�B�t�@�C���b�Z�[�W�̃_�~�[����
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	itemID	�����ꂽ�{�^���̂h�c
*/
VOID ToolBarPseudoDropDown( HWND hWnd, INT itemID )
{
	NMTOOLBAR	stNmToolBar;
	INT	iFrom;


	ZeroMemory( &stNmToolBar, sizeof(NMTOOLBAR) );

	switch( itemID )
	{
		case IDM_IN_UNI_SPACE:
		case IDM_INSTAG_COLOUR:
		case IDM_USERINS_NA:
			stNmToolBar.hdr.hwndFrom = ghInsertTBWnd;
			iFrom = IDTB_INSERT_TOOLBAR;
			break;

		default:	 return;	//	���邱�Ƃ͂Ȃ�
	}

	stNmToolBar.hdr.idFrom   = iFrom;
	stNmToolBar.hdr.code     = TBN_DROPDOWN;
	stNmToolBar.iItem        = itemID;

	ToolBarOnNotify( hWnd, iFrom, (LPNMHDR)(&stNmToolBar) );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�R���e�L�X�g���j���[�Ăт����A�N�V����(�v�͉E�N���b�N�j���o�[�̂��
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	hWndContext	�R���e�L�X�g�����������E�C���h�E�̃n���h��
	@param[in]	xPos		�X�N���[���w���W
	@param[in]	yPos		�X�N���[���x����
	@return		�����������O�A��������������O
*/
LRESULT ToolBarOnContextMenu( HWND hWnd, HWND hWndContext, LONG xPos, LONG yPos )
{
	HMENU	hPopupMenu;

	if( ghRebarWnd != hWndContext ){	return 0;	}

	TRACE( TEXT("REBAR CONTEXT[%d x %d]"), xPos, yPos );

	hPopupMenu = CreatePopupMenu(  );
	AppendMenu( hPopupMenu, MF_STRING, IDM_REBER_DORESET, TEXT("�z�u��������Ԃɖ߂�") );
	TrackPopupMenu( hPopupMenu, 0, xPos, yPos, 0, hWnd, NULL );
	DestroyMenu( hPopupMenu );

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�m�[�e�B�t�@�C���b�Z�[�W�̏���
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	idFrom		NOTIFY�𔭐��������R���g���[���̂h�c
	@param[in]	pstNmhdr	NOTIFY�̏ڍ�
	@return		�����������e�Ƃ��E���ɕԂ����m�͂Ȃ�
*/
LRESULT ToolBarOnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	INT			iItem, i;
	HWND		hWndFrom;
	HMENU		hPopupMenu = NULL, hMainMenu;
	LPNMTOOLBAR	pstNmToolBar;
	TPMPARAMS	stTpmParam;
	RECT		rect;


	if( IDRB_REBAR == idFrom )
	{
		if( RBN_HEIGHTCHANGE == pstNmhdr->code )
		{
			if( !( AppClientAreaCalc( &rect ) ) )	return 0;
			ViewSizeMove( hWnd, &rect );	//	���C���̃c�[���o�[�ŕω���������
			InvalidateRect( hWnd, NULL, TRUE );
			InvalidateRect( ghRebarWnd, NULL, TRUE );
		}
	}

	//	�c�[���o�[�̃h���b�v�_�E�����j���[	IDTB_INSERT_TOOLBAR == idFrom && 
	if( TBN_DROPDOWN == pstNmhdr->code )
	{
		TRACE( TEXT("�h���b�v�_�E������") );
		pstNmToolBar = (LPNMTOOLBAR)pstNmhdr;

		iItem    = pstNmToolBar->iItem;
		hWndFrom = pstNmToolBar->hdr.hwndFrom;

		hMainMenu = GetMenu( hWnd );

		//	�{�^���̍��W���擾
		SendMessage( hWndFrom, TB_GETRECT, (WPARAM)iItem, (LPARAM)(&rect) );
		//	���̃E�B���h�E����Ƃ�����W�֕ϊ�
		MapWindowPoints( hWndFrom, HWND_DESKTOP, (LPPOINT)(&rect), 2 );

		ZeroMemory( &stTpmParam, sizeof(TPMPARAMS) );
		stTpmParam.cbSize = sizeof(TPMPARAMS);
		stTpmParam.rcExclude = rect;

		switch( iItem )
		{
			case IDM_OPEN:
				TrackPopupMenuEx( ghHistyMenu, TPM_VERTICAL, rect.left, rect.bottom, hWnd, &stTpmParam );
				break;

			case IDM_IN_UNI_SPACE:
				hPopupMenu = GetSubMenu( GetSubMenu(hMainMenu,2), 0 );
				TrackPopupMenuEx( hPopupMenu, TPM_VERTICAL, rect.left, rect.bottom, hWnd, &stTpmParam );
				break;

			case IDM_INSTAG_COLOUR:
				hPopupMenu = GetSubMenu( GetSubMenu(hMainMenu,2), 1 );
				TrackPopupMenuEx( hPopupMenu, TPM_VERTICAL, rect.left, rect.bottom, hWnd, &stTpmParam );
				break;

			case IDM_FRMINSBOX_OPEN:
				hPopupMenu = CreatePopupMenu(  );
				for( i = 0; FRAME_MAX > i; i++ ){	AppendMenu( hPopupMenu, MF_STRING, (IDM_INSFRAME_ALPHA + i), TEXT("�g") );	}
				FrameNameModifyPopUp( hPopupMenu, 1 );
				TrackPopupMenuEx( hPopupMenu, TPM_VERTICAL, rect.left, rect.bottom, hWnd, &stTpmParam );
				DestroyMenu( hPopupMenu );
				break;

			case IDM_USERINS_NA:
				hPopupMenu = CreatePopupMenu(  );
				UserDefMenuWrite( hPopupMenu, 0 );
				TrackPopupMenuEx( hPopupMenu, TPM_VERTICAL, rect.left, rect.bottom, hWnd, &stTpmParam );
				DestroyMenu( hPopupMenu );
				break;

			default:	break;
		}

	}

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	���o�[�̔z�u�����ɖ߂�
*/
HRESULT ToolBarBandReset( HWND hWnd )
{
	INT		index;
	UINT	d;
	REBARBANDINFO	stRbandInfo;

	TRACE( TEXT("REBAR RESET") );

	ZeroMemory( &stRbandInfo, sizeof(REBARBANDINFO) );
	stRbandInfo.cbSize = sizeof(REBARBANDINFO);
	stRbandInfo.fMask  = RBBIM_STYLE | RBBIM_SIZE;

	for( d = 0; TB_BAND_COUNT > d; d++ )
	{
		index = SendMessage( ghRebarWnd, RB_IDTOINDEX, gcstReBarDef[d].wID, 0 );
		if( 0 > index ){	continue;	}

		SendMessage( ghRebarWnd, RB_MOVEBAND, index, d );

		stRbandInfo.cx     = gcstReBarDef[d].cx;
		stRbandInfo.fStyle = gcstReBarDef[d].fStyle;
		SendMessage( ghRebarWnd, RB_SETBANDINFO, (WPARAM)d, (LPARAM)&stRbandInfo );
	}

	InvalidateRect( ghRebarWnd, NULL, TRUE );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���o�[�̔z�u�����m�F����
*/
UINT ToolBarBandInfoGet( LPVOID pVoid )
{
	UINT	d;
	REBARBANDINFO	stBandInfo;
	REBARLAYOUTINFO	stInfo[TB_BAND_COUNT];

	ZeroMemory( &stBandInfo, sizeof(REBARBANDINFO) );
	stBandInfo.cbSize = sizeof(REBARBANDINFO);
	stBandInfo.fMask = RBBIM_ID | RBBIM_STYLE | RBBIM_SIZE;

	ZeroMemory( stInfo, sizeof(stInfo) );

	for( d = 0; TB_BAND_COUNT > d; d++ )
	{
		SendMessage( ghRebarWnd, RB_GETBANDINFO, (WPARAM)d, (LPARAM)(&stBandInfo) );
		TRACE( TEXT("ID[%u]  CX[%d]  STYLE[%u]"), stBandInfo.wID, stBandInfo.cx, stBandInfo.fStyle );

		stInfo[d].wID    = stBandInfo.wID;
		stInfo[d].cx     = stBandInfo.cx;
		stInfo[d].fStyle = stBandInfo.fStyle;
	}

	InitToolBarLayout( INIT_SAVE, TB_BAND_COUNT, stInfo );	//	�ۑ�

	return TB_BAND_COUNT;
}
//-------------------------------------------------------------------------------------------------

/*
����N�����Ɉʒu�𕜌����邽�߂ɕK�v�ȏ���RB_GETBANDINFO���b�Z�[�W�Œ��ׂ�B

��������̂ɕK�v�Ȃ̂́A�o���h�̕���ł��鏇�ԁA�o���h�̕��A�����Ăǂ̃o���h�ŉ��s���邩�B

UINT count = SendMessage(hRebar, RB_GETBANDCOUNT, 0, 0);

REBARBANDINFO rbi = {0};
rbi.cbSize = sizeof(REBARBANDINFO);
rbi.fMask = RBBIM_ID | RBBIM_STYLE | RBBIM_SIZE;

DWORD written;
for (UINT i = 0; i < count; ++i)
{
	SendMessage(hRebar, RB_GETBANDINFO, i, (LPARAM)&rbi);

	WriteFile(hfile, &rbi.wID, sizeof(rbi.wID), &written, NULL);
	
	bool br = rbi.fStyle & RBBS_BREAK;
	WriteFile(hfile, &br, sizeof(br), &written, NULL);
	
	WriteFile(hfile, &rbi.cx, sizeof(rbi.cx), &written, NULL);
}

RBBS_BREAK�̓o���h��}���������ǂ����������ł͂Ȃ��A
RB_GETBANDINFO�𑗂������_�ł̉��s�󋵂������Ă����B
�������P�s�ڂ̍ŏ��̃o���h�ɂ�RBBS_BREAK���܂܂��ꍇ�Ɗ܂܂�Ȃ��ꍇ������B


[�I����]
char iniSection[50];
REBARBANDINFO bandInfo;
ZeroMemory(&bandInfo, sizeof(bandInfo)); 
bandInfo.cbSize = sizeof(bandInfo);
bandInfo.fMask = RBBIM_ID | RBBIM_SIZE | RBBIM_STYLE;
int barcnt = SendMessage(wprop->hRebar, RB_GETBANDCOUNT, 0, 0);
for(int i = 0; i < barcnt; i++){
    sprintf(iniSection, INISECTION_BAR, i);
    SendMessage(hRebar, RB_GETBANDINFO, (WPARAM)(UINT)i, (LPARAM)(LPREBARBANDINFO)&bandInfo);

    // wID, cx, fStyle �� ini �t�@�C���ɕۑ�����
    MyWriteProfileInt(iniSection, "ID", bandInfo.wID);
    MyWriteProfileInt(iniSection, "CX", bandInfo.cx);
    MyWriteProfileInt(iniSection, "FL_BREAK", (bandInfo.fStyle & RBBS_BREAK) ? 1 : 0);
    MyWriteProfileInt(iniSection, "FL_HIDDEN", (bandInfo.fStyle & RBBS_HIDDEN) ? 1 : 0);
}


[�N����]
int i;
char iniSection[50];
// CreateToolbarx �̓o���h���̃c�[���o�[���쐬����֐�
HWND (*hwndproc[])(HWND) = {CreateToolbar1, CreateToolbar2};

REBARBANDINFO rbBand;
ZeroMemory(&rbBand, sizeof(REBARBANDINFO));
rbBand.cbSize = sizeof(REBARBANDINFO);
rbBand.fMask  = RBBIM_ID | RBBIM_STYLE | RBBIM_CHILD | RBBIM_CHILDSIZE | 
RBBIM_SIZE;
rbBand.cxMinChild = 0;
rbBand.cyMinChild = 25;
for(i = 0; i < NUM_TOOL; i++){
    sprintf(iniSection, INISECTION_BAR, i);
    rbBand.wID = SZC_GetProfileInt(iniSection, "ID", 9999);
    rbBand.cx = SZC_GetProfileInt(iniSection, "CX", 100);
    rbBand.fStyle = RBBS_CHILDEDGE;
    if(SZC_GetProfileInt(iniSection, "FL_BREAK", 0)) rbBand.fStyle |= RBBS_BREAK;
    if(SZC_GetProfileInt(iniSection, "FL_HIDDEN", 0)) rbBand.fStyle |= RBBS_HIDDEN;
    rbBand.hwndChild = hwndproc[rbBand.wID](hRebar);
    SendMessage(wprop->hRebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand);
}

*/
