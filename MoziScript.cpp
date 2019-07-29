/*! @file
	@brief ����AA�X�N���v�g�̖ʓ|�݂܂�
	���̃t�@�C���� MoziScript.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/10/26
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

���䑋�͍őO�ʌŒ�
�A�C�R���̐���������₷��
�E�N���ɂ����
�A�C�R���f�U�C��������H


�`�r�s�A�`�r�c�ɑΉ��o����悤��
���炩���߁A�����ɑΉ�����AA��ǂݍ���ł����E���O�Ƀt�@�C���𕡐��o�^�o����
���e��SQL�I���������ŁE�Ή������ƁA�`�`�C���[�W��ێ����Ă���
�t�@�C���g�ݍ��킹�ς��čč\���ł���Ƃ�
�����Z�b�g�ێ��ł���悤�ɁH
�t�@�C���͂h�m�h�ɂ܂Ƃ߂�B
[Mozi]
count=2
file0=....
use0=1
file1=....
use1=0


���䑋����
���͂��ꂽ�����́A�땶�����Ɋo���Ă����B����ʒu���e�������ɕێ�
���͓��e���m�肵����A���C���{�b�N�X�̏������[�`���ŏ�������
�\���g�E�{�[�_�[�n�m�n�e�e

���Ԓ����ł���悤�ɁB�A�����s�͈�s�ÂJ����

�ʏ탂�[�h�E���͂d�c�h�s�A�\���X�V�Ɗm��o�@�����E���͂��čX�V�A�ʒu���߂Ċm��
�\���g�́A�ҏW�G���A�Ƀt���[�e�B���O�X�^�e�B�b�N������ňꖇ��
�X�^�e�B�b�N�̓T�u�N���X�Ń}�E�X�_�E���Ƃ��R���e�L�X�g�Ԃ�ǂ�
�h���b�O�ňړ��E�R���e�L�X�g�ɂ͊m��E�X�V�ł�����
�����ƕ����̊Ԃ𓧉߂��邩���Ȃ����I��
���߂Ȃ�땶�����A���Ȃ��Ȃ�ꖇ�ɒ����ăJ�L�R����΂���


IDD_MOZI_SCRIPT_DLG

*/




#define MOZISCRIPT_CLASS	TEXT("MOZISCRIPT_CLASS")
#define MZ_WIDTH	320
#define MZ_HEIGHT	280

#define MZ_PARAMHEI	23

#define MOZIVIEW_CLASS	TEXT("MOZIVIEW_CLASS")
//-------------------------------------------------------------------------------------------------

#define TB_ITEMS	3	//	5
static  TBBUTTON	gstMztbInfo[] = {
	{  0,	IDM_MOZI_DECIDE,	TBSTATE_ENABLED,	TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  1,	IDM_MOZI_REFRESH,	TBSTATE_ENABLED,	TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  2,	IDM_MOZI_SETTING,	TBSTATE_ENABLED,	TBSTYLE_CHECK | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  } 	//	
//	{  0,	0,					TBSTATE_ENABLED,	TBSTYLE_SEP,						{0, 0}, 0, 0  },
//	{  3,	IDM_MOZI_ADVANCE,	0,					TBSTYLE_CHECK | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  } 	//	
};	//	
//-------------------------------------------------------------------------------------------------

typedef struct tagMOZIITEM
{
	TCHAR	cch;	//!<	����
	LPTSTR	ptAA;	//!<	�����`�`

	INT		iLeft;	//!<	���h�b�g
	INT		iTop;	//!<	��h�b�g
	INT		iWidth;	//!<	�ő�h�b�g��
	INT		iLine;	//!<	�g�p�s��

} MOZIITEM, *LPMOZIITEM;
//-------------------------------------------------------------------------------------------------


extern FILES_ITR	gitFileIt;		//!<	�����Ă�t�@�C���̖{��
//#define gstFile	(*gitFileIt)		//!<	�C�e���[�^���\���̂ƌ��Ȃ�

extern INT			gixFocusPage;	//	���ڒ��̃y�[�W�E�O�C���f�b�N�X

extern HFONT		ghAaFont;		//	AA�p�t�H���g

extern  BYTE		gbAlpha;		//	�����x

extern  HWND		ghViewWnd;		//	�ҏW�r���[�E�C���h�E�̃n���h��
extern INT			gdHideXdot;		//	���̉B�ꕔ��
extern INT			gdViewTopLine;	//	�\�����̍ŏ㕔�s�ԍ�



static TCHAR		gatMoziIni[MAX_PATH];	//!<	

static  ATOM		gMoziAtom;		//!<	
EXTERNED HWND		ghMoziWnd;		//!<	
static  HWND		ghMoziToolBar;	//!<	
static HIMAGELIST	ghMoziImgLst;	//!<	

static  ATOM		gMoziViewAtom;
static  HWND		ghMoziViewWnd;	//!<	�\���X�^�e�B�b�N

static  HWND		ghTextWnd;		//!<	��������͘g
static  HWND		ghIsolaLvWnd;	//!<	�A�h�o���Y�h�����I�����X�g�r���[
static  HWND		ghSettiLvWnd;	//!<	�ݒ胊�X�g�r���[

static POINT		gstViewOrigin;	//!<	�r���[�̍���E�C���h�E�ʒu�E
static POINT		gstOffset;		//!<	�r���[���ォ��́A�{�b�N�X�̑��Έʒu
static POINT		gstFrmSz;		//!<	�E�C���h�E�G�b�W����`��̈�܂ł̃I�t�Z�b�g

static INT			gdToolBarHei;	//!<	�c�[���o�[����

static INT			gdMoziInterval;	//!<	�����Ԋu�E���F�L����@���F�k�܂�

static INT			gdNowMode;		//!<	�O�ʏ�@�P�A�h�o���Y�h�@0x10�ݒ�

static INT			gdAvrWidth;		//!<	���ϐ�L��
static INT			gdMaxLine;		//!<	�ő��L�s

static LPTSTR		gptMzBuff;		//!<	�e�L�X�g�g���當���m�ۘg�E��
static DWORD		gcchMzBuf;		//!<	�m�ۘg�̕������E�o�C�g����Ȃ���

static BOOLEAN		gbQuickClose;	//!<	�\��t�����炷������

static WNDPROC		gpfOrigMoziEditProc;	//!<	

static sqlite3		*gpMoziTable;	//!<	�����ꗗ�̃I���������f�^�x

static vector<MOZIITEM>	gvcMoziItem;

typedef vector<MOZIITEM>::iterator	MZTM_ITR;
//-------------------------------------------------------------------------------------------------

static LRESULT	CALLBACK gpfMoziEditProc( HWND , UINT, WPARAM, LPARAM );	//!<	


LRESULT	CALLBACK MoziProc( HWND, UINT, WPARAM, LPARAM );	//!<	
VOID	Mzs_OnCommand( HWND , INT, HWND, UINT );			//!<	
VOID	Mzs_OnPaint( HWND );								//!<	
VOID	Mzs_OnDestroy( HWND );								//!<	
VOID	Mzs_OnContextMenu( HWND, HWND, UINT, UINT );		//!<	
VOID	Mzs_OnDropFiles( HWND , HDROP );					//!<	
LRESULT	Mzs_OnNotify( HWND , INT, LPNMHDR );				//!<	

HRESULT	MoziFileRefresh( HWND );							//!<	
HRESULT	MoziFileRebuild( HWND, UINT );						//!<	

HRESULT	MoziFileListAdd( LPTSTR );							//!<	
HRESULT MoziFileListDelete( HWND  );						//!<	

HRESULT	MoziFileStore( LPTSTR );							//!<	
UINT	CALLBACK MoziItemTablise( LPTSTR, LPCTSTR, INT );	//!<	
VOID	MoziItemRemovePeriod( LPTSTR  );					//!<	

HRESULT	MoziSpaceCreate( VOID );							//!<	

HRESULT	MoziScriptInsert( HWND );							//!<	

LRESULT	CALLBACK MoziViewProc( HWND, UINT, WPARAM, LPARAM );//!<	
VOID	Mzv_OnKey( HWND, UINT, BOOL, INT, UINT );			//!<	
VOID	Mzv_OnPaint( HWND );								//!<	
VOID	Mzv_OnMoving( HWND, LPRECT );						//!<	
BOOL	Mzv_OnWindowPosChanging( HWND, LPWINDOWPOS );		//!<	
VOID	Mzv_OnWindowPosChanged( HWND, const LPWINDOWPOS );	//!<	
VOID	MoziViewDraw( HDC );								//!<	

