/*! @file
	@brief �c�����̖ʓ|�݂܂�
	���̃t�@�C���� VerticalScript.cpp �ł��B
	@author	SikigamiHNQ
	@date	2012/01/11
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

/*
�c�����@�\
�����ȊO�͓���
�E����A��������ׂ�ؑ�
�s�Ԓ���
��Ǔ_�͉E���

�����ȊO�̓��߂͂ǂ����邩

�땶�����ɕ��������Ă����B��Ǔ_�͓��ꏈ�����Ȃ��ƌ��h������낵���Ȃ�
���͂��ꂽ�s���ɕێ��B���������s�ɂȂ�E�\���̖�肾��
�������ɂw�h�b�g�ʒu���v�Z���Ă����Ă����B
�m�肵���Ƃ��́A�e�i���ɁA�p�f�B���O�󔒂���čs�ɕϊ����ăX�e���X���C������

�v���V�[�W���̏������L�ł��Ȃ��H

�c�[���o�[
�m��E�X�V�E���߁E������
�`�F�L�{�L�X�ŏI����������
*/
//-------------------------------------------------------------------------------------------------

#define VERTSCRIPT_CLASS	TEXT("VERTSCRIPT_CLASS")
#define VT_WIDTH	320
#define VT_HEIGHT	240

#define VT_PARAMHEI	25

#define LEFT_PADD	15	//	���]�T�E���S���ɂ������� 

#define IDEO_COMMA	TEXT('�A')	//	11dot
#define IDEO_FSTOP	TEXT('�B')	//	11dot

#define VERTVIEW_CLASS	TEXT("VERTVIEW_CLASS")
//-------------------------------------------------------------------------------------------------

#define TB_ITEMS	5
static  TBBUTTON	gstVttbInfo[] = {
	{  0,	IDM_VLINE_DECIDE,		TBSTATE_ENABLED,	TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  0,	0,						TBSTATE_ENABLED,	TBSTYLE_SEP,						{0, 0}, 0, 0  },
	{  1,	IDM_VLINE_REFRESH,		TBSTATE_ENABLED,	TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  2,	IDCB_VLINE_LEFT_GO,		TBSTATE_ENABLED,	TBSTYLE_CHECK | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  3,	IDM_VLINE_TRANSPARENT,	TBSTATE_ENABLED,	TBSTYLE_CHECK | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  } 	//	
};	//	
//-------------------------------------------------------------------------------------------------


typedef struct tagVERTITEM
{
	TCHAR	cch;	//!<	����
	INT		iWidth;	//!<	�������E���S�ɂȂ�悤�ɑ�����

	INT		iOffset;//!<	�ō�����̉E�I�t�Z�b�g�h�b�g�E���Έʒu�ł͂Ȃ�

	INT		iRow;	//!<	�c�ɂ������̂x�ʒu
	INT		iColumn;//!<	�c�ɂ������̕�����ԍ��E���s�ɒ���

} VERTITEM, *LPVERTITEM;
//-------------------------------------------------------------------------------------------------

extern FILES_ITR	gitFileIt;		//!<	�����Ă�t�@�C���̖{��
//#define gstFile	(*gitFileIt)		//!<	�C�e���[�^���\���̂ƌ��Ȃ�

extern INT			gixFocusPage;	//	���ڒ��̃y�[�W�E�O�C���f�b�N�X

extern HFONT		ghAaFont;		//	AA�p�t�H���g

extern  BYTE		gbAlpha;		//	�����x

extern  HWND		ghViewWnd;		//	�ҏW�r���[�E�C���h�E�̃n���h��
extern INT			gdHideXdot;		//	���̉B�ꕔ��
extern INT			gdViewTopLine;	//	�\�����̍ŏ㕔�s�ԍ�

static  HWND		ghVertToolBar;	//!<	
static HIMAGELIST	ghVertImgLst;	//!<	

static  ATOM		gVertAtom;		//!<	
EXTERNED HWND		ghVertWnd;		//!<	

static  HWND		ghTextWnd;		//!<	��������͘g
static INT			gdToolBarHei;	//!<	�c�[���o�[����

static  ATOM		gVertViewAtom;
static  HWND		ghVertViewWnd;	//!<	�\���X�^�e�B�b�N

static POINT		gstViewOrigin;	//!<	�r���[�̍���E�C���h�E�ʒu�E
static POINT		gstOffset;		//!<	�r���[���ォ��́A�{�b�N�X�̑��Έʒu
static POINT		gstFrmSz;		//!<	�E�C���h�E�G�b�W����`��̈�܂ł̃I�t�Z�b�g

static INT			gdVertInterval;	//!<	�s�Ԋu�h�b�g���E�f�t�H���Q�Q��
static  UINT		gbLeftGo;		//!<	��O������@�O�E����z�u����

static  UINT		gbSpTrans;		//!<	�󔒂��@��O���߁@�O���߂��Ȃ�

static LPTSTR		gptVtBuff;		//!<	�e�L�X�g�g���當���m�ۘg�E��
static DWORD		gcchVtBuf;		//!<	�m�ۘg�̕������E�o�C�g����Ȃ���

static BOOLEAN		gbQuickClose;	//!<	�\��t�����璼������

static WNDPROC		gpfOrigVertEditProc;	//!<	


static  vector<VERTITEM>	gvcVertItem;
typedef vector<VERTITEM>::iterator	VTIM_ITR;
typedef vector<VERTITEM>::reverse_iterator	VTIM_RITR;
//-------------------------------------------------------------------------------------------------

static LRESULT	CALLBACK gpfVertEditProc( HWND , UINT, WPARAM, LPARAM );	//!<	

