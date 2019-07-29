/*! @file
	@brief �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
	���̃t�@�C���� OrinrinViewer.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/08/18
*/
//-------------------------------------------------------------------------------------------------

/*
Orinrin Viewer : AsciiArt Viewer for Japanese Only
Copyright (C) 2011 - 2013 Orinrin/SikigamiHNQ

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.

����{�鍑���p��́�������ꂽ��
*/


#include "stdafx.h"
#include "OrinrinEditor.h"
#include "MaaTemplate.h"
//------------------------------------------------------------------------------------------------------------------------

static CONST TCHAR	gcatLicense[] = {
TEXT("���̃v���O�����̓t���[�\�t�g�E�F�A�ł��B���Ȃ��͂�����A�t���[�\�t�g�E�F�A���c�ɂ���Ĕ��s���ꂽGNU��ʌ��O���p������(�o�[�W����3���A����ȍ~�̃o�[�W�����̂����ǂꂩ)����߂�����̉��ōĔЕz�܂��͉��ς��邱�Ƃ��ł��܂��B\r\n\r\n")
TEXT("���̃v���O�����͗L�p�ł��邱�Ƃ�����ĔЕz����܂����A*�S���̖��ۏ�*�ł��B���Ɖ\���̕ۏ؂����ړI�ւ̓K�����́A���O�Ɏ����ꂽ���̂��܂߁A�S�����݂��܂���B\r\n\r\n")
TEXT("�ڂ�����GNU��ʌ��O���p���������������������B\r\n\r\n")
TEXT("���Ȃ��͂��̃v���O�����Ƌ��ɁAGNU��ʌ��O���p�������̃R�s�[���ꕔ�󂯎���Ă���͂��ł��B\r\n\r\n")
TEXT("�����󂯎���Ă��Ȃ���΁A<http://www.gnu.org/licenses/> ���������������B\r\n\r\n")
};

//-------------------------------------------------------------------------------------------------

//	TODO:	�d�l������������
//	TODO:	��Ɏ�O�ɕ\���Ƀ`�F�L������Ȃ�
//	TODO:	�G�L�X�g���t�@�C���ǉ������瑽�d�ɂȂ�

/*
�E�N�����j���[��Editor�Ɠ��ꂷ��E���f�[�^
���C�����j���[�͂Ȃ���

IDC_ORINRINVIEWER MENU
BEGIN
    POPUP "�@�\(&F)"
    BEGIN
        MENUITEM "�v���t�@�C���쐬�^�J��(&N)",	IDM_MAA_PROFILE_MAKE
        MENUITEM "AA�ꗗ�c���[���č\�z(&T)",	IDM_TREE_RECONSTRUCT
		MENUITEM "�v���t�@�C���g�p����(&H)",	IDM_OPEN_HISTORY
        MENUITEM SEPARATOR
        MENUITEM "��ʐݒ�(&G)",				IDM_GENERAL_OPTION
        MENUITEM "�t�@�C�����Ō���(&F)",		IDM_FINDMAA_DLG_OPEN
        MENUITEM SEPARATOR
        MENUITEM "�h���t�g�{�[�h�\��(&B)",		IDM_DRAUGHT_OPEN
        MENUITEM SEPARATOR
        MENUITEM "��Ɏ�O�ɕ\��(&A)",			IDM_TOPMOST_TOGGLE
        MENUITEM SEPARATOR
        MENUITEM "�o�[�W�������(&I)",			IDM_ABOUT
        MENUITEM SEPARATOR
        MENUITEM "�I��(&Q)",					IDM_EXIT
    END
END

IDM_AATREE_POPUP MENU
BEGIN
    POPUP "�����s�e���v���c���[�̃A��"
    BEGIN
        MENUITEM "��^�u�ŊJ��(&M)",		IDM_AATREE_MAINOPEN
        MENUITEM "���^�u��ǉ�(&S)",		IDM_AATREE_SUBADD
        MENUITEM SEPARATOR
        MENUITEM "�h���t�g�{�[�h�\��(&B)",	IDM_DRAUGHT_OPEN
    END
END

*/

//-------------------------------------------------------------------------------------------------

