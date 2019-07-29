/*! @file
	@brief �t�@�C���ɕۑ��E�J���̖ʓ|���܂�
	���̃t�@�C���� DocFileCtrl.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/04/27
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
#ifdef _DEBUG
#include <chrono>
#endif
//-------------------------------------------------------------------------------------------------

//	TODO:	�ۑ�����Ƃ��A�������O�̃t�@�C��������΁A���������Ď����Ńo�b�N�A�b�v�����

//	TODO:	�ۑ�����Ƃ��A���j�R�[�h��SJIS���I���o����悤�ɁE�f�t�H��SJIS�ł�����


extern list<ONEFILE>	gltMultiFiles;	//!<	�����t�@�C���ێ�
extern FILES_ITR	gitFileIt;			//!<	�����Ă�t�@�C���̖{��
extern INT		gixFocusPage;			//!<	���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X

extern  UINT	gbAutoBUmsg;			//		�����o�b�N�A�b�v���b�Z�[�W�o�����H

extern  UINT	gbSaveMsgOn;			//		�ۑ����b�Z�[�W�o�����H

static TCHAR	gatBackUpDirty[MAX_PATH];

//-------------------------------------------------------------------------------------------------

INT	DocAstSeparatorGetAlloc( FILES_ITR, INT, UINT, LPVOID * );

INT	DocUnicode2UTF8( LPVOID * );
//-------------------------------------------------------------------------------------------------


/*!
	�Y������t�@�C���͊J���ς�
	@param[in]	ptFile	�m�F�������t�@�C����
	@return	UINT	���F�����@�P�ȏ�F�q�b�g�������UNIQUE�ԍ�
*/
LPARAM DocOpendFileCheck( LPTSTR ptFile )
{
	FILES_ITR	itFile;

	for( itFile = gltMultiFiles.begin(); gltMultiFiles.end() != itFile; itFile++ )
	{
		//	�q�b�g�����炻��ł���
		if( !( StrCmp( itFile->atFileName, ptFile ) ) ){	return  itFile->dUnique;	}
	}

	return -1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C������ǂݍ���
	@param[in]	hWnd	�e�ɂ���E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocFileOpen( HWND hWnd )
{
	OPENFILENAME	stOpenFile;
	BOOLEAN	bOpened;
//	LPARAM	dNumber;
	TCHAR	atFilePath[MAX_PATH], atFileName[MAX_STRING];


	ZeroMemory( &stOpenFile, sizeof(OPENFILENAME) );

	ZeroMemory( atFilePath,  sizeof(atFilePath) );
	ZeroMemory( atFileName,  sizeof(atFileName) );

	stOpenFile.lStructSize     = sizeof(OPENFILENAME);
	stOpenFile.hwndOwner       = hWnd;
	stOpenFile.lpstrFilter     = TEXT("�A�X�L�[�A�[�g�t�@�C�� ( mlt, ast, txt )\0*.mlt;*.ast;*.txt\0�S�Ă̌`��(*.*)\0*.*\0\0");
	stOpenFile.nFilterIndex    = 1;
	stOpenFile.lpstrFile       = atFilePath;
	stOpenFile.nMaxFile        = MAX_PATH;
	stOpenFile.lpstrFileTitle  = atFileName;
	stOpenFile.nMaxFileTitle   = MAX_STRING;
//	stOpenFile.lpstrInitialDir = 
	stOpenFile.lpstrTitle      = TEXT("�J���t�@�C�����w�肵�Ă�����");
	stOpenFile.Flags           = OFN_EXPLORER | OFN_HIDEREADONLY;
	stOpenFile.lpstrDefExt     = TEXT("mlt");

	//������ FileOpenDialogue ���o��
	bOpened = GetOpenFileName( &stOpenFile );

	ViewFocusSet(  );

	if( !(bOpened) ){	return  E_ABORT;	}	//	�L�����Z�����Ă��牽�����Ȃ�

	DocDoOpenFile( hWnd, atFilePath );	//	�t�@�C�����w�肵�ēǂݍ��ގ�

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C�������󂯂āA�I�[�|����������
	@param[in]	hWnd	�e�ɂ���E�C���h�E�n���h��
	@param[in]	ptFile	�J���t�@�C���t���p�X
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT DocDoOpenFile( HWND hWnd, LPTSTR ptFile )
{
	LPARAM	dNumber;

	//	�����̃t�@�C�����J�����Ƃ����炻�������t�H�[�J�X���邾���ɂ���̂��ǂ��͂�
	dNumber = DocOpendFileCheck( ptFile );
	if( 1 <= dNumber )	//	�����̃t�@�C���q�b�g�E�������Ɉړ�����
	{
		if( SUCCEEDED( MultiFileTabSelect( dNumber ) ) )	//	�Y���̃^�u�Ƀt�H�[�J�X�ڂ���
		{
			DocMultiFileSelect( dNumber );	//	���̃^�u�̃t�@�C����\��
			return S_OK;
		}
	}

	dNumber = DocFileInflate( ptFile  );	//	�t�@�C�������󂯂āA�J���Ē��g�W�J
	if( !(dNumber) )
	{
		MessageBox( hWnd, TEXT("�t�@�C�����J���Ȃ�������"), TEXT("���ӂ���̂��m�点"), MB_OK | MB_ICONERROR );
		return E_HANDLE;
	}
	else
	{
		MultiFileTabAppend( dNumber, ptFile );	//	�_�C�����O����t�@�C���I�[�|��
		OpenHistoryLogging( hWnd , ptFile );	//	�t�@�C���I�[�|���L�^��ǉ�
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


/*!
	�o�b�N�A�b�v�f�B���N�g���[���m��
	@param[in]	ptCurrent	��f�B���N�g��
*/
VOID DocBackupDirectoryInit( LPTSTR ptCurrent )
{
	StringCchCopy( gatBackUpDirty, MAX_PATH, ptCurrent );
	PathAppend( gatBackUpDirty, BACKUP_DIR );
	CreateDirectory( gatBackUpDirty, NULL );
	//	�߂Ƀf�B���N�g�����������生�������s���邾���Ȃ̂Ŗ�薳��

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�C���^�[�o���Ŏ����ۑ�
	@param[in]	hWnd	�E�C���h�E�n���h���E�ǂ��̃E�C���h�E�n���h�����͔�Œ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocFileBackup( HWND hWnd )
{
	CONST  TCHAR	aatExte[3][5] = { {TEXT(".ast")}, {TEXT(".mlt")}, {TEXT(".txt")} };

	TCHAR	atFilePath[MAX_PATH], atFileName[MAX_STRING];
	TCHAR	atBuffer[MAX_PATH];

	HANDLE	hFile;
	DWORD	wrote;

	LPTSTR	ptExten;	//	�t�@�C�����̊g���q
	TCHAR	atExBuf[10];

	LPVOID	pBuffer;	//	������o�b�t�@�p�|�C���^�[
	INT		iByteSize, iNullTmt, iCrLf;

	LPVOID	pbSplit;
	UINT	cbSplSz = 0; //  WriteFile����Ƃ��̏����o�C�g��

	INT		isAST, isMLT, idExten;

	UINT_PTR	iPages, i;	//	�Ő�

	FILES_ITR	itFile;

	ZeroMemory( atFilePath, sizeof(atFilePath) );
	ZeroMemory( atFileName, sizeof(atFileName) );
	ZeroMemory( atBuffer,   sizeof(atBuffer) );

//�����t�@�C���A�e�t�@�C�����Z�[�u����ɂ́H
	for( itFile = gltMultiFiles.begin(); itFile != gltMultiFiles.end(); itFile++ )
	{
		iPages = itFile->vcCont.size( );	//	���Ő�

		if( 1 >= iPages )	isMLT = FALSE;
		else				isMLT = TRUE;

		isAST = PageListIsNamed( itFile );	//	�łɖ��O���t���Ă�H

		if( isAST ){		idExten = 0;	}	//	AST
		else if( isMLT ){	idExten = 1;	}	//	MLT
		else{				idExten = 2;	}	//	TXT

		StringCchCopy( atBuffer, MAX_PATH, itFile->atFileName );

		if( atBuffer[0] == NULL )	//	���̖��ݒ���
		{
			StringCchCopy( atFileName, MAX_STRING, itFile->atDummyName );
		}
		else
		{
			PathStripPath( atBuffer );
			StringCchCopy( atFileName, MAX_STRING, atBuffer );
		}

		//	�g���q���m�F�E�h�b�g���݂���`��
		ptExten = PathFindExtension( atFileName );	//	�g���q�������Ȃ�NULL�A�Ƃ��������[�ɂȂ�
		if( 0 == *ptExten )
		{
			//	�g���q�w�肪�Ȃ��Ȃ炻�̂܂ܑΉ��̂���������
			StringCchCopy( ptExten, 5, aatExte[idExten] );
		}
		else	//	�����̊g���q����������
		{
			StringCchCopy( atExBuf, 10, ptExten );
			CharLower( atExBuf );	//	��r�̂��߂ɏ������ɂ����Ⴄ

			if( isAST )	//	AST�͗D��I�ɓK�p
			{
				if( StrCmp( atExBuf , aatExte[0] ) )	//	����AST����Ȃ�������ύX
				{
					StringCchCopy( ptExten, 5, aatExte[0] );
				}
			}
			else if( isMLT )	//	���O�������Ǖ����łȂ�MLT����Ȃ��ƃ_��
			{
				if( StrCmp( atExBuf , aatExte[1] ) )	//	����MLT����Ȃ�������ύX
				{
					StringCchCopy( ptExten, 5, aatExte[1] );
				}
			}
			//	�ꖇ�Ȃ�ATXT�ł�MLT�ł��C�ɂ��Ȃ��Ă悩�΂�
		}

		StringCchCopy( atFilePath, MAX_PATH, gatBackUpDirty );
		PathAppend( atFilePath, atFileName );	//	Backup�t�@�C����


		hFile = CreateFile( atFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
		if( INVALID_HANDLE_VALUE == hFile )
		{
			NotifyBalloonExist( TEXT("�o�b�N�A�b�v���o���Ȃ�������E�E�E"), TEXT("�ُ픭��"), NIIF_ERROR );
			//	gbAutoBUmsg	�o�b�N�A�b�v�o���Ȃ��������b�Z�[�W�͏�ɕ\����������
			return E_HANDLE;
		}

		iNullTmt = 1;
		iCrLf = CH_CRLF_CCH;
		SetFilePointer( hFile, 0, NULL, FILE_BEGIN );

		if( isAST )
		{
			pbSplit = NULL;
		}
		else
		{
			pbSplit = malloc( 30 );
			ZeroMemory( pbSplit, 30 );

			cbSplSz = MLT_SPRT_CCH + CH_CRLF_CCH;
			StringCchPrintfA( (LPSTR)pbSplit, 30, ("%s%s"), MLT_SEPARATERA, CH_CRLFA );
		}

		for( i = 0; iPages > i; i++ )	//	�S�ŕۑ�
		{
			if( isAST )
			{
				cbSplSz = DocAstSeparatorGetAlloc( itFile, i, D_SJIS, &pbSplit );

				WriteFile( hFile , pbSplit, (cbSplSz- iNullTmt), &wrote, NULL );
				FREE(pbSplit);
			}
			else
			{
				if( 1 <= i ){	WriteFile( hFile , pbSplit, cbSplSz, &wrote, NULL );	}
			}

			iByteSize = DocPageTextGetAlloc( itFile, i, D_SJIS, &pBuffer, TRUE );

			if( (i+1) == iPages ){	iByteSize -=  iCrLf;	}
			//	�ŏI�ł̖��[�̉��s�͕s�v�̂͂�
			WriteFile( hFile, pBuffer, iByteSize - iNullTmt, &wrote, NULL );

			FREE( pBuffer );
		}

		SetEndOfFile( hFile );
		CloseHandle( hFile );

		FREE( pbSplit );
	}

	if( gbAutoBUmsg ){	NotifyBalloonExist( TEXT("��ƒ��̃t�@�C�����o�b�N�A�b�v�ۑ�������B"), TEXT("���ӂ���̂��m�点"), NIIF_INFO );	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C���ɕۑ�����
	@param[in]	hWnd	�e�ɂ���E�C���h�E�n���h��
	@param[in]	bStyle	�㏑�������l�[�����E�t�H�[�}�b�g�I���̓_�C�����O�ł��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocFileSave( HWND hWnd, UINT bStyle )
{
#ifdef _DEBUG
	std::chrono::system_clock::time_point  start, end; // �^�� auto �ŉ�
	start = std::chrono::system_clock::now(); // �v���J�n����
	double elapsed = 0.0;
	OutputDebugString(TEXT("DocFileSave\n"));
#endif

	CONST  TCHAR	aatExte[3][5] = { {TEXT(".ast")}, {TEXT(".mlt")}, {TEXT(".txt")} };
	CONST  WCHAR	rtHead = 0xFEFF;	//	���j�R�[�h�e�L�X�g�w�b�_

	SYSTEMTIME		stSysTile;
	OPENFILENAME	stSaveFile;

	BOOLEAN	bOpened;

	TCHAR	atFilePath[MAX_PATH], atFileName[MAX_STRING];
	TCHAR	atBuffer[MAX_STRING];

	HANDLE	hFile;
	DWORD	wrote;

	LPTSTR	ptExten;	//	�t�@�C�����̊g���q
	TCHAR	atExBuf[10];

	LPVOID	pBuffer;	//	������o�b�t�@�p�|�C���^�[
	INT		iByteSize, iNullTmt, iCrLf;

	LPVOID	pbSplit;
	UINT	cbSplSz;

	INT		isAST, isMLT, idExten, mbRslt;
	BOOLEAN	bExtChg =FALSE, bLastChg = FALSE;
	BOOLEAN	bForceMLT = FALSE;
	BOOLEAN	bNoName = FALSE;

	BOOLEAN	bUtf8 = FALSE;	//	�t�s�e�W�ŕۑ��Z��
	BOOLEAN	bUnic = FALSE;	//	���j�R�[�h�ŕۑ��Z��

	UINT_PTR	iPages, i;	//	�Ő�

	ZeroMemory( &stSaveFile, sizeof(OPENFILENAME) );

	ZeroMemory( atFilePath,  sizeof(atFilePath) );
	ZeroMemory( atFileName,  sizeof(atFileName) );
	ZeroMemory( atBuffer,  sizeof(atBuffer) );

	//	�ۑ����͏�ɑI�����Ă���t�@�C����ۑ�

	iPages = DocNowFilePageCount( );	//	���Ő�
	if( 1 >= iPages )	isMLT = FALSE;
	else				isMLT = TRUE;

//�����̊g���q��AST�Ȃ�A�����D�悷��

	isAST = PageListIsNamed( gitFileIt );	//	�łɖ��O���t���Ă�H

	//if( isAST ){		idExten = 0;	}	//	AST
	//else if( isMLT ){	idExten = 1;	}	//	MLT
	//else{				idExten = 2;	}	//	TXT
	//	txt�͎g�p���Ȃ����Ƃɂ���
	if( isAST ){	idExten = 0;	}	//	AST
	else{			idExten = 1;	}	//	MLT


	GetLocalTime( &stSysTile );

	StringCchCopy( atFilePath, MAX_PATH, (*gitFileIt).atFileName );

	if( NULL == (*gitFileIt).atFileName[0] )	bNoName = TRUE;

	//	���l�[�����A�t�@�C����������������ۑ��_�C�����O�J��
	if( (bStyle & D_RENAME) || bNoName )
	{

		//������ FileSaveDialogue ���o��
		stSaveFile.lStructSize     = sizeof(OPENFILENAME);
		stSaveFile.hwndOwner       = hWnd;
		stSaveFile.lpstrFilter     = TEXT("[ShiftJIS]�A�X�L�[�A�[�g�t�@�C�� ( mlt, ast, txt )\0*.mlt;*.ast;*.txt\0[UTF8]�A�X�L�[�A�[�g�t�@�C�� ( mlt, ast, txt )\0*.mlt;*.ast;*.txt\0\0");
		stSaveFile.nFilterIndex    = 1;	//	�f�t�H�̃t�B���^�I����
		stSaveFile.lpstrFile       = atFilePath;
		stSaveFile.nMaxFile        = MAX_PATH;
		stSaveFile.lpstrFileTitle  = atFileName;
		stSaveFile.nMaxFileTitle   = MAX_STRING;
//		stSaveFile.lpstrInitialDir = 
		stSaveFile.lpstrTitle      = TEXT("�ۑ�����t�@�C�������w�肵�Ă�");
		stSaveFile.Flags           = OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
		//�f�t�H���g�g���q�̎w��E���ƂŖʓ|���Ă邩�炱���ł͂��Ȃ������ǂ�
		//	stSaveFile.lpstrDefExt = TEXT("ast");
		//	stSaveFile.lpstrDefExt = TEXT("mlt");

		bOpened = GetSaveFileName( &stSaveFile );

		ViewFocusSet(  );

		if( !(bOpened) ){	return  E_ABORT;	}
		//	�L�����Z�����Ă��牽�����Ȃ�

		//�I�������t�B���^�ԍ����P�C���f�b�N�X�œ���
		if(  2 == stSaveFile.nFilterIndex ){	bUtf8 = TRUE;	}
		//	�t�s�e�W�ŕۑ��E���j�R�[�h����ϊ�����΂�낵

		if( bUnic || bUtf8 )
		{	//	�������̂܂܃G�N�X�|�[�g���悤�Ƃ��Ă��疳��
			if( NULL == (*gitFileIt).atFileName[0] )
			{
				MessageBox( hWnd, TEXT("��ɒʏ�̕ۑ������Ă���G�N�X�|�[�g���ĂˁB"), TEXT("���ӂ���̂��m�点"), MB_OK | MB_ICONINFORMATION );
				return E_FAIL;
			}
		}
		else{	bLastChg = TRUE;	}	//	�V�K�ۑ��E���l�[���ۑ�
		//	����t�H�[�}�b�g�̏ꍇ�̓G�N�X�|�[�g�Ƃ��A������Ԃɂ͉e�����Ȃ��悤�ɂ���
	}

	//	�g���q���m�F�E�h�b�g���݂���`��E�g���q�̈ʒu�̃|�C���^�m��
	ptExten = PathFindExtension( atFilePath );	//	�g���q�������Ȃ疖�[�ɂȂ�
	if( 0 == *ptExten )
	{
		//	�g���q�w�肪�Ȃ��Ȃ炻�̂܂ܑΉ��̂���������
		StringCchCopy( ptExten, 5, aatExte[idExten] );
		bExtChg = TRUE;
	}
	else	//	�����̊g���q����������
	{
		StringCchCopy( atExBuf, 10, ptExten );
		CharLower( atExBuf );	//	��r�̂��߂ɏ������ɂ����Ⴄ

		//	�����̊g���q���AAST�Ȃ炻���D�悷��
		if( !( StrCmp( atExBuf, aatExte[0] ) ) )	//	AST�ł���Ȃ�
		{
			//	AST�`�����ێ�����
			isAST = TRUE;	isMLT = FALSE;	idExten = 0;
		}

		//	�ۑ�����g���q��MLT�ŁA������AST���烊�l�[���Ȃ�m�F
		if( !( StrCmp( atExBuf, aatExte[1] ) ) )
		{
			if( isAST && (bStyle & D_RENAME) )	//	����AST�����l�[���Ȃ�
			{
				mbRslt = MessageBox( hWnd, TEXT("MLT�ŕۑ�����ƕŖ��̂��Ȃ��Ȃ����Ⴄ��B\r\n����ł��ǂ������H"), TEXT("���ӂ���̊m�F"), MB_OKCANCEL | MB_ICONQUESTION );
				if( IDOK != mbRslt )	return E_ABORT;

				isMLT = TRUE;	isAST = FALSE;	idExten = 1;
				bForceMLT = TRUE;
			}
		}

		if( isAST )	//	AST�͗D��I�ɓK�p
		{
			if( StrCmp( atExBuf , aatExte[0] ) )	//	����AST����Ȃ�������ύX
			{
				StringCchCopy( ptExten, 5, aatExte[0] );
				bExtChg = TRUE;
			}
		}
		else if( isMLT )	//	���O�������Ǖ����łȂ�MLT����Ȃ��ƃ_��
		{
			if( StrCmp( atExBuf , aatExte[1] ) )	//	����MLT����Ȃ�������ύX
			{
				StringCchCopy( ptExten, 5, aatExte[1] );
				bExtChg = TRUE;
			}
		}
		//	�ꖇ�Ȃ�ATXT�ł�MLT�ł��C�ɂ��Ȃ��Ă悩�΂�
	}


	//	�㏑���Ȃ璼�O�̏�Ԃ̃o�b�N�A�b�v�Ƃ����ׂ�
	//	�������O�̃t�@�C��������΁A���Ă��Ƃ�

	//	�I���W�i���t�@�C�����ɒ���
	if( !(bUnic) &&  !(bUtf8) ){	StringCchCopy( (*gitFileIt).atFileName, MAX_PATH, atFilePath );	}

	hFile = CreateFile( atFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE == hFile )
	{
		MessageBox( hWnd, TEXT("�t�@�C�����J���Ȃ�������"), TEXT("���ӂ���̂��m�点"), MB_OK | MB_ICONERROR );
		return E_HANDLE;
	}

	iNullTmt = 1;
	iCrLf = CH_CRLF_CCH;
	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );

	if( bUnic )	//	���j�R�[�h���[�h�Ȃ�ABOM������
	{
		WriteFile( hFile, &rtHead, 2, &wrote, NULL );
		iNullTmt = 2;
		iCrLf *= 2;
	}

	if( isAST )
	{
		pbSplit = NULL;
	}
	else
	{
		pbSplit = malloc( 30 );
		ZeroMemory( pbSplit, 30 );

		if( bUnic )
		{
			cbSplSz = (MLT_SPRT_CCH + CH_CRLF_CCH) * sizeof(TCHAR);
			StringCchPrintfW( (LPTSTR)pbSplit, 15, TEXT("%s%s"), MLT_SEPARATERW, CH_CRLFW );
		}
		else	//	UTF8�̏ꍇ�́AASCII�����͂��̂܂܂ł���
		{
			cbSplSz = MLT_SPRT_CCH + CH_CRLF_CCH;
			StringCchPrintfA( (LPSTR)pbSplit, 30, ("%s%s"), MLT_SEPARATERA, CH_CRLFA );
		}
	}

	//	�{���̎捞�̓��j�R�[�h�ł��K�v������
	if( bUnic || bUtf8 ){	bStyle |= D_UNI;	}

#ifdef _DEBUG
	end = std::chrono::system_clock::now();  // �v���I������
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); //�����ɗv�������Ԃ��~���b�ɕϊ�
	OutputDebugString(TEXT("Before Write"));
	_RPT1(_CRT_WARN, "ELAPSED:%f", elapsed);
#endif
	for( i = 0; iPages > i; i++ )	//	�S�ŕۑ�
	{
		if( isAST )	//	�`�r�s�̏ꍇ�́A�Ő擪�Ƀ^�C�g���������Ă�
		{
			//	�Ԃ�l�̊m�ۃo�C�g���ɂ͂m�t�k�k�^�[�~�l�[�^�܂�ł�̂Œ���
			cbSplSz = DocAstSeparatorGetAlloc( gitFileIt, i, bStyle, &pbSplit );

			if( bUtf8 ){	cbSplSz = DocUnicode2UTF8( &pbSplit );	}
			//	pbSplit�̒��g��t���ւ���

			WriteFile( hFile , pbSplit, (cbSplSz- iNullTmt), &wrote, NULL );
			FREE(pbSplit);
		}
		else	//	MLT�̏ꍇ�́A��ڈȍ~�ŋ�؂肪�K�v
		{
			if( 1 <= i ){	WriteFile( hFile , pbSplit, cbSplSz, &wrote, NULL );	}
			if( bForceMLT ){	DocAstSeparatorGetAlloc( gitFileIt, i, 0, NULL );	}
		}

		iByteSize = DocPageTextGetAlloc( gitFileIt, i, bStyle, &pBuffer, TRUE );

		if( bUtf8 ){	iByteSize = DocUnicode2UTF8( &pBuffer );	}
		//	pBuffer�̒��g��t���ւ���

		if( (i+1) == iPages ){	iByteSize -=  iCrLf;	}	//	�ŏI�ł̖��[�̉��s�͕s�v�̂͂�
		WriteFile( hFile, pBuffer, iByteSize - iNullTmt, &wrote, NULL );

		FREE( pBuffer );
	}

	SetEndOfFile( hFile );
	CloseHandle( hFile );

#ifdef _DEBUG
	end = std::chrono::system_clock::now();  // �v���I������
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); //�����ɗv�������Ԃ��~���b�ɕϊ�
	OutputDebugString(TEXT("After Write"));
	_RPT1(_CRT_WARN, "ELAPSED:%f", elapsed);
#endif
	FREE( pbSplit );

	//	�G�N�X�|�[�g�Ȃ̂ŕۑ����ĂȂ����Ƃ�
	if( !(bUnic) &&  !(bUtf8) ){	DocModifyContent( FALSE );	}

	//	�Ȃ񂩃��b�Z�[�W
	if( bExtChg )	//	�g���q�ύX�����ꍇ
	{
		//InitLastOpen( INIT_SAVE, atFilePath );	//	���X�g�I�[�|��������
		MultiFileTabRename( (*gitFileIt).dUnique, atFilePath );	//	�^�u���̕ύX
		AppTitleChange( atFilePath );
		StringCchPrintf( atBuffer, MAX_STRING, TEXT("�g���q�� %s �ɂ��ĕۑ�������B"), aatExte[idExten] );
		NotifyBalloonExist( atBuffer, TEXT("���ӂ���̂��m�点"), NIIF_INFO );

		OpenHistoryLogging( hWnd , atFilePath );	//	�t�@�C�����ύX�����̂ŋL�^��蒼��
	}
	else
	{
		//	20110713	�V�K�����l�[�����Ă��烉�X�g�I�[�|��������
		if( bLastChg )
		{
			//InitLastOpen( INIT_SAVE, atFilePath );
			MultiFileTabRename( (*gitFileIt).dUnique, atFilePath );	//	�^�u���̕ύX
			AppTitleChange( atFilePath );

			OpenHistoryLogging( hWnd , atFilePath );	//	�t�@�C�����ύX�����̂ŋL�^��蒼��
		}

		if( bUnic || bUtf8 )
		{
			NotifyBalloonExist( TEXT("�t�@�C���̃G�N�X�|�[�g������B"), TEXT("���ӂ���̂��m�点"), NIIF_INFO );
		}
		else
		{
			if( gbSaveMsgOn ){	NotifyBalloonExist( TEXT("�t�@�C����ۑ�������B"), TEXT("���ӂ���̂��m�点"), NIIF_INFO );	}
		}
	}

	//	�ňꗗ�̏�������
	if( bForceMLT ){	PageListViewRewrite( -1 );	}
#ifdef _DEBUG
	end = std::chrono::system_clock::now();  // �v���I������
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); //�����ɗv�������Ԃ��~���b�ɕϊ�
	OutputDebugString(TEXT("END"));
	_RPT1(_CRT_WARN,"ELAPSED:%f", elapsed);
#endif
	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���j�R�[�h��������󂯎���āA�t�s�e�W�̃A�h���X���m�ۂ��Ă��ǂ��B
	@param[in,out]	pText	���I���j�R�[�h��������E���I�t�s�e�W����������B�������̈�������
	@return	INT	�m�ۂ����o�C�g���ENULL�^�[�~�l�[�^�܂�
*/
INT DocUnicode2UTF8( LPVOID *pText )
{
	UINT_PTR	cchSz;	//	���j�R�[�h�p
	INT	cbSize, rslt;	//	UTF8�p
	LPVOID		pUtf8;	//	�m��

	StringCchLength( (LPTSTR)(*pText), STRSAFE_MAX_CCH, &cchSz );

	//	�K�v�o�C�g���m�F
	cbSize = WideCharToMultiByte( CP_UTF8, 0, (LPTSTR)(*pText), -1, NULL, 0, NULL, NULL );
	TRACE( TEXT("cbSize[%d]"), cbSize );
	pUtf8 = (LPSTR)malloc( cbSize );
	ZeroMemory( pUtf8, cbSize );
	rslt = WideCharToMultiByte( CP_UTF8, 0, (LPTSTR)(*pText), -1, (LPSTR)(pUtf8), cbSize, NULL, NULL );
	TRACE( TEXT("rslt[%d]"), rslt );

	FREE( *pText );	//	���j�R�[�h������̂ق��͔j�󂷂�

	*pText = pUtf8;	//	�t�s�e�W�̂ق��ɕt���ւ���

	return cbSize;
}
//-------------------------------------------------------------------------------------------------

/*!
	�y�[�W���O��AST��؂�t���Ŋm�ۂ���Efree�͌Ă񂾕��ł��
	@param[in]	itFile	�m�ۂ���Ŕԍ�
	@param[in]	dPage	�m�ۂ���Ŕԍ�
	@param[in]	bStyle	�P���j�R�[�h���V�t�gJIS�ŁA��`���ǂ���
	@param[out]	pText	�m�ۂ����̈��Ԃ��E���C�h�������}���`�����ɂȂ�ENULL�Ȃ�Ŗ����폜����
	@return				�m�ۂ����o�C�g���ENULL�^�[�~�l�[�^�܂�
*/
INT DocAstSeparatorGetAlloc( FILES_ITR itFile, INT dPage, UINT bStyle, LPVOID *pText )
{
	UINT	cchSize, cbSize;
	TCHAR	atBuffer[MAX_STRING];

	StringCchPrintf( atBuffer, MAX_STRING, TEXT("[AA][%s]\r\n"), itFile->vcCont.at( dPage ).atPageName );
	StringCchLength( atBuffer, MAX_STRING, &cchSize );

	if( !(pText) )
	{
		ZeroMemory( itFile->vcCont.at( dPage ).atPageName, SUB_STRING * sizeof(TCHAR) );
		return 0;
	}

	if( bStyle & D_UNI )
	{
		cbSize = (cchSize + 1) *  sizeof(TCHAR);	//	NULL�^�[�~�l�[�^

		*pText = (LPTSTR)malloc( cbSize );
		ZeroMemory( *pText, cbSize );
		StringCchCopy( (LPTSTR)(*pText), (cchSize + 1), atBuffer );
	}
	else
	{
		cbSize = WideCharToMultiByte( CP_ACP, WC_NO_BEST_FIT_CHARS, atBuffer, cchSize, NULL, 0, NULL, NULL );
		cbSize++;	//	NULL�^�[�~�l�[�^
		*pText = (LPSTR)malloc( cbSize );
		ZeroMemory( *pText, cbSize );
		WideCharToMultiByte( CP_ACP, WC_NO_BEST_FIT_CHARS, atBuffer, cchSize, (LPSTR)(*pText), cbSize, NULL, NULL );
	}

	return cbSize;
}
//-------------------------------------------------------------------------------------------------Yippee-ki-yay!

/*!
	�摜�ŕł�ۑ��EBMP��PNG�AJPEG�͌����ĂȂ�
	@param[in]	hWnd	�e�ɂ���E�C���h�E�n���h��
	@param[in]	bStyle	�Ȃ񂩃t���O�E�Ƃ�₦�����g�p
	@param[in]	hFont	�`��Ɏg���t�H���g�󂯎��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocImageSave( HWND hWnd, UINT bStyle, HFONT hFont )
{

	LPVOID	pBuffer;
	LPTSTR	ptText;
	UINT	dLines;
	INT		iDotX, iDotY, iByteSize, bType;
	UINT_PTR	cchSize;
	RECT	rect;

	INT	iLine;
	UINT_PTR	cchLen, start, caret = 0;

	BOOL	bOpened;
	OPENFILENAME	stSaveFile;

	TCHAR	atOutName[MAX_PATH], atFileName[MAX_STRING];
	TCHAR	atPart[MIN_STRING];

	HDC		hdc, hMemDC;
	HBITMAP	hBitmap,hOldBmp;
	HFONT	hOldFont;



	//	�Ƃ肠���Ã_�~�[���O�Ńt�@�C��
	StringCchCopy( atOutName, MAX_PATH, gitFileIt->atFileName );
	//	�g���q���I����D�悷��悤�ɂ����Ⴄ
	PathRemoveExtension( atOutName );	//	�g���q���ځ`��

	StringCchPrintf( atPart, MIN_STRING, TEXT("_Page%d"), gixFocusPage );
	StringCchCat( atOutName, MAX_PATH, atPart );

	ZeroMemory( &stSaveFile, sizeof(OPENFILENAME) );
	stSaveFile.lStructSize     = sizeof(OPENFILENAME);
	stSaveFile.hwndOwner       = hWnd;
	stSaveFile.lpstrFilter     = TEXT("BMP �t�@�C�� ( *.bmp )\0*.bmp\0PNG �t�@�C�� ( *.png )\0*.png\0\0");
	stSaveFile.nFilterIndex    = 1;	//	�f�t�H�̃t�B���^�I����
	stSaveFile.lpstrFile       = atOutName;
	stSaveFile.nMaxFile        = MAX_PATH;
	stSaveFile.lpstrFileTitle  = atFileName;
	stSaveFile.nMaxFileTitle   = MAX_STRING;
//		stSaveFile.lpstrInitialDir = 
	stSaveFile.lpstrTitle      = TEXT("�ۑ�����t�@�C�����ƌ`�����w�肵�Ă�����");
	stSaveFile.Flags           = OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;

	bOpened = GetSaveFileName( &stSaveFile );

	ViewFocusSet(  );

	if( !(bOpened) ){	return  E_ABORT;	}

	//�I�������t�B���^�ԍ����P�C���f�b�N�X�œ���
	switch( stSaveFile.nFilterIndex )
	{
		default:	bType = ISAVE_BMP;	break;
		case  2:	bType = ISAVE_PNG;	break;
	}

	dLines = DocNowFilePageLineCount(  );//DocPageParamGet( NULL , NULL );	//	�v��͍̂s��
	iDotX  = DocPageMaxDotGet( -1, -1 );
	iDotY  = dLines * LINE_HEIGHT;
	//	������Ɨ]�T����Ƃ�
	iDotX += 8;
	iDotY += 8;

	SetRect( &rect, 4, 4, iDotX - 4, iDotY- 4 );

	TRACE( TEXT("�T�C�Y %d x %d"), iDotX, iDotY );

	iByteSize = DocPageTextGetAlloc( gitFileIt, gixFocusPage, D_UNI, &pBuffer, TRUE );
	ptText = (LPTSTR)pBuffer;
	StringCchLength( ptText, STRSAFE_MAX_CCH, &cchSize );

	//	�`��p�r�b�g�}�b�v�쐬
	hdc = GetDC( hWnd );

	hBitmap = CreateCompatibleBitmap( hdc, iDotX, iDotY );
	hMemDC  = CreateCompatibleDC( hdc );

	hOldBmp  = SelectBitmap( hMemDC, hBitmap );
	hOldFont = SelectFont( hMemDC, hFont );

	PatBlt( hMemDC, 0, 0, iDotX, iDotY, WHITENESS );

	ReleaseDC( hWnd, hdc );

	iLine  = 0;
	cchLen = 0;
	start  = 0;
	//	������S�̂����Ă���
	for( caret = 0; cchSize > caret; )
	{
		if( TEXT('\r') == ptText[caret] )	//	��s�̏I���
		{
			TextOut( hMemDC, 0, iLine, &(ptText[start]), cchLen );
			cchLen = 0;	//	���������Z�b�g
			caret += 2;	//	���̍s�̊J�n�ʒu
			start = caret;	//	�J�n�ʒu�m�F

			iLine += LINE_HEIGHT;	//	�`��x�ʒu
		}
		else
		{
			cchLen++;
			caret++;
		}
	}
	//	�Ō�̍s�`��
	TextOut( hMemDC, 0, iLine, &(ptText[start]), cchLen );


	FREE(pBuffer);

	if( SUCCEEDED( ImageFileSaveDC( hMemDC, atOutName, bType ) ) )
	{
		//	��������
		TRACE( TEXT("�ۑ� %s"), atOutName );
	}
	else
	{
		//	�����ς�
		TRACE( TEXT("���s %s"), atOutName );
	}

	SelectBitmap( hMemDC, hOldBmp );
	DeleteBitmap( hBitmap );

	SelectFont( hMemDC, hOldFont );

	DeleteDC( hMemDC );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	MLT2HTML���g���āA���J���Ă���t�@�C����HTML�G�N�X�|�[�g
	@param[in]	hWnd	�e�ɂ���E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocHtmlExport( HWND hWnd )
{
	TCHAR	atFilePath[MAX_PATH], atCommandLine[BIG_STRING + 10];
	TCHAR	atExePath[MAX_PATH];

	 PROCESS_INFORMATION	stProInfo;
	 STARTUPINFO	stStartInfo;



	ZeroMemory( atFilePath,  sizeof(atFilePath) );

	StringCchCopy( atFilePath, MAX_PATH, (*gitFileIt).atFileName );

	//	���J���Ă���t�@�C�������ۑ��Ȃ�A�`���[�V
	if( gitFileIt->dModify || ( NULL == atFilePath[0] ) )
	{
		MessageBox( hWnd, TEXT("��Ƀt�@�C����ۑ����Ă���ɂ��ĂˁB"), TEXT("�t�@�C�����ۑ�����ĂȂ���"), MB_OK | MB_ICONERROR );
		return E_ABORT;
	}
	PathQuoteSpaces( atFilePath );

	ZeroMemory( atExePath, sizeof(atExePath) );
	InitParamString( INIT_LOAD, VS_EXT_M2H_PATH, atExePath );
	if( NULL == atExePath[0] )
	{
		MessageBox( hWnd, TEXT("MLT2HTML.exe ��ݒ肵�Ă����ĂˁB"), TEXT("�O���c�[����������"), MB_OK | MB_ICONERROR );
		return E_ABORT;
	}
	PathQuoteSpaces( atExePath );

	ZeroMemory( atCommandLine,  sizeof(atCommandLine) );

	StringCchPrintf( atCommandLine, BIG_STRING + 10, TEXT("%s %s"), atExePath, atFilePath );
	//	�p�X�Ɋ܂܂��X�y�[�X������Ƃ��������Ȃ�����R�}���̂ŃN�I�[�g���Ă���

	ZeroMemory( &stProInfo, sizeof(PROCESS_INFORMATION) );

	ZeroMemory( &stStartInfo, sizeof(STARTUPINFO) );
	stStartInfo.cb = sizeof(STARTUPINFO);

	CreateProcess( NULL, atCommandLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &stStartInfo, &stProInfo );

	CloseHandle( stProInfo.hThread );

	WaitForSingleObject( stProInfo.hProcess, INFINITE );	//	�����E�G�C�g�E���ԂȂ������H

	CloseHandle( stProInfo.hProcess );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

