/*! @file
	@brief �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
	���̃t�@�C���� OrinrinCollector.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/10/06
*/

/*
Orinrin Collector : Clipboard Auto Stocker for Japanese Only
Copyright (C) 2011 - 2012 Orinrin/SikigamiHNQ

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.
*/
//-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "OrinrinCollector.h"
//-------------------------------------------------------------------------------------------------

// �O���[�o���ϐ�:
static HANDLE		ghMutex;					//!<	���d�N���h�~�pMutex

static HINSTANCE	ghInst;						//!<	���݂̃C���^�[�t�F�C�X
static TCHAR		gatTitle[MAX_STRING];		//!<	�^�C�g���o�[�̃e�L�X�g
static TCHAR		gatWindowClass[MAX_STRING];	//!<	���C���E�B���h�E�N���X��

static  HWND		ghToolTipWnd;				//!<	�c�[���`�b�v�̃E�C���h�E�n���h��
static HBRUSH		ghBrush;					//!<	�w�i�F�u���V
static HICON		ghIcon;						//!<	���C���A�C�R��
static  UINT		grdTaskbarResetID;			//!<	�^�X�N�o�[���ċN������Ƃ��̃��b�Z�[�WID��ێ�
static  HWND		ghNextViewer;				//!<	�N���b�v�{�[�h�`�F�C���̎��̂��
static BOOLEAN		gbClipSteal;				//!<	�R�s�[���Ղ���
static BOOLEAN		gGetMsgOn;					//!<	�ۑ�������o���[������
static BOOLEAN		gIsAST;						//!<	�ۑ���AST�`����
static TCHAR		gatIniPath[MAX_PATH];		//!<	�h�m�h�t�@�C���̈ʒu
static TCHAR		gatClipFile[MAX_PATH];		//!<	�N���b�v���e��ۑ�����t�@�C����
static NOTIFYICONDATA	gstNtfyIcon;			//!<	�^�X�N�g���C�A�C�R������̍\����

EXTERNED UINT		gbUniRadixHex;				//!<	���l�Q�Ƃ��P�U�i���^�ŕۑ�����

static  UINT		gbHotMod;					//!<	�|�b�v�A�b�v�z�b�g�L�[�C���q
static  UINT		gbHotVkey;					//!<	�|�b�v�A�b�v�z�b�g�L�[���z�L�[�R�[�h
//-------------------------------------------------------------------------------------------------

/*!
	�A�v���P�[�V�����̃G���g���|�C���g
	@param[in]	hInstance		���̃��W���[���̃C���X�^���X�n���h��
	@param[in]	hPrevInstance	�O�̃C���X�^���X�B���͖��g�p
	@param[in]	lpCmdLine		�R�}���h���C���B�g�[�N�������͂���ĂȂ��A�����̕�����
	@param[in]	nCmdShow		�N�����̕\����Ԃ������Ă�B�\���Ƃ�����������
	@retval FALSE	�r���I��
*/
INT APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//	TODO: �����ɃR�[�h��}�����Ă��������B
	MSG	msg;
	INT	msRslt;

#ifdef _DEBUG
	//_CRTDBG_ALLOC_MEM_DF;		// �w�肪�K�v�ȃt���O
	//_CRTDBG_CHECK_ALWAYS_DF;	//	���������`�F�b�N		_CRTDBG_CHECK_EVERY_128_DF
	//_CRTDBG_LEAK_CHECK_DF;		//	�I�����Ƀ��������[�N���`�F�b�N
	//_CRTDBG_DELAY_FREE_MEM_DF;	//	
	//	�����Ŏg�p����t���O���w��
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	//	���d�N���h�~
	ghMutex = CreateMutex( NULL, TRUE, TEXT("OrinrinCollector") );	//	���łɋN�����Ă��邩����
	if( GetLastError() == ERROR_ALREADY_EXISTS )	//	���łɋN�����Ă���
	{
		MessageBox( NULL, TEXT("�߂ɃA�v���͋N�����Ă��I"), TEXT("���ӂ���̂��m�点"), MB_OK|MB_ICONINFORMATION );
		ReleaseMutex( ghMutex );
		CloseHandle( ghMutex );
		return 0;
	}


	INITCOMMONCONTROLSEX	iccex;
	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccex.dwICC  = ICC_WIN95_CLASSES | ICC_USEREX_CLASSES;
	InitCommonControlsEx( &iccex );

	// �O���[�o������������������Ă��܂��B
	LoadString( hInstance, IDS_APP_TITLE, gatTitle, MAX_STRING );
	LoadString( hInstance, IDC_ORINRINCOLLECTOR, gatWindowClass, MAX_STRING );
	InitWndwClass( hInstance );

	// �A�v���P�[�V�����̏����������s���܂�:
	if( !InitInstance( hInstance, nCmdShow ) )
	{
		return FALSE;
	}


	//	���C�����b�Z�[�W���[�v
	for(;;)
	{
		msRslt = GetMessage( &msg, NULL, 0, 0 );
		if( 1 != msRslt )	break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}