//	�\���p�t�H���g�x�[�X�e�[�u���E������R�s�[���Ďg��
static LOGFONT	gstBaseFont = {
	FONTSZ_NORMAL,			//	�t�H���g�̍���
	0,						//	���ϕ�
	0,						//	��������̕�����X���Ƃ̊p�x
	0,						//	�x�[�X���C����X���Ƃ̊p�x
	FW_NORMAL,				//	�����̑���(0~1000�܂ŁE400=nomal)
	FALSE,					//	�C�^���b�N��
	FALSE,					//	�A���_�[���C��
	FALSE,					//	�ł�������
	DEFAULT_CHARSET,		//	�����Z�b�g
	OUT_OUTLINE_PRECIS,		//	�o�͐��x
	CLIP_DEFAULT_PRECIS,	//	�N���b�s���O���x
	PROOF_QUALITY,			//	�o�͕i��
	VARIABLE_PITCH,			//	�Œ蕝���ϕ�
	TEXT("�l�r �o�S�V�b�N")	//	�t�H���g��
};
//-------------------------------------------------------------------------------------------------

static  UINT	gdUseMode;		//!<	�}�����C���N���b�v�w���E�ݒ�ɒ���
static  UINT	gdUseSubMode;	//!<	

static  HWND	ghMaaWnd;		//!<	���ꂽ�E�C���h�E�n���h��
static TCHAR	gatIniPath[MAX_PATH];	//!<	�h�m�h�t�@�C���̈ʒu

extern  HWND	ghMaaFindDlg;	//!<	MAA�����_�C�����O�n���h��

extern HFONT	ghAaFont;		//!<	�\���p�̃t�H���g

extern  UINT	gdClickDrt;	//

extern HMENU	ghProfHisMenu;	//	����\�����镔���E���I�ɓ��e�쐬���Ȃ�����
//------------------------------------------------------------------------------------------------------------------------

BOOLEAN	SelectFolderDlg( HWND, LPTSTR, UINT_PTR );

HRESULT	ViewingFontNameLoad( VOID );
//------------------------------------------------------------------------------------------------------------------------


/*!
	�A�v���P�[�V�����̃G���g���|�C���g
	@param[in]	hInstance		���̃��W���[���̃C���X�^���X�n���h��
	@param[in]	hPrevInstance	�O�̃C���X�^���X�B���͖��g�p
	@param[in]	lpCmdLine		�R�}���h���C���B�g�[�N�������͂���ĂȂ��A�����̕�����
	@param[in]	nCmdShow		�N�����̕\����Ԃ������Ă�B�\���Ƃ�����������
	@retval FALSE	�r���I��
*/
INT APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//	TODO: �����ɃR�[�h��}�����Ă��������B
	MSG		msg;
	HACCEL	hAccelTable;
	INT		msRslt;

#ifdef _DEBUG
	//_CRTDBG_ALLOC_MEM_DF;		// �w�肪�K�v�ȃt���O
	//_CRTDBG_CHECK_ALWAYS_DF;	//	���������`�F�b�N		_CRTDBG_CHECK_EVERY_128_DF
	//_CRTDBG_LEAK_CHECK_DF;		//	�I�����Ƀ��������[�N���`�F�b�N
	//_CRTDBG_DELAY_FREE_MEM_DF;	//	
	//	�����Ŏg�p����t���O���w��
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	INITCOMMONCONTROLSEX	iccex;
	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccex.dwICC  = ICC_WIN95_CLASSES;
	InitCommonControlsEx( &iccex );

	//	�ݒ�t�@�C���ʒu�m�F
	GetCurrentDirectory( MAX_PATH, gatIniPath );
	PathAppend( gatIniPath, INI_FILE );

	SplitBarClass( hInstance );	//	�X�v���b�g�o�[�̏���

	gdUseMode    = InitParamValue( INIT_LOAD, VL_MAA_LCLICK, MAA_SJISCLIP );
	gdUseSubMode = InitParamValue( INIT_LOAD, VL_MAA_MCLICK, MAA_SJISCLIP );

	ViewingFontNameLoad(  );	//	�t�H���g���m��

	//	�A�v���P�[�V�����̏����������s���܂�:
	ghMaaWnd = MaaTmpltInitialise( hInstance, GetDesktopWindow(), NULL );
	if( !(ghMaaWnd) )	return (-1);

#ifdef USE_HOVERTIP
	HoverTipInitialise( hInstance, ghMaaWnd );
