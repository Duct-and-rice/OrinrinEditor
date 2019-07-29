/*! @file
	@brief MLT�c���[�Ƃ��̕���
	���̃t�@�C���� MaaTemplate.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/06/21
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

HukuTabs �̒��g
2ch\���v�h��\���v\���v11�i�~�j�T�C�Y�j.mlt
2ch\���v�h��\���Ȃ��v\���Ȃ��v01�i��{�j.mlt

PathStripPath	���[���t�@�C�����f�B���N�g���֌W�Ȃ��ɐ؂�o��

*/


#include "stdafx.h"
#include "OrinrinEditor.h"
#include "MaaTemplate.h"
//-------------------------------------------------------------------------------------------------

#ifdef _ORRVW
#define  MAATMPLT_CLASS_NAME	TEXT("ORINRINVIEWER")
#else
#define  MAATMPLT_CLASS_NAME	TEXT("MULTILINEAA_CLASS")
#endif
#define MA_WIDTH	320
#define MA_HEIGHT	320

#ifdef MAA_TEXT_FIND
#define TXTFIND_BARHEI	25
#endif
//-------------------------------------------------------------------------------------------------

static HINSTANCE	ghInst;		//!<	�A�v���̎���

static  HWND	ghMainWnd;		//!<	�{�̃E�C���h�E


static  HWND	ghMaaWnd;		//!<	���̃E�C���h�E�Estatic���������Ă̓C�P�i�C

static  HWND	ghStsBarWnd;	//!<	�X�e�[�^�X�o�[�n���h��

EXTERNED HWND	ghSplitaWnd;	//!<	�X�v���b�g�o�[�n���h��

EXTERNED UINT	gbAAtipView;	//!<	��O�ŁA�`�`�c�[���`�b�v�\��

EXTERNED HWND	ghMaaFindDlg;	//!<	MAA�����_�C�����O�n���h��

static TCHAR	gatProfilePath[MAX_PATH];	//!<	�v���t�@�C���f�B���N�g��

static TCHAR	gatTemplatePath[MAX_PATH];	//!<	MLT���[�g�f�B���N�g��

static list<OPENHIST>	gltProfHist;	//!<	�v���t�J���������E
EXTERNED HMENU	ghProfHisMenu;			//!<	����\�����镔���E���I�ɓ��e�쐬���Ȃ�����

static CONST INT	giStbRoom[] = { 150 , 350 , -1 };
//-------------------------------------------------------------------------------------------------

LRESULT	CALLBACK MaaTmpltWndProc( HWND, UINT, WPARAM, LPARAM );	//!<	
BOOLEAN	Maa_OnCreate( HWND, LPCREATESTRUCT );			//!<	WM_CREATE �̏����E�Œ�Edit�Ƃ�����
VOID	Maa_OnPaint( HWND );							//!<	WM_PAINT �̏����E�g���`��Ƃ�
VOID	Maa_OnDestroy( HWND );							//!<	WM_DESTROY �̏����EBRUSH�Ƃ��̃I�u�W�F�N�g�̔j���Y��Ȃ��悤��
LRESULT	Maa_OnNotify( HWND , INT, LPNMHDR );			//!<	
VOID	Maa_OnDrawItem( HWND, CONST DRAWITEMSTRUCT * );	//!<	
VOID	Maa_OnMeasureItem( HWND, MEASUREITEMSTRUCT * );	//!<	

#ifndef _ORRVW	//	�G�f�B�^�̂�
VOID	Maa_OnActivate( HWND, UINT, HWND, BOOL );		//!<	
VOID	Maa_OnShowWindow( HWND, BOOL, UINT );	//!<	
#endif


#define TREEPROF_AUTOCHECK

INT_PTR	CALLBACK TreeProfileDlgProc( HWND, UINT, WPARAM, LPARAM );	//!<	
HRESULT	TreeProfListUp( HWND, HWND, LPTSTR, HTREEITEM, UINT, INT );	//!<	
UINT	TreeLoadNodeProc( HWND, HWND, HTREEITEM, UINT );			//!<	
VOID	TreeProfCheckState( HWND, HTREEITEM, UINT );				//!<	
#ifdef TREEPROF_AUTOCHECK
UINT	TreeProfCheckExistent( HWND, LPTSTR, HWND, HTREEITEM, UINT );	//!<	
#endif

//-------------------------------------------------------------------------------------------------

/*!
	�����s�`�`�e���v���E�C���h�E�̍쐬
	@param[in]	hInstance	�A�v���̃C���X�^���X
	@param[in]	hParentWnd	�ҏW�r���[�̂������Ă�E�C���h�E�̃n���h���E���ꂪ�{��
	@param[in]	pstFrame	
	@return		������r���[�̃E�C���h�E�n���h��
*/
HWND MaaTmpltInitialise( HINSTANCE hInstance, HWND hParentWnd, LPRECT pstFrame )
{
	WNDCLASSEX	wcex;
	RECT	wdRect, rect;
#ifndef _ORRVW
	RECT	sbRect;
#endif
	INT		bMode = 0;

#ifdef _ORRVW
	INT	bTopMost;
#endif

	WIN32_FIND_DATA	stFindData;
	HANDLE	hFind;

	ghMainWnd  = hParentWnd;
	ghInst = hInstance;

	//	��p�̃E�C���h�E�N���X�쐬
	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= MaaTmpltWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszClassName	= MAATMPLT_CLASS_NAME;
#ifdef _ORRVW
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ORINRINEDITOR));
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ORINRINVIEWER);
#else
	wcex.hIcon			= NULL;
	wcex.hIconSm		= NULL;
	wcex.lpszMenuName	= NULL;
#endif

	RegisterClassEx( &wcex );

	ghMaaFindDlg = NULL;	//	������

#ifdef _ORRVW
	SplitBarClass( hInstance );	//	�X�v���b�g�o�[�̏���
#endif

	InitWindowPos( INIT_LOAD, WDP_MAATPL, &rect );
	if( 0 == rect.right || 0 == rect.bottom )	//	���������O�̓f�[�^����
	{
		GetWindowRect( hParentWnd, &wdRect );
#ifdef _ORRVW
		rect.left = ( wdRect.right  - MA_WIDTH ) / 2;
		rect.top  = ( wdRect.bottom - MA_HEIGHT ) / 2;
#else
		rect.left   = wdRect.right + 64;
		rect.top    = wdRect.top + 64;	//	�I�t�Z�b�g�́A���̃E�C���h�E�Ƃ��Ԃ�Ȃ���ǂ�
#endif
		rect.right  = MA_WIDTH;
		rect.bottom = MA_HEIGHT;
		InitWindowPos( INIT_SAVE, WDP_MAATPL, &rect );//�N�����ۑ�
	}

	ghMaaWnd = CreateWindowEx(
#ifdef _ORRVW
		0, MAATMPLT_CLASS_NAME, TEXT("Orinrin Viewer"),
		WS_OVERLAPPEDWINDOW,
#else
		WS_EX_TOOLWINDOW, MAATMPLT_CLASS_NAME, TEXT("Multi Line AA Template"),
		WS_POPUP | WS_THICKFRAME | WS_BORDER | WS_CAPTION,
#endif
		rect.left, rect.top, rect.right, rect.bottom,
		NULL, NULL, hInstance, NULL);

	if( !(ghMaaWnd) )	return NULL;

	//	��ɍőS�ʂɕ\�����H
#ifdef _ORRVW
	bTopMost = InitParamValue( INIT_LOAD, VL_MAA_TOPMOST, 1 );
	if( bTopMost )
	{
		SetWindowPos( ghMaaWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
		CheckMenuItem( GetMenu(ghMaaWnd), IDM_TOPMOST_TOGGLE, MF_CHECKED );
	}

	ShowWindow( ghMaaWnd, SW_SHOW );
	UpdateWindow( ghMaaWnd );
#else
	if( InitParamValue( INIT_LOAD, VL_MAA_TOPMOST, 1 ) )	//	��\���Ȃ�ON���Ȃ�
	{
		if( InitWindowTopMost( INIT_LOAD, WDP_MAATPL , 0 ) )
		{	SetWindowPos( ghMaaWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );	}

		ShowWindow( ghMaaWnd, SW_SHOW );
		UpdateWindow( ghMaaWnd );
	}
#endif

	OpenProfileInitialise( ghMaaWnd );

	ZeroMemory( gatTemplatePath, sizeof(gatTemplatePath) );

	//	�v���t�@�C�����[�h
	ZeroMemory( gatProfilePath, sizeof(gatProfilePath) );
	InitParamString( INIT_LOAD, VS_PROFILE_NAME, gatProfilePath );

	//	���̃t�@�C���͑��݂��邩�H
	hFind = FindFirstFile( gatProfilePath, &stFindData );	//	TEXT("*")
	if( INVALID_HANDLE_VALUE != hFind ){	FindClose( hFind  );	}
	else{		ZeroMemory( gatProfilePath, sizeof(gatProfilePath) );	};

	if( NULL != gatProfilePath[0] )	//	�N����������Ή������Ȃ�
	{
		bMode = TreeProfileMake( ghMaaWnd, gatProfilePath );
		if( 0 > bMode ){	bMode = 0;	}
	}

	TreeConstruct( ghMaaWnd, gatTemplatePath, TRUE );

	gbAAtipView = InitParamValue( INIT_LOAD, VL_MAATIP_VIEW, 1 );

#ifndef _ORRVW
	//	���������l�������ċA��
	MaaTabBarSizeGet( pstFrame );
	pstFrame->top = pstFrame->bottom;
	GetClientRect( ghStsBarWnd, &sbRect );
	pstFrame->bottom = sbRect.bottom;	//	�X�e�[�^�X�o�[�̕��̖ʓ|����
#endif

	return ghMaaWnd;
}
//-------------------------------------------------------------------------------------------------

