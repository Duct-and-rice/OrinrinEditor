/*! @file
	@brief �u���E�U�R���|�[�l���g�g����Preview�����܂�
	���̃t�@�C���� DocPreview.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/09/01
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
#ifdef ENABLE_PREVIEW
#include "stdafx.h"
#include "OrinrinEditor.h"
//-------------------------------------------------------------------------------------------------

/*
IE�R���|�����ɂ�ATL���v��
ttp://ysmt.blog21.fc2.com/blog-entry-244.html

VCExpress�ɂ�ATL�����ĂȂ��̂ŁA�ʌ��ŃQ�b�g����EVCPro�Ƃ��ɂ͓����Ă邩��

WindowsDriverKit710�ɓ����Ă�
������
ttp://www.microsoft.com/japan/whdc/DevTools/WDK/WDKpkg.mspx
�_�E�\
ttp://www.microsoft.com/download/en/details.aspx?displaylang=en&id=11800


�C���N���[�h�t�@�C���p�X�ɁA"(DDK�t�H���_)\inc\atl71"
���C�u�����t�@�C���p�X�ɁA"(DDK�t�H���_)\lib\atl\i386"��ǉ��B

���O��ATL71�����ǃo�[�W������80

�����N�ŃG���[���o��Ȃ�A
�����J�̒ǉ��̈ˑ��t�@�C���ɁAatlthunk.lib��t����B

*/

#pragma warning( disable : 4995 )	//	ATL���̃C�P�i�C�����x���𒆎~�I

#include <atlbase.h>

#ifndef _ATL
#error IE�R���|�����ɂ�ATL���v��B
#endif

//	DDK��ATL�g���Ȃ�K�v�Ȃ悤��
//#pragma comment(lib, "atlthunk.lib")


//	ATL���g�p���邽�߂ɕK�v�ȃO���[�o���ϐ�
//	���̕ϐ����́A�K���A���̖��O�ɂ��邱�ƁB
extern CComModule	_Module;

#include <atlcom.h>
#include <atlhost.h>

#pragma warning( default : 4995 )	//	�x�����A

//#if _MSC_VER >=1500 // VC2008(VC9.0)�ȍ~

//	DDK�Ȃ�AAtlAxWin80 ���ۂ�


//#if _ATL_VER == 14
#define ATL_AX_WIN	TEXT("AtlAxWin80")
//#else
//#error ATL�o�[�W�����ɍ��킹�Ē�`����K�v������ TEXT(_ATL_VER)
//#endif

//	ATLAXWIN_CLASS	��`����Ă邯��TEXT�ɂȂ��ĂȂ�

CComModule	_Module;

//	�Ȃ񂩗v��݂���
BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP( )

//-------------------------------------------------------------------------------------------------

#define REDRAW_ATSCROLL

#define DOC_PREVIEW_CLASS	TEXT("PREVIEW_CLASS")	//!<	�v���r���[���̃N���X��

#define PVW_WIDTH	 820	//!<	�f�t�H���g��ʕ�
#define PVW_HEIGHT	 480	//!<	�f�t�H���g��ʍ���
//-------------------------------------------------------------------------------------------------

static CComQIPtr<IHTMLDocument2>	gpDocument2;	//!<	�h�d�R���|�[�l���g�̂Ȃɂ�
static CComQIPtr<IWebBrowser2>		gpWebBrowser2;	//!<	�h�d�R���|�[�l���g�̃i�j�J
static  HWND	ghIEwnd;							//!<	�h�d�R���|�[�l���g�̃n���h��



static  HWND	ghPrevWnd;			//!<	���̃E�C���h�E�̃n���h��
static HINSTANCE	ghInst;			//!<	���݂̃C���^�[�t�F�C�X

static  HWND	ghToolWnd;			//!<	�c�[���o�[
static HIMAGELIST ghPrevwImgLst;	//!<	�c�[���o�[�A�C�R���̃C���[�W���X�g

static INT		giViewMode;			//!<	�\����ԁF�O�`�P�̕Ł@�|�P�S��

