/*! @file
	@brief �����p�̃��C���{�b�N�X�̃r���[���̑���ł�
	���̃t�@�C���� LayerBox.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/05/31
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
�֊s���k�L����ɂ́H
���̍s�́A�ŏ��̋󔒈ȊO�̕����̎�O�ƁA���������̌�
���߃G���A�̊J�n�ʒu�ƏI�[�ʒu
���Ȃ��E5dot�E11dot���炢��

�}���㏑����������O�ɁA���k�L�G���A����������ɒǉ����Ă���
��Ɨp������Ƃ��āA�}�������������ŒP�ƂŊm�ۂ��Ă����E���������j�󂵂Ȃ��悤��
�J�n�ʒu�Ƃ����ߗ̈�́A���k�L�ɕ����ĘM���Ă���
*/

//-------------------------------------------------------------------------------------------------

typedef struct tagLAYERBOXSTRUCT
{
	LONG	id;					//!<	�{�b�N�X�̔F���ԍ�

	POINT	stOffset;			//!<	�r���[���ォ��́A�{�b�N�X�̑��Έʒu

	HWND	hBoxWnd;			//!<	�{�b�N�X�̃E�C���h�E�n���h��

	HWND	hTextWnd;			//!<	�e�L�X�g�G���A�̃E�C���h�E�n���h��
//	WNDPROC	pfOrgTextProc;		//!<	�T�u�N���X���v���V�[�W���E����Ȃ��H

	HWND	hToolWnd;			//!<	�c�[���o�[�̃E�C���h�E�n���h��
//	WNDPROC	pfOrgToolProc;		//!<	�T�u�N���X���v���V�[�W���E����Ȃ��H

	vector<ONELINE>	vcLyrImg;	//!<	�\������f�[�^�̕ێ��EAA�p

} LAYERBOXSTRUCT, *LPLAYERBOXSTRUCT;

typedef list<LAYERBOXSTRUCT>::iterator	LAYER_ITR;
typedef vector<ONELINE>::iterator		LYLINE_ITR;

//-------------------------------------------------------------------------------------------------

#define LAYERBOX_CLASS	TEXT("LAYER_BOX")
#define	LB_WIDTH	310
#define LB_HEIGHT	220

#define EDGE_BLANK_NARROW	16	//	�Œ���Ƃ�󔒕�
#define EDGE_BLANK_WIDE		22	//	�L����
//-------------------------------------------------------------------------------------------------