#endif

	DraughtInitialise( hInstance, ghMaaWnd );
	gdClickDrt = gdUseMode;

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ORINRINVIEWER));

	//	���C�����b�Z�[�W���[�v
	for(;;)
	{
		msRslt = GetMessage( &msg, NULL, 0, 0 );
		if( 1 != msRslt )	break;

		//	MAA�����_�C�����O
		if( ghMaaFindDlg )
		{	//�g�b�v�ɗ��Ă邩�ǂ������f����
			if( ghMaaFindDlg == GetForegroundWindow(  ) )
			{
				if( TranslateAccelerator( ghMaaFindDlg, hAccelTable, &msg ) )	continue;
				if( IsDialogMessage( ghMaaFindDlg, &msg ) )	continue;
			}
		}

		if( !TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}


//-------------------------------------------------------------------------------------------------

//	�o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���ł��B
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch( message )
	{
		case WM_INITDIALOG:
			SetDlgItemText( hDlg, IDE_ABOUT_DISP, gcatLicense );
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------


#pragma region ("�ݒ���e�Ǐ�")

/*!
	�p�����[�^�l�̃Z�[�u���[�h�EEditor���ɂ�����
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
		case  VL_MAA_SPLIT:		StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaSplit") );		break;
		case  VL_MAA_LCLICK:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaMethod") );		break;
		case  VL_MAATIP_VIEW:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaToolTip")  );	break;
		case  VL_MAATIP_SIZE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaToolTipSize") );	break;
		case  VL_MAA_TOPMOST:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaTopMost")  );	break;
		case  VL_MAASEP_STYLE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaSepLine")  );	break;
		case  VL_MAA_MCLICK:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaSubMethod") );	break;
		case  VL_MAA_BKCOLOUR:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaBkColour") );	break;
		case  VL_THUMB_HORIZ:	StringCchCopy( atKeyName, SUB_STRING, TEXT("ThumbHoriz")  );	break;
		case  VL_THUMB_VERTI:	StringCchCopy( atKeyName, SUB_STRING, TEXT("ThumbVerti")  );	break;
		case  VL_MAATAB_SNGL:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaTabSingle") );	break;	//	20130521
		default:	return nValue;
	}

	if( dMode  )	//	���[�h
	{
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), nValue );
		GetPrivateProfileString( TEXT("General"), atKeyName, atBuff, atBuff, MIN_STRING, gatIniPath );
		dBuff = StrToInt( atBuff );
	}
	else	//	�Z�[�u
	{
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), nValue );
		WritePrivateProfileString( TEXT("General"), atKeyName, atBuff, gatIniPath );
	}

	return dBuff;
}
//-------------------------------------------------------------------------------------------------


/*!
	������̐ݒ���e���Z�[�u���[�h
	@param[in]		dMode	��O���[�h�@�O�Z�[�u
	@param[in]		dStyle	�p�����[�^�̎��
	@param[in,out]	ptFile	MAX_PATH�ł��邱��
	@return			HRESULT	�I����ԃR�[�h
*/
HRESULT InitParamString( UINT dMode, UINT dStyle, LPTSTR ptFile )
{
	TCHAR	atKeyName[MIN_STRING], atDefault[MAX_PATH];

	if(  !(ptFile) )	return E_INVALIDARG;

	switch( dStyle )
	{
		case VS_PROFILE_NAME:	StringCchCopy( atKeyName, SUB_STRING, TEXT("ProfilePath") );	break;
		case VS_PAGE_FORMAT:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PageFormat")  );	break;
		case VS_FONT_NAME:		StringCchCopy( atKeyName, SUB_STRING, TEXT("FontName") );		break;
		default:	return E_INVALIDARG;
	}

	if( dMode )	//	���[�h
	{
		StringCchCopy( atDefault, MAX_PATH, ptFile );
		GetPrivateProfileString( TEXT("General"), atKeyName, atDefault, ptFile, MAX_PATH, gatIniPath );
	}
	else
	{
		WritePrivateProfileString( TEXT("General"), atKeyName, ptFile, gatIniPath );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


/*!
	�E�C���h�E�ʒu�̃Z�[�u���[�h�EEditor���ɂ�����
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	dStyle	�P�r���[�@�Q����
	@param[in]	pstRect	���[�h���ʂ����邩�A�Z�[�u���e������
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT InitWindowPos( UINT dMode, UINT dStyle, LPRECT pstRect )
{
	TCHAR	atAppName[MIN_STRING], atBuff[MIN_STRING];

	if( !pstRect )	return E_INVALIDARG;

	switch( dStyle )
	{
		case  WDP_MAATPL:	StringCchCopy( atAppName, SUB_STRING, TEXT("MaaTmple") );	break;
		default:	SetRect( pstRect , 0, 0, 0, 0 );	return E_INVALIDARG;
	}

	if( dMode )	//	���[�h
	{
		GetPrivateProfileString( atAppName, TEXT("LEFT"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
		pstRect->left   = StrToInt( atBuff );
		GetPrivateProfileString( atAppName, TEXT("TOP"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
		pstRect->top    = StrToInt( atBuff );
		GetPrivateProfileString( atAppName, TEXT("RIGHT"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
		pstRect->right  = StrToInt( atBuff );
		GetPrivateProfileString( atAppName, TEXT("BOTTOM"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
		pstRect->bottom = StrToInt( atBuff );
	}
	else	//	�Z�[�u
	{
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstRect->left );
		WritePrivateProfileString( atAppName, TEXT("LEFT"), atBuff, gatIniPath );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstRect->top );
		WritePrivateProfileString( atAppName, TEXT("TOP"), atBuff, gatIniPath );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstRect->right );
		WritePrivateProfileString( atAppName, TEXT("RIGHT"), atBuff, gatIniPath );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstRect->bottom );
		WritePrivateProfileString( atAppName, TEXT("BOTTOM"), atBuff, gatIniPath );
	}
	
	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�v���t������INI����ǂ񂾂菑������
	@param[in]		dMode	��O���[�h�@�O�Z�[�u
	@param[in]		dNumber	���[�h�Z�[�u�ԍ�
	@param[in,out]	ptFile	���[�h�F���g������@�Z�[�u�F�ۑ����镶����@MAX_PATH�ł��邱�ƁENULL�Ȃ���e����
	@return			HRESULT	�I����ԃR�[�h
*/
HRESULT InitProfHistory( UINT dMode, UINT dNumber, LPTSTR ptFile )
{
	TCHAR	atKeyName[MIN_STRING], atDefault[MAX_PATH];

	if( dMode  )	//	���[�h
	{
		ZeroMemory( ptFile, sizeof(TCHAR) * MAX_PATH );

		StringCchPrintf( atKeyName, MIN_STRING, TEXT("Hist%X"), dNumber );
		GetPrivateProfileString( TEXT("ProfHistory"), atKeyName, TEXT(""), atDefault, MAX_PATH, gatIniPath );

		if( NULL == atDefault[0] )	return E_NOTIMPL;	//	�L�^����

		StringCchCopy( ptFile, MAX_PATH, atDefault );
	}
	else	//	�Z�[�u
	{
		if( ptFile )
		{
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("Hist%X"), dNumber );
			WritePrivateProfileString( TEXT("ProfHistory"), atKeyName, ptFile, gatIniPath );
		}
		else	//	��U�S�폜
		{
			WritePrivateProfileSection( TEXT("ProfHistory"), NULL, gatIniPath );
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���j���[������������
*/
HRESULT OpenProfMenuModify( HWND hWnd )
{
	HMENU	hMenu, hSubMenu;

	hMenu = GetMenu( hWnd );
	hSubMenu = GetSubMenu( hMenu, 0 );	//	�@�\

	ModifyMenu( hSubMenu, 2, MF_BYPOSITION | MF_POPUP, (UINT_PTR)ghProfHisMenu, TEXT("�t�@�C���g�p����(&H)") );
	//������Œ�͂��܂�C�N�i�C

	DrawMenuBar( hWnd );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


#pragma endregion	//	("�ݒ���e�Ǐ�")


#pragma region ("�N���b�v���鏈��")
/*!
	�N���b�v�{�[�h�ɕ�����\��t���EEditor���ɂ�����
	@param[in]	pDatum	�\��t������������E���j��SJIS
	@param[in]	cbSize	������́ANULL���܂񂾃o�C�g��
	@param[in]	dStyle	��`���Ƃ����������w��
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT DocClipboardDataSet( LPVOID pDatum, INT cbSize, UINT dStyle )
{
	HGLOBAL	hGlobal;
	HANDLE	hClip;
	LPVOID	pBuffer;
	HRESULT	hRslt;
	UINT	ixFormat, ixSqrFmt;

	//	�I���W�i���t�H�[�}�b�g�����`���Ă���
	ixFormat = RegisterClipboardFormat( CLIP_FORMAT );
	ixSqrFmt = RegisterClipboardFormat( CLIP_SQUARE );

	//	�N���b�v����f�[�^�͋��L�������ɓ����
	hGlobal = GlobalAlloc( GHND, cbSize );
	pBuffer = GlobalLock( hGlobal );
	CopyMemory( pBuffer, pDatum, cbSize );
	GlobalUnlock( hGlobal );

	//	�N���b�v�{�[�h�I�[�|��
	OpenClipboard( NULL );

	//	���g���������Ⴄ
	EmptyClipboard(  );

	//	���L�������Ƀu�b���񂾃f�[�^���N���b�y����
	if( dStyle & D_UNI )	hClip = SetClipboardData( CF_UNICODETEXT, hGlobal );
	else					hClip = SetClipboardData( CF_TEXT, hGlobal );

	if( hClip )
	{
		//	�N���b�|����肭��������A�I���W�i�����ł��L�^���Ă���
		SetClipboardData( ixFormat, hGlobal );
		hRslt = S_OK;
	}
	else
	{
		//	�o�^���s�̏ꍇ�́A�����ŋ��L��������j�󂹂Ȃ�����
		GlobalFree( hGlobal );
		hRslt = E_OUTOFMEMORY;
	}

	//	�N���b�v�{�[�h����
	CloseClipboard(  );

	TRACE( TEXT("COPY DONE") );

	return hRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	MAA����SJIS���󂯎���ď�������EEditor���ɂ�����
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	pcCont	AA�̕�����
	@param[in]	cbSize	�o�C�g���E���[NULL�͊܂܂Ȃ�
	@param[in]	dMode	�g�p���[�h�E�f�t�H�������͌ʎw��
	@return		��O�f�t�H���삵���@�O�w�胂�[�h������
*/
UINT ViewMaaMaterialise( HWND hWnd, LPSTR pcCont, UINT cbSize, UINT dMode )
{
	LPTSTR		ptString;
	UINT_PTR	cchSize;
	UINT		uRslt = TRUE;	//	�f�t�H����ł���Ȃ�TRUE�����ł�TRUE�ɂ���

//	FLASHWINFO	stFshWInfo;

	//	�f�t�H����ł��邩�ǂ���
//	if( dMode == gdUseMode ){		uRslt = TRUE;	}
	if( MAA_DEFAULT ==  dMode ){	dMode = gdUseMode;	}
	if( MAA_SUBDEFAULT== dMode ){	dMode = gdUseSubMode;	}

	if( MAA_UNICLIP == dMode )	//	���j�R�[�h
	{
		ptString = SjisDecodeAlloc( pcCont );	//	���j�R�[�h�ɂ��Ă���
		StringCchLength( ptString, STRSAFE_MAX_CCH, &cchSize );

		//	���j�R�[�h�I�ɃN���b�y
		DocClipboardDataSet( ptString, (cchSize + 1) * 2, D_UNI );

		FREE(ptString);
	}
	else if( MAA_DRAUGHT == dMode ){	DraughtItemAdding( hWnd, pcCont );	}	//	�h���t�g�{�[�h�ɒǉ�
	else{	DocClipboardDataSet( pcCont, (cbSize + 1), D_SJIS );	}	//	SJIS�R�s�[


	//ZeroMemory( &stFshWInfo, sizeof(FLASHWINFO) );
	//stFshWInfo.cbSize    = sizeof(FLASHWINFO);	//	���̍\���̂̃T�C�Y
	//stFshWInfo.hwnd      = ghMaaWnd;	//	�����������������E�C���_�E�̃n���h��
	//stFshWInfo.dwFlags   = FLASHW_ALL;	//	�L���v�V�����ƃ^�X�N�o�[�̃{�^������
	//stFshWInfo.uCount    = 2;			//	���������������
	//stFshWInfo.dwTimeout = 0;			//	�Ԋu�B�O�Ńf�t�H���g�I�ȊԊu
	//FlashWindowEx( &stFshWInfo );		//	��������������

	return uRslt;
}
//-------------------------------------------------------------------------------------------------
#pragma endregion	//	("�N���b�v���鏈��")


#pragma region ("�ݒ�_�C�����O")

/*!
	���Ղ����_�C�����O�̃v���V�[�W��
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	message		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam		�ǉ��̏��P
	@param[in]	lParam		�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK OptionDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	UINT	id;
	INT		dValue, dBuff;

	switch( message )
	{
		case WM_INITDIALOG:
			//	 MAA�ꗗ	�A�C�e���폜
			//Edit_SetText( GetDlgItem(hDlg,IDE_AA_DIRECTORY), TEXT("�`�`�f�B���N�g���̓v���t�@�C������ݒ肵�Ă�") );
			//EnableWindow( GetDlgItem(hDlg,IDE_AA_DIRECTORY), FALSE );
			//ShowWindow( GetDlgItem(hDlg,IDB_AADIR_SEARCH), SW_HIDE );

			//	MAA�|�b�v�A�b�v�ɂ���
			dValue = InitParamValue( INIT_LOAD, VL_MAATIP_SIZE, 16 );	//	�T�C�Y�m�F
			if( FONTSZ_REDUCE == dValue )	CheckRadioButton( hDlg, IDRB_POPUP_NOMAL, IDRB_POPUP_REDUCE, IDRB_POPUP_REDUCE );
			else							CheckRadioButton( hDlg, IDRB_POPUP_NOMAL, IDRB_POPUP_REDUCE, IDRB_POPUP_NOMAL );

			dValue = InitParamValue( INIT_LOAD, VL_MAATIP_VIEW, 1 );	//	�|�b�p�b�v���邩
			CheckDlgButton( hDlg, IDCB_POPUP_VISIBLE, dValue ? BST_CHECKED : BST_UNCHECKED );

			//	�����s�e���v�����N���b�N�����Ƃ��̓���
			dValue = InitParamValue( INIT_LOAD, VL_MAA_LCLICK, MAA_SJISCLIP );
			switch( dValue )
			{
				case MAA_UNICLIP:	id = IDRB_SEL_CLIP_UNI;		break;
				default:
				case MAA_SJISCLIP:	id = IDRB_SEL_CLIP_SJIS;	break;
				case MAA_DRAUGHT:	id = IDRB_SEL_DRAUGHT;		break;
			}
			CheckRadioButton( hDlg, IDRB_SEL_INS_EDIT, IDRB_SEL_DRAUGHT, id );

			dValue = InitParamValue( INIT_LOAD, VL_MAA_MCLICK, MAA_SJISCLIP );
			switch( dValue )
			{
				case MAA_UNICLIP:	id = IDRB_SELSUB_CLIP_UNI;	break;
				default:
				case MAA_SJISCLIP:	id = IDRB_SELSUB_CLIP_SJIS;	break;
				case MAA_DRAUGHT:	id = IDRB_SELSUB_DRAUGHT;	break;
			}
			CheckRadioButton( hDlg, IDRB_SELSUB_INS_EDIT, IDRB_SELSUB_DRAUGHT, id );

			return (INT_PTR)TRUE;

		case WM_COMMAND:
			id = LOWORD(wParam);
			switch( id )
			{

				case IDB_APPLY://�K�p
				case IDOK:
					//	MAA�|�b�v�A�b�v�ɂ���
					dValue = FONTSZ_NORMAL;
					if( IsDlgButtonChecked( hDlg, IDRB_POPUP_REDUCE ) ){	dValue =  FONTSZ_REDUCE;	}
					InitParamValue( INIT_SAVE, VL_MAATIP_SIZE, dValue );
					dBuff = IsDlgButtonChecked( hDlg, IDCB_POPUP_VISIBLE );
					AaItemsTipSizeChange( dValue, dBuff );
					InitParamValue( INIT_SAVE, VL_MAATIP_VIEW, dBuff );

					//	MAA�̑���
					if(      IsDlgButtonChecked( hDlg, IDRB_SEL_CLIP_UNI ) ){	dValue = MAA_UNICLIP;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_SEL_DRAUGHT ) ){	dValue = MAA_DRAUGHT;	}
					else{	dValue = MAA_SJISCLIP;	}
					InitParamValue( INIT_SAVE, VL_MAA_LCLICK, dValue );
					gdUseMode = dValue;
					gdClickDrt = gdUseMode;

					if(      IsDlgButtonChecked( hDlg, IDRB_SELSUB_CLIP_UNI ) ){	dValue = MAA_UNICLIP;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_SELSUB_DRAUGHT )  ){	dValue = MAA_DRAUGHT;	}
					else{	dValue = MAA_SJISCLIP;	}
					InitParamValue( INIT_SAVE, VL_MAA_MCLICK, dValue );
					gdUseSubMode = dValue;

					//	�n�j�Ȃ�����Ⴄ
					if( IDOK == id ){	EndDialog( hDlg, IDOK );	}
					return (INT_PTR)TRUE;

				case IDCANCEL:
					EndDialog( hDlg, IDCANCEL );
					return (INT_PTR)TRUE;

				default:	break;
			}
			break;

		default:	break;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�f�B���N�g���I���_�C�A���O�̕\��
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	ptSelFolder	�f�B���N�g����������o�b�t�@�ւ̃|�C���^�[
	@param[in]	cchLen		�o�b�t�@�̕������B�o�C�g������Ȃ���
	@return		��O�F�f�B���N�g���Ƃ����@�O�F�L�����Z������
*/
BOOLEAN SelectDirectoryDlg( HWND hWnd, LPTSTR ptSelFolder, UINT_PTR cchLen )
{
	BROWSEINFO		stBrowseInfo;
	LPITEMIDLIST	pstItemIDList;
	TCHAR	atDisplayName[MAX_PATH];

	if( !(ptSelFolder) )	return FALSE;
	ZeroMemory( ptSelFolder, sizeof(TCHAR) * cchLen );

	//	BROWSEINFO�\���̂ɒl��ݒ�
	stBrowseInfo.hwndOwner		 = hWnd;	//	�_�C�A���O�̐e�E�C���h�E�̃n���h��
	stBrowseInfo.pidlRoot		 = NULL;	//	���[�g�f�B���N�g��������ITEMIDLIST�̃|�C���^�ENULL�̏ꍇ�f�X�N�g�b�v
	stBrowseInfo.pszDisplayName	 = atDisplayName;	//	�I�����ꂽ�f�B���N�g�������󂯎��o�b�t�@�̃|�C���^
	stBrowseInfo.lpszTitle		 = TEXT("�`�`�̓����Ă�f�B���N�g����I������́[�I");	//	�c���[�r���[�̏㕔�ɕ\������镶����
	stBrowseInfo.ulFlags		 = BIF_RETURNONLYFSDIRS;	//	�\�������f�B���N�g���̎�ނ������t���O
	stBrowseInfo.lpfn			 = NULL;		//	BrowseCallbackProc�֐��̃|�C���^
	stBrowseInfo.lParam			 = (LPARAM)0;	//	�R�[���o�b�N�֐��ɓn���l

	//	�f�B���N�g���I���_�C�A���O��\��
	pstItemIDList = SHBrowseForFolder( &stBrowseInfo );
	if( !(pstItemIDList) )
	{
		//	�߂�l��NULL�̏ꍇ�A�f�B���N�g�����I�����ꂸ�Ƀ_�C�A���O������ꂽ�Ƃ�������
		return FALSE;
	}
	else
	{
		//	ItemIDList���p�X���ɕϊ�
		if( !SHGetPathFromIDList( pstItemIDList, atDisplayName ) )
		{
			//	�G���[����
			return FALSE;
		}
		//	atDisplayName�ɑI�����ꂽ�f�B���N�g���̃p�X�������Ă�
		StringCchCopy( ptSelFolder, cchLen, atDisplayName );

		//	pstItemIDList���J�������߂�
		CoTaskMemFree( pstItemIDList );
	}

	return TRUE;
}
//-------------------------------------------------------------------------------------------------
#pragma endregion	//	("�ݒ�_�C�����O")


/*!
	�w�蕶���̕�����s��
	@param[in]	ch	�����v�肽������
	@return		���h�b�g��
*/
INT ViewLetterWidthGet( TCHAR ch )
{
	SIZE	stSize;
	HDC		hdc= GetDC( ghMaaWnd );
	HFONT	hFtOld;

	hFtOld = SelectFont( hdc, ghAaFont );

	GetTextExtentPoint32( hdc, &ch, 1, &stSize );

	SelectFont( hdc, hFtOld );

	ReleaseDC( ghMaaWnd, hdc );

	return stSize.cx;
}
//-------------------------------------------------------------------------------------------------
//	�{�̂�viewCentral
/*!
	������̃h�b�g���𐔂���
	@param[in]	ptStr	��������������
	@return		���h�b�g���E�O�Ȃ�G���[
*/
INT ViewStringWidthGet( LPCTSTR ptStr )
{
	SIZE	stSize;
	UINT	cchSize;
	HDC		hdc= GetDC( ghMaaWnd );
	HFONT	hFtOld;

	StringCchLength( ptStr, STRSAFE_MAX_CCH, &cchSize );

	if( 0 >= cchSize )	return 0;	//	�ُ펖��

	hFtOld = SelectFont( hdc, ghAaFont );

	GetTextExtentPoint32( hdc, ptStr, cchSize, &stSize );

	SelectFont( hdc, hFtOld );

	ReleaseDC( ghMaaWnd, hdc );

	return stSize.cx;
}
//-------------------------------------------------------------------------------------------------

/*!
	MAA�ꗗ����̎g�p���[�h���m��
	@return	�g�p���[�h�@�O�ʏ�}���@�P�����}���@�Q���C���@�R���j�R�s�[�@�SSJIS�R�s�[�@�T�h���t�g�{�[�h��
*/
UINT ViewMaaItemsModeGet( PUINT pdSubMode )
{
	if( pdSubMode ){	*pdSubMode = gdUseSubMode;	}

	return gdUseMode;
}
//-------------------------------------------------------------------------------------------------

/*!
	�\���p�t�H���g�̖��O�𒸂�
*/
HRESULT ViewingFontNameLoad( VOID )
{
	TCHAR	atName[LF_FACESIZE];

	ZeroMemory( atName, sizeof(atName) );	//	�f�t�H�l�[��
	StringCchCopy( atName, LF_FACESIZE, TEXT("�l�r �o�S�V�b�N") );

	InitParamString( INIT_LOAD, VS_FONT_NAME, atName );	//	�Q�b�c�I

	StringCchCopy( gstBaseFont.lfFaceName, LF_FACESIZE, atName );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�\���p�t�H���g�f�[�^���R�s�[����
	@param[in]	pstLogFont	�f�[�^�R�s��\���̂ւ̃|�C���^�[
*/
HRESULT ViewingFontGet( LPLOGFONT pstLogFont )
{
	ZeroMemory( pstLogFont, sizeof(LOGFONT) );	//	�O�̂��ߋ󔒂ɂ���

	*pstLogFont = gstBaseFont;
	//	�\���̂̓R�s�[�ł���
	return S_OK;
}
//-------------------------------------------------------------------------------------------------



#ifdef _DEBUG
VOID OutputDebugStringPlus( DWORD rixError, LPSTR pcFile, INT rdLine, LPSTR pcFunc, LPTSTR ptFormat, ... )
{
	va_list	argp;
	TCHAR	atBuf[MAX_PATH], atOut[MAX_PATH], atFiFu[MAX_PATH], atErrMsg[MAX_PATH];
	CHAR	acFile[MAX_PATH], acFiFu[MAX_PATH];
	UINT	length;

	StringCchCopyA( acFile, MAX_PATH, pcFile );
	PathStripPathA( acFile );

	StringCchPrintfA( acFiFu, MAX_PATH, ("%s %d %s"), acFile, rdLine, pcFunc );
	length = (UINT)strlen( acFiFu );

	ZeroMemory( atFiFu, sizeof(atFiFu) );
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, acFiFu, length, atFiFu, MAX_PATH );
	//	�R�[�h�y�[�W,�����̎�ނ��w�肷��t���O,�}�b�v��������̃A�h���X,�}�b�v��������̃o�C�g��,
	//	�}�b�v�惏�C�h�����������o�b�t�@�̃A�h���X,�o�b�t�@�̃T�C�Y

	va_start(argp, ptFormat);
	StringCchVPrintf( atBuf, MAX_PATH, ptFormat, argp );
	va_end( argp );

	StringCchPrintf( atOut, MAX_PATH, TEXT("%s @ %s\r\n"), atBuf, atFiFu );//

	OutputDebugString( atOut );

	if( rixError )
	{
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, rixError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), atErrMsg, MAX_PATH, NULL );
		//	���b�Z�[�W�ɂ͉��s���܂܂�Ă���悤��
		StringCchPrintf( atBuf, MAX_PATH, TEXT("[%d]%s"), rixError, atErrMsg );//

		OutputDebugString( atBuf );
		SetLastError( 0 );
	}
}
//-------------------------------------------------------------------------------------------------
#endif

