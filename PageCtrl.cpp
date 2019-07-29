/*! @file
	@brief �t�@�C���̃y�[�W�P�ʂ��Ǘ����܂�
	���̃t�@�C���� PageCtrl.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/05/20
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

//	TODO:	�e�ł̏ڍׂ�\���o����悤�ɂ���
//	TODO:	�I��ł̂ݕۑ��Ƃ��A�Ȃ񂩂���ȋ@�\�ق���

#define PAGELIST_CLASS	TEXT("PAGE_LIST")
#define PL_WIDTH	110
#define PL_HEIGHT	300
//-------------------------------------------------------------------------------------------------

//	�z�C�z�C���L���Ă����̂��炤��

extern FILES_ITR	gitFileIt;	//	�����Ă�t�@�C���̖{��

extern INT		gixFocusPage;	//	���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X
extern INT		gixDropPage;	//	�����z�b�g�ԍ�
//-------------------------------------------------------------------------------------------------

static HINSTANCE	ghInst;		//!<	���̃A�v���̎����l

static  ATOM	gPageAtom;		//!<	�N���X�A�g��
static  HWND	ghPageWnd;		//!<	���̃E�C���h�E�̃n���h��

static  HWND	ghToolWnd;		//!<	�c�[���o�[
static  HWND	ghPageListWnd;	//!<	�y�[�W���X�g�r���[�n���h��

#ifdef PGL_TOOLTIP
static  HWND	ghPageTipWnd;	//!<	�c�[���`�b�v�n���h��
static HFONT	ghPgTipFont;	//!<	�c�[���`�b�v�p
static LPTSTR	gptPgTipBuf;	//!<	
#endif
static BOOLEAN	gbPgTipView;	//!<	�Ńc�[���e�B�b�v�\��ON/OFF

static INT		gixPreviSel;	//!<	���O�܂őI�����Ă���

static INT		gixMouseSel;	//!<	�}�E�X�J�[�\�����̃A��
#ifdef PGL_TOOLTIP
static INT	gixPreSel;		//!<	�}�E�X�J�[�\�����̃A��
#endif

static BOOLEAN	gbPgRetFocus;	//!<	�ł�I��������ҏW���Ƀt�H�[�J�X�߂���

static WNDPROC	gpfOrigPageViewProc;	//!<	�ňꗗ�r���[�̌��v���V�[�W��
static WNDPROC	gpfOrigPageToolProc;	//!<	�c�[���o�[�̌��v���V�[�W��

static HIMAGELIST	ghPgLstImgLst;	//!<	

extern INT	gbTmpltDock;		//	�ŁE��s�e���v���̃h�b�L���O
extern BOOLEAN	gbDockTmplView;	//	�������Ă�e���v���͌����Ă��邩

extern  UINT	gdPageByteMax;	//	�냌�X�̍ő�o�C�g��

//extern  HWND	ghMainSplitWnd;	//	���C���̃X�v���b�g�o�[�n���h��
extern  LONG	grdSplitPos;	//	�X�v���b�g�o�[�́A�����́A��ʉE����̃I�t�Z�b�g
//-------------------------------------------------------------------------------------------------

//	�c�[���o�[�E�V�K�쐬�Ƃ�
#define PGTB_ITEMS	9
static TBBUTTON gstPgTlBarInfo[] = {
/*�V�K*/{  0,	IDM_PAGEL_ADD,		TBSTATE_ENABLED | TBSTATE_WRAP,	BTNS_BUTTON | BTNS_AUTOSIZE,	{0, 0}, 0, 0  },	//	
/*�}��*/{  1,	IDM_PAGEL_INSERT,	TBSTATE_ENABLED | TBSTATE_WRAP,	BTNS_BUTTON | BTNS_AUTOSIZE,	{0, 0}, 0, 0  },	//	
/*����*/{  2,	IDM_PAGEL_DUPLICATE,TBSTATE_ENABLED | TBSTATE_WRAP,	BTNS_BUTTON | BTNS_AUTOSIZE,	{0, 0}, 0, 0  },	//	
/*�폜*/{  3,	IDM_PAGEL_DELETE,	TBSTATE_ENABLED | TBSTATE_WRAP,	BTNS_BUTTON | BTNS_AUTOSIZE,	{0, 0}, 0, 0  },	//	
/*����*/{  4,	IDM_PAGEL_COMBINE,	TBSTATE_ENABLED | TBSTATE_WRAP,	BTNS_BUTTON | BTNS_AUTOSIZE,	{0, 0}, 0, 0  },	//	
/*���*/{  5,	IDM_PAGEL_UPFLOW,	TBSTATE_ENABLED | TBSTATE_WRAP,	BTNS_BUTTON | BTNS_AUTOSIZE,	{0, 0}, 0, 0  },	//	
/*����*/{  6,	IDM_PAGEL_DOWNSINK,	TBSTATE_ENABLED | TBSTATE_WRAP,	BTNS_BUTTON | BTNS_AUTOSIZE,	{0, 0}, 0, 0  },	//	
/*����*/{  7,	IDM_PAGEL_RENAME,	TBSTATE_ENABLED | TBSTATE_WRAP,	BTNS_BUTTON | BTNS_AUTOSIZE,	{0, 0}, 0, 0  },	//	
/*�X�V*/{  8,	IDM_PAGEL_DETAIL,	TBSTATE_WRAP,					BTNS_BUTTON | BTNS_AUTOSIZE,	{0, 0}, 0, 0  } 	//	
};	//	���e�ύX������A�c�[���o�[������̐ݒ�Ƃ����ύX�Z��

//-------------------------------------------------------------------------------------------------


LRESULT	CALLBACK PageListProc( HWND, UINT, WPARAM, LPARAM );
VOID	Plt_OnCommand( HWND, INT, HWND, UINT );
VOID	Plt_OnSize( HWND, UINT, INT, INT );
LRESULT	Plt_OnNotify( HWND, INT, LPNMHDR );
VOID	Plt_OnContextMenu( HWND, HWND, UINT, UINT );

LRESULT	PageListNotify( HWND, LPNMLISTVIEW );
HRESULT	PageListNameChange( INT );
HRESULT	PageListSpinning( HWND, INT, INT );
HRESULT	PageListDuplicate( HWND, INT );
HRESULT PageListCombine( HWND, INT );

HRESULT	PageListJump( INT );

LRESULT	CALLBACK gpfPageViewProc( HWND, UINT, WPARAM, LPARAM );
VOID	Plv_OnMouseMove( HWND, INT, INT, UINT );	//!<	
#ifdef PGL_TOOLTIP
LRESULT	Plv_OnNotify( HWND , INT, LPNMHDR );	//!<	
#endif

LRESULT	CALLBACK gpfPageToolProc( HWND, UINT, WPARAM, LPARAM );

INT_PTR	CALLBACK PageNameDlgProc( HWND, UINT, WPARAM, LPARAM );

#ifdef USE_HOVERTIP
LPTSTR	CALLBACK PageListHoverTipInfo( LPVOID );
#endif
//-------------------------------------------------------------------------------------------------


/*!
	�y�[�W�p�肷�Ƃт�`�̍쐬
	@param[in]	hInstance	�A�v���̃C���X�^���X
	@param[in]	hParentWnd	�e�E�C���h�E�̃n���h��
	@param[in]	pstFrame	���C���N���C�����g�̈�
	@return		������r���[�̃E�C���h�E�n���h��
*/
HWND PageListInitialise( HINSTANCE hInstance, HWND hParentWnd, LPRECT pstFrame )
{

	LVCOLUMN	stLvColm;
	RECT		tbRect;
	DWORD		dwExStyle, dwStyle;
	TCHAR		atBuff[MAX_STRING];
	HWND		hPrWnd;

	UINT		ici, resnum;
	HBITMAP		hImg, hMsq;
	INT			spPos;

#ifdef PGL_TOOLTIP
	TTTOOLINFO	stToolInfo;
	LOGFONT	stFont;
#endif
	WNDCLASSEX	wcex;
	RECT	wdRect, clRect, rect;

	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= PageListProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_PGLVPOPUPMENU);
	wcex.lpszClassName	= PAGELIST_CLASS;
	wcex.hIconSm		= NULL;

	gPageAtom = RegisterClassEx( &wcex );

	ghInst = hInstance;

	gixMouseSel = -1;