/*!
	�l�`�`���̈ʒu���Z�b�g
	@param[in]	hMainWnd	���C���E�C���h�E�n���h��
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT MaaTmpltPositionReset( HWND hMainWnd )
{
	RECT	wdRect, rect;

	GetWindowRect( hMainWnd, &wdRect );
	rect.left   = wdRect.right + 64;
	rect.top    = wdRect.top + 64;	//	�I�t�Z�b�g�́A���̃E�C���h�E�Ƃ��Ԃ�Ȃ���ǂ�
	rect.right  = MA_WIDTH;
	rect.bottom = MA_HEIGHT;

	SetWindowPos( ghMaaWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW | SWP_NOZORDER );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


#ifndef _ORRVW
/*!
	�l�`�`���̂u�h�d�v��toggle����
	@param[in]	bSet	��OToggle�����@�O��Ԋm�F
	@return	��O�����Ă�@�O�����Ă�
*/
BOOLEAN MaaViewToggle( UINT bSet )
{
	BOOL	bStyle;

	if( !(ghMaaWnd) )	return FALSE;

	bStyle = IsWindowVisible( ghMaaWnd );	//	���̏�Ԋm�F

	if( bSet )	//	�����ē���ւ���
	{
		if( bStyle )	ShowWindow( ghMaaWnd, SW_HIDE );
		else			ShowWindow( ghMaaWnd, SW_SHOW );

		bStyle = !(bStyle);
	}

	return bStyle;
}
//-------------------------------------------------------------------------------------------------

#endif

/*!
	�E�C���h�E�v���V�[�W��
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	message		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam		�ǉ��̏��P
	@param[in]	lParam		�ǉ��̏��Q
	@retval 0	���b�Z�[�W�����ς�
	@retval no0	�����ł͏����������ɉ�
*/
LRESULT CALLBACK MaaTmpltWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	UINT	uRslt;
#ifdef MAA_TEXT_FIND
	INT		itemID;
	HDC		hdc;
	HWND	hWndChild;