HRESULT	MoziSqlTableOpenClose( UINT );						//!<	
HRESULT	MoziSqlTransOnOff( BYTE );							//!<	
UINT	MoziSqlItemInsert( LPTSTR, LPTSTR, INT, INT );		//!<	
LPTSTR	MoziSqlItemSelect( TCHAR, LPINT, LPINT );			//!<	
UINT	MoziSqlItemCount( LPINT, LPINT );					//!<	
HRESULT	MoziSqlItemDeleteAll( VOID );						//!<	

//-------------------------------------------------------------------------------------------------

/*!
	�����X�N���v�g��INI�t�@�C�����m�ہE�E�C���h�E�N���X�o�^�E�A�v���N���シ���Ă΂��
	@param[in]	ptCurrent	��f�B���N�g��
	@param[in]	hInstance	�C���X�^���X�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
INT MoziInitialise( LPTSTR ptCurrent, HINSTANCE hInstance )
{
	WNDCLASSEX	wcex;
	HBITMAP		hImg, hMsq;
	MZTM_ITR	itMzitm;

	if( !(ptCurrent) || !(hInstance) )
	{
		if( ghMoziWnd ){	DestroyWindow( ghMoziWnd  );	}

		if( ghMoziImgLst  ){	ImageList_Destroy( ghMoziImgLst );	}

		if( !( gvcMoziItem.empty( ) ) )
		{
			for( itMzitm = gvcMoziItem.begin( ); gvcMoziItem.end( ) != itMzitm; itMzitm++ )
			{
				FREE( itMzitm->ptAA );
			}
			gvcMoziItem.clear( );
		}

		MoziSqlTableOpenClose( M_DESTROY );

		FREE( gptMzBuff );

		return S_OK;
	}

	StringCchCopy( gatMoziIni, MAX_PATH, ptCurrent );
	PathAppend( gatMoziIni, MZCX_INI_FILE );

//�����X�N���v�g���䑋
	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= MoziProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= MOZISCRIPT_CLASS;
	wcex.hIconSm		= NULL;

	gMoziAtom = RegisterClassEx( &wcex );

	ghMoziWnd = NULL;

	ZeroMemory( &gstViewOrigin, sizeof(POINT) );
	gdToolBarHei = 0;

	//	�A�C�R���@�m��E�X�V�E�ݒ�E�A�h�o���Y�h
	ghMoziImgLst = ImageList_Create( 16, 16, ILC_COLOR24 | ILC_MASK, 4, 1 );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMP_MOZI_WRITE ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMQ_PAGENAMECHANGE ) );
	ImageList_Add( ghMoziImgLst, hImg, hMsq );	//	�m��
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMP_REFRESH ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMQ_REFRESH ) );
	ImageList_Add( ghMoziImgLst, hImg, hMsq );	//	�X�V
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMP_SETTING ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMQ_SETTING ) );
	ImageList_Add( ghMoziImgLst, hImg, hMsq );	//	�ݒ�
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	//hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMP_MOZI_ADVANCE ) );
	//hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMQ_MOZI_ADVANCE ) );
	//ImageList_Add( ghMoziImgLst, hImg, hMsq );	//	�A�h�o���Y�h
	//DeleteBitmap( hImg );	DeleteBitmap( hMsq );

//	�I��������SQL���A�K����������Ă���
	MoziSqlTableOpenClose( M_CREATE );


//�����\����
	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= MoziViewProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= MOZIVIEW_CLASS;
	wcex.hIconSm		= NULL;

	gMoziViewAtom = RegisterClassEx( &wcex );

	//	�K���ɍ���Ă���
	gptMzBuff = (LPTSTR)malloc( MAX_PATH * sizeof(TCHAR) );
	ZeroMemory( gptMzBuff, MAX_PATH * sizeof(TCHAR) );
	gcchMzBuf = MAX_PATH;


	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�����X�N���v�g�E�C���h�E���
	@param[in]	hInst	�C���X�^���X�n���h��
	@param[in]	hPrWnd	���C���̃E�C���h�E�n���h��
*/
HWND MoziScripterCreate( HINSTANCE hInst, HWND hPrWnd )
{
	LONG	x, y;
	HWND	hDktpWnd;
	UINT	dCount, height;
	TCHAR	atBuffer[MAX_STRING];
	RECT	rect, vwRect, dtRect;

	LVCOLUMN	stLvColm;

	hDktpWnd = GetDesktopWindow(  );
	GetWindowRect( hDktpWnd, &dtRect );

	GetWindowRect( hPrWnd, &rect );
	x = dtRect.right - rect.right;
	if( MZ_WIDTH >  x ){	rect.right = dtRect.right - MZ_WIDTH;	}

	if( ghMoziWnd )
	{
		SetForegroundWindow( ghMoziViewWnd );
		SetWindowPos( ghMoziWnd, HWND_TOP, rect.right, rect.top, 0, 0, SWP_NOSIZE );
		SetForegroundWindow( ghMoziWnd );

		return ghMoziWnd;
	}

	gdMoziInterval = 0;

	//	�{�̃E�C���h�E
	ghMoziWnd = CreateWindowEx( WS_EX_TOOLWINDOW | WS_EX_TOPMOST, MOZISCRIPT_CLASS,
		TEXT("����AA�ϊ�"), WS_POPUP | WS_CAPTION | WS_SYSMENU,
		rect.right, rect.top, MZ_WIDTH, MZ_HEIGHT, NULL, NULL, hInst, NULL );

//	DragAcceptFiles( ghMoziWnd, FALSE );

	gdNowMode = 0;

	gbQuickClose = TRUE;

	//	�c�[���o�[
	ghMoziToolBar = CreateWindowEx( WS_EX_CLIENTEDGE, TOOLBARCLASSNAME, TEXT("mozitoolbar"), WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TOOLTIPS, 0, 0, 0, 0, ghMoziWnd, (HMENU)IDTB_MZSCR_TOOLBAR, hInst, NULL );

	if( 0 == gdToolBarHei )	//	���l���擾�Ȃ�
	{
		GetWindowRect( ghMoziToolBar, &rect );
		gdToolBarHei = rect.bottom - rect.top;
	}

	//	�����c�[���`�b�v�X�^�C����ǉ�
	SendMessage( ghMoziToolBar, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS );

	SendMessage( ghMoziToolBar, TB_SETIMAGELIST, 0, (LPARAM)ghMoziImgLst );
	SendMessage( ghMoziToolBar, TB_SETBUTTONSIZE, 0, MAKELPARAM(16,16) );

	SendMessage( ghMoziToolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0 );

	//	�c�[���`�b�v�������ݒ�E�{�^���e�L�X�g���c�[���`�b�v�ɂȂ�
	StringCchCopy( atBuffer, MAX_STRING, TEXT("�����`�`�}��") );					gstMztbInfo[0].iString = SendMessage( ghMoziToolBar, TB_ADDSTRING, 0, (LPARAM)atBuffer );
	StringCchCopy( atBuffer, MAX_STRING, TEXT("������X�V / �g�p�t�@�C���X�V") );	gstMztbInfo[1].iString = SendMessage( ghMoziToolBar, TB_ADDSTRING, 0, (LPARAM)atBuffer );
	StringCchCopy( atBuffer, MAX_STRING, TEXT("�g�p�t�@�C���ݒ�") );				gstMztbInfo[2].iString = SendMessage( ghMoziToolBar, TB_ADDSTRING, 0, (LPARAM)atBuffer );
//	StringCchCopy( atBuffer, MAX_STRING, TEXT("�A�h�o���Y�h") );					gstMztbInfo[4].iString = SendMessage( ghMoziToolBar, TB_ADDSTRING, 0, (LPARAM)atBuffer );

	SendMessage( ghMoziToolBar , TB_ADDBUTTONS, (WPARAM)TB_ITEMS, (LPARAM)&gstMztbInfo );	//	�c�[���o�[�Ƀ{�^����}��

	SendMessage( ghMoziToolBar , TB_AUTOSIZE, 0, 0 );	//	�{�^���̃T�C�Y�ɍ��킹�ăc�[���o�[�����T�C�Y

	//	�\��t���������`�F�b�N�{�b�N�X��t����
	CreateWindowEx( 0, WC_BUTTON, TEXT("�}�����������"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 128, 2, 170, 23, ghMoziToolBar, (HMENU)IDCB_MZSCR_QUICKCLOSE, hInst, NULL );
	CheckDlgButton( ghMoziToolBar, IDCB_MZSCR_QUICKCLOSE, gbQuickClose ? BST_CHECKED : BST_UNCHECKED );

	InvalidateRect( ghMoziToolBar , NULL, TRUE );		//	�N���C�A���g�S�̂��ĕ`�悷�閽��

	GetClientRect( ghMoziWnd, &rect );


	//	������STATIC
	CreateWindowEx( 0, WC_STATIC, TEXT("������"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, 0, gdToolBarHei, 52, MZ_PARAMHEI, ghMoziWnd, (HMENU)IDS_MZSCR_INTERVAL, hInst, NULL );

	//	������EDIT
	CreateWindowEx( 0, WC_EDIT, TEXT("0"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 54, gdToolBarHei, 50, MZ_PARAMHEI, ghMoziWnd, (HMENU)IDE_MZSCR_INTERVAL, hInst, NULL );

	//	������SPIN
	CreateWindowEx( 0, UPDOWN_CLASS, TEXT("intervalspin"), WS_CHILD | WS_VISIBLE | UDS_AUTOBUDDY, 104, gdToolBarHei, 10, MZ_PARAMHEI, ghMoziWnd, (HMENU)IDUD_MZSCR_INTERVAL, hInst, NULL );

	//	�����ԓ���CBX
	CreateWindowEx( 0, WC_BUTTON, TEXT("�����Ԃ͓���"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 130, gdToolBarHei, 120, MZ_PARAMHEI, ghMoziWnd, (HMENU)IDCB_MZSCR_TRANSPARENT, hInst, NULL );


	height = gdToolBarHei + MZ_PARAMHEI;

	//��������͘g
	ghTextWnd = CreateWindowEx( 0, WC_EDIT, TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE,
		0, height, rect.right, rect.bottom - height, ghMoziWnd, (HMENU)IDE_MZSCR_TEXT, hInst, NULL );
	SetWindowFont( ghTextWnd, ghAaFont, TRUE );

	//	�T�u�N���X
	gpfOrigMoziEditProc = SubclassWindow( ghTextWnd, gpfMoziEditProc );

	//�ݒ�g
	ghSettiLvWnd = CreateWindowEx( 0, WC_LISTVIEW, TEXT("mozisetting"),
		WS_CHILD | WS_BORDER | WS_VSCROLL | LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER,
		0, gdToolBarHei, rect.right, rect.bottom - gdToolBarHei, ghMoziWnd, (HMENU)IDLV_MZSCR_SETTING, hInst, NULL );
	ListView_SetExtendedListViewStyle( ghSettiLvWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES );

	//	�T�u�N���X��	
//	gpfOrgSettiLvProc = SubclassWindow( ghSettiLvWnd, gpfSettiLvProc );

	ZeroMemory( &stLvColm, sizeof(LVCOLUMN) );
	stLvColm.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	stLvColm.fmt = LVCFMT_LEFT;
	stLvColm.pszText = TEXT("�t�@�C����");	stLvColm.cx = 200;	stLvColm.iSubItem = 0x00;	ListView_InsertColumn( ghSettiLvWnd, 0, &stLvColm );
	stLvColm.pszText = TEXT("�t���p�X");	stLvColm.cx = 500;	stLvColm.iSubItem = 0x01;	ListView_InsertColumn( ghSettiLvWnd, 1, &stLvColm );


	//	�I��������SQL�A���g�������Ȃ�f�[�^�ǂݍ���ō\��
	dCount = MoziSqlItemCount( NULL, NULL );
	MoziFileRebuild( ghMoziWnd, dCount ? FALSE : TRUE );

	ShowWindow( ghMoziWnd, SW_SHOW );
	UpdateWindow( ghMoziWnd );


//�\���E�ʒu���ߔ������t���[�e�B���O�E�C���h�[
	ghMoziViewWnd = CreateWindowEx( WS_EX_TOOLWINDOW | WS_EX_LAYERED, MOZIVIEW_CLASS,
		TEXT("�z�u"), WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_VISIBLE, 0, 0, 160, 120, NULL, NULL, hInst, NULL);
	SetLayeredWindowAttributes( ghMoziViewWnd, 0, gbAlpha, LWA_ALPHA );
	//	�����x�̓��C���{�b�N�X�̐ݒ���g��

	ZeroMemory( &gstFrmSz, sizeof(POINT) );
	ClientToScreen( ghMoziViewWnd, &gstFrmSz );

	//	�E�C���h�E�ʒu���m�肳����
	GetWindowRect( ghViewWnd, &vwRect );
	gstViewOrigin.x = vwRect.left;	//	�r���[�E�C���h�E�̈ʒu�L�^
	gstViewOrigin.y = vwRect.top;
	x = (vwRect.left + LINENUM_WID) - gstFrmSz.x;
	y = (vwRect.top  + RULER_AREA)  - gstFrmSz.y;
	TRACE( TEXT("MOZI %d x %d"), x, y );
	//	���̎��_��0dot,0line�̈ʒu�ɃN���C�����g���オ�A�b�[�I

#ifdef _DEBUG
	SetWindowPos( ghMoziViewWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW );
#else
	SetWindowPos( ghMoziViewWnd, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW );
#endif
	gstOffset.x = x - vwRect.left;
	gstOffset.y = y - vwRect.top;

	return ghMoziWnd;
}
//-------------------------------------------------------------------------------------------------

/*!
	�e�L�X�g�{�b�N�X�̓��e���X�N���v�g����
*/
HRESULT MoziEditAssemble( HWND hWnd )
{
	UINT_PTR	cchSz;
	INT			ileng, i, iNdot, iNlnDot, iMxLine, wid;
	INT			iMaxDot, iLastLine, iViewXdot, iYline, iViewYdot, cx, cy;
	HWND		hWorkWnd;
	LPTSTR		ptScript;
	RECT		rect;
	MOZIITEM	stMzitm;

	MZTM_ITR	itMzitm;

	hWorkWnd = GetDlgItem( hWnd, IDE_MZSCR_TEXT );

	//	�������m�F���ăo�b�t�@�쐬
	ileng = Edit_GetTextLength( hWorkWnd );
	cchSz = ileng + 2;

	if( gcchMzBuf <  cchSz )	//	�e�ʑ���Ȃ��Ȃ�g������
	{
		ptScript = (LPTSTR)realloc( gptMzBuff, cchSz * sizeof(TCHAR) );
		gptMzBuff = ptScript;
		gcchMzBuf = cchSz;
	}

	ZeroMemory( gptMzBuff, gcchMzBuf * sizeof(TCHAR) );
	Edit_GetText( hWorkWnd, gptMzBuff, cchSz );


	//	��̃f�[�^�j��
	if( !( gvcMoziItem.empty( ) ) )
	{
		for( itMzitm = gvcMoziItem.begin( ); gvcMoziItem.end( ) != itMzitm; itMzitm++ ){	FREE( itMzitm->ptAA );	}
		gvcMoziItem.clear( );
	}

	iNdot   = 0;
	iNlnDot = 0;
	iMxLine = 1;
	iMaxDot   = 0;
	iLastLine = 0;
	for( i = 0; ileng > i; i++ )	//	�땶�����o�����Ĉʒu�m�F
	{
		ZeroMemory( &stMzitm, sizeof(MOZIITEM) );

		if( TEXT('\r') ==  gptMzBuff[i] )	//	���s
		{
			iLastLine += iMxLine;

			iNdot = 0;
			iNlnDot += (iMxLine * LINE_HEIGHT);
			iMxLine = 1;
			i++;
		}
		else
		{
			stMzitm.ptAA  = MoziSqlItemSelect( gptMzBuff[i], &(stMzitm.iLine), &(stMzitm.iWidth) );
			if( !(stMzitm.ptAA) )	//	�����Ή��������Ȃ�������A���ϕ��Ńp�f�B���O
			{
				wid = ViewLetterWidthGet( gptMzBuff[i] );
				//	�������m�F���āA�K���ɑS�p���p�ƌ��Ȃ��B
				if( 10 <= wid ){	stMzitm.iWidth = gdAvrWidth;	}
				else{	stMzitm.iWidth = gdAvrWidth / 2;	}
				stMzitm.iLine = gdMaxLine;
			}
			stMzitm.cch   = gptMzBuff[i];
			stMzitm.iLeft = iNdot;
			stMzitm.iTop  = iNlnDot;
			if( iMxLine < stMzitm.iLine ){	iMxLine = stMzitm.iLine;	}

			gvcMoziItem.push_back( stMzitm );
			iNdot += stMzitm.iWidth;
		}

		if( iMaxDot < iNdot )	iMaxDot = iNdot;
	}
	iLastLine += iMxLine;	//	�ŏI�s
	iLastLine++;	//	�]�T�t��

	//gdNowMode

	//	���̉�ʂ̍s���ƃh�b�g���m�F
	iYline = ViewAreaSizeGet( &iViewXdot );
	iViewYdot = iYline * LINE_HEIGHT;

	GetWindowRect( ghMoziViewWnd, &rect );
	cx = rect.right  - rect.left;
	cy = rect.bottom - rect.top;
	GetClientRect( ghMoziViewWnd, &rect );
	cx -= rect.right;
	cy -= rect.bottom;

	//	�����E�C���h�E�T�C�Y�ɂȂ�͂�
	cx += iMaxDot;
	cy += (iLastLine * LINE_HEIGHT);

	//	�₽��f�J���Ȃ玩�d
	if( iViewXdot < cx ){	cx =  iViewXdot;	}
	if( iViewYdot < cy ){	cy =  iViewYdot;	}
	//	�������Ă����d�E�ŏ��T�C�Y�͓K��
	if( 66 > cx ){	cx = 66;	}
	if( 66 > cy ){	cy = 66;	}

#ifdef _DEBUG
	SetWindowPos( ghMoziViewWnd, HWND_TOP, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );
#else
	SetWindowPos( ghMoziViewWnd, HWND_TOPMOST, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );
#endif

	InvalidateRect( ghMoziViewWnd, NULL, TRUE );

	return S_OK;
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
LRESULT CALLBACK gpfMoziEditProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
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
			TRACE( TEXT("[%X]MoziEdit COMMAND %d"), hWnd, id );
			
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

	return CallWindowProc( gpfOrigMoziEditProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------


/*!
	�X�N���v�g�r���[�̃E�C���h�E�v���V�[�W��
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�����ς�
	@retval no0	�����ł͏����������ɉ�
*/
LRESULT CALLBACK MoziViewProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		HANDLE_MSG( hWnd, WM_KEYDOWN,			Mzv_OnKey );
		HANDLE_MSG( hWnd, WM_PAINT,				Mzv_OnPaint );		//	��ʂ̍X�V�Ƃ�
		HANDLE_MSG( hWnd, WM_WINDOWPOSCHANGING,	Mzv_OnWindowPosChanging );
		HANDLE_MSG( hWnd, WM_WINDOWPOSCHANGED,	Mzv_OnWindowPosChanged );

		case  WM_MOVING:	Mzv_OnMoving( hWnd, (LPRECT)lParam );	return 0;

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
VOID Mzv_OnMoving( HWND hWnd, LPRECT pstPos )
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
	xSb += gdHideXdot;
	dLine += gdViewTopLine;

	StringCchPrintf( atBuffer, SUB_STRING, TEXT("MOZI %d[dot] %d[line]"), xSb, dLine );
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
BOOL Mzv_OnWindowPosChanging( HWND hWnd, LPWINDOWPOS pstWpos )
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
VOID Mzv_OnWindowPosChanged( HWND hWnd, const LPWINDOWPOS pstWpos )
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
VOID Mzv_OnKey( HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags )
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
	Mzv_OnMoving( hWnd, &rect );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	PAINT�B�����̈悪�o�����Ƃ��ɔ����B�w�i�̈����ɒ��ӁB�w�i��h��Ԃ��Ă���A�I�u�W�F�N�g��`��
	@param[in]	hWnd	�E�C���h�E�n���h��
*/
VOID Mzv_OnPaint( HWND hWnd )
{
	PAINTSTRUCT	ps;
	HDC			hdc;

	hdc = BeginPaint( hWnd, &ps );

	MoziViewDraw( hdc );

	//���A���^�C���X�V����ƃt�H�[�J�X�����������Ȃ�H

	EndPaint( hWnd, &ps );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�\���g�̕`�揈��
	@param[in]	hDC	�`�悷��f�o�C�X�R���e�L�X�g
*/
VOID MoziViewDraw( HDC hDC )
{
	UINT_PTR	cchSize, dLeng, dPos;
	INT_PTR	iItems, iLn;
	INT		x, y, cmr;
	LPTSTR	ptHead;
	HFONT	hFtOld;

	MZTM_ITR	itMzitm;

	hFtOld = SelectFont( hDC, ghAaFont );	//	�t�H���g����������
	SetBkMode( hDC, TRANSPARENT );

	iItems = gvcMoziItem.size( );

	cmr = 0;
	//	�e��������
	for( itMzitm = gvcMoziItem.begin( ); gvcMoziItem.end( ) != itMzitm; itMzitm++ )
	{
		if( !(itMzitm->ptAA) )	continue;	//	�f�[�^�������Ȃ��΂��΂����͂�

		StringCchLength( itMzitm->ptAA, STRSAFE_MAX_CCH, &cchSize );	//	�S�̒����m�F����

		x      = itMzitm->iLeft;

		if( 0 >= x ){	cmr = 0;	}
		x += (gdMoziInterval * cmr);
		cmr++;

		y      = itMzitm->iTop;
		ptHead = itMzitm->ptAA;
		dLeng  = 0;
		dPos   = 0;
		for( iLn = 0; itMzitm->iLine >  iLn; iLn++ )	//	�����̍s����
		{
			while( cchSize >= dLeng )	//	���s�܂ŁE��������NULL�܂�
			{
				if( TEXT('\r') == itMzitm->ptAA[dLeng] )
				{
					ExtTextOut( hDC, x, y, 0, NULL, ptHead, dPos, NULL );

					y      += LINE_HEIGHT;	//	���̍s�ʒu��
					dLeng  += 2;	//	'\n'�����Ď��̍s�̐擪
					dPos    = 0;
					ptHead  = &(itMzitm->ptAA[dLeng]);
					break;
				}

				if( 0 == itMzitm->ptAA[dLeng] )
				{
					ExtTextOut( hDC, x, y, 0, NULL, ptHead, dPos, NULL );
					break;
				}

				dLeng++;	dPos++;
			}
		}
	}

	SelectFont( hDC, hFtOld );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�r���[���ړ�����
	@param[in]	hWnd	�{�̃E�C���h�E�n���h���E���܂�Ӗ��͂Ȃ�
	@param[in]	state	����ԁE�ŏ����Ȃ�Ⴄ�R�g����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MoziMoveFromView( HWND hWnd, UINT state )
{
	RECT	vwRect = {0,0,0,0};
	POINT	lyPoint;

	if( !(ghMoziViewWnd) )	return S_FALSE;

	//	�ŏ������͔�\���ɂ���Ƃ�	SIZE_MINIMIZED

	if( SIZE_MINIMIZED != state )
	{
		GetWindowRect( ghViewWnd, &vwRect );
		gstViewOrigin.x = vwRect.left;//�ʒu�L�^
		gstViewOrigin.y = vwRect.top;
	}

	if( SIZE_MINIMIZED == state )
	{
		ShowWindow( ghMoziViewWnd, SW_HIDE );
	}
	else
	{
		lyPoint.x = gstOffset.x + vwRect.left;
		lyPoint.y = gstOffset.y + vwRect.top;
#ifdef _DEBUG
		SetWindowPos( ghMoziViewWnd, HWND_TOP, lyPoint.x, lyPoint.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW );
#else
		SetWindowPos( ghMoziViewWnd, HWND_TOPMOST, lyPoint.x, lyPoint.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW );
#endif
	}

	return S_OK;
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
LRESULT CALLBACK MoziProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		HANDLE_MSG( hWnd, WM_PAINT,       Mzs_OnPaint );		//	��ʂ̍X�V�Ƃ�
		HANDLE_MSG( hWnd, WM_NOTIFY,      Mzs_OnNotify );		//	�R�����R���g���[���̌ʃC�x���g
		HANDLE_MSG( hWnd, WM_COMMAND,     Mzs_OnCommand );	
		HANDLE_MSG( hWnd, WM_DESTROY,     Mzs_OnDestroy );		//	�I�����̏���
		HANDLE_MSG( hWnd, WM_DROPFILES,   Mzs_OnDropFiles );	//	D&D
		HANDLE_MSG( hWnd, WM_CONTEXTMENU, Mzs_OnContextMenu );

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
VOID Mzs_OnCommand( HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify )
{
	LRESULT	lRslt;

	switch( id )
	{
		case IDM_MOZI_DECIDE:
			if( !(0x10 & gdNowMode) )
			{
				MoziScriptInsert( hWnd );
				if( gbQuickClose  ){	DestroyWindow( hWnd );	}	//	��������H
			}
			break;

		case IDM_MOZI_REFRESH:
			if( 0x10 & gdNowMode  ){	MoziFileRefresh( hWnd );	 return;	}	//	�ݒ�
			else{	MoziEditAssemble( hWnd );	}
			break;

		case IDE_MZSCR_TEXT:	//	�������͘g�E�������^�C���Ńr���[������
			if( EN_UPDATE == codeNotify ){	MoziEditAssemble( hWnd );	}
			break;

		case IDM_MOZI_SETTING:
			lRslt = SendMessage( ghMoziToolBar, TB_ISBUTTONCHECKED, IDM_MOZI_SETTING, 0 );
			if( lRslt  )	//	�ݒ胂�[�h
			{
				gdNowMode |=  0x10;
				DragAcceptFiles( ghMoziWnd, TRUE );

				SetWindowPos( ghSettiLvWnd, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );

				ShowWindow( ghTextWnd, SW_HIDE );
				ShowWindow( GetDlgItem(hWnd,IDS_MZSCR_INTERVAL),	SW_HIDE );
				ShowWindow( GetDlgItem(hWnd,IDE_MZSCR_INTERVAL),	SW_HIDE );
				ShowWindow( GetDlgItem(hWnd,IDUD_MZSCR_INTERVAL),	SW_HIDE );
				ShowWindow( GetDlgItem(hWnd,IDCB_MZSCR_TRANSPARENT),SW_HIDE );
			}
			else
			{
				gdNowMode &= ~0x10;
				DragAcceptFiles( ghMoziWnd, FALSE );

				SetWindowPos( ghTextWnd,    HWND_TOP,    0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
				ShowWindow( GetDlgItem(hWnd,IDS_MZSCR_INTERVAL),	SW_SHOW );
				ShowWindow( GetDlgItem(hWnd,IDE_MZSCR_INTERVAL),	SW_SHOW );
				ShowWindow( GetDlgItem(hWnd,IDUD_MZSCR_INTERVAL),	SW_SHOW );
				ShowWindow( GetDlgItem(hWnd,IDCB_MZSCR_TRANSPARENT),SW_SHOW );

				ShowWindow( ghSettiLvWnd, SW_HIDE );
			}
			break;

//		case IDM_MOZI_ADVANCE:	break;

		case IDCB_MZSCR_QUICKCLOSE:
			gbQuickClose = IsDlgButtonChecked( GetDlgItem(hWnd,IDTB_MZSCR_TOOLBAR), IDCB_MZSCR_QUICKCLOSE ) ? TRUE : FALSE;
			SetFocus( hWnd );
			break;

		case IDM_MOZI_LISTDEL:	MoziFileListDelete( hWnd  );	break;

		case IDM_PASTE:	TRACE( TEXT("MZ PASTE") );	SendMessage( ghTextWnd, WM_PASTE, 0, 0 );	return;
		case IDM_COPY:	SendMessage( ghTextWnd, WM_COPY,  0, 0 );	return;
		case IDM_CUT:	SendMessage( ghTextWnd, WM_CUT,   0, 0 );	return;

		default:	return;
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	PAINT�B�����̈悪�o�����Ƃ��ɔ����B�w�i�̈����ɒ��ӁB�w�i��h��Ԃ��Ă���A�I�u�W�F�N�g��`��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
*/
VOID Mzs_OnPaint( HWND hWnd )
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
VOID Mzs_OnDestroy( HWND hWnd )
{
	MZTM_ITR	itMzitm;

	MainStatusBarSetText( SB_LAYER, TEXT("") );

	if( ghMoziViewWnd ){	DestroyWindow( ghMoziViewWnd  );	}

	//	��̃f�[�^�j��
	if( !( gvcMoziItem.empty( ) ) )
	{
		for( itMzitm = gvcMoziItem.begin( ); gvcMoziItem.end( ) != itMzitm; itMzitm++ ){	FREE( itMzitm->ptAA );	}
		gvcMoziItem.clear( );
	}

	ghMoziWnd = NULL;

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
VOID Mzs_OnContextMenu( HWND hWnd, HWND hWndContext, UINT xPos, UINT yPos )
{
	HMENU	hMenu;
	POINT	stPoint;


	stPoint.x = (SHORT)xPos;	//	��ʍ��W�̓}�C�i�X�����肤��
	stPoint.y = (SHORT)yPos;

	if( ghSettiLvWnd == hWndContext )
	{
		hMenu = CreatePopupMenu(  );

		AppendMenu( hMenu, MF_STRING, IDM_MOZI_LISTDEL, TEXT("���X�g����폜") );

		TrackPopupMenu( hMenu, 0, stPoint.x, stPoint.y, 0, hWnd, NULL );
		DestroyMenu( hMenu );

		return;
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
LRESULT Mzs_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	LPNMUPDOWN	pstNmUpDown;

	if( IDUD_MZSCR_INTERVAL == idFrom )
	{
		pstNmUpDown = (LPNMUPDOWN)pstNmhdr;

		if( UDN_DELTAPOS == pstNmUpDown->hdr.code )
		{
			//	iDelta�@��Ł|�P�A���łP
			TRACE( TEXT("UPDOWN %d"), pstNmUpDown->iDelta );
			gdMoziInterval += (pstNmUpDown->iDelta);
			SetDlgItemInt( hWnd, IDE_MZSCR_INTERVAL, gdMoziInterval, TRUE );

			InvalidateRect( ghMoziViewWnd, NULL, TRUE );	//	�������^�C���H
		}
	}

	return 0;	//	�����Ȃ��Ȃ�O��߂�
}
//-------------------------------------------------------------------------------------------------

/*!
	�h���b�O���h���b�v�̎󂯓���
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	hDrop	�h���b�s���I�u�W�F�N�c�n���h�D
*/
VOID Mzs_OnDropFiles( HWND hWnd, HDROP hDrop )
{
	TCHAR	atFileName[MAX_PATH];//, atExBuf[10];
//	LPTSTR	ptExten;

	ZeroMemory( atFileName, sizeof(atFileName) );

	DragQueryFile( hDrop, 0, atFileName, MAX_PATH );
	DragFinish( hDrop );

	TRACE( TEXT("MOZI DROP[%s]"), atFileName );

	//	�g���q���m�F
	if( FileExtensionCheck( atFileName, TEXT(".ast") ) )
	{
		MoziFileListAdd( atFileName );	//	�o�^�����ɐi��
	}
	//ptExten = PathFindExtension( atFileName );	//	�g���q�������Ȃ�NULL�A�Ƃ��������[�ɂȂ�
	//if( 0 == *ptExten ){	 return;	}
	////	�g���q�w�肪�Ȃ��Ȃ�Ȃɂ��ł��Ȃ�
	//StringCchCopy( atExBuf, 10, ptExten );
	//CharLower( atExBuf );	//	��r�̂��߂ɏ������ɂ����Ⴄ
	//if( StrCmp( atExBuf, TEXT(".ast") ) ){	 return;	}
	////	AST�łȂ��Ȃ�i�j�����Ȃ�


	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ݒ胊�X�g�r���[�Ƀt�@�C�����𑝂₷
*/
HRESULT MoziFileListAdd( LPTSTR ptFilePath )
{
	TCHAR	atFileName[MAX_PATH];

	UINT	iItem;
	LVITEM	stLvi;

	StringCchCopy( atFileName, MAX_PATH, ptFilePath );
	PathStripPath( atFileName );	//	�t�@�C�����̂Ƃ���؂�o��


	iItem = ListView_GetItemCount( ghSettiLvWnd );

	ZeroMemory( &stLvi, sizeof(stLvi) );
	stLvi.mask  = LVIF_TEXT;
	stLvi.iItem = iItem;

	stLvi.pszText  = atFileName;
	stLvi.iSubItem = 0;
	ListView_InsertItem( ghSettiLvWnd, &stLvi );

	stLvi.pszText  = ptFilePath;
	stLvi.iSubItem = 1;
	ListView_SetItem( ghSettiLvWnd, &stLvi );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!

*/
HRESULT MoziFileListDelete( HWND hWnd )
{
	INT	iItem;

	iItem = ListView_GetNextItem( ghSettiLvWnd, -1, LVNI_ALL | LVNI_SELECTED );

	//	�I������Ă郂�m���Ȃ��Ɩ��Ӗ�
	if( 0 >  iItem ){	return  E_ABORT;	}

	ListView_DeleteItem( ghSettiLvWnd, iItem );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���X�g�r���[�̓��e��ۑ����Ďg�p�������č\������
	@param[in]	hWnd	�E�C���h�E�n���h��
*/
HRESULT MoziFileRefresh( HWND hWnd )
{
	TCHAR	atKeyName[MIN_STRING], atBuff[MIN_STRING];
	TCHAR	atFilePath[MAX_PATH];
	INT		iItem, i;
	UINT	bCheck;
	LVITEM	stLvi;

	iItem = ListView_GetItemCount( ghSettiLvWnd );

	//	��U�Z�N�V��������ɂ���
	ZeroMemory( atBuff, sizeof(atBuff) );
	WritePrivateProfileSection( TEXT("MoziScript"), atBuff, gatMoziIni );

	StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), iItem );
	WritePrivateProfileString( TEXT("MoziScript"), TEXT("Count"), atBuff, gatMoziIni );

	MoziSqlItemDeleteAll(  );	//	�����ŃI��������SQL����ɂ���

	ZeroMemory( &stLvi, sizeof(stLvi) );
	stLvi.mask       = LVIF_TEXT;
	stLvi.pszText    = atFilePath;
	stLvi.iSubItem   = 1;
	stLvi.cchTextMax = MAX_PATH;
	for( i = 0; iItem > i; i++ )
	{
		stLvi.iItem   = i;
		ListView_GetItem( ghSettiLvWnd, &stLvi );
		bCheck = ListView_GetCheckState( ghSettiLvWnd, i );

		StringCchPrintf( atKeyName, MIN_STRING, TEXT("File%d"), i );
		WritePrivateProfileString( TEXT("MoziScript"), atKeyName, atFilePath, gatMoziIni );

		StringCchPrintf( atKeyName, MIN_STRING, TEXT("Use%d"), i );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), bCheck );
		WritePrivateProfileString( TEXT("MoziScript"), atKeyName, atBuff, gatMoziIni );

		//�`�F�b�N�L���Ȃ�A���g���L�^���Ă���
		if( bCheck ){	MoziFileStore( atFilePath );	}
	}

	if( bCheck ){	MoziSpaceCreate(  );	}	//	���ϕ��ƍő��L�s��

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g�p�����t�@�C�������[�h���Ďg�p���e���č\������
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	bMode	��OSQL�\�z�@�O���[�h�̂�
*/
HRESULT MoziFileRebuild( HWND hWnd, UINT bMode )
{
	UINT	dCount, d;
	UINT	bCheck;
	TCHAR	atFileName[MAX_PATH], atFilePath[MAX_PATH];
	TCHAR	atKeyName[MIN_STRING];
	LVITEM	stLvi;

	dCount = GetPrivateProfileInt( TEXT("MoziScript"), TEXT("Count"), 0, gatMoziIni );

	if( bMode ){	MoziSqlItemDeleteAll(  );	}	//	�����ŃI��������SQL����ɂ���
	ListView_DeleteAllItems( ghSettiLvWnd );

	ZeroMemory( &stLvi, sizeof(stLvi) );
	stLvi.mask       = LVIF_TEXT;

	for( d = 0; dCount > d; d++ )
	{
		StringCchPrintf( atKeyName, MIN_STRING, TEXT("File%u"), d );

		ZeroMemory( atFilePath, sizeof(atFilePath) );
		GetPrivateProfileString( TEXT("MoziScript"), atKeyName, TEXT(""), atFilePath, MAX_PATH, gatMoziIni );

		StringCchPrintf( atKeyName, MIN_STRING, TEXT("Use%u"), d );
		bCheck = GetPrivateProfileInt( TEXT("MoziScript"), atKeyName, 0, gatMoziIni );


		StringCchCopy( atFileName, MAX_PATH, atFilePath );
		PathStripPath( atFileName );	//	�t�@�C�����̂Ƃ���؂�o��

		stLvi.iItem    = d;

		stLvi.pszText  = atFileName;
		stLvi.iSubItem = 0;
		ListView_InsertItem( ghSettiLvWnd, &stLvi );

		stLvi.pszText  = atFilePath;
		stLvi.iSubItem = 1;
		ListView_SetItem( ghSettiLvWnd, &stLvi );

		ListView_SetCheckState( ghSettiLvWnd, d, bCheck );

		//�`�F�b�N�L���Ȃ�A���g���L�^���Ă���
		if( bCheck && bMode ){	MoziFileStore( atFilePath );	}
	}

	if( bCheck && bMode ){	MoziSpaceCreate(  );	}	//	���ϕ��ƍő��L�s��

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�������̕��ςƂ�B��Ή��������������ꍇ�̃p�f�B���O�Ɏg��
*/
HRESULT MoziSpaceCreate( VOID )
{
	INT	iLine, iAvDot;

	//	��L�ő�s���ƕ��ϕ����m��
	MoziSqlItemCount( &iLine, &iAvDot );

	gdAvrWidth = iAvDot;
	gdMaxLine  = iLine;

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���e����������
	@param[in]	hWnd	�E�C���h�E�n���h���E���܂�Ӗ��͂Ȃ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MoziScriptInsert( HWND hWnd )
{
	UINT	bTranst;	//	���߂��邩�H
	INT		ixNowPage;	//	���̕ł��o���Ă���
	INT		ixTmpPage;	//	��Ɨp�ɂ���
	INT		iXhideBuf = 0, iYhideBuf = 0;

	LPVOID		pBuffer;
	INT			x, y, iByteSize, cmr;
	INT			iX = 0, iY = 0;
	HWND		hLyrWnd;
	RECT		rect;

	MZTM_ITR	itMzitm;

	bTranst = IsDlgButtonChecked( hWnd, IDCB_MZSCR_TRANSPARENT );
	//���߂Ȃ�A�땶�����J�L�R����΂����B�����Ȃ�A���[�N�y�[�W���\���ō���āA�I����������

	ixNowPage = gixFocusPage;	//	�Ŕԍ��o�b�N�A�b�v
	ixTmpPage = gixFocusPage;

#ifdef DO_TRY_CATCH
	try{
#endif

	if( !(bTranst) )	//	�ꎞ�łɍ쐬
	{
		ixTmpPage = DocPageCreate( -1 );	//	��Ɨp�ꎞ�ō쐬
		gixFocusPage = ixTmpPage;			//	���ڕŕύX

		iXhideBuf = gdHideXdot;		//	�X�N���[���ʒu�o�b�N�A�b�v
		iYhideBuf = gdViewTopLine;
		gdHideXdot = 0;
		gdViewTopLine = 0;
	}
	//	���߂Ȃ�{�łɒ��ŁA�񓧉߂Ȃ�_�~�[�łɏ����Ă���{�łɓ]������

	//	�}�������ɂ́A���C���{�b�N�X���\�������Ŏg��
	hLyrWnd = LayerBoxVisibalise( GetModuleHandle(NULL), TEXT(" "), 0x10 );
	//	�_�~�[������n���āA�Ƃ肠�����g���쐬

	GetWindowRect( ghMoziViewWnd, &rect );

	cmr = 0;
	//	�e��������
	for( itMzitm = gvcMoziItem.begin( ); gvcMoziItem.end( ) != itMzitm; itMzitm++ )
	{
		if( !(itMzitm->ptAA) )	continue;	//	�f�[�^�������Ȃ��΂��΂����͂�

		//	���e��]��
		LayerStringReplace( hLyrWnd, itMzitm->ptAA );

		//	���C���̈ʒu��ύX
		x = (rect.left + gstFrmSz.x) + itMzitm->iLeft;

		if( 0 >= itMzitm->iLeft ){	cmr = 0;	}
		x += (gdMoziInterval * cmr);
		cmr++;

		y = (rect.top + gstFrmSz.y)  + itMzitm->iTop;

		LayerBoxPositionChange( hLyrWnd, x, y );

		//	�㏑������
		LayerContentsImportable( hLyrWnd, IDM_LYB_OVERRIDE, &iX, &iY, D_INVISI );
	}

	if( bTranst )	//	�L�����b�g�̈ʒu��K���ɂ��킹��
	{
		//	���߂Ȃ�{�łɁA�񓧉߂Ȃ�ꎞ�łɃ��C�����Ă�
	}
	else	//	�ꎞ�ł���Ԃ�ǂ��ĉ��߂ď���
	{
		iByteSize = DocPageTextGetAlloc( gitFileIt, ixTmpPage, D_UNI, &pBuffer, TRUE );

	//	gixFocusPage = ixNowPage;	//	���ɖ߂�
		DocPageDelete( ixTmpPage, ixNowPage );	//	�ꎞ�ō폜���Ĉړ�
//		DocPageChange( ixNowPage  );	//	�폜������ňړ�

		//	���C���̈ʒu��ύX
		GetWindowRect( ghViewWnd, &rect );	//	�ҏW�r���[����ʒu�ł���
		x = rect.left;// + gstFrmSz.x;
		y = rect.top;//  + gstFrmSz.y;
		ViewPositionTransform( &x, &y, 1 );
		LayerBoxPositionChange( hLyrWnd, x, y );

		gdHideXdot = iXhideBuf;
		gdViewTopLine = iYhideBuf;

		//	���e��]��
		LayerStringReplace( hLyrWnd, (LPTSTR)pBuffer );

		//	�㏑������
		LayerContentsImportable( hLyrWnd, IDM_LYB_OVERRIDE, &iX, &iY, D_INVISI );

		FREE(pBuffer);
	}

	//�L�����b�g�ʒu�C��
	//	�܂��ꏊ���m�F
	GetWindowRect( ghViewWnd, &rect );
	x = rect.left + LINENUM_WID;
	y = rect.top  + RULER_AREA;

	GetWindowRect( ghMoziViewWnd, &rect );
	iX = (rect.left + gstFrmSz.x) - x;
	iY = (rect.top  + gstFrmSz.y) - y;
	//	�����ɂ͂ݏo���Ă���A�����̓}�C�i�X�ɂȂ��Ă���

	iY /= LINE_HEIGHT;

	//	���̎��_�ł́A�X�N���[���ɂ��Y�����l������ĂȂ�
	iX += gdHideXdot;
	iY += gdViewTopLine;	//	��������ő��v

	if( gdHideXdot >    iX )	iX = gdHideXdot + 11;	//	�O�̂���
	if( gdViewTopLine > iY )	iY = gdViewTopLine;

	ViewPosResetCaret( iX, iY );	

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return ETC_MSG( err.what(), E_FAIL );	}
	catch( ... ){	return  ETC_MSG( ("etc error"), E_FAIL );	}
#endif

	//	�I����������
	DestroyWindow( hLyrWnd );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Ŗ��̓��e���Ԃ�����
	@param[in]	ptLter	���ڂ̖��O�E��������NULL
	@param[in]	ptCont	���ڂ̓��e
	@param[in]	cchSize	���e�̕�����
	@return		UINT	���ɈӖ��Ȃ�
*/
UINT CALLBACK MoziItemTablise( LPTSTR ptLter, LPCTSTR ptCont, INT cchSize )
{
	UINT_PTR	cchSz;
	LPTSTR		ptItem;

	LPTSTR	ptCaret, ptNext;
//	TCHAR	ch;
	INT		cl, dot, maxd;

	StringCchLength( ptLter, STRSAFE_MAX_CCH, &cchSz );
	if( 1 != cchSz )	return 0;
	//	�K�C�h���P���łȂ��ƈӖ�������

	ptItem = (LPTSTR)malloc( (cchSize+1) * sizeof(TCHAR) );
	ZeroMemory( ptItem, (cchSize+1) * sizeof(TCHAR) );
	StringCchCopyN( ptItem, (cchSize+1), ptCont, cchSize );

	//	�s���A�ő�h�b�g���̉�͂����邩
	cl = 0;
	maxd = 0;
	ptCaret = ptItem;
	ptNext  = ptCaret;
	while( *ptCaret )
	{
		if( 0x000D == *ptCaret )
		{
			*ptCaret = 0x0000;	//	��������I�_�ɂ���
			MoziItemRemovePeriod( ptNext  );	//	�擪�ƏI���̃s�����h�����j�R�[�h�X�y�c�i�Y�ɒu������
			dot = ViewStringWidthGet( ptNext );
			*ptCaret = 0x000D;

			if( maxd <= dot )	maxd =  dot;	//	�h�b�g�J�E���g
			cl++;	//	�s�J�E���g

			ptCaret++;	//	0x000A�Ɉړ�
			ptCaret++;	//	���̍s�̐擪�Ɉړ�
			ptNext  = ptCaret;	//	�擪���m��
		}
		else
		{
			ptCaret++;	//	���̕����ֈړ�
		}
	}
	//	�ŏI�s�ɂ���
	MoziItemRemovePeriod( ptNext  );	//	�擪�ƏI���̃s�����h�����j�R�[�h�X�y�c�i�Y�ɒu������
	dot = ViewStringWidthGet( ptNext );
	if( maxd <= dot )	maxd = dot;
	cl++;	//	�s�J�E���g


	MoziSqlItemInsert( ptLter, ptItem, cl, maxd );	//	�f�[�^�x�[�X�ɃK���K���o�^

	FREE(ptItem);

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	��s�̃f�[�^���󂯎���āA�擪�ƏI���̃s�����h���R�h�b�g�̃��j�R�[�h�󔒂ɒu��������
	@param[in]	ptText	�^�[�Q�b�g�s�ENULL�ŏI����Ă��邱��
*/
VOID MoziItemRemovePeriod( LPTSTR ptText )
{
	UINT_PTR	cchSize;
	UINT		d;

	StringCchLength( ptText, STRSAFE_MAX_CCH, &cchSize );
	if( 0 == cchSize )	return;	//	������Ȃ��Ȃ�i�j�����Ȃ�

	//	�擪�s�����h����
	for( d = 0; cchSize > d; d++ )
	{
		if( TEXT('.') != ptText[d] )	break;
		//	�s�����h���ᖳ���Ȃ�����I���
		ptText[d] = (TCHAR)0x2006;
	}

	//	�I���s�����h����
	for( d = (cchSize-1); 0 < d; d-- )
	{
		if( TEXT('.') != ptText[d] )	break;
		//	�s�����h���ᖳ���Ȃ�����I���
		ptText[d] = (TCHAR)0x2006;
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�����t�@�C����ǂݍ���łr�p�k�ɒ��߂Ă���
	@param[in]	ptFilePath	�ǂ݂��ރt�@�C����
*/
HRESULT MoziFileStore( LPTSTR ptFilePath )
{
	HANDLE		hFile;
	DWORD		readed, dByteSz;
	UINT_PTR	cchSize;
	LPSTR		pcStr;
	LPTSTR		ptText;

	//	���b�c�I�[�|��
	hFile = CreateFile( ptFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE == hFile ){	return E_INVALIDARG;	}

	dByteSz = GetFileSize( hFile, NULL );
	pcStr = (LPSTR)malloc( dByteSz + 2 );
	ZeroMemory( pcStr, dByteSz + 2 );

	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
	ReadFile( hFile, pcStr, dByteSz, &readed, NULL );
	CloseHandle( hFile );	//	���e�S����荞�񂾂���J��

	//	�g���q���m�F
	if( FileExtensionCheck( ptFilePath, TEXT(".ast") ) )
	{
		ptText = SjisDecodeAlloc( pcStr );	//	SJIS�̓��e�����j�R�[�h�ɂ���
		FREE(pcStr);	//	��������͏I���E���̓��j�R�[�h

		StringCchLength( ptText, STRSAFE_MAX_CCH, &cchSize );

		MoziSqlTransOnOff( M_CREATE );
		DocStringSplitAST( ptText, cchSize, MoziItemTablise );
		MoziSqlTransOnOff( M_DESTROY );

		FREE(ptText);
	}
	else
	{
		FREE(pcStr);	//	��������͏I���
		return E_NOTIMPL;
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------



/*!
	�����L���b�V���p�I���������c�a
*/
HRESULT MoziSqlTableOpenClose( UINT bMode )
{
	//	�c���[���
	CONST CHAR	cacMoziTable[] = { ("CREATE TABLE MoziScr ( id INTEGER PRIMARY KEY, letter TEXT, aacont TEXT, line INTEGER, dot INTEGER )") };
	INT		rslt;
	sqlite3_stmt	*statement;

	if( bMode )
	{
		rslt = sqlite3_open( (":memory:"), &gpMoziTable );
		if( SQLITE_OK != rslt ){	SQL_DEBUG( gpMoziTable );	return E_FAIL;	}

		//�c���[�e�[�u���𐶐�
		rslt = sqlite3_prepare( gpMoziTable, cacMoziTable, -1, &statement, NULL );
		if( SQLITE_OK != rslt ){	SQL_DEBUG( gpMoziTable );	return E_ACCESSDENIED;	}
		rslt = sqlite3_step( statement );	//	���s
		if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpMoziTable );	return E_ACCESSDENIED;	}
		rslt = sqlite3_finalize(statement);

	}
	else
	{

		if( gpMoziTable ){	rslt = sqlite3_close( gpMoziTable );	}
		gpMoziTable = NULL;
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	SQLite��Transaction���������߂���
	@param[in]	mode	��O�J�n�@�O�I��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MoziSqlTransOnOff( BYTE mode )
{
	if( mode )
	{
		sqlite3_exec( gpMoziTable, "BEGIN TRANSACTION",  NULL, NULL, NULL );	//	�g�����U�N�V�����J�n
	}
	else
	{
		sqlite3_exec( gpMoziTable, "COMMIT TRANSACTION", NULL, NULL, NULL );	//	�g�����U�N�V�����I��
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


/*!
	�f�B���N�g�����t�@�C���̃f�[�^�ꎞ�o�b�t�@�Ƀh�s���b
	@param[in]	ptLter	�����R�[�h
	@param[in]	ptCont	�����̂`�`�f�[�^
	@param[in]	iLine	�s��
	@param[in]	iDot	�ő�h�b�g��
	@return	UINT	���ܓo�^����ID�ԍ�
*/
UINT MoziSqlItemInsert( LPTSTR ptLter, LPTSTR ptCont, INT iLine, INT iDot )
{
	CONST CHAR	acMoziItemInsert[] = { ("INSERT INTO MoziScr ( letter, aacont, line, dot ) VALUES ( ?, ?, ?, ? )") };
	CONST CHAR	acAddNumCheck[] = { ("SELECT LAST_INSERT_ROWID( ) FROM MoziScr") };

	INT		rslt;
	UINT	iRast = 0;
	sqlite3_stmt	*statement;


	if( !(gpMoziTable) ){	TRACE( TEXT("MOZI NoDatabase") );	return 0;	}

	rslt = sqlite3_prepare( gpMoziTable, acMoziItemInsert, -1, &statement, NULL);
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpMoziTable );	return 0;	}

	sqlite3_reset( statement );
	rslt = sqlite3_bind_text16( statement, 1, ptLter, -1, SQLITE_STATIC );	//	letter
	rslt = sqlite3_bind_text16( statement, 2, ptCont, -1, SQLITE_STATIC );	//	aacont
	rslt = sqlite3_bind_int(    statement, 3, iLine );	//	line
	rslt = sqlite3_bind_int(    statement, 4, iDot );	//	dot

	rslt = sqlite3_step( statement );
	if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpMoziTable );	}

	sqlite3_finalize( statement );

	//	���ǉ�������̃A�����擾
	rslt = sqlite3_prepare( gpMoziTable, acAddNumCheck, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpMoziTable );	return 0;	}

	rslt = sqlite3_step( statement );

	iRast = sqlite3_column_int( statement, 0 );

	sqlite3_finalize( statement );

	return iRast;
}
//-------------------------------------------------------------------------------------------------


/*!
	�������w�肵�Ă`�`�Q�b�g
	@param[in]	ch		����
	@param[in]	piLine	�s�������o�b�t�@
	@param[in]	piDot	�ő�h�b�g�������o�b�t�@
	@return	LPTSTR	�`�`�{����Allocate���Ė߂��E�J���͎󂯂��K���ł�邱�ƁE����������NULL
*/
LPTSTR MoziSqlItemSelect( TCHAR ch, LPINT piLine, LPINT piDot )
{
	CONST CHAR	acSelect[] = { ("SELECT * FROM MoziScr WHERE letter == ?") };
	INT		rslt,iLine, iDot;
	UINT	index = 0;
	TCHAR	atMozi[3];
	LPTSTR	ptAac = NULL;
	sqlite3_stmt*	statement;


	if( !(gpMoziTable) ){	return NULL;	}

	if( !(piLine) ){	return NULL;	}	*piLine = 0;
	if( !(piDot)  ){	return NULL;	}	*piDot  = 0;


	rslt = sqlite3_prepare( gpMoziTable, acSelect, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpMoziTable );	return 0;	}

	atMozi[ 0] = ch;	atMozi[1] = NULL;	
	sqlite3_reset( statement );
	rslt = sqlite3_bind_text16( statement, 1, atMozi, -1, SQLITE_STATIC );	//	letter

	rslt = sqlite3_step( statement );
	if( SQLITE_ROW == rslt )
	{
		index = sqlite3_column_int( statement , 0 );	//	id
		StringCchCopy( atMozi, 3, (LPCTSTR)sqlite3_column_text16( statement, 1 ) );	//	letter
		rslt = sqlite3_column_bytes16( statement, 2 );	//	�T�C�Y�m�F
		rslt += 2;
		ptAac = (LPTSTR)malloc( rslt );
		ZeroMemory( ptAac, rslt );
		StringCchCopy( ptAac, (rslt / sizeof(TCHAR)), (LPCTSTR)sqlite3_column_text16( statement, 2 ) );	//	aacont
		iLine = sqlite3_column_int( statement , 3 );	//	line
		iDot  = sqlite3_column_int( statement , 4 );	//	dot
		*piLine = iLine;
		*piDot  = iDot;
	}

	sqlite3_finalize( statement );

	return ptAac;
}
//-------------------------------------------------------------------------------------------------


/*!
	�o�^����Ă�����m�ہE�h�b�g�����ϒl�A�s���ő�l���m��
	@param[in]	piLine	�s�������o�b�t�@
	@param[in]	piAvDot	���σh�b�g�������o�b�t�@
	@return		�o�^����Ă����
*/
UINT MoziSqlItemCount( LPINT piLine, LPINT piAvDot )
{
	INT		iLine, iAvDot;
	UINT	rslt, iCount;
	sqlite3_stmt*	statement;

	rslt = sqlite3_prepare( gpMoziTable, ("SELECT COUNT(id), MAX(line), AVG(dot) FROM MoziScr"), -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpMoziTable );	return 0;	}

	sqlite3_step( statement );
	iCount = sqlite3_column_int( statement, 0 );
	iLine  = sqlite3_column_int( statement, 1 );
	iAvDot = sqlite3_column_int( statement, 2 );
	sqlite3_finalize(statement);

	if( piLine )	*piLine  = iLine;
	if( piAvDot )	*piAvDot = iAvDot;

	return iCount;
}
//-------------------------------------------------------------------------------------------------

/*!
	�S�f�[�^�j��
*/
HRESULT MoziSqlItemDeleteAll( VOID )
{
	INT		rslt;
	sqlite3_stmt	*statement;

	if( !(gpMoziTable) ){	return E_NOTIMPL;	}

	rslt = sqlite3_prepare( gpMoziTable, ("DELETE FROM MoziScr"), -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpMoziTable );	return E_OUTOFMEMORY;	}
	rslt = sqlite3_step( statement );
	sqlite3_finalize( statement );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------