#ifdef PGL_TOOLTIP
	gixPreSel = -1;
#endif
	gixPreviSel = -1;

	gbPgTipView = InitParamValue( INIT_LOAD, VL_PAGETIP_VIEW, 1 );

	//	�t�H�[�J�X����
	gbPgRetFocus = InitParamValue( INIT_LOAD, VL_PGL_RETFCS, 0 );

	InitWindowPos( INIT_LOAD, WDP_PLIST, &rect );
	if( 0 == rect.right || 0 == rect.bottom )	//	���������O�̓f�[�^����
	{
		GetWindowRect( hParentWnd, &wdRect );
		rect.left   = wdRect.left - PL_WIDTH;
		rect.top    = wdRect.top;
		rect.right  = PL_WIDTH;
		rect.bottom = PL_HEIGHT;
		InitWindowPos( INIT_SAVE, WDP_PLIST, &rect );	//	�N�����ۑ�
	}

	if( gbTmpltDock )	//	���[���E�B���h�[�Ƀh�b�L������
	{
		spPos = grdSplitPos - SPLITBAR_WIDTH;	//	�E����̃I�t�Z�b�g

		hPrWnd    = hParentWnd;
		dwExStyle = 0;
		dwStyle   = WS_CHILD | WS_VISIBLE;
		rect = *pstFrame;	//	�N���C�����g�Ɏg����̈�
		rect.left  = rect.right - spPos;
		rect.right = PLIST_DOCK;
		rect.bottom >>= 1;
	}
	else
	{
		dwExStyle = WS_EX_TOOLWINDOW;
		if( InitWindowTopMost( INIT_LOAD, WDP_PLIST , 0 ) ){	dwExStyle |=  WS_EX_TOPMOST;	}
		dwStyle = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_VISIBLE | WS_SYSMENU;
		hPrWnd = NULL;
	}
	ghPageWnd = CreateWindowEx( dwExStyle, PAGELIST_CLASS, TEXT("Page List"), dwStyle,
		rect.left, rect.top, rect.right, rect.bottom, hPrWnd, NULL, hInstance, NULL);
	//	�E�C���h�E���쐬

	GetClientRect( ghPageWnd, &clRect );

	//	�c�[���o�[�E�c�̂��
	ghToolWnd = CreateWindowEx( 0, TOOLBARCLASSNAME, TEXT("pagetoolbar"),
		WS_CHILD | WS_VISIBLE | CCS_NORESIZE | CCS_LEFT | CCS_NODIVIDER | TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TOOLTIPS,// | TBSTYLE_WRAPABLE,
		0, 0, 0, 0, ghPageWnd, (HMENU)IDTB_PAGE_TOOLBAR, hInstance, NULL);
	//	�����c�[���`�b�v�X�^�C����ǉ�
	SendMessage( ghToolWnd, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS );

	//stToolBmp.hInst = HINST_COMMCTRL;
	//stToolBmp.nID   = IDB_STD_SMALL_COLOR;
	//tbixStd = SendMessage( ghToolWnd, TB_ADDBITMAP, 0, (LPARAM)&stToolBmp );
	ghPgLstImgLst = ImageList_Create( 16, 16, ILC_COLOR24 | ILC_MASK, 9, 1 );
	resnum = IDBMPQ_PAGE_TB_FIRST;
	for( ici = 0; 9 > ici; ici++ )
	{
		hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( (resnum++) ) );
		hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( (resnum++) ) );
		ImageList_Add( ghPgLstImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ�
		DeleteBitmap( hImg );	DeleteBitmap( hMsq );
	}
	SendMessage( ghToolWnd, TB_SETIMAGELIST, 0, (LPARAM)ghPgLstImgLst );

	SendMessage( ghToolWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0 );
	//	�c�[���`�b�v�������ݒ�E�{�^���e�L�X�g���c�[���`�b�v�ɂȂ�
	StringCchCopy( atBuff, MAX_STRING, TEXT("�����ɐV�K�쐬\r\nAlt + Shift + I ") );	gstPgTlBarInfo[ 0].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, TEXT("�I��ł̎��ɑ}��\r\nAlt + I") );			gstPgTlBarInfo[ 1].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, TEXT("�I��ł𕡐�\r\nAlt + C") );				gstPgTlBarInfo[ 2].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, TEXT("�I��ł��폜\r\nAlt + D") );				gstPgTlBarInfo[ 3].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, TEXT("���̕łƓ���\r\nAlt + G") );				gstPgTlBarInfo[ 4].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, TEXT("�ł���ֈړ�\r\nAlt + U") );				gstPgTlBarInfo[ 5].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, TEXT("�ł����ֈړ�\r\nAlt + J") );				gstPgTlBarInfo[ 6].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, TEXT("�Ŗ��̂̕ύX\r\nAlt + N") );				gstPgTlBarInfo[ 7].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, TEXT("�ŐV�̏��ɍX�V") );						gstPgTlBarInfo[ 8].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );

	SendMessage( ghToolWnd, TB_SETROWS, MAKEWPARAM(PGTB_ITEMS,TRUE), (LPARAM)(&tbRect) );

	SendMessage( ghToolWnd, TB_ADDBUTTONS, (WPARAM)PGTB_ITEMS, (LPARAM)&gstPgTlBarInfo );	//	�c�[���o�[�Ƀ{�^����}��

	SendMessage( ghToolWnd, TB_GETITEMRECT, 0, (LPARAM)(&tbRect) );
	MoveWindow( ghToolWnd, 0, 0, tbRect.right, rect.bottom, TRUE );
	InvalidateRect( ghToolWnd , NULL, TRUE );	//	�N���C�����g�S�̂��ĕ`�悷��

//	�T�u�N���X��
	gpfOrigPageToolProc = SubclassWindow( ghToolWnd, gpfPageToolProc );

	tbRect.bottom  = rect.bottom;
	tbRect.left    = 0;
	tbRect.top     = 0;