LRESULT	CALLBACK VertProc( HWND, UINT, WPARAM, LPARAM );	//!<	
VOID	Vrt_OnCommand( HWND , INT, HWND, UINT );	//!<	
VOID	Vrt_OnPaint( HWND );	//!<	
VOID	Vrt_OnDestroy( HWND );	//!<	
LRESULT	Vrt_OnNotify( HWND , INT, LPNMHDR );	//!<	

LRESULT	CALLBACK VertViewProc( HWND, UINT, WPARAM, LPARAM );	//!<	
VOID	Vvw_OnKey( HWND, UINT, BOOL, INT, UINT );			//!<	
VOID	Vvw_OnPaint( HWND );								//!<	
VOID	Vvw_OnMoving( HWND, LPRECT );						//!<	
BOOL	Vvw_OnWindowPosChanging( HWND, LPWINDOWPOS );		//!<	
VOID	Vvw_OnWindowPosChanged( HWND, const LPWINDOWPOS );	//!<	

HRESULT	VertTextAssemble( HWND );	//!<	
VOID	VertViewDraw( HDC );		//!<	
HRESULT	VertScriptInsert( HWND );	//!<	
//-------------------------------------------------------------------------------------------------


/*!
	�E�C���h�E�N���X�o�^�Ƃ��E�A�v���N���シ���Ă΂��
	@param[in]	ptCurrent	��f�B���N�g���E�g��Ȃ��H
	@param[in]	hInstance	�C���X�^���X�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
INT VertInitialise( LPTSTR ptCurrent, HINSTANCE hInstance )
{
	WNDCLASSEX	wcex;
	HBITMAP		hImg, hMsq;

	if( !(ptCurrent) || !(hInstance) )
	{
		if( ghVertWnd ){	DestroyWindow( ghVertWnd  );	}

		FREE( gptVtBuff );

		return S_OK;
	}


//�c�������䑋
	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= VertProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= VERTSCRIPT_CLASS;
	wcex.hIconSm		= NULL;

	gVertAtom = RegisterClassEx( &wcex );

	ghVertWnd = NULL;

	ZeroMemory( &gstViewOrigin, sizeof(POINT) );


//�����\����
	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= VertViewProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= VERTVIEW_CLASS;
	wcex.hIconSm		= NULL;

	gVertViewAtom = RegisterClassEx( &wcex );

	//	�K���ɍ���Ă���
	gptVtBuff = (LPTSTR)malloc( MAX_PATH * sizeof(TCHAR) );
	ZeroMemory( gptVtBuff, MAX_PATH * sizeof(TCHAR) );
	gcchVtBuf = MAX_PATH;

	//	�A�C�R���@�m��E�X�V�E������E����
	ghVertImgLst = ImageList_Create( 16, 16, ILC_COLOR24 | ILC_MASK, 4, 1 );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMP_MOZI_WRITE ) );	//	�Ώۖ��O����
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMQ_PAGENAMECHANGE ) );
	ImageList_Add( ghVertImgLst, hImg, hMsq );	//	�m��
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMP_REFRESH ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMQ_REFRESH ) );
	ImageList_Add( ghVertImgLst, hImg, hMsq );	//	�X�V
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMP_VERT_LEFT ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMQ_VERT_LEFT ) );
	ImageList_Add( ghVertImgLst, hImg, hMsq );	//	������
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMP_VERT_TRANS ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMQ_VERT_TRANS ) );
	ImageList_Add( ghVertImgLst, hImg, hMsq );	//	����
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );


	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�c�����X�N���v�g�E�C���h�E���
	@param[in]	hInst	�C���X�^���X�n���h��
	@param[in]	hPrWnd	���C���̃E�C���h�E�n���h��
*/
HWND VertScripterCreate( HINSTANCE hInst, HWND hPrWnd )
{
	LONG	x, y;
	HWND	hDktpWnd;
	UINT	height;
	TCHAR	atBuffer[MAX_STRING];
	RECT	rect, vwRect, dtRect;

	hDktpWnd = GetDesktopWindow(  );
	GetWindowRect( hDktpWnd, &dtRect );

	GetWindowRect( hPrWnd, &rect );
	x = dtRect.right - rect.right;
	if( VT_WIDTH >  x ){	rect.right = dtRect.right - VT_WIDTH;	}

	if( ghVertWnd )
	{
		SetForegroundWindow( ghVertViewWnd );
		SetWindowPos( ghVertWnd, HWND_TOP, rect.right, rect.top, 0, 0, SWP_NOSIZE );
		SetForegroundWindow( ghVertWnd );


		return ghVertWnd;
	}

	gbLeftGo = 0;

	gbSpTrans = 0;

	gbQuickClose = 1;	//	������ԂŒ�������

	//	�{�̃E�C���h�E
	ghVertWnd = CreateWindowEx( WS_EX_TOOLWINDOW | WS_EX_TOPMOST, VERTSCRIPT_CLASS,
		TEXT("�c����"), WS_POPUP | WS_CAPTION | WS_SYSMENU,
		rect.right, rect.top, VT_WIDTH, VT_HEIGHT, NULL, NULL, hInst, NULL );

	//	�c�[���o�[
	ghVertToolBar = CreateWindowEx( WS_EX_CLIENTEDGE, TOOLBARCLASSNAME, TEXT("verttoolbar"), WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TOOLTIPS, 0, 0, 0, 0, ghVertWnd, (HMENU)IDTB_VLINE_TOOLBAR, hInst, NULL );

	if( 0 == gdToolBarHei )	//	���l���擾�Ȃ�
	{
		GetWindowRect( ghVertToolBar, &rect );
		gdToolBarHei = rect.bottom - rect.top;
	}

	//	�����c�[���`�b�v�X�^�C����ǉ�
	SendMessage( ghVertToolBar, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS );

	SendMessage( ghVertToolBar, TB_SETIMAGELIST, 0, (LPARAM)ghVertImgLst );
	SendMessage( ghVertToolBar, TB_SETBUTTONSIZE, 0, MAKELPARAM(16,16) );

	SendMessage( ghVertToolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0 );

	//	�c�[���`�b�v�������ݒ�E�{�^���e�L�X�g���c�[���`�b�v�ɂȂ�
	StringCchCopy( atBuffer, MAX_STRING, TEXT("������}��") );	gstVttbInfo[0].iString = SendMessage( ghVertToolBar, TB_ADDSTRING, 0, (LPARAM)atBuffer );
	StringCchCopy( atBuffer, MAX_STRING, TEXT("������X�V") );	gstVttbInfo[2].iString = SendMessage( ghVertToolBar, TB_ADDSTRING, 0, (LPARAM)atBuffer );
	StringCchCopy( atBuffer, MAX_STRING, TEXT("������z�u") );	gstVttbInfo[3].iString = SendMessage( ghVertToolBar, TB_ADDSTRING, 0, (LPARAM)atBuffer );
	StringCchCopy( atBuffer, MAX_STRING, TEXT("�󔒂𓧉�") );	gstVttbInfo[4].iString = SendMessage( ghVertToolBar, TB_ADDSTRING, 0, (LPARAM)atBuffer );

	SendMessage( ghVertToolBar , TB_ADDBUTTONS, (WPARAM)TB_ITEMS, (LPARAM)&gstVttbInfo );	//	�c�[���o�[�Ƀ{�^����}��

	SendMessage( ghVertToolBar , TB_AUTOSIZE, 0, 0 );	//	�{�^���̃T�C�Y�ɍ��킹�ăc�[���o�[�����T�C�Y
	InvalidateRect( ghVertToolBar , NULL, TRUE );		//	�N���C�A���g�S�̂��ĕ`�悷�閽��

	GetClientRect( ghVertWnd, &rect );

	//	������STATIC
	CreateWindowEx( 0, WC_STATIC, TEXT("�s��"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | SS_RIGHT, 2, gdToolBarHei, 45, VT_PARAMHEI, ghVertWnd, (HMENU)IDS_VLINE_INTERVAL, hInst, NULL );
	//	������EDIT
	gdVertInterval = 22;
	CreateWindowEx( 0, WC_EDIT, TEXT("22"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 49, gdToolBarHei, 50, VT_PARAMHEI, ghVertWnd, (HMENU)IDE_VLINE_INTERVAL, hInst, NULL );
	//	������SPIN
	CreateWindowEx( 0, UPDOWN_CLASS, TEXT("intervalspin"), WS_CHILD | WS_VISIBLE | UDS_AUTOBUDDY, 99, gdToolBarHei, 10, VT_PARAMHEI, ghVertWnd, (HMENU)IDUD_VLINE_INTERVAL, hInst, NULL );

	CreateWindowEx( 0, WC_BUTTON, TEXT("�m�肵�������"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 120, gdToolBarHei, 180, VT_PARAMHEI, ghVertWnd, (HMENU)IDCB_VLINE_QUICKCLOSE, hInst, NULL );
	CheckDlgButton( ghVertWnd, IDCB_VLINE_QUICKCLOSE, gbQuickClose ? BST_CHECKED : BST_UNCHECKED );

	height = gdToolBarHei + VT_PARAMHEI;

	//��������͘g
	ghTextWnd = CreateWindowEx( 0, WC_EDIT, TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE,
		0, height, rect.right, rect.bottom - height, ghVertWnd, (HMENU)IDE_VLINE_TEXT, hInst, NULL );
	SetWindowFont( ghTextWnd, ghAaFont, TRUE );

	//	�T�u�N���X
	gpfOrigVertEditProc = SubclassWindow( ghTextWnd, gpfVertEditProc );


	ShowWindow( ghVertWnd, SW_SHOW );
	UpdateWindow( ghVertWnd );


//�\���E�ʒu���ߔ������t���[�e�B���O�E�C���h�[
	ghVertViewWnd = CreateWindowEx( WS_EX_TOOLWINDOW | WS_EX_LAYERED, VERTVIEW_CLASS,
		TEXT("�z�u"), WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_VISIBLE, 0, 0, 160, 120, NULL, NULL, hInst, NULL);
	SetLayeredWindowAttributes( ghVertViewWnd, 0, gbAlpha, LWA_ALPHA );
	//	�����x�̓��C���{�b�N�X�̐ݒ���g��

	ZeroMemory( &gstFrmSz, sizeof(POINT) );
	ClientToScreen( ghVertViewWnd, &gstFrmSz );

	//	�E�C���h�E�ʒu���m�肳����
	GetWindowRect( ghViewWnd, &vwRect );	//	�ҏW��
	gstViewOrigin.x = vwRect.left;	//	�r���[�E�C���h�E�̈ʒu�L�^
	gstViewOrigin.y = vwRect.top;
	x = (vwRect.left + LINENUM_WID) - gstFrmSz.x;
	y = (vwRect.top  + RULER_AREA)  - gstFrmSz.y;
	TRACE( TEXT("VERT %d x %d"), x, y );
	//	���̎��_��0dot,0line�̈ʒu�ɃN���C�����g���オ�A�b�[�I

#ifdef _DEBUG
	SetWindowPos( ghVertViewWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW );
#else
	SetWindowPos( ghVertViewWnd, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW );
#endif
	gstOffset.x = x - vwRect.left;
	gstOffset.y = y - vwRect.top;


	return ghVertWnd;
}
//-------------------------------------------------------------------------------------------------


/*!
	�G�f�B�b�g�{�b�N�X�T�u�N���X
	@param[in]	hWnd	�E�C���h�E�̃n���h��
	@param[in]	msg		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@return	�����������ʂƂ�
*/
LRESULT CALLBACK gpfVertEditProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	INT		len;
	INT		id;
	HWND	hWndCtl;
	UINT	codeNotify;

	switch( msg )
	{
		default:	break;

		case WM_COMMAND:
			id         = LOWORD(wParam);	//	���������R�}���h�̎��ʎq
			hWndCtl    = (HWND)lParam;		//	�R�}���h�𔭐��������q�E�C���h�E�̃n���h��
			codeNotify = HIWORD(wParam);	//	�ǉ��̒ʒm���b�Z�[�W
			TRACE( TEXT("[%X]VertEdit COMMAND %d"), hWnd, id );
			
			switch( id )	//	�L�[�{�[�h�V���[�g�J�b�g���u�b�Ƃ΂�
			{
				case IDM_PASTE:	SendMessage( hWnd, WM_PASTE, 0, 0 );	return 0;
				case IDM_COPY:	SendMessage( hWnd, WM_COPY,  0, 0 );	return 0;
				case IDM_CUT:	SendMessage( hWnd, WM_CUT,   0, 0 );	return 0;
				case IDM_UNDO:	SendMessage( hWnd, WM_UNDO,  0, 0 );	return 0;
				case IDM_ALLSEL:
					len = GetWindowTextLength( hWnd );
					SendMessage( hWnd, EM_SETSEL, 0, len );
					break;
				default:	break;
			}

			break;
	}

	return CallWindowProc( gpfOrigVertEditProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------


/*!
	�E�C���h�E�v���V�[�W��
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�����ς�
	@retval no0	�����ł͏����������ɉ�
*/
LRESULT CALLBACK VertProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		HANDLE_MSG( hWnd, WM_PAINT,   Vrt_OnPaint );	//	��ʂ̍X�V�Ƃ�
		HANDLE_MSG( hWnd, WM_NOTIFY,  Vrt_OnNotify );	//	�R�����R���g���[���̌ʃC�x���g
		HANDLE_MSG( hWnd, WM_COMMAND, Vrt_OnCommand );	
		HANDLE_MSG( hWnd, WM_DESTROY, Vrt_OnDestroy );	//	�I�����̏���

		default:	break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------


/*!
	COMMAND���b�Z�[�W�̎󂯎��B�{�^�������ꂽ�Ƃ��Ŕ���
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	id			���b�Z�[�W�𔭐��������q�E�C���h�E�̎��ʎq	LOWORD(wParam)
	@param[in]	hWndCtl		���b�Z�[�W�𔭐��������q�E�C���h�E�̃n���h��	lParam
	@param[in]	codeNotify	�ʒm���b�Z�[�W	HIWORD(wParam)
	@return		�Ȃ�
*/
VOID Vrt_OnCommand( HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify )
{
	LRESULT	lRslt;

	switch( id )
	{
		case  IDM_VLINE_DECIDE:	//	�m��
			VertScriptInsert( hWnd );
			if( gbQuickClose  ){	DestroyWindow( hWnd );	}	//	��������H
			break;

		case  IDM_VLINE_REFRESH:	VertTextAssemble( hWnd );	break;

		case  IDE_VLINE_TEXT:	//	�������͘g�E�������^�C���Ńr���[������
			if( EN_UPDATE == codeNotify ){	VertTextAssemble( hWnd );	}
			break;

		case  IDCB_VLINE_LEFT_GO:	//	������
			lRslt = SendMessage( ghVertToolBar, TB_ISBUTTONCHECKED, IDCB_VLINE_LEFT_GO, 0 );
			if( lRslt ){	gbLeftGo = 1;	}	//	������
			else{	gbLeftGo = 0;	}	//	�`�F�L��Ԃ��m�F���ď�������
			VertTextAssemble( hWnd );
			break;

		case  IDM_VLINE_TRANSPARENT:	//	�󔒓���
			lRslt = SendMessage( ghVertToolBar, TB_ISBUTTONCHECKED, IDM_VLINE_TRANSPARENT, 0 );
			if( lRslt ){	gbSpTrans = 1;	}	//	���߂���
			else{	gbSpTrans = 0;	}	//	���߂��Ȃ�
			break;

		case  IDCB_VLINE_QUICKCLOSE:	//	��������
			gbQuickClose = IsDlgButtonChecked( hWnd, IDCB_VLINE_QUICKCLOSE ) ? TRUE : FALSE;
			break;

		case  IDM_PASTE:	TRACE( TEXT("VT PASTE") );	SendMessage( ghTextWnd, WM_PASTE, 0, 0 );	return;
		case  IDM_COPY:		SendMessage( ghTextWnd, WM_COPY,  0, 0 );	return;
		case  IDM_CUT:		SendMessage( ghTextWnd, WM_CUT,   0, 0 );	return;

		default:	return;
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	PAINT�B�����̈悪�o�����Ƃ��ɔ����B�w�i�̈����ɒ��ӁB�w�i��h��Ԃ��Ă���A�I�u�W�F�N�g��`��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
*/
VOID Vrt_OnPaint( HWND hWnd )
{
	PAINTSTRUCT	ps;
	HDC			hdc;

	hdc = BeginPaint( hWnd, &ps );

	EndPaint( hWnd, &ps );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�����Ƃ��ɔ����B�f�o�C�X�R���e�L�X�g�Ƃ��m�ۂ�����ʍ\���̃������Ƃ����I���B
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Vrt_OnDestroy( HWND hWnd )
{
	MainStatusBarSetText( SB_LAYER, TEXT("") );

	if( ghVertViewWnd ){	DestroyWindow( ghVertViewWnd  );	}

	ghVertWnd = NULL;

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�m�[�e�B�t�@�C���b�Z�[�W�̏���
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	idFrom		NOTIFY�𔭐��������R���g���[���̂h�c
	@param[in]	pstNmhdr	NOTIFY�̏ڍ�
	@return		�����������e�Ƃ�
*/
LRESULT Vrt_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	LPNMUPDOWN	pstNmUpDown;

	if( IDUD_VLINE_INTERVAL == idFrom )
	{
		pstNmUpDown = (LPNMUPDOWN)pstNmhdr;

		if( UDN_DELTAPOS == pstNmUpDown->hdr.code )
		{
			//	iDelta�@��Ł|�P�A���łP
			TRACE( TEXT("UPDOWN %d"), pstNmUpDown->iDelta );
			gdVertInterval -= (pstNmUpDown->iDelta);
			SetDlgItemInt( hWnd, IDE_VLINE_INTERVAL, gdVertInterval, TRUE );
			VertTextAssemble( hWnd );
			InvalidateRect( ghVertViewWnd, NULL, TRUE );	//	�������^�C���H
		}
	}

	return 0;	//	�����Ȃ��Ȃ�O��߂�
}
//-------------------------------------------------------------------------------------------------

/*!
	�e�L�X�g�{�b�N�X�̓��e���X�N���v�g����
*/
HRESULT VertTextAssemble( HWND hWnd )
{
	UINT_PTR	cchSz;
	INT			ileng, i, iRow, iClm, iLine, iMaxRow;
	INT			iMaxY, iMaxX, iViewXdot, iViewYdot, iYline, cx, cy;	//	�z�u���̃T�C�Y�ύX�Ɏg�p����
	INT			iLnDot, iLnCnt, bkLine;
	UINT_PTR	iMozis;
	HWND		hWorkWnd;
	LPTSTR		ptScript;
	RECT		rect;
	VERTITEM	stVtitm;

	VTIM_ITR	itVtitm;


	hWorkWnd = GetDlgItem( hWnd, IDE_VLINE_TEXT );

	//	�������m�F���ăo�b�t�@�쐬
	ileng = Edit_GetTextLength( hWorkWnd );
	cchSz = ileng + 2;

	if( gcchVtBuf <  cchSz )	//	�e�ʑ���Ȃ��Ȃ�g������
	{
		ptScript = (LPTSTR)realloc( gptVtBuff, cchSz * sizeof(TCHAR) );
		gptVtBuff = ptScript;
		gcchVtBuf = cchSz;
	}

	ZeroMemory( gptVtBuff, gcchVtBuf * sizeof(TCHAR) );
	Edit_GetText( hWorkWnd, gptVtBuff, cchSz );

	//	��̃f�[�^�j��
	gvcVertItem.clear( );	//	���I�������͖����̂ŕ��ʂɃN�����[�ł���

	//	���g�����Ȃ炱���ŏI���
	if( 0 >= ileng )	return S_FALSE;

	//	�땶�����o�����Ď捞
	iRow  = 0;
	iClm  = 0;
	iLine = 1;	//	�s��
	iMaxRow = 0;	//	��s�̍ő啶����
	for( i = 0; ileng > i; i++ )
	{
		ZeroMemory( &stVtitm, sizeof(VERTITEM) );

		if( TEXT('\r') ==  gptVtBuff[i] )	//	���s
		{
			//	�ő啶�����m�F
			if( iMaxRow < iRow )	iMaxRow = iRow;

			i++;
			iLine++;
			iClm++;
			iRow = 0;
		}
		else
		{
			stVtitm.cch      = gptVtBuff[i];	//	�����m��
			stVtitm.iWidth   = ViewLetterWidthGet( stVtitm.cch );	//	������
			stVtitm.iOffset  = LEFT_PADD;	//	�ō�����̉E�I�t�Z�b�g�h�b�g
			stVtitm.iRow     = iRow++;	//	�c�ɂ������̂x�ʒu
			stVtitm.iColumn  = iClm;	//	�c�ɂ������̕�����ԍ��E���s�ɒ���

			gvcVertItem.push_back( stVtitm );
		}
	}
	//	�ő啶�����m�F
	if( iMaxRow < iRow )	iMaxRow = iRow;

	iMaxY = (iMaxRow+1) * LINE_HEIGHT;	//	�c�E�]�T����Ƃ�
	iMaxX  = LEFT_PADD + (gdVertInterval * iLine );	//	�����]�T

	//	�܂��ǂݍ���œ��e���m�F�E�ʒu�����͂��̌�
	if( gbLeftGo ){	iLnCnt = 0;	}
	else{	iLnCnt = iClm;	}
	//	�E���炩�����炩
	iLnDot = LEFT_PADD + (gdVertInterval * iLnCnt );	//	��ʒu�ɒ��ӁE�s�͂O�C���f�b�N�X��
	bkLine = 0;
	iMozis = gvcVertItem.size( );

	for( itVtitm = gvcVertItem.begin(); itVtitm != gvcVertItem.end(); itVtitm++ )	//	�땶�����m�F���Ă���
	{
		if( bkLine != itVtitm->iColumn )	//	���s�͂�����
		{
			if( gbLeftGo ){	iLnCnt++;	}
			else{	iLnCnt--;	}	//	�s���ڂ�
			if( 0 > iLnCnt ){	iLnCnt =  0;	}

			iLnDot = LEFT_PADD + (gdVertInterval * iLnCnt );	//	�h�b�g�ʒu�C��
		}
		bkLine = itVtitm->iColumn;

		//	�������S�Ȃ̂ŁA���̔����̈ʒu�����J�n�ʒu
		itVtitm->iOffset = iLnDot - ((itVtitm->iWidth+1) / 2);	//	�l�̌ܓ�
		//	��Ǔ_�̃o���C�͓��ʂȈʒu�ɂ��Ă���
		if( IDEO_COMMA == itVtitm->cch || IDEO_FSTOP == itVtitm->cch )
		{
			itVtitm->iOffset = iLnDot - 3;	//	�Œ�l����
		}
		//	�͂ݏo���ɒ���
		if( 0 >  itVtitm->iOffset ){	itVtitm->iOffset = 0;	}
	}

	//�z�u���傫������
	//	���̉�ʂ̍s���ƃh�b�g���m�F
	iYline = ViewAreaSizeGet( &iViewXdot );
	iViewYdot = iYline * LINE_HEIGHT;

	GetWindowRect( ghVertViewWnd, &rect );
	cx = rect.right  - rect.left;
	cy = rect.bottom - rect.top;
	GetClientRect( ghVertViewWnd, &rect );
	cx -= rect.right;
	cy -= rect.bottom;

	//	�����E�C���h�E�T�C�Y�ɂȂ�͂�
	cx += iMaxX;
	cy += iMaxY;

	//	�₽��f�J���Ȃ玩�d
	if( iViewXdot < cx ){	cx =  iViewXdot;	}
	if( iViewYdot < cy ){	cy =  iViewYdot;	}
	//	�������Ă����d�E�ŏ��T�C�Y�͓K��
	if( 66 > cx ){	cx = 66;	}
	if( 66 > cy ){	cy = 66;	}


#ifdef _DEBUG
	SetWindowPos( ghVertViewWnd, HWND_TOP, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );
#else
	SetWindowPos( ghVertViewWnd, HWND_TOPMOST, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );
#endif

	InvalidateRect( ghVertViewWnd, NULL, TRUE );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�z�u���̃E�C���h�E�v���V�[�W��
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�����ς�
	@retval no0	�����ł͏����������ɉ�
*/
LRESULT CALLBACK VertViewProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		HANDLE_MSG( hWnd, WM_KEYDOWN,			Vvw_OnKey );
		HANDLE_MSG( hWnd, WM_PAINT,				Vvw_OnPaint );		//	��ʂ̍X�V�Ƃ�
		HANDLE_MSG( hWnd, WM_WINDOWPOSCHANGING,	Vvw_OnWindowPosChanging );
		HANDLE_MSG( hWnd, WM_WINDOWPOSCHANGED,	Vvw_OnWindowPosChanged );

		case  WM_MOVING:	Vvw_OnMoving( hWnd, (LPRECT)lParam );	return 0;

		default:	break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	��������Ă���Ƃ��ɔ����E�}�E�X�ŃE�C���h�E�h���b�O���Ƃ�
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	pstPos	���̏u�Ԃ̃X�N���[�����W
*/
VOID Vvw_OnMoving( HWND hWnd, LPRECT pstPos )
{
	LONG	xEt, yEt, xLy, yLy, xSb, ySb;
	LONG	dLine, dRema;
	BOOLEAN	bMinus = FALSE;
	TCHAR	atBuffer[SUB_STRING];

	//	�t���[�����̍���X�N���[�����W
	xLy = pstPos->left + gstFrmSz.x;
	yLy = pstPos->top  + gstFrmSz.y;

	//	�r���[�̍���e�L�X�g�G���A�ʒu
	xEt = (gstViewOrigin.x + LINENUM_WID);
	yEt = (gstViewOrigin.y + RULER_AREA);
//	TRACE( TEXT("%d x %d"), xEt, yEt );

	//	�I�t�Z�b�g��
	xSb = xLy - xEt;	//	�w�͂��̂܂܃h�b�g��
	ySb = yLy - yEt;	//	�x���h�b�g�Ȃ̂ōs���ɂ��Ȃ��Ƃ����Ȃ�

	if( 0 > ySb ){	ySb *= -1;	bMinus = TRUE;	}	//	�}�C�i�X�␳
	//	�s���I�ȃ��m�����߂���Ă΂�
	dLine = ySb / LINE_HEIGHT;
	dRema = ySb % LINE_HEIGHT;
	if( (LINE_HEIGHT/2) < dRema ){	dLine++;	}
	if( bMinus ){	dLine *= -1;	}else{	dLine++;	}

	//	20110704	�����ł́A�܂��ʒu�̓X�N���[���̃Y�����l������ĂȂ�
	xSb   += gdHideXdot;
	dLine += gdViewTopLine;

	StringCchPrintf( atBuffer, SUB_STRING, TEXT("VLINE %d[dot] %d[line]"), xSb, dLine );
	MainStatusBarSetText( SB_LAYER, atBuffer );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�B���h�E�̃T�C�Y�ύX����������O�ɑ����Ă���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	pstWpos	�V�����ʒu�Ƒ傫���������Ă�
	@return		����Message������������O
*/
BOOL Vvw_OnWindowPosChanging( HWND hWnd, LPWINDOWPOS pstWpos )
{
	INT		clPosY, vwTopY, dSabun, dRem;
	BOOLEAN	bMinus = FALSE;
	RECT	vwRect;

	//	�ړ����Ȃ������Ƃ��͉������Ȃ��ł���
	if( SWP_NOMOVE & pstWpos->flags )	return TRUE;

	clPosY = pstWpos->y + gstFrmSz.y;	//	�\���ʒu��TOP

	//	�\����������s�P�ʂɍ��킹��
	GetWindowRect( ghViewWnd, &vwRect );
	gstViewOrigin.x = vwRect.left;//�ʒu�L�^�E���������ς����̂���Ȃ�
	gstViewOrigin.y = vwRect.top;
	vwTopY = (vwRect.top  + RULER_AREA);

	dSabun = vwTopY - clPosY;
	if( 0 > dSabun ){	dSabun *= -1;	bMinus = TRUE;	}

	dRem = dSabun % LINE_HEIGHT;

	if( 0 == dRem ){	return TRUE;	}

	if( (LINE_HEIGHT/2) < dRem ){	dRem = dRem - LINE_HEIGHT;	}

	if( bMinus ){	dRem *=  -1;	}

	pstWpos->y += dRem;

	return FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�B���h�E�̃T�C�Y�ύX�����������瑗���Ă���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	pstWpos	�V�����ʒu�Ƒ傫���������Ă�
*/
VOID Vvw_OnWindowPosChanged( HWND hWnd, const LPWINDOWPOS pstWpos )
{
	RECT	vwRect;

	InvalidateRect( hWnd, NULL, TRUE );

	//	�ړ����Ȃ������Ƃ��͉������Ȃ��ł���
	if( SWP_NOMOVE & pstWpos->flags )	return;

	GetWindowRect( ghViewWnd, &vwRect );
	gstViewOrigin.x = vwRect.left;	//	�ʒu�L�^
	gstViewOrigin.y = vwRect.top;

	gstOffset.x = pstWpos->x - vwRect.left;
	gstOffset.y = pstWpos->y - vwRect.top;

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�L�[�_�E���������E�L�[�{�[�h�ňړ��p
	@param[in]	hWnd	�E�C���h�E�n���h���E�r���[�̂Ƃ͌���Ȃ��̂Œ��ӃZ��
	@param[in]	vk		�����ꂽ�L�[�����z�L�[�R�[�h�ŗ���
	@param[in]	fDown	��O�_�E���@�O�A�b�v
	@param[in]	cRepeat	�A���I�T���񐔁E���ĂȂ��H
	@param[in]	flags	�L�[�t���O���낢��
	@return		����
*/
VOID Vvw_OnKey( HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags )
{
	RECT	rect;

	GetWindowRect( hWnd, &rect );

	if( fDown )
	{
		switch( vk )
		{
			case VK_RIGHT:	TRACE( TEXT("�E") );	rect.left++;	break;
			case VK_LEFT:	TRACE( TEXT("��") );	rect.left--;	break;
			case VK_DOWN:	TRACE( TEXT("��") );	rect.top += LINE_HEIGHT;	break;
			case  VK_UP:	TRACE( TEXT("��") );	rect.top -= LINE_HEIGHT;	break;
			default:	return;
		}
	}

#ifdef _DEBUG
	SetWindowPos( hWnd, HWND_TOP, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
#else
	SetWindowPos( hWnd, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
#endif

	Vvw_OnMoving( hWnd, &rect );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	PAINT�B�����̈悪�o�����Ƃ��ɔ����B�w�i�̈����ɒ��ӁB�w�i��h��Ԃ��Ă���A�I�u�W�F�N�g��`��
	@param[in]	hWnd	�E�C���h�E�n���h��
*/
VOID Vvw_OnPaint( HWND hWnd )
{
	PAINTSTRUCT	ps;
	HDC			hdc;

	hdc = BeginPaint( hWnd, &ps );

	VertViewDraw( hdc );

	//���A���^�C���X�V����ƃt�H�[�J�X�����������Ȃ�H

	EndPaint( hWnd, &ps );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�\���g�̕`�揈��
	@param[in]	hDC	�`�悷��f�o�C�X�R���e�L�X�g
*/
VOID VertViewDraw( HDC hDC )
{
	INT_PTR	iItems;
	INT		x, y;
	TCHAR	atMozi[2];
	HFONT	hFtOld;

	VTIM_ITR	itVtitm;

	hFtOld = SelectFont( hDC, ghAaFont );	//	�t�H���g����������
	SetBkMode( hDC, TRANSPARENT );

	iItems = gvcVertItem.size( );

	atMozi[1] = 0;

	for( itVtitm = gvcVertItem.begin(); itVtitm != gvcVertItem.end(); itVtitm++ )	//	�땶�����m�F���Ă���
	{
		atMozi[0] = itVtitm->cch;
		x = itVtitm->iOffset;
		y = itVtitm->iRow * LINE_HEIGHT;
		ExtTextOut( hDC, x, y, 0, NULL, atMozi, 1, NULL );
	}

	SelectFont( hDC, hFtOld );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	���e����������
	@param[in]	hWnd	�E�C���h�E�n���h���E���܂�Ӗ��͂Ȃ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT	VertScriptInsert( HWND hWnd )
{
	LPTSTR		ptText;
	INT			iTgtRow, iMaxRow, iMaxClm;
	INT			iRitDot, iNeedPadd;
	HWND		hLyrWnd;
	INT			iX, iY;
	RECT		rect;

	wstring		wsBuffer;	//	��������������ꂿ�Ⴄ

	VTIM_ITR	itVtitm;
	VTIM_RITR	itRvsVtitm;

//�E����Ȃ�t�ɂ��ǂ�B������Ȃ琳���BRow����l�ɂ��āA���O�ʒu�Ƃ̃X�L�}�͋󔒖���
//���s�Ƃ�����܂����Ĉ�q����̕�����ɂ��āA�X�e���X���C���œ\��B

	//	�c���̑傫�������߂�
	iMaxRow = 0;	iMaxClm = 0;
	for( itVtitm = gvcVertItem.begin(); itVtitm != gvcVertItem.end(); itVtitm++ )
	{
		if( iMaxRow < itVtitm->iRow )		iMaxRow = itVtitm->iRow;
		if( iMaxClm < itVtitm->iColumn )	iMaxClm = itVtitm->iColumn;
	}

	ptText = NULL;
	wsBuffer.clear();
	for( iTgtRow = 0; iMaxRow >= iTgtRow; iTgtRow++ )	//	�����ʒu���ォ�珇�ԂɌ��Ă���
	{
		iRitDot = 0;

		if( gbLeftGo )	//	������
		{
			for( itVtitm = gvcVertItem.begin(); itVtitm != gvcVertItem.end(); itVtitm++ )
			{
				if( iTgtRow == itVtitm->iRow )	//	����ROW����v������
				{
					iNeedPadd = itVtitm->iOffset - iRitDot;	//	�����܂ł̉E�[����̃h�b�g��
					if( 0 > iNeedPadd ){	iNeedPadd = 0;	}	//	�d�Ȃ����ꍇ�Ƃ����蓾��

					ptText = DocPaddingSpaceMake( iNeedPadd );	//	�O�Ȃ�NULL���Ԃ�
					if( ptText )	//	���ߕ����p������
					{
						wsBuffer += wstring( ptText );
						FREE( ptText );
					}
					wsBuffer += itVtitm->cch;	//	�{���̕���������

					iRitDot += iNeedPadd;	//	�����܂ł̖��ߗʂ�
					iRitDot += itVtitm->iWidth;	//	�g�p���𖄂߂�
				}
			}
		}
		else
		{
			//	�E����
			for( itRvsVtitm = gvcVertItem.rbegin(); itRvsVtitm != gvcVertItem.rend(); itRvsVtitm++ )
			{
				if( iTgtRow == itRvsVtitm->iRow )	//	����ROW����v������
				{
					iNeedPadd = itRvsVtitm->iOffset - iRitDot;	//	�����܂ł̉E�[����̃h�b�g��
					if( 0 > iNeedPadd ){	iNeedPadd = 0;	}	//	�d�Ȃ����ꍇ�Ƃ����蓾��

					ptText = DocPaddingSpaceMake( iNeedPadd );	//	�O�Ȃ�NULL���Ԃ�
					if( ptText )	//	���ߕ����p������
					{
						wsBuffer += wstring( ptText );
						FREE( ptText );
					}
					wsBuffer += itRvsVtitm->cch;	//	�{���̕���������

					iRitDot += iNeedPadd;	//	�����܂ł̖��ߗʂ�
					iRitDot += itRvsVtitm->iWidth;	//	�g�p���𖄂߂�
				}
			}
		}

		wsBuffer += wstring( CH_CRLFW );	//	��s�I���������s
	}

	//	�}�������ɂ́A���C���{�b�N�X���\�������Ŏg��
	hLyrWnd = LayerBoxVisibalise( GetModuleHandle(NULL), wsBuffer.c_str(), 0x10 );
	//	���C���̈ʒu��ύX
	GetWindowRect( ghVertViewWnd, &rect );
	LayerBoxPositionChange( hLyrWnd, (rect.left + gstFrmSz.x), (rect.top + gstFrmSz.y) );
	//	�ݒ�ɂ��΁A�󔒂�S�����ߎw��ɂ���
	if( gbSpTrans ){	LayerTransparentToggle( hLyrWnd, 1 );	}
	//	�㏑������
	LayerContentsImportable( hLyrWnd, IDM_LYB_OVERRIDE, &iX, &iY, D_INVISI );
	ViewPosResetCaret( iX, iY );	
	//	�I����������
	DestroyWindow( hLyrWnd );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�r���[���ړ�����
	@param[in]	hWnd	�{�̃E�C���h�E�n���h���E���܂�Ӗ��͂Ȃ�
	@param[in]	state	����ԁE�ŏ����Ȃ�Ⴄ�R�g����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT VertMoveFromView( HWND hWnd, UINT state )
{
	RECT	vwRect = {0,0,0,0};
	POINT	lyPoint;

	if( !(ghVertViewWnd) )	return S_FALSE;

	//	�ŏ������͔�\���ɂ���Ƃ�	SIZE_MINIMIZED

	if( SIZE_MINIMIZED != state )
	{
		GetWindowRect( ghViewWnd, &vwRect );
		gstViewOrigin.x = vwRect.left;//�ʒu�L�^
		gstViewOrigin.y = vwRect.top;
	}

	if( SIZE_MINIMIZED == state )
	{
		ShowWindow( ghVertViewWnd, SW_HIDE );
	}
	else
	{
		lyPoint.x = gstOffset.x + vwRect.left;
		lyPoint.y = gstOffset.y + vwRect.top;
#ifdef _DEBUG
		SetWindowPos( ghVertViewWnd, HWND_TOP, lyPoint.x, lyPoint.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW );
#else
		SetWindowPos( ghVertViewWnd, HWND_TOPMOST, lyPoint.x, lyPoint.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW );
#endif
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


