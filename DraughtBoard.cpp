/*! @file
	@brief �h���t�g�{�[�h�̊Ǘ����܂�
	���̃t�@�C���� DraughtBoard.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/11/30
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

/*
�c�[���`�b�v�̕\����\���ƕ����T�C�Y�͂l�`�`�ɏ]��

�\���T�C�Y�ύX


*/

#include "stdafx.h"
#include "OrinrinEditor.h"

#include "MaaTemplate.h"
//-------------------------------------------------------------------------------------------------


#define DRAUGHT_BOARD_CLASS	TEXT("DRAUGHT_BOARD")

#define TTMSG_NO_ITEM	TEXT("NO ITEM")

//	�ꖇ�̃p�l���T�C�Y
//#define THM_WIDTH	128
//#define THM_HEIGHT	128

//	�T�C�Y�ύX
#define DTHMSZ_ULTRALIGHT	 80
#define DTHMSZ_REGULAR		128
#define DTHMSZ_DEMIBOLD		160
#define DTHMSZ_ULTRABOLD	192

//	�p�l���͂T���R�ɕ��ׂ�
#define TPNL_HORIZ	5
#define TPNL_VERTI	3
//-------------------------------------------------------------------------------------------------


//	�g�p����\���̂�MAA�̂Ƌ��ʂł�����
//-------------------------------------------------------------------------------------------------

extern HFONT	ghAaFont;		//	AA�p�t�H���g

#ifdef MAA_TOOLTIP
extern HFONT	ghTipFont;		//	�c�[���`�b�v�p

static  HWND	ghDrghtTipWnd;	//!<	�c�[���`�b�v
static LPTSTR	gptTipBuffer;	//!<	�`�b�v���e
#endif

extern  UINT	gbAAtipView;	//	��O�ŁA�`�`�c�[���`�b�v�\��

static  HWND	ghPtWnd;

static  ATOM	gDraughtAtom;	//!<	�E�C���h�E�N���X�A�g��
static  HWND	ghDraughtWnd;	//!<	���̃E�C���h�E�n���h��


EXTERNED UINT	gdClickDrt;		//!<	�A�C�e�������N���b�N�����Ƃ��̊�{����E�O�ʏ�}���@�P��`�}���@�Q���C���{�b�N�X�J���@�RUNI�N���b�v�@�SSJIS�N���b�v
EXTERNED UINT	gdSubClickDrt;	//!<	�A�C�e���𒆃N���b�N�����Ƃ��̊�{����E�O�ʏ�}���@�P��`�}���@�Q���C���{�b�N�X�J���@�RUNI�N���b�v�@�SSJIS�N���b�v
//�N���b�y�{�[�h�փR�s�郂�[�h�̓R�s�[���[�h�X���b�v�ɏ]��

static HDC		ghNonItemDC;	//!<	�A�C�e�������̊G
static HBITMAP	ghNonItemBMP, ghOldBmp;	
static HPEN		ghLinePen;

static HFONT	ghAreaFont;		//!<	�T�C�Y�\���p

static INT		giItemSel;		//!<	�}�E�X�J�[�\�����ɂ���g�ԍ�
static INT		giTarget;		//!<	�N���b�N�����A�C�e���ԍ��E�|�P�Ŗ���

static  UINT	gbThumb;		//!<	�T���l��Ԃł��邩
static  LONG	gdVwTop;		//!<	�\������Ă��ԍ���̍s�ԍ��O�C���f�b�N�X
static  HWND	ghScrBarWnd;	//!<	�T���l�p�X�N���[���o�@�[

static INT		giItemWidth;	//!<	�A�C�e���̕�
static INT		giItemHeight;	//!<	�A�C�e���̍���

//static POINT	gstMainLsPt;	//!<	���C������J�����ꍇ�̍ŏI�ʒu
static POINT	gstViewLsPt;	//!<	�l�`�`����J�����ꍇ�̍ŏI�ʒu


static vector<AAMATRIX>	gvcDrtItems;	//!<	
//-------------------------------------------------------------------------------------------------

INT		DraughtTargetItemSet( LPPOINT );				//!<	
DOUBLE	DraughtAspectKeeping( LPSIZE, UINT );			//!<	
INT		DraughtItemDelete( CONST INT  );				//!<	
HRESULT	DraughtItemUse( HWND, INT );					//!<	
HRESULT	DraughtItemExport( HWND, LPTSTR );				//!<	
VOID	DraughtButtonUp( HWND, INT, INT, UINT, UINT );	//!<	
HRESULT	DraughtFrameResize( HWND, INT, INT );

LRESULT CALLBACK DraughtProc( HWND, UINT, WPARAM, LPARAM );
VOID	Drt_OnCommand( HWND , INT, HWND, UINT );		//!<	
VOID	Drt_OnPaint( HWND );							//!<	
//VOID	Drt_OnSize( HWND , UINT, INT, INT );			//!<	
VOID	Drt_OnMouseMove( HWND, INT, INT, UINT );		//!<	
VOID	Drt_OnLButtonUp( HWND, INT, INT, UINT );		//!<	
VOID	Drt_OnMButtonUp( HWND, INT, INT, UINT );		//!<	
VOID	Drt_OnContextMenu( HWND, HWND, UINT, UINT );	//!<	
VOID	Drt_OnDestroy( HWND );							//!<	
VOID	Drt_OnKillFocus( HWND, HWND );					//!<	
VOID	Drt_OnVScroll( HWND , HWND, UINT, INT );		//!<	
VOID	Drt_OnMouseWheel( HWND, INT, INT, INT, UINT );	//!<	
#ifdef MAA_TOOLTIP
LRESULT	Drt_OnNotify( HWND , INT, LPNMHDR );			//!<	
#endif

#ifdef USE_HOVERTIP
LPTSTR	CALLBACK DraughtHoverTipInfo( LPVOID  );		//!<	HoverTip�p�̃R�[���o�b�N���
#endif
//-------------------------------------------------------------------------------------------------