//���X�g�r���[	LVS_SHOWSELALWAYS
	ghPageListWnd = CreateWindowEx( 0, WC_LISTVIEW, TEXT("pagelist"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | 
#ifdef PAGE_MULTISELECT
		LVS_REPORT | LVS_NOSORTHEADER,
#else
		LVS_REPORT | LVS_NOSORTHEADER | LVS_SINGLESEL,
#endif
		tbRect.right, clRect.top, clRect.right - tbRect.right, clRect.bottom, ghPageWnd,
		(HMENU)IDLV_PAGELISTVIEW, hInstance, NULL );
	ListView_SetExtendedListViewStyle( ghPageListWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	gpfOrigPageViewProc = SubclassWindow( ghPageListWnd, gpfPageViewProc );

	ZeroMemory( &stLvColm, sizeof(LVCOLUMN) );
	stLvColm.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	stLvColm.fmt = LVCFMT_LEFT;
	stLvColm.pszText = TEXT("No");		stLvColm.cx =  28;	stLvColm.iSubItem = 0;	ListView_InsertColumn( ghPageListWnd, 0, &stLvColm );
	stLvColm.pszText = TEXT("��");		stLvColm.cx =  67;	stLvColm.iSubItem = 1;	ListView_InsertColumn( ghPageListWnd, 1, &stLvColm );
	stLvColm.pszText = TEXT("Byte");	stLvColm.cx =  45;	stLvColm.iSubItem = 2;	ListView_InsertColumn( ghPageListWnd, 2, &stLvColm );
	stLvColm.pszText = TEXT("Line");	stLvColm.cx =  45;	stLvColm.iSubItem = 3;	ListView_InsertColumn( ghPageListWnd, 3, &stLvColm );


//�c�[���`�b�v
#ifdef PGL_TOOLTIP
	ghPageTipWnd = CreateWindowEx( WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, TTS_NOPREFIX | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, ghPageWnd, NULL, hInstance, NULL );

	ViewingFontGet( &stFont );
	stFont.lfHeight = FONTSZ_REDUCE;
	ghPgTipFont = CreateFontIndirect( &stFont );
	SetWindowFont( ghPageTipWnd, ghPgTipFont, TRUE );

	//	�c�[���`�b�v���R�[���o�b�N�Ŋ���t��
	ZeroMemory( &stToolInfo, sizeof(TTTOOLINFO) );
	stToolInfo.cbSize   = sizeof(TTTOOLINFO);
	stToolInfo.uFlags   = TTF_SUBCLASS;
	stToolInfo.hinst    = NULL;	//	
	stToolInfo.hwnd     = ghPageListWnd;
	stToolInfo.uId      = IDLV_PAGELISTVIEW;
	GetClientRect( ghPageListWnd, &stToolInfo.rect );
	stToolInfo.lpszText = LPSTR_TEXTCALLBACK;	//	�R�����w�肷��ƃR�[���o�b�N�ɂȂ�
	SendMessage( ghPageTipWnd, TTM_ADDTOOL, 0, (LPARAM)&stToolInfo );
	SendMessage( ghPageTipWnd, TTM_SETMAXTIPWIDTH, 0, 0 );	//	�`�b�v�̕��B�O�ݒ�ł����B���ꂵ�Ƃ��Ȃ��Ɖ��s����Ȃ�

//	�c�[���`�b�v�̃|�b�v�f�B���C�́ATTM_SETDELAYTIME �Œ����o����
//	wParam	TTDT_INITIAL	�\���܂ł̎���
//	lParam	The LOWORD specifies the delay time, in milliseconds. The HIWORD must be zero.
//	SendMessage( ghPageTipWnd, TTM_SETDELAYTIME, TTDT_INITIAL, MAKELPARAM(2222,0) );
	//	���ʖ���
#endif

	ShowWindow( ghPageWnd, SW_SHOW );
	UpdateWindow( ghPageWnd );

	return ghPageWnd;
}
//-------------------------------------------------------------------------------------------------

/*!
	�c�[���o�[�T�u�N���X
	WindowsXP�ŁA�c�[���o�[�̃{�^����Ń}�E�X�̍��{�^�����������܂܉E�{�^���������ƁA
	����ȍ~�̃}�E�X����𐳏�Ɏ󂯕t���Ȃ��Ȃ�B����̑΍�
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	msg		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
*/
LRESULT CALLBACK gpfPageToolProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			if( SendMessage(hWnd, TB_GETHOTITEM, 0, 0) >= 0 ){	ReleaseCapture(   );	}
			return 0;
	}

	return CallWindowProc( gpfOrigPageToolProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------


/*!
	�E�C���h�E�v���V�[�W��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�����ς�
	@retval no0	�����ł͏����������ɉ�
*/
LRESULT CALLBACK PageListProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		HANDLE_MSG( hWnd, WM_COMMAND,     Plt_OnCommand );	
		HANDLE_MSG( hWnd, WM_SIZE,        Plt_OnSize );	
		HANDLE_MSG( hWnd, WM_NOTIFY,      Plt_OnNotify );	//	�R�����R���g���[���̌ʃC�x���g
		HANDLE_MSG( hWnd, WM_CONTEXTMENU, Plt_OnContextMenu );

		case WM_DESTROY:
#ifdef PGL_TOOLTIP
			SetWindowFont( ghPageTipWnd, GetStockFont(DEFAULT_GUI_FONT), FALSE );
			DeleteFont( ghPgTipFont );
			FREE( gptPgTipBuf );
#endif
			ImageList_Destroy( ghPgLstImgLst );
			return 0;

		case WM_CLOSE:	ShowWindow( ghPageWnd, SW_HIDE );	return 0;

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
VOID Plt_OnCommand( HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify )
{
	INT	iPage, iItem, mRslt, iDiff;
	LONG_PTR	rdExStyle;
#ifdef PAGE_MULTISELECT
	INT	iNxItem = 0, iCount, i;
#endif

	switch( id )
	{
		case  IDM_WINDOW_CHANGE:	WindowFocusChange( WND_PAGE,  1 );	return;
		case  IDM_WINDOW_CHG_RVRS:	WindowFocusChange( WND_PAGE, -1 );	return;

		case IDM_PAGEL_ADD:	//	�����V�K�쐬�͂��ł��L��
			iPage = DocPageCreate( -1 );
			PageListInsert( iPage );	//	�y�[�W���X�g�r���[�ɒǉ�
			DocPageChange( iPage );
			DocModifyContent( TRUE );
			DocFileBackup( hWnd );	//	�o�b�N�A�b�v���Ă���
			ViewFocusSet(  );	//	�t�H�[�J�X�߂�
			return;

		case IDM_TOPMOST_TOGGLE:	//	�펞�őS�ʂƒʏ�E�C���h�E�̃g�O��
			rdExStyle = GetWindowLongPtr( hWnd, GWL_EXSTYLE );
			if( WS_EX_TOPMOST & rdExStyle )
			{
				SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
				InitWindowTopMost( INIT_SAVE, WDP_PLIST, 0 );
			}
			else
			{
				SetWindowPos( hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
				InitWindowTopMost( INIT_SAVE, WDP_PLIST, 1 );
			}
			return;

		case IDM_PAGEL_AATIP_TOGGLE:	//	���e�|�b�p�b�v���邩
			gbPgTipView = gbPgTipView ? FALSE : TRUE;
			InitParamValue( INIT_SAVE, VL_PAGETIP_VIEW, gbPgTipView );
			return;

		case IDM_PAGEL_RETURN_FOCUS:	//	�őI��������t�H�[�J�X�߂���
			gbPgRetFocus = gbPgRetFocus ? FALSE : TRUE;
			InitParamValue( INIT_SAVE, VL_PGL_RETFCS, gbPgRetFocus );
			return;


		default:	break;
	}

	//	�I������Ă�����m�ہE�Ȃ񂩃w���ɂȂ��Ă�
//	ListView_GetSelectedCount

	//	�I������Ă鍀�ڂ��m��
	iItem = ListView_GetNextItem( ghPageListWnd, -1, LVNI_ALL | LVNI_SELECTED );
#ifdef PAGE_MULTISELECT
	iCount = ListView_GetItemCount( ghPageListWnd );

	if( 0 <= iItem )	//	�����I������Ă邩�ǂ����m�F
	{
		iNxItem = ListView_GetNextItem( ghPageListWnd, iItem, LVNI_ALL | LVNI_SELECTED );
	}
#endif

	//	�I������Ă郂�m���Ȃ��Ɩ��Ӗ�
	if( 0 >  iItem ){	iItem = gixFocusPage;	}
	//	���I���Ȃ�A���ݕł��I������Ă��郂�m�ƌ��Ȃ�

	//	�����I���Ȃ�A����Page��Current�ɂ��邩

	switch( id )
	{
		case IDM_PAGE_PREV:	//	�O�̕łֈړ�
			iDiff = iItem - 1;
			PageListJump( iDiff );
			return;

		case IDM_PAGE_NEXT:	//	���̕łֈړ�
			iDiff = iItem + 1;
			PageListJump( iDiff );
			return;
		//	20120110	�ňړ�������ύX�L��ɂȂ����Ⴄ�̏C��


		case IDM_PAGEL_INSERT:	//	�C�ӈʒu�V�K�쐬
#ifdef PAGE_MULTISELECT
			if( 0 <= iNxItem ){	return;	}	//	�����I�����Ă��珈�����Ȃ�
#endif
			iPage = DocPageCreate( iItem );
			PageListInsert( iPage );	//	�y�[�W���X�g�r���[�ɒǉ�
			DocPageChange( iPage );
			DocFileBackup( hWnd );	//	�o�b�N�A�b�v���Ă���
			break;

		case IDM_PAGEL_RENAME:	//	�Ŗ��̕ύX
#ifdef PAGE_MULTISELECT
			if( 0 <= iNxItem ){	return;	}	//	�����I�����Ă��珈�����Ȃ�
#endif
			if( FAILED( PageListNameChange( iItem ) ) ){	 return;	}
			break;

		case IDM_PAGEL_DELETE:	//	���̕ł��폜
#ifdef PAGE_MULTISELECT
			if( 0 <= iNxItem )	//	�����I�����Ă�̂Ȃ�A���ł��m�F�����
			{
				mRslt = MessageBox( hWnd, TEXT("�����̕ł��폜���悤�Ƃ��Ă��B\r\n�{���ɍ폜���Ă����̂����H"), TEXT("���ӂ���̊m�F"), MB_YESNO | MB_DEFBUTTON2 );
				if( IDYES == mRslt )
				{
					for( i = 0; iCount > i; i++ )	//	�ꉞ���~�b�g
					{
						iItem = ListView_GetNextItem( ghPageListWnd, -1, LVNI_ALL | LVNI_SELECTED );
						if( 0 > iItem ){	break;	}
						DocPageDelete( iItem , -1 );
					}
				}
			}
			else
			{
#endif
				mRslt = MessageBoxCheckBox( hWnd, ghInst, 2 );	//	�m�F�����
				if( IDYES == mRslt ){	DocPageDelete( iItem , -1 );	}
#ifdef PAGE_MULTISELECT
			}
#endif
			break;

		case IDM_PAGEL_DIVIDE:	//	�����̓r���[�̃��j���[
			break;

		case IDM_PAGEL_COMBINE:	//	����
#ifdef PAGE_MULTISELECT
			if( 0 <= iNxItem ){	return;	}	//	�����I�����Ă��珈�����Ȃ�
#endif
			//	�m�F�����
			mRslt = MessageBoxCheckBox( hWnd, ghInst, 0 );
			if( IDYES == mRslt ){	PageListCombine( hWnd , iItem );	}
			break;

		case IDM_PAGEL_UPFLOW:		//	���ړ�
#ifdef PAGE_MULTISELECT
			if( 0 <= iNxItem ){	return;	}	//	�����I�����Ă��珈�����Ȃ�
#endif
			PageListSpinning( hWnd, iItem, 1 );
			break;

		case IDM_PAGEL_DOWNSINK:	//	���ړ�
#ifdef PAGE_MULTISELECT
			if( 0 <= iNxItem ){	return;	}	//	�����I�����Ă��珈�����Ȃ�
#endif
			PageListSpinning( hWnd, iItem, -1 );
			break;

		case IDM_PAGEL_DUPLICATE:	//	�ŕ���
#ifdef PAGE_MULTISELECT
			if( 0 <= iNxItem ){	return;	}	//	�����I�����Ă��珈�����Ȃ�
#endif
			PageListDuplicate( hWnd, iItem );
			break;

		case IDM_PAGEL_DETAIL:
			TRACE( TEXT("�ŐV�̏��ɍX�V") );
			return;

		default:	TRACE( TEXT("������[%d]"), id );	return;
	}

	//	�X�V�ɂȂ��͂������ɖ߂�
	DocModifyContent( TRUE );

	ViewFocusSet(  );	//	�t�H�[�J�X�߂�

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�T�C�Y�ύX���ꂽ
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	state	�Ȃɂ��̏��
	@param[in]	cx		�ύX���ꂽ�N���C�����g��
	@param[in]	cy		�ύX���ꂽ�N���C�����g����
*/
VOID Plt_OnSize( HWND hWnd, UINT state, INT cx, INT cy )
{
	RECT	tbRect;
#ifdef PGL_TOOLTIP
	TTTOOLINFO	stToolInfo;
#endif

	tbRect.right = 0;
	if( ghToolWnd )
	{
		SendMessage( ghToolWnd, TB_GETITEMRECT, 0, (LPARAM)(&tbRect) );
		MoveWindow( ghToolWnd, 0, 0, tbRect.right, cy, TRUE );
	}

	MoveWindow( ghPageListWnd, tbRect.right, 0, cx - tbRect.right, cy, TRUE );

#ifdef PGL_TOOLTIP
	//	�K�v�ȏ����������΂���
	ZeroMemory( &stToolInfo, sizeof(TTTOOLINFO) );
	stToolInfo.cbSize = sizeof(TTTOOLINFO);
	stToolInfo.hwnd   = ghPageListWnd;
	stToolInfo.uId    = IDLV_PAGELISTVIEW;
	GetClientRect( ghPageListWnd, &stToolInfo.rect );
	SendMessage( ghPageTipWnd, TTM_NEWTOOLRECT, 0, (LPARAM)&stToolInfo );
#endif
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
LRESULT Plt_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	//	�y�[�W���X�g�r���[
	if( IDLV_PAGELISTVIEW == idFrom ){	return PageListNotify( hWnd, (LPNMLISTVIEW)pstNmhdr );	}

	return 0;	//	�����Ȃ��Ȃ�O��߂�
}
//-------------------------------------------------------------------------------------------------

/*!
	�R���e�L�X�g���j���[�Ăт����A�N�V����(�v�͉E�N���b�N�j
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	hWndContext	�R���e�L�X�g�����������E�C���h�E�̃n���h��
	@param[in]	xPos		�X�N���[���w���W
	@param[in]	yPos		�X�N���[���x����
	@return		����
*/
VOID Plt_OnContextMenu( HWND hWnd, HWND hWndContext, UINT xPos, UINT yPos )
{
	HMENU	hMenu, hSubMenu;
	UINT	dRslt;
	INT		iCount, iItem;
	BOOLEAN	bSel;
	LONG_PTR	rdExStyle;

	POINT	stPoint;

	stPoint.x = (SHORT)xPos;	//	��ʍ��W�̓}�C�i�X�����肤��
	stPoint.y = (SHORT)yPos;

	bSel = FALSE;
	iCount  = ListView_GetItemCount( ghPageListWnd );
	iItem = ListView_GetNextItem( ghPageListWnd, -1, LVNI_ALL | LVNI_SELECTED);
	if( 0 <= iItem )	bSel = TRUE;


	hMenu = LoadMenu( GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_PGLVPOPUPMENU) );
	hSubMenu = GetSubMenu( hMenu, 0 );

	//	�ł��P�����Ȃ��Ȃ�A�폜�𖳌���
	if( 1 >= iCount ){	EnableMenuItem( hSubMenu, IDM_PAGEL_DELETE, MF_GRAYED );	}

	if( gbTmpltDock )
	{
		EnableMenuItem( hSubMenu, IDM_TOPMOST_TOGGLE, MF_GRAYED );
	}
	else
	{
		//	�őO�ʂł��邩
		rdExStyle = GetWindowLongPtr( hWnd, GWL_EXSTYLE );
		if( WS_EX_TOPMOST & rdExStyle ){	CheckMenuItem( hSubMenu , IDM_TOPMOST_TOGGLE, MF_BYCOMMAND | MF_CHECKED );	}
	}

	//	�|�b�p�b�v�\���L�邩�H
	if( gbPgTipView ){	CheckMenuItem( hSubMenu, IDM_PAGEL_AATIP_TOGGLE, MF_CHECKED );	}

	//	�I���Ńt�H�[�J�X�߂肩�H
	if( gbPgRetFocus ){	CheckMenuItem( hSubMenu, IDM_PAGEL_RETURN_FOCUS, MF_CHECKED );	}


	//	���I���Ȃ�A�I�����ĂȂ��Ǝg���Ȃ��@�\�𖳌���
	if( !(bSel) )
	{
		EnableMenuItem( hSubMenu, IDM_PAGEL_INSERT, MF_GRAYED );	//	�C�Ӎ쐬
		EnableMenuItem( hSubMenu, IDM_PAGEL_DELETE, MF_GRAYED );	//	�폜
		EnableMenuItem( hSubMenu, IDM_PAGEL_COMBINE, MF_GRAYED );	//	����
		EnableMenuItem( hSubMenu, IDM_PAGEL_UPFLOW, MF_GRAYED );	//	����
		EnableMenuItem( hSubMenu, IDM_PAGEL_DOWNSINK, MF_GRAYED );	//	���~
		EnableMenuItem( hSubMenu, IDM_PAGEL_DUPLICATE, MF_GRAYED );	//	����
		EnableMenuItem( hSubMenu, IDM_PAGEL_RENAME, MF_GRAYED );	//	����
	}

	dRslt = TrackPopupMenu( hSubMenu, 0, stPoint.x, stPoint.y, 0, hWnd, NULL );
	//	�I�������łO���|�P�H�A�I�������炻�̃��j���[�̂h�c��WM_COMMAND�����s
	DestroyMenu( hMenu );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t���[�e�B���O�ňꗗ�̈ʒu���Z�b�g
	@param[in]	hMainWnd	���C���E�C���h�E�n���h��
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT PageListPositionReset( HWND hMainWnd )
{
	RECT	wdRect, rect;

	GetWindowRect( hMainWnd, &wdRect );
	rect.left   = wdRect.left - PL_WIDTH;
	rect.top    = wdRect.top;
	rect.right  = PL_WIDTH;
	rect.bottom = PL_HEIGHT;

	SetWindowPos( ghPageWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW | SWP_NOZORDER );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/* !
	�e�E�C���h�E���ړ�������傫���ς������
	@param[in]	hPrntWnd	�e�E�C���h�E�n���h��
	@param[in]	pstFrame	�N���C�A���g�T�C�Y
*/
VOID PageListResize( HWND hPrntWnd, LPRECT pstFrame )
{
	RECT	rect, tbRect;
//gbTmpltDock

	rect = *pstFrame;	//	�N���C�����g�Ɏg����̈�
	rect.left  = rect.right - (grdSplitPos - SPLITBAR_WIDTH);	//	���ʒu
	rect.right = (grdSplitPos - SPLITBAR_WIDTH);	//	��
	if( gbDockTmplView )
	{
		rect.bottom >>= 1;
	}
	else
	{
		DockingTabSizeGet( &tbRect );	//	�^�u�̍������v��
		rect.bottom -= tbRect.bottom;
	}

	SetWindowPos( ghPageWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	���X�g�т�[��NOTIFY���b�Z�[�W����
	@param[in]	hWnd	�e�E�C���h�E�n���h��
	@param[in]	pstLv	�ǉ����\���̂ւ̃|�C���^�[
	@return		LRESULT	�Ȃ񂩂��낢������
*/
LRESULT PageListNotify( HWND hWnd, LPNMLISTVIEW pstLv )
{
	HWND	hLvWnd;
	INT		iCount, iItem, nmCode;//, iPage;

	INT		iSel;

	DWORD	lvClmn;
	INT		lvLine;
	LPNMLVCUSTOMDRAW	pstDraw;

	PAGEINFOS	stInfo;

	hLvWnd = pstLv->hdr.hwndFrom;
	nmCode = pstLv->hdr.code;
	//	�Ȃ�炩�̃A�N�V�����̋N������ROW�ʒu���Q�b�g���� 
	iCount = ListView_GetItemCount( hLvWnd );
	iItem  = pstLv->iItem;

	//	���ʂ̃N���b�N�ɂ���
	if( NM_CLICK == nmCode )
	{
		if( 0 <= iItem )	//	�Y���y�[�W�ւ̈ړ�
		{
			TRACE( TEXT("�y�[�W�I��[%d]"), iItem );
			DocPageChange( iItem );

			//	�t�H�[�J�X�߂��H
			if( gbPgRetFocus  ){	ViewFocusSet(  );	}
		}
	}

	//	�_�u���N���b�N
	if( NM_DBLCLK == nmCode )
	{
		if( 0 <= iItem )	//	�Ŗ���DIALOGUE���j���L
		{
			if( SUCCEEDED( PageListNameChange( iItem ) ) )
			{
				DocModifyContent( TRUE );
			}
		}
	}

	if( NM_RETURN == nmCode )
	{
//#ifdef PAGE_MULTISELECT

		iSel = ListView_GetNextItem( hLvWnd, -1, LVNI_ALL | LVNI_SELECTED );
		if( 0 > iSel )	return 0;	//	�I�����ĂȂ�������I���

		TRACE( TEXT("NM_RETURN[%d]"), iSel );
	}
//	NM_KEYDOWN	NM_CHAR	�֒m�ł���


//�J�X�^���h���[�E�T�u�N���X�̒����Ə�肭�����Ȃ��EWhy?
	if( NM_CUSTOMDRAW == nmCode )
	{
		pstDraw = (LPNMLVCUSTOMDRAW)pstLv;

		//	�����ň����������e��Ԃ��B�����鏉��o�^
		if( pstDraw->nmcd.dwDrawStage == CDDS_PREPAINT ){		return CDRF_NOTIFYSUBITEMDRAW;	}
		if( pstDraw->nmcd.dwDrawStage == CDDS_ITEMPREPAINT ){	return CDRF_NOTIFYSUBITEMDRAW;	}

		if( pstDraw->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT|CDDS_SUBITEM) )//(CDDS_ITEMPREPAINT|CDDS_SUBITEM)
		{
			lvLine = pstDraw->nmcd.dwItemSpec;	//	�s�E�Ŕԍ��ɂȂ�
			lvClmn = pstDraw->iSubItem;			//	�J����

			if( gixFocusPage == lvLine )	//	���̍s�E�Ƃ肠������
			{
				pstDraw->clrTextBk = 0x00FF8080;
			}
			else if( gixPreviSel == lvLine )	//	�O�̍s�E�Ƃ�₦���D
			{
				pstDraw->clrTextBk = 0x00CCCCDD;
			}
			else
			{
				pstDraw->clrTextBk = 0xFF000000;	//	����Ńf�t�H�F�w��E����
			}

			if( 0 == lvClmn )
			{
				if( NowPageInfoGet( lvLine, NULL ) )
				{
					pstDraw->clrTextBk = 0x00C0C0C0;
				}
				//else
				//{
				//	pstDraw->clrTextBk = 0xFF000000;
				//}
			}

			//	�ĕ`�悹���Ƃ��������ɕς��E�o�C�g���v�Z�̂Ƃ���ōĕ`�悵�Ă�H
			if( 2 == lvClmn )
			{
				stInfo.dMasqus = PI_BYTES;
				NowPageInfoGet( lvLine, &stInfo );

				if( gdPageByteMax <  (UINT)(stInfo.iBytes) )	pstDraw->clrTextBk = 0x000000FF;
				//else	pstDraw->clrTextBk = 0xFF000000;
			}
			//else
			//{
			//	pstDraw->clrTextBk = 0xFF000000;	//	����Ńf�t�H�F�w��
			//}

			return CDRF_NEWFONT;
		}
	}

	return 0;	//	�ʏ�͂O
}
//-------------------------------------------------------------------------------------------------

/*!
	���X�g�r���[�N����
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT PageListClear( VOID )
{
	ListView_DeleteAllItems( ghPageListWnd );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�J���Ă���œ��e��ύX
	@param[in]	iPage		�J���łO�C���f�b�N�X�E�\���͂P�C���f�b�N�X�Ȃ̂Œ���
	@param[in]	iPrePage	���O�܂őI�����Ă��ŁE�؂�ւ����ꍇ�́|�P
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListViewChange( INT iPage, INT iPrePage )
{
	//	�t�H�[�J�X�y�[�W�́A�����ɗ���O�ɕύX���Ă�������

	LONG	iItem;

	gixPreviSel = iPrePage;	//	���O�̑I��ł��L�^���Ă���

	InvalidateRect( ghPageListWnd, NULL, TRUE );

	iItem = ListView_GetItemCount( ghPageListWnd );
	if( iItem <= iPage || 0 > iPage )	return E_OUTOFMEMORY;

	//	�I����Ԃ�ύX
	ListView_SetItemState( ghPageListWnd, iPage, LVIS_SELECTED, LVIS_SELECTED );

	//	�r���[����������
	ViewEditReset(  );

	//	�����ŊJ�����ł𓊉��z�b�g�L�[�ԍ��ɃZ�b�g
	gixDropPage = iPage;

//	ViewFocusSet(  );	//	�����ł̓t�H�[�J�X���Ȃ��ق����ǂ���


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��̈ʒu�Ƀy�[�W��ǉ�
	@param[in]	iBefore	�|�P�Ȃ疖���ǉ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListInsert( INT iBefore )
{
	//	���X�g�r���[�̓r���}�����Ăł��������H
	UINT	iItem, i;
	TCHAR	atBuffer[MIN_STRING];
	LVITEM	stLvi;

	iItem = ListView_GetItemCount( ghPageListWnd );

	//	�Ƃ肠�������[�ɒǉ��E�r���ǉ��͌��

	ZeroMemory( &stLvi, sizeof(stLvi) );
	stLvi.mask  = LVIF_TEXT;

	//	�ꗗ�ԍ��͏��increment�ł����̂��H

	ZeroMemory( atBuffer, sizeof(atBuffer) );

	if( 0 > iBefore )	//	�����ǉ��Ȃ��s���₹�΂���
	{
		stLvi.iItem = iItem;

		StringCchPrintf( atBuffer, MIN_STRING, TEXT("%u"), iItem + 1 );
		stLvi.pszText  = atBuffer;
	}
	else
	{
		stLvi.iItem = iBefore;
		stLvi.pszText  = TEXT("");
	}

	stLvi.iSubItem = 0;
	ListView_InsertItem( ghPageListWnd, &stLvi );

	stLvi.pszText  = TEXT("");
	stLvi.iSubItem =  1;
	ListView_SetItem( ghPageListWnd, &stLvi );

	stLvi.pszText = TEXT("0");	//	byte
	stLvi.iSubItem =  2;
	ListView_SetItem( ghPageListWnd, &stLvi );

	stLvi.pszText = TEXT("1");	//	line
	stLvi.iSubItem =  3;
	ListView_SetItem( ghPageListWnd, &stLvi );


	if( 0 <= iBefore )
	{
		//	�A�ԐU�蒼��
		iItem = ListView_GetItemCount( ghPageListWnd );
		for( i = iBefore; iItem > i; i++ )
		{
			StringCchPrintf( atBuffer, MIN_STRING, TEXT("%u"), i + 1 );
			ListView_SetItemText( ghPageListWnd, i, 0, atBuffer );
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Ń��X�g���č\������
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListBuild( LPVOID pVoid )
{
	INT			i, iLastPage;
	PAGE_ITR	itPage;
	LVITEM		stLvi;
	TCHAR		atBuffer[MIN_STRING];

	ZeroMemory( &stLvi, sizeof(stLvi) );
	stLvi.mask  = LVIF_TEXT;

#pragma message ("�ňꗗ�č\���E���ڒ���")

	i = 0;
	for( itPage = (*gitFileIt).vcCont.begin(); itPage != (*gitFileIt).vcCont.end(); itPage++ )
	{
		stLvi.iItem    = i;
		StringCchPrintf( atBuffer, MIN_STRING, TEXT("%u"), i + 1 );
		stLvi.pszText  = atBuffer;
		stLvi.iSubItem = 0;
		ListView_InsertItem( ghPageListWnd, &stLvi );

		stLvi.pszText  = itPage->atPageName;
		stLvi.iSubItem =  1;
		ListView_SetItem( ghPageListWnd, &stLvi );

		StringCchPrintf( atBuffer, MIN_STRING, TEXT("%d"), itPage->dByteSz );
		stLvi.pszText  = atBuffer;
		stLvi.iSubItem =  2;
		ListView_SetItem( ghPageListWnd, &stLvi );

		StringCchPrintf( atBuffer, MIN_STRING, TEXT("%u"), itPage->ltPage.size() );
		stLvi.iSubItem =  3;
		ListView_SetItem( ghPageListWnd, &stLvi );

		i++;
	}

	//	���Ă��ł����X�g�ɘI�o������
	iLastPage = (*gitFileIt).dNowPage;
	ListView_EnsureVisible( ghPageListWnd, iLastPage, FALSE );	//	�Ώەł̃��C���܂ŃX�N���[���������Ⴄ

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I�΂ꂽ�ł��㉺�ړ�
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	iPage	�ړ�������Ŕԍ�
	@param[in]	bDir	���F��ց@���F����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListSpinning( HWND hWnd, INT iPage, INT bDir )
{
	INT	iItem, i = 0;
	PAGE_ITR	itPage, itSwap;

	iItem = ListView_GetItemCount( ghPageListWnd );

	if( 0 == bDir ){	return E_INVALIDARG;	}	//	���܂�Ӗ��͂Ȃ�

	//	����ȏな�ɂ����Ȃ��Ȃ牽�����Ȃ�
	if( (0 == iPage) && (0 < bDir) ){	return  E_ABORT;	}

	//	����ȏに�ɃC�P�i�C�Ȃ牽�����Ȃ�
	if( (iItem <= (iPage+1)) && (0 > bDir) ){	return  E_ABORT;	}

	TRACE( TEXT("�ňړ�����[%d]"), iPage );

	//	�X���ʒu�܂ŃC�e���[�^�������Ă���
	itPage = (*gitFileIt).vcCont.begin(  );
	std::advance( itPage, iPage );

	//	�X���b�v�Ώ�
	if( 0 <  bDir ){	itSwap = itPage - 1;	i = iPage - 1;	}	//	���Ɉړ��Ȃ璼�O�̂�ƌ���
	if( 0 >  bDir ){	itSwap = itPage + 1;	i = iPage + 1;	}	//	���Ȃ璼��̂ƃ`�F���W�Q�b�^�[

	iter_swap( itPage, itSwap );	//	�X���b�s���O�I

	//	�\�����e����ւ��āA�I�������Ă���
	PageListViewRewrite( iPage );
	PageListViewRewrite( i );

	//	�I����Ԃ�ύX
	ListView_SetItemState( ghPageListWnd, i, LVIS_SELECTED, LVIS_SELECTED );

	DocPageChange( i );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ꗗ�̎w��̕ł��폜����
	@param[in]	iPage	�T�N�b���Ŕԍ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListDelete( INT iPage )
{
	UINT	iItem, i;
	TCHAR	atBuffer[MIN_STRING];

	ListView_DeleteItem( ghPageListWnd, iPage );
	//	Delete������A���X�g�r���[�͎����ŋl�߂���

	//	�A�ԐU�蒼��
	iItem = ListView_GetItemCount( ghPageListWnd );
	for( i = 0; iItem > i; i++ )
	{
		StringCchPrintf( atBuffer, MIN_STRING, TEXT("%u"), i + 1 );
		ListView_SetItemText( ghPageListWnd, i, 0, atBuffer );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�e�ł̏󋵁E�����͂��̂����\���̂ɂ����ق�����������
	@param[in]	iPage	�Ŕԍ�
	@param[in]	dByte	�o�C�g��
	@param[in]	dLine	�s��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListInfoSet( INT iPage, INT dByte, INT dLine )
{
	INT		iPageCnt;
	TCHAR	atBuffer[MIN_STRING];

	iPageCnt = ListView_GetItemCount( ghPageListWnd );
	if( iPageCnt <= iPage ){	return E_OUTOFMEMORY;	}

	StringCchPrintf( atBuffer, MIN_STRING, TEXT("%d"), dByte );	//	byte
	ListView_SetItemText( ghPageListWnd, iPage, 2, atBuffer );

	StringCchPrintf( atBuffer, MIN_STRING, TEXT("%d"), dLine );	//	line
	ListView_SetItemText( ghPageListWnd, iPage, 3, atBuffer );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	����ł̏���������������
	@param[in]	iPage	�Ŕԍ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListViewRewrite( INT iPage )
{
#pragma message ("�ňꗗ�ĕ`��E���ڒ���")
	UINT_PTR	dLines;
	UINT		dBytes;
	INT			iPageCount, i;
	TCHAR	atBuffer[MIN_STRING];

	iPageCount = ListView_GetItemCount( ghPageListWnd );
	if( iPageCount <= iPage )	return E_OUTOFMEMORY;

	if( 0 >  iPage )	//	�ċA�őS�`��
	{
		for( i = 0; iPageCount > i; i++ )
		{
			PageListViewRewrite( i );
		}

		return S_OK;
	}

	StringCchPrintf( atBuffer, MIN_STRING, TEXT("%d"), iPage + 1 );
	ListView_SetItemText( ghPageListWnd, iPage, 0, atBuffer );

	ListView_SetItemText( ghPageListWnd, iPage, 1, (*gitFileIt).vcCont.at( iPage ).atPageName );

	dBytes = (*gitFileIt).vcCont.at( iPage ).dByteSz;
	StringCchPrintf( atBuffer, MIN_STRING, TEXT("%d"), dBytes );	//	byte
	ListView_SetItemText( ghPageListWnd, iPage, 2, atBuffer );

	dLines = (*gitFileIt).vcCont.at( iPage ).ltPage.size( );
	StringCchPrintf( atBuffer, MIN_STRING, TEXT("%d"), dLines );	//	line
	ListView_SetItemText( ghPageListWnd, iPage, 3, atBuffer );



	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�H�[�J�X�ł��ړ�
	@param[in]	iDiff	�ړ���E�͈͊O��������Ȃ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListJump( INT iDiff )
{
	INT	iItem;

	if( 0 >  iDiff )	return E_OUTOFMEMORY;

	iItem = ListView_GetItemCount( ghPageListWnd );
	if( iItem <= iDiff )	return E_OUTOFMEMORY;

	ListView_EnsureVisible( ghPageListWnd, iDiff, FALSE );

	DocPageChange( iDiff );

	ViewFocusSet(  );	//	�t�H�[�J�X�߂�

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���̕ύX�_�C�����O�{�b�N�X�̃��Z�[�W�n���h�������Ă΂�
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK PageNameDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	static INT cdPage;
//	TCHAR	atBuffer[MAX_PATH];

	switch( message )
	{
		case WM_INITDIALOG:
			cdPage = lParam;	//	�ҏW����Ŕԍ�
			Edit_SetText( GetDlgItem(hDlg,IDE_PAGENAME), (*gitFileIt).vcCont.at( cdPage ).atPageName );
			SetFocus( GetDlgItem(hDlg,IDE_PAGENAME) );
			return (INT_PTR)FALSE;

		case WM_COMMAND:
			if( IDOK == LOWORD(wParam) )
			{
				Edit_GetText( GetDlgItem(hDlg,IDE_PAGENAME), (*gitFileIt).vcCont.at( cdPage ).atPageName, SUB_STRING );
				EndDialog( hDlg, IDOK );
				return (INT_PTR)TRUE;
			}

			if( IDCANCEL == LOWORD(wParam) )
			{
				EndDialog( hDlg, IDCANCEL );
				return (INT_PTR)TRUE;
			}

			break;
	}
	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Ŗ��O�ύX�_�C�����O�Ƃ�
	@param[in]	dPage	�Ŕԍ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListNameChange( INT dPage )
{
	INT_PTR	iRslt;

	iRslt = DialogBoxParam( ghInst, MAKEINTRESOURCE(IDD_PAGE_NAME_DLG), ghPageWnd, PageNameDlgProc, dPage );
	if( IDOK == iRslt )
	{
		PageListNameSet( dPage, (*gitFileIt).vcCont.at( dPage ).atPageName );
		return S_OK;
	}

	return E_ABORT;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Ŗ��O���Z�b�g
	@param[in]	dPage	�Ŕԍ�
	@param[in]	ptName	�Ŗ���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListNameSet( INT dPage, LPTSTR ptName )
{
	INT		iPageCount;
	LVITEM	stLvi;

	iPageCount = ListView_GetItemCount( ghPageListWnd );
	if( iPageCount <= dPage )	return E_OUTOFMEMORY;

	ZeroMemory( &stLvi, sizeof(stLvi) );
	stLvi.mask     = LVIF_TEXT;
	stLvi.iItem    = dPage;
	stLvi.pszText  = ptName;
	stLvi.iSubItem =  1;	//	���O
	ListView_SetItem( ghPageListWnd, &stLvi );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���J���Ă���ł̖��O���Z�b�g
	@param[in]	ptName	�Ŗ��́E�U�R�����ȓ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListNameRewrite( LPTSTR ptName )
{
	UINT_PTR	cchSize;

	StringCchLength( ptName, STRSAFE_MAX_CCH, &cchSize );

	//	�͂ݏo���Ȃ��悤�ɁA�T�C�Y���[������NULL
	if( SUB_STRING <= cchSize ){	ptName[(SUB_STRING-1)] = NULL;	}

	//	�{�̏���������
	StringCchCopy( (*gitFileIt).vcCont.at( gixFocusPage ).atPageName, SUB_STRING, ptName );

	PageListNameSet( gixFocusPage, ptName );	//	�\��������

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���O�̕t���Ă���ł����邩
	@param[in]	itFile	�`�F�b�N�Ώۃt�@�C���̃C�e���[�^
	@return	INT	��O���O�t�����������@�O�Ȃ�����
*/
INT PageListIsNamed( FILES_ITR itFile )
{
	UINT_PTR	iPageCount, i;

	iPageCount = itFile->vcCont.size(  );
	for( i = 0; iPageCount > i; i++ )
	{
		if( 0 != itFile->vcCont.at( i ).atPageName[0] ){	return TRUE;	}
	}

	return FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�H�[�J�X�ł̓��e���A���̕ł�����ăR�s�[����
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	iNowPage	�t�H�[�J�X���Ă��
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT PageListDuplicate( HWND hWnd, INT iNowPage )
{
	INT		iNewPage;
//	INT_PTR	iNext, iTotal;
	LINE_ITR	itLine;

	TRACE( TEXT("�ŕ���") );

	//iTotal = DocNowFilePageCount(  );
	//iNext = iNowPage + 1;	//	���̕�
	//if( iTotal <= iNext ){	iNext =  -1;	}	//	�S�ł�葽���Ȃ疖�[�w��

	iNewPage = DocPageCreate( iNowPage );	//	�V��
	PageListInsert( iNewPage  );	//	�y�[�W���X�g�r���[�ɒǉ�

	//	��̈�s������Ă�̂ŁA�폜���Ă���
	(*gitFileIt).vcCont.at( iNewPage ).ltPage.clear(  );

	std::copy(	(*gitFileIt).vcCont.at( iNowPage ).ltPage.begin(),
				(*gitFileIt).vcCont.at( iNowPage ).ltPage.end(),
				back_inserter( (*gitFileIt).vcCont.at( iNewPage ).ltPage ) );

#pragma message ("PageListDuplicate ������ł̓��e�̍Čv�Z���邩�H")

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�H�[�J�X�ł̎��̕łƓ�������
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	iNowPage	�t�H�[�J�X���Ă��
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT PageListCombine( HWND hWnd, INT iNowPage )
{
	 INT	iLastLine, iLastDot;
	 INT	iNext;
	INT_PTR	iTotal;
	ONELINE	stLine;
	LINE_ITR	itLine;

	ZeroONELINE( &stLine );

	TRACE( TEXT("�œ���") );

	iTotal = DocNowFilePageCount(  );

	//	�Ő�������Ȃ��Ȃ�i�j�����Ȃ�
	if( 1 >= iTotal )	return E_ACCESSDENIED;


	iNext = iNowPage + 1;	//	���̕�
	if( iTotal <= iNext ){	return E_OUTOFMEMORY;	}	//	���[�łȂ牽�����Ȃ�

	//	��؂�Ƃ��ĉ��s�����
	(*gitFileIt).vcCont.at( iNowPage ).ltPage.push_back( stLine );

	//	��ɏ󋵃`�F�b�N���āA�����[�h�Ȃ�A�L�����b�g�ʒu�𖖒[�ɂ��ă��[�h���āA���ɖ߂�
	if( (*gitFileIt).vcCont.at( iNext ).ptRawData )	//	���f�[�^��Ԃ�������
	{
		//	���̖��[�ʒu�`�F�b�N
		iLastLine = DocNowFilePageLineCount(  ) - 1;
		iLastDot  = DocLineParamGet( iLastLine, NULL, NULL );

		DocInsertString( &iLastDot, &iLastLine, NULL, (*gitFileIt).vcCont.at( iNext ).ptRawData, 0, TRUE );
	}
	else
	{
		//	���̕ł̑S�̂��R�s�[�����Ⴄ
		std::copy(	(*gitFileIt).vcCont.at( iNext ).ltPage.begin(),
					(*gitFileIt).vcCont.at( iNext ).ltPage.end(),
					back_inserter( (*gitFileIt).vcCont.at( iNowPage ).ltPage ) );
	}

	SqnFreeAll( &((*gitFileIt).vcCont.at( iNowPage ).stUndoLog) );	//	�A���h�D���O�폜

	DocPageDelete( iNext , -1 );	//	���̕ł͍폜�����Ⴄ

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e�����X�g�r���[�T�u�N���X
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	msg		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
*/
LRESULT CALLBACK gpfPageViewProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
//	INT	id;

	switch( msg )
	{
		HANDLE_MSG( hWnd, WM_MOUSEMOVE, Plv_OnMouseMove );	
		HANDLE_MSG( hWnd, WM_COMMAND,   Plt_OnCommand );	
#ifdef PGL_TOOLTIP
		HANDLE_MSG( hWnd, WM_NOTIFY,    Plv_OnNotify  );	//	�R�����R���g���[���̌ʃC�x���g
#endif

#ifdef USE_HOVERTIP
		case WM_MOUSEHOVER:
			HoverTipOnMouseHover( hWnd, wParam, lParam, PageListHoverTipInfo );
			return 0;

		case WM_MOUSELEAVE:
			HoverTipOnMouseLeave( hWnd );
			gixMouseSel = -1;
			return 0;
#endif
	}

	return CallWindowProc( gpfOrigPageViewProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	�}�E�X���������Ƃ��̏���
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	x			�N���C�A���g���W�w
	@param[in]	y			�N���C�A���g���W�x
	@param[in]	keyFlags	������Ă鑼�̃{�^��
	@return		�Ȃ�
*/
VOID Plv_OnMouseMove( HWND hWnd, INT x, INT y, UINT keyFlags )
{
	LVHITTESTINFO	stHitInfo;
	INT	iItem;
	BOOLEAN	bReDraw = FALSE;

	//	���̂Ƃ��}�E�X�J�[�\�����ɂ���A�C�e����I�����Ă���

	ZeroMemory( &stHitInfo, sizeof(LVHITTESTINFO) );
	stHitInfo.pt.x = 10;	//	�������d�v�Ȃ̂ł����͓K���ł���
	stHitInfo.pt.y = y;

	iItem = ListView_HitTest( hWnd, &stHitInfo );
	if( gixMouseSel != iItem )	bReDraw = TRUE;
	gixMouseSel = iItem;

#ifdef USE_HOVERTIP
	if( bReDraw ){	HoverTipResist( ghPageListWnd );	}
#endif

#ifdef PGL_TOOLTIP
	if( bReDraw ){	SendMessage( ghPageTipWnd, TTM_UPDATE, 0, 0 );	}
#endif

//	TRACE( TEXT("PLV MM %d,%d,%d [%d]"), iItem, stHitInfo.iItem, stHitInfo.iSubItem, bReDraw );

	return;
}
//-------------------------------------------------------------------------------------------------

#ifdef PGL_TOOLTIP
/*!
	�m�[�e�B�t�@�C���b�Z�[�W�̏���
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	idFrom		NOTIFY�𔭐��������R���g���[���̂h�c
	@param[in]	pstNmhdr	NOTIFY�̏ڍ�
	@return		�����������e�Ƃ�
*/
LRESULT Plv_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	INT				dBytes;
	UINT_PTR		rdLength;
	LPNMTTDISPINFO	pstDispInfo;

	if( TTN_GETDISPINFO == pstNmhdr->code )	//	�c�[���`�b�v�̓��e�̖₢���킹��������
	{
		if( IDLV_PAGELISTVIEW == idFrom )
		{
			pstDispInfo = (LPNMTTDISPINFO)pstNmhdr;

			if( gixPreSel != gixMouseSel )
			{

				ZeroMemory( pstDispInfo->szText, sizeof(pstDispInfo->szText) );
				pstDispInfo->lpszText = NULL;

				FREE( gptPgTipBuf );

				if( !(gbPgTipView) ){	return 0;	}	//	��\���Ȃ牽�����Ȃ��ł���
				if( 0 > gixMouseSel ){	return 0;	}

				TRACE( TEXT("1 TTN_GETDISPINFO %d  %X"), gixMouseSel, pstDispInfo->uFlags );
				gixPreSel = gixMouseSel;

				//	�Y���y�[�W�����������
				dBytes = DocPageTextGetAlloc( gitFileIt, gixMouseSel, D_UNI, (LPVOID *)(&gptPgTipBuf), FALSE );

				if( gptPgTipBuf )
				{
					StringCchLength( gptPgTipBuf, STRSAFE_MAX_CCH, &rdLength );
					//if( 2 <= rdLength )
					//{
					//	//	���[�ɗ]�v�ȉ��s������̂ŏ����Ă���
					//	gptPgTipBuf[rdLength-1] = NULL;
					//	gptPgTipBuf[rdLength-2] = NULL;
					//	rdLength -= 2;
					//}
					//	�]�v�ȉ��s�͖����Ȃ���

					pstDispInfo->lpszText = gptPgTipBuf;
				}
			}
			else
			{
				TRACE( TEXT("2 TTN_GETDISPINFO %d  %X"), gixMouseSel, pstDispInfo->uFlags );

				ZeroMemory( pstDispInfo->szText, sizeof(pstDispInfo->szText) );
				pstDispInfo->lpszText = gptPgTipBuf;
			}

			return 0;
		}
	}

//	�����Ȃ������瑱����H
	return CallWindowProc( gpfOrigPageViewProc, hWnd, WM_NOTIFY, (WPARAM)idFrom, (LPARAM)pstNmhdr );

	//	�������[�v���ĂȂ����A���v��
}
//-------------------------------------------------------------------------------------------------
#endif

#ifdef USE_HOVERTIP
/*!
	HoverTip�p�̃R�[���o�b�N���
	@param[in]	pVoid	����`
	@return	�m�ۂ���������E��������NULL
*/
LPTSTR CALLBACK PageListHoverTipInfo( LPVOID pVoid )
{
	INT		dBytes;
	LPTSTR	ptBuffer = NULL;

	if( !(gbPgTipView) ){	return NULL;	}	//	��\���Ȃ牽�����Ȃ��ł���
	if( 0 > gixMouseSel ){	return NULL;	}

	//	�Y���y�[�W�����������
	dBytes = DocPageTextGetAlloc( gitFileIt, gixMouseSel, D_UNI, (LPVOID *)(&ptBuffer), FALSE );
	TRACE( TEXT("HOVER CALL %d, by[%d]"), gixMouseSel, dBytes );

	return ptBuffer;
}
//-------------------------------------------------------------------------------------------------
#endif