extern  HWND	ghPrntWnd;				//	�e�E�C���h�E�n���h��
extern list<ONEFILE>	gltMultiFiles;	//	�����t�@�C���ێ�
extern FILES_ITR	gitFileIt;			//	�����Ă�t�@�C���̖{��
extern INT		gixFocusPage;			//	���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X
//-------------------------------------------------------------------------------------------------

#define TB_ITEMS	1
static  TBBUTTON	gstTBInfo[] = {
	{ 0, IDM_PVW_ALLVW, TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE | TBSTYLE_CHECK, {0, 0}, 0, 0 }
};	//	


#define PREV_HEADER_FILE	TEXT("Preview.htm")
#define SEPARATE_TAG	("[SEPARATE]")

static CONST CHAR	gcacWeek[7][4] = { ("��"), ("��"), ("��"), ("��"), ("��"), ("��"), ("�y") };

static  CHAR	gacResHeaderFmt[] = { "<dt>%d ���O�F�������̗d������[sage] ���e���F%d/%02d/%02d(%s) %02d:%02d:%02d ID:OrinEdit99</dt> <dd>" };	//	
static  CHAR	gacResFooterFmt[] = { "<br></dd>\r\n" };


static LPSTR	gpcHtmlHdr;
static LPSTR	gpcHtmlFtr;
//-------------------------------------------------------------------------------------------------


LRESULT	CALLBACK PreviewWndProc( HWND, UINT, WPARAM, LPARAM );	//!<	
VOID	Pvw_OnCommand( HWND , INT, HWND, UINT );	//!<	WM_COMMAND �̏���
VOID	Pvw_OnSize( HWND , UINT, INT, INT );		//!<	
VOID	Pvw_OnPaint( HWND );						//!<	WM_PAINT �̏����E�g���`��Ƃ�
VOID	Pvw_OnDestroy( HWND );						//!<	WM_DESTROY �̏����EBRUSH�Ƃ��̃I�u�W�F�N�g�̔j���Y��Ȃ��悤��

HRESULT	PreviewHeaderGet( VOID );	//!<	

HRESULT	PreviewPageWrite( INT );	//!<	
//-------------------------------------------------------------------------------------------------