#endif

	switch( message )
	{
		HANDLE_MSG( hWnd, WM_CREATE,      Maa_OnCreate );		//	��ʂ̍\���p�[�c�����B�{�^���Ƃ�
		HANDLE_MSG( hWnd, WM_PAINT,       Maa_OnPaint );		//	��ʂ̍X�V�Ƃ�
		HANDLE_MSG( hWnd, WM_COMMAND,     Maa_OnCommand );		//	�{�^�������ꂽ�Ƃ��̃R�}���h����
		HANDLE_MSG( hWnd, WM_DESTROY,     Maa_OnDestroy );		//	�\�t�g�I�����̏���
		HANDLE_MSG( hWnd, WM_NOTIFY,      Maa_OnNotify );		//	�R�����R���g���[���̌ʃC�x���g
		HANDLE_MSG( hWnd, WM_SIZE,        Maa_OnSize  );		//	
		HANDLE_MSG( hWnd, WM_CHAR,        Maa_OnChar  );		//	
		HANDLE_MSG( hWnd, WM_DRAWITEM,    Maa_OnDrawItem  );	//	
		HANDLE_MSG( hWnd, WM_MEASUREITEM, Maa_OnMeasureItem );	//	
		HANDLE_MSG( hWnd, WM_CONTEXTMENU, Maa_OnContextMenu );	//	
#ifndef _ORRVW	//	�G�f�B�^�̂�
		HANDLE_MSG( hWnd, WM_ACTIVATE,    Maa_OnActivate  );	//	�A�N�e�B�u�ɂȂ�����͂��ꂽ��
		HANDLE_MSG( hWnd, WM_SHOWWINDOW,  Maa_OnShowWindow );	//	
#endif
		HANDLE_MSG( hWnd, WM_KEYDOWN,     Aai_OnKey );			//	20120221
		HANDLE_MSG( hWnd, WM_KEYUP,       Aai_OnKey );			//	

		HANDLE_MSG( hWnd, WM_VSCROLL,     Aai_OnVScroll );		//	

#ifdef MAA_TEXT_FIND
		case  WM_CTLCOLORSTATIC:	//	�����񕔕��̐F�ύX
			hdc = (HDC)(wParam);
			hWndChild = (HWND)(lParam);

			itemID = GetDlgCtrlID( hWndChild );

			//	
			if( IDS_MAA_TXTFIND_FIND   == itemID || IDS_MAA_TXTFIND_MSGBOX == itemID || 
				IDB_MAA_TXTFIND_TOP_GO == itemID || IDB_MAA_TXTFIND_NOW_GO == itemID )
			{
				SetBkColor( hdc, GetSysColor( COLOR_WINDOW ) );
				return (LRESULT)GetSysColorBrush( COLOR_WINDOW );
			}
			break;
#endif
		case WM_MOUSEWHEEL:	//	�Ԃ�l���K�v�ȏꍇ���l��
			uRslt = Maa_OnMouseWheel( hWnd, (INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam), (INT)(SHORT)HIWORD(wParam), (UINT)(SHORT)LOWORD(wParam) );
			break;

		default:	break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

#ifndef _ORRVW	//	�G�f�B�^�̂�

/*!
	�E�C���h�E����\�����\����Ԃɂ��ꂽ�Ƃ��ɔ��ł���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	fShow	��O�\����Ԃɂ��ꂽ�@�O��\����Ԃɂ��ꂽ
	@param[in]	status	�OShowWindow�����ő��삳�ꂽ�@��O���̑��̗��R
*/
VOID Maa_OnShowWindow( HWND hWnd, BOOL fShow, UINT status )
{
	INT	rslt;
/*
SW_OTHERUNZOOM	 4	The window is being uncovered because a maximize window was restored or minimized.
SW_OTHERZOOM	 2	The window is being covered by another window that has been maximized.
SW_PARENTCLOSING 1	The window's owner window is being minimized.
SW_PARENTOPENING 3	The window's owner window is being restored.
*/
	TRACE( TEXT("MAA SHOWWINDOW fShow[%u] status[%u]"), fShow, status );

	if( fShow )	//	�\����Ԃɂ��ꂽ�Ƃ�
	{
		rslt = InitParamValue( INIT_LOAD, VL_MAA_TOPMOST, 1 );
		if( !(rslt) )	//	��\���w���ł������ꍇ��
		{
			ShowWindow( ghMaaWnd, SW_HIDE );	//	��\���ɂ��Ă���
		}
	}
	//�����ł���u�\�������

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�������ԂȂ�����Ă΂��
	@param[in]	hWnd			�E�C���h�E�n���h��
	@param[in]	state			���
	@param[in]	hWndActDeact	�������Ԗ��͔񂠂����ԂɂȂ��ENULL�ł���
	@param[in]	fMinimized		�ŏ����Ȃ�NULL
*/
VOID Maa_OnActivate( HWND hWnd, UINT state, HWND hWndActDeact, BOOL fMinimized )
{
//	LONG_PTR	rdExStyle;
//	HWND		hWorkWnd;
	BOOL		bStyle;

/*
WA_INACTIVE		0	�E�B���h�E����A�N�e�B�u�ɂȂ�܂��B
WA_ACTIVE		1	�}�E�X���N���b�N����ȊO�̕��@ (�E�B���h�E�̑I���ɃL�[�{�[�h �C���^�[�t�F�C�X���g�p������@�Ȃ�) �ŁA�E�B���h�E���A�N�e�B�u�ɂȂ�܂��B
WA_CLICKACTIVE	2	�}�E�X�N���b�N�ɂ���āA�E�B���h�E���A�N�e�B�u�ɂȂ�܂��B
*/
	bStyle = IsWindowVisible( ghMaaWnd );	//	���̏�Ԋm�F
	TRACE( TEXT("MAA ACTIVATE[%d] STATE[%u] HWND[%X][%X] MIN[%u]"), bStyle, state, hWndActDeact, ghMainWnd, fMinimized );

//���C�����������͑��̃t���[�e�B���O������t�H�[�J�X�𓾂��ꍇ�͂Ȃɂ����Ȃ�
//�P�ƂŃA�N�e�B�x�[�g������A���C�������t�H�A�O���E���h�ɂ���H

	return;
}
//-------------------------------------------------------------------------------------------------

#endif

/*!
	�N���G�C�g�B
	@param[in]	hWnd			�e�E�C���h�E�̃n���h��
	@param[in]	lpCreateStruct	�A�v���P�[�V�����̏��������e
	@return	TRUE	���ɂȂ�
*/
BOOLEAN Maa_OnCreate( HWND hWnd, LPCREATESTRUCT lpCreateStruct )
{
	HINSTANCE lcInst = lpCreateStruct->hInstance;	//	�󂯎������������񂩂�A�C���X�^���X�n���h�����Ђ��ς�
	RECT	rect, sbRect, tbRect;

	INT	iTfTop;
	INT	spPos;


	GetClientRect( hWnd, &rect );

	//	�X�e�[�^�X�o�[
	ghStsBarWnd = CreateStatusWindow( WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP, TEXT(""), hWnd, IDSB_STATUSBAR );
	GetClientRect( ghStsBarWnd, &sbRect );
	rect.bottom -= sbRect.bottom;
	SendMessage( ghStsBarWnd, SB_SETPARTS, 3, (LPARAM)giStbRoom );

#ifdef MAA_TEXT_FIND
	//	�����p�[�c�p�G���A
	rect.bottom -= TXTFIND_BARHEI;
	iTfTop = rect.bottom + 1;
	//	�u�����v�X�^�e�B�b�N
	CreateWindowEx( 0, WC_STATIC, TEXT("����"), WS_VISIBLE | WS_CHILD | SS_RIGHT | SS_CENTERIMAGE, 1, iTfTop, 40, 23, hWnd, (HMENU)IDS_MAA_TXTFIND_FIND, lcInst, NULL );
	//	�����P��G�f�B�b�g
	CreateWindowEx( 0, WC_EDIT, TEXT(""), WS_VISIBLE | WS_CHILD | WS_BORDER, 42, iTfTop, 139, 23, hWnd, (HMENU)IDE_MAA_TXTFIND_TEXT, lcInst, NULL );
	//	�擪����E�\���ʒu���烉�a�I�{�^��
	CreateWindowEx( 0, WC_BUTTON, TEXT("�擪����"), WS_VISIBLE | WS_CHILD | WS_GROUP | BS_AUTORADIOBUTTON | BS_VCENTER, 183, iTfTop, 80, 23, hWnd, (HMENU)IDB_MAA_TXTFIND_TOP_GO, lcInst, NULL );
	CreateWindowEx( 0, WC_BUTTON, TEXT("���ł���"), WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_VCENTER, 265, iTfTop, 80, 23, hWnd, (HMENU)IDB_MAA_TXTFIND_NOW_GO, lcInst, NULL );
	CheckDlgButton( hWnd, IDB_MAA_TXTFIND_TOP_GO, BST_CHECKED );
	//	�������{�^��
	CreateWindowEx( 0, WC_BUTTON, TEXT("������"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 347, iTfTop, 65, 23, hWnd, (HMENU)IDB_MAA_TXTFIND_NEXT, lcInst, NULL );
	//	���b�Z�[�W�\���g
	CreateWindowEx( 0, WC_STATIC, TEXT(""), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE, 414, iTfTop, 120, 23, hWnd, (HMENU)IDS_MAA_TXTFIND_MSGBOX, lcInst, NULL );
#endif

	TreeInitialise( hWnd, lcInst, &rect );	//	�c���[�r���[�Ƃ��C�Ƀr���[���

	MaaTabBarSizeGet( &tbRect );

	//	�ݒ肩��X�v���b�g�o�[�̈ʒu����������
	spPos = InitParamValue( INIT_LOAD, VL_MAA_SPLIT, TMPL_DOCK );
	ghSplitaWnd = SplitBarCreate( lcInst, hWnd, spPos, tbRect.bottom, rect.bottom - tbRect.bottom );

	AaItemsInitialise( hWnd, lcInst, &rect );	//	MLT�̒��g�\���r���[����

	return TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	COMMAND���b�Z�[�W�̎󂯎��B�{�^�������ꂽ�Ƃ��Ŕ���
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	id			���b�Z�[�W�𔭐��������q�E�C���h�E�̎��ʎq	LOWORD(wParam)
	@param[in]	hwndCtl		���b�Z�[�W�𔭐��������q�E�C���h�E�̃n���h��	lParam
	@param[in]	codeNotify	�ʒm���b�Z�[�W	HIWORD(wParam)
	@return		�Ȃ�
*/
VOID Maa_OnCommand( HWND hWnd, INT id, HWND hwndCtl, UINT codeNotify )
{
	LONG_PTR	rdExStyle;

#ifdef _ORRVW
	if( IDM_OPEN_HIS_FIRST <= id && id <= IDM_OPEN_HIS_LAST )	//	�J��
	{
		OpenProfileLoad( hWnd, id );
		return;
	}
	else if( IDM_OPEN_HIS_CLEAR ==  id )	//	�t�@�C���I�[�|�������N�����[
	{
		OpenProfileLogging( hWnd, NULL );
		return;
	}
#endif

	switch( id )
	{
		//	���X�g�X�^�e�B�b�N�ł̃N���b�N�͂����ɂ���
		case IDSO_AAITEMS:	TRACE( TEXT("static") );	break;

		//	�h���t�g�{�[�h�I�[�|��
		case IDM_DRAUGHT_OPEN:	DraughtWindowCreate( GetModuleHandle(NULL), ghMaaWnd, 0 );	break;

		//	�T���l�C���I�[�|��
		case IDM_MAA_THUMBNAIL_OPEN:	DraughtWindowCreate( GetModuleHandle(NULL), ghMaaWnd, 1 );	break;

		//	�g�p�A�C�e���ꗗ�̃��X�g�{�b�N�X�ő��삪������
		case IDLB_FAVLIST:	FavListSelected( hWnd, codeNotify );	break;

		case IDM_TOPMOST_TOGGLE:	//	�펞�őS�ʂƒʏ�E�C���h�E�̃g�O��
			rdExStyle = GetWindowLongPtr( hWnd, GWL_EXSTYLE );
			if( WS_EX_TOPMOST & rdExStyle )
			{
				SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
#ifdef _ORRVW
				CheckMenuItem( GetMenu(hWnd), IDM_TOPMOST_TOGGLE, MF_UNCHECKED );
				InitParamValue( INIT_SAVE, VL_MAA_TOPMOST, 0 );
#else
				InitWindowTopMost( INIT_SAVE, WDP_MAATPL, 0 );
#endif
			}
			else
			{
				SetWindowPos( hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
#ifdef _ORRVW
				CheckMenuItem( GetMenu(hWnd), IDM_TOPMOST_TOGGLE, MF_CHECKED );
				InitParamValue( INIT_SAVE, VL_MAA_TOPMOST, 1 );
#else
				InitWindowTopMost( INIT_SAVE, WDP_MAATPL, 1 );
#endif
			}
#ifdef _ORRVW
			DrawMenuBar( hWnd );
#endif
			break;

		//	�`�`�^�C�g���R���{�b�N�X�ő��삪������
		case IDCB_AAITEMTITLE:	AaTitleSelect( hWnd, codeNotify );	break;

		//	�t�H�[�J�X���Ă�^�u�����
		case IDM_FILE_CLOSE:	TabMultipleSelDelete( hWnd );	break;


		//	�����ɗ���̂́A���C�����j���[����̑I���̂݁H�z���g���H

#ifdef _ORRVW
		case IDM_MAAITEM_BKCOLOUR:	MaaBackColourChoose( hWnd );	break;

		case IDM_ABOUT:	DialogBox( ghInst , MAKEINTRESOURCE(IDD_ORRVWR_ABOUTBOX), hWnd, About );	break;
		case IDM_EXIT:	DestroyWindow( hWnd );	break;

		case IDM_GENERAL_OPTION:	//	�ݒ�
			DialogBoxParam( ghInst, MAKEINTRESOURCE(IDD_ORRVWR_OPTION_DLG), hWnd, OptionDlgProc, NULL );
			break;

		case IDM_MAA_PROFILE_MAKE:	TreeProfileOpen( hWnd );	break;
		case IDM_TREE_RECONSTRUCT:	TreeProfileRebuild( hWnd  );	break;

#else
		case  IDM_WINDOW_CHANGE:	WindowFocusChange( WND_MAAT,  1 );	break;
		case  IDM_WINDOW_CHG_RVRS:	WindowFocusChange( WND_MAAT, -1 );	break;
#endif
		//	�d�R���e�L�X�g�E�A�N�Z�����[�^�@�u���j���[�E�A�N�Z�����[�^
		case IDM_FINDMAA_DLG_OPEN:	TreeMaaFileFind( hWnd );	break;
		//�S�������ł��邩�H

		case IDM_TMPLT_GROUP_NEXT:
		case IDM_TMPLT_GROUP_PREV:
			TRACE( TEXT("Ctrl�{����") );
			break;

#ifdef MAA_TEXT_FIND
		case IDM_FIND_JUMP_NEXT:	AacFindTextEntry( hWnd, 0 );	break;
		//	�`�`���e�����Z��
		case IDB_MAA_TXTFIND_NEXT:	AacFindTextEntry( hWnd, 1 );	break;
#endif
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
VOID Maa_OnPaint( HWND hWnd )
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
VOID Maa_OnDestroy( HWND hWnd )
{
	RECT	rect;
	DWORD	dwStyle;

	dwStyle = GetWindowStyle( hWnd );
	if( !(dwStyle & WS_MINIMIZE) )
	{
		//	�ő剻���Ă���L�^���Ȃ��H

		//	�X�v���b�g�o�[�̈ʒu���L�^
		SplitBarPosGet( ghSplitaWnd, &rect );
		InitParamValue( INIT_SAVE, VL_MAA_SPLIT, rect.left );

		//	�E�C���h�E�ʒu���L�^
		GetWindowRect( hWnd, &rect );
		rect.right  -= rect.left;
		rect.bottom -= rect.top;
		InitWindowPos( INIT_SAVE, WDP_MAATPL, &rect );//�I�����ۑ�
	}

	AaItemsInitialise( NULL, NULL, NULL );
	AacMatrixClear(   );	//	�����̓��e�S�j��

	TreeInitialise( NULL, NULL, NULL );

	SqlDatabaseOpenClose( M_DESTROY, NULL );

#ifdef _ORRVW

#ifdef USE_HOVERTIP
	HoverTipInitialise( NULL, NULL );
#endif
	DraughtInitialise( NULL, NULL );

	OpenProfileInitialise( NULL );

	PostQuitMessage( 0 );
#endif

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�̃T�C�Y�ύX
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	state	�E�C���h�E�̏�ԃt���O
	@param[in]	cx		�N���C�����g�w��
	@param[in]	cy		�N���C�����g�x����
*/
VOID Maa_OnSize( HWND hWnd, UINT state, INT cx, INT cy )
{
	RECT	rect, sbRect, tbRect;
	INT	iTfTop;

	GetClientRect( hWnd, &rect );

	MoveWindow( ghStsBarWnd, 0, 0, 0, 0, TRUE );	//	�X�e�[�^�X�o�[
	GetClientRect( ghStsBarWnd, &sbRect );
	rect.bottom -= sbRect.bottom;	//	�X�e�[�^�X�o�[�̕��̖ʓ|����

#ifdef MAA_TEXT_FIND
	//	�����p�[�c�p�G���A
	rect.bottom -= TXTFIND_BARHEI;
	iTfTop = rect.bottom + 1;

	//	�����p�[�c�̍Ĕz�u
	SetWindowPos( GetDlgItem(hWnd,IDS_MAA_TXTFIND_FIND),   HWND_TOP,   1, iTfTop, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	SetWindowPos( GetDlgItem(hWnd,IDE_MAA_TXTFIND_TEXT),   HWND_TOP,  42, iTfTop, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	SetWindowPos( GetDlgItem(hWnd,IDB_MAA_TXTFIND_TOP_GO), HWND_TOP, 183, iTfTop, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	SetWindowPos( GetDlgItem(hWnd,IDB_MAA_TXTFIND_NOW_GO), HWND_TOP, 265, iTfTop, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	SetWindowPos( GetDlgItem(hWnd,IDB_MAA_TXTFIND_NEXT),   HWND_TOP, 347, iTfTop, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	SetWindowPos( GetDlgItem(hWnd,IDS_MAA_TXTFIND_MSGBOX), HWND_TOP, 414, iTfTop, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
#endif

	TabBarResize( hWnd, &rect );	//	�^�u�o�[
	MaaTabBarSizeGet( &tbRect );
	rect.top = tbRect.bottom;
	rect.bottom -= tbRect.bottom;	//	�^�u�o�[�̕��̖ʓ|������

	SplitBarResize( ghSplitaWnd, &rect );	//	�l�`�`���̃T�C�Y�ύX�ɂ��X�v���b�g�o�[

	TreeResize( hWnd, &rect );		//	�c���[�r���[
	AaItemsResize( hWnd, &rect );	//	�ꗗ�r���[

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�m�[�e�B�t�@�C���b�Z�[�W�̏���
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	idFrom		NOTIFY�𔭐��������R���g���[���̂h�c
	@param[in]	pstNmhdr	NOTIFY�̏ڍ�
	@return		�����������e�Ƃ�
*/
LRESULT Maa_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	//	�c���[�r���[�̂��
	if( IDTV_ITEMTREE == idFrom ){	TreeNotify( hWnd , (LPNMTREEVIEW)pstNmhdr );	}

	//	�^�u�o�[�̂��
	if( IDTB_TREESEL == idFrom ){	TabBarNotify( hWnd , pstNmhdr );	}

	return 0;	//	�����Ȃ��Ȃ�O��߂�
}
//-------------------------------------------------------------------------------------------------

/*!
	�I�[�i�[�h���[�̔�����������
	@param[in]	hWnd			�e�E�C���h�E�̃n���h��
	@param[in]	*pstDrawItem	�h���[�ׂ̈̏��
*/
VOID Maa_OnDrawItem( HWND hWnd, CONST DRAWITEMSTRUCT *pstDrawItem )
{
	if( IDSO_AAITEMS == pstDrawItem->CtlID )
	{
		AaItemsDrawItem( hWnd, pstDrawItem );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I�[�i�[�h���[�̕`��G���A�̏���
	@param[in]	hWnd			�e�E�C���h�E�̃n���h��
	@param[in]	*pstMeasureItem	�`��ׂ̈̏����o��������ꂽ��
*/
VOID Maa_OnMeasureItem( HWND hWnd, MEASUREITEMSTRUCT *pstMeasureItem )
{
#if 0
	if( IDSO_AAITEMS == pstMeasureItem->CtlID )
	{
		AaItemsMeasureItem( hWnd, pstMeasureItem );
	}
#endif
	return;
}
//-------------------------------------------------------------------------------------------------


/*!
	�܂����ق��`��������
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	xPos	�����������W�w
	@param[in]	yPos	�����������W�x
	@param[in]	zDelta	��]�ʁEWHEEL_DELTA�̔{���E���̒l�͑O(��)�A���̒l�͌��(��O)�։񂳂ꂽ
	@param[in]	fwKeys	������Ă�L�[
	@return		��O�t�H�[�J�X�Ɛ^�����Ⴄ�@�O����
*/
UINT Maa_OnMouseWheel( HWND hWnd, INT xPos, INT yPos, INT zDelta, UINT fwKeys )
{
	HWND	hChdWnd;
	POINT	stPoint;

	stPoint.x = xPos;
	stPoint.y = yPos;
	ScreenToClient( ghMaaWnd, &stPoint );
	hChdWnd = ChildWindowFromPointEx( ghMaaWnd, stPoint, CWP_SKIPINVISIBLE | CWP_SKIPDISABLED | CWP_SKIPTRANSPARENT );

	if( hChdWnd == hWnd )	return 0;

	if( AaItemsIsUnderCursor( ghMaaWnd, hChdWnd , zDelta ) )	return 1;

	TreeFavIsUnderCursor( ghMaaWnd, hChdWnd, xPos, yPos, zDelta, fwKeys );

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�X�e�[�^�X�o�[�ɂȂ񂩕\��
	@param[in]	room	�`�悷��g�O�C���f�b�N�X
	@param[in]	ptMsg	�`�悷�镶����
*/
VOID StatusBarMsgSet( UINT room, LPTSTR ptMsg )
{

	SendMessage( ghStsBarWnd, SB_SETTEXT, room, (LPARAM)ptMsg );

	UpdateWindow( ghStsBarWnd );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�v���b�t�@�C���������MLT�f�B���N�g�����w�肵���蒼���J������
	@param[in]	hWnd	�E�C���h�E�n���h���E�ǂ��̂��낤�H
	@param[in]	ptProf	�J���v���b�t�@�C����
	@return		UINT	���F�r�p�k����\�z�@�O�t�@�C������\�z�Z���@���F�G���[
*/
INT TreeProfileMake( HWND hWnd, LPTSTR ptProf )
{
	OPENFILENAME	stOpenFile;
	BOOLEAN	bOpened;
	UINT	iCount;
	TCHAR	atFilePath[MAX_PATH], atFileName[MAX_STRING], atBuffer[MAX_PATH];
	TCHAR	atFolder[MAX_PATH];
	INT_PTR	iRslt;



	ZeroMemory( atFilePath,  sizeof(atFilePath) );
	ZeroMemory( atFileName,  sizeof(atFileName) );

	if( ptProf )
	{
		StringCchCopy( atFilePath, MAX_PATH, ptProf );
	}
	else	//	�J���w�肪�Ȃ�������A����������w�肷��
	{
		ZeroMemory( &stOpenFile, sizeof(OPENFILENAME) );
		stOpenFile.lStructSize     = sizeof(OPENFILENAME);
		stOpenFile.hwndOwner       = ghMaaWnd;
		stOpenFile.lpstrFilter     = TEXT("�v���t�@�C��(*.qor)\0*.qor\0�S�Ẵt�@�C��(*.*)\0*.*\0\0");
//		stOpenFile.nFilterIndex    = 1;
		stOpenFile.lpstrFile       = atFilePath;
		stOpenFile.nMaxFile        = MAX_PATH;
		stOpenFile.lpstrFileTitle  = atFileName;
		stOpenFile.nMaxFileTitle   = MAX_STRING;
	//	stOpenFile.lpstrInitialDir = 
		stOpenFile.lpstrTitle      = TEXT("�t�@�C�������w�肷�邩�A�쐬�������t�@�C��������͂��Ă�");
		stOpenFile.Flags           = OFN_EXPLORER | OFN_HIDEREADONLY;
		stOpenFile.lpstrDefExt     = TEXT("qor");

		//������ FileSaveDialogue ���o��
		bOpened = GetOpenFileName( &stOpenFile );

		if( !(bOpened) ){	return -1;	}	//	�L�����Z�����Ă��牽�����Ȃ�

		StringCchCopy( gatProfilePath, MAX_PATH, atFilePath );
		InitParamString( INIT_SAVE, VS_PROFILE_NAME, atFilePath );
	}


	TabMultipleStore( hWnd );	//	�����̃f�[�^����O�ɊJ���Ă镛�^�u��ۑ�
	TabMultipleDeleteAll( hWnd );	//	���^�u�S������

	SqlDatabaseOpenClose( M_DESTROY, NULL );	//	�����̃f�[�^�͕���

	//	�v���b�t�@�C�������t���p�X�œn��

	SqlDatabaseOpenClose( M_CREATE, atFilePath );//MAA_TREE_CACHE	TEXT(":memory:")
	//	���ŊJ��������ɍ����

	//	�v���t�@�C�������X�e�[�^�X�o�[�ɕ\�����Ă���
	StringCchCopy( atBuffer, MAX_PATH, atFilePath );
	PathStripPath( atBuffer );
	StatusBarMsgSet( SBMAA_PROFNAME, atBuffer );	//	�v���t�@�C����

	ZeroMemory( atFolder,  sizeof(atFolder) );

	//	�J����PROFILE���L�^
	OpenProfileLogging( hWnd, atFilePath );


	//	MLT�f�B���N�g�����Z�b�g
	ZeroMemory( gatTemplatePath,  sizeof(gatTemplatePath) );
	SqlTreeProfSelect( NULL, 0, gatTemplatePath, MAX_PATH );
	//	�f�B���N�g�����Ȃ�������t�H���_�I�[�|���_�C�����O�Ŏw�肵�čč\�z
	//	�܂ރf�B���N�g���Ɗ܂܂Ȃ��f�B���N�g�����w��o����悤�ɂ���
	//	�f�B���N�g�����L������ACache�����čč\�z����΂���
	if( NULL == gatTemplatePath[0] )
	{
		//�_�C�����O�Ăяo��
		//�_�C�����O�ł́A�f�B���N�g�������̃f�B���N�g�������X�g�A�b�v
		//ON/OFF��Check���āA�č\�z�t���c���[�č\�z
		//OFF�͍̂č\�z���Ȃ�
		//	�c���[�č\�z�Ȃ�A���̃_�C�����O���Ăяo��
		iRslt = DialogBoxParam( ghInst, MAKEINTRESOURCE(IDD_PROFILE_TREESEL_DLG), hWnd, TreeProfileDlgProc, (LPARAM)atFolder );
		if( IDCANCEL == iRslt ){	return -1;	}

		StringCchCopy( gatTemplatePath, MAX_PATH, atFolder );
	}

	//	�f�[�^���邩�H
	iCount = SqlTreeCount( 1, NULL );
	if( 0 >= iCount )	return 0;

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	��Profile�J������������
*/
INT TreeProfileOpen( HWND hWnd )
{
	INT	iRslt;

	iRslt = TreeProfileMake( hWnd, NULL );
	if( 0 > iRslt ){	return 0;	}	//	���e�ύX�Ȃ̂ŁA���Ȃ�i�j�����Ȃ��B

	//	�����ŁA�\�z������Ăׂ΂�낵��
	TreeConstruct( ghMaaWnd, gatTemplatePath, TRUE );
	//�����̊J���Ȃ�P�A�V�K�쐬�Ȃ�O���߂�͂�
	//������ɂ��Ă��A���^�u�č\�z���K�v

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	���݂̃v���t�@�C���̓��e��ҏW����
	@param[in]	hWnd	�n���h��
*/
INT TreeProfileRebuild( HWND hWnd )
{
	TCHAR	atFolder[MAX_PATH];
	INT_PTR	iRslt;

	//	�v���t�@�C�����J���ĂȂ��ꍇ�́A�x�����b�Z�[�W�����ĂȂɂ����Ȃ�

	//	���[�g�f�B���N�g�����A�_�C�����O�ɓn���ăI�[�|��
	StringCchCopy( atFolder, MAX_PATH, gatTemplatePath );	//	���[�g�f�B���N�g�����R�s�[

	iRslt = DialogBoxParam( ghInst, MAKEINTRESOURCE(IDD_PROFILE_TREESEL_DLG), hWnd, TreeProfileDlgProc, (LPARAM)atFolder );
	if( IDCANCEL == iRslt ){	return (-1);	}	//	���e�ύX�Ȃ̂ŁA�L�����Z���Ȃ�i�j�����Ȃ��B
	//	���[�g�f�B���N�g���ύX���Ă���č\�z�J�n
	if( NULL != atFolder[0] )	//	���̎��_�ŋ�͂Ȃ����H
	{
		StringCchCopy( gatTemplatePath, MAX_PATH, atFolder );
		//	����������J�����ꍇ�́A���^�u�č\�z�s�v
		TreeConstruct( ghMaaWnd, gatTemplatePath, FALSE );
	}

	return iRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	�c���[�Ɏ�荞�ރf�B���N�g���[��ݒ肵�č\�z����
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK TreeProfileDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	static BOOLEAN	cbAct;
	static LPTSTR	ptFolder;
	static  HWND	chTvWnd;

	static HTREEITEM	chTreeRoot;
	static TVINSERTSTRUCT	cstRootIns;

	HWND	hWorkWnd;
	TCHAR	atTgtDir[MAX_PATH];
	INT		id;

	DWORD	mPos;
	UINT	bCheck, count;
	INT		idFrom;
	LPNMHDR	pstNmhdr;

	LPNMTREEVIEW	pstNmTrVw;
	TVHITTESTINFO	stTreeHit;
	HTREEITEM		hItem;

	SHFILEINFO		stShFileInfo;

//���̂܂܃��X�g�A�b�v����ƁA���e�ҏW�̂Ƃ��ɒ��g���𒃋�ׂɂȂ�

	switch( message )
	{
		default:	break;

		case WM_INITDIALOG:
			ptFolder = (LPTSTR)lParam;

			cbAct = FALSE;

			hWorkWnd = GetDlgItem( hDlg, IDPB_PRTREE_PROGRESS );
			SendMessage( hWorkWnd, PBM_SETRANGE32, 0, 0xFF );
			SendMessage( hWorkWnd, PBM_SETPOS, 0, 0 );
			SendMessage( hWorkWnd, PBM_SETSTEP, 1, 0 );
			ShowWindow( hWorkWnd, SW_HIDE );

			Edit_SetText( GetDlgItem(hDlg,IDE_PRTREE_DIR), ptFolder );

			chTvWnd = GetDlgItem( hDlg, IDTV_PRTREE_DIR_TVIEW );

			//	�c���[�r���[�\�z
			TreeView_DeleteAllItems( chTvWnd );
			//	���[�g�A�C�e�����
			ZeroMemory( &cstRootIns, sizeof(TVINSERTSTRUCT) );
			cstRootIns.hParent      = TVI_ROOT;
			cstRootIns.hInsertAfter = TVI_SORT;
			cstRootIns.item.mask    = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			cstRootIns.item.pszText = TEXT("ROOT");
			cstRootIns.item.lParam  = 0;

			SHGetFileInfo( TEXT(""), 0, &stShFileInfo, sizeof(SHFILEINFO), (SHGFI_SYSICONINDEX|SHGFI_SMALLICON) );
			cstRootIns.item.iImage = stShFileInfo.iIcon;
			SHGetFileInfo( TEXT(""), 0, &stShFileInfo, sizeof(SHFILEINFO), (SHGFI_SYSICONINDEX|SHGFI_SMALLICON|SHGFI_OPENICON) );
			cstRootIns.item.iSelectedImage = stShFileInfo.iIcon;

			chTreeRoot = TreeView_InsertItem( chTvWnd, &cstRootIns );

			SqlTreeCacheOpenClose( M_CREATE );

			//	�J�����Ƃ��A���[�g�t�H���_�w�肪�L���Ȃ�A�������X�g�A�b�v���āA�f�B���N�g�����m�F���ă`�F�b�N�t����
			//if( NULL != ptFolder[0] )
			//{
			//	TreeProfListUp( hDlg, chTvWnd, ptFolder, chTreeRoot, 0, 1 );
			//	TreeView_Expand( chTvWnd, chTreeRoot, TVE_EXPAND );
			//}
			//�J�����Ƃ��ɂ��Əd��
			return (INT_PTR)TRUE;


		case WM_COMMAND:
			id = LOWORD(wParam);
			switch( id )
			{
				case  IDB_PRTREE_DIR_REF:	//	�f�B���N�g���Q��
					if( SelectDirectoryDlg( hDlg, atTgtDir, MAX_PATH ) )
					{
						Edit_SetText( GetDlgItem(hDlg,IDE_PRTREE_DIR), atTgtDir );
					}
					return (INT_PTR)TRUE;

				case  IDB_PRTREE_LISTUP:	//	���X�g�A�b�|�J�n
					Edit_GetText( GetDlgItem(hDlg,IDE_PRTREE_DIR), atTgtDir, MAX_PATH );
					if( NULL != atTgtDir[0] )
					{
						TreeView_DeleteAllItems( chTvWnd  );	//	��U�S�j�󂵂ă��[�g��蒼��
						SqlTreeNodeAllDelete( 0 );	//	�L���b�V�����j��
						chTreeRoot = TreeView_InsertItem( chTvWnd, &cstRootIns );	//	���[�g�쐬
#ifndef TREEPROF_AUTOCHECK
						TreeView_SetCheckState( chTvWnd , chTreeRoot, TRUE );	//	�`�F�L�}�[�N�H
#endif
						UpdateWindow( chTvWnd );

						StringCchCopy( ptFolder, MAX_PATH, atTgtDir );	//	�������L����������

						hWorkWnd = GetDlgItem( hDlg, IDPB_PRTREE_PROGRESS );
						SendMessage( hWorkWnd, PBM_SETPOS, 0, 0 );
						ShowWindow( hWorkWnd, SW_SHOW );
						TreeProfListUp( hDlg, chTvWnd, atTgtDir, chTreeRoot, 0, 1 );
						TreeView_Expand( chTvWnd, chTreeRoot, TVE_EXPAND );
#ifdef TREEPROF_AUTOCHECK
						//	���̃��[�g�ƁAPROFILE�̃��[�g���m�F���āA�����Ȃ�A�`�F�b�N��t���Ă���
						TreeProfCheckExistent( hDlg, atTgtDir, chTvWnd, chTreeRoot, 0 );
#endif
						ShowWindow( hWorkWnd, SW_HIDE );
					}
					return (INT_PTR)TRUE;

				case IDOK:
					count = TreeView_GetCount( chTvWnd );
					if( 1 >= count )
					{
						MessageBox( hDlg, TEXT("���X�g�A�b�v�o���Ă��Ȃ��݂����B\r\n���̂܂܂���c���[�����Ȃ���B"), TEXT("���ӂ���̂��m�点"), MB_OK | MB_ICONERROR );
						return (INT_PTR)TRUE;
					}

					//	�f�B���N�g���I���ɍ��킹��SQL���Œ�
					if( SUCCEEDED( TreeLoadDirCheck( hDlg, chTvWnd ) ) )
					{
						SqlTreeCacheOpenClose( M_DESTROY );
						EndDialog(hDlg, IDOK );
					}
					return (INT_PTR)TRUE;

				case IDCANCEL:
					SqlTreeCacheOpenClose( M_DESTROY );
					EndDialog(hDlg, IDCANCEL );
					return (INT_PTR)TRUE;
			}
			break;


		case WM_NOTIFY:
			idFrom   = (INT)wParam;
			pstNmhdr = (LPNMHDR)lParam;
			if( IDTV_PRTREE_DIR_TVIEW == idFrom )
			{
				pstNmTrVw = (LPNMTREEVIEW)pstNmhdr;
				if( NM_CLICK == pstNmhdr->code )
				{
					ZeroMemory( &stTreeHit, sizeof(TVHITTESTINFO) );
					mPos = GetMessagePos( );
					stTreeHit.pt.x = GET_X_LPARAM( mPos );
					stTreeHit.pt.y = GET_Y_LPARAM( mPos );
					MapWindowPoints( HWND_DESKTOP, chTvWnd, &stTreeHit.pt, 1 );
					TreeView_HitTest( chTvWnd, &stTreeHit );

					//	�`�F�b�N������O�ɗ���
					if ( TVHT_ONITEMSTATEICON & stTreeHit.flags )
					{
						//	�`�F�b�N�����鎞���`�F�b�N�������Ă��Ȃ����
						bCheck = TreeView_GetCheckState( chTvWnd, stTreeHit.hItem );

						//	��ɉ��������X�L���@�`�����Ȃ��Ƃ��������Ȃ�悤��
						hItem = TreeView_GetChild( chTvWnd, stTreeHit.hItem );
						TreeProfCheckState( chTvWnd, hItem, bCheck );

						//	������ɂ́A�`�F�b�N��������A���n�m�ɂ���
						if( !(bCheck)  )	//	�`�F�L����Ƃ�
						{
							hItem = TreeView_GetParent( chTvWnd, stTreeHit.hItem );
							while( hItem  )
							{
								TreeView_SetCheckState( chTvWnd, hItem, TRUE );
								hItem = TreeView_GetParent( chTvWnd, hItem );
							}
						}
					}

					SetWindowLong( hDlg, DWL_MSGRESULT, 0 );
					return (INT_PTR)TRUE;
				}

			}
			break;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Ă΂��x�ɁA�v���O���X�o�[�𑝂₷
	@param[in]	hDlg	�_�C�����O�n���h��
*/
VOID TreeProfProgressUp( HWND hDlg )
{
	HWND	hProgWnd;
	UINT	pos;

	hProgWnd = GetDlgItem( hDlg, IDPB_PRTREE_PROGRESS );

	pos = SendMessage( hProgWnd, PBM_GETPOS, 0, 0 );
	pos++;
	pos &= 0xFF;
	SendMessage( hProgWnd, PBM_SETPOS, pos, 0 );
	UpdateWindow( hProgWnd );

	return;
}
//-------------------------------------------------------------------------------------------------

#ifdef TREEPROF_AUTOCHECK
/*!
	�����̃v���t�̓��e�`�F�L���Č�����E�ċA
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	ptTgDir	�I�����Ă�f�B���N�g���E�v���t�̂ƈႤ�Ȃ牽�����Ȃ��ENULL�Ȃ�m�F���Ȃ�
	@param[in]	hTvWnd	�c���[�r���[�n���h��
	@param[in]	hNode	�m�F����m�[�h
	@param[in]	sqlID	�e�m�[�h��sqlID�E���[�g�͂O
	@return	�`�F�b�N������
*/
UINT TreeProfCheckExistent( HWND hDlg, LPTSTR ptTgDir, HWND hTvWnd, HTREEITEM hNode, UINT sqlID )
{
	UINT		checked = 0, tgtID;
	TCHAR		atProfRoot[MAX_PATH];
	TCHAR		atName[MAX_PATH];
	HTREEITEM	hItem, hRoot;
	TVITEM		stItem;

	if( ptTgDir )	//	�������L���Ȃ̂̓��[�g�̎��̂�
	{
		ZeroMemory( atProfRoot, sizeof(atProfRoot) );
		SqlTreeProfSelect( NULL, 0, atProfRoot, MAX_PATH );
		//	�قȂ�Ȃ牽�����Ȃ�
		if( StrCmp( atProfRoot, ptTgDir ) )	return 0;

		//���[�g�Ȃ̂ŁAChild����n�߂�
		hRoot = hNode;
		hItem = TreeView_GetChild( hTvWnd , hNode );	//	�q�m�[�h���m�F
		hNode = hItem;
		if( !(hNode) )	return 0;
		sqlID = 0;	//	��薳���͂�
	}

	do{
		//	���m�[�h�̖��̊m�F���āA�eID�ƈꏏ�Ƀf�[�^�Ђ��ς�
		ZeroMemory( &stItem, sizeof(TVITEM) );
		stItem.mask       = TVIF_HANDLE | TVIF_TEXT;
		stItem.hItem      = hNode;
		stItem.pszText    = atName;
		stItem.cchTextMax = MAX_PATH;
		TreeView_GetItem( hTvWnd, &stItem );
		tgtID = SqlTreeFileGetOnParent( atName, sqlID );
		//	�����łh�c�L���ł���΁A�v���t�Ɋ܂܂�Ă���
		if( tgtID )
		{
			checked++;
			TreeView_SetCheckState( hTvWnd, hNode, TRUE );
			//	�f�B���N�g���̃`�F�b�N���L���łȂ���΁A���̉��͉�������
			hItem = TreeView_GetChild( hTvWnd , hNode );	//	�q�m�[�h���m�F
			//	���݂��Ă��牺�̊K�w���`�F�b�N
			if( hItem ){	checked += TreeProfCheckExistent( hDlg, NULL , hTvWnd, hItem, tgtID );	}
		}

		//	�I������玟�ɂ���
		hItem = TreeView_GetNextSibling( hTvWnd, hNode );
		hNode = hItem;

		TreeProfProgressUp( hDlg );
	}
	while( hNode );

	if( ptTgDir )	//	�������L���Ȃ̂̓��[�g�̎��̂�
	{
		if( checked ){	TreeView_SetCheckState( hTvWnd, hRoot, TRUE );	}
	}

	return checked;
}
//-------------------------------------------------------------------------------------------------
#endif
/*!
	�ċA�ŁA�ȉ��̃c���[�̃`���b�N��ON/OFF����
	@param[in]	hTvWnd	�c���[�r���[�n���h��
	@param[in]	hNode	�m�F����m�[�h
	@param[in]	bCheck	ON/OFF�̃Z�b�g
*/
VOID TreeProfCheckState( HWND hTvWnd, HTREEITEM hNode, UINT bCheck )
{
	HTREEITEM	hItem;

	if( !(hNode) )	return;

	do
	{
		TreeView_SetCheckState( hTvWnd, hNode, bCheck ? FALSE : TRUE );

		hItem = TreeView_GetChild( hTvWnd , hNode );	//	�q�m�[�h���m�F

		//	���݂��Ă��牺�̊K�w���`�F�b�N
		if( hItem ){	TreeProfCheckState( hTvWnd, hItem, bCheck );	}

		//	�I������玟�ɂ���
		hItem = TreeView_GetNextSibling( hTvWnd, hNode );
		hNode = hItem;
	}
	while( hNode );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	���X�g�A�b�v����
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	hTvWnd	�c���[�r���[�n���h��
	@param[in]	ptRoot	MLT���[�g�f�B���N�g��
	@param[in]	hTreePr	�Ώۃf�B���N�g���̃c���[�A�C�e���E�����ɂԂ牺���Ă���
	@param[in]	dPrntID	SQL��ID�E�f�B���N�g���ԍ�
	@param[in]	fCheck	�P�S�`���b�N����@�O�r�p�k�Ɋ����Ȃ�`�F�L�@�|�P�`���b�N���Ȃ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT TreeProfListUp( HWND hDlg, HWND hTvWnd, LPTSTR ptRoot, HTREEITEM hTreePr, UINT dPrntID, INT fCheck )
{
	HANDLE	hFind;
	TCHAR	atPath[MAX_PATH], atNewTop[MAX_PATH], atTarget[MAX_PATH];
	BOOL	bRslt;
	UINT	dPnID = 0;

	WIN32_FIND_DATA	stFindData;

	HTREEITEM	hNewParent, hLastDir = TVI_FIRST;
	TVINSERTSTRUCT	stTreeIns;
	SHFILEINFO	stShFileInfo;

	ZeroMemory( &stTreeIns, sizeof(TVINSERTSTRUCT) );
	stTreeIns.hParent      = hTreePr;
	stTreeIns.hInsertAfter = TVI_LAST;
	stTreeIns.item.mask    = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	ZeroMemory( atTarget, sizeof(atTarget) );
	StringCchCopy( atTarget, MAX_PATH, ptRoot );
	PathAppend( atTarget, TEXT("*") );

	hFind = FindFirstFile( atTarget, &stFindData );	//	TEXT("*")
	do{
		if( lstrcmp( stFindData.cFileName, TEXT("..") ) && lstrcmp( stFindData.cFileName, TEXT(".") ) )
		{
			TreeProfProgressUp( hDlg );

			StringCchCopy( atPath, MAX_PATH, ptRoot );
			PathAppend( atPath, stFindData.cFileName );

			SHGetFileInfo( atPath, 0, &stShFileInfo, sizeof(SHFILEINFO), (SHGFI_SYSICONINDEX|SHGFI_SMALLICON) );
			stTreeIns.item.iImage = stShFileInfo.iIcon;
			SHGetFileInfo( atPath, 0, &stShFileInfo, sizeof(SHFILEINFO), (SHGFI_SYSICONINDEX|SHGFI_SMALLICON|SHGFI_OPENICON) );
			stTreeIns.item.iSelectedImage = stShFileInfo.iIcon;

			stTreeIns.item.pszText = stFindData.cFileName;

			if( stFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{	//	�f�B���N�g���̏ꍇ
				dPnID = SqlTreeCacheInsert( FILE_ATTRIBUTE_DIRECTORY, dPrntID, stFindData.cFileName );

				stTreeIns.item.lParam  = dPnID;	//	�ʂ��ԍ��ł��邱�Ƃɒ���
				stTreeIns.hInsertAfter = hLastDir;
				hNewParent = TreeView_InsertItem( hTvWnd, &stTreeIns );
				hLastDir = hNewParent;

#ifndef TREEPROF_AUTOCHECK
				TreeView_SetCheckState( hTvWnd , hNewParent, TRUE );	//	�`�F�L�}�[�N�H
#endif
				StringCchCopy( atNewTop, MAX_PATH, ptRoot );
				PathAppend( atNewTop, stFindData.cFileName );

				TreeProfListUp( hDlg, hTvWnd, atNewTop, hNewParent, dPnID, fCheck );	//	�Y���f�B���N�g�������ċA����

			}
			else
			{	//	�t�@�C���̏ꍇ
				bRslt  = PathMatchSpec( stFindData.cFileName, TEXT("*.mlt") );	//	�q�b�g������TRUE
				bRslt |= PathMatchSpec( stFindData.cFileName, TEXT("*.ast") );	//	�q�b�g������TRUE
				bRslt |= PathMatchSpec( stFindData.cFileName, TEXT("*.txt") );	//	�q�b�g������TRUE
				if( bRslt )	//	20110720	AST��ǉ�	20120223	TXT���ǉ�
				{
					dPnID = SqlTreeCacheInsert( FILE_ATTRIBUTE_NORMAL, dPrntID, stFindData.cFileName );

					stTreeIns.item.lParam  = dPnID;	//	�ʂ��ԍ��ł��邱�Ƃɒ���
					stTreeIns.hInsertAfter = TVI_LAST;
					hNewParent = TreeView_InsertItem( hTvWnd, &stTreeIns );

#ifndef TREEPROF_AUTOCHECK
					TreeView_SetCheckState( hTvWnd , hNewParent, TRUE );	//	�`�F�L�}�[�N�H
#endif
				}
			}
		}

	}while( FindNextFile( hFind, &stFindData ) );

	FindClose( hFind );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�c���[�v���t�@�C���쐬�ŁA�`�F�b�N����Ă�ł�ꂭ�Ƃ�Ɵ����̃t�@�C�����L�^
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	hTvWnd	�c���[�r���[�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT TreeLoadDirCheck( HWND hDlg, HWND hTvWnd )
{
	TCHAR	atTgtDir[MAX_PATH];
	HTREEITEM	hTreeRoot, hItem;
	UINT	dCacheMax, dCacheCnt, m, count;
	UINT	dType, dPrnt, index, logoa;
	TCHAR	atName[MAX_PATH];
	HWND	hWorkWnd;

	hTreeRoot = TreeView_GetRoot( hTvWnd  );	//	�Ƃ�₦�����[�g�m��
	//	���[�g�͊֌W�Ȃ��̂ŁA�����𒲂ׂ�
	hItem = TreeView_GetChild( hTvWnd, hTreeRoot );	//	�q�m�[�h���m�F


	hWorkWnd = GetDlgItem( hDlg, IDPB_PRTREE_PROGRESS );
	SendMessage( hWorkWnd, PBM_SETPOS, 0, 0 );
	ShowWindow( hWorkWnd, SW_SHOW );

	//	�`�F�b�N�󋵂��m�F�E�t�@�C���̃`�F�b�N�������Ȃ烄�o�C
	count = TreeLoadNodeProc( hDlg, hTvWnd, hItem, 0 );
	TRACE( TEXT("%u"), count );
	if( 0 == count )
	{
		ShowWindow( hWorkWnd, SW_HIDE );
		MessageBox( hDlg, TEXT("�t�@�C��������I������ĂȂ���B\r\n���̂܂܂��Ǝg���Ȃ���B"), TEXT("���ӂ���̂��m�点"), MB_OK | MB_ICONERROR );
		return E_ABORT;
	}

	TreeLoadNodeProc( hDlg, hTvWnd, hItem, 1 );

	Edit_GetText( GetDlgItem(hDlg,IDE_PRTREE_DIR), atTgtDir, MAX_PATH );

	//	�c���[�f�[�^�̓���ւ�
	SqlTransactionOnOff( TRUE );	//	�g�����U�N�V�����J�n

	SqlTreeProfUpdate( NULL, atTgtDir );	//	���[�g�p�X��ύX
	SqlTreeNodeAllDelete( 1 );	//	�t�@�C������\�z����ꍇ�A�{��SQL�̒��g����ɂ��Ă���

	dCacheCnt = SqlTreeCount( 3, &dCacheMax );
	index = 0;
	for( m = 0; dCacheMax > m; m++ )
	{
		ZeroMemory( atName, sizeof(atName) );
		index = SqlTreeNodePickUpID( index, &dType, &dPrnt, atName, 0x00 );
		TRACE( TEXT("[%4u]%4u\t%4u\t%4u\t%s"), m, index, dType, dPrnt, atName );
		if( 0 >= index )	break;	//	�f�[�^�����Ȃ�����I���

		logoa = SqlTreeNodeInsert( index, dType, dPrnt, atName );

		TreeProfProgressUp( hDlg );
	}

	ShowWindow( hWorkWnd, SW_HIDE );

	SqlTransactionOnOff( FALSE );	//	�g�����U�N�V�����I��

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ċA�����ŁA�c���[�̃`�F�b�N�󋵂𒲂ׂăL���b�V���𑀍�
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	hTvWnd	�c���[�n���h��
	@param[in]	hNode	�`�F�L���_�m�[�h
	@param[in]	bFixe	��O���ۂɑ���@�OCheck�󋵂̊m�F
*/
UINT TreeLoadNodeProc( HWND hDlg, HWND hTvWnd, HTREEITEM hNode, UINT bFixe )
{
	TCHAR		atName[MAX_PATH];
	INT			param;
	UINT		dRslt, dType, dPrID, count = 0;
	HTREEITEM	hItem;
	TVITEM		stItem;

	do
	{
		//	�Y������A�C�e���h�c������������
		ZeroMemory( &stItem, sizeof(TVITEM) );
		stItem.mask  = TVIF_HANDLE | TVIF_PARAM | TVIF_TEXT;
		stItem.hItem      = hNode;
		stItem.pszText    = atName;
		stItem.cchTextMax = MAX_PATH;
		TreeView_GetItem( hTvWnd, &stItem );
		param = stItem.lParam;

		//	�`�F�b�N�󋵂��m�F
		dRslt = TreeView_GetCheckState( hTvWnd, hNode );
		TRACE( TEXT("Node:[%d][%d]%s"), dRslt, param, atName );

		if( bFixe )
		{
			//	�`�F�b�N������������
			if( !(dRslt)  ){	SqlTreeCacheDelID( param  );	}
		}
		else
		{	//	���͒l�𒴂����l�Ȃ̂ŁA�ڕW���Q�b�g����ɂ́|�P����
			SqlTreeNodePickUpID( param-1, &dType, &dPrID, atName, 0x00 );
			//	�`�F�b�N������A�t�@�C���ł���ꍇ
			if( dRslt && (FILE_ATTRIBUTE_NORMAL==dType) ){	count++;	}
		}

		hItem = TreeView_GetChild( hTvWnd , hNode );	//	�q�m�[�h���m�F

		//	���݂��Ă��牺�̊K�w���`�F�b�N
		if( hItem ){	count += TreeLoadNodeProc( hDlg, hTvWnd, hItem, bFixe );	}

		//	�I������玟�ɂ���
		hItem = TreeView_GetNextSibling( hTvWnd, hNode );
		hNode = hItem;

		TreeProfProgressUp( hDlg );
	}
	while( hNode );

	return count;
}
//-------------------------------------------------------------------------------------------------

/*!
	�p�X���󂯎���āA�擪�̂ł�ꂭ�Ƃ���R�s�[����
	@param[in]	ptSource	���p�X�EMAX_PATH�ł��邱��
	@param[in]	ptSplits	�擪�p�X���R�s�[����o�b�t�@�EMAX_PATH�ł��邱��
	@return		���p�X�́A���̃f�B���N�g���ʒu
*/
LPTSTR PathSplitFirstPath( LPTSTR ptSource, LPTSTR ptSplits )
{
	UINT	d;

	if( !(ptSource) )	return NULL;
	if( NULL ==  ptSource[0] )	return NULL;

	for( d = 0; MAX_PATH > d; d++ )
	{
		if( TEXT('\\') == ptSource[d] ){	ptSplits[d] =  NULL;	d++;	break;	}
		else if( NULL ==  ptSource[d] ){	ptSplits[d] =  NULL;	break;	}
		else{	ptSplits[d] = ptSource[d];	}
	}

	return &(ptSource[d]);
}
//-------------------------------------------------------------------------------------------------

/*!
	������̎��̉��s�܂ł����
	@param[in]	ptSource	��������
	@param[in]	*ptNextLn	���̍s�̐擪
	@return		�m�ۂ���������Efree�Z��
*/
LPTSTR StringLineGet( LPCTSTR ptSource, LPCTSTR *ptNextLn )
{
	UINT		t = 0;
	UINT_PTR	len;
	wstring		wStr;
	LPTSTR		ptDest;

	while( ptSource[t] )
	{
		if( TEXT('\r') == ptSource[t] || TEXT('\n') == ptSource[t] )	break;

		wStr += ptSource[t];
		t++;
	}

	if( TEXT('\r') == ptSource[t] || TEXT('\n') == ptSource[t] )	t++;
	if( TEXT('\r') == ptSource[t] || TEXT('\n') == ptSource[t] )	t++;
	*ptNextLn = &(ptSource[t]);

	len = wStr.size() + 1;
	ptDest = (LPTSTR)malloc( len * sizeof(TCHAR) );
	ZeroMemory( ptDest, len * sizeof(TCHAR) );
	StringCchCopy( ptDest, len, wStr.c_str() );

	return ptDest;
}
//-------------------------------------------------------------------------------------------------

/*
�c���[������T�[�`
�����e�ԍ������ғ��ŃT�[�`�E�����f�B���N�g�����ɂ́A�P�ꖼ��������Ȃ�����A
���ꂪ�������m���Ɣ��f�ł���͂�

*/

/*!
	�������ă��X�g�r���[�ɓ����
	@param[in]	hDlg	�_�C�����O�n���h��
*/
HRESULT MaaFindExecute( HWND hDlg )
{
	UINT	dCnt, dMax, d;
	UINT	dItem, dType, dPrntID, dOwnID;
	UINT	dDmyType, dDmyID;
	TCHAR	atPattern[MAX_PATH];
	TCHAR	atFileName[MAX_PATH], atPrntName[MAX_PATH];
	HWND	hLvWnd, hEdWnd;
	LVITEM	stLvi;

	hEdWnd = GetDlgItem( hDlg, IDE_MAA_FIND_NAME );
	hLvWnd = GetDlgItem( hDlg, IDLV_MAA_FINDED_FILE );

	ListView_DeleteAllItems( hLvWnd );

	ZeroMemory( atPattern, sizeof(atPattern) );
	GetDlgItemText( hDlg, IDE_MAA_FIND_NAME, atPattern, MAX_PATH );
	//	�󕶎���Ȃ猟�����Ȃ�
	if( NULL == atPattern[0] )	return  E_ABORT;


	dCnt = SqlTreeCount( 1, &dMax );

	dOwnID = 0;
	for( d = 0; dMax > d; d++ )
	{
		dOwnID = SqlTreeFileSearch( atPattern, dOwnID );	//	�q�b�g���m�F
		if( 0 == dOwnID )	break;	//	����ȏ㖳���悤�Ȃ�I���

		ZeroMemory( atFileName, sizeof(atFileName) );
		dType   = 0;
		dPrntID = 0;

		//	�Y���h�c�̓��e���m�F
		SqlTreeNodePickUpID( dOwnID, &dType, &dPrntID, atFileName, 0x11 );
		if( FILE_ATTRIBUTE_NORMAL == dType )
		{
			//	�������������e�t�@�C���������X�g�r���[�ɕ\��
			dItem = ListView_GetItemCount( hLvWnd );

			ZeroMemory( &stLvi, sizeof(stLvi) );
			stLvi.iItem = dItem;

			stLvi.mask     = LVIF_TEXT | LVIF_PARAM;
			stLvi.pszText  = atFileName;
			stLvi.lParam   = dOwnID;
			stLvi.iSubItem = 0;
			ListView_InsertItem( hLvWnd, &stLvi );

			SqlTreeNodePickUpID( dPrntID, &dDmyType, &dDmyID, atPrntName, 0x11 );

			stLvi.mask     = LVIF_TEXT;
			stLvi.pszText  = atPrntName;
			stLvi.iSubItem = 1;
			ListView_SetItem( hLvWnd, &stLvi );
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


/*!
	MAA�����_�C�����O�̃m�[�e�B�t�@�C���b�Z�[�W�̏���
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	idFrom		NOTIFY�𔭐��������R���g���[���̂h�c
	@param[in]	pstNmhdr	NOTIFY�̏ڍ�
	@return		�����������e�Ƃ�
*/
INT_PTR MaaFindOnNotify( HWND hDlg, INT idFrom, LPNMHDR pstNmhdr )
{
	HWND	hLvWnd;
	INT		iItem, nmCode;
	LPNMLISTVIEW	pstNmLv;
	LVITEM			stLvi;
	HTREEITEM		hTgtItem;

	if( IDLV_MAA_FINDED_FILE == idFrom )
	{
		pstNmLv = (LPNMLISTVIEW)pstNmhdr;

		hLvWnd = pstNmLv->hdr.hwndFrom;
		nmCode = pstNmLv->hdr.code;

		//	�I������Ă鍀�ڂ��m��
		iItem = ListView_GetNextItem( hLvWnd, -1, LVNI_ALL | LVNI_SELECTED );

		if( 0 >  iItem )	return FALSE;	//	���I����ԂȂ牽�����Ȃ�

		//	�_�u���N���b�N�ł������ꍇ
		if( NM_DBLCLK == nmCode )
		{
			ZeroMemory( &stLvi, sizeof(stLvi) );
			stLvi.mask     = LVIF_PARAM;
			stLvi.iItem    = iItem;
			stLvi.iSubItem = 0;
			ListView_GetItem( hLvWnd, &stLvi );

			hTgtItem = MaaSelectIDfile( hDlg, stLvi.lParam );	//	SqlID�n���ĊJ���悤�ɂ���
			//	�c���[�̂�I����Ԃɂ��Ă���

			if( hTgtItem )
			{
				SetForegroundWindow( ghMaaWnd );

				//	�����ŁA�^�u�I������`�F�C��������΂���
				TabMultipleCtrlFromFind( ghMaaWnd );
				//AaTitleClear(  );	//	�����ŃN�����[���Ȃ�����
				//TreeSelItemProc( ghMaaWnd, hTgtItem, 0 );	//	�n���n���h���AMAA���̃n���h���ɂ��Ă����Ȃ��Ƃ܂����H
			}
		}
	}

	return TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�����_�C�����O�̃v���V�[�W��
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK TreeMaaFindDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	HWND	hWorkWnd;
	UINT	id;
//	HWND	hWndChild;
	LVCOLUMN	stLvColm;


	switch( message )
	{
		default:	break;

		case WM_INITDIALOG:
			hWorkWnd = GetDlgItem( hDlg, IDLV_MAA_FINDED_FILE );
			ListView_SetExtendedListViewStyle( hWorkWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
			ZeroMemory( &stLvColm, sizeof(LVCOLUMN) );
			stLvColm.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			stLvColm.fmt = LVCFMT_LEFT;
			stLvColm.pszText = TEXT("�t�@�C����");	stLvColm.cx = 250;	stLvColm.iSubItem = 0;	ListView_InsertColumn( hWorkWnd, 0, &stLvColm );
			stLvColm.pszText = TEXT("����");		stLvColm.cx = 250;	stLvColm.iSubItem = 1;	ListView_InsertColumn( hWorkWnd, 1, &stLvColm );
			SetFocus( GetDlgItem(hDlg,IDE_MAA_FIND_NAME) );
			return (INT_PTR)FALSE;

		case WM_COMMAND:
			id = LOWORD(wParam);
			hWorkWnd = GetDlgItem( hDlg, IDE_FIND_TEXT );
			switch( id )
			{
				case IDCANCEL:	DestroyWindow( hDlg );	ghMaaFindDlg = NULL;	return (INT_PTR)TRUE;

				case IDOK:		MaaFindExecute( hDlg );	return (INT_PTR)TRUE;	//	��������

				case IDM_PASTE:	SendMessage( hWorkWnd, WM_PASTE, 0, 0 );	return (INT_PTR)TRUE;
				case IDM_COPY:	SendMessage( hWorkWnd, WM_COPY,  0, 0 );	return (INT_PTR)TRUE;
				case IDM_CUT:	SendMessage( hWorkWnd, WM_CUT,   0, 0 );	return (INT_PTR)TRUE;
				case IDM_UNDO:	SendMessage( hWorkWnd, WM_UNDO,  0, 0 );	return (INT_PTR)TRUE;

				default:	break;
			}
			break;

		case WM_CLOSE:		DestroyWindow( hDlg );	ghMaaFindDlg = NULL;	return (INT_PTR)TRUE;

		case WM_DESTROY:	ghMaaFindDlg = NULL;	return (INT_PTR)TRUE;

		case WM_NOTIFY:		MaaFindOnNotify( hDlg, (INT)(wParam), (LPNMHDR)(lParam) );	return (INT_PTR)TRUE;

	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------�@��...

/*!
	MAA�t�@�C���������̏���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT TreeMaaFileFind( HWND hWnd )
{
	HINSTANCE	hInst;

	hInst = GetModuleHandle( NULL );


	if( ghMaaFindDlg )
	{
		SetForegroundWindow( ghMaaFindDlg );
		return S_OK;
	}

	ghMaaFindDlg = CreateDialogParam( hInst, MAKEINTRESOURCE(IDD_FIND_MAA_DLG), hWnd, TreeMaaFindDlgProc, 0 );

	ShowWindow( ghMaaFindDlg, SW_SHOW );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------





/*!
	�t�@�C������v���t������荞�񂾂菑�����񂾂�
	@param[in]	hWnd	�E�C���h�E�n���h���ENULL�Ȃ�f�X�g���C
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT OpenProfileInitialise( HWND hWnd )
{
	TCHAR	atString[MAX_PATH+10];
	UINT	d;
	UINT_PTR	dItems;
	OPENHIST	stProfHist;
	OPHIS_ITR	itHist;

	if( hWnd )	//	���[�h
	{
		gltProfHist.clear( );	//	�Ƃ肠�����S�폜

		for( d = 0; OPENHIST_MAX > d; d++ )
		{
			ZeroMemory( &stProfHist, sizeof(OPENHIST) );

			if( SUCCEEDED( InitProfHistory( INIT_LOAD, d, stProfHist.atFile ) ) )
			{
				gltProfHist.push_back( stProfHist );
			}
			else{	break;	}	//	���g����������L�^�E����������I���
		}

		if( ghProfHisMenu ){	DestroyMenu( ghProfHisMenu );	}
		//	���j���[�쐬
		ghProfHisMenu = CreatePopupMenu(  );
		AppendMenu( ghProfHisMenu, MF_SEPARATOR, 0 , NULL );	//	�Z�b�p���^�[
		AppendMenu( ghProfHisMenu, MF_STRING, IDM_OPEN_HIS_CLEAR, TEXT("�����N����") );
		//	�R�}���h�h�c���t�@�C���Ɠ����ł��邱�Ƃɒ��ӁE���͕ʂ������薳��

		dItems = gltProfHist.size( );
		if( 0 == dItems )
		{
			//	�I�[�|�������������ꍇ
			InsertMenu( ghProfHisMenu, 0, MF_STRING | MF_BYPOSITION | MF_GRAYED, IDM_OPEN_HIS_FIRST, TEXT("(��)") );
		}
		else
		{
			//	�I�[�|����������ׂ�
			for( itHist = gltProfHist.begin(), d = dItems-1; gltProfHist.end() != itHist; itHist++, d-- )
			{
				StringCchPrintf( atString, MAX_PATH+10, TEXT("(&%X) %s"), d, itHist->atFile );
				InsertMenu( ghProfHisMenu, 0, MF_STRING | MF_BYPOSITION, (IDM_OPEN_HIS_FIRST + d), atString );
				itHist->dMenuNumber = (IDM_OPEN_HIS_FIRST + d);
			}
		}
		
		//	�R���e�L�X�g���j���[�͕K�v�ɉ����ă��[�h����̂ŁA�����ł̓C�W��Ȃ�
#ifdef _ORRVW
		OpenProfMenuModify( hWnd );
#endif
	}
	else	//	�I����
	{
		if( ghProfHisMenu ){	DestroyMenu( ghProfHisMenu );	}

		InitProfHistory( INIT_SAVE, 0, NULL );	//	��U�S�폜

		//	���g��ۑ�
		for( itHist = gltProfHist.begin(), d = 0; gltProfHist.end() != itHist; itHist++, d++ )
		{
			InitProfHistory( INIT_SAVE, d, itHist->atFile );
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


/*!
	�J�����v���t������ԍ��w�肵�ēǂݍ���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	id		�����w�胁�b�Z�[�W�E���j���[�h�c�ł���
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT OpenProfileLoad( HWND hWnd, INT id )
{
	UINT_PTR	dNumber, dItems;
	TCHAR		atFilePath[MAX_PATH];
	OPHIS_ITR	itHist;

	WIN32_FIND_DATA	stFindData;
	HANDLE	hFind;

	dNumber = id - IDM_OPEN_HIS_FIRST;

	TRACE( TEXT("�v���t -> %d"), dNumber );
	if( OPENHIST_MAX <= dNumber ){	return E_OUTOFMEMORY;	}

	dItems = gltProfHist.size();
	dNumber = (dItems-1) - dNumber;

	itHist = gltProfHist.begin();
	std::advance( itHist , dNumber );	//	�����i�߂�

	//	�I�������v���t���J��
	StringCchCopy( atFilePath, MAX_PATH, itHist->atFile );

	//	���̃t�@�C���͑��݂��邩�H
	hFind = FindFirstFile( atFilePath, &stFindData );	//	TEXT("*")
	if( INVALID_HANDLE_VALUE != hFind ){	FindClose( hFind  );	}
	else{	ZeroMemory( atFilePath, sizeof(atFilePath) );	};

	if( NULL != atFilePath[0]  )	//	������Ή������Ȃ�
	{
		StringCchCopy( gatProfilePath, MAX_PATH, atFilePath );
		TreeProfileMake( ghMaaWnd, gatProfilePath );

		InitParamString( INIT_SAVE, VS_PROFILE_NAME, gatProfilePath );

		TreeConstruct( ghMaaWnd, gatTemplatePath, TRUE );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


/*!
	�J����PROFILE���L�^
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	ptProf	�J�����v���t�t�@�C����
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT OpenProfileLogging( HWND hWnd, LPCTSTR ptProf )
{
	UINT_PTR	dItems;
	OPENHIST	stProfHist;
	OPHIS_ITR	itHist;

	if( ptProf )
	{
		ZeroMemory( &stProfHist, sizeof(OPENHIST) );

		StringCchCopy( stProfHist.atFile, MAX_PATH, ptProf );
		//�����̓��e�Ȃ�ŐV�ɓ���ւ���̂ŁA�������Ă���
		for( itHist = gltProfHist.begin(); gltProfHist.end() != itHist; itHist++ )
		{
			if( !StrCmp( itHist->atFile, stProfHist.atFile ) )	//	�������̂���������폜����
			{
				gltProfHist.erase( itHist );
				break;
			}
		}

		gltProfHist.push_back( stProfHist );	//	���X�g�����قǐV����

		//	�����͂ݏo���悤�Ȃ�Â��̂��폜����
		dItems = gltProfHist.size( );
		if( OPENHIST_MAX <  dItems )
		{
			gltProfHist.pop_front(  );
		}
	}
	else	//	������w�薳���ꍇ�͑S�N��
	{
		gltProfHist.clear();
	}

	OpenProfileInitialise( NULL );	//	�Â��̔j�󂵂�
	OpenProfileInitialise( hWnd );	//	�ŐV�̓��e�ō�蒼��

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

