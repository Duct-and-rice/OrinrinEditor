/*! @file
	@brief �g�̖ʓ|����
	���̃t�@�C���� FrameCtrl.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/06/08
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
�����s�f�Ђ��g���ɂ�
�E��؂�́����Ƃ��A�ǂݏ��̃^�C�~���O�œK���ɕύX����
�E�e�p�[�c�̕��ƍ����������Ă���
�E�z�u�p�_�~�[���C���{�b�N�X�݂����Ȃ̂�����
�E�ڕW���ɍ��킹�āA��p�[�c����ׂ�E�E��́A��p�[�c�̉E�������v�Z�����邩�H
�E�����Ɍ������Ĕz�u���Ă���
�E�E���Ɍ������Ă����͓̂��������̂͂�
�E�z�u�o������A�g�{�b�N�X�Ȃ炻�̂܂܃��C�������A�}���Ȃ�A������������炷�������đ}����

�`�`�����z�u�Ǝ��Ă邩�H

INI�t�@�C���́A������擪�̔��p�͖�������炵���H
���������p�󔒂Ƃ���
*/

#define FRAMEINSERTBOX_CLASS	TEXT("FRAMEINSBOX_CLASS")
#define FIB_WIDTH	600
#define FIB_HEIGHT	200

#define TRANCE_COLOUR	RGB(0x66,0x77,0x88)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