/*!
	�v���r���[�E�C���h�E�N���X���쐬
	@param[in]	hInstance	�A�v���̃C���X�^���X
	@param[in]	hParentWnd	�e�E�C���h�E�̃n���h��
	@return	�Ȃ�
*/
VOID PreviewInitialise( HINSTANCE hInstance, HWND hParentWnd )
{
	WNDCLASSEX	wcex;
	GUID	guid;

	HBITMAP	hImg, hMsq;

	if( hInstance )
	{
		ghInst = hInstance;

		_Module.Init( ObjectMap, hInstance, &guid );

		//	��p�̃E�C���h�E�N���X�쐬
		ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
		wcex.cbSize			= sizeof(WNDCLASSEX);
		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= PreviewWndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIcon			= NULL;
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= DOC_PREVIEW_CLASS;
		wcex.hIconSm		= NULL;

		RegisterClassEx( &wcex );

		ghPrevWnd = NULL;

		PreviewHeaderGet(  );

		CoInitialize( NULL );

		giViewMode = 0;

		ghPrevwImgLst = ImageList_Create( 16, 16, ILC_COLOR24 | ILC_MASK, 1, 1 );
		hImg = LoadBitmap( ghInst, MAKEINTRESOURCE( (IDBMP_PREVIEW_ALL) ) );
		hMsq = LoadBitmap( ghInst, MAKEINTRESOURCE( (IDBMQ_PREVIEW_ALL) ) );
		ImageList_Add( ghPrevwImgLst , hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ�
		DeleteBitmap( hImg );	DeleteBitmap( hMsq );
	}
	else
	{
		//	�����J�����܂܂��������Y��ɕ��Ȃ��ƃ��������[�N
		if( ghPrevWnd ){	SendMessage( ghPrevWnd, WM_CLOSE, 0, 0 );	}

		CoUninitialize( );

		FREE(gpcHtmlHdr);

		ImageList_Destroy( ghPrevwImgLst );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�e���v���f�B���N�g���ɓ����Ă�HeaderFooter�t�@�C���̒��g���Q�b�g����
*/
HRESULT PreviewHeaderGet( VOID )
{
	TCHAR	atPrevFile[MAX_PATH];

	HANDLE	hFile;
	DWORD	readed;

	INT		iByteSize;
	LPSTR	pcText, pcNext;


	gpcHtmlHdr = NULL;
	gpcHtmlFtr = NULL;

	//	�w�b�_�E�t�b�^���m��
	StringCchCopy( atPrevFile, MAX_PATH, ExePathGet() );
	PathAppend( atPrevFile, TEMPLATE_DIR );
	PathAppend( atPrevFile, PREV_HEADER_FILE );

	hFile = CreateFile( atPrevFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE == hFile ){	return E_HANDLE;	}

	iByteSize = GetFileSize( hFile, NULL );
	pcText = (LPSTR)malloc( iByteSize + 2 );
	ZeroMemory( pcText, iByteSize + 2 );

	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
	ReadFile( hFile, pcText, iByteSize, &readed, NULL );
	CloseHandle( hFile );	//	���e�S����荞�񂾂���J��

	gpcHtmlHdr = pcText;	//	HEADER�����Efree�ɂ�����

	pcNext = StrStrA( pcText, SEPARATE_TAG );
	pcNext[0] = NULL;	//	HEADER��FOOTER�̋��E�Ƀk���^�[�~�l�[�^
	pcNext++;
	gpcHtmlFtr = NextLineA( pcNext );	//	���̍s���炪�{��



	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*�I
	�E�C���h�E���쐬
	@param[in]	iNowPage	�v���r���肽���Ŕԍ�
	@param[in]	bForeg		��O�Ȃ�ăv���r���[�̂Ƃ��Ƀt�H�A�O�����h�ɂ���E�O�Ȃ炵�Ȃ��E�v���r���[�J���ĂȂ��Ȃ牽�����Ȃ�
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT PreviewVisibalise( INT iNowPage, BOOLEAN bForeg )
{
	HWND	hWnd;
	TCHAR	atBuffer[MAX_STRING];
//	UINT	iIndex;
	RECT	rect;
	RECT	tbRect;

	HRESULT	hRslt = S_OK;

	CComPtr<IUnknown>	comPunkIE;	
	CComPtr<IDispatch>	pDispatch;
	CComVariant	vEmpty;
	CComVariant	vUrl( TEXT("about:blank") );

#ifdef REDRAW_ATSCROLL
	LONG	height, offhei, scrtop;
//	CComQIPtr<IHTMLWindow2>		pWindow2;
	CComQIPtr<IHTMLElement>		pElement;
	CComQIPtr<IHTMLElement2>	pElement2;
#endif

	if( ghPrevWnd )	//	�߂�Preview���L������
	{
	//	SendMessage( ghToolWnd, TB_CHECKBUTTON, IDM_PVW_ALLVW, FALSE );

#pragma message ("�S�v���������ɁA�X�N���[���o�[�̈ʒu�o���Ă����āA�����܂�Scroll������H")

#ifdef REDRAW_ATSCROLL
		gpWebBrowser2->get_Height( &height );	//	�����R���|�[�l���g�̍���

		gpDocument2->get_body( &pElement );
		pElement.QueryInterface( &pElement2 );
		pElement.Release(  );

		pElement2->get_scrollHeight( &offhei );	//	�S�̂̍���
		pElement2->get_scrollTop( &scrtop );	//	�\���ʒu�̃X�N���[����
		pElement2.Release(  );

#endif

		//	���e��������
		if( 0 > giViewMode ){	PreviewPageWrite(  -1 );	}
		else{	PreviewPageWrite( iNowPage );	}

		gpWebBrowser2->Refresh(  );

#ifdef REDRAW_ATSCROLL
		gpDocument2->get_body( &pElement );
		pElement.QueryInterface( &pElement2 );
		pElement.Release(  );

		pElement2->get_scrollHeight( &offhei );	//	�S�̂̍���

		pElement2->put_scrollTop(  scrtop );	//	�\���ʒu�̃X�N���[����
		pElement2.Release(  );
#endif

		InvalidateRect( ghPrevWnd, NULL, TRUE );

		if( bForeg )	SetForegroundWindow( ghPrevWnd );

		return S_FALSE;
	}

	//	�v���r���[�J���ĂȂ��Ƃ��ɁA��t�H�A�O�����h�Ȃ牽�����Ȃ�
	if( !(bForeg) ){	return  E_ABORT;	}


	InitWindowPos( INIT_LOAD, WDP_PREVIEW, &rect );
	if( 0 >= rect.right || 0 >= rect.bottom )	//	���������O�̓f�[�^����
	{
		hWnd = GetDesktopWindow( );
		GetWindowRect( hWnd, &rect );
		rect.left   = ( rect.right  - PVW_WIDTH ) / 2;
		rect.top    = ( rect.bottom - PVW_HEIGHT ) / 2;
		rect.right  = PVW_WIDTH;
		rect.bottom = PVW_HEIGHT;
		InitWindowPos( INIT_SAVE , WDP_PREVIEW, &rect );	//	�N�����ۑ�
	}


	ghPrevWnd = CreateWindowEx( WS_EX_TOOLWINDOW, DOC_PREVIEW_CLASS, TEXT("IE�R���|�[�l���g�ɂ��v���r���["),
		WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_VISIBLE | WS_SYSMENU,
		rect.left, rect.top, rect.right, rect.bottom, NULL, NULL, ghInst, NULL );

	//�c�[���o�[���
	ghToolWnd = CreateWindowEx( 0, TOOLBARCLASSNAME, TEXT("toolbar"),
		WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TOOLTIPS | CCS_NODIVIDER,
		0, 0, 0, 0, ghPrevWnd, (HMENU)IDW_PVW_TOOL_BAR, ghInst, NULL );

	//	�����c�[���`�b�v�X�^�C����ǉ�
	SendMessage( ghToolWnd, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS );

	SendMessage( ghToolWnd, TB_SETIMAGELIST, 0, (LPARAM)ghPrevwImgLst );

	SendMessage( ghToolWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(16,16) );

	SendMessage( ghToolWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0 );
	//	�c�[���`�b�v�������ݒ�E�{�^���e�L�X�g���c�[���`�b�v�ɂȂ�
	StringCchCopy( atBuffer, MAX_STRING, TEXT("�S�v���r���[�X�^�C��") );	gstTBInfo[0].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuffer );

	SendMessage( ghToolWnd , TB_ADDBUTTONS, (WPARAM)TB_ITEMS, (LPARAM)&gstTBInfo );	//	�c�[���o�[�Ƀ{�^����}��

	SendMessage( ghToolWnd , TB_AUTOSIZE, 0, 0 );	//	�{�^���̃T�C�Y�ɍ��킹�ăc�[���o�[�����T�C�Y
	InvalidateRect( ghToolWnd , NULL, TRUE );		//	�N���C�A���g�S�̂��ĕ`�悷�閽��

	//	�c�[���o�[�T�u�N���X�����K�v

	GetWindowRect( ghToolWnd, &tbRect );
	tbRect.right  -= tbRect.left;
	tbRect.bottom -= tbRect.top;
	tbRect.left = 0;
	tbRect.top  = 0;

	GetClientRect( ghPrevWnd, &rect );
	rect.top     = tbRect.bottom;
	rect.bottom -= tbRect.bottom;

	AtlAxWinInit(  );

	ghIEwnd = CreateWindowEx( 0, ATL_AX_WIN, TEXT("Shell.Explorer.2"),
		WS_CHILD | WS_VISIBLE, rect.left, rect.top, rect.right, rect.bottom,
		ghPrevWnd, (HMENU)IDW_PVW_VIEW_WNDW, ghInst, NULL );

	//	ActiveX�R���g���[���̃C���^�[�t�F�[�X��v��
	if( SUCCEEDED( AtlAxGetControl( ghIEwnd, &comPunkIE ) ) )
	{
		gpWebBrowser2 = comPunkIE;	//	�|�C���^�Ɋi�[

		if( gpWebBrowser2 )
		{
			gpWebBrowser2->Navigate2( &vUrl, &vEmpty, &vEmpty, &vEmpty, &vEmpty );

			while( 1 )
			{
				hRslt = gpWebBrowser2->get_Document( &pDispatch );
				if( SUCCEEDED(hRslt) && pDispatch )
				{
					gpDocument2 = pDispatch;
					if( gpDocument2 ){	hRslt = S_OK;	break;	}
				}
				Sleep(100);
			}
		}
		else
		{
			NotifyBalloonExist( TEXT("�h�d�R���|�[�l���g���������o���Ȃ�������E�E�E"), TEXT("���ӂ���̂��m�点"), NIIF_ERROR );
			hRslt = E_ACCESSDENIED;
		}
	}

	if( SUCCEEDED(hRslt)  ){	PreviewPageWrite( iNowPage );	}

	UpdateWindow( ghPrevWnd );

	return hRslt;
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
LRESULT CALLBACK PreviewWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		HANDLE_MSG( hWnd, WM_SIZE,    Pvw_OnSize );	
		HANDLE_MSG( hWnd, WM_PAINT,   Pvw_OnPaint );	
		HANDLE_MSG( hWnd, WM_COMMAND, Pvw_OnCommand );	
		HANDLE_MSG( hWnd, WM_DESTROY, Pvw_OnDestroy );

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
VOID Pvw_OnCommand( HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify )
{
	LRESULT	lRslt;

	switch( id )
	{
		case IDM_PVW_ALLVW:	//	�S�v��ON/OFF
			lRslt = SendMessage( ghToolWnd, TB_ISBUTTONCHECKED, IDM_PVW_ALLVW, 0 );
			if( lRslt )	PreviewPageWrite(  -1 );
			else		PreviewPageWrite( gixFocusPage );
			InvalidateRect( ghPrevWnd, NULL, TRUE );
			break;

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
VOID Pvw_OnSize( HWND hWnd, UINT state, INT cx, INT cy )
{
	HWND	hWorkWnd;
	RECT	tbRect, rect;

	hWorkWnd = GetDlgItem( hWnd, IDW_PVW_TOOL_BAR );

	MoveWindow( hWorkWnd, 0, 0, 0, 0, TRUE );	//	�c�[���o�[�͐��l�Ȃ��Ă�����ɍ��킹�Ă����

	GetWindowRect( hWorkWnd, &tbRect );
	tbRect.right  -= tbRect.left;
	tbRect.bottom -= tbRect.top;
	tbRect.left = 0;
	tbRect.top  = 0;

	GetClientRect( ghPrevWnd, &rect );
	rect.top     = tbRect.bottom;
	rect.bottom -= tbRect.bottom;

	hWorkWnd = GetDlgItem( hWnd, IDW_PVW_VIEW_WNDW );

	MoveWindow( hWorkWnd, rect.left, rect.top, rect.right, rect.bottom, TRUE );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	PAINT�B�����̈悪�o�����Ƃ��ɔ����B�w�i�̈����ɒ��ӁB�w�i��h��Ԃ��Ă���A�I�u�W�F�N�g��`��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Pvw_OnPaint( HWND hWnd )
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
VOID Pvw_OnDestroy( HWND hWnd )
{
	RECT	rect;

	//	�E�C���h�E�ʒu���L�^
	GetWindowRect( ghPrevWnd, &rect );
	rect.right  -= rect.left;
	rect.bottom -= rect.top;
	InitWindowPos( INIT_SAVE , WDP_PREVIEW, &rect );	//	�I�����ۑ�

	ghPrevWnd = NULL;

	gpDocument2.Release( );

	gpWebBrowser2.Release( );

	if( hWnd )	PostMessage( ghPrntWnd, WMP_PREVIEW_CLOSE, 0, 0 );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ł�\������
	@param[in]	iViewPage	�Ŕԍ��E�|�P�Ȃ�S��
*/
HRESULT PreviewPageWrite( INT iViewPage )
{
	HRESULT	hRslt;

	VARIANT		*param;
	SAFEARRAY	*sfArray;

	UINT_PTR	szSize;
	INT		szCont, bstrLen;
	INT_PTR	iPage, i;
	LPSTR	pcContent;

	CHAR	acSeper[MAX_STRING];
	BSTR	bstr;

	string	asString;

	SYSTEMTIME	stTime;

	hRslt = S_OK;

	giViewMode = iViewPage;

	if( !(gpcHtmlHdr) )
	{
		NotifyBalloonExist( TEXT("�v���r���[�p�e���v���[�g�t�@�C����������Ȃ���"), TEXT("���ӂ���̂��m�点"), NIIF_ERROR );
		return E_HANDLE;
	}

	//	�\�����e���r�i�h�r�ł���
	GetLocalTime( &stTime );

	if( 0 >  iViewPage )	//	�S�v��
	{
		asString  = string( gpcHtmlHdr );

		iPage = DocNowFilePageCount(  );	//	�Ő��m�F����
		for( i = 0; iPage > i; i++ )
		{
			StringCchPrintfA( acSeper, MAX_STRING, gacResHeaderFmt, (i+1),
				stTime.wYear, stTime.wMonth, stTime.wDay,
				gcacWeek[stTime.wDayOfWeek],
				stTime.wHour, stTime.wMinute, stTime.wSecond );

			pcContent = DocPageTextPreviewAlloc( i, &szCont );

			asString += string( acSeper );
			asString += string( pcContent );
			asString += string( gacResFooterFmt );

			FREE(pcContent);
		}

		asString += string( gpcHtmlFtr );
	}
	else
	{
		StringCchPrintfA( acSeper, MAX_STRING, gacResHeaderFmt, (iViewPage+1),
			stTime.wYear, stTime.wMonth, stTime.wDay,
			gcacWeek[stTime.wDayOfWeek],
			stTime.wHour, stTime.wMinute, stTime.wSecond );

		pcContent = DocPageTextPreviewAlloc( iViewPage, &szCont );

		asString  = string( gpcHtmlHdr );
		asString += string( acSeper );
		asString += string( pcContent );
		asString += string( gacResFooterFmt );
		asString += string( gpcHtmlFtr );

		FREE(pcContent);
	}

	//	BSTR�ɕK�v�ȃT�C�Y�m�F
	szSize = asString.size( );
	bstrLen = MultiByteToWideChar( CP_ACP, 0, asString.c_str( ), szSize, NULL, 0 );

	//	�o�b�t�@���m��
	bstr = SysAllocStringLen( NULL, bstrLen );

	//	BSTR�Ƀu�`����
	MultiByteToWideChar( CP_ACP, 0, asString.c_str( ), szSize, bstr, bstrLen );

	sfArray = SafeArrayCreateVector( VT_VARIANT, 0, 1 );
			
	if (sfArray == NULL || gpDocument2 == NULL)
	{
		goto cleanup;
	}

	hRslt = SafeArrayAccessData(sfArray, (LPVOID*)&param );
	param->vt = VT_BSTR;
	param->bstrVal = bstr;
	hRslt = SafeArrayUnaccessData(sfArray);
	hRslt = gpDocument2->writeln(sfArray);

	hRslt = gpDocument2->close( );

cleanup:
	if( bstr )
	{
		SysFreeString( bstr );
		hRslt = SafeArrayAccessData( sfArray, (LPVOID*)&param );
		param->vt = VT_BSTR;
		param->bstrVal = NULL;
		hRslt = SafeArrayUnaccessData( sfArray );
	}

	if( sfArray ){	SafeArrayDestroy( sfArray );	}

	return hRslt;
}
//-------------------------------------------------------------------------------------------------

#endif