#define TB_ITEMS	8
static  TBBUTTON	gstTBInfo[] = {
	{ 0,	IDM_LYB_INSERT,		TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	�}��
	{ 1,	IDM_LYB_OVERRIDE,	TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	�㏑
	{ 0,	0,					TBSTATE_ENABLED,	TBSTYLE_SEP,						{0, 0}, 0, 0  },
	{ 2,	IDM_LYB_COPY,		TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  },	//	�R�s�[
	{ 0,	0,					TBSTATE_ENABLED,	TBSTYLE_SEP,						{0, 0}, 0, 0  },
	{ 3,	IDM_LYB_DO_EDIT,	TBSTATE_ENABLED,	TBSTYLE_CHECK | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	�ҏW�{�b�N�XON/OFF
	{ 0,	0,					TBSTATE_ENABLED,	TBSTYLE_SEP,						{0, 0}, 0, 0  },
	{ 4,	IDM_LYB_DELETE,		TBSTATE_ENABLED,	TBSTYLE_AUTOSIZE,					{0, 0}, 0, 0  } 	//	20120507	���e�N�����[

};	//	
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

extern INT		gdDocXdot;		//!<	�L�����b�g�̂w�h�b�g�E�h�L�������g�ʒu
extern INT		gdDocLine;		//!<	�L�����b�g�̂x�s���E�h�L�������g�ʒu

extern INT		gdHideXdot;		//!<	���̉B�ꕔ��
extern INT		gdViewTopLine;	//!<	�\�����̍ŏ㕔�s�ԍ�

extern HFONT	ghAaFont;		//!<	AA�p�t�H���g

extern  HWND	ghViewWnd;		//!<	�r���[�E�C���h�E�n���h��

static POINT	gstViewOrigin;	//!<	�r���[�̍���E�C���h�E�ʒu�E

static  ATOM	gLyrBoxAtom;	//!<	���C���{�b�N�X���̃N���X�A�g��

static  LONG	gdBoxID;		//!<	�ʂ��ԍ��E���INCREMENT

static POINT	gstFrmSz;		//!<	�E�C���h�E�G�b�W����X�^�e�B�b�N�܂ł̃I�t�Z�b�g
static INT		gdToolBarHei;	//!<	�c�[���o�[����

EXTERNED BYTE	gbAlpha;		//!<	�����x

static BOOLEAN	gbQuickClose;	//!<	�\��t�����璼������

static WNDPROC	gpfOrigLyrTBProc;	//!<	
static WNDPROC	gpfOrigLyrEditProc;	//!<	
//	���v���V�[�W���͋��ʂŖ�薳���H

static HIMAGELIST	ghLayerImgLst;	//!<	

static  list<LAYERBOXSTRUCT>	gltLayer;	//!<	�����̃��C���{�b�N�X���J�����Ƃ�
//-------------------------------------------------------------------------------------------------

static LRESULT	CALLBACK gpfLayerTBProc( HWND, UINT, WPARAM, LPARAM );	//!<	
static LRESULT	CALLBACK gpfLyrEditProc( HWND, UINT, WPARAM, LPARAM );	//!<	

LRESULT	CALLBACK LayerBoxProc( HWND, UINT, WPARAM, LPARAM );	//!<	

BOOLEAN	Lyb_OnCreate( HWND, LPCREATESTRUCT );				//!<	WM_CREATE �̏���
VOID	Lyb_OnCommand( HWND , INT, HWND, UINT );			//!<	
//VOID	Lyb_OnSize( HWND , UINT, INT, INT );				//!<	
VOID	Lyb_OnKey( HWND, UINT, BOOL, INT, UINT );			//!<	
VOID	Lyb_OnPaint( HWND );								//!<	
VOID	Lyb_OnDestroy( HWND );								//!<	
VOID	Lyb_OnMoving( HWND, LPRECT );						//!<	
BOOL	Lyb_OnWindowPosChanging( HWND, LPWINDOWPOS );		//!<	
VOID	Lyb_OnWindowPosChanged( HWND, const LPWINDOWPOS );	//!<	
VOID	Lyb_OnLButtonDown( HWND, BOOL, INT, INT, UINT );	//!<	
VOID	Lyb_OnContextMenu( HWND, HWND, UINT, UINT );		//!<	

HRESULT	LayerEditOnOff( HWND, UINT );						//!<	


HRESULT	LayerStringObliterate( LAYER_ITR  );				//!<	
HRESULT	LayerFromString( LAYER_ITR, LPCTSTR );				//!<	
HRESULT	LayerFromSelectArea( LAYER_ITR , UINT );			//!<	
HRESULT	LayerFromClipboard( LAYER_ITR );					//!<	
HRESULT	LayerForClipboard( HWND, UINT );					//!<	
HRESULT	LayerOnDelete( HWND );								//!<	
INT		LayerInputLetter( LAYER_ITR, INT, INT, TCHAR );		//!<	
LPTSTR	LayerLineTextGetAlloc( LAYER_ITR, INT );			//!<	
HRESULT	LayerBoxSetString( LAYER_ITR, LPCTSTR, UINT, LPPOINT, UINT );	//!<	
HRESULT	LayerBoxSizeAdjust( LAYER_ITR );					//!<	

INT		LayerTransparentAdjust( LAYER_ITR, INT, INT );		//!<	

#ifdef EDGE_BLANK_STYLE
HRESULT	LayerEdgeBlankSizeCheck( HWND, INT );				//!<	
#endif
//-------------------------------------------------------------------------------------------------

/*!
	�{�b�N�X�̍쐬�E�ŏ���1��̂�
	@param[in]	hInstance	�A�v���̃C���X�^���X
	@param[in]	pstFrame	�N���C�����g�T�C�Y
	@return		����
*/
VOID LayerBoxInitialise( HINSTANCE hInstance, LPRECT pstFrame )
{
	WNDCLASSEX	wcex;
	HBITMAP	hImg, hMsq;

	if( !(hInstance) )	//	�j�󖽗�
	{
		ImageList_Destroy( ghLayerImgLst );

		return;
	}

	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= LayerBoxProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_MENU+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= LAYERBOX_CLASS;
	wcex.hIconSm		= NULL;

	gLyrBoxAtom = RegisterClassEx( &wcex );

	gbQuickClose = TRUE;

	gdBoxID = 0;

	//�c�[���o�[�p�C���[�W���X�g�쐬
	ghLayerImgLst = ImageList_Create( 16, 16, ILC_COLOR24 | ILC_MASK, 5, 1 );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( (IDBMP_LAYERINSERT) ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( (IDBMQ_LAYERINSERT) ) );
	ImageList_Add( ghLayerImgLst, hImg, hMsq );
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( (IDBMP_LAAYEROVERWRITE) ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( (IDBMQ_LAAYEROVERWRITE) ) );
	ImageList_Add( ghLayerImgLst, hImg, hMsq );
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( (IDBMP_COPY) ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( (IDBMQ_COPY) ) );
	ImageList_Add( ghLayerImgLst, hImg, hMsq );
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( (IDBMP_LAYERTEXTEDIT) ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( (IDBMQ_LAYERTEXTEDIT) ) );
	ImageList_Add( ghLayerImgLst, hImg, hMsq );
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( (IDBMP_DELETE) ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( (IDBMQ_DELETE) ) );
	ImageList_Add( ghLayerImgLst, hImg, hMsq );
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ꂢ��ڂ�������̃A���t�@���X�V!?
	@param[in]	dParam	�V�����A���t�@�l
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT LayerBoxAlphaSet( UINT dParam )
{
	gbAlpha = dParam & 0xFF;

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*�I
	���C���{�b�N�X���쐬
	@param[in]	hInst	�����l
	@param[in]	ptStr	�\�����ׂ�������Ȃ�L���A�Ⴄ�Ȃ�NULL
	@param[in]	bNormal	0x00���ʂɏ����@0x10������
	@return		HWND	�쐬���ꂽ���C���{�b�N�X�̃E�C���h�E�n���h��
*/
HWND LayerBoxVisibalise( HINSTANCE hInst, LPCTSTR ptStr, UINT bNormal )
{
	INT		x, y;
	RECT	vwRect, rect;
	DWORD	dwStyle;

	BOOLEAN	bSelect = FALSE;
	UINT	bSqSel = 0;

	LAYERBOXSTRUCT	stLayer;
	LAYER_ITR	itLyr;


//	stLayer.pfOrgTextProc = NULL;	//	���Ƃ�
//	stLayer.pfOrgToolProc = NULL;	//	���Ƃ�
	stLayer.id = gdBoxID;	//	�{�b�N�X�̔F���ԍ�

	bSelect = IsSelecting( &bSqSel );

	stLayer.vcLyrImg.clear( );	//	�\������f�[�^�̕ێ��EAA�p

	if( 0x10 & bNormal ){	dwStyle = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU;	}
	else{		dwStyle = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_VISIBLE | WS_SYSMENU;	}

	//	�ꏊ�͂O�ɂ��Ă����΃N���C�����g�ʒu�Ōv�Z�o����
	stLayer.hBoxWnd = CreateWindowEx( WS_EX_TOOLWINDOW | WS_EX_LAYERED, LAYERBOX_CLASS,
		TEXT("���C��"), dwStyle, 0, 0, LB_WIDTH, LB_HEIGHT, NULL, NULL, hInst, NULL);

	//	�h�c���E�C���h�E�n���h���ɕۑ����Ă���
	WndTagSet( stLayer.hBoxWnd, stLayer.id );

	SetLayeredWindowAttributes( stLayer.hBoxWnd, 0, gbAlpha, LWA_ALPHA );

	//	�c�[���o�[�̃E�C���h�E�n���h��
	stLayer.hToolWnd = GetDlgItem( stLayer.hBoxWnd, IDW_LYB_TOOL_BAR );

	//	�E�C���h�E�ʒu���m�肳����
	GetWindowRect( ghViewWnd, &vwRect );
	gstViewOrigin.x = vwRect.left;//�ʒu�L�^�E���������ς����̂���Ȃ�
	gstViewOrigin.y = vwRect.top;
	//x = (vwRect.left + LINENUM_WID) - gstFrmSz.x;
	//y = (vwRect.top  + RULER_AREA)  - gstFrmSz.y;
	//���̎��_�ŁA�ҏW�G���A�̂O�C�O�������Ă���
	x = gdDocXdot;
	y = gdDocLine * LINE_HEIGHT;
	ViewPositionTransform( &x, &y, TRUE );
	x += (vwRect.left - gstFrmSz.x);
	y += (vwRect.top  - gstFrmSz.y);
	TRACE( TEXT("%d x %d"), x, y );

#ifdef _DEBUG
	SetWindowPos( stLayer.hBoxWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE );
#else
	SetWindowPos( stLayer.hBoxWnd, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE );
#endif
	stLayer.stOffset.x = x - vwRect.left;
	stLayer.stOffset.y = y - vwRect.top;

	GetClientRect( stLayer.hBoxWnd, &rect );

	//	�ҏW�p�G�f�B�b�g
	stLayer.hTextWnd = CreateWindowEx( 0, WC_EDIT, TEXT(""), 
		WS_CHILD | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		0, gdToolBarHei, rect.right, rect.bottom - gdToolBarHei,
		stLayer.hBoxWnd, (HMENU)IDE_LYB_TEXTEDIT, hInst, NULL );
	SetWindowFont( stLayer.hTextWnd, ghAaFont, TRUE );

	//	�T�u�N���X
	gpfOrigLyrEditProc = SubclassWindow( stLayer.hTextWnd, gpfLyrEditProc );

	//	���C�����X�g�ɋL�^
	gltLayer.push_back( stLayer );
	itLyr = gltLayer.end();
	itLyr--;	//	�ǉ������͖̂��[�����炱��ł���

	//	�D�揇�ʂɒ���
	if( ptStr  )	//	�L�������񂪂���Ȃ�
	{
		TRACE( TEXT("LAYER from STRING") );
		LayerFromString( itLyr, ptStr );
	}
	else if( bSelect )	//	�I��͈͂��L���ł���	DocIsSelecting
	{
		TRACE( TEXT("LAYER from Select") );
		LayerFromSelectArea( itLyr, bSqSel );
	}
	else	//	�ǂ��ł��Ȃ��Ȃ�N���b�v�{�[�h����
	{
		TRACE( TEXT("LAYER from ClipBoard") );
		LayerFromClipboard( itLyr );
	}

	if( !(0x10 & bNormal) )
	{
		ShowWindow( stLayer.hBoxWnd, SW_SHOW );
		UpdateWindow( stLayer.hBoxWnd );

		GetWindowRect( stLayer.hBoxWnd, &rect );
		Lyb_OnMoving( stLayer.hBoxWnd, &rect );
	}

	gdBoxID++;

	return stLayer.hBoxWnd;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C���{�b�N�X�̈ʒu���O������ύX
	@param[in]	hWnd	�Ώۂ̃��C���{�b�N�X�̃n���h��
	@param[in]	x		�`��ʒu�̃X�N���[���w�ʒu
	@param[in]	y		�`��ʒu�̃X�N���[���x�ʒu
*/
HRESULT LayerBoxPositionChange( HWND hWnd, LONG x, LONG y )
{
	LAYER_ITR	itLyr;

	for( itLyr = gltLayer.begin(); itLyr != gltLayer.end(); itLyr++ )
	{
		if( itLyr->hBoxWnd == hWnd )
		{
			x -= gstFrmSz.x;
			y -= gstFrmSz.y;
			SetWindowPos( hWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�c�[���o�[�T�u�N���X
	WindowsXP�ŁA�c�[���o�[�̃{�^����Ń}�E�X�̍��{�^�����������܂܉E�{�^���������ƁA
	����ȍ~�̃}�E�X����𐳏�Ɏ󂯕t���Ȃ��Ȃ�B����̑΍�
	@param[in]	hWnd	�c�[���o�[�n���h��
	@param[in]	msg		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@return	�����������ʂƂ�
*/
LRESULT CALLBACK gpfLayerTBProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	INT		itemID;
	HDC		hdc;
	HWND	hWndChild;

	switch( msg )
	{
		case WM_CTLCOLORSTATIC:	//	�`�F�b�N�{�b�N�X�̕����񕔕��̐F�ύX
			hdc = (HDC)(wParam);
			hWndChild = (HWND)(lParam);

			itemID = GetDlgCtrlID( hWndChild );

			if( IDCB_LAYER_QUICKCLOSE == itemID || IDCB_LAYER_EDGE_BLANK == itemID )
			{
				SetBkColor( hdc, GetSysColor( COLOR_WINDOW ) );
				return (LRESULT)GetSysColorBrush( COLOR_WINDOW );
			}
			break;


		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			if( SendMessage(hWnd, TB_GETHOTITEM, 0, 0) >= 0 ){	ReleaseCapture(   );	}
			return 0;
	}

	return CallWindowProc( gpfOrigLyrTBProc, hWnd, msg, wParam, lParam );
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
LRESULT CALLBACK gpfLyrEditProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
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
			TRACE( TEXT("[%X]LyrEdit COMMAND %d"), hWnd, id );
			
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

	return CallWindowProc( gpfOrigLyrEditProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------


/*!
	���C���{�b�N�X�̃E�C���h�E�v���V�[�W��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�����ς�
	@retval no0	�����ł͏����������ɉ�
*/
LRESULT CALLBACK LayerBoxProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		HANDLE_MSG( hWnd, WM_CREATE,			Lyb_OnCreate );		
		HANDLE_MSG( hWnd, WM_COMMAND,			Lyb_OnCommand );	
		HANDLE_MSG( hWnd, WM_PAINT,				Lyb_OnPaint );		
		HANDLE_MSG( hWnd, WM_DESTROY,			Lyb_OnDestroy );	
		HANDLE_MSG( hWnd, WM_KEYDOWN,			Lyb_OnKey );		
		HANDLE_MSG( hWnd, WM_LBUTTONDBLCLK,		Lyb_OnLButtonDown );	
		HANDLE_MSG( hWnd, WM_CONTEXTMENU,		Lyb_OnContextMenu );	
		HANDLE_MSG( hWnd, WM_WINDOWPOSCHANGING,	Lyb_OnWindowPosChanging );	
		HANDLE_MSG( hWnd, WM_WINDOWPOSCHANGED,	Lyb_OnWindowPosChanged );	
	//	WM_WINDOWPOSCHANGED ���g�����ꍇ�AWM_SIZE�͔������Ȃ��悤��
	//	HANDLE_MSG( hWnd, WM_SIZE,				Lyb_OnSize );	

		case WM_MOVING:	Lyb_OnMoving( hWnd, (LPRECT)lParam );	return 0;

		default:	break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	���C���{�b�N�X�̃N���G�C�g�B
	@param[in]	hWnd			�e�E�C���h�E�̃n���h��
	@param[in]	lpCreateStruct	�A�v���P�[�V�����̏��������e
	@return	TRUE	���ɂȂ�
*/
BOOLEAN Lyb_OnCreate( HWND hWnd, LPCREATESTRUCT lpCreateStruct )
{
	HINSTANCE	lcInst  = lpCreateStruct->hInstance;	//	�󂯎������������񂩂�A�C���X�^���X�n���h�����Ђ��ς�
	HWND	hToolWnd, hWorkWnd;
	TCHAR	atBuffer[MAX_STRING];
//	UINT	iIndex;
	RECT	tbRect;
//	TBADDBITMAP	stToolBmp;


	hToolWnd = CreateWindowEx( WS_EX_CLIENTEDGE, TOOLBARCLASSNAME, TEXT("toolbar"), WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TOOLTIPS, 0, 0, 0, 0, hWnd, (HMENU)IDW_LYB_TOOL_BAR, lcInst, NULL );

	//	�����c�[���`�b�v�X�^�C����ǉ�
	SendMessage( hToolWnd, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS );

	//stToolBmp.hInst = HINST_COMMCTRL;
	//stToolBmp.nID   = IDB_STD_SMALL_COLOR;
	//iIndex = SendMessage( hToolWnd, TB_ADDBITMAP, 0, (LPARAM)&stToolBmp );
	SendMessage( hToolWnd, TB_SETIMAGELIST, 0, (LPARAM)ghLayerImgLst );

	SendMessage( hToolWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0 );
	//	�c�[���`�b�v�������ݒ�E�{�^���e�L�X�g���c�[���`�b�v�ɂȂ�
	StringCchCopy( atBuffer, MAX_STRING, TEXT("���̕ӂɑ}��") );	gstTBInfo[0].iString = SendMessage( hToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuffer );
	StringCchCopy( atBuffer, MAX_STRING, TEXT("������ɏ㏑") );	gstTBInfo[1].iString = SendMessage( hToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuffer );
	StringCchCopy( atBuffer, MAX_STRING, TEXT("�R�s�[����") );		gstTBInfo[3].iString = SendMessage( hToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuffer );
	StringCchCopy( atBuffer, MAX_STRING, TEXT("�e�L�X�g�ҏW") );	gstTBInfo[5].iString = SendMessage( hToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuffer );
	StringCchCopy( atBuffer, MAX_STRING, TEXT("���e���폜") );		gstTBInfo[7].iString = SendMessage( hToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuffer );

	SendMessage( hToolWnd , TB_ADDBUTTONS, (WPARAM)TB_ITEMS, (LPARAM)&gstTBInfo );	//	�c�[���o�[�Ƀ{�^����}��

	SendMessage( hToolWnd , TB_AUTOSIZE, 0, 0 );	//	�{�^���̃T�C�Y�ɍ��킹�ăc�[���o�[�����T�C�Y
	InvalidateRect( hToolWnd , NULL, TRUE );		//	�N���C�A���g�S�̂��ĕ`�悷�閽��

	//	�c�[���o�[�T�u�N���X��
	gpfOrigLyrTBProc = SubclassWindow( hToolWnd, gpfLayerTBProc );

	//	�\��t���������`�F�b�N�{�b�N�X��t����
	CreateWindowEx( 0, WC_BUTTON, TEXT("�\�t�������"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 150, 2, 138, 23, hToolWnd, (HMENU)IDCB_LAYER_QUICKCLOSE, lcInst, NULL );
	CheckDlgButton( hToolWnd, IDCB_LAYER_QUICKCLOSE, gbQuickClose ? BST_CHECKED : BST_UNCHECKED );

#ifdef EDGE_BLANK_STYLE
	hWorkWnd = CreateWindowEx( 0, WC_COMBOBOX, TEXT(""), WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 290, 0, 123, 70, hToolWnd, (HMENU)IDCB_LAYER_EDGE_BLANK, lcInst, NULL );
	ComboBox_AddString( hWorkWnd, TEXT("���������Ȃ�") );
	ComboBox_AddString( hWorkWnd, TEXT("����������") );
	ComboBox_AddString( hWorkWnd, TEXT("�L��������") );
	ComboBox_SetCurSel( hWorkWnd, 0 );
#endif


	if( 0 == gdBoxID )	//	�ŏ��̈�ڂ̂Ƃ��Ɍv�Z
	{
		//GetClientRect( hToolWnd, &tbRect );
		//gdToolBarHei = tbRect.bottom + 5;
		GetWindowRect( hToolWnd, &tbRect );
		gdToolBarHei = tbRect.bottom - tbRect.top;


		//	�X�N���[���ʒu�͂O�O�Ȃ̂��|�C����
		gstFrmSz.x = 0;
		gstFrmSz.y = gdToolBarHei;
		ClientToScreen( hWnd, &gstFrmSz );
		TRACE( TEXT("%d x %d"), gstFrmSz.x, gstFrmSz.y );
	}

	return TRUE;
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
VOID Lyb_OnCommand( HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify )
{
	LRESULT	lRslt;
	INT		bEdgeBlank;
	INT		iXpos, iYln;

	switch( id )
	{
		case IDE_LYB_TEXTEDIT:	//	���C����\���ɂ��Ă�KILLFOCUS�o��
			if( EN_SETFOCUS  == codeNotify ){	TRACE( TEXT("LYREDIT_SETFOCUS") );	}

			if( EN_KILLFOCUS == codeNotify )
			{
				TRACE( TEXT("LYREDIT_KILLFOCUS") );
				ViewFocusSet(  );
			}
			break;

		case IDM_LYB_INSERT:	//	�\��t����
		case IDM_LYB_OVERRIDE:
			LayerContentsImportable( hWnd, id, &iXpos, &iYln, 0 );
			ViewPosResetCaret( iXpos, iYln );	
			DocPageInfoRenew( -1, 1 );
			if( gbQuickClose  ){	DestroyWindow( hWnd );	}	//	��������H
			break;

		case IDM_LYB_COPY:	//	�N���b�v�{�[�h��
			LayerForClipboard( hWnd, D_UNI );
			break;

		case IDM_LYB_DO_EDIT:	//	�������ҏW
			lRslt = SendMessage( GetDlgItem(hWnd,IDW_LYB_TOOL_BAR), TB_GETSTATE, IDM_LYB_DO_EDIT, 0 );
			LayerEditOnOff( hWnd, (lRslt&TBSTATE_CHECKED) ? TRUE : FALSE );
			SendMessage( GetDlgItem(hWnd,IDW_LYB_TOOL_BAR), TB_SETSTATE, IDM_LYB_INSERT,   (lRslt&TBSTATE_CHECKED) ? 0 : TBSTATE_ENABLED );
			SendMessage( GetDlgItem(hWnd,IDW_LYB_TOOL_BAR), TB_SETSTATE, IDM_LYB_OVERRIDE, (lRslt&TBSTATE_CHECKED) ? 0 : TBSTATE_ENABLED );
			break;

		case IDM_LYB_DELETE:	//	���e���폜
			LayerOnDelete( hWnd );
			break;

		case IDCB_LAYER_QUICKCLOSE:	//	�\��t��������邩�H
			gbQuickClose = IsDlgButtonChecked( GetDlgItem(hWnd,IDW_LYB_TOOL_BAR), IDCB_LAYER_QUICKCLOSE ) ? TRUE : FALSE;
			SetFocus( hWnd );
			break;

#ifdef EDGE_BLANK_STYLE
		case IDCB_LAYER_EDGE_BLANK:	//	���k�L���邩
			if( CBN_SELCHANGE == codeNotify )
			{
				bEdgeBlank = ComboBox_GetCurSel( hWndCtl );
				if( 1 == bEdgeBlank ){			LayerEdgeBlankSizeCheck( hWnd, EDGE_BLANK_NARROW );	}
				else if( 2 ==  bEdgeBlank ){	LayerEdgeBlankSizeCheck( hWnd, EDGE_BLANK_WIDE );	}
			}
			break;
#endif

		case IDM_LYB_TRANCE_RELEASE:	//	���ߑI��������
			LayerTransparentToggle( hWnd, 0 );
			InvalidateRect( hWnd, NULL, TRUE );
			break;

		case IDM_LYB_TRANCE_ALL:	//	�󔒂�S�����ߗ̈�ɐݒ�
			LayerTransparentToggle( hWnd, 1 );
			InvalidateRect( hWnd, NULL, TRUE );
			break;

		default:	TRACE( TEXT("Layer���m�̃R�}���h %d"), id );	break;
	}

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
VOID Lyb_OnKey( HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags )
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

	SetWindowPos( hWnd, HWND_TOP, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	Lyb_OnMoving( hWnd, &rect );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	PAINT�B�����̈悪�o�����Ƃ��ɔ����B�w�i�̈����ɒ��ӁB�w�i��h��Ԃ��Ă���A�I�u�W�F�N�g��`��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Lyb_OnPaint( HWND hWnd )
{
	PAINTSTRUCT	ps;
	HFONT		hFtOld;
	COLORREF	clrTextOld, clrBackOld;
	HDC			hdc;
	INT			height;
	UINT_PTR	iLines, i;
	LPTSTR		ptText;
	RECT		rect;
	LAYER_ITR	itLyr;

	UINT_PTR	mz, cchLen;
	UINT		bStyle, cchMr, cbSize;
	INT			width, rdStart;
	BOOLEAN		doDraw, bRslt;

	hdc = BeginPaint( hWnd, &ps );

#ifdef DO_TRY_CATCH
	try{
#endif

	height = gdToolBarHei;

	for( itLyr = gltLayer.begin(); itLyr != gltLayer.end(); itLyr++ )
	{
		if( itLyr->hBoxWnd == hWnd )
		{
			clrTextOld = SetTextColor( hdc, CLR_BLACK );
			clrBackOld = SetBkColor(   hdc, CLR_WHITE );

			GetClientRect( hWnd, &rect );
			FillRect( hdc, &rect, GetStockBrush( WHITE_BRUSH ) );

			hFtOld = SelectFont( hdc, ghAaFont );

			iLines = itLyr->vcLyrImg.size( );

			for( i = 0; iLines > i; i++ )
			{
				cchLen = itLyr->vcLyrImg.at( i ).vcLine.size(  );	//	�K�v������
				if( 0 >= cchLen ){	height += LINE_HEIGHT;	continue;	}

				cbSize = (cchLen+1) * sizeof(TCHAR);
				ptText = (LPTSTR)malloc( cbSize );	//	�ʂ邽�[�݂ˁ[�������₷
				ZeroMemory( ptText, cbSize );

				bStyle  = itLyr->vcLyrImg.at( i ).vcLine.at( 0 ).mzStyle;
				bStyle &= CT_LYR_TRNC;
				cchMr   = 0;
				width   = 0;
				rdStart = 0;//itLyr->vcLyrImg.at( i ).dOffset;
				doDraw  = FALSE;

				for( mz = 0; cchLen >= mz; mz++ )
				{
					if( cchLen ==  mz ){	doDraw = TRUE;	}	//	���[�܂ł����������
					else
					{
						//	�����X�^�C���������Ȃ�
						if( bStyle == (itLyr->vcLyrImg.at( i ).vcLine.at( mz ).mzStyle & CT_LYR_TRNC) )
						{
							ptText[cchMr++] = itLyr->vcLyrImg.at( i ).vcLine.at( mz ).cchMozi;	//	��q����̕�����Ƃ��Ċm��
							width += itLyr->vcLyrImg.at( i ).vcLine.at( mz ).rdWidth;
						}
						else{	doDraw = TRUE;	}
					}

					if( doDraw )	//	�`��^�C�~���O�ł���Ȃ�
					{
						if( bStyle & CT_LYR_TRNC )	//	���ߕ����̏ꍇ�w�i�F�Ƙg�h��ׂ�
						{
							SetBkColor(   hdc, CLR_SILVER );	//	LTGRAY_BRUSH

							SetRect( &rect, rdStart, height, rdStart + width, height + LINE_HEIGHT );
							FillRect( hdc, &rect, GetStockBrush( LTGRAY_BRUSH ) );
						}
						else
						{
							SetBkColor(   hdc, CLR_WHITE );
						}

						bRslt = ExtTextOut( hdc, rdStart, height, 0, NULL, ptText, cchMr, NULL );
						if( !(bRslt)  ){	TRACE( TEXT("ExtTextOut error") );	}

						if( cchLen != mz )
						{
							rdStart += width;
							//	�`�悵����A���̕�����V�����X�^�C���Ƃ��ēo�^���ă��[�v�ĊJ
							bStyle  = itLyr->vcLyrImg.at( i ).vcLine.at( mz ).mzStyle;
							bStyle &= CT_LYR_TRNC;
							ZeroMemory( ptText, cbSize );
							ptText[0] = itLyr->vcLyrImg.at( i ).vcLine.at( mz ).cchMozi;
							width  = itLyr->vcLyrImg.at( i ).vcLine.at( mz ).rdWidth;
							cchMr  = 1;
						}
						doDraw = FALSE;
					}
				}

				FREE( ptText );

				height += LINE_HEIGHT;
			}

			SelectFont( hdc, hFtOld );

			break;
		}
	}

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	ETC_MSG( err.what(), 0 );	 return;	}
	catch( ... ){	ETC_MSG( ("etc error"), 0 );	 return;	}
#endif

	EndPaint( hWnd, &ps );


	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�����Ƃ��ɔ����B�f�o�C�X�R���e�L�X�g�Ƃ��m�ۂ�����ʍ\���̃������Ƃ����I���B
	@param[in]	hWnd	�E�C���h�E�n���h��
	@return		����
*/
VOID Lyb_OnDestroy( HWND hWnd )
{
	LAYER_ITR	itLyr;

	for( itLyr = gltLayer.begin(); itLyr != gltLayer.end(); itLyr++ )
	{
		if( itLyr->hBoxWnd == hWnd )
		{
			LayerStringObliterate( itLyr );
			MainStatusBarSetText( SB_LAYER, TEXT("") );

			gltLayer.erase( itLyr );

			//SubclassWindow( itLyr->hTextWnd, gpfOrigLyrEditProc );	//	�T�u�N���X�����ɖ߂�

			break;
		}
	}


	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�B���h�E�̃T�C�Y�ύX����������O�ɑ����Ă���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	pstWpos	�V�����ʒu�Ƒ傫���������Ă�
	@return		����Message������������O
*/
BOOL Lyb_OnWindowPosChanging( HWND hWnd, LPWINDOWPOS pstWpos )
{
	INT		clPosY, vwTopY, dSabun, dRem;
	BOOLEAN	bMinus = FALSE;
	RECT	vwRect;


	//TRACE( TEXT("WM_WINDOWPOSCHANGING POS[%d %d] SIZE[%d %d] FLAG[%d]"), 
	//	pstWpos->x, pstWpos->y, pstWpos->cx, pstWpos->cy, pstWpos->flags );

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

//	TRACE( TEXT("NH[%d]"), pstWpos->y );

	return FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�B���h�E�̃T�C�Y�ύX�����������瑗���Ă���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	pstWpos	�V�����ʒu�Ƒ傫���������Ă�
*/
VOID Lyb_OnWindowPosChanged( HWND hWnd, const LPWINDOWPOS pstWpos )
{
	BOOLEAN	bHit = FALSE;
	LAYER_ITR	itLyr;
	RECT	vwRect, rect;

	//TRACE( TEXT("WM_WINDOWPOSCHANGED POS[%d %d] SIZE[%d %d] FLAG[%d]"), 
	//	pstWpos->x, pstWpos->y, pstWpos->cx, pstWpos->cy, pstWpos->flags );


	for( itLyr = gltLayer.begin(); itLyr != gltLayer.end(); itLyr++ )
	{
		if( itLyr->hBoxWnd == hWnd ){	bHit = TRUE;	break;	}
	}
	if( !(bHit) )	return;

	GetClientRect( hWnd, &rect );
	MoveWindow( itLyr->hToolWnd, 0, 0, 0, 0, TRUE );	//	�c�[���o�[�͐��l�Ȃ��Ă�����ɍ��킹�Ă����
	SetWindowPos( itLyr->hTextWnd, HWND_TOP, 0, 0, rect.right, rect.bottom - gdToolBarHei, SWP_NOMOVE | SWP_NOZORDER );

	//	�ړ����Ȃ������Ƃ��͉������Ȃ��ł���
	if( SWP_NOMOVE & pstWpos->flags )	return;

	GetWindowRect( ghViewWnd, &vwRect );
	gstViewOrigin.x = vwRect.left;//�ʒu�L�^�E���������ς����̂���Ȃ�
	gstViewOrigin.y = vwRect.top;

	itLyr->stOffset.x = pstWpos->x - vwRect.left;
	itLyr->stOffset.y = pstWpos->y - vwRect.top;

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	��������Ă���Ƃ��ɔ����E�}�E�X�ŃE�C���h�E�h���b�O���Ƃ�
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	pstPos	���̏u�Ԃ̃X�N���[�����W
*/
VOID Lyb_OnMoving( HWND hWnd, LPRECT pstPos )
{
	LONG	xEt, yEt, xLy, yLy, xSb, ySb;
	LONG	dLine, dRema;
	BOOLEAN	bMinus = FALSE;
	TCHAR	atBuffer[SUB_STRING];

	//	���C���{�b�N�X�E�C���h�E�̃X�N���[�����W����ƉE��
//	TRACE( TEXT("WM_MOVING [L%d T%d R%d B%d]"), pstPos->left, pstPos->top, pstPos->right, pstPos->bottom );
	//	���C���R���e���c�̍���X�N���[�����W
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
	xSb += gdHideXdot;
	dLine += gdViewTopLine;

	StringCchPrintf( atBuffer, SUB_STRING, TEXT("Layer %d[dot] %d[line]"), xSb, dLine );
	MainStatusBarSetText( SB_LAYER, atBuffer );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�}�E�X�̍��{�^�����_�E�����ꂽ�Ƃ��E�_�u���N���b�N�p�E�N���X�X�^�C����CS_DBLCLKS��t���Ȃ��ƃ��b�Z�[�W���Ȃ�
	@param[in]	hWnd			�E�C���h�E�n���h���E�r���[�̂Ƃ͌���Ȃ��̂Œ��ӃZ��
	@param[in]	fDoubleClick	��O�_�u���N���b�N���ꂽ�ꍇ
	@param[in]	x				���������w���W�l
	@param[in]	y				���������x���W�l
	@param[in]	keyFlags		���ɉ�����Ă�L�[�ɂ���
*/
VOID Lyb_OnLButtonDown( HWND hWnd, BOOL fDoubleClick, INT x, INT y, UINT keyFlags )
{
	INT			sy, iDot, iLine;
	LAYER_ITR	itLyr;
	RECT		rect;
	BOOLEAN		bGet = FALSE;


	iDot = x;	//	�ʒu���킹
	sy = y - gdToolBarHei;	if( 0 > sy )	sy = 0;
	iLine = sy / LINE_HEIGHT;

	TRACE( TEXT("�}�E�X�{�^���_�E��[%d][%dx%d(%d)]"), fDoubleClick, iDot, sy, iLine );

	if( !(fDoubleClick) )	 return;	//	�_�u�E�N���b�N�łȂ��Ɨp�͂Ȃ�

	for( itLyr = gltLayer.begin(); itLyr != gltLayer.end(); itLyr++ )
	{
		if( itLyr->hBoxWnd == hWnd )
		{
			bGet = TRUE;	break;
		}
	}
	if( !(bGet) )	 return;	//	�q�b�g���Ȃ������E���蓾�Ȃ��͂�������

	//	�J�[�\���q�b�g�ʒu���A�A���󔒂̕����Ȃ�΃}�[�N�𔽓]������
	if( LayerTransparentAdjust( itLyr, iDot, iLine ) )
	{
		GetClientRect( hWnd, &rect );
		rect.top    = (iLine * LINE_HEIGHT) + gdToolBarHei;
		rect.bottom = rect.top + LINE_HEIGHT;
		InvalidateRect( hWnd, &rect, TRUE );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�R���e�L�X�g���j���[�Ăт����A�N�V����(�v�͉E�N���b�N�j
	@param[in]	hWnd		�E�C���h�E�n���h���E�r���[�̂Ƃ͌���Ȃ��̂Œ��ӃZ��
	@param[in]	hWndContext	�R���e�L�X�g�����������E�C���h�E�̃n���h��
	@param[in]	xPos		�X�N���[���w���W
	@param[in]	yPos		�X�N���[���x����
	@return		����
*/
VOID Lyb_OnContextMenu( HWND hWnd, HWND hWndContext, UINT xPos, UINT yPos )
{
	INT		posX, posY;
	HMENU	hMenu, hSubMenu;
	UINT	dRslt;

	posX = (SHORT)xPos;	//	��ʍ��W�̓}�C�i�X�����肤��
	posY = (SHORT)yPos;

	TRACE( TEXT("LAYER_WM_CONTEXTMENU %d x %d"), posX, posY );

	hMenu = LoadMenu( GetModuleHandle(NULL), MAKEINTRESOURCE(IDM_LAYERBOX_POPUP) );
	hSubMenu = GetSubMenu( hMenu, 0 );

	dRslt = TrackPopupMenu( hSubMenu, 0, posX, posY, 0, hWnd, NULL );	//	TPM_CENTERALIGN | TPM_VCENTERALIGN | 
	DestroyMenu( hMenu );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	���ߍ����G���A��S�I��������S����������
	@param[in]	hWnd	�{�̃E�C���h�E�n���h���E���܂�Ӗ��͂Ȃ�
	@param[in]	bMode	��O�S�I���@�O�S����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT LayerTransparentToggle( HWND hWnd, UINT bMode )
{
	TCHAR	chb;
	INT_PTR	iLines, iL;
	LETR_ITR	itMozi;
	LAYER_ITR	itLyr;

#ifdef DO_TRY_CATCH
	try{
#endif

	for( itLyr = gltLayer.begin(); itLyr != gltLayer.end(); itLyr++ )
	{
		if( itLyr->hBoxWnd == hWnd ){	break;	}
	}
	if( itLyr == gltLayer.end( ) )	return E_OUTOFMEMORY;

	TRACE( TEXT("���ߑI�����������I�� %u"), bMode );

	//	�s���m�F
	iLines = itLyr->vcLyrImg.size(  );

	for( iL = 0; iLines > iL; iL++ )
	{
		//	�������C�e���[�^�Ŋm��
		for( itMozi = itLyr->vcLyrImg.at( iL ).vcLine.begin( );
		itMozi != itLyr->vcLyrImg.at( iL ).vcLine.end( ); itMozi++ )
		{
			if( bMode )
			{
				chb = itMozi->cchMozi;
				if( iswspace( chb ) ){	itMozi->mzStyle |=  CT_LYR_TRNC;	}
			}
			else
			{
				itMozi->mzStyle &= ~CT_LYR_TRNC;
			}
		}
		//	�S������
	}

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (HRESULT)ETC_MSG( err.what(), E_UNEXPECTED );	}
	catch( ... ){	return (HRESULT)ETC_MSG( ("etc error") , E_UNEXPECTED );	}
#endif

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�s���ƃh�b�g�l���󂯎���āA���̏ꏊ��
	@param[in]	itLyr	�Ώۃ��C���{�b�N�X�̃C�e���[�^
	@param[in]	dNowDot	���̃L�����b�g�E
	@param[in]	rdLine	�Ώۂ̍s�ԍ��E��΂O�C���f�b�N�X��
	@return		������
*/
INT LayerTransparentAdjust( LAYER_ITR itLyr, INT dNowDot, INT rdLine )
{
	INT_PTR	i, iCount, iLines, iLetter;
	INT		dDotCnt = 0, dPrvCnt = 0, rdWidth = 0;
	TCHAR	ch, chb;
	LETR_ITR	itMozi, itHead, itTail, itTemp;

#ifdef DO_TRY_CATCH
	try{
#endif

	//	�s�̂͂ݏo�����H
	iLines = itLyr->vcLyrImg.size(  );
	if( 0 >= iLines )	return 0;
	if( iLines <= rdLine )	return 0;

	//	�������m�F
	iCount = itLyr->vcLyrImg.at( rdLine ).vcLine.size(  );
	if( 0 >= iCount )	return 0;

	//	�������C�e���[�^�Ŋm��
	itMozi = itLyr->vcLyrImg.at( rdLine ).vcLine.begin( );

	for( i = 0, iLetter = 0; iCount > i; i++, iLetter++ )
	{
		if( dNowDot <= dDotCnt ){	break;	}

		dPrvCnt = dDotCnt;
		rdWidth = itLyr->vcLyrImg.at( rdLine ).vcLine.at( i ).rdWidth;
		dDotCnt += rdWidth;
	}	//	�U��؂�悤�Ȃ疖�[

	if( iCount <= iLetter )	return 0;

	if(  1 <= iLetter )	//	�������Ŕ���
	{
		iLetter--;
		itMozi += iLetter;
	}

	ch = itLyr->vcLyrImg.at( rdLine ).vcLine.at( iLetter ).cchMozi;
	//	�Y���ӏ��̕������m�F����
	if( !( iswspace( ch ) ) )	return 0;
	//	�󔒂łȂ��Ȃ牽�����Ȃ��ł���

	//	���̏ꏊ���瓪�����ɒH���āA�r�؂�ڂ�T��
	itHead = itLyr->vcLyrImg.at( rdLine ).vcLine.begin( );
	for( ; itHead != itMozi; itMozi-- )
	{
		chb = itMozi->cchMozi;
		if(  !( iswspace( chb ) ) ){	itMozi++;	break;	}
	}
	if( itHead == itMozi )	//	�擪���m�F
	{
		chb = itMozi->cchMozi;
		if(  !( iswspace( chb ) ) ){	itMozi++;	}
	}
	//	��󔒕����Ƀq�b�g�������A�擪�ʒu�ł���


	//	���̏ꏊ����A�����O���[�v�̏��܂Ŋm�F
	itTail = itLyr->vcLyrImg.at( rdLine ).vcLine.end( );
	for( itTemp = itMozi; itTemp != itTail; itTemp++ )
	{
		chb = itTemp->cchMozi;	//	�󔒂ł���Ԃ�
		if(  !( iswspace( chb ) ) ){	break;	}

		itTemp->mzStyle ^= CT_LYR_TRNC;
	}

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return ETC_MSG( err.what(), 0 );	}
	catch( ... ){	return  ETC_MSG( ("etc error"), 0 );	}
#endif

	return iLetter;
}
//-------------------------------------------------------------------------------------------------

/*!
	�r���[���ړ�����
	@param[in]	hWnd	�{�̃E�C���h�E�n���h���E���܂�Ӗ��͂Ȃ�
	@param[in]	state	����ԁE�ŏ����Ȃ�Ⴄ�R�g����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT LayerMoveFromView( HWND hWnd, UINT state )
{
	LAYER_ITR	itLyr;
	RECT	vwRect = {0,0,0,0};
	POINT	lyPoint;

	//	�ŏ������͔�\���ɂ���Ƃ�	SIZE_MINIMIZED

	if( SIZE_MINIMIZED != state )
	{
		GetWindowRect( ghViewWnd, &vwRect );
		gstViewOrigin.x = vwRect.left;//�ʒu�L�^
		gstViewOrigin.y = vwRect.top;
	}

	for( itLyr = gltLayer.begin(); itLyr != gltLayer.end(); itLyr++ )
	{
		if( SIZE_MINIMIZED == state )
		{
			ShowWindow( itLyr->hBoxWnd, SW_HIDE );
		}
		else
		{
	//		ShowWindow( itLyr->hBoxWnd, SW_SHOW );

			lyPoint.x = itLyr->stOffset.x + vwRect.left;
			lyPoint.y = itLyr->stOffset.y + vwRect.top;
	#ifdef _DEBUG
			SetWindowPos( itLyr->hBoxWnd, HWND_TOP, lyPoint.x, lyPoint.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW );
	#else
			SetWindowPos( itLyr->hBoxWnd, HWND_TOPMOST, lyPoint.x, lyPoint.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW );
	#endif
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�s�̓��e�𕶎���Ŋm�ہE�|�C���^�J���͌Ă񂾂ق��ł��
	@param[in]	itLyr	�ێ����Ă�C�e���[�^
	@param[in]	il		�s�ԍ�
	@return		LPTSTR	������̂ۂ��񂽁`
*/
LPTSTR LayerLineTextGetAlloc( LAYER_ITR itLyr, INT il )
{
	UINT_PTR	cchSize, i = 0;
	LPTSTR	ptText;

	cchSize = itLyr->vcLyrImg.at( il ).vcLine.size( );
	if( 0 >= cchSize )	return NULL;

	ptText = (LPTSTR)malloc( (cchSize+1) * sizeof(TCHAR) );	//	�ʂ邽�[�݂ˁ[�������₷
	ZeroMemory( ptText, (cchSize+1) * sizeof(TCHAR) );

	for( i = 0; cchSize > i; i++ )
	{
		ptText[i] = itLyr->vcLyrImg.at( il ).vcLine.at( i ).cchMozi;
	}

	return ptText;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Ώۂ̃��C���{�b�N�X�̕ێ����Ă镶�����j�󂷂�
	@param[in]	itLyr	�Ώۂ̃��C���{�b�N�X�������C�e���[�^
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT LayerStringObliterate( LAYER_ITR itLyr )
{
	UINT_PTR	j, iLine;

	iLine = itLyr->vcLyrImg.size( );
	for( j = 0; iLine > j; j++ )
	{
		itLyr->vcLyrImg.at( j ).vcLine.clear( );	//	�e�s�̒��g�S����
	}
	itLyr->vcLyrImg.clear(  );	//	�s��S����

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C���{�b�N�X�̒��g�̕ҏW��ON/OFF
	@param[in]	hWnd	���C���{�b�N�X�̃E�C���h�E�n���h��
	@param[in]	dStyle	��O�ҏW����@�O�I��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT LayerEditOnOff( HWND hWnd, UINT dStyle )
{
	UINT_PTR	i, iLines;
	INT			ndx;
	UINT		cchSize;
	LPTSTR		ptStr;
	ONELINE		stLine;
	LAYER_ITR	itLyr;


	for( itLyr = gltLayer.begin(); itLyr != gltLayer.end(); itLyr++ )
	{
		if( itLyr->hBoxWnd == hWnd )
		{
			if( dStyle )	//	�ҏW����
			{
		//		SetLayeredWindowAttributes( hWnd, 0, 0xFF, LWA_ALPHA );
				Edit_SetText( itLyr->hTextWnd, TEXT("") );

				SetFocus( itLyr->hTextWnd );

				iLines = itLyr->vcLyrImg.size( );
				for( i = 0; iLines > i; i++ )
				{
					if( 0 != i )	//	���̍s�ɐi�ނ悤�Ȃ���s����Ƃ�
					{
						ndx = GetWindowTextLength( itLyr->hTextWnd );
						SendMessage( itLyr->hTextWnd, EM_SETSEL, ndx, ndx );
						SendMessage( itLyr->hTextWnd, EM_REPLACESEL, 0, (LPARAM)(CH_CRLFW) );
					}

					//	��s�����g�������
					ptStr = LayerLineTextGetAlloc( itLyr, i );
					if( ptStr )
					{
						ndx = GetWindowTextLength( itLyr->hTextWnd );
						SendMessage( itLyr->hTextWnd, EM_SETSEL, ndx, ndx );
						SendMessage( itLyr->hTextWnd, EM_REPLACESEL, 0, (LPARAM)ptStr );

						FREE(ptStr);
					}
				}

				ShowWindow( itLyr->hTextWnd, SW_SHOW );
			}
			else	//	�I��
			{
				ndx = Edit_GetTextLength( itLyr->hTextWnd );
				ndx += 2;	//	�ʂ邽�݂˂���
				ptStr = (LPTSTR)malloc( ndx * sizeof(TCHAR) );
				ZeroMemory( ptStr, ndx * sizeof(TCHAR) );
				Edit_GetText( itLyr->hTextWnd, ptStr, ndx );
				ShowWindow( itLyr->hTextWnd, SW_HIDE );

				StringCchLength( ptStr, STRSAFE_MAX_CCH, &cchSize );

				LayerStringObliterate( itLyr );
				ZeroONELINE( &stLine );
				itLyr->vcLyrImg.push_back( stLine );	//	�딭��

				LayerBoxSetString( itLyr, ptStr, cchSize, NULL, 0x00 );

				FREE(ptStr);

				InvalidateRect( hWnd, NULL, TRUE );
		//		SetLayeredWindowAttributes( hWnd, 0, gbAlpha, LWA_ALPHA );
			}

			break;
		}
	}

	UpdateWindow( hWnd );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C���{�b�N�X�̓��e�����ւ���E�O������H
	@param[in]	hLyrWnd	���C���{�b�L�X�E�C���h�E�̃n���h��
	@param[in]	ptStr	�\�����镶����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT LayerStringReplace( HWND hLyrWnd, LPTSTR ptStr )
{
	UINT		cchSize;
	ONELINE		stLine;
	LAYER_ITR	itLyr;

	//	�C�e���[�^�ŒT���ď�������
	for( itLyr = gltLayer.begin(); itLyr != gltLayer.end(); itLyr++ )
	{
		if( itLyr->hBoxWnd == hLyrWnd )
		{
			StringCchLength( ptStr, STRSAFE_MAX_CCH, &cchSize );

			LayerStringObliterate( itLyr );
			ZeroONELINE( &stLine );
			itLyr->vcLyrImg.push_back( stLine );	//	�딭��

			LayerBoxSetString( itLyr, ptStr, cchSize, NULL, 0x00 );

			break;
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�������\�����e�ɂ���
	@param[in]	itLyr	���C���{�b�L�X�̃C�e���[�^
	@param[in]	ptStr	�\�����镶����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT LayerFromString( LAYER_ITR itLyr, LPCTSTR ptStr )
{
	UINT	cchSize;
	ONELINE	stLine;

	ZeroONELINE( &stLine );

	itLyr->vcLyrImg.push_back( stLine );	//	�딭��

	StringCchLength( ptStr, STRSAFE_MAX_CCH, &cchSize );

	LayerBoxSetString( itLyr, ptStr, cchSize, NULL, 0x01 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�\�����e��I��͈͂��璸�Ղ���
	@param[in]	itLyr	���C���{�b�L�X�̃C�e���[�^
	@param[in]	bSqSel	��`�I�𒆂ł��邩
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT LayerFromSelectArea( LAYER_ITR itLyr, UINT bSqSel )
{
	LPTSTR	ptString = NULL;
	UINT	cchSize, cbSize;
	LPPOINT	pstPos;
	ONELINE	stLine;

	TRACE( TEXT("�I��͈͂���擾") );
#ifdef DO_TRY_CATCH
	try{
#endif

	ZeroONELINE( &stLine );

	itLyr->vcLyrImg.push_back( stLine );	//	�딭��

	cbSize = DocSelectTextGetAlloc( D_UNI | bSqSel, (LPVOID *)(&ptString), &pstPos );

	StringCchLength( ptString, STRSAFE_MAX_CCH, &cchSize );

	LayerBoxSetString( itLyr, ptString, cchSize, (bSqSel & D_SQUARE) ? pstPos : NULL, 0x01 );

	FREE(ptString);
	FREE(pstPos);

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (HRESULT)ETC_MSG( err.what(), E_UNEXPECTED );	}
	catch( ... ){	return (HRESULT)ETC_MSG( ("etc error") , E_UNEXPECTED );	}
#endif
	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�\�����e��������Ղځ[�ǂ��璸�Ղ���
	@param[in]	itLyr	�ێ����Ă�C�e���[�^
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT LayerFromClipboard( LAYER_ITR itLyr )
{
	LPTSTR	ptString = NULL;
	UINT	cchSize, dStyle;//, i;
//	INT		insDot, yLine;
	ONELINE	stLine;

	ZeroONELINE( &stLine );

	itLyr->vcLyrImg.push_back( stLine );	//	�딭��

	//	������؂ځ[�ǂ���̏ꍇ�́A��`�ł��֌W�Ȃ�
	ptString = DocClipboardDataGet( &dStyle );

	StringCchLength( ptString, STRSAFE_MAX_CCH, &cchSize );

	LayerBoxSetString( itLyr, ptString, cchSize, NULL, 0x01 );

	FREE( ptString );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�{�b�N�X���e�ɍ��킹�ăT�C�Y�L����
	@param[in]	itLyr	�ێ����Ă�C�e���[�^
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT LayerBoxSizeAdjust( LAYER_ITR itLyr )
{
	INT	dViewXdot, dYline, dViewYdot;
	INT	iMaxDot = 0, iYdot;
	INT_PTR	iLine, i;
	SIZE	wdSize, tgtSize;//clSize

#ifdef DO_TRY_CATCH
	try{
#endif
	//	�ŏ��T�C�Y���Ă��Ƃ�
	//clSize.cx = LB_WIDTH  - gstFrmSz.x;
	//clSize.cy = LB_HEIGHT - gstFrmSz.y;

	//	���̉�ʂ̍s���ƃh�b�g���m�F
	dYline = ViewAreaSizeGet( &dViewXdot );
	dViewYdot = dYline * LINE_HEIGHT;

	//	�g���Ă�����e����T�C�Y���m�F
	iLine = itLyr->vcLyrImg.size(  );
	iYdot = iLine * LINE_HEIGHT;
	for( i = 0; iLine > i; i++ )	//	�ő�h�b�g�����m�F
	{
		if( iMaxDot < itLyr->vcLyrImg.at( i ).iDotCnt ){	iMaxDot = itLyr->vcLyrImg.at( i ).iDotCnt;	}
	}
	//	�����E�C���h�E�T�C�Y�ɂȂ�͂�
	wdSize.cx = gstFrmSz.x + iMaxDot + gstFrmSz.x;
	wdSize.cy = gstFrmSz.y + iYdot + gstFrmSz.x;

	if( LB_WIDTH >  wdSize.cx ){	tgtSize.cx = LB_WIDTH;	}
	else if( dViewXdot < wdSize.cx ){	tgtSize.cx =  dViewXdot;	}
	else{	tgtSize.cx =  wdSize.cx;	}

	if( LB_HEIGHT > wdSize.cy ){	tgtSize.cy =  LB_HEIGHT;	}
	else if( dViewYdot < wdSize.cy ){	tgtSize.cy =  dViewYdot;	}
	else{	tgtSize.cy =  wdSize.cy;	}

#ifdef _DEBUG
	SetWindowPos( itLyr->hBoxWnd, HWND_TOP, 0, 0, tgtSize.cx, tgtSize.cy, SWP_NOMOVE | SWP_NOZORDER );
#else
	SetWindowPos( itLyr->hBoxWnd, HWND_TOPMOST, 0, 0, tgtSize.cx, tgtSize.cy, SWP_NOMOVE | SWP_NOZORDER );
#endif

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (HRESULT)ETC_MSG( err.what(), E_UNEXPECTED );	}
	catch( ... ){	return (HRESULT)ETC_MSG( ("etc error") , E_UNEXPECTED );	}
#endif

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C���{�b�N�X�ɕ�������L�^����
	@param[in]	itLyr	�Ώۃ��C���{�b�L�X�̃C�e���[�^
	@param[in]	ptText	�L�^���镶����
	@param[in]	cchSize	������̕�����
	@param[in]	pstPt	��`�̂Ƃ��̃I�t�Z�b�g��
	@param[in]	bStyle	��O���e�ɍ��킹�ăT�C�Y�ύX�@�O�i�j�����Ȃ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT LayerBoxSetString( LAYER_ITR itLyr, LPCTSTR ptText, UINT cchSize, LPPOINT pstPt, UINT bStyle )
{
	UINT_PTR	i, j, iLine, iTexts;
	LONG	dMin = 0;
	INT		insDot, yLine, dSpDot, dSpMozi, iLines = 0, dOffset;
	LPTSTR	ptBuff, ptSpace = NULL;
	ONELINE	stLine;

#ifdef DO_TRY_CATCH
	try{
#endif
	ZeroONELINE( &stLine );

	//	�I�t�Z�b�g�ݒ肪�L��ꍇ�A���̕��𖄂߂�󔒂��K�v
	if( pstPt )	//	�ŏ��I�t�Z�b�g�l��T���āA���������[�ɂ���
	{
		dMin = pstPt[0].x;

		yLine = 0;
		for( i = 0; cchSize > i; i++ )
		{
			if( CC_CR == ptText[i] && CC_LF == ptText[i+1] )	//	���s�ł�������
			{
				//	�I�t�Z�b�g�ŏ���������
				if( dMin > pstPt[yLine].x ){	dMin = pstPt[yLine].x;	}

				i++;		//	0x0D,0x0A������A�땶����΂��̂��|�C���g
				yLine++;	//	���s��������Focus�͎��̍s��
			}
		}
		//	���̎��_�ŁAyLine�͍s���ɂȂ��Ă�
		iLines = yLine;

		//	��s�ڂ̋󔒂�����đM�����͂��Ă���
		insDot = 0;
		dOffset = pstPt[0].x - dMin;
		ptSpace = DocPaddingSpaceUni( dOffset, NULL, NULL, NULL );
		//	�O���󔒂͖��������̂Ń��j�R�[�h�g���Ė�薳��
		StringCchLength( ptSpace, STRSAFE_MAX_CCH, &iTexts );
		for( j = 0; iTexts > j; j++ )
		{
			insDot += LayerInputLetter( itLyr, insDot, 0, ptSpace[j] );
		}
		FREE(ptSpace);
	}

	yLine = 0;	insDot = 0;
	for( i = 0; cchSize > i; i++ )
	{
		if( CC_CR == ptText[i] && CC_LF == ptText[i+1] )	//	���s�ł�������
		{
			itLyr->vcLyrImg.push_back( stLine );	//	���̍s�����

			i++;		//	0x0D,0x0A������A�땶����΂��̂��|�C���g
			yLine++;	//	���s��������Focus�͎��̍s��
			insDot = 0;	//	�����čs�̐擪�ł���

			//	�I�t�Z�b�g���̋󔒂����
			if( pstPt && (iLines > yLine) )
			{
				dOffset = pstPt[yLine].x - dMin;
				ptSpace = DocPaddingSpaceUni( dOffset, NULL, NULL, NULL );
				//	�O���󔒂͖��������̂Ń��j�R�[�h�g���Ė�薳��
				StringCchLength( ptSpace, STRSAFE_MAX_CCH, &iTexts );
				for( j = 0; iTexts > j; j++ )
				{
					insDot += LayerInputLetter( itLyr, insDot, yLine, ptSpace[j] );
				}
				FREE(ptSpace);
			}

		}
		else if( CC_TAB == ptText[i] )
		{
			//	�^�u�͑}�����Ȃ�
		}
		else
		{
			insDot += LayerInputLetter( itLyr, insDot, yLine, ptText[i] );
		}
	}

	//	�������`�ƑO���󔒊m�F
	iLine = itLyr->vcLyrImg.size( );
	for( i = 0; iLine > i; i++ )
	{
		//	���[�󔒍폜
		ptBuff = DocLastSpDel( &(itLyr->vcLyrImg.at( i ).vcLine) );
		FREE(ptBuff);	//	�g��Ȃ����A�󂯂ĊJ�����Ȃ��ƃC�P�Ȃ�

		//	�擪�󔒊m�F
		dSpMozi = 0;
		dSpDot = LayerHeadSpaceCheck( &(itLyr->vcLyrImg.at( i ).vcLine), &dSpMozi );

		itLyr->vcLyrImg.at( i ).dFrtSpDot  = dSpDot;
		itLyr->vcLyrImg.at( i ).dFrtSpMozi = dSpMozi;

		//	��`�I�t�Z�b�g
		//if( pstPt ){	itLyr->vcLyrImg.at( i ).dOffset = pstPt[i].x - dMin;	}
	}

	//	�T�C�Y����
	if( bStyle ){	LayerBoxSizeAdjust( itLyr );	}

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (HRESULT)ETC_MSG( err.what(), E_UNEXPECTED );	}
	catch( ... ){	return (HRESULT)ETC_MSG( ("etc error") , E_UNEXPECTED );	}
#endif

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�s���󔒊m�F�E�s���I�h�͋󔒂ƌ��Ȃ��E�擪�̂݁A�Ƃ��I�v�\���ł��邩��
	@param[in]	*vcTgLine	�Y������s�̃x�N�^�[�ւ̃|�C���^�`
	@param[out]	pdMozi		������������|�C���^�`
	@return	��󔒂Ɏ���܂ł̃h�b�g��
*/
INT LayerHeadSpaceCheck( vector<LETTER> *vcTgLine, PINT pdMozi )
{
	TCHAR		ch;
	INT			cchSp, dDot;	//	�������ƃh�b�g��
	UINT_PTR	i, iMozi;

#ifdef DO_TRY_CATCH
	try{
#endif
	iMozi = vcTgLine->size(  );

	dDot = 0;	cchSp = 0;
	for( i = 0; iMozi > i; i++ )
	{
		ch = vcTgLine->at( i ).cchMozi;

		//	�����X�y�[�X�ł��s���I�h�ł��Ȃ��Ȃ�A�]���͂����܂�
		if( !( iswspace(ch) ) && TEXT('.') != ch )
		{
			if( pdMozi ){	*pdMozi = cchSp;	}
			return dDot;
		}

		dDot += vcTgLine->at( i ).rdWidth;	//	�h�b�g��
		cchSp++;	//	������
	}

	if( pdMozi ){	*pdMozi = cchSp;	}
#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return ETC_MSG( err.what(), 0 );	}
	catch( ... ){	return  ETC_MSG( ("etc error"), 0 );	}
#endif

	return dDot;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��s�̃h�b�g�ʒu(�L�����b�g�ʒu)�Ɉ땶���ǉ�����
	@param[in]	itLyr	�Ώۃ��C���{�b�L�X�̃C�e���[�^
	@param[in]	nowDot	�}������h�b�g�ʒu�E�g���ĂȂ�
	@param[in]	rdLine	�Ώۂ̍s�ԍ��E��΂O�C���f�b�N�X��
	@param[in]	ch		�ǉ�����������
	@return		INT		�ǉ����������̃h�b�g��
*/
INT LayerInputLetter( LAYER_ITR itLyr, INT nowDot, INT rdLine, TCHAR ch )
{
	LETTER	stLetter;
//	INT		iRslt;

#ifdef DO_TRY_CATCH
	try{
#endif
	//	�f�[�^�쐬
	DocLetterDataCheck( &stLetter, ch );	//	�w��s�̃h�b�g�ʒu(�L�����b�g�ʒu)�Ɉ땶���ǉ�����E���C���{�b�N�X

	itLyr->vcLyrImg.at( rdLine ).vcLine.push_back( stLetter );

	itLyr->vcLyrImg.at( rdLine ).iDotCnt += stLetter.rdWidth;
	itLyr->vcLyrImg.at( rdLine ).iByteSz += stLetter.mzByte;

//	iRslt = DocBadSpaceCheck( rdLine );

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return ETC_MSG( err.what(), 0 );	}
	catch( ... ){	return  ETC_MSG( ("etc error"), 0 );	}
#endif

	return stLetter.rdWidth;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Y���G���A�ɏ㏑��������}��������
	@param[in]	hWnd	�{�b�N�X�̃E�C���h�E�n���h��
	@param[in]	cmdID	�}�����㏑����
	@param[out]	pXdot	���ꍞ�݈ʒu�w�h�b�g�ENULL�ł���
	@param[out]	pYline	���ꍞ�݈ʒu�x���C���ENULL�ł���
	@param[in]	dStyle	�s�����ʏ������H
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT LayerContentsImportable( HWND hWnd, UINT cmdID, LPINT pXdot, LPINT pYline, UINT dStyle )
{
	RECT		vwRect, lyRect;
	POINT		conPoint;
	INT			xTgDot, xDot, iSrcDot, iSabun, iDivid, iSpDot;
	INT			dGap, dInLen, dInBgn, dInEnd, dEndot;
	INT			dLeft, dRight;
	INT			iPageLine, yTgLine, dWkLine, dLyLine;
	INT			iMinus, iMozi, iStMozi, iEdMozi;
	INT			dBkLeft, dBkRight, dBkStMozi, dBkEdMozi;
	INT_PTR		dNeedLine;
	UINT_PTR	cchSize;
	LPTSTR		ptStr, ptBuffer;
	BOOLEAN		bFirst = TRUE;	//	�Ȃ񂩏���������FALSE
	BOOLEAN		bSpace, bBkSpase;

#ifdef EDGE_BLANK_STYLE
	INT		bEdgeBlank;
	INT		xDotEx, iMoziEx;
#endif
	LAYER_ITR	itLyr;

	LETR_ITR	itLtr, itDel;
	wstring	wsBuff;

#ifdef DO_TRY_CATCH
	try{
#endif

	for( itLyr = gltLayer.begin(); itLyr != gltLayer.end(); itLyr++ )
	{
		if( itLyr->hBoxWnd == hWnd ){	break;	}
	}
	if( itLyr == gltLayer.end( ) )	return E_OUTOFMEMORY;


	//	�܂��ꏊ���m�F
	GetWindowRect( ghViewWnd, &vwRect );
	vwRect.left += LINENUM_WID;
	vwRect.top  += RULER_AREA;

	GetWindowRect( itLyr->hBoxWnd, &lyRect );
	conPoint.x = lyRect.left + gstFrmSz.x;
	conPoint.y = lyRect.top  + gstFrmSz.y;
	//	�����ɂ͂ݏo���Ă���A�����̓}�C�i�X�ɂȂ��Ă���
	xTgDot   =  conPoint.x - vwRect.left;
	yTgLine  =  conPoint.y - vwRect.top;

	yTgLine /= LINE_HEIGHT;

	//	20110704	���̎��_�ł́A�X�N���[���ɂ��Y�����l������ĂȂ�
	xTgDot  += gdHideXdot;
	yTgLine += gdViewTopLine;
	//	��������ő��v

	xDot = xTgDot;

	TRACE( TEXT("LAYER IMPORT[%d:%d]"), xTgDot, yTgLine );

	if( pXdot  )	*pXdot  = xTgDot;
	if( pYline )	*pYline = yTgLine;


	//	�g���s���m�F
	dNeedLine = itLyr->vcLyrImg.size( );
	//	�ŏI�s�̋󔒊m�F
	ptStr = LayerLineTextGetAlloc( itLyr, dNeedLine - 1 );
	if( !(ptStr) )	dNeedLine--;	//	�Ō�󔒂Ȃ�g��Ȃ�
	FREE(ptStr);

	iPageLine = DocPageParamGet( NULL, NULL );	//	���̕ł̍s���m�F�E����ւ��Ă����邩

	//	�S�̍s�����A�ǉ��s��������������A���s���₷
	if( iPageLine < (dNeedLine + yTgLine) )
	{
		iMinus = ( dNeedLine + yTgLine ) - iPageLine;	//	�ǉ�����s��
		DocAdditionalLine( iMinus, &bFirst );//	bFirst = FALSE;
		TRACE( TEXT("ADD LINE[%d]"), iMinus );
	}

	//	���k�L����ɂ́A�O��̋󔒕����ʂ𑝂₹�΂���
	//	���ߗ̈悪�����ꍇ�́A�񓧉߂Ƃ���E��ɃX�L�������邩�B
	bEdgeBlank = ComboBox_GetCurSel( GetDlgItem( GetDlgItem(hWnd,IDW_LYB_TOOL_BAR), IDCB_LAYER_EDGE_BLANK ) );
	if( 1 == bEdgeBlank ){			LayerEdgeBlankSizeCheck( hWnd, EDGE_BLANK_NARROW );	}
	else if( 2 ==  bEdgeBlank ){	LayerEdgeBlankSizeCheck( hWnd, EDGE_BLANK_WIDE );	}


	//���k�L����ɂ͋������ߗ̈������


//�e�s���ɑ}���ʒu���݂āA�����悯�āA�X�L�}�𖄂߂�
//�����܂łɑ���Ȃ��Ȃ�p�f�B���O�E���C������Offset�ƍs���s�B�����h�`���l��
//�㏑���̏ꍇ�́A�h�b�g���ɍ��킹�ăX�L�}�����āA�O��Y���Ȃ��悤��
//�}���E�㏑��������̏ꍇ���A���C�����̑O�󔒂��l�����ăp�f�B���O����
	for( dWkLine = yTgLine, dLyLine = 0; (yTgLine+dNeedLine) > dWkLine; dWkLine++, dLyLine++ )
	{
		if( 0 > dWkLine )	continue;	//	��ɂ߂荞��ł�̂͏��������Ⴂ����

		TRACE( TEXT("Check Line V[%d] L[%d]"), dWkLine, dLyLine );

		//	�}�����e�̈ʒu�̊m�F�E�����ŁA�e�������ɂ΂点�΂����B
		//	�s�P�ʂł͂Ȃ��A���ߗ̈�ŋ�؂�ꂽ�����̈斈�ɔ��肷��

		//	dLyLine�F���C�����̍s�ԍ��@dWkLine�F�r���[�̍s�ԍ�
		iSpDot  = itLyr->vcLyrImg.at( dLyLine ).dFrtSpDot;	//	���C�����h�b�g�I�t�Z�b�g
		//	�s������A���߂Ĕ�󔒂��o�Ă���h�b�g

		xDot   = xTgDot + iSpDot;	//	���C�����I�u�W�F�N�g��}���ʒu
		//	�}�C�i�X�������ꍇ���C����������̊J�n�ʒu�����炷

		//	�K�v�ȏ��𒊏o�E�g�p�o�C�g�����m�F���Ă���
		itLtr  = itLyr->vcLyrImg.at( dLyLine ).vcLine.begin( );
		itLtr += itLyr->vcLyrImg.at( dLyLine ).dFrtSpMozi;	//	�󔒈ȊO�̊J�n�ʒu
		//�����ŊJ�n�ʒu�܂ł��炵�Ă���

		//	��s���󋵂��݂Ȃ���}�����Ă���
		while( itLtr != itLyr->vcLyrImg.at( dLyLine ).vcLine.end( ) )
		{
			while( 0 > xDot )	//	�}�C�i�X��������{�ɂȂ�܂ł��炵�Ă���
			{
				//	�Y���s�̖����܂ŃC�b����I��
				if( itLtr == itLyr->vcLyrImg.at( dLyLine ).vcLine.end( ) )	break;

				//���߃t���O������ΏI��
				if( itLtr->mzStyle & CT_LYR_TRNC )	break;

				xDot   += itLtr->rdWidth;
				iSpDot += itLtr->rdWidth;

				itLtr++;
			}

			//	�}�����e�̊m��
			wsBuff.clear( );	dInLen = 0;
			for(  ; itLtr != itLyr->vcLyrImg.at( dLyLine ).vcLine.end( ); itLtr++ )
			{
				//���߃t���O������ΏI��
				if( itLtr->mzStyle & CT_LYR_TRNC )	break;

				wsBuff += itLtr->cchMozi;	dInLen += itLtr->rdWidth;
			}	//	��������I���܂�

			if( 0 != dInLen )	//	�}���ł�����e���Ȃ�������Ȃɂ�����ł���
			{
				cchSize = wsBuff.size( ) + 1;	//	dInLen�F�}�����e�̃h�b�g��
				ptStr = (LPTSTR)malloc( cchSize * sizeof(TCHAR) );
				StringCchCopy( ptStr, cchSize, wsBuff.c_str( ) );

				dGap = 0;

				//	�}���ʒu�̒���
				iSrcDot = DocLineParamGet( dWkLine, NULL, NULL );	//	�}���s�̖��[�h�b�g�ʒu
				iSabun  = xTgDot - iSrcDot;	//	�{�Ȃ瑫��ĂȂ�
				iDivid  = iSabun + iSpDot;	//	���C�������l��
				if( 0 < iDivid )	//	�s���[����ɂ��Ă�
				{
					xDot = iSrcDot;
					ptBuffer = DocPaddingSpaceWithPeriod( iDivid, NULL, NULL, NULL, TRUE );
					//	�s���[���烌�C�����I�u�W�F�N�g�܂ł𖄂߂��
					if( ptBuffer )
					{
						DocInsertString( &xDot, &dWkLine, NULL, ptBuffer, dStyle, bFirst );	bFirst = FALSE;
						FREE(ptBuffer);
					}

					//�]�T������Ȃ�A���k�L�͂��܂�֌W�Ȃ����H
				}
				else if( 0 > iDivid )	//	�����̕�����̂ق��������ꍇ
				{
					//	���̒n�_�̏󋵂��m�F���āA�󔒃G���A�Ȃ疄�߂Ɏg��
					//	�����G���A�Ȃ�A���߂���p�f�B���O�ł���Ƃ���܂ł𖄂ߒ���
					iMozi = DocLetterPosGetAdjust( &xDot, dWkLine, -1 );	//	���̕����ʒu���m�F
				//	iMozi�F�}���ʒu������			xDot�F������}���ʒu�h�b�g

#ifndef EDGE_BLANK_STYLE
					//	�����̕������󔒂��A�󔒂Ȃ�ǂ��܂ő����Ă邩�m�F
					DocLineStateCheckWithDot( xDot, dWkLine, &dLeft, &dRight, &iStMozi, NULL, &bSpace );
					//	dRight �g���ĂȂ�
#endif
					//	��ɏ㏑���G���A�̏������Ȃ��ƁA�p�f�B���O�������炸���
					//	�㏑���̏ꍇ��������������ɍ폜���ăM���b�v�p�f�B���O
					if( IDM_LYB_OVERRIDE == cmdID )
					{
						dInBgn  = xTgDot + iSpDot;	//	�{�b�N�X���[�{�����I�t�Z�b�g��������J�n�ʒu
						dInEnd  = dInBgn + dInLen;	//	�J�n�ʒu�{�����񕝁�������I�[�ʒu

//�������ɂ́A�����Ŕ͈͂��L����΂����H
//���ߗ̈�̕��Ɗ������ޔ͈͂ɂ���ẮA�O�������������މ\���H
//��Ƀs�^���̈ʒu�ɍ��킹�Ă���̂ŁA���͖����H

						//	20110817	���X�Y�����o��̂��C���������菇�ԈႢ
						dEndot  = dInEnd;
#ifdef EDGE_BLANK_STYLE
						//	������ dEndot ���I�t�Z�b�g����H
						if( 1 == bEdgeBlank ){		dEndot += EDGE_BLANK_NARROW;	}
						else if( 2 == bEdgeBlank ){	dEndot += EDGE_BLANK_WIDE;	}
#endif
						iEdMozi = DocLetterPosGetAdjust( &dEndot , dWkLine, 1 );	//	�㏑�������̈�
						//	�L�����b�g�ʒu�C��

						//	�㔼�́A���܂��Ă�Space���l�����ăp�f�B���O����
						DocLineStateCheckWithDot( dEndot, dWkLine, &dBkLeft, &dBkRight, &dBkStMozi, &dBkEdMozi, &bBkSpase );
						if( bBkSpase )	//	�㔼�̋󔒍ĕ�
						{
							dEndot  = dBkRight;
							iEdMozi = DocLetterPosGetAdjust( &dEndot , dWkLine, 1 );	//	�㏑�������̈�
							//���͗v��Ȃ�������
							dGap    = dBkRight - dInEnd;	//	���̕����̈ێ��̂��߂̃M���b�v��
							//	dBkRight�F�󔒈ʒu���[�@dInEnd�F�}�������񃊃A�����[
						}
						else
						{
							//dEndot  = dInEnd;
							//iEdMozi = DocLetterPosGetAdjust( &dEndot , dWkLine, 1 );	//	�㏑�������̈�
							dGap    = dEndot - dInEnd;	//	���̕����̈ێ��̂��߂̃M���b�v��
							//	dEndot�F�}��������L�����b�g���[�@dInEnd�F�}�������񃊃A�����[
						}

						//	�Y���������폜
						DocRangeDeleteByMozi( xDot, dWkLine, iMozi, iEdMozi, &bFirst );

						if( 0 < dGap )
						{
							dInBgn  = xDot;
							//	���C�����I�u�W�F�N�g���[���猳�G�̊J�n�ʒu�܂ł𖄂߂鐔�h�b�g�̋�
							ptBuffer = DocPaddingSpaceWithPeriod( dGap, NULL, NULL, NULL, TRUE );
							if( ptBuffer )
							{
								DocInsertString( &dInBgn, &dWkLine, NULL, ptBuffer, dStyle, bFirst );	bFirst = FALSE;
								FREE(ptBuffer);
							}
						}
					}

#ifdef EDGE_BLANK_STYLE
					if( bEdgeBlank )
					{
						//	�I�t�Z�b�g�ʒu�m�F
						xDotEx  = (xTgDot + iSpDot);

						if( 1 == bEdgeBlank ){		xDotEx -= EDGE_BLANK_NARROW;	}
						else if( 2 == bEdgeBlank ){	xDotEx -= EDGE_BLANK_WIDE;	}
						else{	;	}

						if( 0 > xDotEx ){	xDotEx =  0;	}

						iMoziEx = DocLetterPosGetAdjust( &xDotEx, dWkLine, -1 );	//	���̕����ʒu���m�F
					//	iMoziEx�F�}���ʒu������			xDotEx�F�}���ʒu�I�t�Z�b�g

						//	�����̕������󔒂��A�󔒂Ȃ�ǂ��܂ő����Ă邩�m�F
						DocLineStateCheckWithDot( xDotEx, dWkLine, &dLeft, &dRight, &iStMozi, NULL, &bSpace );

						dGap = (xTgDot + iSpDot) - xDotEx;	//	�O���̖��ߏ���
						xDot = xDotEx;
					}
					else
					{
						//	�����̕������󔒂��A�󔒂Ȃ�ǂ��܂ő����Ă邩�m�F
						DocLineStateCheckWithDot( xDot, dWkLine, &dLeft, &dRight, &iStMozi, NULL, &bSpace );
						iMoziEx = iStMozi;	//	���ɈӖ��͂Ȃ�
#endif
						dGap = (xTgDot + iSpDot) - xDot;	//	�O���̖��ߏ���
#ifdef EDGE_BLANK_STYLE
					}
#endif
					if( bSpace )	//	�󔒂Ȃ�A�M���b�v���ƍ��킹�ē��꒼��
					{
						dGap  += (xDot - dLeft);	//	�p�f�B���O�h�b�g��
						//	�����̋󔒂���U�폜����
						DocRangeDeleteByMozi( dLeft, dWkLine, iStMozi, iMozi, &bFirst );
					}
					else	//	�����ł���Ȃ�A�Ȃɂ����Ȃ�
					{
#ifdef EDGE_BLANK_STYLE
						//	�K�v�Ȃ�A�����̕��������U�폜����
						if( bEdgeBlank )	DocRangeDeleteByMozi( xDot, dWkLine, iMoziEx, iMozi, &bFirst );
#endif
						dLeft = xDot;	//	�M���b�v�J�n�ʒu
					}

					ptBuffer = DocPaddingSpaceWithPeriod( dGap, NULL, NULL, NULL, TRUE );
					if( ptBuffer )
					{
						DocInsertString( &dLeft, &dWkLine, NULL, ptBuffer, dStyle, bFirst );	bFirst = FALSE;
						FREE(ptBuffer);
					}

					xDot = dLeft;	//	�}���ʒu�ł���悤��
				}
				else	//	���[�ʒu�Ƀs�^���ł���
				{
					//	���ɂ��邱�ƂȂ��H
				}


				//	�Y���̏ꏊ�֕�����}��
				DocInsertString( &xDot, &dWkLine, NULL, ptStr, dStyle, bFirst );	bFirst = FALSE;
				//	xDot�ɂ͑}���I�[�h�b�g������

				FREE(ptStr);
			}

			//	���̎��_�ŁAitLtr��end�����ߊJ�n�ʒu�ł���ExDot�͓��ߊJ�n�h�b�g�ł���
			if( itLtr == itLyr->vcLyrImg.at( dLyLine ).vcLine.end( ) )	break;
			//	�I����Ă��炱�̍s�̏����I���

			//	���߂��Ȃ��Ƃ���܂Ői�߂�
			for(  ; itLtr != itLyr->vcLyrImg.at( dLyLine ).vcLine.end( ); itLtr++ )
			{
				//���߃t���O�������Ȃ�ΏI��
				if( !(itLtr->mzStyle & CT_LYR_TRNC) )	break;
				xDot += itLtr->rdWidth;
			}	//	��������I���܂�

			iSpDot = xDot;	//	�}���J�n�ʒu�𒲐�
			iSpDot -= xTgDot;
		}

		//DocBadSpaceCheck( dWkLine );	//	�o�b�h�󔒃`�F�L
		//	DocInsertString�̒��̕��ł���Ă�̂ŁA�����ł͕s�v���Ǝv���
	}

	TRACE( TEXT("Layer Insert OK�I") );

	//	�ŏI�I�ȃL�����b�g�̈ʒu�����Z�b�g
	//	ViewPosResetCaret( xDot , dWkLine-1 );
	//	�����ł͏������Ȃ�
#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (HRESULT)ETC_MSG( err.what(), E_UNEXPECTED );	}
	catch( ... ){	return (HRESULT)ETC_MSG( ("etc error") , E_UNEXPECTED );	}
#endif

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C���{�b�N�X�̓��e���N���b�v����
	@param[in]	hWnd	�{�b�N�X�̃E�C���h�E�n���h��
	@param[in]	bStyle	���j�R�[�h���V�t�gJIS��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT LayerForClipboard( HWND hWnd, UINT bStyle )
{
	INT_PTR	iLines, iL, cchSize, cbSize;
	LETR_ITR	itMozi;

	LAYER_ITR	itLyr;

	string	srString;
	wstring	wsString;

	//	�Y���̃��C���[�{�b�N�X���m�F
	for( itLyr = gltLayer.begin(); itLyr != gltLayer.end(); itLyr++ )
	{
		if( itLyr->hBoxWnd == hWnd ){	break;	}
	}
	if( itLyr == gltLayer.end( ) )	return E_OUTOFMEMORY;


	iLines = itLyr->vcLyrImg.size( );	//	�s���m�F

	srString.clear( );
	wsString.clear( );

	for( iL = 0; iLines > iL; iL++ )
	{
		//	�������C�e���[�^�Ŋm��
		for( itMozi = itLyr->vcLyrImg.at( iL ).vcLine.begin( ); itMozi != itLyr->vcLyrImg.at( iL ).vcLine.end( ); itMozi++ )
		{
			srString += string( itMozi->acSjis );
			wsString += itMozi->cchMozi;
		}

		srString +=  string( CH_CRLFA );
		wsString += wstring( CH_CRLFW );
	}

	if( bStyle & D_UNI )	//	���j�R�[�h�ł���
	{
		cchSize = wsString.size( ) + 1;
		DocClipboardDataSet( (LPTSTR)(wsString.c_str()), cchSize * sizeof(TCHAR), bStyle );
	}
	else
	{
		cbSize = srString.size( ) + 1;
		DocClipboardDataSet( (LPSTR)(srString.c_str()), cbSize, bStyle );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C���{�b�N�X�̓��e���폜����
	@param[in]	hWnd	�{�b�N�X�̃E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT LayerOnDelete( HWND hWnd )
{
	ONELINE		stLine;

	LAYER_ITR	itLyr;

#ifdef DO_TRY_CATCH
	try{
#endif

	//	�Y���̃��C���[�{�b�N�X���m�F
	for( itLyr = gltLayer.begin(); itLyr != gltLayer.end(); itLyr++ )
	{
		if( itLyr->hBoxWnd == hWnd ){	break;	}
	}
	if( itLyr == gltLayer.end( ) )	return E_OUTOFMEMORY;

	LayerStringObliterate( itLyr );	//	���g�j��
	ZeroONELINE( &stLine );
	itLyr->vcLyrImg.push_back( stLine );	//	��f�[�^���쐬���Ă���

	InvalidateRect( hWnd, NULL, TRUE );

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (HRESULT)ETC_MSG( err.what(), E_UNEXPECTED );	}
	catch( ... ){	return (HRESULT)ETC_MSG( ("etc error") , E_UNEXPECTED );	}
#endif

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

#ifdef EDGE_BLANK_STYLE

/*!
	�������w�肵���Ƃ��ɁA���̋������ߗ̈���L�����Z������
	@param[in]	hWnd	�{�b�N�X�̃E�C���h�E�n���h��
	@param[in]	iCanWid	�L�����Z������ő啝
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT LayerEdgeBlankSizeCheck( HWND hWnd, INT iCanWid )
{
	INT_PTR	iLines;
	INT		iWidth;

	LAYER_ITR	itLyr;

	LYLINE_ITR	itLine;
	LETR_ITR	itMozi, itMzx;

#ifdef DO_TRY_CATCH
	try{
#endif

	//	�Y���̃��C���[�{�b�N�X���m�F
	for( itLyr = gltLayer.begin(); itLyr != gltLayer.end(); itLyr++ )
	{
		if( itLyr->hBoxWnd == hWnd ){	break;	}
	}
	if( itLyr == gltLayer.end( ) )	return E_OUTOFMEMORY;

	iLines = itLyr->vcLyrImg.size( );	//	�s���m�F


	//	��s�����Ă���
	for( itLine = itLyr->vcLyrImg.begin( ); itLine != itLyr->vcLyrImg.end( ); itLine++ )
	{
		//	�������C�e���[�^�Ŋm��
		for( itMozi = itLine->vcLine.begin( ); itMozi != itLine->vcLine.end( ); itMozi++ )
		{
			if(  itMozi->mzStyle & CT_LYR_TRNC )	//	���ߗ̈�Ƀq�b�g������
			{
				//	���̗̈�̕����Q�b�c����
				iWidth = 0;
				for( itMzx = itMozi; itMzx != itLine->vcLine.end( ); itMzx++ )
				{
					if( !(itMzx->mzStyle & CT_LYR_TRNC) )	break;	//	�O�ꂽ��I���
					iWidth += itMzx->rdWidth;
				}

				if( iCanWid >=  iWidth )	//	�����������Ⴂ�Ȃ�
				{
					for( ; itMzx != itMozi; itMozi++ )
					{
						itMozi->mzStyle &= ~CT_LYR_TRNC;
					}
				}
				else
				{
					itMozi = itMzx;
				}
				itMozi--;	//	���[�v�擪�ŃC���N�����邽�߁A��U�߂�
			}
		}
	}

	InvalidateRect( hWnd , NULL, TRUE );	//	�ĕ`��

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (HRESULT)ETC_MSG( err.what(), E_UNEXPECTED );	}
	catch( ... ){	return (HRESULT)ETC_MSG( ("etc error") , E_UNEXPECTED );	}
#endif
	return S_OK;
}
//-------------------------------------------------------------------------------------------------

#endif