#define TB_ITEMS	26
static  TBBUTTON	gstFIBTBInfo[] = {
	{ 20,	IDM_FRAME_INS_DECIDE,	TBSTATE_ENABLED,	TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE,		{0, 0}, 0, 0  },	//	
	{  0,	0,						TBSTATE_ENABLED,	TBSTYLE_SEP,							{0, 0}, 0, 0  },
	{  0,	IDM_INSFRAME_ALPHA   ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  1,	IDM_INSFRAME_BRAVO   ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  2,	IDM_INSFRAME_CHARLIE ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  3,	IDM_INSFRAME_DELTA   ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  4,	IDM_INSFRAME_ECHO    ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  5,	IDM_INSFRAME_FOXTROT ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  6,	IDM_INSFRAME_GOLF    ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  7,	IDM_INSFRAME_HOTEL   ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  8,	IDM_INSFRAME_INDIA   ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  9,	IDM_INSFRAME_JULIETTE,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{ 10,	IDM_INSFRAME_KILO,		TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{ 11,	IDM_INSFRAME_LIMA,		TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{ 12,	IDM_INSFRAME_MIKE,		TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{ 13,	IDM_INSFRAME_NOVEMBER,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{ 14,	IDM_INSFRAME_OSCAR,		TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{ 15,	IDM_INSFRAME_PAPA,		TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{ 16,	IDM_INSFRAME_QUEBEC,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{ 17,	IDM_INSFRAME_ROMEO,		TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{ 18,	IDM_INSFRAME_SIERRA,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{ 19,	IDM_INSFRAME_TANGO,		TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	{0, 0}, 0, 0  },	//	
	{  0,	0,						TBSTATE_ENABLED,	TBSTYLE_SEP,							{0, 0}, 0, 0  },
	{ 21,	IDM_FRMINSBOX_QCLOSE,	TBSTATE_ENABLED,	TBSTYLE_CHECK | TBSTYLE_AUTOSIZE,		{0, 0}, 0, 0  },	//	
	{  0,	0,						TBSTATE_ENABLED,	TBSTYLE_SEP,							{0, 0}, 0, 0  },
	{ 22,	IDM_FRMINSBOX_PADDING,	TBSTATE_ENABLED,	TBSTYLE_CHECK | TBSTYLE_AUTOSIZE,		{0, 0}, 0, 0  } 	//	
};	//	

CONST  TCHAR	*gatDefaultName[20] = {
	{ TEXT("ALPHA") },		{ TEXT("BRAVO") },	{ TEXT("CHARLIE") },	{ TEXT("DELTA") },		{ TEXT("ECHO") },
	{ TEXT("FOXTROT") },	{ TEXT("GOLF") },	{ TEXT("HOTEL") },		{ TEXT("INDIA") },		{ TEXT("JULIETTE") },
	{ TEXT("KILO") },		{ TEXT("LIMA") },	{ TEXT("MIKE") },		{ TEXT("NOVEMBER") },	{ TEXT("OSCAR") },
	{ TEXT("POPPA") },		{ TEXT("QUEBEC") },	{ TEXT("ROMEO") },		{ TEXT("SIERRA") },		{ TEXT("TANGO") }
};

//-------------------------------------------------------------------------------------------------


extern FILES_ITR	gitFileIt;	//!<	�����Ă�t�@�C���̖{��
extern INT		gixFocusPage;	//!<	���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X


extern  HWND	ghViewWnd;		//!<	�r���[�E�C���h�E�n���h��

extern INT		gdHideXdot;		//!<	���̉B�ꕔ��
extern INT		gdViewTopLine;	//!<	�\�����̍ŏ㕔�s�ԍ�

extern HFONT	ghAaFont;		//!<	AA�p�t�H���g

static INT		gNowSel;		//!<	�I�𒆂̘g�O�C���f�b�N�X

static TCHAR	gatFrameIni[MAX_PATH];	//!<	20110707	�g�p��INI�����E���₵��


static  ATOM		gFrmInsAtom;	//!<	
static  HWND		ghFrInbxWnd;
static  HWND		ghFIBtlbrWnd;
static HBRUSH		ghBgBrush;

static HIMAGELIST	ghFrameImgLst;	//!<	�}��BOX�p�̃c�[���o�[

static POINT		gstViewOrigin;	//!<	�r���[�̍���E�C���h�E�ʒu�E
static POINT		gstOffset;		//!<	�r���[���ォ��́A�{�b�N�X�̑��Έʒu
static POINT		gstFrmSz;		//!<	�E�C���h�E�G�b�W����`��̈�܂ł̃I�t�Z�b�g
static INT			gdToolBarHei;	//!<	�c�[���o�[����

static  UINT		gdSelect;		//!<	�I�������g�ԍ��O�`�X
static BOOLEAN		gbQuickClose;	//!<	�\��t�����璼������

extern HFONT		ghAaFont;		//	AA�p�t�H���g

static  RECT		gstOrigRect;	//!<	�_�C�����O�N�����A�܂�ŏ��E�C���h�E�T�C�Y

static LPTSTR		gptFrmSample;	//!<	�g�ݒ�_�C�����O�̌��{�p
static  RECT		gstSamplePos;	//!<	���{���̍���ʒu�ƁA�������̃I�t�Z�b�g��
static FRAMEINFO	gstNowFrameInfo;

static LPTSTR		gptFrmBox;		//!<	�}���g�p�̕�����

static  UINT		gbMultiPaddTemp;	//!<	�O���ɉ����悤�Ƀp�f�B���O���邩�E�}��BOX�p

static FRAMEINFO	gstFrameInfo[FRAME_MAX];	//!<	�z��ŕK�v���m�ۂł�����
//-------------------------------------------------------------------------------------------------

INT_PTR	CALLBACK FrameEditDlgProc( HWND, UINT, WPARAM, LPARAM );	//!<	

INT_PTR	Frm_OnInitDialog( HWND , HWND, LPARAM );	//!<	
INT_PTR	Frm_OnCommand( HWND , INT, HWND, UINT );	//!<	
INT_PTR	Frm_OnDrawItem( HWND , CONST LPDRAWITEMSTRUCT );	//!<	
INT_PTR	Frm_OnNotify( HWND , INT, LPNMHDR );	//!<	

HRESULT	InitFrameItem( UINT, UINT, LPFRAMEINFO );	//!<	

HRESULT	FramePartsUpdate( HWND, HWND, LPFRAMEITEM );	//!<	

HRESULT	FrameDataGet( UINT, LPFRAMEINFO );	//!<	
HRESULT	FrameInfoDisp( HWND );	//!<	

VOID	FrameDataTranslate( LPTSTR, UINT );	//!<	
//INT		FramePartsSizeCalc( LPTSTR, PINT );

UINT	FrameMultiSubstring( LPCTSTR, CONST UINT, LPTSTR, CONST UINT_PTR, CONST INT );	//!<	

INT		FrameMultiSizeGet( LPFRAMEINFO, PINT, PINT );	//!<	
LPTSTR	FrameMakeOutsideBoundary( CONST INT, CONST INT, LPFRAMEINFO );	//!<	
LPTSTR	FrameMakeInsideBoundary( UINT , PINT, LPFRAMEINFO );	//!<	


INT		FrameInsBoxSizeGet( LPRECT );
VOID	FrameInsBoxFrmDraw( HDC );
VOID	FrameDrawItem( HDC, INT, LPTSTR );
INT_PTR	Frm_OnSize( HWND, UINT, INT, INT );
INT_PTR	Frm_OnWindowPosChanging( HWND, LPWINDOWPOS );

HRESULT	FrameInsBoxDoInsert( HWND );

LRESULT	CALLBACK FrameInsProc( HWND, UINT, WPARAM, LPARAM );
VOID	Fib_OnPaint( HWND );
VOID	Fib_OnCommand( HWND, INT, HWND, UINT );
VOID	Fib_OnDestroy( HWND );
VOID	Fib_OnMoving( HWND, LPRECT );			//!<	
VOID	Fib_OnKey( HWND, UINT, BOOL, INT, UINT );	//!<	
BOOL	Fib_OnWindowPosChanging( HWND, LPWINDOWPOS );		//!<	
VOID	Fib_OnWindowPosChanged( HWND, const LPWINDOWPOS );	//!<	
//-------------------------------------------------------------------------------------------------

/*!
	�g�ݒ��INI�t�@�C�����m�ہE�A�v���N���シ���Ă΂��
	@param[in]	ptCurrent	��f�B���N�g��
	@param[in]	hInstance	�C���X�^���X�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FrameInitialise( LPTSTR ptCurrent, HINSTANCE hInstance )
{
	WNDCLASSEX	wcex;
	UINT		resnum, ici;
	HBITMAP		hImg, hMsq;
	INT	iRslt;

	if( !(ptCurrent) || !(hInstance) )
	{
		if( ghFrInbxWnd ){	DestroyWindow( ghFrInbxWnd );	}
		if( ghBgBrush ){	DeleteBrush( ghBgBrush );	}

		if( ghFrameImgLst ){	ImageList_Destroy( ghFrameImgLst  );	}

		return S_OK;
	}

	StringCchCopy( gatFrameIni, MAX_PATH, ptCurrent );
	PathAppend( gatFrameIni, FRAME_INI_FILE );

//�g�}����
	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= FrameInsProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= FRAMEINSERTBOX_CLASS;
	wcex.hIconSm		= NULL;

	gFrmInsAtom = RegisterClassEx( &wcex );

	ghBgBrush = CreateSolidBrush( TRANCE_COLOUR );

	ghFrInbxWnd = NULL;

	gptFrmBox = NULL;

	gdSelect = 0;

	gbQuickClose = TRUE;

	ZeroMemory( &gstViewOrigin, sizeof(POINT) );
	ZeroMemory( &gstFrmSz, sizeof(POINT) );
	gdToolBarHei = 0;

	//	�A�C�R��
	ghFrameImgLst = ImageList_Create( 16, 16, ILC_COLOR24 | ILC_MASK, 23, 1 );
	resnum = IDBMP_FRMINS_ALPHA;
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMQ_FRMINS_SEL ) );
	for( ici = 0; FRAME_MAX > ici; ici++ )
	{
		hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( (resnum++) ) );
		iRslt = ImageList_Add( ghFrameImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ��E�O�`�P�X
		DeleteBitmap( hImg );
	}
	DeleteBitmap( hMsq );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMP_FRMINS_INSERT ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMQ_FRMINS_INSERT ) );
	iRslt = ImageList_Add( ghFrameImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ��E�Q�O
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMP_REFRESH ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMQ_REFRESH ) );
	iRslt = ImageList_Add( ghFrameImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ��E�Q�P
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMP_FRMINS_PADD ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMQ_FRMINS_PADD ) );
	iRslt = ImageList_Add( ghFrameImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ��E�Q�Q
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g���̂̓��e�Ń��j���[��ύX
	@param[in]	hWnd	���C���E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FrameNameModifyMenu( HWND hWnd )
{
	HMENU	hMenu, hSubMenu;
	UINT	i, j, k;
	TCHAR	atBuffer[MAX_PATH], atName[MAX_STRING];

	//	���j���[�\���ς�����炱�����ύX�E�ǂ��ɂ��Ȃ��̂�
	hMenu = GetMenu( hWnd );
	hSubMenu = GetSubMenu( hMenu, 2 );

	//	�g
	for( i = 0, j = 1; FRAME_MAX > i; i++, j++ )
	{
		FrameNameLoad( i, atName, MAX_STRING );

		if(  9 >= j ){		k = j + '0';	}
		else if( 10 == j ){	k = '0';	}
		else{		k = 'A' + j - 11;	}
		StringCchPrintf( atBuffer, MAX_PATH, TEXT("%s(&%c)"), atName, k );
		ModifyMenu( hSubMenu, IDM_INSFRAME_ALPHA+i, MF_BYCOMMAND | MFT_STRING, IDM_INSFRAME_ALPHA+i, atBuffer );
		//	���j���[���\�[�X�ԍ��̘A�Ԃɒ���
	}

	DrawMenuBar( hWnd );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�|�b�p�b�v���j���[�p�ɖ��O����������΂�����
	@param[in]	hPopMenu	�Ώۂ̃|�b�p�b�v���j���[�n���h��
	@param[in]	bMode		��O���j���[�L�[�t����@�O�t���Ȃ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FrameNameModifyPopUp( HMENU hPopMenu, UINT bMode )
{
	UINT	i, j, k;
	TCHAR	atBuffer[MAX_PATH], atName[MAX_STRING];

	for( i = 0, j = 1; FRAME_MAX > i; i++, j++ )
	{
		FrameNameLoad( i, atName, MAX_STRING );

		if( bMode )
		{
			if(  9 >= j ){		k = j + '0';	}
			else if( 10 == j ){	k = '0';	}
			else{		k = 'A' + j - 11;	}
			StringCchPrintf( atBuffer, MAX_PATH, TEXT("%s(&%c)"), atName, k );
		}
		else
		{
			StringCchPrintf( atBuffer, MAX_PATH, TEXT("%s"), atName );
		}
		ModifyMenu( hPopMenu, IDM_INSFRAME_ALPHA+i, MF_BYCOMMAND | MFT_STRING, IDM_INSFRAME_ALPHA+i, atBuffer );
		//	���j���[���\�[�X�ԍ��̘A�Ԃɒ���
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g�̖��O�����������Ă���
	@param[in]	dNumber	�g�ԍ��O�C���f�b�N�X
	@param[out]	ptNamed	���O�����o�b�t�@�ւ̃|�C���^�[
	@param[in]	cchSize	�o�b�t�@�̕������E�o�C�g����Ȃ���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FrameNameLoad( UINT dNumber, LPTSTR ptNamed, UINT_PTR cchSize )
{
	TCHAR	atAppName[MIN_STRING];

	if( !(ptNamed) || 0 >= cchSize )	return E_INVALIDARG;

	if( FRAME_MAX <= dNumber )	return E_OUTOFMEMORY;

	//	�����APP�������
	StringCchPrintf( atAppName, MIN_STRING, TEXT("Frame%u"), dNumber );

	GetPrivateProfileString( atAppName, TEXT("Name"), gatDefaultName[dNumber], ptNamed, cchSize, gatFrameIni );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

//	20110707	�ꏊ�ς���
/*!
	�g���̃Z�[�u���[�h
	@param[in]		dMode	��O���[�h�@�O�Z�[�u
	@param[in]		dNumber	�g�ԍ��O�C���f�b�N�X
	@param[in,out]	pstInfo	�������o�b�t�@��������ۑ����e�������肷��\���̂ۂ��񂽁`
	@return			HRESULT	�I����ԃR�[�h
*/
HRESULT InitFrameItem( UINT dMode, UINT dNumber, LPFRAMEINFO pstInfo )
{
	TCHAR	atAppName[MIN_STRING], atBuff[MIN_STRING];
	TCHAR	atBuffer[PARTS_CCH];

	//	�����APP�������
	StringCchPrintf( atAppName, MIN_STRING, TEXT("Frame%u"), dNumber );

//20120105	�����s�������A�����Ɓ����ɂ�鑊�ݕϊ�������p��

	if( dMode )	//	���[�h
	{
		GetPrivateProfileString( atAppName, TEXT("Name"), gatDefaultName[dNumber], pstInfo->atFrameName, MAX_STRING, gatFrameIni );

		GetPrivateProfileString( atAppName, TEXT("Daybreak"),  TEXT("��"), pstInfo->stDaybreak.atParts, PARTS_CCH, gatFrameIni );
		GetPrivateProfileString( atAppName, TEXT("Morning"),   TEXT("��"), pstInfo->stMorning.atParts, PARTS_CCH, gatFrameIni );
		GetPrivateProfileString( atAppName, TEXT("Noon"),      TEXT("��"), pstInfo->stNoon.atParts, PARTS_CCH, gatFrameIni );
		GetPrivateProfileString( atAppName, TEXT("Afternoon"), TEXT("��"), pstInfo->stAfternoon.atParts, PARTS_CCH, gatFrameIni );
		GetPrivateProfileString( atAppName, TEXT("Nightfall"), TEXT("��"), pstInfo->stNightfall.atParts, PARTS_CCH, gatFrameIni );
		GetPrivateProfileString( atAppName, TEXT("Twilight"),  TEXT("��"), pstInfo->stTwilight.atParts, PARTS_CCH, gatFrameIni );
		GetPrivateProfileString( atAppName, TEXT("Midnight"),  TEXT("��"), pstInfo->stMidnight.atParts, PARTS_CCH, gatFrameIni );
		GetPrivateProfileString( atAppName, TEXT("Dawn"),      TEXT("��"), pstInfo->stDawn.atParts, PARTS_CCH, gatFrameIni );

		FrameDataTranslate( pstInfo->stDaybreak.atParts , 1 );
		FrameDataTranslate( pstInfo->stMorning.atParts , 1 );
		FrameDataTranslate( pstInfo->stNoon.atParts , 1 );
		FrameDataTranslate( pstInfo->stAfternoon.atParts , 1 );
		FrameDataTranslate( pstInfo->stNightfall.atParts , 1 );
		FrameDataTranslate( pstInfo->stTwilight.atParts , 1 );
		FrameDataTranslate( pstInfo->stMidnight.atParts , 1 );
		FrameDataTranslate( pstInfo->stDawn.atParts , 1 );

		GetPrivateProfileString( atAppName, TEXT("LEFTOFFSET"),  TEXT("0"), atBuff, MIN_STRING, gatFrameIni );
		pstInfo->dLeftOffset  = StrToInt( atBuff );
		GetPrivateProfileString( atAppName, TEXT("RIGHTOFFSET"), TEXT("0"), atBuff, MIN_STRING, gatFrameIni );
		pstInfo->dRightOffset = StrToInt( atBuff );

		//�ǉ�
		GetPrivateProfileString( atAppName, TEXT("RestPadding"),  TEXT("1"), atBuff, MIN_STRING, gatFrameIni );
		pstInfo->dRestPadd = StrToInt( atBuff );

	}
	else	//	�Z�[�u
	{
		WritePrivateProfileString( atAppName, TEXT("Name"), pstInfo->atFrameName, gatFrameIni );

		StringCchCopy( atBuffer, PARTS_CCH, pstInfo->stDaybreak.atParts );	FrameDataTranslate( atBuffer, 0 );
		WritePrivateProfileString( atAppName, TEXT("Daybreak"),  atBuffer, gatFrameIni );
		StringCchCopy( atBuffer, PARTS_CCH, pstInfo->stMorning.atParts );	FrameDataTranslate( atBuffer, 0 );
		WritePrivateProfileString( atAppName, TEXT("Morning"),   atBuffer, gatFrameIni );
		StringCchCopy( atBuffer, PARTS_CCH, pstInfo->stNoon.atParts );		FrameDataTranslate( atBuffer, 0 );
		WritePrivateProfileString( atAppName, TEXT("Noon"),      atBuffer, gatFrameIni );
		StringCchCopy( atBuffer, PARTS_CCH, pstInfo->stAfternoon.atParts );	FrameDataTranslate( atBuffer, 0 );
		WritePrivateProfileString( atAppName, TEXT("Afternoon"), atBuffer, gatFrameIni );
		StringCchCopy( atBuffer, PARTS_CCH, pstInfo->stNightfall.atParts );	FrameDataTranslate( atBuffer, 0 );
		WritePrivateProfileString( atAppName, TEXT("Nightfall"), atBuffer, gatFrameIni );
		StringCchCopy( atBuffer, PARTS_CCH, pstInfo->stTwilight.atParts );	FrameDataTranslate( atBuffer, 0 );
		WritePrivateProfileString( atAppName, TEXT("Twilight"),  atBuffer, gatFrameIni );
		StringCchCopy( atBuffer, PARTS_CCH, pstInfo->stMidnight.atParts );	FrameDataTranslate( atBuffer, 0 );
		WritePrivateProfileString( atAppName, TEXT("Midnight"),  atBuffer, gatFrameIni );
		StringCchCopy( atBuffer, PARTS_CCH, pstInfo->stDawn.atParts );		FrameDataTranslate( atBuffer, 0 );
		WritePrivateProfileString( atAppName, TEXT("Dawn"),      atBuffer, gatFrameIni );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->dLeftOffset );
		WritePrivateProfileString( atAppName, TEXT("LEFTOFFSET"), atBuff, gatFrameIni );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->dRightOffset );
		WritePrivateProfileString( atAppName, TEXT("RIGHTOFFSET"), atBuff, gatFrameIni );

		//	�ǉ�
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->dRestPadd );
		WritePrivateProfileString( atAppName, TEXT("RestPadding"), atBuff, gatFrameIni );
	}


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	����Z����B�O���Z�Ȃ�O��Ԃ�
	@param[in]	iLeft	�����鐔
	@param[in]	iRight	���鐔
	@return	INT	�v�Z����
*/
INT Divinus( INT iLeft, INT iRight )
{
	 INT	iAnswer;

	if( 0 == iRight )	return 0;

	iAnswer = iLeft / iRight;

	return iAnswer;
}
//-------------------------------------------------------------------------------------------------


/*!
	�g�ݒ�̃_�C�����O���J��
	@param[in]	hInst	�A�v���̎���
	@param[in]	hWnd	�{�̂̃E�C���h�E�n���h���ł���悤�ɂ��邱��
	@param[in]	dRsv	���g�p
	@return �I���R�[�h
*/
INT_PTR FrameEditDialogue( HINSTANCE hInst, HWND hWnd, UINT dRsv )
{
	INT_PTR	iRslt;

	gNowSel = 0;

	iRslt = DialogBoxParam( hInst, MAKEINTRESOURCE(IDD_FRAME_EDIT_DLG_2), hWnd, FrameEditDlgProc, 0 );

	//	�������ʂɂ���ẮA�����Ń��j���[�̓��e����
	if( IDYES == iRslt ){	FrameNameModifyMenu( hWnd );	}

	return iRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g�ݒ�_�C�����O�v���V�[�W��
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK FrameEditDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		default:	break;

		case WM_INITDIALOG:	return Frm_OnInitDialog( hDlg, (HWND)(wParam), lParam );
		case WM_COMMAND:	return Frm_OnCommand( hDlg, (INT)(LOWORD(wParam)), (HWND)(lParam), (UINT)HIWORD(wParam) );
		case WM_DRAWITEM:	return Frm_OnDrawItem( hDlg, ((CONST LPDRAWITEMSTRUCT)(lParam)) );
		case WM_NOTIFY:		return Frm_OnNotify( hDlg, (INT)(wParam), (LPNMHDR)(lParam) );
		case WM_SIZE:		return Frm_OnSize( hDlg, (UINT)(wParam), (INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam) );

		case WM_WINDOWPOSCHANGING:	return Frm_OnWindowPosChanging( hDlg, (LPWINDOWPOS)(lParam) );

	}
	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�_�C�����O�N�����̏�����
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	hWndFocus	�Ȃ񂾂���
	@param[in]	lParam		�_�C�����O�I�[�|������Ƃ��ɌĂяo�������n�����l
	@return		���ɈӖ��͂Ȃ�
*/
INT_PTR Frm_OnInitDialog( HWND hDlg, HWND hWndFocus, LPARAM lParam )
{
	HWND	hWorkWnd;
	UINT	i, ofs;
	RECT	rect;
	POINT	point;

	GetWindowRect( hDlg, &gstOrigRect );
	gstOrigRect.bottom -= gstOrigRect.top;
	gstOrigRect.right  -= gstOrigRect.left;
	gstOrigRect.top  = 0;
	gstOrigRect.left = 0;

	TRACE( TEXT("FRM DLG SIZE [%d x %d]"), gstOrigRect.right, gstOrigRect.bottom );



	//	�R���{�b�N�X�ɖ��O����Ƃ�
	hWorkWnd = GetDlgItem( hDlg, IDCB_BOX_NAME_SEL );

	for( i = 0; FRAME_MAX > i; i++ )	//	�o�b�t�@�Ɋm��
	{
		FrameDataGet( i, &(gstFrameInfo[i]) );
		//	INI�t�@�C��������������āA�R���{�b�N�X�ɖ��O�����ꂿ�Ⴄ
		ComboBox_AddString( hWorkWnd, gstFrameInfo[i].atFrameName );
	}

	ComboBox_SetCurSel( hWorkWnd, gNowSel );

	//	�p�[�c���������
	FrameInfoDisp( hDlg );

	SetWindowFont( GetDlgItem(hDlg,IDS_FRAME_IMAGE), ghAaFont, FALSE );

	//	���{�E�C���h�E�̈ʒu���m�肵�Ă���
	GetWindowRect( GetDlgItem(hDlg,IDS_FRAME_IMAGE), &gstSamplePos );
	//	�������m��
	gstSamplePos.right -= gstSamplePos.left;
	gstSamplePos.bottom -= gstSamplePos.top;

	//	�N���C�����g�̈�Ƃ̃Y���𒲐�
	GetClientRect( GetDlgItem(hDlg,IDS_FRAME_IMAGE), &rect );
	ofs = gstSamplePos.right - rect.right;	gstSamplePos.right += ofs;
	ofs = gstSamplePos.bottom - rect.bottom;	gstSamplePos.bottom += ofs;

	//	�N���C�A���g��ł̈ʒu���m��
	point.x = gstSamplePos.left;	point.y = gstSamplePos.top;
	ScreenToClient( hDlg, &point );
	gstSamplePos.left = point.x;	gstSamplePos.top  = point.y;

	//	�N���C�A���g�̕������̃I�t�Z�b�g�ʂ��m��
	GetClientRect( hDlg, &rect );
	gstSamplePos.right  = rect.right  - gstSamplePos.right;
	gstSamplePos.bottom = rect.bottom - gstSamplePos.bottom;

	//xx = cx - gstSamplePos.right;
	//yy = cy - gstSamplePos.bottom;

	//SetWindowPos( hSmpWnd, HWND_TOP, 0, 0, xx, yy, SWP_NOMOVE | SWP_NOZORDER );



	GetClientRect( GetDlgItem(hDlg,IDS_FRAME_IMAGE), &rect );
	//	������ԂŊm��
	gptFrmSample = FrameMakeOutsideBoundary( rect.right, rect.bottom, &(gstFrameInfo[gNowSel]) );


	return (INT_PTR)TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�_�C�����O��COMMAND���b�Z�[�W�̎󂯎��
	@param[in]	hDlg		�_�C�����O�[�n���h��
	@param[in]	id			���b�Z�[�W�𔭐��������q�E�C���h�E�̎��ʎq	LOWORD(wParam)
	@param[in]	hWndCtl		���b�Z�[�W�𔭐��������q�E�C���h�E�̃n���h��	lParam
	@param[in]	codeNotify	�ʒm���b�Z�[�W	HIWORD(wParam)
	@return		�������������񂩂�����
*/
INT_PTR Frm_OnCommand( HWND hDlg, INT id, HWND hWndCtl, UINT codeNotify )
{
	static BOOLEAN	cbNameMod = FALSE;	//	�_�C�����O�I���p�̍P�v�I�Ȃ���
	static BOOLEAN	cbNameChg = FALSE;	//	APPLY�p
	UINT	i;
	INT		iRslt;
	HWND	hCmboxWnd;
	RECT	rect;

	switch( id )
	{
		default:	break;

		case IDCANCEL:
		case IDB_CANCEL:
			FREE( gptFrmSample );
			EndDialog( hDlg, IDCANCEL );
			return (INT_PTR)TRUE;

		case IDB_APPLY:
		case IDB_OK:
			hCmboxWnd = GetDlgItem( hDlg, IDCB_BOX_NAME_SEL );
			for( i = 0; FRAME_MAX > i; i++ )
			{
				InitFrameItem( INIT_SAVE, i, &(gstFrameInfo[i]) );
				if( cbNameChg )
				{
					ComboBox_DeleteString( hCmboxWnd, 0 );//�擪������
					ComboBox_AddString( hCmboxWnd, gstFrameInfo[i].atFrameName );//�����ɕt������
				}
			}
			ComboBox_SetCurSel( hCmboxWnd, gNowSel );
			cbNameChg = FALSE;
			if( IDB_OK ==  id )
			{
				FREE( gptFrmSample );
				EndDialog( hDlg, cbNameMod ? IDYES : IDOK );
			}
			return (INT_PTR)TRUE;

		case IDE_BOXP_MORNING:		if( EN_UPDATE == codeNotify ){	FramePartsUpdate( hDlg , hWndCtl, &(gstFrameInfo[gNowSel].stMorning) );	}		return (INT_PTR)TRUE;
		case IDE_BOXP_NOON:			if( EN_UPDATE == codeNotify ){	FramePartsUpdate( hDlg , hWndCtl, &(gstFrameInfo[gNowSel].stNoon) );	}		return (INT_PTR)TRUE;
		case IDE_BOXP_AFTERNOON:	if( EN_UPDATE == codeNotify ){	FramePartsUpdate( hDlg , hWndCtl, &(gstFrameInfo[gNowSel].stAfternoon) );	}	return (INT_PTR)TRUE;
		case IDE_BOXP_DAYBREAK:		if( EN_UPDATE == codeNotify ){	FramePartsUpdate( hDlg , hWndCtl, &(gstFrameInfo[gNowSel].stDaybreak) );	}	return (INT_PTR)TRUE;
		case IDE_BOXP_NIGHTFALL:	if( EN_UPDATE == codeNotify ){	FramePartsUpdate( hDlg , hWndCtl, &(gstFrameInfo[gNowSel].stNightfall) );	}	return (INT_PTR)TRUE;
		case IDE_BOXP_TWILIGHT:		if( EN_UPDATE == codeNotify ){	FramePartsUpdate( hDlg , hWndCtl, &(gstFrameInfo[gNowSel].stTwilight) );	}	return (INT_PTR)TRUE;
		case IDE_BOXP_MIDNIGHT:		if( EN_UPDATE == codeNotify ){	FramePartsUpdate( hDlg , hWndCtl, &(gstFrameInfo[gNowSel].stMidnight) );	}	return (INT_PTR)TRUE;
		case IDE_BOXP_DAWN:			if( EN_UPDATE == codeNotify ){	FramePartsUpdate( hDlg , hWndCtl, &(gstFrameInfo[gNowSel].stDawn) );	}		return (INT_PTR)TRUE;

		case IDS_FRAME_IMAGE:
			if( STN_DBLCLK == codeNotify )	//	�_�{�[�N���b�N���ꂽ
			{
				TRACE( TEXT("���ځ[�������") );
				InvalidateRect( hWndCtl, NULL, TRUE );
			}
			return (INT_PTR)TRUE;

		case IDB_FRM_PADDING:
			iRslt = Button_GetCheck( hWndCtl );
			gstFrameInfo[gNowSel].dRestPadd = (BST_CHECKED == iRslt) ? TRUE : FALSE;

			GetClientRect( GetDlgItem(hDlg,IDS_FRAME_IMAGE), &rect );
			FREE( gptFrmSample );
			gptFrmSample = FrameMakeOutsideBoundary( rect.right, rect.bottom, &(gstFrameInfo[gNowSel]) );

			InvalidateRect( GetDlgItem(hDlg,IDS_FRAME_IMAGE), NULL, TRUE );
			return (INT_PTR)TRUE;

		case IDB_BOXP_NAME_APPLY:	//	���̂�ύX����
			Edit_GetText( GetDlgItem(hDlg,IDE_BOXP_NAME_EDIT), gstFrameInfo[gNowSel].atFrameName, MAX_STRING );
			cbNameMod = TRUE;	cbNameChg = TRUE;
			return (INT_PTR)TRUE;

		case IDCB_BOX_NAME_SEL:
			if( CBN_SELCHANGE == codeNotify )	//	�I�����ύX���ꂽ
			{
				gNowSel = ComboBox_GetCurSel( hWndCtl );
				FrameInfoDisp( hDlg );

				GetClientRect( GetDlgItem(hDlg,IDS_FRAME_IMAGE), &rect );
				FREE( gptFrmSample );
				gptFrmSample = FrameMakeOutsideBoundary( rect.right, rect.bottom, &(gstFrameInfo[gNowSel]) );

				InvalidateRect( GetDlgItem(hDlg,IDS_FRAME_IMAGE), NULL, TRUE );
			}
			return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�e�p�[�c���X�V���ꂽ��
	@param[in]	hDlg	�_�C�����Q�n���h�D
	@param[in]	hWndCtl	�Ώۂ̃p�[�cEDITBOX
	@param[in]	pstItem	�Ώۃp�[�c�̃f�[�^
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FramePartsUpdate( HWND hDlg, HWND hWndCtl, LPFRAMEITEM pstItem )
{
	TCHAR	atBuffer[PARTS_CCH];

	if( Edit_GetTextLength( hWndCtl ) )
	{
		Edit_GetText( hWndCtl, atBuffer, PARTS_CCH );
		atBuffer[PARTS_CCH-1] = 0;
		StringCchCopy( pstItem->atParts, PARTS_CCH, atBuffer );
	}
	else	//	�������Ȃ�������A�S�p�󔒂ɂ����Ⴄ
	{
		StringCchCopy( pstItem->atParts, PARTS_CCH, TEXT("�@") );
	}

	//	�h�b�g���m�F����
	//pstItem->dDot = FramePartsSizeCalc( pstItem->atParts, &(pstItem->iLine) );
	pstItem->iLine = DocStringInfoCount( pstItem->atParts, 0, &(pstItem->dDot), NULL );

	//	���łɍĕ`��
	InvalidateRect( GetDlgItem(hDlg,IDS_FRAME_IMAGE), NULL, TRUE );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�_�C�����O�̃T�C�Y�ύX����������O�ɑ����Ă���
	@param[in]	hDlg	�_�C�����O�̃n���h��
	@param[in]	pstWpos	�V�����ʒu�Ƒ傫���������Ă�
	@return		�������������񂩂������ł����H
*/
INT_PTR Frm_OnWindowPosChanging( HWND hDlg, LPWINDOWPOS pstWpos )
{
	//	�ړ����Ȃ������Ƃ��͉������Ȃ��ł���
	if( SWP_NOSIZE & pstWpos->flags )	return FALSE;

	//	x,y�F�E�C���h�E������W�@cx,cy�F�E�C���h�E�̕�����
	TRACE( TEXT("FRM CHANGING [%d x %d][%d x %d]"), pstWpos->x, pstWpos->y, pstWpos->cx, pstWpos->cy );

	if( gstOrigRect.right > pstWpos->cx )	pstWpos->cx = gstOrigRect.right;
	if( gstOrigRect.bottom > pstWpos->cy )	pstWpos->cy = gstOrigRect.bottom;

	return (INT_PTR)TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�_�C�����O���T�C�Y�ύX���ꂽ�Ƃ�
	@param[in]	hDlg	�_�C�����O�̃n���h��
	@param[in]	state	�ύX�̏�ԁESIZE_MINIMIZED �Ƃ�
	@param[in]	cx		�N���C�����g�w�T�C�Y
	@param[in]	cy		�N���C�����g�x�T�C�Y
	@return	�������������񂩂�����
*/
INT_PTR Frm_OnSize( HWND hDlg, UINT state, INT cx, INT cy )
{
	HWND	hSmpWnd;
	INT		xx, yy;
	RECT	rect;

	TRACE( TEXT("FRM SIZE [%d x %d]"), cx, cy );

	hSmpWnd = GetDlgItem( hDlg, IDS_FRAME_IMAGE );

	//	�������ɏ�ɑS�J
	xx = cx - gstSamplePos.right;
	yy = cy - gstSamplePos.bottom;

	SetWindowPos( hSmpWnd, HWND_TOP, 0, 0, xx, yy, SWP_NOMOVE | SWP_NOZORDER );

	GetClientRect( hSmpWnd, &rect );
	FREE( gptFrmSample );
	gptFrmSample = FrameMakeOutsideBoundary( rect.right, rect.bottom, &(gstFrameInfo[gNowSel]) );

	InvalidateRect( GetDlgItem( hDlg, IDS_FRAME_IMAGE ), NULL, TRUE );

	return (INT_PTR)TRUE;
}
//-------------------------------------------------------------------------------------------------


/*!
	�I�[�i�[�h���[�̏����E�X�^�e�B�b�N�̃A��
	@param[in]	hDlg		�_�C�����Q�n���h�D
	@param[in]	pstDrawItem	�h���[���ւ̃|�C���^�[
	@return		�������������񂩂�����
*/
INT_PTR Frm_OnDrawItem( HWND hDlg, CONST LPDRAWITEMSTRUCT pstDrawItem )
{
//	HFONT	hFtOld;

//	LPTSTR	ptMultiStr;

	if( IDS_FRAME_IMAGE != pstDrawItem->CtlID ){	return (INT_PTR)FALSE;	}

//	hFtOld = SelectFont( pstDrawItem->hDC, ghAaFont );	//	�t�H���g�ݒ�

	FillRect( pstDrawItem->hDC, &(pstDrawItem->rcItem), GetSysColorBrush( COLOR_WINDOW ) );
	SetBkMode( pstDrawItem->hDC, TRANSPARENT );
//�������畡���s��������΂�����

//	ptMultiStr = FrameMakeOutsideBoundary( pstDrawItem->rcItem.right, pstDrawItem->rcItem.bottom, &(gstFrameInfo[gNowSel]) );

	DrawText( pstDrawItem->hDC, gptFrmSample, -1, &(pstDrawItem->rcItem), DT_LEFT | DT_NOPREFIX | DT_NOCLIP | DT_WORDBREAK );
							//	ptMultiStr
//	FREE( ptMultiStr );

//	SelectFont( pstDrawItem->hDC, hFtOld );

	return (INT_PTR)TRUE;
}
//-------------------------------------------------------------------------------------------------

/*
�`�敝�͌��܂��Ă���B�g�w��Ȃ�O����A�͈͎w��Ȃ�A������{���E�̕����l�`�w��

����p�[�c�ƉE��p�[�c�̕����m�F���āA�c�肪��p�[�c�g���B�g�����͕�����Z�o
��L�s�����قȂ�Ȃ�A���������킹��B�E���͓r���Ő؂鎖���l��


�������������͓����B��L�s���قȂ�Ȃ�A�㑤�����킹��B

���́A�K�v�s���m�F����B�����s�ɂȂ�Ȃ�A�r���Ő؂�B
�����͂O��_�A�E���́A�E��E���p�[�c�̍��ɍ��킹��{�I�t�Z�b�g

*/

/*!
	�n���ꂽ�p�[�c����A�K�v�ȂƂ���𔲂��o���ĕ�������
	@param[in]	bEnable	�s���L���͈͂ł��邩�ǂ����̔��f
	@param[in]	pstItem	�p�[�c�����������
	@param[out]	ptDest	����������������o�b�t�@�ւ̃|�C���^�[
	@param[in]	cchSz	�o�b�t�@�̕������E�o�C�g�ɔ�
	@return	
*/
UINT FrameMakeMultiSubLine( CONST BOOLEAN bEnable, LPFRAMEITEM pstItem, LPTSTR ptDest, CONST UINT_PTR cchSz )
{
	LPTSTR	ptBufStr;

	if( bEnable )	//	�L���ł��邩
	{
		//	�}���`�s�̈ꕔ���u�b�R����
		FrameMultiSubstring( pstItem->atParts, pstItem->iNowLn, ptDest, cchSz, pstItem->dDot );
		//�ő啝�ɖ����Ȃ��s�́APadding����
		pstItem->iNowLn++;
	}
	else	//	�󔒂ł���
	{
		ptBufStr = DocPaddingSpaceWithPeriod( pstItem->dDot, NULL, NULL, NULL, TRUE );//DocPaddingSpaceMake( pstItem->dDot );
		StringCchCopy( ptDest, cchSz, ptBufStr );
		FREE( ptBufStr );
	}

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g�p�[�c�́A�V��Ə��̐�L�s���ƁA�����̃h�b�g�����m�ۂ���
*/
INT FrameMultiSizeGet( LPFRAMEINFO pstInfo, PINT piUpLine, PINT piDnLine )
{
	INT	iUpLine, iDnLine;

	//	�s���̊m�F
	iUpLine = pstInfo->stMorning.iLine;	//	����
	if( iUpLine < pstInfo->stNoon.iLine )	iUpLine = pstInfo->stNoon.iLine;	//	��
	if( iUpLine < pstInfo->stAfternoon.iLine )	iUpLine = pstInfo->stAfternoon.iLine;	//	�E��

	iDnLine = pstInfo->stDawn.iLine;	//	����
	if( iDnLine < pstInfo->stMidnight.iLine )	iDnLine = pstInfo->stMidnight.iLine;	//	��
	if( iDnLine < pstInfo->stTwilight.iLine )	iDnLine = pstInfo->stTwilight.iLine;	//	�E��

	if( piUpLine )	*piUpLine = iUpLine;
	if( piDnLine )	*piDnLine = iDnLine;

	return pstInfo->stDaybreak.dDot;
}
//-------------------------------------------------------------------------------------------------

/*!
	��������󂯂āA�O�p�f�B���O���āA���҂�����ɂȂ�悤�Ƀp�f�B���O������؂����肷��
	@param[in]		iFwOffs	�O�ǉ����E�O�Ŗ���
	@param[in,out]	ptStr	������E���H���Ė߂�
	@param[in]		cchSz	������̕�����
	@param[in]		iMaxDot	�S�̂̃h�b�g���E�O�Ŗ���
	@return	
*/
UINT StringWidthAdjust( CONST UINT iFwOffs, LPTSTR ptStr, CONST UINT_PTR cchSz, CONST INT iMaxDot )
{
	INT			iDot, iPadd;
	INT			iDotCnt, iBuf;
	UINT_PTR	dm, dMozi;
	TCHAR		atWork[MAX_PATH];
	LPTSTR		ptBufStr;

	ZeroMemory( atWork, sizeof(atWork) );

	if( 1 <= iFwOffs )
	{
		ptBufStr = DocPaddingSpaceWithPeriod( iFwOffs, NULL, NULL, NULL, TRUE );//DocPaddingSpaceMake( iFwOffs );
		if( ptBufStr )
		{
			StringCchCopy( atWork, MAX_PATH, ptBufStr );
			FREE( ptBufStr );
		}
	}
	StringCchCat( atWork, MAX_PATH, ptStr );

	iDot = ViewStringWidthGet( atWork );

	if( (0 != iMaxDot) && (iDot != iMaxDot) )	//	�O�ł͂Ȃ��A���x�ł������ꍇ
	{
		if( iDot < iMaxDot )	//	�w�蕝�̂ق����L���Ȃ�p�f�B���O�[
		{
			iPadd = iMaxDot - iDot;
			ptBufStr = DocPaddingSpaceWithPeriod( iPadd, NULL, NULL, NULL, TRUE );//DocPaddingSpaceMake( iPadd );
			if( ptBufStr )
			{
				StringCchCat( atWork, MAX_PATH, ptBufStr );
				FREE( ptBufStr );
			}
		}
		else	//	�����łȂ��Ȃ�Ԃ����؂�
		{
			StringCchLength( atWork, MAX_PATH, &dMozi );
			iDotCnt = 0;	//	�����m�F���Ă���
			for( dm = 0; dMozi > dm; dm++ )
			{
				iBuf = ViewLetterWidthGet( atWork[dm] );
				if( iMaxDot < (iDotCnt+iBuf) )
				{
					atWork[dm] = NULL;	//	��U���������
					iBuf = iMaxDot - iDotCnt;
					if( 0 < iBuf )
					{
						ptBufStr = DocPaddingSpaceWithPeriod( iBuf, NULL, NULL, NULL, TRUE );//DocPaddingSpaceMake( iBuf );
						if( ptBufStr )
						{
							StringCchCat( atWork, MAX_PATH, ptBufStr );
							FREE( ptBufStr );
						}
					}
					break;
				}
				iDotCnt += iBuf;
			}
		}

		iDot = ViewStringWidthGet( atWork );
	}

	StringCchCopy( ptStr, cchSz, atWork );

	return iDot;
}
//-------------------------------------------------------------------------------------------------

/*!
	�O�g�ɍ��킹�āA�����s�g�����
	@param[in]	iWidth	�O�g���h�b�g��
	@param[in]	iHeight	�O�g���h�b�g��
	@param[in]	pstInfo	�g���g�̏��
	@return	�m�ۂ����������Ԃ��Efree�ɒ��ӁE���s�Ȃ�NULL
*/
LPTSTR FrameMakeOutsideBoundary( CONST INT iWidth, CONST INT iHeight, LPFRAMEINFO pstInfo )
{
	LPFRAMEITEM	pstItem;

	TCHAR		atSubStr[MAX_PATH];
	LPTSTR		ptBufStr;
	INT	iLines, i;	//	�S�̍s��
	INT	iUpLine, iMdLine, iDnLine;	//	�V��A���A���̐�L�s

	INT	iRitOccup;
	INT	iOfsLine, iRight;
	INT	iRoofDot, iFloorDot;
	INT	iRoofCnt, iFloorCnt;	//	��Ɖ��̃p�[�c�̌�
	INT	iRfRstDot, iFlRstDot;
	INT	iRitOff;//, iPillarDot, iFloodDot;
	INT	iRitBuf;
	INT	iRealWid;
	INT	ic;
	INT	iTgtLn;	//	�e���̃t�H�[�J�X�s��
	UINT	bMultiPadd;
	BOOLEAN	bEnable;

	UINT_PTR	cchTotal, dCount, d;

	HRESULT	hRslt;

	wstring	wsWorkStr;
	vector<wstring>	vcString;	//	��Ɨp

#ifdef DO_TRY_CATCH
	try{
#endif

	wsWorkStr.assign( TEXT("") );

	iLines =  iHeight / LINE_HEIGHT;	//	�؂�̂Ăł���
	TRACE( TEXT("MF LINE %d"), iLines );

	bMultiPadd = pstInfo->dRestPadd;	//	�p�f�B���O���邩�ǂ���

	//	�s���̊m�F
	FrameMultiSizeGet( pstInfo, &iUpLine, &iDnLine );

	iMdLine = iLines - (iUpLine + iDnLine);	//	��
	TRACE( TEXT("MF R[%d] P[%d] F[%d]"), iUpLine, iMdLine, iDnLine );
	//	���� iMdLine ���}�C�i�X�ɂȂ�����H�@��������Ȃ��Ȃ邾��
	if( 0 > iMdLine ){	iLines -= iMdLine;	iMdLine = 0;	}	//	�����ɒ��ӃZ��
	//	�͂ݏo���ƃo�O��̂ŁA�͂ݏo�������͖����������Ƃɂ���

	for( i = 0; iLines > i; i++ )
	{
		vcString.push_back( wsWorkStr );	//	��Ɋm��
	}



	iRealWid = iWidth;

	//	�E�p�[�c�̐�L���A��Ԓ����̂��m�F
	iRitOccup = pstInfo->stAfternoon.dDot;	//	�E��
	if( iRitOccup <  pstInfo->stTwilight.dDot ){	iRitOccup = pstInfo->stTwilight.dDot;	}	//	�E��
	iRitBuf = pstInfo->dRightOffset + pstInfo->stNightfall.dDot;	//	�E�ƃI�t�Z�b�g
	if( iRitOccup < iRitBuf ){	iRitOccup = iRitBuf;	}	//	�E
	//	iRitOccup�́A�E�p�[�c�̍ő��L���ł���
	iRitOff = iWidth - iRitOccup;	//	�E�p�[�c�J�n�ʒu���m��

	//	�V��Ɏg����h�b�g�����Q�b�g
		//iRitOff = iWidth - pstInfo->stAfternoon.dDot;	//	�E��̐�L��
	iRoofDot  = iRitOff - pstInfo->stMorning.dDot;	//	�V��Ɏg����h�b�g��
	if( 1 <= pstInfo->dLeftOffset ){	iRoofDot -= pstInfo->dLeftOffset;	}
	iRoofCnt  = Divinus( iRoofDot, pstInfo->stNoon.dDot );	//	��L�h�b�g���m�F���āA���o���B�؂�̂Ăł���
	iRfRstDot = iRoofDot - (iRoofCnt * pstInfo->stNoon.dDot);

//�돜�Z����

	//	�����ƉE���Ə�
		//iRitOff = iWidth - pstInfo->stTwilight.dDot;	//	�E���̐�L��
	iFloorDot = iRitOff - pstInfo->stDawn.dDot;	//	���Ɏg����h�b�g��
	if( 1 <= pstInfo->dLeftOffset ){	iFloorDot -= pstInfo->dLeftOffset;	}
	iFloorCnt = Divinus( iFloorDot , pstInfo->stMidnight.dDot );	//	��L�h�b�g���m�F���āA���o���B
	iFlRstDot = iFloorDot - (iFloorCnt * pstInfo->stMidnight.dDot);

	//	�E���J�n�ʒu�E�p�f�B���O���l���Z��
	if( bMultiPadd )
	{
		iRight = iRitOff + pstInfo->dRightOffset;
		//	���̃I�t�Z�b�g�Ȃ�A�����ɂ߂荞�ނ̂ŁA���̕��[�܂ł̒������k�߂�
		if( -1 >= pstInfo->dLeftOffset ){	iRight +=  pstInfo->dLeftOffset;	}
		//	�����̈����ɒ���
	}
	else
	{
		iRight = (iRoofCnt * pstInfo->stNoon.dDot) + pstInfo->stMorning.dDot + pstInfo->dRightOffset;
		iRitBuf = (iFloorCnt * pstInfo->stMidnight.dDot) + pstInfo->stDawn.dDot + pstInfo->dRightOffset;
		if( iRight < iRitBuf ){	iRight = iRitBuf;	};	//	�������ɍ��킹��

		iRight +=  pstInfo->dLeftOffset;
	}

	//	�g���킹�g�僂�[�h�̂Ƃ��̓I�t�Z�b�g�͍l�����Ȃ�

	TRACE( TEXT("MF RD[%d]C[%d][%d] FD[%d]C[%d][%d]"), iRoofDot, iRoofCnt, iRfRstDot, iFloorDot, iFloorCnt, iFlRstDot );

	//	��ƂɌ����ăN�����[
	pstInfo->stDaybreak.iNowLn  = 0;
	pstInfo->stMorning.iNowLn   = 0;
	pstInfo->stNoon.iNowLn      = 0;
	pstInfo->stAfternoon.iNowLn = 0;
	pstInfo->stNightfall.iNowLn = 0;
	pstInfo->stTwilight.iNowLn  = 0;
	pstInfo->stMidnight.iNowLn  = 0;
	pstInfo->stDawn.iNowLn      = 0;

	//	�V�䂩��E���[�����킹��
	for( iTgtLn = 0, iOfsLine = 0; iUpLine > iTgtLn; iTgtLn++, iOfsLine++ )
	{
		//	����
		if( 0 >= (iUpLine - iTgtLn) - pstInfo->stMorning.iLine )	bEnable = TRUE;
		else	bEnable = FALSE;
		FrameMakeMultiSubLine( bEnable, &(pstInfo->stMorning), atSubStr, MAX_PATH );
		if( 1 <= pstInfo->dLeftOffset ){	StringWidthAdjust( pstInfo->dLeftOffset, atSubStr, MAX_PATH, 0 );	}
		vcString.at( iOfsLine ).append( atSubStr );

		//	��
		if( 0 >= (iUpLine - iTgtLn) - pstInfo->stNoon.iLine )	bEnable = TRUE;
		else	bEnable = FALSE;
		FrameMakeMultiSubLine( bEnable, &(pstInfo->stNoon), atSubStr, MAX_PATH );
		//	�K�v���J��Ԃ�
		for( ic = 0; iRoofCnt >  ic; ic++ ){	vcString.at( iOfsLine ).append( atSubStr  );	}
		//	�]���Ă�h�b�g���߂�
		if( (1 <= iRfRstDot) && bMultiPadd )
		{
			StringWidthAdjust( 0, atSubStr, MAX_PATH, iRfRstDot );
			vcString.at( iOfsLine ).append( atSubStr );
		}

		//	�E��
		if( 0 >= (iUpLine - iTgtLn) - pstInfo->stAfternoon.iLine )	bEnable = TRUE;
		else	bEnable = FALSE;
		FrameMakeMultiSubLine( bEnable, &(pstInfo->stAfternoon), atSubStr, MAX_PATH );
		vcString.at( iOfsLine ).append( atSubStr );
	}

	//	�������Ă���
	for( iTgtLn = 0; iMdLine > iTgtLn; iTgtLn++, iOfsLine++ )
	{
		//	��
		pstItem = &(pstInfo->stDaybreak);		//	�E�J�n�ʒu�܂Ńp�f�B���O���Ă���
		FrameMultiSubstring( pstItem->atParts, pstItem->iNowLn, atSubStr, MAX_PATH, iRight );
		//	���̃p�f�B���O���Z�b�g
		if( -1 >= pstInfo->dLeftOffset ){	StringWidthAdjust( -(pstInfo->dLeftOffset), atSubStr, MAX_PATH, 0 );	}
		pstItem->iNowLn++;	//	��s�����[�v�����Ă���
		if( pstItem->iLine <= pstItem->iNowLn ){	pstItem->iNowLn = 0;	}
		vcString.at( iOfsLine ).append( atSubStr );

		//	�E
		pstItem = &(pstInfo->stNightfall);		//	���[�Ȃ̂Ńp�f�B���O�͕s�v
		FrameMultiSubstring( pstItem->atParts, pstItem->iNowLn, atSubStr, MAX_PATH, 0 );
		pstItem->iNowLn++;	//	��s�����[�v�����Ă���
		if( pstItem->iLine <= pstItem->iNowLn ){	pstItem->iNowLn = 0;	}
		vcString.at( iOfsLine ).append( atSubStr );
	}

	//	���E��[�����킹��
	for( iTgtLn = 0; iDnLine > iTgtLn; iTgtLn++, iOfsLine++ )
	{
		//	����
		if( iTgtLn < pstInfo->stDawn.iLine )	bEnable = TRUE;
		else	bEnable = FALSE;
		FrameMakeMultiSubLine( bEnable, &(pstInfo->stDawn), atSubStr, MAX_PATH );
		if( 1 <= pstInfo->dLeftOffset ){	StringWidthAdjust( pstInfo->dLeftOffset, atSubStr, MAX_PATH, 0 );	}
		vcString.at( iOfsLine ).append( atSubStr );

		//	��
		if( iTgtLn < pstInfo->stMidnight.iLine )	bEnable = TRUE;
		else	bEnable = FALSE;
		FrameMakeMultiSubLine( bEnable, &(pstInfo->stMidnight), atSubStr, MAX_PATH );
		//	�K�v���J��Ԃ�
		for( ic = 0; iFloorCnt > ic; ic++ ){	vcString.at( iOfsLine ).append( atSubStr );	}
		//	�]���Ă�h�b�g���߂�
		if( (1 <= iFlRstDot) && bMultiPadd )
		{
			StringWidthAdjust( 0, atSubStr, MAX_PATH, iFlRstDot );
			vcString.at( iOfsLine ).append( atSubStr );
		}

		//	�E��
		if( iTgtLn < pstInfo->stTwilight.iLine )
		{
			FrameMultiSubstring( pstInfo->stTwilight.atParts, pstInfo->stTwilight.iNowLn, atSubStr, MAX_PATH, 0 );
			pstInfo->stTwilight.iNowLn++;	//	��s�����[�v�����Ă���
			vcString.at( iOfsLine ).append( atSubStr );
		}
	}

	cchTotal = 0;
	dCount = vcString.size();	//	�L���s��
	//	�S�̂̕��������m��
	for( d = 0; dCount > d; d++ ){	cchTotal += vcString.at( d ).size();	}
	cchTotal += (dCount * sizeof(TCHAR));	//���s���{�]�T
	ptBufStr = (LPTSTR)malloc( cchTotal * sizeof(TCHAR) );	//	�T�C�Y�����
	if( ptBufStr )	//	�`�F�b�N
	{
		ZeroMemory( ptBufStr, cchTotal * sizeof(TCHAR) );
		for( d = 0; dCount > d; d++ )	//	�S���R�s�[
		{
			if( 0 != d )	hRslt = StringCchCat( ptBufStr, cchTotal, TEXT("\r\n") );
			hRslt = StringCchCat( ptBufStr, cchTotal, vcString.at( d ).c_str() );
		}
	}

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (LPTSTR)ETC_MSG( err.what( ), NULL );	}
	catch( ... ){	return (LPTSTR)ETC_MSG( ("etc error"), NULL );	}
#endif

	return ptBufStr;
}
//-------------------------------------------------------------------------------------------------

/*!
	���g�ɍ��킹�āA�����s�g�����
	@param[in]		dType	�O�������@�P�V��@�Q��
	@param[in,out]	piValue	�����ɂ���Ď󂯓n������
	@param[in]		pstInfo	�g���g�̏��
	@return	�m�ۂ����������Ԃ��Efree�ɒ��ӁE���s�Ȃ�NULL
*/
LPTSTR FrameMakeInsideBoundary( UINT dType, PINT piValue, LPFRAMEINFO pstInfo )
{
	static INT	iRoofCnt, iFloorCnt;	//	��Ɖ��̃p�[�c�̌�
	static INT	iRfRstDot, iFlRstDot;

	TCHAR		atSubStr[MAX_PATH];
	LPTSTR		ptBufStr;

	UINT	bMultiPadd;
	INT	iUpLine, iDnLine;
	INT	i, ic;
	INT	iRitOff;
	INT	iRitBuf, iRitVle;
	INT	iRoofDot, iFloorDot;
	INT	iTgtLn;	//	�e���̃t�H�[�J�X�s��
	INT	iOfsLine;
	BOOLEAN	bEnable;

	UINT_PTR	cchTotal, dCount, d;

	HRESULT	hRslt;

	wstring	wsWorkStr;
	vector<wstring>	vcString;	//	��Ɨp

#ifdef DO_TRY_CATCH
	try{
#endif

	wsWorkStr.assign( TEXT("") );

	bMultiPadd = pstInfo->dRestPadd;	//	�p�f�B���O���邩�ǂ���

	if( 0 == dType )	//	����������static�Ŏ����Ă�����
	{
		//	piValue[in]�����̃h�b�g���@[out]�E���J�n�ʒu

		//	�E���J�n�ʒu���m��
		iRitOff = pstInfo->stDaybreak.dDot + *piValue;
		//	���I�t�Z�b�g�}�C�i�X�Ȃ�A�����������ɂ߂荞�ށE�}�C�i�X�v�Z�ɒ���
		if( -1 >= pstInfo->dLeftOffset ){	iRitOff +=  -(pstInfo->dLeftOffset);	}
		//	�E�I�t�Z�b�g�}�C�i�X�Ȃ�A�E���������ɂ߂荞��
		if( -1 >= pstInfo->dRightOffset ){	iRitOff +=  -(pstInfo->dRightOffset);	}
		//	���Ȃ킿�A�E�J�n�ʒu�����E�Ɉړ�����

		//	�V��Ɏg����h�b�g�����Q�b�g
		iRoofDot  = iRitOff - pstInfo->stMorning.dDot;	//	�V��Ɏg����h�b�g��
		if( 1 <= pstInfo->dLeftOffset ){	iRoofDot -= pstInfo->dLeftOffset;	}
		iRoofCnt  = Divinus( iRoofDot, pstInfo->stNoon.dDot );	//	��L�h�b�g���m�F���āA���o���B�؂�̂Ăł���
		iRfRstDot = iRoofDot - (iRoofCnt * pstInfo->stNoon.dDot);	//	�p�[�c�����Ă�������]��h�b�g��

		//	�����ƉE���Ə�
		iFloorDot = iRitOff - pstInfo->stDawn.dDot;	//	���Ɏg����h�b�g��
		if( 1 <= pstInfo->dLeftOffset ){	iFloorDot -= pstInfo->dLeftOffset;	}
		iFloorCnt = Divinus( iFloorDot , pstInfo->stMidnight.dDot );	//	��L�h�b�g���m�F���āA���o���B
		iFlRstDot = iFloorDot - (iFloorCnt * pstInfo->stMidnight.dDot);	//	�p�[�c�����Ă�������]��h�b�g��

		if( !(bMultiPadd) )
		{
			//	�p�f�B���O���Ȃ��̂Ȃ�A�]�蕪�̓t���Ɏg���A�E���J�n�ʒu�́A��蒷�����ɍ��킹��
			if( 0 != iRfRstDot ){	iRoofCnt++;		}
			if( 0 != iFlRstDot ){	iFloorCnt++;	}

			//	�͂ݏo�����m��
			iRitVle = pstInfo->stNoon.dDot - iFlRstDot;
			iRitBuf = pstInfo->stMidnight.dDot - iFlRstDot;
			if( iRitVle < iRitBuf ){	iRitVle = iRitBuf;	}

			iRitOff += iRitVle;
		}

		//	�E�I�t�Z�b�g�u���X�Ȃ�A�E���J�n�ʒu�͂��E�Ɉړ�
		if( 1 <= pstInfo->dRightOffset ){	iRitOff += pstInfo->dRightOffset;	}

		*piValue = iRitOff;	//	�E�J�n�ʒu��߂�

		return NULL;
	}
	else if( 1 == dType )	//	�V��S��
	{
		FrameMultiSizeGet( pstInfo, &iUpLine, NULL );	//	�V��̍s��

		//	��ƂɌ����ăN�����[
		pstInfo->stMorning.iNowLn   = 0;
		pstInfo->stNoon.iNowLn      = 0;
		pstInfo->stAfternoon.iNowLn = 0;

		//	��Ɋm��
		for( i = 0; iUpLine > i; i++ ){	vcString.push_back( wsWorkStr );	}

		//	�V�䂩��E���[�����킹��
		for( iTgtLn = 0, iOfsLine = 0; iUpLine > iTgtLn; iTgtLn++, iOfsLine++ )
		{
			//	����
			if( 0 >= (iUpLine - iTgtLn) - pstInfo->stMorning.iLine )	bEnable = TRUE;
			else	bEnable = FALSE;
			FrameMakeMultiSubLine( bEnable, &(pstInfo->stMorning), atSubStr, MAX_PATH );
			if( 1 <= pstInfo->dLeftOffset ){	StringWidthAdjust( pstInfo->dLeftOffset, atSubStr, MAX_PATH, 0 );	}
			vcString.at( iOfsLine ).append( atSubStr );

			//	��
			if( 0 >= (iUpLine - iTgtLn) - pstInfo->stNoon.iLine )	bEnable = TRUE;
			else	bEnable = FALSE;
			FrameMakeMultiSubLine( bEnable, &(pstInfo->stNoon), atSubStr, MAX_PATH );
			//	�K�v���J��Ԃ�
			for( ic = 0; iRoofCnt > ic; ic++ ){	vcString.at( iOfsLine ).append( atSubStr );	}
			//	�]���Ă�h�b�g���߂�
			if( (1 <= iRfRstDot) && bMultiPadd )
			{
				StringWidthAdjust( 0, atSubStr, MAX_PATH, iRfRstDot );
				vcString.at( iOfsLine ).append( atSubStr );
			}

			//	�E��
			if( 0 >= (iUpLine - iTgtLn) - pstInfo->stAfternoon.iLine )	bEnable = TRUE;
			else	bEnable = FALSE;
			FrameMakeMultiSubLine( bEnable, &(pstInfo->stAfternoon), atSubStr, MAX_PATH );
			vcString.at( iOfsLine ).append( atSubStr );
		}
	}
	else if( 2 == dType )	//	���S��
	{
		FrameMultiSizeGet( pstInfo, NULL, &iDnLine );	//	���̍s��

		//	��ƂɌ����ăN�����[
		pstInfo->stTwilight.iNowLn  = 0;
		pstInfo->stMidnight.iNowLn  = 0;
		pstInfo->stDawn.iNowLn      = 0;

		//	��Ɋm��
		for( i = 0; iDnLine > i; i++ ){	vcString.push_back( wsWorkStr );	}

		//	���E��[�����킹��
		for( iTgtLn = 0, iOfsLine = 0; iDnLine > iTgtLn; iTgtLn++, iOfsLine++ )
		{
			//	����
			if( iTgtLn < pstInfo->stDawn.iLine )	bEnable = TRUE;
			else	bEnable = FALSE;
			FrameMakeMultiSubLine( bEnable, &(pstInfo->stDawn), atSubStr, MAX_PATH );
			if( 1 <= pstInfo->dLeftOffset ){	StringWidthAdjust( pstInfo->dLeftOffset, atSubStr, MAX_PATH, 0 );	}
			vcString.at( iOfsLine ).append( atSubStr );

			//	��
			if( iTgtLn < pstInfo->stMidnight.iLine )	bEnable = TRUE;
			else	bEnable = FALSE;
			FrameMakeMultiSubLine( bEnable, &(pstInfo->stMidnight), atSubStr, MAX_PATH );
			//	�K�v���J��Ԃ�
			for( ic = 0; iFloorCnt > ic; ic++ ){	vcString.at( iOfsLine ).append( atSubStr );	}
			//	�]���Ă�h�b�g���߂�
			if( (1 <= iFlRstDot) && bMultiPadd )
			{
				StringWidthAdjust( 0, atSubStr, MAX_PATH, iFlRstDot );
				vcString.at( iOfsLine ).append( atSubStr );
			}

			//	�E��
			if( iTgtLn < pstInfo->stTwilight.iLine )
			{
				FrameMultiSubstring( pstInfo->stTwilight.atParts, pstInfo->stTwilight.iNowLn, atSubStr, MAX_PATH, 0 );
				pstInfo->stTwilight.iNowLn++;	//	��s�����[�v�����Ă���
				vcString.at( iOfsLine ).append( atSubStr );
			}
		}

	}
	else{	return NULL;	}

	cchTotal = 0;
	dCount = vcString.size();	//	�L���s��
	//	�S�̂̕��������m��
	for( d = 0; dCount > d; d++ ){	cchTotal += vcString.at( d ).size();	}
	cchTotal += ((dCount+1) * sizeof(TCHAR));	//���s���{�]�T
	ptBufStr = (LPTSTR)malloc( cchTotal * sizeof(TCHAR) );	//	�T�C�Y�����
	if( ptBufStr )	//	�`�F�b�N
	{
		ZeroMemory( ptBufStr, cchTotal * sizeof(TCHAR) );
		for( d = 0; dCount > d; d++ )	//	�S���R�s�[
		{
			hRslt = StringCchCat( ptBufStr, cchTotal, vcString.at( d ).c_str() );
			hRslt = StringCchCat( ptBufStr, cchTotal, TEXT("\r\n") );
			//	���s�͏�ɕK�v�ł���
		}
	}

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (LPTSTR)ETC_MSG( err.what( ), NULL );	}
	catch( ... ){	return (LPTSTR)ETC_MSG( ("etc error"), NULL );	}
#endif

	return ptBufStr;
}
//-------------------------------------------------------------------------------------------------

/*!
	�m�[�e�B�t�@�C���b�Z�[�W�̏���
	@param[in]	hDlg		�_�C�����Q�n���h�D
	@param[in]	idFrom		NOTIFY�𔭐��������R���g���[���̂h�c
	@param[in]	pstNmhdr	NOTIFY�̏ڍ�
	@return		�����������e�Ƃ�
*/
INT_PTR Frm_OnNotify( HWND hDlg, INT idFrom, LPNMHDR pstNmhdr )
{
	INT	nmCode;
	TCHAR	atBuff[MIN_STRING];
	LPNMUPDOWN	pstUpDown;
	RECT	rect;

	nmCode = pstNmhdr->code;

	//	����������f���^�����A�E�ŕ�

	if( IDSP_LEFT_OFFSET == idFrom )
	{
		pstUpDown = (LPNMUPDOWN)pstNmhdr;
		if( UDN_DELTAPOS == nmCode )
		{
			TRACE( TEXT("%d %d"), pstUpDown->iPos, pstUpDown->iDelta );
			if( 0 < pstUpDown->iDelta ){		gstFrameInfo[gNowSel].dLeftOffset +=  1;	}
			else if( 0 >pstUpDown->iDelta ){	gstFrameInfo[gNowSel].dLeftOffset -=  1;	}

			StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), gstFrameInfo[gNowSel].dLeftOffset );
			Edit_SetText( GetDlgItem(hDlg,IDE_LEFT_OFFSET), atBuff );

			GetClientRect( GetDlgItem(hDlg,IDS_FRAME_IMAGE), &rect );
			FREE( gptFrmSample );
			gptFrmSample = FrameMakeOutsideBoundary( rect.right, rect.bottom, &(gstFrameInfo[gNowSel]) );

			InvalidateRect( GetDlgItem(hDlg,IDS_FRAME_IMAGE), NULL, TRUE );
		}
		return (INT_PTR)TRUE;
	}

	if( IDSP_RIGHT_OFFSET == idFrom )
	{
		pstUpDown = (LPNMUPDOWN)pstNmhdr;
		if( UDN_DELTAPOS == nmCode )
		{
			TRACE( TEXT("%d %d"), pstUpDown->iPos, pstUpDown->iDelta );
			if( 0 < pstUpDown->iDelta ){		gstFrameInfo[gNowSel].dRightOffset -= 1;	}
			else if( 0 > pstUpDown->iDelta ){	gstFrameInfo[gNowSel].dRightOffset += 1;	}

			StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), gstFrameInfo[gNowSel].dRightOffset );
			Edit_SetText( GetDlgItem(hDlg,IDE_RIGHT_OFFSET), atBuff );

			GetClientRect( GetDlgItem(hDlg,IDS_FRAME_IMAGE), &rect );
			FREE( gptFrmSample );
			gptFrmSample = FrameMakeOutsideBoundary( rect.right, rect.bottom, &(gstFrameInfo[gNowSel]) );

			InvalidateRect( GetDlgItem(hDlg,IDS_FRAME_IMAGE), NULL, TRUE );
		}
		return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	����̘g�f�[�^��INI�t�@�C�����烍�[�h���ă������Ɋm�ہE�h�b�g�����v�Z
	@param[in]	dNumber		�g�ԍ�
	@param[in]	pstFrame	�g�f�[�^�����\���̂ւ̃|�C���^�`
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FrameDataGet( UINT dNumber, LPFRAMEINFO pstFrame )
{
	InitFrameItem( INIT_LOAD, dNumber, pstFrame );

	pstFrame->stDaybreak.iLine  = DocStringInfoCount( pstFrame->stDaybreak.atParts,  0, &(pstFrame->stDaybreak.dDot), NULL );
	pstFrame->stMorning.iLine   = DocStringInfoCount( pstFrame->stMorning.atParts,   0, &(pstFrame->stMorning.dDot), NULL );
	pstFrame->stNoon.iLine      = DocStringInfoCount( pstFrame->stNoon.atParts,      0, &(pstFrame->stNoon.dDot), NULL );
	pstFrame->stAfternoon.iLine = DocStringInfoCount( pstFrame->stAfternoon.atParts, 0, &(pstFrame->stAfternoon.dDot), NULL );
	pstFrame->stNightfall.iLine = DocStringInfoCount( pstFrame->stNightfall.atParts, 0, &(pstFrame->stNightfall.dDot), NULL );
	pstFrame->stTwilight.iLine  = DocStringInfoCount( pstFrame->stTwilight.atParts,  0, &(pstFrame->stTwilight.dDot), NULL );
	pstFrame->stMidnight.iLine  = DocStringInfoCount( pstFrame->stMidnight.atParts,  0, &(pstFrame->stMidnight.dDot), NULL );
	pstFrame->stDawn.iLine      = DocStringInfoCount( pstFrame->stDawn.atParts,      0, &(pstFrame->stDawn.dDot), NULL );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�G�f�B�b�g�{�b�N�X�ɐݒ���e������
	@param[in]	hDlg	�_�C�����Q�n���h�D
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FrameInfoDisp( HWND hDlg )
{
	TCHAR	atBuff[MIN_STRING];

	//	���O�\��
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_NAME_EDIT), gstFrameInfo[gNowSel].atFrameName );

	//	�p�[�c���������
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_MORNING),   gstFrameInfo[gNowSel].stMorning.atParts );
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_NOON),      gstFrameInfo[gNowSel].stNoon.atParts );
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_AFTERNOON), gstFrameInfo[gNowSel].stAfternoon.atParts );
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_DAYBREAK),  gstFrameInfo[gNowSel].stDaybreak.atParts );
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_NIGHTFALL), gstFrameInfo[gNowSel].stNightfall.atParts );
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_TWILIGHT),  gstFrameInfo[gNowSel].stTwilight.atParts );
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_MIDNIGHT),  gstFrameInfo[gNowSel].stMidnight.atParts );
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_DAWN),      gstFrameInfo[gNowSel].stDawn.atParts );

	StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"),   gstFrameInfo[gNowSel].dLeftOffset );
	Edit_SetText( GetDlgItem(hDlg,IDE_LEFT_OFFSET),    atBuff );

	StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"),   gstFrameInfo[gNowSel].dRightOffset );
	Edit_SetText( GetDlgItem(hDlg,IDE_RIGHT_OFFSET),   atBuff );

	//	�V��Ə��̗]�蕔�����߂邩�ǂ���
	Button_SetCheck( GetDlgItem( hDlg, IDB_FRM_PADDING ), gstFrameInfo[gNowSel].dRestPadd ? BST_CHECKED : BST_UNCHECKED );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g������
	@param[in]	dMode	���ꂽ���g�̔ԍ��O�C���f�b�N�X
	@param[in]	dStyle	��`���ǂ���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocFrameInsert( INT dMode, INT dStyle )
{
	UINT_PTR	iLines;
	INT_PTR		iLns, iLast;
	INT			iTop, iBtm, iInX, iEndot, iPadding, i, baseDot;
	INT			xMidLen;
	LPTSTR		ptPadding;
	LPTSTR		ptString;

	FRAMEINFO	stInfo;

	INT			iMidLine, iUpLine, iDnLine;
	LPFRAMEITEM	pstItem;
	TCHAR		atSubStr[MAX_PATH];	//	����邩�H

#ifdef DO_TRY_CATCH
	try{
#endif

	FrameDataGet( dMode, &stInfo );

	iLines = DocNowFilePageLineCount( );	//	�y�[�W�S�̂̍s��

	//	�J�n�n�_����J�n	//	D_SQUARE
	iTop = gitFileIt->vcCont.at( gixFocusPage ).dSelLineTop;
	iBtm = gitFileIt->vcCont.at( gixFocusPage ).dSelLineBottom;
	if( 0 >  iTop ){	iTop = 0;	}
	if( 0 >  iBtm ){	iBtm = iLines - 1;	}

	//	���[���m�F�E���e���Ȃ��Ȃ�A�g�p�s�߂�
	iInX = DocLineParamGet( iBtm, NULL, NULL );
	if( 0 == iInX ){	 iBtm--;	}

	//	��`�I�𖳂��Ƃ݂Ȃ�

	ViewSelPageAll( -1 );	//	�͈͂Ƃ����̂ŉ������Ă���

	//	�I��͈͓��ł����Ƃ������h�b�g�����m�F
	baseDot = DocPageMaxDotGet( iTop, iBtm );

//�V��̍s���A���̕��A���̍s�����m��

	iMidLine = (iBtm - iTop) + 1;	//	�Ԃ̍s���m��

	xMidLen = baseDot;
	FrameMakeInsideBoundary( 0, &xMidLen, &stInfo );
	//	������Ԃ��m�肷��

	//	�V��p�[�c�쐬
	ptString = FrameMakeInsideBoundary( 1, &xMidLen, &stInfo );
	FrameMultiSizeGet( &stInfo, &iUpLine, NULL );	//	�V��̍s��
	iLns = iTop;	//	���ڍs
	iInX = 0;	//	�V��ǉ�
	DocInsertString( &iInX, &iLns, NULL, ptString, 0, TRUE );
	FREE( ptString );

	//	���ƉE����
	stInfo.stDaybreak.iNowLn  = 0;
	stInfo.stNightfall.iNowLn = 0;
	for( i = 0; iMidLine > i; i++, iLns++ )
	{
		//	��
		pstItem = &(stInfo.stDaybreak);
		//	���߂�̂̓p�[�c�ő�ʒu�܂�
		FrameMultiSubstring( pstItem->atParts, pstItem->iNowLn, atSubStr, MAX_PATH, pstItem->dDot );
		//	���̃p�f�B���O���Z�b�g
		if( -1 >= stInfo.dLeftOffset ){	StringWidthAdjust( -(stInfo.dLeftOffset), atSubStr, MAX_PATH, 0 );	}
		pstItem->iNowLn++;	//	��s�����[�v�����Ă���
		if( pstItem->iLine <= pstItem->iNowLn ){	pstItem->iNowLn = 0;	}
		iInX = 0;	//	���[���炢���
		DocInsertString( &iInX, &iLns, NULL, atSubStr, 0, FALSE );

		//	�E
		iEndot = DocLineParamGet( iLns, NULL, NULL );	//	���̍s�̖��[
		iPadding = xMidLen - iEndot;	//	���ߗʊm�F
		ptPadding = DocPaddingSpaceWithPeriod( iPadding, NULL, NULL, NULL, TRUE );//DocPaddingSpaceMake( iPadding );
		if( ptPadding )
		{
			DocInsertString( &iEndot, &iLns, NULL, ptPadding, 0, FALSE );
			FREE( ptPadding );
		}
		pstItem = &(stInfo.stNightfall);
		FrameMultiSubstring( pstItem->atParts, pstItem->iNowLn, atSubStr, MAX_PATH, 0 );
		pstItem->iNowLn++;	//	��s�����[�v�����Ă���
		if( pstItem->iLine <= pstItem->iNowLn ){	pstItem->iNowLn = 0;	}
		DocInsertString( &iEndot, &iLns, NULL, atSubStr, 0, FALSE );
	}

	//	�s�����d�n�e�Ȃ炱���ł������Ȏ��ɂȂ�
	iLast = DocPageParamGet( NULL, NULL );
	if( iLast <= iLns )
	{
		iLns = iLast - 1;
		iInX = DocLineParamGet( iLns, NULL, NULL );
		DocCrLfAdd( iInX , iLns, FALSE );	//	�������邽�߂ɉ��s
		iLns++;
	}

	//	�����
	ptString = FrameMakeInsideBoundary( 2, &xMidLen, &stInfo );
	FrameMultiSizeGet( &stInfo, NULL, &iDnLine );	//	���̍s��
	iInX = 0;	//	�V��ǉ�
	DocInsertString( &iInX, &iLns, NULL, ptString, 0, FALSE );
	FREE( ptString );


#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (HRESULT)ETC_MSG( err.what(), E_UNEXPECTED );	}
	catch( ... ){	return (HRESULT)ETC_MSG( ("etc error") , E_UNEXPECTED );	}
#endif
#ifdef DO_TRY_CATCH
	try{
#endif

	//	�ŏI�I�ȃL�����b�g�̈ʒu�����Z�b�g
	ViewPosResetCaret( iInX , iLns );

	ViewRedrawSetLine( iTop );
	DocBadSpaceCheck( iTop );	//	�o�b�h�󔒃`�F�L

	//	���s���Ă邩��A����ȍ~�S���ĕ`��K�v
	iLns = DocNowFilePageLineCount( );	//	���ݍs���ĔF��
	for( i = iTop; iLns > i; i++ )
	{
		DocBadSpaceCheck( i );	//	�o�b�h�󔒃`�F�L
		ViewRedrawSetLine(  i );
	}
	//ViewRedrawSetLine( i );	//	�O�̂���
	//DocBadSpaceCheck( i );	//	�o�b�h�󔒃`�F�L


	DocPageInfoRenew( -1, 1 );

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (HRESULT)ETC_MSG( err.what(), E_UNEXPECTED );	}
	catch( ... ){	return (HRESULT)ETC_MSG( ("etc error") , E_UNEXPECTED );	}
#endif
	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�����sFrame�́A�����E�����E�������������E0x0D0A�E���p�󔒂̑��ݕϊ�
	@param[in,out]	ptData	�ϊ����o�b�t�@�ŁA�ϊ��㕶��������BPARTS_CCH�T�C�Y�ł��邱��
	@param[in]		bMode	�P�F���������s�ɂ���@�O�F���s�������ɂ���
*/
VOID FrameDataTranslate( LPTSTR ptData, UINT bMode )
{
	TCHAR	atBuffer[SUB_STRING];
	UINT_PTR	i, j, cchLen;

	ZeroMemory( atBuffer, sizeof(atBuffer) );

	StringCchLength( ptData, PARTS_CCH, &cchLen );	//	�����m�F

	for( i = 0, j = 0; cchLen > i; i++, j++ )
	{
		if( 0x0000 == ptData[i] )	break;	//	�����Ӗ��͂Ȃ����ǈ��S�΍�

		if( bMode  )	//	���������s�ɂ���
		{
			if( 0x005C == ptData[i] )	//	�G�X�P�[�v�V�[�P���X
			{
				i++;	//	���̕������d�v
				if( 'n' == ptData[i] )
				{
					atBuffer[j++] = 0x000D;
					atBuffer[j] = 0x000A;
				}
				else if( 's' == ptData[i] )
				{
					atBuffer[j] = 0x0020;
				}
				else	//	�����ł�����
				{
					atBuffer[j] = ptData[i];
				}
			}
			else	//	�֌W�Ȃ��Ȃ炻�̂܂܃R�s�[���Ă���
			{
				atBuffer[j] = ptData[i];
			}
		}
		else	//	���s�������ɂ���
		{
			if( 0x005C == ptData[i] )	//	���L��
			{
				atBuffer[j++] = 0x005C;
				atBuffer[j] = 0x005C;	//	�d�˂�
			}
			else if( 0x000D == ptData[i] )	//	���s�͂�����
			{
				atBuffer[j++] = 0x005C;
				atBuffer[j] = TEXT('n');	//	�G�X�P�[�v�V�[�P���X
				i++;	//	���ɐi�߂�
			}
			else if( 0x0020 == ptData[i] )	//	���p�󔒂͂�����
			{
				atBuffer[j++] = 0x005C;
				atBuffer[j] = TEXT('s');	//	�G�X�P�[�v�V�[�P���X
			}
			else	//	�֌W�Ȃ��Ȃ炻�̂܂܃R�s�[���Ă���
			{
				atBuffer[j] = ptData[i];
			}
		}
	}

	StringCchCopy( ptData, PARTS_CCH, atBuffer );	//	�����߂�

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	���s���܂ޕ�������󂯎���āA�w��s�̓��e���o�b�t�@�ɓ����
	@param[in]	ptSrc	��������
	@param[in]	dLine	�؂�o���s�ԍ��E�O�C���f�b�N�X
	@param[out]	ptDest	�؂�o���������������o�b�t�@�ւ̃|�C���^�[
	@param[in]	cchSz	�o�b�t�@�̕������E�o�C�g�ɔ�
	@param[in]	iUseDot	�K�v�Ƃ���h�b�g���E����Ȃ��Ȃ�p�f�B���O�E�����Ȃ���u
	@return	UINT	�S�̂̍s��
*/
UINT FrameMultiSubstring( LPCTSTR ptSrc, CONST UINT dLine, LPTSTR ptDest, CONST UINT_PTR cchSz, CONST INT iUseDot )
{
	LPTSTR		ptPadding;
	INT			iPaDot, iStrDot;
	UINT_PTR	cchSrc, c, d;
	UINT		iLnCnt;

	StringCchLength( ptSrc, STRSAFE_MAX_CCH , &cchSrc );	//	��������̒����m�F

	ZeroMemory( ptDest, cchSz * sizeof(TCHAR) );	//	�Ƃ肠�����E�P����

	iLnCnt = 0;	d = 0;
	for( c = 0; cchSrc > c; c++ )
	{
		if( 0x000D == ptSrc[c] )	//	�������傤�͂�����
		{
			c++;	//	0x0A���΂�
			iLnCnt++;	//	�t�H�[�J�X�s��
		}
		else	//	���ʂ̕���
		{
			if( dLine == iLnCnt )	//	�s����v������
			{
				if( cchSz > d ){	ptDest[d] = ptSrc[c];	d++;	}
			}
		}
	}
	ptDest[(cchSz-1)] = NULL;	//	�k���^�[�~�l�[�^

//	StringCchLength( ptDest, cchSz, &cchSrc );	//	�u�b�R������������̒���
	iStrDot = ViewStringWidthGet( ptDest );	//	�u�b�R������������̃h�b�g��
	//	�p�f�B���O�������Ⴄ
	iPaDot = iUseDot - iStrDot;
	if( 1 <= iPaDot )
	{
		ptPadding = DocPaddingSpaceWithPeriod( iPaDot, NULL, NULL, NULL, TRUE );//DocPaddingSpaceMake( iPaDot );
		StringCchCat( ptDest, cchSz, ptPadding );
		FREE( ptPadding );
	}

	iLnCnt++;	//	�O�C���f�b�N�X�Ȃ̂łP���₷�̂�����
	return iLnCnt;
}
//-------------------------------------------------------------------------------------------------

//�}���E�C���h�E�ɂ���

/*!
	�}���p�E�C���h�E���
	@param[in]	hInst	�����n���h��
	@param[in]	hPrWnd	���C���̃E�C���h�E�n���h��
*/
HWND FrameInsBoxCreate( HINSTANCE hInst, HWND hPrWnd )
{
	INT			x, y;
	UINT		d;
	TCHAR		atBuffer[MAX_STRING];
	RECT		rect, vwRect;
//	TBADDBITMAP	stToolBmp;

	RECT	stFrmRct;
	INT		topOst;

	if( ghFrInbxWnd )
	{
		SetForegroundWindow( ghFrInbxWnd );
		return ghFrInbxWnd;
	}

	//	�{�̃E�C���h�E
	ghFrInbxWnd = CreateWindowEx( WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		FRAMEINSERTBOX_CLASS, TEXT("�g�}���{�b�N�X"),
		WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU,
		0, 0, FIB_WIDTH, FIB_HEIGHT, NULL, NULL, hInst, NULL );
	SetLayeredWindowAttributes( ghFrInbxWnd, TRANCE_COLOUR, 0xFF, LWA_COLORKEY );
										//	TRANCE_COLOUR

	//	�c�[���o�[
	ghFIBtlbrWnd = CreateWindowEx( WS_EX_CLIENTEDGE, TOOLBARCLASSNAME, TEXT("fibtoolbar"),
		WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TOOLTIPS,
		0, 0, 0, 0, ghFrInbxWnd, (HMENU)IDTB_FRMINSBOX_TOOLBAR, hInst, NULL );

	if( 0 == gdToolBarHei )	//	���l���擾�Ȃ�
	{
		GetWindowRect( ghFIBtlbrWnd, &rect );
		gdToolBarHei = rect.bottom - rect.top;

		gstFrmSz.x = 0;
		gstFrmSz.y = gdToolBarHei;
		ClientToScreen( ghFrInbxWnd, &gstFrmSz );
	}

	//	�����c�[���`�b�v�X�^�C����ǉ�
	SendMessage( ghFIBtlbrWnd, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS );

	//	�A�C�R��
	SendMessage( ghFIBtlbrWnd, TB_SETIMAGELIST, 0, (LPARAM)ghFrameImgLst );
	SendMessage( ghFIBtlbrWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(16,16) );
	SendMessage( ghFIBtlbrWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0 );

	//	�c�[���`�b�v�������ݒ�E�{�^���e�L�X�g���c�[���`�b�v�ɂȂ�
	StringCchCopy( atBuffer, MAX_STRING, TEXT("�}������") );
	gstFIBTBInfo[0].iString = SendMessage( ghFIBtlbrWnd, TB_ADDSTRING, 0, (LPARAM)atBuffer );
	for( d = 0; FRAME_MAX > d; d++ )
	{
		FrameNameLoad( d, atBuffer, MAX_STRING );
		gstFIBTBInfo[d+2].iString = SendMessage( ghFIBtlbrWnd, TB_ADDSTRING, 0, (LPARAM)atBuffer );
	}
	StringCchCopy( atBuffer, MAX_STRING, TEXT("�}�����������") );
	gstFIBTBInfo[23].iString = SendMessage( ghFIBtlbrWnd, TB_ADDSTRING, 0, (LPARAM)atBuffer );
	StringCchCopy( atBuffer, MAX_STRING, TEXT("�㉺�̒[���𖄂߂�\r\n�i�����ł̕ύX�͕ۑ�����܂���j") );
	gstFIBTBInfo[25].iString = SendMessage( ghFIBtlbrWnd, TB_ADDSTRING, 0, (LPARAM)atBuffer );


	SendMessage( ghFIBtlbrWnd , TB_ADDBUTTONS, (WPARAM)TB_ITEMS, (LPARAM)&gstFIBTBInfo );	//	�c�[���o�[�Ƀ{�^����}��

	SendMessage( ghFIBtlbrWnd , TB_AUTOSIZE, 0, 0 );	//	�{�^���̃T�C�Y�ɍ��킹�ăc�[���o�[�����T�C�Y
	InvalidateRect( ghFIBtlbrWnd , NULL, TRUE );		//	�N���C�A���g�S�̂��ĕ`�悷�閽��

	//	������ԂƂ��ăA���t�@���`�F�L��Ԃ�
	SendMessage( ghFIBtlbrWnd, TB_CHECKBUTTON, IDM_INSFRAME_ALPHA, TRUE );
	gdSelect = 0;

	//	�������邩�ǂ���
	SendMessage( ghFIBtlbrWnd, TB_CHECKBUTTON, IDM_FRMINSBOX_QCLOSE, gbQuickClose );

	FrameDataGet( gdSelect , &gstNowFrameInfo );	//	�g�p�[�c���m��

	//	���߂邩�ǂ���
	SendMessage( ghFIBtlbrWnd, TB_CHECKBUTTON, IDM_FRMINSBOX_PADDING, gstNowFrameInfo.dRestPadd );
	gbMultiPaddTemp = gstNowFrameInfo.dRestPadd;	//	

	topOst = FrameInsBoxSizeGet( &stFrmRct );	//	FRAME���Ă͂ߘg�̃T�C�Y
	gptFrmBox = FrameMakeOutsideBoundary( stFrmRct.right, stFrmRct.bottom, &gstNowFrameInfo );

	//	�E�C���h�E�ʒu���m�肳����
	GetWindowRect( ghViewWnd, &vwRect );
	gstViewOrigin.x = vwRect.left;//�ʒu�L�^�E���������ς����̂���Ȃ�
	gstViewOrigin.y = vwRect.top;
	x = (vwRect.left + LINENUM_WID) - gstFrmSz.x;
	y = (vwRect.top  + RULER_AREA)  - gstFrmSz.y;
	TRACE( TEXT("Frame %d x %d"), x, y );

#ifdef _DEBUG
	SetWindowPos( ghFrInbxWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW );
#else
	SetWindowPos( ghFrInbxWnd, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW );
#endif
	gstOffset.x = x - vwRect.left;
	gstOffset.y = y - vwRect.top;


	return ghFrInbxWnd;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t���[���T�C�Y���m��
	@param[out]	pstRect	�t���[���T�C�Y������o�b�t�@
	@return		�����I�t�Z�b�g�i�c�[���o�[�̍����j
*/
INT FrameInsBoxSizeGet( LPRECT pstRect )
{
	RECT	rect;

	GetClientRect( ghFrInbxWnd, &rect );

	//	�N���C�����g��������A�c�[���o�[�������k��
	rect.bottom -= gdToolBarHei;

	*pstRect = rect;

	return gdToolBarHei;
}
//-------------------------------------------------------------------------------------------------

/*!
	�}�����s
	@param[in]	hWnd	�E�C���h�E�n���h��
*/
HRESULT FrameInsBoxDoInsert( HWND hWnd )
{
	INT			iX, iY;
	HWND		hLyrWnd;
	RECT		rect;

	//	�}�������ɂ́A���C���{�b�N�X���\�������Ŏg��
	hLyrWnd = LayerBoxVisibalise( GetModuleHandle(NULL), gptFrmBox, 0x10 );

	//	���C���̈ʒu��ύX
	GetWindowRect( hWnd, &rect );
	LayerBoxPositionChange( hLyrWnd, (rect.left + gstFrmSz.x), (rect.top + gstFrmSz.y) );
	//	�󔒂�S�����ߎw��ɂ���
	LayerTransparentToggle( hLyrWnd, 1 );
	//	�㏑������
	LayerContentsImportable( hLyrWnd, IDM_LYB_OVERRIDE, &iX, &iY, D_INVISI );
	ViewPosResetCaret( iX, iY );	
	//	�I����������
	DestroyWindow( hLyrWnd );

	if( gbQuickClose )	DestroyWindow( hWnd );

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
LRESULT CALLBACK FrameInsProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		HANDLE_MSG( hWnd, WM_PAINT,		Fib_OnPaint );		//	��ʂ̍X�V�Ƃ�
		HANDLE_MSG( hWnd, WM_KEYDOWN,	Fib_OnKey );
		HANDLE_MSG( hWnd, WM_COMMAND,	Fib_OnCommand );	
		HANDLE_MSG( hWnd, WM_DESTROY,	Fib_OnDestroy );	//	�I�����̏���
		HANDLE_MSG( hWnd, WM_WINDOWPOSCHANGING, Fib_OnWindowPosChanging );
		HANDLE_MSG( hWnd, WM_WINDOWPOSCHANGED,  Fib_OnWindowPosChanged );

		case WM_MOVING:	Fib_OnMoving( hWnd, (LPRECT)lParam );	return 0;

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
VOID Fib_OnCommand( HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify )
{
	RECT	stFrmRct;
	INT		topOst, iRslt = -1;

	switch( id )
	{
		case IDM_FRAME_INS_DECIDE:	FrameInsBoxDoInsert( hWnd );	return;

		case IDM_INSFRAME_ALPHA:	gdSelect = 0;	break;
		case IDM_INSFRAME_BRAVO:	gdSelect = 1;	break;
		case IDM_INSFRAME_CHARLIE:	gdSelect = 2;	break;
		case IDM_INSFRAME_DELTA:	gdSelect = 3;	break;
		case IDM_INSFRAME_ECHO:		gdSelect = 4;	break;
		case IDM_INSFRAME_FOXTROT:	gdSelect = 5;	break;
		case IDM_INSFRAME_GOLF:		gdSelect = 6;	break;
		case IDM_INSFRAME_HOTEL:	gdSelect = 7;	break;
		case IDM_INSFRAME_INDIA:	gdSelect = 8;	break;
		case IDM_INSFRAME_JULIETTE:	gdSelect = 9;	break;

		case IDM_INSFRAME_KILO:		gdSelect = 10;	break;
		case IDM_INSFRAME_LIMA:		gdSelect = 11;	break;
		case IDM_INSFRAME_MIKE:		gdSelect = 12;	break;
		case IDM_INSFRAME_NOVEMBER:	gdSelect = 13;	break;
		case IDM_INSFRAME_OSCAR:	gdSelect = 14;	break;
		case IDM_INSFRAME_PAPA:		gdSelect = 15;	break;
		case IDM_INSFRAME_QUEBEC:	gdSelect = 16;	break;
		case IDM_INSFRAME_ROMEO:	gdSelect = 17;	break;
		case IDM_INSFRAME_SIERRA:	gdSelect = 18;	break;
		case IDM_INSFRAME_TANGO:	gdSelect = 19;	break;

		case IDM_FRMINSBOX_QCLOSE:	gbQuickClose = SendMessage( ghFIBtlbrWnd, TB_ISBUTTONCHECKED, IDM_FRMINSBOX_QCLOSE, 0 );	return;

		case IDM_FRMINSBOX_PADDING:	iRslt = SendMessage( ghFIBtlbrWnd, TB_ISBUTTONCHECKED, IDM_FRMINSBOX_PADDING, 0 );	break;

		default:	return;
	}

	FrameDataGet( gdSelect, &gstNowFrameInfo );	//	�g�p�[�c���m��

	if( 0 <= iRslt ){	gstNowFrameInfo.dRestPadd = iRslt;	gbMultiPaddTemp = iRslt;	}
	else
	{
		gbMultiPaddTemp = gstNowFrameInfo.dRestPadd;
		SendMessage( ghFIBtlbrWnd, TB_CHECKBUTTON, IDM_FRMINSBOX_PADDING, gstNowFrameInfo.dRestPadd );
	}

	topOst = FrameInsBoxSizeGet( &stFrmRct );	//	FRAME���Ă͂ߘg�̃T�C�Y
	FREE( gptFrmBox );
	gptFrmBox = FrameMakeOutsideBoundary( stFrmRct.right, stFrmRct.bottom, &gstNowFrameInfo );

	InvalidateRect( hWnd, NULL, TRUE );

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
VOID Fib_OnKey( HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags )
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
	Fib_OnMoving( hWnd, &rect );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	PAINT�B�����̈悪�o�����Ƃ��ɔ����B�w�i�̈����ɒ��ӁB�w�i��h��Ԃ��Ă���A�I�u�W�F�N�g��`��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
*/
VOID Fib_OnPaint( HWND hWnd )
{
	PAINTSTRUCT	ps;
	HDC			hdc;

	RECT	rect;

	GetClientRect( hWnd, &rect );

	hdc = BeginPaint( hWnd, &ps );

	FillRect( hdc, &rect, ghBgBrush );

	//	������ĕ`��
	FrameInsBoxFrmDraw( hdc );

	EndPaint( hWnd, &ps );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�`��
	@param[in]	hDC	�`�悷��f�o�C�X�R���e�L�X�g
*/
VOID FrameInsBoxFrmDraw( HDC hDC )
{
	//UINT_PTR	cchSize;
	HFONT	hOldFnt;
	INT		topOst, iYpos;
	RECT	stFrmRct;

	UINT	dLines, d;
//	LPTSTR	ptMultiStr;
	TCHAR	atBuffer[MAX_PATH];

//	SetBkMode( hDC, OPAQUE );
	SetBkColor( hDC, ViewBackColourGet( NULL ) );	//	

	hOldFnt = SelectFont( hDC, ghAaFont );	//	�t�H���g��������

	topOst = FrameInsBoxSizeGet( &stFrmRct );	//	FRAME���Ă͂ߘg�̃T�C�Y

//	ptMultiStr = FrameMakeOutsideBoundary( stFrmRct.right, stFrmRct.bottom, &gstNowFrameInfo );

	dLines = DocStringInfoCount( gptFrmBox, 0, NULL, NULL );	//	�s���m��
							//	ptMultiStr

//	stFrmRct.top = topOst;
//	stFrmRct.bottom += topOst;
//	DrawText( hDC, ptMultiStr, -1, &stFrmRct, DT_LEFT | DT_NOPREFIX | DT_NOCLIP | DT_WORDBREAK );

	iYpos = topOst;	//
	for( d = 0; dLines > d; d++ )
	{					//	ptMultiStr
		FrameMultiSubstring( gptFrmBox, d, atBuffer, MAX_PATH, 0 );
		FrameDrawItem( hDC, iYpos, atBuffer );
		iYpos += LINE_HEIGHT;
	}

//	FREE( ptMultiStr );

	SelectFont( hDC , hOldFnt );	//	�t�H���g�߂�

	return;
}
//-This way-----------------------------------------------------------------------------------------------

/*!
	��s���̕`��
	@param[in]	hDC		�`�悷��f�o�C�X�R���e�L�X�g
	@param[in]	iY		�`�悷�鍂���i���͏�ɂO�ł�낵�j
	@param[in]	ptLine	��s���̃f�[�^
*/
VOID FrameDrawItem( HDC hDC, INT iY, LPTSTR ptLine )
{
	UINT_PTR	cchSize, cl;
	UINT		iX, caret, len;
	INT			mRslt, mBase;
	LPTSTR		ptBgn;
	SIZE		stSize;

	StringCchLength( ptLine, STRSAFE_MAX_CCH, &cchSize );

	caret = 0;
	iX = 0;
	for( cl = 0; cchSize > cl; )
	{
		mRslt = iswspace(  ptLine[cl] );	//	�J�n�ʒu�̕����^�C�v�m�F
		ptBgn = &(ptLine[cl]);

		for( len= 0; cchSize > cl; len++, cl++ )
		{
			mBase = iswspace(  ptBgn[len] );	//	�����^�C�v���m�F���Ă���
			if( mRslt != mBase ){	break;	}	//	�^�C�v���ς������
		}
		GetTextExtentPoint32( hDC, ptBgn, len, &stSize );	//	�h�b�g���m�F

		if( mRslt ){	SetBkMode( hDC, TRANSPARENT );	}
		else{	SetBkMode( hDC, OPAQUE );	}

		ExtTextOut( hDC, iX, iY, 0, NULL, ptBgn, len, NULL );

		iX += stSize.cx;
	}

	return;
}
//-First Comes Rock-----------------------------------------------------------------------------------------------


/*!
	�E�C���h�E�����Ƃ��ɔ����B�f�o�C�X�R���e�L�X�g�Ƃ��m�ۂ�����ʍ\���̃������Ƃ����I���B
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Fib_OnDestroy( HWND hWnd )
{
	FREE( gptFrmBox );

	MainStatusBarSetText( SB_LAYER, TEXT("") );

	ghFrInbxWnd = NULL;

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	��������Ă���Ƃ��ɔ����E�}�E�X�ŃE�C���h�E�h���b�O���Ƃ�
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	pstPos	���̏u�Ԃ̃X�N���[�����W
*/
VOID Fib_OnMoving( HWND hWnd, LPRECT pstPos )
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

	StringCchPrintf( atBuffer, SUB_STRING, TEXT("Frame %d[dot] %d[line]"), xSb, dLine );
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
BOOL Fib_OnWindowPosChanging( HWND hWnd, LPWINDOWPOS pstWpos )
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
VOID Fib_OnWindowPosChanged( HWND hWnd, const LPWINDOWPOS pstWpos )
{
	RECT	vwRect;
	RECT	stFrmRct;
	INT		topOst;

	MoveWindow( ghFIBtlbrWnd, 0, 0, 0, 0, TRUE );	//	�c�[���o�[�͐��l�Ȃ��Ă�����ɍ��킹�Ă����

	FrameDataGet( gdSelect, &gstNowFrameInfo );	//	�g�p�[�c���m��
	gstNowFrameInfo.dRestPadd = gbMultiPaddTemp;	//	�ꎞ�ݒ�

	topOst = FrameInsBoxSizeGet( &stFrmRct );	//	FRAME���Ă͂ߘg�̃T�C�Y
	FREE( gptFrmBox );
	gptFrmBox = FrameMakeOutsideBoundary( stFrmRct.right, stFrmRct.bottom, &gstNowFrameInfo );

	InvalidateRect( hWnd, NULL, TRUE );

	//	�ړ����Ȃ������Ƃ��͉������Ȃ��ł���
	if( SWP_NOMOVE & pstWpos->flags )	return;

	GetWindowRect( ghViewWnd, &vwRect );
	gstViewOrigin.x = vwRect.left;//�ʒu�L�^�E���������ς����̂���Ȃ�
	gstViewOrigin.y = vwRect.top;

	gstOffset.x = pstWpos->x - vwRect.left;
	gstOffset.y = pstWpos->y - vwRect.top;

	return;
}
//-------------------------------------------------------------------------------------------------


/*!
	�r���[���ړ�����
	@param[in]	hWnd	�{�̃E�C���h�E�n���h���E���܂�Ӗ��͂Ȃ�
	@param[in]	state	����ԁE�ŏ����Ȃ�Ⴄ�R�g����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FrameMoveFromView( HWND hWnd, UINT state )
{
	RECT	vwRect = {0,0,0,0};
	POINT	lyPoint;

	if( !(ghFrInbxWnd) )	return S_FALSE;

	//	�ŏ������͔�\���ɂ���Ƃ�	SIZE_MINIMIZED

	if( SIZE_MINIMIZED != state )
	{
		GetWindowRect( ghViewWnd, &vwRect );
		gstViewOrigin.x = vwRect.left;//�ʒu�L�^
		gstViewOrigin.y = vwRect.top;
	}

	if( SIZE_MINIMIZED == state )
	{
		ShowWindow( ghFrInbxWnd, SW_HIDE );
	}
	else
	{
		lyPoint.x = gstOffset.x + vwRect.left;
		lyPoint.y = gstOffset.y + vwRect.top;
#ifdef _DEBUG
		SetWindowPos( ghFrInbxWnd, HWND_TOP, lyPoint.x, lyPoint.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW );
#else
		SetWindowPos( ghFrInbxWnd, HWND_TOPMOST, lyPoint.x, lyPoint.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW );
#endif
	}


	return S_OK;
}
//-------------------------------------------------------------------------------------------------