/*!
	�e���|������AA��\������E�C���h�E�̍쐬
	@param[in]	hInstance	�A�v���̃C���X�^���X
	@param[in]	hPtWnd		���C�����E�C���h�E�n���h��
	@return	�I����ԃR�[�h
*/
HRESULT DraughtInitialise( HINSTANCE hInstance, HWND hPtWnd )
{
	WNDCLASSEX	wcex;


	if( hInstance )
	{
		ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
		wcex.cbSize			= sizeof(WNDCLASSEX);
		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= DraughtProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIcon			= NULL;
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= DRAUGHT_BOARD_CLASS;
		wcex.hIconSm		= NULL;

		gDraughtAtom = RegisterClassEx( &wcex );

		ghNonItemDC = NULL;

		//	�T�C�Y����
		giItemWidth  = InitParamValue( INIT_LOAD, VL_THUMB_HORIZ, DTHMSZ_REGULAR );
		giItemHeight = InitParamValue( INIT_LOAD, VL_THUMB_VERTI, DTHMSZ_REGULAR );


		//	�T�C�Y�\���p�t�H���g
		ghAreaFont = CreateFont( FONTSZ_REDUCE, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH, TEXT("MS UI Gothic") );

		ghPtWnd = hPtWnd;

		//	�������
		//gstMainLsPt.x = -1;
		gstViewLsPt.x = -1;

		giItemSel = -1;

#ifndef _ORRVW
		//	�N���b�N����w�胍�[�h�E�f�t�H����͒ʏ�}��
		gdClickDrt    = InitParamValue( INIT_LOAD, VL_DRT_LCLICK, MAA_INSERT );
		gdSubClickDrt = InitParamValue( INIT_LOAD, VL_DRT_MCLICK, MAA_INSERT );
		//	Viewer�̏ꍇ�̓R�s�[���[�h�ɏ]��
#endif
	}
	else
	{
		if( ghNonItemDC )
		{
			SelectBitmap( ghNonItemDC, ghOldBmp );
			SelectPen( ghNonItemDC, GetStockPen(NULL_PEN) );
			DeleteObject( ghNonItemDC );
		}
		if( ghNonItemBMP  ){	DeleteBitmap( ghNonItemBMP );	}
		if( ghLinePen ){	DeletePen( ghLinePen  );	}
		if( ghAreaFont ){	DeleteFont( ghAreaFont );	}

#ifdef MAA_TOOLTIP
		FREE( gptTipBuffer );
#endif
		DraughtItemDelete( -1 );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


/*!
	�\���p�E�C���h�E�����
	@param[in]	hInstance	�A�v���̃C���X�^���X
	@param[in]	hPtWnd		�Ăяo�������̃E�C���h�E�n���h��
	@param[in]	bThumb		��OMAA�̃T���l�\���Ƃ��ČĂ΂ꂽ
	@return	������E�C���h�E�̃n���h��
*/
HWND DraughtWindowCreate( HINSTANCE hInstance, HWND hPtWnd, UINT bThumb )
{
	INT_PTR	iItems;
	INT		iRslt, iScWid = 0, iScHei;
	HDC		hdc;

	INT		iBrdrWid = 0;
	TCHAR	atCaption[SUB_STRING];

	RECT	wdRect, rect;
#ifdef MAA_TOOLTIP
	TTTOOLINFO	stToolInfo;
#endif

	INT		iCapHei, iXfrm, iYfrm;
	INT		iLines, iStep = 0;
	LONG	rigOffs = 0;
	SCROLLINFO	stScrollInfo;

	if( !(hPtWnd ) )	//	�j�󂷂�E����Ȃ��H
	{
		return NULL;
	}

	//	�߂ɏo���Ă����炻�̂܂ܕԂ�
	if( ghDraughtWnd  ){	UpdateWindow( ghDraughtWnd );	return ghDraughtWnd;	}

	gbThumb = bThumb;

	iItems = gvcDrtItems.size( );	//	���݌��E�����ł͎g��Ȃ�


	iCapHei = GetSystemMetrics( SM_CYSMCAPTION );
	iXfrm   = GetSystemMetrics( SM_CXFIXEDFRAME );
	iYfrm   = GetSystemMetrics( SM_CYFIXEDFRAME );

	if( 0 >  gstViewLsPt.x )	//	���ݒ�Ȃ�
	{
		GetWindowRect( hPtWnd, &wdRect );
		rect.left   = wdRect.left + 32;	//	�I�t�Z�b�g�l�ɓ��ɈӖ��͂Ȃ�
		rect.top    = wdRect.top  + 32;
	
		gstViewLsPt.x = rect.left;
		gstViewLsPt.y = rect.top;
	}
	else
	{
		rect.left   = gstViewLsPt.x;
		rect.top    = gstViewLsPt.y;
	}
	rect.right  = (giItemWidth  * TPNL_HORIZ) + (iXfrm * 2);
	rect.bottom = (giItemHeight * TPNL_VERTI);
	iScHei = rect.bottom;
	rect.bottom += ((iYfrm * 2) + iCapHei);


//	if( ghPtWnd == hPtWnd )	�Ăт��������ɂ���āA���X�g�ʒu�����[�h�E��܂ł��Ȃ��ėǂ���


	if( gbThumb )	//	�T���l���[�h
	{
		gdVwTop = 0;

		iItems = AacItemCount( 0  );	//	���݌��E�X�N���[���o�[�̍��ݐݒ�ɂ���
		if( 0 >= iItems )	return NULL;	//	�A�C�e���J���ĂȂ��Ȃ牽�����Ȃ�

		iLines = (iItems + (TPNL_HORIZ-1)) / TPNL_HORIZ;	//	�s���E�؂�グ����

		iStep = iLines - TPNL_VERTI;	//	�������΁`�̍��ݐ�
		if( 0 > iStep ){	iStep = 0;	}

		rigOffs = rect.right;

		iScWid = GetSystemMetrics( SM_CXVSCROLL );	//	�����X�N���[���o�[�̕��m��
		rect.right += iScWid;

		iBrdrWid = GetSystemMetrics( SM_CXFIXEDFRAME );	//	�g�̕��m��
		rect.right += (iBrdrWid*2);

		StringCchCopy( atCaption, SUB_STRING, TEXT("MAA THUMBNAIL") );
	}
	else
	{
		StringCchCopy( atCaption, SUB_STRING, TEXT("DRAUGHT BOARD") );
	}

	//	�E�C���h�E�쐬	TOPMOST���邩�H	�v��
	ghDraughtWnd = CreateWindowEx( WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
		DRAUGHT_BOARD_CLASS, atCaption, WS_POPUP | WS_VISIBLE | WS_CAPTION,
		rect.left, rect.top, rect.right, rect.bottom, NULL, NULL, hInstance, NULL );

#ifdef MAA_TOOLTIP
	FREE( gptTipBuffer );

	//	�c�[���`�b�v
	ghDrghtTipWnd = CreateWindowEx( WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, ghDraughtWnd, NULL, hInstance, NULL );
	SetWindowFont( ghDrghtTipWnd, ghTipFont, TRUE );

	//	�c�[���`�b�v���R�[���o�b�N�Ŋ���t��
	ZeroMemory( &stToolInfo, sizeof(TTTOOLINFO) );
	GetClientRect( ghDraughtWnd, &stToolInfo.rect );
	stToolInfo.cbSize   = sizeof(TTTOOLINFO);
	stToolInfo.uFlags   = TTF_SUBCLASS;
	stToolInfo.hinst    = NULL;	//	
	stToolInfo.hwnd     = ghDraughtWnd;
	stToolInfo.uId      = IDTT_DRT_TOOLTIP;
	stToolInfo.lpszText = LPSTR_TEXTCALLBACK;	//	�R�����w�肷��ƃR�[���o�b�N�ɂȂ�
	SendMessage( ghDrghtTipWnd, TTM_ADDTOOL, 0, (LPARAM)&stToolInfo );
	SendMessage( ghDrghtTipWnd, TTM_SETMAXTIPWIDTH, 0 , 0 );	//	�`�b�v�̕��B�O�ݒ�ł����B���ꂵ�Ƃ��Ȃ��Ɖ��s����Ȃ�
#endif

	if( gbThumb )	//	�T���l���[�h
	{
		//	�ꗗ�̃X�N���[���o�[
		ghScrBarWnd = CreateWindowEx( 0, WC_SCROLLBAR, TEXT("scroll"), WS_VISIBLE | WS_CHILD | SBS_VERT,
			rigOffs, 0, iScWid, iScHei, ghDraughtWnd, (HMENU)IDSB_DRT_THUM_SCROLL, hInstance, NULL );

		ZeroMemory( &stScrollInfo, sizeof(SCROLLINFO) );
		stScrollInfo.cbSize = sizeof(SCROLLINFO);
		stScrollInfo.fMask = SIF_ALL | SIF_DISABLENOSCROLL;
		stScrollInfo.nMax  = iStep;
		stScrollInfo.nPos  = 0;
		stScrollInfo.nPage = 1;
		stScrollInfo.nTrackPos = 0;
		SetScrollInfo( ghScrBarWnd, SB_CTL, &stScrollInfo, TRUE );
	}

	if( !(ghNonItemDC) )	//	�����ߕ`��p�r�b�g�}�b�v�쐬
	{
		hdc = GetDC( ghDraughtWnd );

		ghNonItemDC  = CreateCompatibleDC( hdc );
		ghNonItemBMP = CreateCompatibleBitmap( hdc, giItemWidth, giItemHeight );

		ghOldBmp = SelectBitmap( ghNonItemDC, ghNonItemBMP );
		SelectFont( ghNonItemDC, ghAaFont );

		iRslt = PatBlt( ghNonItemDC, 0, 0, giItemWidth, giItemHeight, WHITENESS );

		ReleaseDC( ghDraughtWnd, hdc );

		SetRect( &rect, 0, 0, giItemWidth, giItemHeight );
		iRslt = DrawText( ghNonItemDC, TEXT("NO ITEM"), 7, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_NOCLIP | DT_SINGLELINE );

		SelectFont( ghNonItemDC, GetStockFont(DEFAULT_GUI_FONT) );

		ghLinePen = CreatePen( PS_SOLID, 1, 0 );
		SelectPen( ghNonItemDC, ghLinePen );
	}

	UpdateWindow( ghDraughtWnd );

	return ghDraughtWnd;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�v���V�[�W��
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	iWidth	�V�����A�C�e����
	@param[in]	iHeight	�V�����A�C�e������
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT DraughtFrameResize( HWND hWnd, INT iWidth, INT iHeight )
{
	INT		iBrdrWid = 0;
	INT		iScWid = 0, iScHei;
	INT		iCapHei, iXfrm, iYfrm;
	LONG	rigOffs = 0;
	RECT	rect;

	giItemWidth  = iWidth;
	giItemHeight = iHeight;

	iCapHei = GetSystemMetrics( SM_CYSMCAPTION );
	iXfrm   = GetSystemMetrics( SM_CXFIXEDFRAME );
	iYfrm   = GetSystemMetrics( SM_CYFIXEDFRAME );

	rect.left   = gstViewLsPt.x;
	rect.top    = gstViewLsPt.y;
	rect.right  = (giItemWidth  * TPNL_HORIZ) + (iXfrm * 2);
	rect.bottom = (giItemHeight * TPNL_VERTI);
	iScHei      = rect.bottom;
	rect.bottom += ((iYfrm * 2) + iCapHei);

	if( gbThumb )	//	�T���l���[�h
	{
		rigOffs = rect.right;

		iScWid = GetSystemMetrics( SM_CXVSCROLL );	//	�����X�N���[���o�[�̕��m��
		rect.right += iScWid;

		iBrdrWid = GetSystemMetrics( SM_CXFIXEDFRAME );	//	�g�̕��m��
		rect.right += (iBrdrWid*2);
	}

	//	�E�C���h�E�T�C�Y�ύX
	SetWindowPos( ghDraughtWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_NOMOVE );

	if( gbThumb )	//	�T���l���[�h�E�X�N���[���o�[�̈ʒu�ύX
	{
		SetWindowPos( ghScrBarWnd, HWND_TOP, rigOffs, 0, iScWid, iScHei, SWP_NOZORDER );
	}

	InvalidateRect( ghDraughtWnd, NULL, TRUE );

	//	�L�^
	InitParamValue( INIT_SAVE, VL_THUMB_HORIZ, giItemWidth );
	InitParamValue( INIT_SAVE, VL_THUMB_VERTI, giItemHeight );

	return S_OK;
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
LRESULT CALLBACK DraughtProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	//	HANDLE_MSG( hWnd, WM_SIZE,        Drt_OnSize );	
		HANDLE_MSG( hWnd, WM_COMMAND,     Drt_OnCommand );	
		HANDLE_MSG( hWnd, WM_MOUSEMOVE,   Drt_OnMouseMove );	//	�}�E�X��������
		HANDLE_MSG( hWnd, WM_LBUTTONUP,   Drt_OnLButtonUp );
		HANDLE_MSG( hWnd, WM_MBUTTONUP,   Drt_OnMButtonUp );
		HANDLE_MSG( hWnd, WM_PAINT,       Drt_OnPaint );
		HANDLE_MSG( hWnd, WM_CONTEXTMENU, Drt_OnContextMenu );	//	�E�N�����j���[
		HANDLE_MSG( hWnd, WM_DESTROY,     Drt_OnDestroy );
		HANDLE_MSG( hWnd, WM_KILLFOCUS,   Drt_OnKillFocus );	//	�t�H�[�J�X��������
		HANDLE_MSG( hWnd, WM_VSCROLL,     Drt_OnVScroll );		//	�c�X�N���[���֘A
		HANDLE_MSG( hWnd, WM_MOUSEWHEEL,  Drt_OnMouseWheel );	//	�}�E�X�z�E�B�[��
#ifdef MAA_TOOLTIP
		HANDLE_MSG( hWnd, WM_NOTIFY,      Drt_OnNotify );	//	�R�����R���g���[���̌ʃC�x���g
#endif

#ifdef USE_HOVERTIP
		case WM_MOUSEHOVER:
			HoverTipOnMouseHover( hWnd, wParam, lParam, DraughtHoverTipInfo );
			return 0;

		case WM_MOUSELEAVE:
			HoverTipOnMouseLeave( hWnd );
			giItemSel = -1;
			return 0;
#endif


//		case WM_CLOSE:	ShowWindow( ghDraughtWnd, SW_HIDE );	return 0;

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
VOID Drt_OnCommand( HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify )
{
	
	switch( id )
	{
#ifndef _ORRVW
		case IDM_DRAUGHT_INSERTEDIT:
		case IDM_DRAUGHT_INTERRUPTEDIT:
		case IDM_DRAUGHT_LAYERBOX:
#endif
		case IDM_DRAUGHT_UNICLIP:
		case IDM_DRAUGHT_SJISCLIP:		DraughtItemUse( hWnd , id );	DestroyWindow( hWnd );	break;

		case IDM_THUMB_DRAUGHT_ADD:		DraughtItemUse( hWnd , id );	break;	//	Draught�ǉ��Ȃ���Ȃ������������낤

		case IDM_DRAUGHT_DELETE:		DraughtItemDelete( giTarget );	InvalidateRect( hWnd , NULL, TRUE );	break;
		case IDM_DRAUGHT_ALLDELETE:		DraughtItemDelete( -1 );	DestroyWindow( hWnd );	break;

		case IDM_DB_THUMB_ULTRALIGHT:	DraughtFrameResize( hWnd, DTHMSZ_ULTRALIGHT, DTHMSZ_ULTRALIGHT );	break;
		case IDM_DB_THUMB_REGULAR:		DraughtFrameResize( hWnd, DTHMSZ_REGULAR,    DTHMSZ_REGULAR );	break;
		case IDM_DB_THUMB_DEMIBOLD:		DraughtFrameResize( hWnd, DTHMSZ_DEMIBOLD,   DTHMSZ_DEMIBOLD );	break;
		case IDM_DB_THUMB_ULTRABOLD:	DraughtFrameResize( hWnd, DTHMSZ_ULTRABOLD,  DTHMSZ_ULTRABOLD );	break;

		case IDM_DRAUGHT_EXPORT:		DraughtItemExport( hWnd, NULL );	break;
		case IDM_DRAUGHT_CLOSE:			DestroyWindow( hWnd );	break;

		default:	break;
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	PAINT�B�����̈悪�o�����Ƃ��ɔ����B�w�i�̈����ɒ��ӁB�w�i��h��Ԃ��Ă���A�I�u�W�F�N�g��`��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Drt_OnPaint( HWND hWnd )
{
	PAINTSTRUCT	ps;
	HDC			hdc, hAaDC;
	HBITMAP		hOldBmp, hBmp;
	HFONT		hOldFnt;
	UINT		x = 0, y = 0;
	INT_PTR		iItems;
	UINT_PTR	cchLen;
	TCHAR		atArea[MIN_STRING];
	SIZE		stSize, stOrgSize, stArea;

	MAAM_ITR	itItem;


	hdc = BeginPaint( hWnd, &ps );

	hOldFnt = SelectFont( hdc, ghAreaFont );

	SetStretchBltMode( hdc, HALFTONE );

	if( gbThumb )	//	�T���l���[�h
	{
		iItems = gdVwTop * TPNL_HORIZ;

		for( y = 0; TPNL_VERTI > y; y++ )
		{
			for( x = 0; TPNL_HORIZ > x; x++ )
			{
				ZeroMemory( atArea, sizeof(atArea) );

				hBmp = AacArtImageGet( hWnd, iItems, &stSize, &stArea );
				if( hBmp )
				{
					stOrgSize = stSize;
					DraughtAspectKeeping( &stSize, TRUE );

					hAaDC = CreateCompatibleDC( hdc );
					hOldBmp = SelectBitmap( hAaDC, hBmp );

					StretchBlt( hdc, (x * giItemWidth), (y * giItemHeight), stSize.cx, stSize.cy,	//	�R�s�[��c�b�A����w�x�A���A����
						hAaDC, 0, 0, stOrgSize.cx, stOrgSize.cy,	//	�R�s�[���c�b�A����w�x�A���A����
						SRCCOPY );	//	���X�^�I�y���[�V�����R�[�h

					SelectBitmap( hAaDC, hOldBmp );

					StringCchPrintf( atArea, MIN_STRING, TEXT("%dDOT x %dLINE"), stArea.cx, stArea.cy );
					StringCchLength( atArea, MIN_STRING, &cchLen );
					ExtTextOut( hdc, (x * giItemWidth)+1, ((y+1) * giItemHeight)-12, 0, NULL, atArea, cchLen, NULL );

					DeleteDC( hAaDC );

					iItems++;
				}
				else
				{
					BitBlt( hdc, (x * giItemWidth), (y * giItemHeight), giItemWidth, giItemHeight, ghNonItemDC, 0, 0, SRCCOPY );
				}
			}
		}
	}
	else
	{
		itItem = gvcDrtItems.begin();

		for( y = 0; TPNL_VERTI > y; y++ )
		{
			for( x = 0; TPNL_HORIZ > x; x++ )
			{
				if( itItem != gvcDrtItems.end() )
				{
					ZeroMemory( atArea, sizeof(atArea) );

					stSize = itItem->stSize;
					DraughtAspectKeeping( &stSize, TRUE );

					hAaDC = CreateCompatibleDC( hdc );
					hOldBmp = SelectBitmap( hAaDC, itItem->hThumbBmp );

					StretchBlt( hdc, (x * giItemWidth), (y * giItemHeight), stSize.cx, stSize.cy,	//	�R�s�[��c�b�A����w�x�A���A����
						hAaDC, 0, 0, itItem->stSize.cx, itItem->stSize.cy,	//	�R�s�[���c�b�A����w�x�A���A����
						SRCCOPY );	//	���X�^�I�y���[�V�����R�[�h	//	itItem->hThumbDC

					SelectBitmap( hAaDC, hOldBmp );

					StringCchPrintf( atArea, MIN_STRING, TEXT("%dDOT x %dLINE"), itItem->iMaxDot, itItem->iLines );
					StringCchLength( atArea, MIN_STRING, &cchLen );
					ExtTextOut( hdc, (x * giItemWidth)+1, ((y+1) * giItemHeight)-12, 0, NULL, atArea, cchLen, NULL );

					DeleteDC( hAaDC );

					itItem++;
				}
				else
				{
					BitBlt( hdc, (x * giItemWidth), (y * giItemHeight), giItemWidth, giItemHeight, ghNonItemDC, 0, 0, SRCCOPY );
				}
			}
		}
	}

	SelectFont( hdc, hOldFnt );

	for( y = 1; TPNL_HORIZ > y; y++ )	//	�c��
	{
		MoveToEx( hdc, (y * giItemWidth), 0, NULL );
		LineTo( hdc, (y * giItemWidth), (giItemHeight * TPNL_VERTI) );
	}

	for( x = 1; TPNL_VERTI > x; x++ )	//	����
	{
		MoveToEx( hdc, 0, (x * giItemHeight), NULL );
		LineTo(   hdc, (giItemWidth * TPNL_HORIZ), (x * giItemHeight) );
	}

	EndPaint( hWnd, &ps );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�H�[�J�X���������ꍇ
	@param[in]	hWnd			�E�C���h�E�n���h��
	@param[in]	hwndNewFocus	�t�H�[�J�X�𓾂��E�C���h�E�̃n���h��
*/
VOID Drt_OnKillFocus( HWND hWnd, HWND hwndNewFocus )
{
	RECT	rect;

	GetWindowRect( hWnd, &rect );
	gstViewLsPt.x = rect.left;
	gstViewLsPt.y = rect.top;

	DestroyWindow( hWnd );

	return;
}
//-------------------------------------------------------------------------------------------------

#ifdef MAA_TOOLTIP
/*!
	�m�[�e�B�t�@�C���b�Z�[�W�̏���
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	idFrom		NOTIFY�𔭐��������R���g���[���̂h�c
	@param[in]	pstNmhdr	NOTIFY�̏ڍ�
	@return		�����������e�Ƃ�
*/
LRESULT Drt_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	INT		iTarget, iOffset, i;
	INT_PTR	iItems;
	LPSTR	pcConts = NULL;
	POINT	stMosPos;
	LPNMTTDISPINFO	pstDispInfo;

	MAAM_ITR	itItem;

	if( TTN_GETDISPINFO ==  pstNmhdr->code )	//	�c�[���`�b�v�̓��e�̖₢���킹��������
	{
		GetCursorPos( &stMosPos );	//	�X�N���[�����W
		ScreenToClient( hWnd, &stMosPos );	//	�N���C�����g���W�ɕϊ�

		pstDispInfo = (LPNMTTDISPINFO)pstNmhdr;

		ZeroMemory( &(pstDispInfo->szText), sizeof(pstDispInfo->szText) );
		pstDispInfo->lpszText = NULL;

		if( !(gbAAtipView) ){	return 0;	}	//	��\���Ȃ牽�����Ȃ��ł���

		FREE( gptTipBuffer );

		iTarget = DraughtTargetItemSet( &stMosPos );
		TRACE( TEXT("TARGET %d"), iTarget );

		if( gbThumb )	//	�T���l�C��
		{
			iOffset = gdVwTop * TPNL_HORIZ;
			iTarget = iOffset + iTarget;
			pcConts = AacAsciiArtGet( iTarget );	//	�Y������C���f�b�N�XAA�����������Ă���

			gptTipBuffer = SjisDecodeAlloc( pcConts );
			FREE( pcConts );
		}
		else
		{
			iItems = gvcDrtItems.size( );	//	���݌�
			if( iItems > iTarget )	//	�ێ������ł����
			{
				for( i = 0, itItem = gvcDrtItems.begin(); gvcDrtItems.end() != itItem; i++, itItem++ )
				{
					if( iTarget == i )	//	�q�b�g
					{
						gptTipBuffer = SjisDecodeAlloc( itItem->pcItem );
						break;
					}
				}
			}
		}

		//	������NULL��Ԃ��ƁA����ȍ~�̃`�b�v���o�Ă��Ȃ�
		if( gptTipBuffer  ){	pstDispInfo->lpszText = gptTipBuffer;	}
		else{					pstDispInfo->lpszText = TTMSG_NO_ITEM;	}
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------
#endif

/*!
	�}�E�X���������Ƃ��̏���
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	x			�N���C�A���g���W�w
	@param[in]	y			�N���C�A���g���W�x
	@param[in]	keyFlags	������Ă鑼�̃{�^��
	@return		�Ȃ�
*/
VOID Drt_OnMouseMove( HWND hWnd, INT x, INT y, UINT keyFlags )
{
	INT		iTarget;
	POINT	point;
	BOOLEAN		bReDraw = FALSE;

	point.x = x;
	point.y = y;

	iTarget = DraughtTargetItemSet( &point );	//	�}�E�X�J�[�\�����̘g���m�F
	if( giItemSel !=  iTarget ){	bReDraw =  TRUE;	}
	giItemSel = iTarget;

#ifdef USE_HOVERTIP
	//	���߂Ă̘g�Ȃ�
	if( bReDraw && gbAAtipView ){	HoverTipResist( ghDraughtWnd  );	}
#endif

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�}�E�X�̒��{�^�������������ꂽ�Ƃ�
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	x			���������w���W�l
	@param[in]	y			���������x���W�l
	@param[in]	keyFlags	���ɉ�����Ă�L�[�ɂ���
*/
VOID Drt_OnMButtonUp( HWND hWnd, INT x, INT y, UINT keyFlags )
{
	TRACE( TEXT("MUP %d x %d"), x , y );	//	�N���C�����g���W

	DraughtButtonUp( hWnd, x, y, keyFlags, WM_MBUTTONUP );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�}�E�X�̍��{�^�������������ꂽ�Ƃ�
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	x			���������w���W�l
	@param[in]	y			���������x���W�l
	@param[in]	keyFlags	���ɉ�����Ă�L�[�ɂ���
*/
VOID Drt_OnLButtonUp( HWND hWnd, INT x, INT y, UINT keyFlags )
{
	TRACE( TEXT("LUP %d x %d"), x , y );	//	�N���C�����g���W

	DraughtButtonUp( hWnd, x, y, keyFlags, WM_LBUTTONUP );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�}�E�X�̃{�^�������������ꂽ�Ƃ�
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	x			���������w���W�l
	@param[in]	y			���������x���W�l
	@param[in]	keyFlags	���ɉ�����Ă�L�[�ɂ���
	@param[in]	message		�������ꂽ�{�^���^�C�v	WM_LBUTTONUP	WM_MBUTTONUP
*/
VOID DraughtButtonUp( HWND hWnd, INT x, INT y, UINT keyFlags, UINT message )
{
	POINT	stPos;
	INT		id;
	UINT	dMode, dSubMode, dStyle;

	stPos.x = x;
	stPos.y = y;

	giTarget = DraughtTargetItemSet( &stPos );
	TRACE( TEXT("TARGET %d"), giTarget );

	//	�T���l���ŃN���b�N�����Ȃ�AMAA�̃f�t�H����ɏ]��
	if( gbThumb )
	{
		dMode = ViewMaaItemsModeGet( &dSubMode );
		//	���N���b�N�̏ꍇ
		if( WM_MBUTTONUP == message )	dMode = dSubMode;

		switch( dMode )
		{
	#ifndef _ORRVW
			case  0:	id = IDM_DRAUGHT_INSERTEDIT;	break;
			case  1:	id = IDM_DRAUGHT_INTERRUPTEDIT;	break;
			case  2:	id = IDM_DRAUGHT_LAYERBOX;	break;
	#endif
			case  3:	id = IDM_DRAUGHT_UNICLIP;	break;
			default:	//	�Ƃ肠�����R�s�[
			case  4:	id = IDM_DRAUGHT_SJISCLIP;	break;
			case  5:	id = IDM_THUMB_DRAUGHT_ADD;	break;
		}
	}
	else
	{
		if( WM_MBUTTONUP == message ){	dStyle = gdSubClickDrt;	}
		else{							dStyle = gdClickDrt;	}

		switch( dStyle )
		{
	#ifndef _ORRVW
			case  0:	id = IDM_DRAUGHT_INSERTEDIT;	break;
			case  1:	id = IDM_DRAUGHT_INTERRUPTEDIT;	break;
			case  2:	id = IDM_DRAUGHT_LAYERBOX;	break;
	#endif
			case  3:	id = IDM_DRAUGHT_UNICLIP;	break;
			default:	//	�h���t�g���Ȃ�A�Ƃ肠�����R�s�[
			case  4:	id = IDM_DRAUGHT_SJISCLIP;	break;
		}
	}

	FORWARD_WM_COMMAND( hWnd, id, ghDraughtWnd, 0, SendMessage );

	return;
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
VOID Drt_OnContextMenu( HWND hWnd, HWND hWndContext, UINT xPos, UINT yPos )
{
	HMENU	hMenu, hSubMenu;
	UINT	dRslt;

	POINT	stPoint, stPos;

	stPoint.x = (SHORT)xPos;	//	��ʍ��W�̓}�C�i�X�����肤��
	stPoint.y = (SHORT)yPos;

	TRACE( TEXT("CTX %d x %d"), stPoint.x, stPoint.y );

	stPos = stPoint;
	ScreenToClient( hWnd, &stPos );
	giTarget = DraughtTargetItemSet( &stPos );
	TRACE( TEXT("TARGET %d"), giTarget );

	hMenu = LoadMenu( GetModuleHandle(NULL), MAKEINTRESOURCE(IDM_DRAUGHT_POPUP) );
	hSubMenu = GetSubMenu( hMenu, 0 );

	if( gbThumb )	//	�T���l���Ȃ�
	{
		DeleteMenu( hSubMenu, IDM_DRAUGHT_ALLDELETE, MF_BYCOMMAND );	//	�S�폜��j��
		DeleteMenu( hSubMenu, IDM_DRAUGHT_EXPORT,    MF_BYCOMMAND );	//	�G�N�X�|�[�g��j��
		//	������ύX
		ModifyMenu( hSubMenu, IDM_DRAUGHT_CLOSE,     MF_BYCOMMAND | MFT_STRING, IDM_DRAUGHT_CLOSE, TEXT("�T���l�C�������(&Q)") );
		ModifyMenu( hSubMenu, IDM_DRAUGHT_DELETE,    MF_BYCOMMAND | MFT_STRING, IDM_THUMB_DRAUGHT_ADD, TEXT("�h���t�g�{�[�h�ɒǉ�(&D)") );
	}

	if( giItemWidth == giItemHeight )	//	�Y������T�C�Y�Ƀ`�F�L���I
	{
		switch( giItemWidth )
		{
			case DTHMSZ_ULTRALIGHT:	CheckMenuItem( hSubMenu, IDM_DB_THUMB_ULTRALIGHT, MF_CHECKED );	break;
			case DTHMSZ_REGULAR:	CheckMenuItem( hSubMenu, IDM_DB_THUMB_REGULAR, MF_CHECKED );	break;
			case DTHMSZ_DEMIBOLD:	CheckMenuItem( hSubMenu, IDM_DB_THUMB_DEMIBOLD, MF_CHECKED );	break;
			case DTHMSZ_ULTRABOLD:	CheckMenuItem( hSubMenu, IDM_DB_THUMB_ULTRABOLD, MF_CHECKED );	break;
			default:	break;
		}
	}

	dRslt = TrackPopupMenu( hSubMenu, 0, stPoint.x, stPoint.y, 0, hWnd, NULL );
	//	�I�������łO���|�P�H�ATPM_RETURNCMD����������A�I�������炻�̃��j���[�̂h�c��WM_COMMAND�����s
	DestroyMenu( hMenu );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�����Ƃ��ɔ����B
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Drt_OnDestroy( HWND hWnd )
{
	ghDraughtWnd = NULL;
	ghScrBarWnd  = NULL;

#ifdef MAA_TOOLTIP
	FREE( gptTipBuffer );
#endif
	return;
}
//-------------------------------------------------------------------------------------------------


/*!
	�z�C�[�����]
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	xPos	�X�N���[���w���W
	@param[in]	yPos	�X�N���[���x���W
	@param[in]	zDelta	��]�ʁE�P�Q�O�P�ʁEWHEEL_DELTA
	@param[in]	fwKeys	���ɉ�����Ă����L�[
*/
VOID Drt_OnMouseWheel( HWND hWnd, INT xPos, INT yPos, INT zDelta, UINT fwKeys )
{
	UINT	dCode;

//	HWND	hChdWnd;
//	POINT	stPoint;

	TRACE( TEXT("POS[%d x %d] DELTA[%d] K[%X]"), xPos, yPos, zDelta, fwKeys );
	//	fwKeys	SHIFT 0x4, CTRL 0x8

	if( 0 < zDelta )		dCode = SB_LINEUP;
	else if( 0 > zDelta )	dCode = SB_LINEDOWN;
	else					dCode = SB_ENDSCROLL;

	Drt_OnVScroll( hWnd, ghScrBarWnd, dCode, 1 );	//	pos�͑������g�p

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�X�N���[���̏���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	hwndCtl	�X�N���[���o�[�̃E�C���h�E�n���h��
	@param[in]	code	�����ԃR�[�h
	@param[in]	pos		�܂݂̈ʒu
	@return		����
*/
VOID Drt_OnVScroll( HWND hWnd, HWND hwndCtl, UINT code, INT pos )
{
	INT	maePos;
	SCROLLINFO	stScrollInfo;

//	if( ghScrBarWnd != hwndCtl )	return;

	//	��Ԃ����₵��
	ZeroMemory( &stScrollInfo, sizeof(SCROLLINFO) );
	stScrollInfo.cbSize = sizeof(SCROLLINFO);
	stScrollInfo.fMask = SIF_ALL;
	GetScrollInfo( ghScrBarWnd, SB_CTL, &stScrollInfo );

	maePos = gdVwTop;

	switch( code )	//	�X�N���[�������ɍ��킹�ē��e�����炷
	{
		case SB_TOP:
			gdVwTop = 0;
			break;

		case SB_LINEUP:
			gdVwTop--;
			if( 0 > gdVwTop )	gdVwTop = 0;
			break;

		case SB_PAGEUP:
			gdVwTop -= 3;
			if( 0 > gdVwTop )	gdVwTop = 0;
			break;

		case SB_LINEDOWN:
			gdVwTop++;
			if( stScrollInfo.nMax <=  gdVwTop ){	gdVwTop = stScrollInfo.nMax;	}
			break;

		case SB_PAGEDOWN:
			gdVwTop += 3;
			if( stScrollInfo.nMax <=  gdVwTop ){	gdVwTop = stScrollInfo.nMax;	}
			break;

		case SB_BOTTOM:
			gdVwTop = stScrollInfo.nMax;
			break;

		case SB_THUMBTRACK:
			gdVwTop = stScrollInfo.nTrackPos;
			break;

		default:	return;
	}
	//	�ω��Ȃ��Ȃ�Ȃɂ�����ł���
	if( maePos == gdVwTop  )	return;

	InvalidateRect( ghDraughtWnd, NULL, TRUE );
	UpdateWindow( ghDraughtWnd );	//	���A���^�C���`��ɕK�v

	stScrollInfo.fMask = SIF_POS;
	stScrollInfo.nPos  = gdVwTop;
	SetScrollInfo( ghScrBarWnd, SB_CTL, &stScrollInfo, TRUE );

	return;
}
//-------------------------------------------------------------------------------------------------

#ifdef USE_HOVERTIP
/*!
	HoverTip�p�̃R�[���o�b�N���
	@param[in]	pVoid	����`
	@return	�m�ۂ���������E��������NULL
*/
LPTSTR CALLBACK DraughtHoverTipInfo( LPVOID pVoid )
{
	INT		iTarget, iOffset, i;
	INT_PTR	iItems;
	LPSTR	pcConts = NULL;
	LPTSTR	ptBuffer = NULL;

	MAAM_ITR	itItem;


	if( !(gbAAtipView) ){	return NULL;	}	//	��\���Ȃ牽�����Ȃ��ł���
	if( 0 > giItemSel ){	return NULL;	}

	if( gbThumb )	//	�T���l�C��
	{
		iOffset = gdVwTop * TPNL_HORIZ;
		iTarget = iOffset + giItemSel;
		pcConts = AacAsciiArtGet( iTarget );	//	�Y������C���f�b�N�XAA�����������Ă���

		ptBuffer = SjisDecodeAlloc( pcConts );
		FREE( pcConts );
	}
	else
	{
		iTarget = giItemSel;
		iItems  = gvcDrtItems.size( );	//	���݌�
		if( iItems > iTarget )	//	�ێ������ł����
		{
			for( i = 0, itItem = gvcDrtItems.begin(); gvcDrtItems.end() != itItem; i++, itItem++ )
			{
				if( iTarget == i )	//	�q�b�g
				{
					ptBuffer = SjisDecodeAlloc( itItem->pcItem );
					break;
				}
			}
		}
	}

	return ptBuffer;
}
//-------------------------------------------------------------------------------------------------
#endif




#ifndef _ORRVW

/*!
	�ҏW�̑I��͈͂��炢������
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	bSqSel	��`�ł��邩�ǂ���
*/
UINT DraughtItemAddFromSelect( HWND hWnd, UINT bSqSel )
{
	LPTSTR	ptString = NULL;
	UINT	cchSize, cbSize;
	LPPOINT	pstPos = NULL;

	UINT_PTR	i, j, iTexts;
	LONG	dMin = 0;
	INT		insDot, yLine, iLines = 0, dOffset;
	LPTSTR	ptSpace = NULL;

	LPSTR	pcArts;
	wstring	wsString;


	//	���������Ȃ̂�Unicode�Œ�
	cbSize = DocSelectTextGetAlloc( D_UNI | bSqSel, (LPVOID *)(&ptString), (bSqSel & D_SQUARE) ? &pstPos : NULL );

	StringCchLength( ptString, STRSAFE_MAX_CCH, &cchSize );

	if( 0 >= cchSize )	return 0;	//	������Ȃ��Ȃ�Ȃɂ����Ȃ�

	//	�I�t�Z�b�g�ݒ肪�L��ꍇ�A���̕��𖄂߂�󔒂��K�v
	if( pstPos )	//	�ŏ��I�t�Z�b�g�l��T���āA���������[�ɂ���
	{
		dMin = pstPos[0].x;

		yLine = 0;
		for( i = 0; cchSize > i; i++ )
		{
			if( CC_CR == ptString[i] && CC_LF == ptString[i+1] )	//	���s�ł�������
			{
				//	�I�t�Z�b�g�ŏ���������
				if( dMin > pstPos[yLine].x ){	dMin =  pstPos[yLine].x;	}

				i++;		//	0x0D,0x0A������A�땶����΂��̂��|�C���g
				yLine++;	//	���s��������Focus�͎��̍s��
			}
		}
		//	���̎��_�ŁAyLine�͍s���ɂȂ��Ă�
		iLines = yLine;

		//	��s�ڂ̋󔒂�����đM�����͂��Ă���
		insDot = 0;
		dOffset = pstPos[0].x - dMin;
		ptSpace = DocPaddingSpaceUni( dOffset, NULL, NULL, NULL );
		//	�O���󔒂͖��������̂Ń��j�R�[�h�g���Ė�薳��
		StringCchLength( ptSpace, STRSAFE_MAX_CCH, &iTexts );
		for( j = 0; iTexts > j; j++ ){	wsString +=  ptSpace[j];	}
		FREE(ptSpace);
	}

	yLine = 0;	insDot = 0;
	for( i = 0; cchSize > i; i++ )
	{
		if( CC_CR == ptString[i] && CC_LF == ptString[i+1] )	//	���s�ł�������
		{
			wsString +=  wstring( TEXT("\r\n") );

			i++;		//	0x0D,0x0A������A�땶����΂��̂��|�C���g
			yLine++;	//	���s��������Focus�͎��̍s��

			//	�I�t�Z�b�g���̋󔒂����
			if( pstPos && (iLines > yLine) )
			{
				dOffset = pstPos[yLine].x - dMin;
				ptSpace = DocPaddingSpaceUni( dOffset, NULL, NULL, NULL );
				//	�O���󔒂͖��������̂Ń��j�R�[�h�g���Ė�薳��
				StringCchLength( ptSpace, STRSAFE_MAX_CCH, &iTexts );
				for( j = 0; iTexts > j; j++ ){	wsString +=  ptSpace[j];	}
				FREE(ptSpace);
			}
		}
		else if( CC_TAB ==  ptString[i] ){	/*	�^�u�͑}�����Ȃ�	*/	}
		else{	wsString += ptString[i];	}
	}

	FREE(ptString);
	FREE(pstPos);

	pcArts =  SjisEncodeAlloc( wsString.c_str() );	//	�I��͈͂��h���t�g�{�[�h�Ɉڂ��Ƃ�

	DraughtItemAdding( hWnd, pcArts );

	FREE(pcArts);

	return yLine;
}
//-------------------------------------------------------------------------------------------------
#endif

/*!
	AA�e�L�X�g���m�ۂ��Ď�荞��
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	pcArts	�`�`�e�L�X�gSJIS
	@return		�ǉ���̃A�C�e������
*/
UINT DraughtItemAdding( HWND hWnd, LPSTR pcArts )
{
	UINT_PTR	cbSize;
	AAMATRIX	stItem;

	INT_PTR	iItems;


	StringCchLengthA( pcArts, STRSAFE_MAX_CCH, &cbSize );

	stItem.cbItem = cbSize;
	stItem.pcItem = (LPSTR)malloc( (cbSize + 1) );
	ZeroMemory( stItem.pcItem, (cbSize + 1) );
	StringCchCopyA( stItem.pcItem, (cbSize + 1), pcArts );


	DraughtAaImageing( hWnd, &stItem );


	gvcDrtItems.push_back( stItem );

	do	//	�݂͂����Ă���H
	{
		iItems = gvcDrtItems.size( );
		if( (TPNL_HORIZ * TPNL_VERTI) < iItems ){	DraughtItemDelete(  0 );	}

	}while( (TPNL_HORIZ * TPNL_VERTI) < iItems );

	return iItems;
}
//-------------------------------------------------------------------------------------------------

/*!
	�`�`�̃T���l�p�C���[�W�����
	@param[in]		hWnd	�E�C���h�E�n���h��
	@param[in,out]	pstItem	�C���[�W��肽��AA�̃f�[�^�Ƃ�
*/
UINT DraughtAaImageing( HWND hWnd, LPAAMATRIX pstItem )
{
	UINT_PTR	cchSize;
	LPTSTR		ptTextaa;
	INT		iRslt, iYdot, iXdot, iLine;
	HDC		hdc, hMemDC;
	HBITMAP	hOldBmp;
	RECT	rect;



	ptTextaa = SjisDecodeAlloc( pstItem->pcItem );
	StringCchLength( ptTextaa, STRSAFE_MAX_CCH, &cchSize );

	//	���`�`�̃T�C�Y�m�肪�K�v
	iXdot = TextViewSizeGet( ptTextaa, &iLine );
	iYdot = iLine * LINE_HEIGHT;

	pstItem->iMaxDot = iXdot;
	pstItem->iLines  = iLine;

	if( giItemWidth >  iXdot )	iXdot = giItemWidth;
	if( giItemHeight > iYdot )	iYdot = giItemHeight;

	pstItem->stSize.cx = iXdot;
	pstItem->stSize.cy = iYdot;

	SetRect( &rect, 0, 0, iXdot, iYdot );
	//	����܂�傫���Ȃ�A�������Ƃ��H

	//	�`��p�r�b�g�}�b�v�쐬
	hdc = GetDC( hWnd );

	//	�T���l�pBMP�E����̓t���T�C�Y	//	pstItem->hThumbDC	pstItem->hOldBmp
	hMemDC = CreateCompatibleDC( hdc );
	pstItem->hThumbBmp = CreateCompatibleBitmap( hdc, rect.right, rect.bottom );
	hOldBmp = SelectBitmap( hMemDC, pstItem->hThumbBmp );
	SelectFont( hMemDC, ghAaFont );

	ReleaseDC( ghDraughtWnd, hdc );

	iRslt = PatBlt( hMemDC, 0, 0, rect.right, rect.bottom, WHITENESS );

	iRslt = DrawText( hMemDC, ptTextaa, cchSize, &rect, DT_LEFT | DT_NOPREFIX | DT_NOCLIP | DT_WORDBREAK );

	SelectFont( hMemDC, GetStockFont(DEFAULT_GUI_FONT) );
	SelectBitmap( hMemDC, hOldBmp );
	DeleteDC( hMemDC );

	FREE( ptTextaa );

	return 0;
}
//-------------------------------------------------------------------------------------------------

/*!
	Target�A�C�e�����g���E�N���b�v�{�[�h�ցE���Ɏg�������Ƃ��́H
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	id		���샂�[�h�h�c
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DraughtItemUse( HWND hWnd, INT id )
{
	LPSTR		pcAaItem;
	INT_PTR		iItems, i, iOffset, iTarget;
	UINT_PTR	cbSize;
	UINT		dMode;
	MAAM_ITR	itItem;

	if( gbThumb )	//	�T���l���[�h
	{
		iOffset = gdVwTop * TPNL_HORIZ;
		iTarget = iOffset + giTarget;

		pcAaItem = AacAsciiArtGet( iTarget );
		if( !(pcAaItem) )	return E_OUTOFMEMORY;

		switch( id )
		{
			case IDM_DRAUGHT_INSERTEDIT:	dMode = MAA_INSERT;		break;
			case IDM_DRAUGHT_INTERRUPTEDIT:	dMode = MAA_INTERRUPT;	break;
			case IDM_DRAUGHT_LAYERBOX:		dMode = MAA_LAYERED;	break;
			default:
			case IDM_DRAUGHT_UNICLIP:		dMode = MAA_UNICLIP;	break;
			case IDM_DRAUGHT_SJISCLIP:		dMode = MAA_SJISCLIP;	break;

			case IDM_THUMB_DRAUGHT_ADD:		dMode = MAA_DRAUGHT;	break;
		}
		StringCchLengthA( pcAaItem, STRSAFE_MAX_CCH, &cbSize );

		ViewMaaMaterialise( hWnd, pcAaItem, cbSize, dMode );

		if( id != IDM_THUMB_DRAUGHT_ADD )
		{
			//	�����ł��C�ɓ���ɓ����E���v���H
			if( SUCCEEDED( AaItemsFavUpload( pcAaItem, cbSize ) ) )
			{
				FavContsRedrawRequest( hWnd );
			}
		}

		FREE(pcAaItem);
	}
	else
	{
		iItems = gvcDrtItems.size( );	//	���݌�
		if( 0 >= iItems )	return E_OUTOFMEMORY;

		for( i = 0, itItem = gvcDrtItems.begin(); gvcDrtItems.end() != itItem; i++, itItem++ )
		{
			if( giTarget == i )	//	�q�b�g
			{
				switch( id )
				{
					case IDM_DRAUGHT_INSERTEDIT:	dMode = MAA_INSERT;		break;
					case IDM_DRAUGHT_INTERRUPTEDIT:	dMode = MAA_INTERRUPT;	break;
					case IDM_DRAUGHT_LAYERBOX:		dMode = MAA_LAYERED;	break;
					default:
					case IDM_DRAUGHT_UNICLIP:		dMode = MAA_UNICLIP;	break;
					case IDM_DRAUGHT_SJISCLIP:		dMode = MAA_SJISCLIP;	break;
				}
				StringCchLengthA( itItem->pcItem, STRSAFE_MAX_CCH, &cbSize );

				ViewMaaMaterialise( hWnd, itItem->pcItem, cbSize, dMode );
			}
		}
	}

	return E_INVALIDARG;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ΏۃA�C�e���폜�EREDRAW�̖ʓ|�͕ʂŌ���
	@param[in]	iTarget	�Ώۂ̒ʂ��ԍ��E�}�C�i�X�Ȃ�S�폜
	@return	��O�폜�����@�O�폜�ł��񂩂���
*/
INT DraughtItemDelete( CONST INT iTarget )
{
	INT_PTR	iItems, i;
	INT		delCnt;
	MAAM_ITR	itItem;

	iItems = gvcDrtItems.size( );	//	���݌�
	if( 0 >= iItems )	return 0;

	delCnt = 0;
	for( i = 0, itItem = gvcDrtItems.begin(); gvcDrtItems.end() != itItem; i++, itItem++ )
	{
		if( iTarget == i || 0 > iTarget )
		{
			FREE( itItem->pcItem );
			DeleteBitmap( itItem->hThumbBmp );
			delCnt++;

			if( iTarget == i )
			{
				gvcDrtItems.erase( itItem );
				return 1;
			}
		}
	}

	if( 0 > iTarget ){	gvcDrtItems.clear( );	}

	return delCnt;
}
//-------------------------------------------------------------------------------------------------

/*!
	�N���C�����g���W���󂯎���āA�^�[�Q�b�g�A�C�e���ʂ��ԍ����o��
	@param[in]	pstPos	�N���C�����g���W���������\���̃|�C���^�[
	@return		���ォ��A���ɒʂ��ԍ��O�C���f�b�N�X�@�����Ł|�P
*/
INT DraughtTargetItemSet( LPPOINT pstPos )
{
	INT	ix, iy, number;

	ix = pstPos->x / giItemWidth;
	iy = pstPos->y / giItemHeight;

	if( 0 > ix || TPNL_HORIZ <= ix || 0 > iy || TPNL_VERTI <= iy )	return -1;

	number = ix + iy * TPNL_HORIZ;

	return number;
}
//-------------------------------------------------------------------------------------------------

/*!
	�c�����󂯎���āA�A�X�y�N�g����ێ����ăT���l�T�C�Y�ɏk������
	@param[in,out]	pstSize	�󂯎���āA�߂�
	@param[in]		bOrgRem	��O�T���l�T�C�Y��菬�����Ȃ�I���W�i���T�C�Y
	@return	DOUBLE	ZOOM��
*/
DOUBLE DraughtAspectKeeping( LPSIZE pstSize, UINT bOrgRem )
{
	INT	iOrgWid, iOrgHei;
	INT	iZomWid, iZomHei;
	DOUBLE	ddPercent, ddBuff;

	iOrgWid = pstSize->cx;
	iOrgHei = pstSize->cy;
	ddPercent = 1.0;

	if( iOrgWid >= iOrgHei )	//	�����`������
	{
		iZomWid = giItemWidth;

		if( giItemWidth == iOrgWid )	//	�s�^���Ȃ牽�����邱�ƂȂ�
		{
			iZomHei = iOrgHei;
		}
		else
		{
			ddPercent = (DOUBLE)giItemWidth / (DOUBLE)iOrgWid;
			if( bOrgRem && giItemWidth > iOrgWid )	//	�T�C�Y�ȉ��Ȃ�I���W�i���T�C�Y�ł�낵
			{
				iZomWid = iOrgWid;
				iZomHei = iOrgHei;
			}
			else
			{
				ddBuff  = (DOUBLE)iOrgHei * ddPercent;
				ddBuff += 0.5;	//	�l�̌ܓ�
				iZomHei = ddBuff;
			}
		}
	}
	else	//	�c��
	{
		iZomHei = giItemHeight;

		if( giItemHeight == iOrgHei )	//	�s�^���Ȃ牽�����邱�ƂȂ�
		{
			iZomWid = iOrgWid;
		}
		else
		{
			ddPercent = (DOUBLE)giItemHeight / (DOUBLE)iOrgHei;
			if( bOrgRem && giItemHeight >  iOrgHei )	//	�T�C�Y�ȉ��Ȃ�I���W�i���T�C�Y�ł�낵
			{
				iZomWid = iOrgWid;
				iZomHei = iOrgHei;
			}
			else
			{
				ddBuff  = (DOUBLE)iOrgWid * ddPercent;
				ddBuff += 0.5;	//	�l�̌ܓ�
				iZomWid = ddBuff;
			}
		}
	}

	pstSize->cx = iZomWid;
	pstSize->cy = iZomHei;

	return ddPercent;
}
//-------------------------------------------------------------------------------------------------

/*!
	�h���t�g�{�[�h�̓��e���t�@�C���ɏ����o��
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	ptPath	���g�p
*/
HRESULT DraughtItemExport( HWND hWnd, LPTSTR ptPath )
{
	CONST CHAR	cacSplit[] = ("[SPLIT]\r\n");	//	�X�a�x�s�d

	UINT_PTR	dItems, cbSize;
	TCHAR		atPath[MAX_PATH], atName[MAX_PATH];
	BOOLEAN		bOpened;
	OPENFILENAME	stOpenFile;

	MAAM_ITR	itItem;

	HANDLE	hFile;
	DWORD	wrote;


	dItems = gvcDrtItems.size();
	if( 0 >= dItems )	return E_NOTIMPL;	//	��Ȃ牽�����Ȃ�

	//�t�@�C�����m��
	ZeroMemory( atPath, sizeof(atPath) );
	ZeroMemory( atName, sizeof(atName) );

	ZeroMemory( &stOpenFile, sizeof(OPENFILENAME) );
	stOpenFile.lStructSize     = sizeof(OPENFILENAME);
	stOpenFile.hwndOwner       = ghPtWnd;
	stOpenFile.lpstrFilter     = TEXT("�����s�e���v���t�@�C��(*.mlt)\0*.mlt\0�S�Ă̌`��(*.*)\0*.*\0\0");
	stOpenFile.nFilterIndex    = 1;
	stOpenFile.lpstrFile       = atPath;
	stOpenFile.nMaxFile        = MAX_PATH;
	stOpenFile.lpstrFileTitle  = atName;
	stOpenFile.nMaxFileTitle   = MAX_PATH;
	stOpenFile.lpstrTitle      = TEXT("�ۑ�����t�@�C�������w�肵�Ă�");
	stOpenFile.Flags           = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
	stOpenFile.lpstrDefExt     = TEXT("mlt");

	//������ FileOpenDialogue ���o��
	bOpened = GetSaveFileName( &stOpenFile );
	wrote = CommDlgExtendedError();

	TRACE( TEXT("�t�@�C���ۑ��_�C�����O�ʉ�[%X]"), wrote );

#ifndef _ORRVW
	ViewFocusSet(  );
#endif
	if( !(bOpened) ){	return  E_ABORT;	}	//	�L�����Z�����Ă��牽�����Ȃ�

	hFile = CreateFile( atPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE == hFile ){	return E_HANDLE;	}

	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );

	for( itItem = gvcDrtItems.begin(); gvcDrtItems.end() != itItem; itItem++ )
	{
		StringCchLengthA( itItem->pcItem, STRSAFE_MAX_LENGTH, &cbSize );
		
		WriteFile( hFile, itItem->pcItem, cbSize, &wrote, NULL );
		WriteFile( hFile, cacSplit, 9, &wrote, NULL );	//	�Œ�l����
	}

	CloseHandle( hFile );

	MessageBox( hWnd, TEXT("�t�@�C���ɕۑ�������"), TEXT("���ӂ���̂��m�点"), MB_OK | MB_ICONINFORMATION );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