//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�N���X����
	���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
	�A�v���P�[�V�������A�֘A�t����ꂽ�������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A���̊֐����Ăяo���Ă��������B
	@param[in]	hInstance	���̃��W���[���̃C���X�^���X�n���h��
	@return		�o�^�����N���X�A�g��
*/
ATOM InitWndwClass( HINSTANCE hInstance )
{
	WNDCLASSEX	wcex;

	//����ɂ��擾�͔j�󂵂Ȃ��Ă悢�H
	ghIcon = LoadIcon( hInstance, MAKEINTRESOURCE(IDI_ORINRINCOLLECTOR) );
	ghBrush = CreateSolidBrush( BASIC_COLOUR );

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= ghIcon;
	wcex.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground	= ghBrush;//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= gatWindowClass;
	wcex.hIconSm		= ghIcon;

	return RegisterClassEx( &wcex );
}
//-------------------------------------------------------------------------------------------------

/*!
	�C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
		���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
		���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
	@param[in]	hInstance	�C���X�^���X�n���h��
	@param[in]	nCmdShow	�N�����̕\�����
*/
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	HWND	hWnd;
	INT		xxx, yyy;
	RECT	rect;

	BOOL	bHotRslt;

	ghInst = hInstance;	//	�O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B


	//	�ݒ�t�@�C���ʒu�m�F
	GetModuleFileName( hInstance, gatIniPath, MAX_PATH );
	PathRemoveFileSpec( gatIniPath );
	PathAppend( gatIniPath, INI_FILE );

	//	������ŏ����ݒ�m��
	gbClipSteal   = InitParamValue( INIT_LOAD, VL_COLLECT_AON,  0 );	//	�R�s�y�ۑ��E�f�t�H�͋N�����n�e�e
	gGetMsgOn     = InitParamValue( INIT_LOAD, VL_USE_BALLOON,  1 );
	gbUniRadixHex = InitParamValue( INIT_LOAD, VL_UNIRADIX_HEX, 0 );

	//	�����ݒ�� Ctrl+Shift+C
	gbHotMod      = InitParamValue( INIT_LOAD, VL_COLHOT_MODY, (MOD_CONTROL | MOD_SHIFT) );
	gbHotVkey     = InitParamValue( INIT_LOAD, VL_COLHOT_VKEY, VK_C );


	hWnd = GetDesktopWindow(  );
	GetWindowRect( hWnd, &rect );
	xxx = ( rect.right  - WCL_WIDTH ) / 2;
	yyy = ( rect.bottom - WCL_HEIGHT ) / 2;

	hWnd = CreateWindowEx( WS_EX_TOOLWINDOW | WS_EX_APPWINDOW, gatWindowClass, gatTitle, WS_CAPTION | WS_POPUPWINDOW, xxx, yyy, WCL_WIDTH, WCL_HEIGHT, NULL, NULL, hInstance, NULL);

	if( !hWnd ){	return FALSE;	}


	//	�^�X�N�g���C�����ځ`�񂵂��Ƃ��̍ċN�����b�Z�[�W�ԍ����m��
	grdTaskbarResetID = RegisterWindowMessage( TEXT("TaskbarCreated") );

	//	�N���b�v�{�[�h�`�F�[���Ɏ�����o�^
	ghNextViewer = SetClipboardViewer( hWnd );


	FileListViewInit( hWnd );	//	���X�g������
	FileListViewGet( hWnd, 0, gatClipFile );	//	��荞�݃t�@�C�����m�ۂ��Ă���
	FileTypeCheck( gatClipFile );	//	AST�������łȂ������m�F

	if( gGetMsgOn ){		Button_SetCheck( GetDlgItem(hWnd,IDB_CLIP_USE_BALLOON)  , BST_CHECKED );	}
	if( gbUniRadixHex ){	Button_SetCheck( GetDlgItem(hWnd,IDB_CLIP_UNIRADIX_HEX) , BST_CHECKED );	}
	if( gbClipSteal ){		Button_SetCheck( GetDlgItem(hWnd,IDB_CLIP_STEAL_ACT_ON) , BST_CHECKED );	}

	TasktrayIconAdd( hWnd );

	//	�Ƃ肠�����ACtrl+Shift+C
	bHotRslt = RegisterHotKey( hWnd, IDHK_CLIPSTEAL_FILECHANGE, gbHotMod, gbHotVkey );

	ShowWindow( hWnd, SW_HIDE );	//	SW_HIDE

	return TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	���W�X�^�z�b�g�L�[�ƃz�b�g�L�[�R���g���[���̏C���q�����
	@param[in]	bSrc	���̏C���q�R�[�h
	@param[in]	bDrct	��O���W�X�^���R���g���[���@�O�R���g���[�������W�X�^
	@return	�ϊ������R�[�h
*/
UINT RegHotModExchange( UINT bSrc, BOOLEAN bDrct )
{
	BYTE	bDest = 0;

	if( bDrct  )	//	���W�X�^���R���g���[��
	{
		if( bSrc & MOD_SHIFT )		bDest |= HOTKEYF_SHIFT;		//	�V�t�g
		if( bSrc & MOD_CONTROL )	bDest |= HOTKEYF_CONTROL;	//	�R���g���[��
		if( bSrc & MOD_ALT )		bDest |= HOTKEYF_ALT;		//	�A���^�l�[�g
	}
	else	//	�R���g���[�������W�X�^
	{
		if( bSrc & HOTKEYF_SHIFT )		bDest |= MOD_SHIFT;		//	�V�t�g
		if( bSrc & HOTKEYF_CONTROL )	bDest |= MOD_CONTROL;	//	�R���g���[��
		if( bSrc & HOTKEYF_ALT )		bDest |= MOD_ALT;		//	�A���^�l�[�g
	}

	return bDest;
}
//-------------------------------------------------------------------------------------------------

/*!
	�R���g���[���Ƀc�[���`�b�v��ݒ�E�c�[���`�b�v�n���h���͑��ϐ��Ŋm��
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	itemID	�R���g���[���̂h�c�ԍ�
	@param[in]	ptText	�`�b�v������������
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT ToolTipSetting( HWND hWnd, UINT itemID, LPTSTR ptText )
{
	TTTOOLINFO	stToolInfo;

	ZeroMemory( &stToolInfo, sizeof(TTTOOLINFO) );

	stToolInfo.cbSize   = sizeof(TTTOOLINFO);
	stToolInfo.uFlags   = TTF_SUBCLASS;
	stToolInfo.hinst    = NULL;	//	
	stToolInfo.hwnd     = GetDlgItem( hWnd, itemID );
	stToolInfo.uId      = itemID;
	GetClientRect( stToolInfo.hwnd, &stToolInfo.rect );
	stToolInfo.lpszText = ptText;

	SendMessage( ghToolTipWnd, TTM_ADDTOOL, 0, (LPARAM)&stToolInfo );
	SendMessage( ghToolTipWnd, TTM_SETMAXTIPWIDTH, 0, 0 );	//	�`�b�v�̕��B�O�ݒ�ł����B���ꂵ�Ƃ��Ȃ��Ɖ��s����Ȃ�

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���X�g�r���[��������
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
*/
HRESULT FileListViewInit( HWND hWnd )
{
	UINT	dItems, d;
	TCHAR	atFilePath[MAX_PATH];
	HWND	hLvWnd = GetDlgItem( hWnd, IDLV_CLIPSTEAL_FILELISTVW );


	ListView_DeleteAllItems( hLvWnd );

	dItems = InitParamValue( INIT_LOAD, VL_CLIPFILECNT, 1 );
	for( d = 0; dItems > d; d++ )
	{
		InitClipStealOpen( INIT_LOAD, d, atFilePath );
		FileListViewAdd( hWnd, atFilePath );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���X�g�r���[�Ƀt�@�C������ǉ�����
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	ptPath	�t�@�C���t���p�X
	@return		�ǉ���̃A�C�e����
*/
INT FileListViewAdd( HWND hWnd, LPTSTR ptPath )
{
	INT	iCount;
	TCHAR	atName[MAX_PATH];
	LVITEM	stLvi;
	HWND	hLvWnd = GetDlgItem( hWnd, IDLV_CLIPSTEAL_FILELISTVW );

	iCount = ListView_GetItemCount( hLvWnd );

	if( !(ptPath) ){	return iCount;	}
	if( NULL == ptPath[0] ){	return iCount;	}

	StringCchCopy( atName, MAX_PATH, ptPath );
	PathStripPath( atName );

	ZeroMemory( &stLvi, sizeof(LVITEM) );
	stLvi.mask     = LVIF_TEXT;
	stLvi.iItem    = iCount;

	stLvi.iSubItem = 0;
	stLvi.pszText  = atName;
	ListView_InsertItem( hLvWnd, &stLvi );

	stLvi.iSubItem = 1;
	stLvi.pszText  = ptPath;
	ListView_SetItem( hLvWnd, &stLvi );

	iCount = ListView_GetItemCount( hLvWnd );

	return iCount;
}
//-------------------------------------------------------------------------------------------------

/*!
	���X�g�r���[����t�@�C�������m��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	iNumber	���X�g��̔ԍ�
	@param[in]	ptFile	�t�@�C���t���p�X�EMAX_PATH�ł��邱��
*/
HRESULT FileListViewGet( HWND hWnd, INT iNumber, LPTSTR ptFile )
{
	LVITEM	stLvi;
	TCHAR	atBuff[MAX_PATH];
	INT		iCount;
	HWND	hLvWnd = GetDlgItem( hWnd, IDLV_CLIPSTEAL_FILELISTVW );

	iCount = ListView_GetItemCount( hLvWnd );
	if( iCount <= iNumber ){	return E_OUTOFMEMORY;	}

	ZeroMemory( &stLvi, sizeof(LVITEM) );
	stLvi.mask       = LVIF_TEXT;
	stLvi.iItem      = iNumber;
	stLvi.iSubItem   = 1;
	stLvi.pszText    = atBuff;
	stLvi.cchTextMax = MAX_PATH;
	ListView_GetItem( hLvWnd, &stLvi );

	StringCchCopy( ptFile, MAX_PATH, atBuff );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���X�g�r���[�̃A�C�e�����폜����
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		�폜��̃A�C�e����
*/
INT FileListViewDelete( HWND hWnd )
{
	HWND	hLvWnd = GetDlgItem( hWnd, IDLV_CLIPSTEAL_FILELISTVW );
	INT		iItem, iCount;
	TCHAR	atPath[MAX_PATH];

	//	�I������Ă鍀�ڂ��m��
	iItem = ListView_GetNextItem( hLvWnd, -1, LVNI_ALL | LVNI_SELECTED );

	//	�I������Ă郂�m���Ȃ��Ɩ��Ӗ�
	if( 0 <= iItem )
	{
		//	�����I�𒆂̃A���Ȃ�폜���Ȃ�
		FileListViewGet( hWnd, iItem, atPath );
		if( 0 == StrCmp( gatClipFile, atPath ) )
		{
			MessageBox( hWnd, TEXT("���̃t�@�C���͎g�p������B\r\n�폜�ł��Ȃ���B"), TEXT("���ӂ���̂��m�点"), MB_OK | MB_ICONERROR );
		}
		else	//	��薳���Ȃ�폜
		{
			ListView_DeleteItem( hLvWnd, iItem );
		}
	}

	iCount = ListView_GetItemCount( hLvWnd );

	return iCount;
}
//-------------------------------------------------------------------------------------------------

/*!
	�S�ݒ���Z�[�u����
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	bActOn	�N�����R�s�y�ۑ��@�\�n�m�ɂ��Ă�����
*/
HRESULT InitSettingSave( HWND hWnd, UINT bActOn )
{
	HWND	hLvWnd = GetDlgItem( hWnd, IDLV_CLIPSTEAL_FILELISTVW );
	INT		iCount, i;
	TCHAR	atBuff[MAX_PATH];


	iCount = ListView_GetItemCount( hLvWnd );

	//	��U�Z�N�V��������ɂ���
	ZeroMemory( atBuff, sizeof(atBuff) );
	WritePrivateProfileSection( TEXT("Collector"), atBuff, gatIniPath );

	InitParamValue( INIT_SAVE, VL_USE_BALLOON,  gGetMsgOn );
	InitParamValue( INIT_SAVE, VL_UNIRADIX_HEX, gbUniRadixHex );
	InitParamValue( INIT_SAVE, VL_CLIPFILECNT,  iCount );
	InitParamValue( INIT_SAVE, VL_COLLECT_AON,  bActOn );
	InitParamValue( INIT_SAVE, VL_COLHOT_MODY,  gbHotMod );
	InitParamValue( INIT_SAVE, VL_COLHOT_VKEY,  gbHotVkey );

	for( i = 0; iCount > i; i++ )
	{
		FileListViewGet( hWnd, i, atBuff );
		InitClipStealOpen( INIT_SAVE, i, atBuff );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�R�s�y�ۑ��p�t�@�C���̃Z�[�u���[�h
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	dNumber	�ۑ��t�@�C���̔ԍ�
	@param[out]	ptFile	�t���p�X�EMAX_PATH�ł��邱��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT InitClipStealOpen( UINT dMode, UINT dNumber, LPTSTR ptFile )
{
	TCHAR	atKey[MIN_STRING];

	if(  !(ptFile) )	return 0;

	ZeroMemory( atKey, sizeof(atKey) );
	if( 0 == dNumber )	StringCchCopy( atKey, MIN_STRING, TEXT("CopySaveFile") );
	else	StringCchPrintf( atKey, MIN_STRING, TEXT("CopySaveFile%d"), dNumber );

	if( dMode ){	GetPrivateProfileString( TEXT("Collector"), atKey, TEXT(""), ptFile, MAX_PATH, gatIniPath );	}
	else{			WritePrivateProfileString( TEXT("Collector"), atKey, ptFile, gatIniPath );	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

BOOLEAN FileTypeCheck( LPTSTR ptFile )
{
	LPTSTR		ptExten;	//	�t�@�C�����̊g���q
	TCHAR		atExBuf[10];

	//	�ۑ��`���m�F
	ptExten = PathFindExtension( ptFile );	//	�g���q�������Ȃ�NULL�A�Ƃ��������[�ɂȂ�
	StringCchCopy( atExBuf, 10, ptExten );	//	����̂��߃R�s�[
	CharLower( atExBuf );	//	��r�̂��߂ɏ������ɂ����Ⴄ
	//	AST�ł��邩
	if( !( StrCmp( atExBuf , TEXT(".ast") ) ) ){	gIsAST = TRUE;	}
	else	gIsAST =  FALSE;	//	AST����Ȃ��Ȃ�MLT�Ƃ��Ĉ���

	return gIsAST;
}
//-------------------------------------------------------------------------------------------------

/*!
	�p�����[�^�l�̃Z�[�u���[�h
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	dStyle	�p�����[�^�̎��
	@param[in]	nValue	���[�h�F�f�t�H�l�@�Z�[�u�F�l
	@return		INT	���[�h�F�l�@�Z�[�u�F�O
*/
INT InitParamValue( UINT dMode, UINT dStyle, INT nValue )
{
	TCHAR	atKeyName[MIN_STRING], atBuff[MIN_STRING];
	INT	dBuff = 0;

	switch( dStyle )
	{
		case VL_UNIRADIX_HEX:	StringCchCopy( atKeyName, SUB_STRING, TEXT("UniRadixHex") );	break;
		case VL_USE_BALLOON:	StringCchCopy( atKeyName, SUB_STRING, TEXT("UseBalloon")  );	break;
		case VL_CLIPFILECNT:	StringCchCopy( atKeyName, SUB_STRING, TEXT("FileCount") );		break;
		case VL_COLLECT_AON:	StringCchCopy( atKeyName, SUB_STRING, TEXT("CollectActOn") );	break;
		case VL_COLHOT_MODY:	StringCchCopy( atKeyName, SUB_STRING, TEXT("CollectHotMod") );	break;
		case VL_COLHOT_VKEY:	StringCchCopy( atKeyName, SUB_STRING, TEXT("CollectHotVkey") );	break;

		default:	return nValue;
	}

	StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), nValue );

	if( dMode )	//	���[�h
	{
		GetPrivateProfileString( TEXT("Collector"), atKeyName, atBuff, atBuff, MIN_STRING, gatIniPath );
		dBuff = StrToInt( atBuff );
	}
	else	//	�Z�[�u
	{
		WritePrivateProfileString( TEXT("Collector"), atKeyName, atBuff, gatIniPath );
	}

	return dBuff;
}
//-------------------------------------------------------------------------------------------------


/*!
	���C���̃E�C���h�E�v���V�[�W��
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	message		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam		�ǉ��̏��P
	@param[in]	lParam		�ǉ��̏��Q
	@retval 0	���b�Z�[�W�����ς�
	@retval no0	�����ł͏����������ɉ�
*/
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		HANDLE_MSG( hWnd, WM_CREATE,         Cls_OnCreate );	//	��ʂ̍\���p�[�c�����B�{�^���Ƃ�
		HANDLE_MSG( hWnd, WM_PAINT,          Cls_OnPaint  );	//	��ʂ̍X�V�Ƃ�
		HANDLE_MSG( hWnd, WM_COMMAND,        Cls_OnCommand );	//	�{�^�������ꂽ�Ƃ��̃R�}���h����
		HANDLE_MSG( hWnd, WM_DESTROY,        Cls_OnDestroy );	//	�\�t�g�I�����̏���
		HANDLE_MSG( hWnd, WM_HOTKEY,         Cls_OnHotKey );	//	
		HANDLE_MSG( hWnd, WM_CTLCOLORSTATIC, Cls_OnCtlColor );
		HANDLE_MSG( hWnd, WM_DRAWCLIPBOARD,  Cls_OnDrawClipboard );	//	�N���b�v�{�[�h�ɕύX����������
		HANDLE_MSG( hWnd, WM_CHANGECBCHAIN,  Cls_OnChangeCBChain );	//	�N���b�v�{�[�h�r���[���`�F�C���ɕύX����������

		case WM_CLOSE:	//	�ݒ����Ƃ�
			RegisterHotKey( hWnd, IDHK_CLIPSTEAL_FILECHANGE, gbHotMod, gbHotVkey );	//	�o�^������
			ShowWindow( hWnd, SW_HIDE );
			return 0;

		case WMP_TRAYNOTIFYICON:	//	�^�X�N�g���C�̃A�C�R�����د����ꂽ��
			TaskTrayIconEvent( hWnd, (UINT)wParam, (UINT)lParam );
			return 0;

		default:
			if( grdTaskbarResetID == message )	//	�^�X�N�g���C�����ځ`�񂵂���
			{
				TasktrayIconAdd( hWnd );	//	�^�X�N�g���C�����ځ`�񂵂��Ƃ��̃��b�Z�[�W�B
				return 0;					//	�o�^������
			}
			break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	���C���̃N���G�C�g�B
	@param[in]	hWnd			�e�E�C���h�E�̃n���h��
	@param[in]	lpCreateStruct	�A�v���P�[�V�����̏��������e
	@return	TRUE	���ɂȂ�
*/
BOOLEAN Cls_OnCreate( HWND hWnd, LPCREATESTRUCT lpCreateStruct )
{
	HINSTANCE	lcInst = lpCreateStruct->hInstance;	//	�󂯎������������񂩂�A�C���X�^���X�n���h�����Ђ��ς�
	HWND	hWorkWnd;
	RECT	rect;
	LVCOLUMN	stLvColm;

	UINT	bCtrlMod;

	GetClientRect( hWnd, &rect );

//�c�[���`�b�v
	ghToolTipWnd = CreateWindowEx( 0, TOOLTIPS_CLASS, NULL, TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWnd, NULL, lcInst, NULL );

//�A�C�R��
	hWorkWnd = CreateWindowEx( 0, WC_STATIC, TEXT(""), WS_CHILD | WS_VISIBLE | SS_ICON, 8, 8, 32, 32, hWnd, (HMENU)IDC_MYICON, lcInst, NULL );
	SendMessage( hWorkWnd, STM_SETICON, (WPARAM)ghIcon, 0 );

	CreateWindowEx( 0, WC_STATIC, TEXT("OrinrinCollector, Version 1.2 (2012.510.2200.920)"), WS_CHILD | WS_VISIBLE, 44, 8, 370, 23, hWnd, (HMENU)IDC_STATIC, lcInst, NULL );

	CreateWindowEx( 0, WC_STATIC, TEXT("���Ղ����N���b�v���e��ۑ�����t�@�C����"), WS_CHILD | WS_VISIBLE, 8, 48, 370, 23, hWnd, (HMENU)IDC_STATIC, lcInst, NULL );

	CreateWindowEx( 0, WC_EDIT, TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 8, 70, rect.right-16-160, 23, hWnd, (HMENU)IDE_CLIPSTEAL_FILE, lcInst, NULL );
	ToolTipSetting( hWnd, IDE_CLIPSTEAL_FILE, TEXT("�t�@�C��������͂��ĂˁBMLT��AST���g�����B") );

	CreateWindowEx( 0, WC_BUTTON, TEXT("�Q��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect.right-8-160, 70, 50, 23, hWnd, (HMENU)IDB_CLIPSTEAL_REF, lcInst, NULL );
	ToolTipSetting( hWnd, IDB_CLIPSTEAL_REF, TEXT("�t�@�C���I���_�C�����O���J���āA�t�@�C�����w��ł����B") );

	CreateWindowEx( 0, WC_BUTTON, TEXT("�ǉ�"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect.right-8-100, 70, 50, 23, hWnd, (HMENU)IDB_CLIPSTEAL_FILEADD, lcInst, NULL );
	ToolTipSetting( hWnd, IDB_CLIPSTEAL_FILEADD, TEXT("�w�肵���t�@�C�����A�g�p���X�g�ɒǉ������B") );

	CreateWindowEx( 0, WC_BUTTON, TEXT("�폜"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect.right-8-50, 70, 50, 23, hWnd, (HMENU)IDB_CLIPSTEAL_FILEDEL, lcInst, NULL );
	ToolTipSetting( hWnd, IDB_CLIPSTEAL_FILEDEL, TEXT("�g�p���X�g�őI�������t�@�C�����A���X�g����폜�����B\r\n�t�@�C�����̂��폜����킯����Ȃ���B") );

	hWorkWnd = CreateWindowEx( 0, WC_LISTVIEW, TEXT("filelv"), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LVS_REPORT | LVS_NOSORTHEADER | LVS_SINGLESEL, 8, 95, rect.right-16, 105, hWnd, (HMENU)IDLV_CLIPSTEAL_FILELISTVW, lcInst, NULL );
	ListView_SetExtendedListViewStyle( hWorkWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	ToolTipSetting( hWnd, IDLV_CLIPSTEAL_FILELISTVW, TEXT("�g�p����t�@�C���̈ꗗ����B") );

	ZeroMemory( &stLvColm, sizeof(LVCOLUMN) );
	stLvColm.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	stLvColm.fmt = LVCFMT_LEFT;
	stLvColm.pszText = TEXT("�t�@�C����");	stLvColm.cx = 120;	stLvColm.iSubItem = 0;	ListView_InsertColumn( hWorkWnd, 0, &stLvColm );
	stLvColm.pszText = TEXT("�t���p�X");	stLvColm.cx = 300;	stLvColm.iSubItem = 1;	ListView_InsertColumn( hWorkWnd, 1, &stLvColm );

	CreateWindowEx( 0, WC_BUTTON, TEXT("�ۑ�������o���[�����b�Z�[�W��\������"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 8, 207, rect.right-16, 23, hWnd, (HMENU)IDB_CLIP_USE_BALLOON, lcInst, NULL );
	ToolTipSetting( hWnd, IDB_CLIP_USE_BALLOON, TEXT("�t�@�C���Ɏ�荞�񂾂�A�o���[�����b�Z�[�W�ł��m�点�����B") );

	CreateWindowEx( 0, WC_BUTTON, TEXT("���j�R�[�h���l�Q�Ƃ͂P�U�i���^�i��`�F�b�N�łP�O�i���j"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 8, 234, rect.right-16, 23, hWnd, (HMENU)IDB_CLIP_UNIRADIX_HEX, lcInst, NULL );
	ToolTipSetting( hWnd, IDB_CLIP_UNIRADIX_HEX, TEXT("���Ղ������e�Ƀ��j�R�[�h���܂܂�Ă�����A�ǂ������`���ŕۑ�����̂������߂ĂˁB") );

	CreateWindowEx( 0, WC_BUTTON, TEXT("�N�������Ƃ��ɁA�R�s�y�ۑ����n�m�ɂ���"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 8, 261, rect.right-16, 23, hWnd, (HMENU)IDB_CLIP_STEAL_ACT_ON, lcInst, NULL );
	ToolTipSetting( hWnd, IDB_CLIP_STEAL_ACT_ON, TEXT("�N�������Ƃ��ɁA�R�s�y�ۑ��@�\���n�m�ɂ��Ă�����B") );

	CreateWindowEx( 0, WC_STATIC, TEXT("���j���[�|�b�v�A�b�v�z�b�g�L�["), WS_CHILD | WS_VISIBLE, 8, 290, 260, 23, hWnd, (HMENU)IDC_STATIC, lcInst, NULL );
	hWorkWnd = CreateWindowEx( 0, HOTKEY_CLASS, TEXT(""), WS_CHILD | WS_VISIBLE, 270, 290, 200, 23, hWnd, (HMENU)IDHK_CLIP_POPUP_KEYBIND, lcInst, NULL );
	ToolTipSetting( hWnd, IDHK_CLIP_POPUP_KEYBIND, TEXT("�������N���b�N���āA�L�[�̑g�ݍ��킹��ݒ肵�ĂˁB") );

	bCtrlMod = RegHotModExchange( gbHotMod, 1 );
	SendMessage( hWorkWnd , HKM_SETHOTKEY, MAKEWORD(gbHotVkey, bCtrlMod), 0 );

	CreateWindowEx( 0, WC_BUTTON, TEXT("�ۑ����ĕ���"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect.right-8-150, rect.bottom-30, 150, 23, hWnd, (HMENU)IDB_CLIP_SAVE_AND_EXIT, lcInst, NULL );
	ToolTipSetting( hWnd, IDB_CLIP_SAVE_AND_EXIT, TEXT("�ύX���e��ۑ����āA���̑�������B") );

	return TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C����COMMAND���b�Z�[�W�̎󂯎��B�{�^�������ꂽ�Ƃ��Ŕ���
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	id			���b�Z�[�W�𔭐��������q�E�C���h�E�̎��ʎq	LOWORD(wParam)
	@param[in]	hwndCtl		���b�Z�[�W�𔭐��������q�E�C���h�E�̃n���h��	lParam
	@param[in]	codeNotify	�ʒm���b�Z�[�W	HIWORD(wParam)
	@return		�Ȃ�
*/
VOID Cls_OnCommand( HWND hWnd, INT id, HWND hwndCtl, UINT codeNotify )
{
	TCHAR	atPath[MAX_PATH], atBuff[MAX_PATH];
	INT		iRslt, iTgt;

	UINT	bActOn;
	BOOL	bHotRslt;

	LRESULT	lRslt;
	UINT	bMod, bVkey;

	switch( id )
	{
		case IDM_EXIT:	DestroyWindow( hWnd );	break;


		case IDM_CLIPSTEAL_OPTION:
			ShowWindow( hWnd, SW_SHOW );
			UnregisterHotKey( hWnd, IDHK_CLIPSTEAL_FILECHANGE );	//	�L�[�o�C���h�擾�̂��߂Ɉꎞ�I�ɉ���
			break;

		case IDM_CLIPSTEAL_TOGGLE:
			gbClipSteal = !(gbClipSteal);
			TaskTrayIconCaptionChange( hWnd );
			break;

		case IDB_CLIPSTEAL_REF:	//	�t�@�C���J���_�C�����O�ďo
			if( SelectFileDlg( hWnd, atPath, MAX_PATH ) )
			{
				Edit_SetText( GetDlgItem(hWnd,IDE_CLIPSTEAL_FILE), atPath );
			}
			break;

		case IDB_CLIPSTEAL_FILEADD:
			Edit_GetText( GetDlgItem(hWnd,IDE_CLIPSTEAL_FILE), atPath, MAX_PATH );
			iRslt = FileListViewAdd( hWnd, atPath );
			if( 1 == iRslt )	//	�P���ڂȂ�
			{
				StringCchCopy( gatClipFile, MAX_PATH, atPath );
				FileTypeCheck( gatClipFile );
			}	//	��荞�݃t�@�C���Ƃ��ēo�^����
			break;

		case IDB_CLIPSTEAL_FILEDEL:
			FileListViewDelete( hWnd );
			break;

		case IDB_CLIP_SAVE_AND_EXIT:
			//	�z�b�g�L�[�ݒ���m��
			lRslt = SendDlgItemMessage( hWnd, IDHK_CLIP_POPUP_KEYBIND, HKM_GETHOTKEY, 0, 0 );
			bVkey = LOBYTE( lRslt );
			bMod  = RegHotModExchange( HIBYTE( lRslt ), 0 );
			//	�������W�X�g���s��������Ȃ��E�����Ŕ�O���ǂ�
			bHotRslt = RegisterHotKey( hWnd, IDHK_CLIPSTEAL_FILECHANGE, bMod, bVkey );
			if( bHotRslt )
			{
				gbHotVkey = bVkey;
				gbHotMod  = bMod;
			}
			else
			{
				MessageBox( hWnd, TEXT("�z�b�g�L�[���o�^�o���Ȃ�������B\r\n�߂Ɏg���Ă�݂����B\r\n���̑g�ݍ��킹�������Ă݂āI"), TEXT("���ӂ���̂��m�点"), MB_OK | MB_ICONERROR );
				break;
			}

			if( BST_CHECKED == IsDlgButtonChecked( hWnd, IDB_CLIP_USE_BALLOON ) ){	gGetMsgOn = TRUE;	}
			else{	gGetMsgOn = FALSE;	}

			if( BST_CHECKED == IsDlgButtonChecked( hWnd, IDB_CLIP_UNIRADIX_HEX ) ){	gbUniRadixHex = TRUE;	}
			else{	gbUniRadixHex = FALSE;	}

			bActOn = IsDlgButtonChecked( hWnd, IDB_CLIP_STEAL_ACT_ON );

			InitSettingSave( hWnd, bActOn );

			ShowWindow( hWnd, SW_HIDE );
			break;

		default:	break;
	}

	if( IDM_CLIPSTEAL_SELECT <= id )	//	���j���[�I��
	{
		iTgt = id - IDM_CLIPSTEAL_SELECT;	//	���X�g��̈ʒu
		FileListViewGet( hWnd, iTgt, atPath );
		StringCchCopy( gatClipFile, MAX_PATH, atPath );
		FileTypeCheck( gatClipFile );
		//	��荞�݃t�@�C���Ƃ��ēo�^����
		PathStripPath( atPath );
		StringCchPrintf( atBuff, MAX_PATH, TEXT("%s �ɕۑ����Ă�����"), atPath );
		TaskTrayIconBalloon( hWnd, atBuff, TEXT("�ۑ����ύX������"), NIIF_INFO );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	PAINT�B�����̈悪�o�����Ƃ��ɔ����B�w�i�̈����ɒ��ӁB�w�i��h��Ԃ��Ă���A�I�u�W�F�N�g��`��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Cls_OnPaint( HWND hWnd )
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
VOID Cls_OnDestroy( HWND hWnd )
{
	UnregisterHotKey( hWnd, IDHK_CLIPSTEAL_FILECHANGE );

	ChangeClipboardChain( hWnd , ghNextViewer );	//	�N���b�v�{�[�h�`�F�C�����痣�E

	gstNtfyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
	Shell_NotifyIcon( NIM_DELETE, &gstNtfyIcon );	//	�g���C�A�C�R���j��

	DeleteBrush( ghBrush );

	PostQuitMessage( 0 );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�R���g���[���̐F�ς�
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	hdc			�F��h��f�o�C�X�R���e�L�X�g
	@param[in]	hWndChild	���������R���g���[���̃n���h��
	@param[in]	type		���������R���g���[���̃^�C�v
	@return		���̐F�̃u���V
*/
HBRUSH Cls_OnCtlColor( HWND hWnd, HDC hdc, HWND hWndChild, INT type )
{
	SetBkColor( hdc, BASIC_COLOUR );

	return ghBrush;
}
//-------------------------------------------------------------------------------------------------

/*!
	�z�b�g�L�[�����ꂽ��Ă΂��
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	idHotKey	�o�^���Ă���h�c
	@param[in]	fuModifiers	CTRL�ASHIFT�AALT �̑g�ݍ��킹���
	@param[in]	vk			������Ă����ʃL�[
	@return		����
*/
VOID Cls_OnHotKey( HWND hWnd, INT idHotKey, UINT fuModifiers, UINT vk )
{
	POINT	stPoint;
	HMENU	hMenu;

	if( IDHK_CLIPSTEAL_FILECHANGE == idHotKey )
	{
		GetCursorPos( &stPoint );

		SetForegroundWindow( hWnd );//�������Ȃ��ƃ|�b�v�A�b�v�������Ȃ��炵��

		hMenu = CreateFileSelMenu( hWnd, 1 );
		TrackPopupMenu( hMenu, 0, stPoint.x, stPoint.y, 0, hWnd, NULL );
		DestroyMenu( hMenu );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C���I���_�C�����O�̕\��
	@param[in]	hWnd		�e�E�C���h�E�n���h��
	@param[in]	ptSelFile	�t�@�C����������o�b�t�@�ւ̃|�C���^�[
	@param[in]	cchLen		�o�b�t�@�̕������B�o�C�g������Ȃ���
	@return		��O�F�t�@�C���Ƃ����@�O�F�L�����Z������
*/
BOOLEAN SelectFileDlg( HWND hWnd, LPTSTR ptSelFile, UINT_PTR cchLen )
{
	OPENFILENAME	stOpenFile;
	BOOLEAN	bOpened;
	TCHAR	atFilePath[MAX_PATH], atFileName[MAX_STRING];

	if( !(ptSelFile) )	return FALSE;

	ZeroMemory( &stOpenFile, sizeof(OPENFILENAME) );

	ZeroMemory( atFilePath,  sizeof(atFilePath) );
	ZeroMemory( atFileName,  sizeof(atFileName) );

	stOpenFile.lStructSize     = sizeof(OPENFILENAME);
	stOpenFile.hwndOwner       = hWnd;
	stOpenFile.lpstrFilter     = TEXT("�A�X�L�[�A�[�g�t�@�C�� ( mlt, ast )\0*.mlt;*.ast\0�S�Ă̌`��(*.*)\0*.*\0\0");
	stOpenFile.nFilterIndex    = 1;
	stOpenFile.lpstrFile       = atFilePath;
	stOpenFile.nMaxFile        = MAX_PATH;
	stOpenFile.lpstrFileTitle  = atFileName;
	stOpenFile.nMaxFileTitle   = MAX_STRING;
//	stOpenFile.lpstrInitialDir = 
	stOpenFile.lpstrTitle      = TEXT("���ߍ��ރt�@�C�����w�肷��̂ł�");
	stOpenFile.Flags           = OFN_EXPLORER | OFN_HIDEREADONLY;
	stOpenFile.lpstrDefExt     = TEXT("mlt");

	//������ FileSaveDialogue ���o��
	bOpened = GetOpenFileName( &stOpenFile );

	if( !(bOpened) ){	return FALSE;	}	//	�L�����Z�����Ă��牽�����Ȃ�

	StringCchCopy( ptSelFile, cchLen, atFilePath );

	return TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�^�X�N�g���C�ɃA�C�R����\��������
	@param[in]	hWnd	�E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT TasktrayIconAdd( HWND hWnd )
{
	INT	crdCount = 0;


	//	�^�X�N�g���C�A�C�R���ڑ�
	ZeroMemory( &gstNtfyIcon, sizeof(gstNtfyIcon) );
	gstNtfyIcon.cbSize = sizeof(NOTIFYICONDATA);
	gstNtfyIcon.hWnd   = hWnd;
	gstNtfyIcon.uID    = IDC_ORINRINCOLLECTOR;
	gstNtfyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	gstNtfyIcon.uCallbackMessage = WMP_TRAYNOTIFYICON;
	gstNtfyIcon.hIcon  = ghIcon;
	StringCchPrintf( gstNtfyIcon.szTip, 128, TEXT("OrinrinCollector �[ %s"), gbClipSteal ? TEXT("����>�@<����") : TEXT("�y�y�y�D�D�D") );

	// �A�C�R����o�^�ł��邩�G���[�ɂȂ�܂ŌJ��Ԃ�
	for( ; ; )
	{
		if( Shell_NotifyIcon(NIM_ADD, &gstNtfyIcon) )
		{
			break;	//	�o�^�ł�����I���
		}
		else
		{
			if ( ERROR_TIMEOUT != GetLastError() || 10 <= crdCount )
			{	//	�^�C���A�E�g�ȊO�Ŏ��s�A�������͂P�O�񃊃g���C���Ă��_����
				MessageBox( hWnd, TEXT("�^�X�N�g���C�ɃA�C�R����o�^�ł��Ȃ������̂ł��B\r\n�I������̂ł��B"), NULL, MB_OK | MB_ICONERROR );
				SendMessage( hWnd, WM_DESTROY, 0, 0 );
				break;
			}

			//	�o�^�ł��Ă��Ȃ����Ƃ��m�F����
			if( Shell_NotifyIcon( NIM_MODIFY, &gstNtfyIcon) )
			{
				return S_OK;	//	���͓o�^�ł��Ă�
			}
			else
			{
				Sleep( 500 );	//	�o�^�ł��Ă��Ȃ�����
			}

			crdCount++;
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�^�X�N�g���C�A�C�R���ŃC�x���g������������
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	uID		�C�x���g�����������g���C�A�C�R���̂h�c
	@param[in]	message	�}�E�X�A�L�[�{�[�h�̑����WMmessage
	@return ���ɂȂ�
*/
VOID TaskTrayIconEvent( HWND hWnd, UINT uID, UINT message )
{
	POINT	stPoint;
	HMENU	hMenu, hSubMenu;

	//	�}�E�X�̉E�����{�^���������L��

	GetCursorPos( &stPoint );

	if( WM_RBUTTONUP == message )	//	���j���[�ďo
	{
		SetForegroundWindow( hWnd );//�������Ȃ��ƃ|�b�v�A�b�v�������Ȃ��炵��

		//	�|�b�v�A�b�v���j���[���J��
		hMenu = LoadMenu( ghInst, MAKEINTRESOURCE(IDC_ORINRINCOLLECTOR) );
		hSubMenu = GetSubMenu( hMenu, 0 );

		if( gbClipSteal )	CheckMenuItem( hSubMenu, IDM_CLIPSTEAL_TOGGLE, MF_CHECKED );

		TrackPopupMenu( hSubMenu, 0, stPoint.x, stPoint.y, 0, hWnd, NULL );
		DestroyMenu( hMenu );
	}

	if( WM_LBUTTONUP == message )	//	�I���̃A��
	{
		SetForegroundWindow( hWnd );//�������Ȃ��ƃ|�b�v�A�b�v�������Ȃ��炵��

		hMenu = CreateFileSelMenu( hWnd, 0 );
		TrackPopupMenu( hMenu, 0, stPoint.x, stPoint.y, 0, hWnd, NULL );
		DestroyMenu( hMenu );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C���I���|�b�p�[���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	bStyle	��O�����ON/OFF�����@�O�t�@�C���I���̂�
	@return		��������j���[�̃n���h���E�Ă񂾑��ŏ�������
*/
HMENU CreateFileSelMenu( HWND hWnd, UINT bStyle )
{
	HWND	hLvWnd = GetDlgItem( hWnd, IDLV_CLIPSTEAL_FILELISTVW );
	INT		iCount, i;
	HMENU	hMenu;
	TCHAR	atName[MAX_PATH], atPath[MAX_PATH];

	iCount = ListView_GetItemCount( hLvWnd );

	hMenu = CreatePopupMenu(  );
	if( 0 == iCount )
	{
		AppendMenu( hMenu, MF_STRING, IDM_CLIPSTEAL_SELECT, TEXT("�i�فj") );
		EnableMenuItem( hMenu, IDM_CLIPSTEAL_SELECT, MF_GRAYED );
	}
	else
	{
		if( bStyle )
		{
			AppendMenu( hMenu, MF_STRING, IDM_CLIPSTEAL_TOGGLE, TEXT("�R�s�[���e�𒸑Ղ���") );
			AppendMenu( hMenu, MF_SEPARATOR, 0, NULL );

			if( gbClipSteal )	CheckMenuItem( hMenu, IDM_CLIPSTEAL_TOGGLE, MF_CHECKED );
		}

		for( i = 0; iCount > i; i++ )
		{
			FileListViewGet( hWnd, i, atPath );
			StringCchCopy( atName, MAX_PATH, atPath );
			PathStripPath( atName );
			AppendMenu( hMenu, MF_STRING, IDM_CLIPSTEAL_SELECT+i, atName );

			//	�g�p���̂�����o���ă`�F�b�N����
			if( 0 == StrCmp( gatClipFile, atPath ) )
			{
				CheckMenuItem( hMenu, IDM_CLIPSTEAL_SELECT+i, MF_CHECKED );
			}
		}
	}

	return hMenu;
}
//-------------------------------------------------------------------------------------------------

/*!
	�󋵂ɍ��킹�ăA�C�R���̕\�����b�Z�[�W��ύX����
	@param[in]	hWnd	�E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT TaskTrayIconCaptionChange( HWND hWnd )
{
	gstNtfyIcon.uFlags = NIF_TIP;

	StringCchPrintf( gstNtfyIcon.szTip, 128, TEXT("OrinrinCollector �[ %s"), gbClipSteal ? TEXT("����>�@<����") : TEXT("�y�y�y�D�D�D") );

	Shell_NotifyIcon( NIM_MODIFY, &gstNtfyIcon );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�^�X�N�g���C�A�C�R���ɂ΂��`��߂��[�����ڂ���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	ptInfo	�o���[���̖{��
	@param[in]	ptTitle	�o���[���̃^�C�g��
	@param[in]	dIconTy	�������A�C�R���A�P���A�Q�x���A�R�G���[
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT TaskTrayIconBalloon( HWND hWnd, LPTSTR ptInfo, LPTSTR ptTitle, DWORD dIconTy )
{
	gstNtfyIcon.uFlags       = NIF_INFO;
	StringCchCopy( gstNtfyIcon.szInfoTitle, 64, ptTitle );
	StringCchCopy( gstNtfyIcon.szInfo, 256, ptInfo );
	gstNtfyIcon.uTimeout     = 11111;	//	�����łP�P�b���炢�\�������Ă݂�E�����ĂȂ�
	gstNtfyIcon.dwInfoFlags  = dIconTy;	//	�p����A�C�R���^�C�v
	Shell_NotifyIcon( NIM_MODIFY, &gstNtfyIcon );

//	NIIF_INFO       0x00000001
//	NIIF_WARNING    0x00000002
//	NIIF_ERROR      0x00000003

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�N���b�v�{�[�h�r���[���`�F�C������N�������ꂽ�Ƃ��̌q����������
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	hWndRemove	�`�F�C�����痣���E�C���h�E�n���h��
	@param[in]	hWndNext	�����HWND�́A���̂�̃E�C���h�E�n���h��
*/
VOID Cls_OnChangeCBChain( HWND hWnd, HWND hWndRemove, HWND hWndNext )
{
	//	�����n���h�����A�����̎��̃��c��������A���̍s����ύX���Ċ����B
	if( hWndRemove == ghNextViewer ){	ghNextViewer = hWndNext;	}
	else if( hWndNext ){	SendMessage( hWndNext, WM_CHANGECBCHAIN, (WPARAM)hWndRemove, (LPARAM)hWndNext );	}
	//	�֌W�Ȃ��Ȃ�A�������e�����ɓn���B
	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�N���b�v�{�[�h�̓��e���ύX���ꂽ�ꍇ�̊m�ۏ����B�����ŏ������񂾏ꍇ�������B
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Cls_OnDrawClipboard( HWND hWnd )
{
	OutputDebugString( TEXT("�N���b�v�{�[�h���e�m�F\r\n") );

	//	�ύX�����������Ƃ��A���̃r���[���ɒm�点�Ă���
	if( ghNextViewer )	SendMessage( ghNextViewer, WM_DRAWCLIPBOARD, 0, 0 );

	ClipStealDoing( hWnd );

	return;
}
//-------------------------------------------------------------------------------------------------


/*!
	�N���b�v�{�[�h�ɕύX���������ꍇ�̏����E��������̕ύX�ɔ������Ȃ��悤�ɂ��ɂ�
	@param[in]	hWnd	�E�C���h�E�n���h���E�g��Ȃ���
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT ClipStealDoing( HWND hWnd )
{
	LPTSTR		ptTexts;
	LPSTR		pcStrs;
	CHAR		acBuffer[MAX_STRING];
	TCHAR		atMsg[MAX_STRING];

	UINT_PTR	cbSize, cbSplSz;
	DWORD		wrote;

	HANDLE	hFile;

	SYSTEMTIME	stTime;


	//	�ۑ�����t�@�C���w�肪�����Ȃ�i�j�����Ȃ�
	if( NULL == gatClipFile[0] ){	return E_NOTIMPL;	}

	//	�N���b�v�X�e�B�[���@�\�L���H
	if( !(gbClipSteal) ){	return  S_FALSE;	}


	ptTexts = ClipboardDataGet( NULL );
	if( !(ptTexts) ){	return E_ACCESSDENIED;	}
	//	�N���b�v���ꂽ�̂�������ł͂Ȃ������璼���I���

	//	SJIS�^�ɕϊ�
	pcStrs = SjisEncodeAlloc( ptTexts );
	StringCchLengthA( pcStrs, STRSAFE_MAX_CCH, &cbSize );

	//	�Q�b�g�����R�s�y������̕ۑ�����
	hFile = CreateFile( gatClipFile, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE != hFile )
	{
		SetFilePointer( hFile, 0, NULL, FILE_END );

		if( gIsAST )
		{
			GetLocalTime( &stTime );
			StringCchPrintfA( acBuffer, MAX_STRING, ("[AA][%04u/%02u/%02u %02u:%02u:%02u]\r\n"),
				stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond );
			StringCchLengthA( acBuffer, MAX_STRING, &cbSplSz );
			WriteFile( hFile, acBuffer, cbSplSz, &wrote, NULL );
		}

		WriteFile( hFile, pcStrs, cbSize, &wrote, NULL );

		if( gIsAST ){	StringCchCopyA( acBuffer, MAX_STRING, ("\r\n") );	}
		else{	StringCchCopyA( acBuffer, MAX_STRING, ("\r\n[SPLIT]\r\n") );	}
		StringCchLengthA( acBuffer, MAX_STRING, &cbSplSz );
		WriteFile( hFile, acBuffer, cbSplSz, &wrote, NULL );

		SetEndOfFile( hFile );
		CloseHandle( hFile );
	}

	FREE( ptTexts );
	FREE( pcStrs );

	//	�ۑ��������b�Z�[�W���K�v
	if( gGetMsgOn )
	{
		StringCchPrintf( atMsg, MAX_STRING, TEXT("%u Byte �擾"), cbSize );
		TaskTrayIconBalloon( hWnd, TEXT("�R�s�[���ꂽ�������ۑ�������B"), atMsg, 1 );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�N���b�v�{�[�h�̃f�[�^�����������E�������m���ADocInsDelCtrl.cpp �ɂ���
	@param[in]	pVoid	���ɂȂ�
	@return		�m�ۂ���������Emalloc���Ă�̂ŁA�����Ă񂾕���free�Y��Ȃ��悤��
*/
LPTSTR ClipboardDataGet( LPVOID pVoid )
{
	LPTSTR	ptString = NULL, ptClipTxt;
	LPSTR	pcStr, pcClipTp;	//	�ϊ��p�Վ�
	DWORD	cbSize;
	UINT	dEnumFmt;
	INT		ixCount, iC;
	HANDLE	hClipData;

	//	�N���b�v�{�[�h�̒��g���`�F�L�E�ǂ����ɂ��Ă����j�R�[�h�e�L�X�g�t���O�͂���
	if( IsClipboardFormatAvailable( CF_UNICODETEXT ) )
	{
		OpenClipboard( NULL );	//	�N���b�v�{�[�h���I�[�|������
		//	�J�����������Ƒ��̃A�v���ɖ��f�Ȃ̂ł����߂�悤��

		dEnumFmt = 0;	//	�����l�͂O
		ixCount = CountClipboardFormats(  );
		for( iC = 0; ixCount > iC; iC++ )
		{	//	���Ԃɗ񋓂��āA��Ƀq�b�g�����t�H�[�}�b�g�ň���
			dEnumFmt = EnumClipboardFormats( dEnumFmt );
			if( CF_UNICODETEXT == dEnumFmt || CF_TEXT == dEnumFmt ){	break;	}
		}
		if( 0 == dEnumFmt ){	return NULL;	}
		//	����ȏ�񋓂��������A�������s�Ȃ�O�ɂȂ�

		//	�N���b�v�{�[�h�̃f�[�^���Q�b�c�I
		//	�n���h���̃I�[�i�[�̓N���b�v�{�[�h�Ȃ̂ŁA�����炩��͑��삵�Ȃ��悤��
		//	���g�̕ύX�Ȃǂ����Ă̂ق��ł���
		hClipData = GetClipboardData( dEnumFmt );

		if( CF_UNICODETEXT == dEnumFmt )
		{
			//	�擾�f�[�^�������BTEXT�Ȃ�A�n���h���̓O���[�o���������n���h��
			//	�V���ɃR�s�[���ꂽ��n���h���͖����ɂȂ�̂ŁA���g���R�s�[����
			ptClipTxt = (LPTSTR)GlobalLock( hClipData );
			cbSize    = GlobalSize( (HGLOBAL)hClipData );
			//	�m�ۏo����̂̓o�C�g�T�C�Y�E�e�L�X�g���Ɩ�����NULL�^�[�~�l�[�^�܂�

			if( 0 < cbSize )
			{
				ptString = (LPTSTR)malloc( cbSize );
				StringCchCopy( ptString, (cbSize / 2), ptClipTxt );
			}
		}
		else	//	�񃆃j�R�[�h���D�悳��Ă���ꍇ
		{
			pcClipTp = (LPSTR)GlobalLock( hClipData );
			cbSize   = GlobalSize( (HGLOBAL)hClipData );

			if( 0 < cbSize )
			{
				pcStr = (LPSTR)malloc( cbSize );
				StringCchCopyA( pcStr, cbSize, pcClipTp );

				ptString = SjisDecodeAlloc( pcStr );	//	SJIS�̓��e�����j�R�[�h�ɂ���
				free( pcStr );
			}
		}


		//	�g���I���������Ă���
		GlobalUnlock( hClipData );
		CloseClipboard(  );
	}

	return ptString;
}
//-------------------------------------------------------------------------------------------------

