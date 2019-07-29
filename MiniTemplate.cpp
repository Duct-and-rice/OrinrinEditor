/*! @file
	@brief ���^�l�`�`�e���v���[�g�̖ʓ|���܂�
	���̃t�@�C���� MiniTemplate.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/08/29
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

#ifdef MINI_TEMPLATE

#define MINITEMPLATE_CLASS	TEXT("MINI_TEMPLATE")
#define MT_WIDTH	240
#define MT_HEIGHT	240
//-------------------------------------------------------------------------------------------------


extern HFONT	ghAaFont;		//	AA�p�t�H���g

extern INT		gbTmpltDock;	//	�e���v���̃h�b�L���O
extern BOOLEAN	gbDockTmplView;	//	�������Ă�e���v���͌����Ă��邩

extern  HWND	ghMainSplitWnd;	//	���C���̃X�v���b�g�o�[�n���h��
extern  LONG	grdSplitPos;	//	�X�v���b�g�o�[�́A�����́A��ʉE����̃I�t�Z�b�g


static  ATOM	gMnTmplAtom;	//!<	
static  HWND	ghMnTmplWnd;	//!<	���^MAA�e���v���{�̃E�C���h�E
static  HWND	ghTitleBxWnd;	//!<	���ڃR���{�b�N�X
static  HWND	ghItemStcWnd;	//!<	���e�X�^�e�B�b�N�E�I�[�i�[�h���[�Ń���
//	�c�[���`�b�v��HoverTip�Ŏ��O�ŕ`�ʃZ��

static  HWND	ghMainWnd;		//!<	�ҏW�r���[�̂���{�̃E�C���h�E

static WNDPROC	gpfOrigMmaaTitleProc;	//!<	
static WNDPROC	gpfOrigMmaaItemProc;	//!<	

static  vector<AAMATRIX>	gvcMmaaTmpls;	//!<	�e���v���̕ێ�
//-------------------------------------------------------------------------------------------------


LRESULT	CALLBACK MmaaTmpleProc( HWND, UINT, WPARAM, LPARAM );
VOID	Mma_OnCommand( HWND, INT, HWND, UINT );
VOID	Mma_OnSize( HWND, UINT, INT, INT );
LRESULT	Mma_OnNotify( HWND, INT, LPNMHDR );
VOID	Mma_OnContextMenu( HWND, HWND, UINT, UINT );

UINT	CALLBACK MmaaTmpleItemData( LPTSTR, LPCTSTR, INT );

UINT	MmaaTmpleItemListOn( UINT );
HRESULT	MmaaTmpleItemReload( HWND );

LRESULT	CALLBACK gpfMmaaTitleProc( HWND, UINT, WPARAM, LPARAM );
LRESULT	CALLBACK gpfMmaaItemProc(  HWND, UINT, WPARAM, LPARAM );
LRESULT	Mlv_OnNotify( HWND, INT, LPNMHDR );
//-------------------------------------------------------------------------------------------------


/*!
	���^�l�`�`�e���v���E�C���h�E�̍쐬
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

	INT		spPos;


	WNDCLASSEX	wcex;
	RECT		wdRect, clRect, rect, cbxRect, tbRect, mtbRect;
	LVCOLUMN	stLvColm;

	//	�j��
	if( !(hInstance) && !(hParentWnd) )
	{

		return NULL;
	}

	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= MmaaTmpleProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= MINITEMPLATE_CLASS;
	wcex.hIconSm		= NULL;

	gMnTmplAtom = RegisterClassEx( &wcex );

	ghMainWnd = hParentWnd;

//�e���v���f�[�^�ǂݏo��
//	TemplateItemLoad( AA_BRUSH_FILE, BrushTmpleItemData );


	InitWindowPos( INIT_LOAD, WDP_MMAATPL, &rect );
	if( 0 == rect.right || 0 == rect.bottom )	//	���������O�̓f�[�^����
	{
		GetWindowRect( hParentWnd, &wdRect );
		rect.left   = wdRect.right + 64;
		rect.top    = wdRect.top + 64;
		rect.right  = MT_WIDTH;
		rect.bottom = MT_HEIGHT;
		InitWindowPos( INIT_SAVE , WDP_MMAATPL, &rect );	//	�N�����ۑ�
	}

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
		if( InitWindowTopMost( INIT_LOAD, WDP_MMAATPL, 0 ) ){	dwExStyle |=  WS_EX_TOPMOST;	}
		dwStyle = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_VISIBLE | WS_SYSMENU;
	}

	//	�{��
	ghMnTmplWnd = CreateWindowEx( dwExStyle, MINITEMPLATE_CLASS, TEXT("Mimi Maa Template"),
		dwStyle, rect.left, rect.top, rect.right, rect.bottom, hPrWnd, NULL, hInstance, NULL);

	GetClientRect( ghMnTmplWnd, &clRect );

	//	���ږ��R���{�b�N�X
	ghTitleBxWnd = CreateWindowEx( 0, WC_COMBOBOX, TEXT("Mmaa Items"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST,
		0, 0, clRect.right, 127, ghMnTmplWnd,
		(HMENU)IDCB_MT_CATEGORY, hInstance, NULL );

	gpfOrigMmaaTitleProc = SubclassWindow( ghTitleBxWnd, gpfMmaaTitleProc );

	//	���ڃ^�C�g���E�l�`�`���Ɠ������
	//dItems = gvcMmaaTmpls.size( );
	//for( i = 0; dItems > i; i++ )
	//{
	//	ComboBox_AddString( ghTitleBxWnd, gvcBrTmpls.at( i ).atCtgryName );
	//}
	//ComboBox_SetCurSel( ghTitleBxWnd, 0 );


	GetClientRect( ghTitleBxWnd, &cbxRect );

	ghItemStcWnd = CreateWindowEx( WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES, WC_STATIC, TEXT(""),
		WS_VISIBLE | WS_CHILD | SS_OWNERDRAW | SS_NOTIFY,
		0, cbxRect.bottom, clRect.right, clRect.bottom - cbxRect.bottom,
		ghMnTmplWnd, (HMENU)IDLV_MT_ITEMSTATIC, hInstance, NULL );
	SetWindowFont( ghItemStcWnd, ghAaFont, TRUE );	//	�I�[�i�[�h���[���邩��s�v


	gpfOrigMmaaItemProc = SubclassWindow( ghItemStcWnd, gpfMmaaItemProc );

	BrushTmpleItemListOn( 0 );	//	���g�ǉ�

	if( !(gbTmpltDock) )
	{
		ShowWindow( ghMnTmplWnd, SW_SHOW );
		UpdateWindow( ghMnTmplWnd );
	}

	return ghMnTmplWnd;
}
//-------------------------------------------------------------------------------------------------

/*!
	�h�b�L���O��ԂŔ����E�������Ă�E�C���h�E�����T�C�Y���ꂽ��
	@param[in]	hPrntWnd	�������Ă�E�C���h�E�n���h��
	@param[in]	pstFrame	�g����T�C�Y
*/
VOID MmaaTmpleResize( HWND hPrntWnd, LPRECT pstFrame )
{
	RECT	rect, tbRect;
	//INT	iHei, iTak;

	//gbTmpltDock
	if( !(ghMnTmplWnd) )	return;

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

	SetWindowPos( ghMnTmplWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_NOZORDER );

	return;
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
LRESULT CALLBACK MmaaTmpleProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		HANDLE_MSG( hWnd, WM_SIZE,        Mma_OnSize );	
		HANDLE_MSG( hWnd, WM_COMMAND,     Mma_OnCommand );	
		HANDLE_MSG( hWnd, WM_NOTIFY,      Mma_OnNotify );	//	�R�����R���g���[���̌ʃC�x���g
		HANDLE_MSG( hWnd, WM_CONTEXTMENU, Mma_OnContextMenu );

		case WM_MOUSEWHEEL:	SendMessage( ghItemStcWnd, WM_MOUSEWHEEL, wParam, lParam );	return 0;

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
#error ��肩��
	switch( id )
	{
		case IDCB_MT_CATEGORY:	//	�J�e�S���I���R���{�b�N�X
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

		case  IDM_WINDOW_CHANGE:	WindowFocusChange( WND_MMAA,  1 );	break;
		case  IDM_WINDOW_CHG_RVRS:	WindowFocusChange( WND_MMAA, -1 );	break;

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
	AA�\���X�^�e�B�b�N�̃T�u�N���X�v���V�[�W���E�c�[���`�b�v�̏����ɕK�v
	@param[in]	hWnd	���X�g�̃n���h��
	@param[in]	msg		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@return		LRESULT	�������ʂƂ�
*/
LRESULT CALLBACK gpfMmaaItemProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
//		HANDLE_MSG( hWnd, WM_CHAR,        Mai_OnChar );
//		HANDLE_MSG( hWnd, WM_KEYDOWN,     Mai_OnKey );			//	
//		HANDLE_MSG( hWnd, WM_KEYUP,       Mai_OnKey );			//	

		HANDLE_MSG( hWnd, WM_MOUSEMOVE,   Mai_OnMouseMove );	//	�}�E�X��������
		HANDLE_MSG( hWnd, WM_LBUTTONUP,   Mai_OnLButtonUp );	//	�}�E�X���{�^������
//		HANDLE_MSG( hWnd, WM_MBUTTONUP,   Mai_OnMButtonUp );	//	�}�E�X���{�^������
//		HANDLE_MSG( hWnd, WM_CONTEXTMENU, Mai_OnContextMenu );	//	�R���e�L�X�g���j���[����
		HANDLE_MSG( hWnd, WM_DROPFILES,   Mai_OnDropFiles );	//	�h���O���h���b�v�̎�t

#ifdef USE_HOVERTIP
		case WM_MOUSEHOVER:
			HoverTipOnMouseHover( hWnd, wParam, lParam, MmaaItemsHoverTipInfo );
			return 0;

		case WM_MOUSELEAVE:
			HoverTipOnMouseLeave( hWnd );
			gixNowToolTip = -1;
			return 0;
#endif


		default:	break;
	}

	return CallWindowProc( gpfOrigMmaaItemProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------



#endif	//	MINI_TEMPLATE

