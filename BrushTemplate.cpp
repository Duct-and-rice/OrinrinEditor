/*! @file
	@brief �u���V�e���v���[�g�̖ʓ|���܂�
	���̃t�@�C���� BrushTemplate.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/06/20
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

#define BRUSHTEMPLATE_CLASS	TEXT("BRUSH_TEMPLATE")
#define BT_WIDTH	240
#define BT_HEIGHT	240

#define BTV_R_MARGIN	18
//-------------------------------------------------------------------------------------------------

#define TB_ITEMS	1
static  TBBUTTON	gstBrTBInfo[] = {
	{ 0,	IDM_BRUSH_ON_OFF,	TBSTATE_ENABLED,	TBSTYLE_CHECK | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0 }	//	On/Off
};	//	
//-------------------------------------------------------------------------------------------------

extern HFONT	ghAaFont;		//	AA�p�t�H���g

extern INT		gbTmpltDock;	//	�e���v���̃h�b�L���O
extern BOOLEAN	gbDockTmplView;	//	�������Ă�e���v���͌����Ă��邩

//extern  HWND	ghMainSplitWnd;	//	���C���̃X�v���b�g�o�[�n���h��
extern  LONG	grdSplitPos;	//	�X�v���b�g�o�[�́A�����́A��ʉE����̃I�t�Z�b�g

static HIMAGELIST	ghBrushImgLst;

static  UINT	gbBrushMode;	//!<	���u���V���[�h

static  ATOM	gBrTmplAtom;	//!<	
static  HWND	ghBrTmplWnd;	//!<	�u���V�p���b�g�{�̃E�C���h�E
static  HWND	ghBrTlBarWnd;	//!<	�c�[���o�[
static  HWND	ghCtgryBxWnd;	//!<	�J�e�S���R���{�b�N�X
static  HWND	ghLvItemWnd;	//!<	���e���X�g�r���[
static  HWND	ghBrLvTipWnd;	//!<	Brush���X�g�c�[���`�b�v

static  HWND	ghMainWnd;		//!<	�ҏW�r���[�̂���{�̃E�C���h�E

static  UINT	gNowGroup;		//!<	�J�e�S��

static WNDPROC	gpfOrigBrushCtgryProc;	//!<	
static WNDPROC	gpfOrigBrushItemProc;	//!<	

static  UINT	gBrhClmCnt;	//!<	�\���J������

static WNDPROC	gpfOrigTBProc;	//!<	

static vector<AATEMPLATE>	gvcBrTmpls;	//!<	�e���v���̕ێ�
//-------------------------------------------------------------------------------------------------


LRESULT	CALLBACK BrushTmpleProc( HWND, UINT, WPARAM, LPARAM );
VOID	Btp_OnCommand( HWND, INT, HWND, UINT );
VOID	Btp_OnSize( HWND, UINT, INT, INT );
LRESULT	Btp_OnNotify( HWND, INT, LPNMHDR );
VOID	Btp_OnContextMenu( HWND, HWND, UINT, UINT );

UINT	CALLBACK BrushTmpleItemData( LPTSTR, LPCTSTR, INT );

UINT	BrushTmpleItemListOn( UINT );
HRESULT	BrushTmpleItemReload( HWND );

LRESULT	CALLBACK gpfBrushCtgryProc( HWND, UINT, WPARAM, LPARAM );
LRESULT	CALLBACK gpfBrushItemProc(  HWND, UINT, WPARAM, LPARAM );
LRESULT	Blv_OnNotify( HWND, INT, LPNMHDR );

static LRESULT	CALLBACK gpfToolbarProc( HWND, UINT, WPARAM, LPARAM );
//-------------------------------------------------------------------------------------------------


/*!
	�u���V�e���v���E�C���h�E�̍쐬
	@param[in]	hInstance	�A�v���̃C���X�^���X
	@param[in]	hParentWnd	���C���E�C���h�E�̃n���h��
	@param[in]	pstFrame	
	@param[in]	hMaaWnd		�����s�e���v���̃E�C���h�E�n���h��
	@return		������r���[�̃E�C���h�E�n���h��
*/
HWND BrushTmpleInitialise( HINSTANCE hInstance, HWND hParentWnd, LPRECT pstFrame, HWND hMaaWnd )
{
	DWORD		dwExStyle, dwStyle;
	HWND		hPrWnd;
	UINT_PTR	dItems, i;
	TCHAR		atBuffer[MAX_STRING];

	HBITMAP	hImg, hMsq;
	INT		spPos;


	WNDCLASSEX	wcex;
	RECT		wdRect, clRect, rect, cbxRect, tbRect, mtbRect;
	LVCOLUMN	stLvColm;

	TTTOOLINFO	stToolInfo;


	//	�j��
	if( !(hInstance) && !(hParentWnd) )
	{
		ImageList_Destroy( ghBrushImgLst  );
		return NULL;
	}

	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= BrushTmpleProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= BRUSHTEMPLATE_CLASS;
	wcex.hIconSm		= NULL;

	gBrTmplAtom = RegisterClassEx( &wcex );

	ghMainWnd = hParentWnd;

	gbBrushMode = FALSE;

//�e���v���f�[�^�ǂݏo��
	TemplateItemLoad( AA_BRUSH_FILE, BrushTmpleItemData );


	InitWindowPos( INIT_LOAD, WDP_BRTMPL, &rect );
	if( 0 == rect.right || 0 == rect.bottom )	//	���������O�̓f�[�^����
	{
		GetWindowRect( hParentWnd, &wdRect );
		rect.left   = wdRect.right + 32;
		rect.top    = wdRect.top + 32;
		rect.right  = BT_WIDTH;
		rect.bottom = BT_HEIGHT;
		InitWindowPos( INIT_SAVE, WDP_BRTMPL, &rect );//�N�����ۑ�
	}

	//	�J�������m�F
	gBrhClmCnt = InitParamValue( INIT_LOAD, VL_BRUSHTMP_CLM, 4 );

	if( gbTmpltDock )
	{
		spPos = grdSplitPos - SPLITBAR_WIDTH;	//	�E����̃I�t�Z�b�g

		hPrWnd    = hParentWnd;
		dwExStyle = 0;
		dwStyle   = WS_CHILD;

		rect = *pstFrame;	//	�N���C�����g�Ɏg����̈�
		rect.left  = rect.right - spPos;
		rect.right = PLIST_DOCK;
		rect.bottom >>= 1;
		rect.top    += rect.bottom;

		DockingTabSizeGet( &mtbRect );
		rect.top    += mtbRect.bottom;
		rect.bottom -= mtbRect.bottom;
	}
	else
	{
		hPrWnd = NULL;
		//	��ɍőS�ʂɕ\�����H
		dwExStyle = WS_EX_TOOLWINDOW;
		if( InitWindowTopMost( INIT_LOAD, WDP_BRTMPL, 0 ) ){	dwExStyle |=  WS_EX_TOPMOST;	}
		dwStyle = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_VISIBLE | WS_SYSMENU;
	}

	//	�{��
	ghBrTmplWnd = CreateWindowEx( dwExStyle, BRUSHTEMPLATE_CLASS, TEXT("Brush Template"),
		dwStyle, rect.left, rect.top, rect.right, rect.bottom, hPrWnd, NULL, hInstance, NULL);

	//	�c�[���o�[
	ghBrTlBarWnd = CreateWindowEx( WS_EX_CLIENTEDGE, TOOLBARCLASSNAME, TEXT("brtoolbar"), WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TOOLTIPS, 0, 0, 0, 0, ghBrTmplWnd, (HMENU)IDW_BRUSH_TOOL_BAR, hInstance, NULL );

	//	�����c�[���`�b�v�X�^�C����ǉ�
	SendMessage( ghBrTlBarWnd, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS );

	ghBrushImgLst = ImageList_Create( 16, 16, ILC_COLOR24 | ILC_MASK, 1, 1 );
	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( (IDBMP_BRUSH_MODE) ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( (IDBMQ_BRUSH_MODE) ) );
	ImageList_Add( ghBrushImgLst , hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ�
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );
	SendMessage( ghBrTlBarWnd, TB_SETIMAGELIST, 0, (LPARAM)ghBrushImgLst );

	SendMessage( ghBrTlBarWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0 );
	//	�c�[���`�b�v�������ݒ�E�{�^���e�L�X�g���c�[���`�b�v�ɂȂ�
	StringCchCopy( atBuffer, MAX_STRING, TEXT("�u���V���[�h ON/OFF") );
	gstBrTBInfo[0].iString = SendMessage( ghBrTlBarWnd, TB_ADDSTRING, 0, (LPARAM)atBuffer );

	SendMessage( ghBrTlBarWnd , TB_ADDBUTTONS, (WPARAM)TB_ITEMS, (LPARAM)&gstBrTBInfo );	//	�c�[���o�[�Ƀ{�^����}��

	SendMessage( ghBrTlBarWnd , TB_AUTOSIZE, 0, 0 );	//	�{�^���̃T�C�Y�ɍ��킹�ăc�[���o�[�����T�C�Y
	InvalidateRect( ghBrTlBarWnd , NULL, TRUE );		//	�N���C�A���g�S�̂��ĕ`�悷�閽��

	//	�c�[���o�[�T�u�N���X��
	gpfOrigTBProc = SubclassWindow( ghBrTlBarWnd, gpfToolbarProc );

	GetClientRect( ghBrTlBarWnd, &tbRect );





	GetClientRect( ghBrTmplWnd, &clRect );

	//	�J�e�S���R���{�b�N�X
	ghCtgryBxWnd = CreateWindowEx( 0, WC_COMBOBOX, TEXT("BrCategory"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST,
		0, tbRect.bottom, clRect.right, 127, ghBrTmplWnd,
		(HMENU)IDCB_BT_CATEGORY, hInstance, NULL );

	gpfOrigBrushCtgryProc = SubclassWindow( ghCtgryBxWnd, gpfBrushCtgryProc );

	dItems = gvcBrTmpls.size( );
	for( i = 0; dItems > i; i++ )
	{
		ComboBox_AddString( ghCtgryBxWnd, gvcBrTmpls.at( i ).atCtgryName );
	}
	ComboBox_SetCurSel( ghCtgryBxWnd, 0 );
	gNowGroup = 0;

	GetClientRect( ghCtgryBxWnd, &cbxRect );

	//	�ꗗ���X�g�r���[
	ghLvItemWnd = CreateWindowEx( 0, WC_LISTVIEW, TEXT("brushitem"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LVS_REPORT | LVS_NOSORTHEADER | LVS_NOCOLUMNHEADER | LVS_SINGLESEL,
		0, tbRect.bottom + cbxRect.bottom, clRect.right, clRect.bottom - (cbxRect.bottom + tbRect.bottom),
		ghBrTmplWnd, (HMENU)IDLV_BT_ITEMVIEW, hInstance, NULL );
	ListView_SetExtendedListViewStyle( ghLvItemWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	SetWindowFont( ghLvItemWnd, ghAaFont, TRUE );

	gpfOrigBrushItemProc = SubclassWindow( ghLvItemWnd, gpfBrushItemProc );

	ZeroMemory( &stLvColm, sizeof(LVCOLUMN) );
	stLvColm.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	stLvColm.fmt      = LVCFMT_LEFT;
	stLvColm.pszText  = TEXT("Brush");
	stLvColm.cx       = 10;	//	��ō��킹��̂œK���ŗǂ�
	stLvColm.iSubItem = 0;

	for( i = 0; gBrhClmCnt > i; i++ )
	{
		stLvColm.iSubItem = i;
		ListView_InsertColumn( ghLvItemWnd, i, &stLvColm );
	}

	BrushTmpleItemListOn( 0 );	//	���g�ǉ�

	//	���X�g�r���[�c�[���`�b�v
	ghBrLvTipWnd = CreateWindowEx( WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, TTS_NOPREFIX | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, ghBrTmplWnd, NULL, hInstance, NULL );
//	SetWindowFont( ghBrLvTipWnd, GetStockFont(DEFAULT_GUI_FONT), FALSE );

	//	�c�[���`�b�v���R�[���o�b�N�Ŋ���t��
	ZeroMemory( &stToolInfo, sizeof(TTTOOLINFO) );
	stToolInfo.cbSize   = sizeof(TTTOOLINFO);
	stToolInfo.uFlags   = TTF_SUBCLASS;
	stToolInfo.hinst    = NULL;	//	
	stToolInfo.hwnd     = ghLvItemWnd;
	stToolInfo.uId      = IDLV_BT_ITEMVIEW;
	GetClientRect( ghLvItemWnd, &stToolInfo.rect );
	stToolInfo.lpszText = LPSTR_TEXTCALLBACK;	//	�R�����w�肷��ƃR�[���o�b�N�ɂȂ�
	SendMessage( ghBrLvTipWnd, TTM_ADDTOOL, 0, (LPARAM)&stToolInfo );
	SendMessage( ghBrLvTipWnd, TTM_SETMAXTIPWIDTH, 0, 0 );	//	�`�b�v�̕��B�O�ݒ�ł����B���ꂵ�Ƃ��Ȃ��Ɖ��s����Ȃ�


	if( !(gbTmpltDock) )
	{
		ShowWindow( ghBrTmplWnd, SW_SHOW );
		UpdateWindow( ghBrTmplWnd );
	}

	return ghBrTmplWnd;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t���[�e�B���O�u���V�e���v���̈ʒu���Z�b�g
	@param[in]	hMainWnd	���C���E�C���h�E�n���h��
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT BrushTmplePositionReset( HWND hMainWnd )
{
	RECT	wdRect, rect;

	GetWindowRect( hMainWnd, &wdRect );
	rect.left   = wdRect.right + 32;
	rect.top    = wdRect.top + 32;
	rect.right  = BT_WIDTH;
	rect.bottom = BT_HEIGHT;

	SetWindowPos( ghBrTmplWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW | SWP_NOZORDER );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�h�b�L���O��ԂŔ����E�������Ă�E�C���h�E�����T�C�Y���ꂽ��
	@param[in]	hPrntWnd	�������Ă�E�C���h�E�n���h��
	@param[in]	pstFrame	�g����T�C�Y
*/
VOID BrushTmpleResize( HWND hPrntWnd, LPRECT pstFrame )
{
	RECT	rect, tbRect;
	//INT	iHei, iTak;

	//gbTmpltDock
	if( !(ghBrTmplWnd) )	return;

	//	��\���Ȃ牽�����Ȃ��ł���
	if( !(gbDockTmplView) )	return;

	rect = *pstFrame;	//	�N���C�����g�Ɏg����̈�
	rect.left    = rect.right - (grdSplitPos - SPLITBAR_WIDTH);
	rect.right   = (grdSplitPos - SPLITBAR_WIDTH);
	rect.bottom >>= 1;
	rect.top    += rect.bottom;

	DockingTabSizeGet( &tbRect );
	rect.top    += tbRect.bottom;
	rect.bottom -= tbRect.bottom;

	SetWindowPos( ghBrTmplWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_NOZORDER );

	return;
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
	@return	�����������ʂƂ�
*/
LRESULT CALLBACK gpfToolbarProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			if( SendMessage(hWnd, TB_GETHOTITEM, 0, 0) >= 0 ){	ReleaseCapture(   );	}
			return 0;
	}

	return CallWindowProc( gpfOrigTBProc, hWnd, msg, wParam, lParam );
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
LRESULT CALLBACK BrushTmpleProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		HANDLE_MSG( hWnd, WM_SIZE,        Btp_OnSize );	
		HANDLE_MSG( hWnd, WM_COMMAND,     Btp_OnCommand );	
		HANDLE_MSG( hWnd, WM_NOTIFY,      Btp_OnNotify );	//	�R�����R���g���[���̌ʃC�x���g
		HANDLE_MSG( hWnd, WM_CONTEXTMENU, Btp_OnContextMenu );

		case WM_MOUSEWHEEL:	SendMessage( ghLvItemWnd, WM_MOUSEWHEEL, wParam, lParam );	return 0;

		case WM_CLOSE:	ShowWindow( ghBrTmplWnd , SW_HIDE );	return 0;

		case WMP_BRUSH_TOGGLE:
			if( gbBrushMode )
			{
				SendMessage( ghBrTlBarWnd, TB_SETSTATE, IDM_BRUSH_ON_OFF, TBSTATE_ENABLED );
				gbBrushMode = FALSE;
			}
			else
			{
				SendMessage( ghBrTlBarWnd, TB_SETSTATE, IDM_BRUSH_ON_OFF, (TBSTATE_CHECKED | TBSTATE_ENABLED) );
				gbBrushMode = TRUE;
			}
			ViewBrushStyleSetting( gbBrushMode, NULL );	//	�r���[�E�C���h�E�Ƀ��[�h�t����
			return gbBrushMode;

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
VOID Btp_OnCommand( HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify )
{
	INT			rslt;
	UINT		dClm;
	LRESULT		lRslt;
	LONG_PTR	rdExStyle;
	TCHAR		atItem[SUB_STRING];

	ZeroMemory( atItem, sizeof(atItem) );

	switch( id )
	{
		case IDCB_BT_CATEGORY:	//	�J�e�S���I���R���{�b�N�X
			if( 0 < gvcBrTmpls.size() )
			{
				if( CBN_SELCHANGE == codeNotify )	//	�I��ύX���ꂽ��
				{
					rslt = ComboBox_GetCurSel( ghCtgryBxWnd );
					gNowGroup = rslt;

					BrushTmpleItemListOn( rslt );

					//	Brush����
					gbBrushMode = FALSE;
					SendMessage( ghBrTlBarWnd, TB_SETSTATE, IDM_BRUSH_ON_OFF, TBSTATE_ENABLED );
					StringCchCopy( atItem, SUB_STRING, gvcBrTmpls.at( gNowGroup ).vcItems.at( 0 ).c_str( ) );
					ViewBrushStyleSetting( gbBrushMode, atItem );
				}
			}
			break;

		case IDM_BRUSH_ON_OFF:	//	�����ɗ��鎞�_��ON/OFF�؂�ւ���Ă�
			lRslt = SendMessage( ghBrTlBarWnd, TB_GETSTATE, IDM_BRUSH_ON_OFF, 0 );
			gbBrushMode = (lRslt & TBSTATE_CHECKED) ? TRUE : FALSE;
			ViewBrushStyleSetting( gbBrushMode, NULL );	//	��Ԃ�������
			break;

		case  IDM_WINDOW_CHANGE:	WindowFocusChange( WND_BRUSH,  1 );	break;
		case  IDM_WINDOW_CHG_RVRS:	WindowFocusChange( WND_BRUSH, -1 );	break;

		case IDM_TMPL_GRID_INCREASE:
		case IDM_TMPL_GRID_DECREASE:
			dClm = TemplateGridFluctuate( ghLvItemWnd, ((IDM_TMPL_GRID_INCREASE == id) ? 1 : -1) );
			if( dClm )
			{
				gBrhClmCnt = dClm;
				BrushTmpleItemListOn( gNowGroup );
				InitParamValue( INIT_SAVE, VL_BRUSHTMP_CLM, gBrhClmCnt );
			}
			break;

		case IDM_TMPLT_GROUP_PREV:
			if( 0 < gNowGroup )
			{
				gNowGroup--;
				ComboBox_SetCurSel( ghCtgryBxWnd, gNowGroup );
				BrushTmpleItemListOn( gNowGroup );
			}
			break;

		case IDM_TMPLT_GROUP_NEXT:
			if( (gNowGroup + 1) < gvcBrTmpls.size() )
			{
				gNowGroup++;
				ComboBox_SetCurSel( ghCtgryBxWnd, gNowGroup );
				BrushTmpleItemListOn( gNowGroup );
			}
			break;

		case IDM_TOPMOST_TOGGLE:	//	�펞�őS�ʂƒʏ�E�C���h�E�̃g�O��
			rdExStyle = GetWindowLongPtr( hWnd, GWL_EXSTYLE );
			if( WS_EX_TOPMOST & rdExStyle )
			{
				SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
				InitWindowTopMost( INIT_SAVE, WDP_BRTMPL, 0 );
			}
			else
			{
				SetWindowPos( hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
				InitWindowTopMost( INIT_SAVE, WDP_BRTMPL, 1 );
			}
			break;

		//	�e���v���t�@�C�������[�h
		case IDM_TMPLT_RELOAD:	BrushTmpleItemReload( hWnd );	break;

		default:	break;
	}

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
VOID Btp_OnSize( HWND hWnd, UINT state, INT cx, INT cy )
{
	LONG	width;
	UINT	i;
	RECT	cbxRect, tbrRect, rect;

	if( !(ghBrTlBarWnd) )	return;
	MoveWindow( ghBrTlBarWnd, 0, 0, 0, 0, TRUE );	//	�c�[���o�[�͐��l�Ȃ��Ă�����ɍ��킹�Ă����
	GetClientRect( ghBrTlBarWnd, &tbrRect );

	if( !(ghCtgryBxWnd) )	return;
	MoveWindow( ghCtgryBxWnd, 0, tbrRect.bottom, cx, 127, TRUE );
	GetClientRect( ghCtgryBxWnd, &cbxRect );

	if( !(ghLvItemWnd) )	return;
	MoveWindow( ghLvItemWnd, 0, (cbxRect.bottom + tbrRect.bottom), cx, cy - (cbxRect.bottom + tbrRect.bottom), TRUE );

	GetClientRect( ghLvItemWnd, &rect );
	width = rect.right / gBrhClmCnt;

	for( i = 0; gBrhClmCnt > i; i++ )
	{
		ListView_SetColumnWidth( ghLvItemWnd, i, width );
	}


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
LRESULT Btp_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	HWND	hLvWnd;
	INT		iPos, iItem, nmCode, iSubItem;
	INT_PTR	items;
	TCHAR	atItem[SUB_STRING];
	LPNMLISTVIEW	pstLv;
	LVHITTESTINFO	stHitTestInfo;

	if( IDLV_BT_ITEMVIEW == idFrom )
	{
		pstLv = (LPNMLISTVIEW)pstNmhdr;

		hLvWnd = pstLv->hdr.hwndFrom;
		nmCode = pstLv->hdr.code;

		stHitTestInfo.pt = pstLv->ptAction;
		ListView_SubItemHitTest( hLvWnd, &stHitTestInfo );

		iItem = stHitTestInfo.iItem;
		iSubItem = stHitTestInfo.iSubItem;
		iPos = iItem * gBrhClmCnt + iSubItem;

		//	���ʂ̃N���b�N�ɂ���
		if( NM_CLICK == nmCode )
		{
			TRACE( TEXT("BRUSH TMPL[%d x %d]"), iItem, iSubItem );

			if( 0 < gvcBrTmpls.size() )
			{
				items = gvcBrTmpls.at( gNowGroup ).vcItems.size( );

				if( 0 <= iPos && iPos <  items )	//	�Ȃ񂩑I������
				{
					StringCchCopy( atItem, SUB_STRING, gvcBrTmpls.at( gNowGroup ).vcItems.at( iPos ).c_str( ) );

					//	�u���V�Z�b�g
					gbBrushMode = TRUE;
					SendMessage( ghBrTlBarWnd, TB_SETSTATE, IDM_BRUSH_ON_OFF, (TBSTATE_CHECKED | TBSTATE_ENABLED) );
					//	�r���[�E�C���h�E�Ƀ��[�h�t����
					ViewBrushStyleSetting( gbBrushMode, atItem );

					ViewFocusSet(  );	//	20110720	�t�H�[�J�X��`��ɖ߂�
				}
			}
			else
			{
				ViewFocusSet(  );
			}
		}
	}

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
VOID Btp_OnContextMenu( HWND hWnd, HWND hWndContext, UINT xPos, UINT yPos )
{
	HMENU	hMenu, hSubMenu;
	UINT	dRslt;
	LONG_PTR	rdExStyle;

	POINT	stPoint;

	stPoint.x = (SHORT)xPos;	//	��ʍ��W�̓}�C�i�X�����肤��
	stPoint.y = (SHORT)yPos;

	hMenu = LoadMenu( GetModuleHandle(NULL), MAKEINTRESOURCE(IDM_TEMPLATE_POPUP) );
	hSubMenu = GetSubMenu( hMenu, 0 );

	//	��̉��Ȃ��O�\�����폜
	if( gbTmpltDock ){	DeleteMenu( hSubMenu, IDM_TOPMOST_TOGGLE, MF_BYCOMMAND );	}

	rdExStyle = GetWindowLongPtr( hWnd, GWL_EXSTYLE );
	if( WS_EX_TOPMOST & rdExStyle ){	CheckMenuItem( hSubMenu , IDM_TOPMOST_TOGGLE, MF_BYCOMMAND | MF_CHECKED );	}

	dRslt = TrackPopupMenu( hSubMenu, 0, stPoint.x, stPoint.y, 0, hWnd, NULL );
	//	�I�������łO���|�P�H�A�I�������炻�̃��j���[�̂h�c��WM_COMMAND�����s
	DestroyMenu( hMenu );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Z�b�g���̂ƁA���e���i�ǂ������̂ݗL���j����s�Â󂯎��
	@param[in]	ptName	�Z�b�g���́E�����Ȃ�NULL�E�������̑��ݗD��
	@param[in]	ptLine	���ڂ̓��e�E�����Ȃ�NULL�E����NULL�Ŗ��[����
	@param[in]	cchSize	�ǂ������̓��e�̕�����
	@return		���ɈӖ��͂Ȃ�
*/
UINT CALLBACK BrushTmpleItemData( LPTSTR ptName, LPCTSTR ptLine, INT cchSize )
{
//	����NULL��������A�{�̂ɒǉ�����������
	static AATEMPLATE	cstItem;


	if( ptName )	//	�Z�b�g���́E�J�n�ł�����
	{
		StringCchCopy( cstItem.atCtgryName, SUB_STRING, ptName );
		cstItem.vcItems.clear(  );
	}
	else if( ptLine )	//	�{�̃f�[�^
	{
		cstItem.vcItems.push_back( wstring( ptLine ) );
	}
	else	//	�ǂ�����NULL�A�Z�b�g�I��
	{
		gvcBrTmpls.push_back( cstItem );
	}

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e�������X�g�ɓW�J
	@param[in]	listNum	�W�J�������Z�b�g�ԍ��E�O�C���f�b�N�X
	@return	UINT	�A�C�e���̌�
*/
UINT BrushTmpleItemListOn( UINT listNum )
{
	INT			width;
	UINT_PTR	i, items;
	TCHAR		atItem[SUB_STRING];
	LVITEM		stLvi;
	RECT		rect;

	if( 0 >= gvcBrTmpls.size() ){	return 0;	}

	ZeroMemory( atItem, sizeof(atItem) );

	items = gvcBrTmpls.at( listNum ).vcItems.size( );

	TRACE( TEXT("BRUSH open NUM[%u] ITEM[%u] GRID[%d]"), listNum, items, gBrhClmCnt );

	ListView_DeleteAllItems( ghLvItemWnd );

	ZeroMemory( &stLvi, sizeof(stLvi) );
	stLvi.mask    = LVIF_TEXT;
	stLvi.pszText = atItem;

	for( i = 0; items > i; i++ )
	{
		StringCchCopy( atItem, SUB_STRING, gvcBrTmpls.at( listNum ).vcItems.at( i ).c_str( ) );

		stLvi.iItem    = i / gBrhClmCnt;
		stLvi.iSubItem = i % gBrhClmCnt;
		if( 0 == stLvi.iSubItem )	ListView_InsertItem( ghLvItemWnd, &stLvi );
		else						ListView_SetItem( ghLvItemWnd, &stLvi );
	}

	//	�u�`���񂾂畝����
	GetClientRect( ghLvItemWnd, &rect );
	width = rect.right / gBrhClmCnt;
	for( i = 0; gBrhClmCnt > i; i++ ){	ListView_SetColumnWidth( ghLvItemWnd, i, width );	}

	return items;
}
//-------------------------------------------------------------------------------------------------


/*!
	�A�C�e���t�@�C���œǍ�
	@param[in]	hWnd		�E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT BrushTmpleItemReload( HWND hWnd )
{
	TEMPL_ITR	itTmpl;

	gNowGroup = 0;	//	�Ƃ肠�����O�ɖ߂�

	gbBrushMode = 0;	//	�I�������ċ�ɂ��Ă���
	ViewBrushStyleSetting( gbBrushMode, TEXT("") );

	for( itTmpl = gvcBrTmpls.begin( );  gvcBrTmpls.end( ) != itTmpl; itTmpl++ ){	itTmpl->vcItems.clear();	}
	gvcBrTmpls.clear(  );	//	��U���e�j��
	
	//	�J�e�S���R���{�b�N�X�̒��g��S�j��
	while( ComboBox_GetCount( ghCtgryBxWnd )  ){	ComboBox_DeleteString( ghCtgryBxWnd, 0 );	}

	TemplateItemLoad( AA_BRUSH_FILE, BrushTmpleItemData );	//	�Ăу��[�h

	for( itTmpl = gvcBrTmpls.begin( );  gvcBrTmpls.end( ) != itTmpl; itTmpl++ )
	{
		ComboBox_AddString( ghCtgryBxWnd, itTmpl->atCtgryName );
	}
	ComboBox_SetCurSel( ghCtgryBxWnd, 0 );

	BrushTmpleItemListOn( 0 );	//	�O�ł�\��

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


/*!
	�J�e�S���R���{�b�N�X�T�u�N���X
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	msg		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
*/
LRESULT CALLBACK gpfBrushCtgryProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	INT	id;

	switch( msg )
	{
		case WM_COMMAND:
			id = LOWORD(wParam);
			switch( id )
			{
				case IDM_WINDOW_CHANGE:
				case IDM_WINDOW_CHG_RVRS:
				case IDM_TMPL_GRID_INCREASE:
				case IDM_TMPL_GRID_DECREASE:
				case IDM_TMPLT_GROUP_PREV:
				case IDM_TMPLT_GROUP_NEXT:
					SendMessage( ghBrTmplWnd, WM_COMMAND, wParam, lParam );
					return 0;
			}
			break;

		case WM_MOUSEWHEEL:
			SendMessage( ghLvItemWnd, WM_MOUSEWHEEL, wParam, lParam );
			return 0;
	}

	return CallWindowProc( gpfOrigBrushCtgryProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e�����X�g�r���[�T�u�N���X
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	msg		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
*/
LRESULT CALLBACK gpfBrushItemProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	INT	id;

	switch( msg )
	{
		HANDLE_MSG( hWnd, WM_NOTIFY, Blv_OnNotify );	//	�R�����R���g���[���̌ʃC�x���g

		case WM_COMMAND:
			id = LOWORD(wParam);
			switch( id )
			{
				case IDM_WINDOW_CHANGE:
				case IDM_WINDOW_CHG_RVRS:
				case IDM_TMPL_GRID_INCREASE:
				case IDM_TMPL_GRID_DECREASE:
				case IDM_TMPLT_GROUP_PREV:
				case IDM_TMPLT_GROUP_NEXT:
					SendMessage( ghBrTmplWnd, WM_COMMAND, wParam, lParam );
					return 0;
			}
			break;
	}

	return CallWindowProc( gpfOrigBrushItemProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	�m�[�e�B�t�@�C���b�Z�[�W�̏���
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	idFrom		NOTIFY�𔭐��������R���g���[���̂h�c
	@param[in]	pstNmhdr	NOTIFY�̏ڍ�
	@return		�����������e�Ƃ�
*/
LRESULT Blv_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	HWND	hLvWnd;
	INT		iPos, iItem, nmCode, iSubItem, iDot;
	INT_PTR	items;
	TCHAR	atItem[SUB_STRING];
	LVHITTESTINFO	stHitTestInfo;
	LPNMLISTVIEW	pstLv;
	LPNMTTDISPINFO	pstDispInfo;

	pstLv = (LPNMLISTVIEW)pstNmhdr;

	//	���X�g�r���[���̂̃v���V�[�W���Ȃ̂�
	hLvWnd = hWnd;
	nmCode = pstLv->hdr.code;

	if( TTN_GETDISPINFO == nmCode )
	{
		if( IDLV_BT_ITEMVIEW == idFrom )
		{
			ZeroMemory( &stHitTestInfo, sizeof(LVHITTESTINFO) );
			GetCursorPos( &(stHitTestInfo.pt) );
			ScreenToClient( hLvWnd, &(stHitTestInfo.pt) );
			ListView_SubItemHitTest( hLvWnd, &stHitTestInfo );

			iItem = stHitTestInfo.iItem;
			iSubItem = stHitTestInfo.iSubItem;
			iPos = iItem * gBrhClmCnt + iSubItem;

			TRACE( TEXT("BLvTT[%d]"), iPos );

			pstDispInfo = (LPNMTTDISPINFO)pstNmhdr;

			ZeroMemory( &(pstDispInfo->szText), sizeof(pstDispInfo->szText) );
			pstDispInfo->lpszText = pstDispInfo->szText;

			if( 0 < gvcBrTmpls.size( ) )
			{
				items = gvcBrTmpls.at( gNowGroup ).vcItems.size( );

				if( 0 <= iPos && iPos <  items )	//	�Ȃ񂩑I������
				{
					StringCchCopy( atItem, SUB_STRING, gvcBrTmpls.at( gNowGroup ).vcItems.at( iPos ).c_str( ) );
					iDot = ViewStringWidthGet( atItem );

					StringCchPrintf( pstDispInfo->szText, 80, TEXT("%s [%d Dot]"), atItem, iDot );
				}
			}

			return 0;
		}
	}

	return CallWindowProc( gpfOrigBrushItemProc, hWnd, WM_NOTIFY, (WPARAM)idFrom, (LPARAM)pstNmhdr );
}
//-------------------------------------------------------------------------------------------------
