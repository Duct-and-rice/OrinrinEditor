/*! @file
	@brief �h�L�������g�̓��e�̊Ǘ������܂�
	���̃t�@�C���� DocCore.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/04/30
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
	�������́A�L�����b�g�̍����̕������Ő�����B�L�����b�g�����[�Ȃ�O������
*/

//-------------------------------------------------------------------------------------------------

//	�\���̐錾�̓R�����ֈړ�

//-------------------------------------------------------------------------------------------------

#define PAGE_LINE_MAX	80
#define LINE_MOZI_MAX	255
//-------------------------------------------------------------------------------------------------

/*
(*pstTexts).acSjis	�\���̃|�C���^�͂���ł�����

�|�C���^�m�ۂ͂������
vector<int>::iterator it = vec.begin();   // vec �� vector<int>�^�̕ϐ�
it++;
int* p = &*it;  // �����v�f�̃A�h���X���擾���悤�Ƃ��Ă���BOK

gstFile���A�}�N���ŃC�e���[�^�|�C���^�Ɏd���ďグ��
�t�H�[�J�X���Ă�t�@�C���͏�Ɉ�Ȃ̂ŁA�|�C���^�̒��g��ς��邩�A
�������̓}�N���ŃC�e���[�^���̂ɂ��Ă��܂��΂�����

(*ltrItr).cchMozi;	����ł������ۂ�
*/

//#define FILE_PRELOAD	//	��ɕł�vector���m�ۂ��Ă݂�E���X�g�ŗv��Ȃ��E�����p�ɂ��邩�H


EXTERNED list<ONEFILE>	gltMultiFiles;	//!<	�����t�@�C���ێ�
//�C�e���[�^��typedef�̓w�b�_��

static LPARAM	gdNextNumber;		//!<	�J�����t�@�C���̒ʂ��ԍ��E��ɃC���N��

EXTERNED FILES_ITR	gitFileIt;		//!<	�����Ă�t�@�C���̖{��

EXTERNED INT		gixFocusPage;	//!<	���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X

EXTERNED INT		gixDropPage;	//!<	�����z�b�g�ԍ�

extern  UINT		gbUniRadixHex;	//	���j�R�[�h���l�Q�Ƃ��P�U�i���ł��邩
extern  UINT		gbCrLfCode;		//	���s�R�[�h�F�O������΁E��O�x�x 
//-------------------------------------------------------------------------------------------------

UINT	CALLBACK DocPageLoad( LPTSTR, LPCTSTR, INT );
//-------------------------------------------------------------------------------------------------

/*!
	�Ȃ񂩏�����
*/
HRESULT DocInitialise( UINT dMode )
{
	FILES_ITR	itFile;
	PAGE_ITR	itPage;


	if( dMode )	//	�쐬��
	{
		gdNextNumber = 1;
	}
	else
	{
		for( itFile = gltMultiFiles.begin( ); itFile != gltMultiFiles.end(); itFile++ )
		{
			for( itPage = itFile->vcCont.begin( ); itPage != itFile->vcCont.end(); itPage++ )
			{
				FREE( itPage->ptRawData );
			}
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ύX������
	@param[in]	dMode	��O�ύX�����@�O�ۑ���������ύX�͂Ȃ��������Ƃ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocModifyContent( UINT dMode )
{
	if( dMode )
	{
		if( (*gitFileIt).dModify )	return S_FALSE;
		//	�ύX�̂Ƃ��A�߂ɕύX�̏������Ă��牽�����Ȃ��ėǂ�

		MainStatusBarSetText( SB_MODIFY, MODIFY_MSG );
	}
	else
	{
		MainStatusBarSetText( SB_MODIFY, TEXT("") );
	}

	DocMultiFileModify( dMode );

	(*gitFileIt).dModify =  dMode;	//	�����ŋL�^���Ă���

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�V�����t�@�C���u���������ăt�H�[�J�X����E�t�@�C���R�A����
	@param[in]	ptDmyName	�_�~�[����Ԃ��BNULL�BMAX_PATH�ł��邱��
	@return		LPARAM	�Ή����郆�j�[�N�ԍ�
*/
LPARAM DocMultiFileCreate( LPTSTR ptDmyName )
{
	ONEFILE	stFile;
	FILES_ITR	itNew;

#ifdef DO_TRY_CATCH
	try{
#endif

	ZeroMemory( stFile.atFileName, sizeof(stFile.atFileName) );
	stFile.dModify   = FALSE;
	stFile.dNowPage  = 0;
	stFile.dUnique   = gdNextNumber++;
	stFile.stCaret.x = 0;
	stFile.stCaret.y = 0;

	ZeroMemory( stFile.atDummyName, sizeof(stFile.atDummyName) );
	StringCchPrintf( stFile.atDummyName, MAX_PATH, TEXT("%s%d.%s"), NAME_DUMMY_NAME, stFile.dUnique, NAME_DUMMY_EXT );

	if( ptDmyName ){	StringCchCopy( ptDmyName, MAX_PATH, stFile.atDummyName );	}

	stFile.vcCont.clear(  );

	gltMultiFiles.push_back( stFile );

	//	�V�K�쐬�̏���
	gixFocusPage = -1;

	PageListClear(  );	//	�y�[�W���X�g�r���[���j��


	itNew = gltMultiFiles.end( );
	itNew--;	//	���[�ɒǉ��������炱��ł���

	gitFileIt = itNew;	//	�t�@�C���Ȃ�

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return ETC_MSG( err.what(), 0 );	}
	catch( ... ){	return  ETC_MSG( ("etc error"), 0 );	}
#endif

	return stFile.dUnique;
}
//-------------------------------------------------------------------------------------------------

/*!
	�N�����̊��S�V�K�쐬�E�J���t�@�C�����S�������ꍇ�̏���
	@param[in]	ptFile	�J�����t�@�C����Dummy����Ԃ��EMAX_PATH�ł��邱��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocActivateEmptyCreate( LPTSTR ptFile )
{
	INT	iNewPage;

	DocMultiFileCreate( ptFile );	//	�V�����t�@�C���u����̏����E�����ŕԂ茌�͗v��Ȃ�
	iNewPage = DocPageCreate( -1 );	//	�y�[�W����Ă���
	PageListInsert( iNewPage  );	//	�y�[�W���X�g�r���[�ɒǉ�
	DocPageChange( iNewPage );		//	���̕łɃt�H�[�J�X�����킹��
	MultiFileTabFirst( ptFile );	//	���S�V�K�쐬
	AppTitleChange( ptFile );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���e��ύX������^�u�̃t�@�C������[�ύX]����
	@param[in]	dMode	��O�ύX�����@�O�ύX�͂Ȃ��������Ƃ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocMultiFileModify( UINT dMode )
{
	TCHAR	atFile[MAX_PATH];	//!<	�t�@�C����

	StringCchCopy( atFile, MAX_PATH, (*gitFileIt).atFileName );
	if( 0 == atFile[0] ){	StringCchCopy( atFile, MAX_PATH , (*gitFileIt).atDummyName );	}

	PathStripPath( atFile );

	if( dMode ){	StringCchCat( atFile, MAX_PATH, MODIFY_MSG );	}

	MultiFileTabRename( (*gitFileIt).dUnique, atFile );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C���^�u��I�������E�t�@�C���R�A����
	@param[in]	uqNumber	�I�����ꂽ�t�@�C����UNIQUE�ԍ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocMultiFileSelect( LPARAM uqNumber )
{
	FILES_ITR	itNow;
	POINT	stCaret;

	for( itNow = gltMultiFiles.begin(); itNow != gltMultiFiles.end(); itNow++ )
	{
		if( uqNumber == itNow->dUnique )	break;
	}
	if( itNow == gltMultiFiles.end() )	return E_OUTOFMEMORY;

	ViewSelPageAll( -1 );	//	���J���Ă�ł͈̔͑I����j��

	PageListClear(  );	//	�y�[�W���X�g�r���[���j��

	gitFileIt = itNow;	//	�t�@�C���Ȃ�

//	TODO:	����ǂݍ��ݎ��̃o�C�g���v�Z�ԈႦ�Ă�悤���E�Ȃ������H

	PageListBuild( NULL );	//	�y�[�W���X�g��蒼��

	AppTitleChange( itNow->atFileName );	//	�L���v�V�����̓��e���ύX

	gixFocusPage = itNow->dNowPage;

	DocModifyContent( itNow->dModify );	//	�ύX�������ǂ���

	DocCaretPosMemory( INIT_LOAD, &stCaret );	//	��ɓǂݏo���Ȃ��Ǝ��ŃN�����[�����

	PageListViewChange( gixFocusPage,  -1 );	//	�S���ǂݍ��񂾂̂Ń��X�g�y�[�W��\������

	ViewPosResetCaret( stCaret.x, stCaret.y );	//	Caret�ʒu�Đݒ�

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�S���e��j���E�t�@�C���R�A����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocMultiFileCloseAll( VOID )
{
	UINT_PTR	i, iPage, iLine;
	FILES_ITR	itNow;
	LINE_ITR	itLine;

	for( itNow = gltMultiFiles.begin( ); itNow != gltMultiFiles.end(); itNow++ )
	{
		iPage = itNow->vcCont.size( );
		for( i = 0; iPage > i; i++ )
		{
			iLine  = itNow->vcCont.at( i ).ltPage.size( );

			itLine = itNow->vcCont.at( i ).ltPage.begin();
			for( itLine = itNow->vcCont.at( i ).ltPage.begin(); itLine != itNow->vcCont.at( i ).ltPage.end(); itLine++ )
			{
				itLine->vcLine.clear( );	//	�e�s�̒��g�S����
			}
			itNow->vcCont.at( i ).ltPage.clear( );	//	�s��S����

			SqnFreeAll( &(itNow->vcCont.at( i ).stUndoLog) );
		}
		itNow->vcCont.clear(  );	//	�y�[�W��S����
	}

	gltMultiFiles.clear(  );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C���^�u�����Ƃ��E�Ō�̈�͕���Ȃ��悤�ɂ��邩�E�t�@�C���R�A����
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	uqNumber	�������^�u�̒ʂ��ԍ�
	@return		LPARAM		�J���������^�u�̒ʂ��ԍ��E���s������O
*/
LPARAM DocMultiFileClose( HWND hWnd, LPARAM uqNumber )
{
	INT			iRslt;
	UINT_PTR	i, iPage, iLine;
	UINT_PTR	iCount;
	LPARAM	dNowNum, dPrevi;
	FILES_ITR	itNow;
	LINE_ITR	itLine;
	TCHAR		atBuffer[MAX_PATH];

	//	������J���ĂȂ��Ȃ���Ȃ�
	iCount = gltMultiFiles.size();
	if( 1 >= iCount )	return 0;

	//	�Ώۂ͍��̃t�@�C���ȊO��������Ȃ��B���������Ƃ��͂��̃t�@�C���Ɉړ����ď�������B
	//	������A���t�@�C���Ƀt�H�[�J�X����B�J���Ă���t�@�C���������A�ׂ̃t�@�C���Ƀt�H�[�J�X����

	dNowNum = gitFileIt->dUnique;	//	���J���Ă郄�c�̔ԍ�

	itNow = gltMultiFiles.begin( );
	itNow++;	//	���̂�̒ʂ��ԍ����m�ۂ��Ă����B
	dPrevi = itNow->dUnique;

	//	�������t�@�C���C�e���[�^��T��
	for( itNow = gltMultiFiles.begin( ); itNow != gltMultiFiles.end(); itNow++ )
	{
		if( uqNumber == itNow->dUnique )	break;
		dPrevi = itNow->dUnique;
	}
	if( itNow == gltMultiFiles.end() )	return 0;
	//	�����폜�Ώۂ��擪�Ȃ�AdPrevi�͎��̂�̂܂܁A���ȍ~�Ȃ�A���O�̂������Ă�͂�
	//	���̎��_�ŁAitNow �͍폜����t�@�C���ł���

	if( dNowNum != uqNumber )	//	�J���Ă�t�@�C���ƕ������t�@�C�����قȂ�Ȃ�
	{
		gixFocusPage = -1;
		DocMultiFileSelect( uqNumber  );	//	����\��t�@�C�����J��
		dPrevi = dNowNum;	//	���ɖ߂��ɂ�
	}


	//	�����ύX���c���Ă�Ȃ璍�ӂ𑣂�
	if( gitFileIt->dModify )
	{
		StringCchPrintf( atBuffer, MAX_PATH, TEXT("������Ƃ܂��āI\r\n[%s] �͕ύX�����܂܂���B\r\n�����ŕۑ����ĕ��邩���H"), PathFindFileName( gitFileIt->atFileName ) );
		iRslt = MessageBox( hWnd, atBuffer, TEXT("���ӂ���̊m�F"), MB_YESNOCANCEL | MB_ICONQUESTION );
		if( IDCANCEL == iRslt ){	return 0;	}

		if( IDYES == iRslt ){	DocFileSave( hWnd, D_SJIS );	}
	}

	//	DocContentsObliterate���̂��

	iPage = itNow->vcCont.size( );
	for( i = 0; iPage > i; i++ )
	{
		iLine = itNow->vcCont.at( i ).ltPage.size( );

		for( itLine = itNow->vcCont.at( i ).ltPage.begin(); itLine != itNow->vcCont.at( i ).ltPage.end(); itLine++ )
		{
			itLine->vcLine.clear( );	//	�e�s�̒��g�S����
		}
		itNow->vcCont.at( i ).ltPage.clear( );	//	�s��S����

		FREE( itNow->vcCont.at( i ).ptRawData );

		SqnFreeAll( &(itNow->vcCont.at( i ).stUndoLog) );
	}
	itNow->vcCont.clear(  );	//	�y�[�W��S����

	gltMultiFiles.erase( itNow );	//	�{�̂�����


	gixFocusPage = -1;
	DocMultiFileSelect( dPrevi );	//	���t�@�C���������͗׃t�@�C�����J������

	return dPrevi;
}
//-------------------------------------------------------------------------------------------------

/*!
	�J���Ă�^�u�������Ă���
	@param[in]	iTgt		�ǂݍ��݂����ԍ��E�����Ȃ�t�@�C�����̂݊m��
	@param[in]	ptFile		�t�@�C���p�X�����EMAX_PATH�ł��邱��
	@param[in]	ptIniPath	INI�t�@�C���̃p�X
	@return		HRESULT	�I����ԃR�[�h
*/
INT DocMultiFileFetch( INT iTgt, LPTSTR ptFile, LPTSTR ptIniPath )
{
	TCHAR	atKeyName[MIN_STRING];
	INT		iCount;

	assert( ptIniPath );

	iCount = GetPrivateProfileInt( TEXT("MultiOpen"), TEXT("Count"), 0, ptIniPath );
	if( 0 > iTgt )	return iCount;

	assert( ptFile );

	if( iCount <= iTgt ){	ptFile[0] = NULL;	return iCount;	}
	//	�I�[�o�[���Ă��疳���ɂ��ďI��

	StringCchPrintf( atKeyName, MIN_STRING, TEXT("Item%u"), iTgt );

	GetPrivateProfileString( TEXT("MultiOpen"), atKeyName, TEXT(""), ptFile, MAX_PATH, ptIniPath );

	return iCount;
}
//-------------------------------------------------------------------------------------------------

/*!
	�J���Ă�^�u���L�^����E�t�@�C���R�A����
	@param[in]	ptIniPath	INI�t�@�C���̃p�X
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocMultiFileStore( LPTSTR ptIniPath )
{
	TCHAR	atKeyName[MIN_STRING], atBuff[MIN_STRING];
	UINT	i;
	FILES_ITR	itNow;

	assert( ptIniPath );

	//	��U�Z�N�V��������ɂ���
	ZeroMemory( atBuff, sizeof(atBuff) );
	WritePrivateProfileSection( TEXT("MultiOpen"), atBuff, ptIniPath );

	//	�t�@�C���������L�^
	i = 0;
	for( itNow = gltMultiFiles.begin( ); itNow != gltMultiFiles.end(); itNow++ )
	{
		if( NULL != itNow->atFileName[0] )
		{
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("Item%u"), i );
			WritePrivateProfileString( TEXT("MultiOpen"), atKeyName, itNow->atFileName, ptIniPath );
			i++;
		}
	}

	//	�����L�^
	StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), i );
	WritePrivateProfileString( TEXT("MultiOpen"), TEXT("Count"), atBuff, ptIniPath );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Ώۃt�@�C���̖��O���Q�b�c�I����
	@param[in]	tabNum	���O��m�肽�����c�̃^�u�ԍ�
	@return		LPTSTR	���O�o�b�t�@�̃|�C���^�[�E�����Ȃ�NULL��Ԃ�
*/
LPTSTR DocMultiFileNameGet( INT tabNum )
{
	INT	i;
	FILES_ITR	itNow;

	//	�q�b�g����܂ŃT�[�`
	for( i = 0, itNow = gltMultiFiles.begin(); itNow != gltMultiFiles.end(); i++, itNow++ )
	{
		if( tabNum == i )	break;
	}
	if( itNow == gltMultiFiles.end() )	return NULL;	//	�q�b�g�����E�A���G�i�[�C

	//	�������Ȃ�_�~�[��
	if( NULL == itNow->atFileName[ 0] ){	return itNow->atDummyName;	}

	return itNow->atFileName;	//	�t�@�C�����߂�
}
//-------------------------------------------------------------------------------------------------

/*!
	Caret�ʒu���펞�L�^�E�t�@�C���؂�ւ����Ƃ��ɈӖ�������
	@param[in]		dMode	��O���[�h�@�O�Z�[�u
	@param[in,out]	pstPos	Caret�ʒu�A�h�b�g�A�s��
*/
VOID DocCaretPosMemory( UINT dMode, LPPOINT pstPos )
{
	if( dMode )	//	���[�h
	{
		pstPos->x = gitFileIt->stCaret.x;
		pstPos->y = gitFileIt->stCaret.y;
	}
	else	//	�Z�[�u
	{
		gitFileIt->stCaret.x = pstPos->x;
		gitFileIt->stCaret.y = pstPos->y;
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�V�����t�@�C�����J��
	@param[in]	hWnd	�e�ɂ���E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocOpenFromNull( HWND hWnd )
{
	LPARAM	dNumber;

	TCHAR	atDummyName[MAX_PATH];
	//	�����t�@�C�������Ȃ�A�j���͕s�v�A�V�����t�@�C���C���X�^���X����đΉ�

	//	�V�����t�@�C���u����̏���
	dNumber = DocMultiFileCreate( atDummyName );	//	�t�@�C����V�K�쐬����Ƃ�

	MultiFileTabAppend( dNumber, (*gitFileIt).atDummyName );	//	�t�@�C���̐V�K�쐬����

	AppTitleChange( atDummyName );

	gixFocusPage = DocPageCreate( -1 );
	PageListInsert( gixFocusPage  );	//	�y�[�W���X�g�r���[�ɒǉ�
	DocPageChange( 0 );

	ViewRedrawSetLine( -1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C������O�ɕύX���m�F
	@param[in]	hWnd	�e�ɂ���E�C���h�E�n���h��
	@param[in]	dMode	��O���郁�b�Z�[�W����@�O�ύX����������f�ʂ�
	@return	�P���Ă����@�O�_��
*/
INT DocFileCloseCheck( HWND hWnd, UINT dMode )
{
	INT		rslt, ret = 0;

	TCHAR	atMessage[BIG_STRING];
	BOOLEAN	bMod = FALSE;
	FILES_ITR	itFiles;

	//	���ۑ��̃t�@�C�����`�F�L
	for( itFiles = gltMultiFiles.begin(); itFiles != gltMultiFiles.end(); itFiles++ )
	{
		if( itFiles->dModify )
		{
			StringCchPrintf( atMessage, BIG_STRING, TEXT("������Ƃ܂����I\r\n%s �͕ۑ����ĂȂ���B�����ŕۑ����邩���H"), itFiles->atFileName[0] ? PathFindFileName( itFiles->atFileName ) : itFiles->atDummyName );
			rslt = MessageBox( hWnd, atMessage, TEXT("���ӂ���̊m�F"), MB_YESNOCANCEL | MB_ICONQUESTION );
			if( IDCANCEL ==  rslt ){	return 0;	}	//	�L�����Z���Ȃ�I��邱�Ǝ��̂Ƃ���
			if( IDYES == rslt ){	DocFileSave( hWnd, D_SJIS );	}	//	�ۑ�����Ȃ�Z�[�u���Ă�
			//	NO�Ȃ牽�����������m�F
			bMod = TRUE;	//	���ۑ���������
		}
	}

	if( !(bMod) )	//	���ۑ����Ȃ������Ȃ�m�F���b�Z�[�W
	{
		rslt = MessageBox( hWnd, TEXT("�����I��邩���H"), TEXT("���ӂ���̊m�F"), MB_YESNO | MB_ICONQUESTION );
		if( IDYES == rslt ){	ret = 1;	}
		else{					ret = 0;	}
	}

	return ret;
}
//-------------------------------------------------------------------------------------------------

#ifdef BIG_TEXT_SEPARATE
//	2014/05/28
/*!
	�t�@�C�����e���m�F���āA�������K�v���ǂ����m�F����
	@param[in]	ptStr	�Ώە�����ւ̃|�C���^�[
	@param[in]	cchSize	���̕�����̕�����
	@return	�O�Ȃɂ����Ȃ��@�P�������[�h�@�Q�Ǎ����~
*/
UINT DocFileHugeCheck( LPTSTR ptStr, UINT_PTR cchSize )
{
	LPTSTR		ptBuff;
//	UINT_PTR	d;
	UINT_PTR	dCount;
	UINT		dRslt;

	//	�`�r�s�Ȃ牽������K�v�͖���
	if( 0 == StrCmpN( AST_SEPARATERW , ptStr, 4 ) ){	return 0;	}

	ptBuff = StrStr( ptStr, MLT_SEPARATERW );	//	�Z�p���[�^��T��
	if( ptBuff ){	return 0;	}	//	�L��Ȃ��薳��

	//	�s�����m�F�E�P�O�O�s�ȏ゠��Ȃ番�����K�v�Ƃ݂Ȃ�
	dCount = 0;

	do{
		ptBuff = StrStr( ptStr , TEXT("\r\n") );	//	���s��T��
		if( ptBuff ){	dCount++;	}else{	break;	}

		ptStr = ptBuff+2;	//	���s���i�񂾈ʒu���K�v
	}while( ptBuff );

	//	���s���Ȃ��P�O�O�O�����ȏ������ꍇ�̓G���[�Ƃ���
	if( 0 == dCount && 1000 <= cchSize )
	{
		DocHugeFileTreatment( 2 );
		return 2;
	}

	if( 100 >= dCount ){	return 0;	}	//	�P�O�O�s�ȉ��Ȃ��薳��

	//	�����ŁA��^�t�@�C���Ȃ̂łǂ����邩�̊m�F�E���̂܂܁^�����^��߂�
	dRslt = DocHugeFileTreatment( 1 );

	return dRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C�����e�𕪊�����
	@param[in]	ptSource	�Ώە�����ւ̃|�C���^�[�E�I�[NULL���Q�ȏ�m�ۂ���Ă��邱��
	@param[in]	cchSource	���̕�����̕�����
	@return	�������ʂ̓������|�C���^�[�E�J���͌Ă񂾕��ł��ENULL�Ȃ玸�s
*/
LPTSTR DocFileHugeSeparate( LPTSTR ptSource, UINT_PTR cchSource )
{
	LPTSTR		ptDest, ptBuff;
	UINT_PTR	dd, ds, dLineCnt, dEmptyCnt, cchDest;

	cchDest = cchSource;	//	�Ƃ肠��������Ŋm�ہE����Ȃ��Ȃ�����realloc���邩
	ptDest = (LPTSTR)malloc( sizeof(TCHAR) * cchDest );
	if( !(ptDest) ){	return NULL;	}	//	�Ȃ񂩃G���[�E���ʂ͖���
	ZeroMemory( ptDest, sizeof(TCHAR) * cchDest );

	dd = 0;
	dLineCnt = 0;	//	���̍s���́A�ōX�V���������烊�Z�b�g����
	dEmptyCnt = 0;
	for( ds = 0; cchSource > ds; ds++ )
	{
		if( NULL == ptSource[ds] )	break;

		ptDest[dd] = ptSource[ds];	//	�Ƃ肠�����������ʂ��Ă���
		dd++;	//	�������͐i�߂Ă���
		ptDest[dd] = NULL;	//	�^�[�~�l�[�g

		if( 0xD == ptSource[ds] && 0xA == ptSource[ds+1] )	//	���s���������ꍇ
		{
			ds++;	//	���̉��s������
			ptDest[dd] = ptSource[ds];
			dd++;	//	�������͐i�߂Ă���
			ptDest[dd] = NULL;	//	�^�[�~�l�[�g

			if( cchDest <= (dd+12) )	//	�c�菭�Ȃ��Ȃ��Ă�����
			{
				ptBuff = (LPTSTR)realloc( ptDest, sizeof(TCHAR) * (cchDest + 0x800) );	//	�K���Ɋg��
				if( !(ptBuff)  )	//	�����G���[�ɂȂ�����
				{
					free( ptDest );	//	�g�p�̈�͊J������
					return NULL;	//	�G���[�߂�
				}
				ptDest = ptBuff;	//	�t���ւ���
				cchDest += 0x800;	//	�T�C�Y�g��
			}//�����ɂ͗v��Ȃ�����

			dLineCnt++;	//	�s��������
			if( 40 <= dLineCnt ){	dEmptyCnt++;	}	//	�K��s�����������s�J�E���g�J�n

			//	�S�O�s�ȍ~�łS�s�ȏ�̋󂫂����邩�A�P�O�O�s�ȍ~�łP�s�ȏ�̋󂫂����邩�A�Q�T�U�s�ȏ㑱���Ă���
			if( (5 <= dEmptyCnt) || (100 <= dLineCnt && 2 <= dEmptyCnt) || (256 <= dLineCnt) )
			{
				StringCchCat( ptDest, cchDest, MLT_SEPARATERW );
				StringCchCat( ptDest, cchDest, TEXT("\r\n") );
				StringCchLength( ptDest, cchDest, &dd );	//	�ʒu���킹

				dLineCnt = 0;
				dEmptyCnt = 0;
			}
		}
		else
		{
			dEmptyCnt = 0;	//	���s�ȊO�Ȃ烊�Z�b�g�E�T�O�s�����ĂĂ��A�ʏ�̕����񂪑����Ȃ��J�E���g�͐i�܂Ȃ�
		}

		if( cchDest <= (dd+12) )	//	�c�菭�Ȃ��Ȃ��Ă�����
		{
			ptBuff = (LPTSTR)realloc( ptDest, sizeof(TCHAR) * (cchDest + 0x800) );	//	�K���Ɋg��
			if( !(ptBuff)  )	//	�����G���[�ɂȂ�����
			{
				free( ptDest );	//	�g�p�̈�͊J������
				return NULL;	//	�G���[�߂�
			}
			ptDest = ptBuff;	//	�t���ւ���
			cchDest += 0x800;	//	�T�C�Y�g��
		}

	}

	return ptDest;
}
//-------------------------------------------------------------------------------------------------

#endif

/*!
	�t�@�C�����m��
	@param[in]	ptFileName	�w�肳�ꂽ�t�@�C�����ŊJ��
	@return		LPARAM	�O���s�@�P�`����
*/
LPARAM DocFileInflate( LPTSTR ptFileName )
{
	CONST WCHAR rtHead = 0xFEFF;	//	���j�R�[�h�e�L�X�g�w�b�_
	WCHAR	rtUniBuf;

	HANDLE	hFile;
	DWORD	readed;

	LPVOID	pBuffer;	//	������o�b�t�@�p�|�C���^�[
	INT		iByteSize;

	LPTSTR	ptString;
	LPSTR	pcText;
	UINT_PTR	cchSize;

	LPARAM	dNumber;

#ifdef BIG_TEXT_SEPARATE	//	�ŋ�؂�̂Ȃ�TXT���ǂ������m�F����
	UINT	dSepRslt;
	LPTSTR	ptSepBuff;
#endif

#ifdef _DEBUG
	DWORD	dTcStart, dTcEnd;
#endif

	//TCHAR	atBuff[10];
	//ZeroMemory( atBuff, sizeof(atBuff) );

#ifdef _DEBUG
	dTcStart = GetTickCount(  );
#endif
	assert( ptFileName );	//	�t�@�C���J���Ȃ��̂̓o�O

	//	�t�@�C����������ۂ������玩���I�ɃA�E�c�I
	if( NULL == ptFileName[0] ){	return 0;	}

	//	���b�c�I�[�|��
	hFile = CreateFile( ptFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE == hFile ){	return 0;	}

	//InitLastOpen( INIT_SAVE, ptFileName );	//	�����t�@�C���ł͈Ӗ�������

	//	�������ԓ���

	iByteSize = GetFileSize( hFile, NULL );
	pBuffer = malloc( iByteSize + 4 );	//	�o�b�t�@�͏����傫�߂Ɏ��
	ZeroMemory( pBuffer, iByteSize + 4 );

	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
	ReadFile( hFile, pBuffer, iByteSize, &readed, NULL );
	CloseHandle( hFile );	//	���e�S����荞�񂾂���J��

	//	���j�R�[�h�`�F�b�N
	CopyMemory( &rtUniBuf, pBuffer, 2 );
	if( rtHead == rtUniBuf )	//	���j�R�[�h�w�b�_�������
	{
		ptString = (LPTSTR)pBuffer;
		ptString++;	//	���j�R�[�h�w�b�_���i�߂Ă���
	}
	else
	{
		pcText = (LPSTR)pBuffer;
		//	�V�t�gJIS���J���ꍇ�A&#0000;�̕������ǂ��ɂ�����Ƃ�����
		ptString = SjisDecodeAlloc( pcText );	//	�t�@�C�����J���Ƃ�

		FREE( pBuffer );	//	�������ŊJ��
		pBuffer = ptString;	//	�|�C���g����Ƃ����ύX

	}

	StringCchLength( ptString, STRSAFE_MAX_CCH, &cchSize );

#ifdef BIG_TEXT_SEPARATE	//	�ŋ�؂�̂Ȃ�TXT���ǂ������m�F����
	dSepRslt = DocFileHugeCheck( ptString, cchSize );
	if(  1 == dSepRslt )	//	������������
	{
		ptSepBuff = DocFileHugeSeparate( ptString, cchSize );
		if( !(ptSepBuff)  ){	return 0;	}	//	�Ȃ񂩃~�X���Ă�Ȃ璆�~

		FREE( pBuffer );	//	��{����U�J��
		pBuffer = ptSepBuff;	//	�Ō�ɊJ������̈�Ƃ��ă|�C���g����Ƃ����ύX
		ptString = ptSepBuff;	//	�Ǎ������𑱂���ʒu�Ƃ��ăZ�b�g

		StringCchLength( ptString, STRSAFE_MAX_CCH , &cchSize );	//	�T�C�Y�Ǎ�����
	}
	else if( 2 == dSepRslt )	//	�Ǎ����~
	{
		FREE( pBuffer );	//	��{���J��
		return 0;
	}
	//	�O�Ȃ牽�����Ȃ�
#endif

	//	�V�����t�@�C���u����̏���	2014/05/28���ɂ������̂��ړ�����
	dNumber = DocMultiFileCreate( NULL );	//	���ۂ̃t�@�C�����J���Ƃ�
	if( 0 >= dNumber )	return 0;

	StringCchCopy( (*gitFileIt).atFileName, MAX_PATH, ptFileName );


	//	����AST�Ȃ�A�擪��[AA]�ɂȂ��Ă�͂��E�����͒��ł��
	if( StrCmpN( AST_SEPARATERW, ptString, 4 ) )
	{
		DocStringSplitMLT( ptString , cchSize, DocPageLoad );
	}
	else
	{
		DocStringSplitAST( ptString , cchSize, DocPageLoad );
	}

	//	�t�@�C���J������L�����b�g�Ƃ��X�N���[�������Z�b�g����
	ViewEditReset(  );

	FREE( pBuffer );	//	��ptString

	DocPageChange( 0  );	//	�S���ǂݍ��񂾂̂ōŏ��̃y�[�W��\������
	PageListViewChange( -1, -1 );	//	���O�Ń��Z�b�g

	AppTitleChange( ptFileName );

#ifdef _DEBUG
	dTcEnd = GetTickCount(  );
	TRACE( TEXT("LOAD START[%u]  END[%u]    ELAPSE[%u]"), dTcStart, dTcEnd, (dTcEnd - dTcStart) );
#endif

	return dNumber;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ł�����ē��e���Ԃ�����
	@param[in]	ptName	���ڂ̖��O�E��������NULL
	@param[in]	ptCont	���ڂ̓��e
	@param[in]	cchSize	���e�̕�����
	@return		UINT	���ɈӖ��Ȃ�
*/
UINT CALLBACK DocPageLoad( LPTSTR ptName, LPCTSTR ptCont, INT cchSize )
{
	gixFocusPage = DocPageCreate(  -1 );	//	�ł��쐬
	PageListInsert( gixFocusPage  );	//	�y�[�W���X�g�r���[�ɒǉ�

	//	�V����������y�[�W�ɂ���

	if( ptName ){	DocPageNameSet( ptName );	}	//	���O���Z�b�g���Ă���
	(*gitFileIt).vcCont.at( gixFocusPage ).ptRawData = (LPTSTR)malloc( (cchSize+2) * sizeof(TCHAR) );
	ZeroMemory( (*gitFileIt).vcCont.at( gixFocusPage ).ptRawData, (cchSize+2) * sizeof(TCHAR) );

	//HRESULT hRslt = 
	StringCchCopy( (*gitFileIt).vcCont.at( gixFocusPage ).ptRawData, (cchSize+2), ptCont );

	//	�o�b�t�@�ɕ������ۑ��������Ă���

	DocPageParamGet( NULL, NULL );	//	�Čv�Z�����Ⴄ�E�x���Ǎ��q�b�g

	return 1;
}
//-------------------------------------------------------------------------------------------------

#ifdef FILE_PRELOAD
/*!
	�l�k�s�������͂s�w�s�̕Ő��𒲂ׂ�
	@param[in]	ptStr	����Ώە�����ւ̃|�C���^�[
	@param[in]	cchSize	���̕�����̕�����
	@return		UINT	�Ő�
*/
UINT DocPreloadMLT( LPTSTR ptString, INT cchSize )
{
	LPTSTR	ptCaret;	//	�Ǎ��J�n�E���݈ʒu
	LPTSTR	ptEnd;		//	�y�[�W�̖��[�ʒu�E�Z�p���[�^�̒��O
	INT		iLines, iDots, iMozis;
	UINT	dPage;
	UINT_PTR	cchItem;
	BOOLEAN	bLast = FALSE;

	ptCaret = ptString;	//	�܂��͍ŏ�����

	dPage = 0;

	//	�n�_�ɂ̓Z�p���[�^�������̂Ƃ݂Ȃ��B�A������Z�p���[�^�́A�󔒓��e�Ƃ��Ĉ���
	do
	{
		ptEnd = StrStr( ptCaret, MLT_SEPARATERW );	//	�Z�p���[�^��T��
		if( !ptEnd )	//	������Ȃ������灁���ꂪ�Ō�Ȃ灁NULL
		{
			ptEnd = ptString + cchSize;
			bLast = TRUE;
		}
		cchItem = ptEnd - ptCaret;	//	WCHAR�P�ʂȂ̂Ōv�Z���ʂ͕������̂悤��
		//	�ŏI�łłȂ��ꍇ�͖��[�̉��s������
		if( !(bLast) && 0 < cchItem ){	cchItem -=  CH_CRLF_CCH;	}

		dPage++;

		//	�ł̏����m��
		iLines = DocStringInfoCount( ptCaret, cchItem, &iDots, &iMozis );

		ptCaret = NextLineW( ptEnd );	//	�Z�p���[�^�̎��̍s���炪�{��

	}while( *ptCaret  );	//	�f�[�^�L����胋�[�v�ŒT��


	return dPage;
}
//-------------------------------------------------------------------------------------------------
#endif

/*!
	�l�k�s�������͂s�w�s�ȃ��j�R�[�h��������󂯎���ĕ������y�[�W�ɓ����
	@param[in]	ptStr		����Ώە�����ւ̃|�C���^�[
	@param[in]	cchSize		���̕�����̕�����
	@param[in]	pfPageLoad	���e������R�[���o�b�N�����̃A��
	@return		UINT		�쐬�����Ő�
*/
UINT DocStringSplitMLT( LPTSTR ptStr, INT cchSize, PAGELOAD pfPageLoad )
{
	LPTSTR	ptCaret;	//	�Ǎ��J�n�E���݈ʒu
	LPTSTR	ptEnd;		//	�y�[�W�̖��[�ʒu�E�Z�p���[�^�̒��O
	UINT	iNumber;	//	�ʂ��ԍ��J�E���g
#ifdef FILE_PRELOAD
	UINT	dPage;		
#endif
	UINT	cchItem;
//	INT		dmyX = 0, dmyY = 0;
	BOOLEAN	bLast = FALSE;

	ptCaret = ptStr;	//	�܂��͍ŏ�����

	iNumber = 0;	//	�ʂ��ԍ��O�C���f�b�N�X
	//	�n�_�ɂ̓Z�p���[�^�������̂Ƃ݂Ȃ��B�A������Z�p���[�^�́A�󔒓��e�Ƃ��Ĉ���

#ifdef FILE_PRELOAD
	dPage = DocPreloadMLT( ptStr, cchSize );
#endif

	do
	{
		ptEnd = StrStr( ptCaret, MLT_SEPARATERW );	//	�Z�p���[�^��T��
		if( !ptEnd )	//	������Ȃ������灁���ꂪ�Ō�Ȃ灁NULL
		{
			ptEnd = ptStr + cchSize;	//	WCHAR�T�C�Y�Ōv�Z�����H
			bLast = TRUE;
		}
		cchItem = ptEnd - ptCaret;	//	WCHAR�P�ʂȂ̂Ōv�Z���ʂ͕������̂悤��

		//	�ŏI�łłȂ��ꍇ�͖��[�̉��s������
		if( !(bLast) && 0 < cchItem )
		{
			cchItem -=  CH_CRLF_CCH;
			ptCaret[cchItem] = 0;
		}

		pfPageLoad( NULL, ptCaret, cchItem );

		iNumber++;

		ptCaret = NextLineW( ptEnd );	//	�Z�p���[�^�̎��̍s���炪�{��

	}while( *ptCaret );	//	�f�[�^�L����胋�[�v�ŒT��

	return iNumber;
}
//-------------------------------------------------------------------------------------------------

/*!
	�`�r�s�ȃ��j�R�[�h��������󂯎���ĕ������y�[�W�ɓ����
	@param[in]	ptStr		����Ώە�����ւ̃|�C���^�[
	@param[in]	cchSize		���̕�����̕�����
	@param[in]	pfPageLoad	���e������R�[���o�b�N�����̃A��
	@return		UINT		�쐬�����Ő�
*/
UINT DocStringSplitAST( LPTSTR ptStr, INT cchSize, PAGELOAD pfPageLoad )
{
	LPTSTR	ptCaret;	//	�Ǎ��J�n�E���݈ʒu
	LPTSTR	ptStart;	//	�Z�p���[�^�̒��O
	LPTSTR	ptEnd;
	UINT	iNumber;	//	�ʂ��ԍ��J�E���g
	UINT	cchItem;
	BOOLEAN	bLast;
	TCHAR	atName[MAX_PATH];

	ptCaret = ptStr;	//	�܂��͍ŏ�����

	iNumber = 0;	//	�ʂ��ԍ��O�C���f�b�N�X

	bLast = FALSE;



	do	//	�Ƃ肠������ԍŏ���ptCaret��[AA]�ɂȂ��Ă�
	{
		ptStart = NextLineW( ptCaret );	//	���̍s���炪�{��

		ptCaret += 5;	//	[AA][
		cchItem = ptStart - ptCaret;	//	���O�����̕�����
		cchItem -= 3;	//	]rn

		ZeroMemory( atName, sizeof(atName) );	//	���O�m��
		if( 0 < cchItem )	StringCchCopyN( atName, MAX_PATH, ptCaret, cchItem );

		ptCaret = ptStart;	//	�{�̕���

		ptEnd = StrStr( ptCaret, AST_SEPARATERW );	//	�Z�p���[�^��T��
		//	���̎��_��ptEnd�͎���[AA]�������Ă�E��������NULL(�Ō�̃R�})
		if( !ptEnd )	//	������Ȃ������灁���ꂪ�Ō�Ȃ灁NULL
		{
			ptEnd = ptStr + cchSize;	//	WCHAR�T�C�Y�Ōv�Z�����H
			bLast = TRUE;
		}
		cchItem = ptEnd - ptCaret;	//	WCHAR�P�ʂȂ̂Ōv�Z���ʂ͕������̂悤��

		if( !(bLast) && 0 < cchItem )	//	�ŏI�łłȂ��ꍇ�͖��[�̉��s������
		{
			cchItem -= CH_CRLF_CCH;
			ptCaret[cchItem] = 0;
		}

		pfPageLoad( atName, ptCaret, cchItem );

		iNumber++;

		ptCaret = ptEnd;

	}while( *ptCaret );	//	�f�[�^�L����胋�[�v�ŒT��

	return iNumber;
}
//-------------------------------------------------------------------------------------------------

/*!
	�`�r�c�Ȃr�i�h�r��������󂯎���ĕ������y�[�W�ɓ����
	@param[in]	pcStr		����Ώ�SJIS������ւ̃|�C���^�[
	@param[in]	cbSize		���̕�����̕�����
	@param[in]	pfPageLoad	���e������R�[���o�b�N�����̃A��
	@return		UINT		�쐬�����Ő�
*/
UINT DocImportSplitASD( LPSTR pcStr, INT cbSize, PAGELOAD pfPageLoad )
{
//ASD�Ȃ�ASJIS�̂܂܂�0x01,0x01�A0x02,0x02��Ή�����K�v������
//0x01,0x01�����s�A0x02,0x02�������̋�؂�A�A�C�e����؂肪���s

	LPSTR	pcCaret;	//	�Ǎ��J�n�E���݈ʒu
	LPSTR	pcEnd, pcDesc;
	UINT	iNumber;	//	�ʂ��ԍ��J�E���g
	UINT	cbItem, d;
	BOOLEAN	bLast;

	LPTSTR		ptName, ptCont;
	UINT_PTR	cchItem;


	pcCaret = pcStr;	//	�܂��͍ŏ�����

	iNumber = 0;	//	�ʂ��ԍ��O�C���f�b�N�X

	bLast = FALSE;


	do	//	�Ƃ�₦�����s
	{
		pcEnd = NextLineA( pcCaret );	//	���̍s�܂łłP�A�C�e��
		//if( !(*pcEnd) )	//	������Ȃ������灁���ꂪ�Ō�Ȃ灁NULL
		//{
		//	pcEnd = pcStr + cbSize;	//	CHAR�T�C�Y�Ōv�Z�����H
		//	bLast = TRUE;
		//}
		//	���g��NULL�Ȃ����ŁA�|�C���^�͗L���Ȃ̂œ��Ɉʒu�v�Z�͕s�v��
		cbItem  = pcEnd - pcCaret;	//	��s�̕�����

		pcDesc = NULL;
		ptName = NULL;
		ptCont = NULL;

		for( d = 0; cbItem > d; d++ )
		{
			if( (0x0D == pcCaret[d]) && (0x0A == pcCaret[d+1]) )	//	���[�ł��邩
			{
				pcCaret[d]   = 0x00;	//	���[�Ȃ̂�NULL�ɂ���
				pcCaret[d+1] = 0x00;

				if( pcDesc ){	ptName =  SjisDecodeAlloc( pcDesc );	}

				break;
			}

			//	�������Ԓ���
			if( (0x01 == pcCaret[d]) && (0x01 == pcCaret[d+1]) )	//	���s�ł��邩
			{
				pcCaret[d]   = 0x0D;	//	����
				pcCaret[d+1] = 0x0A;	//	����
				d++;	//	�ϊ������̂Ŏ��ɐi�߂�̂��悢
			}

			if( (0x02 == pcCaret[d]) && (0x02 == pcCaret[d+1]) )	//	�A�C�e���Ɛ����̋�؂�
			{
				pcDesc = &(pcCaret[d+2]);	//	�����J�n�ʒu

				pcCaret[d]   = 0x00;	//	�d�؂�Ȃ̂�NULL�ɂ���
				pcCaret[d+1] = 0x00;
				d++;	//	�ϊ������̂Ŏ��ɐi�߂�̂��悢
			}
		}

		ptCont = SjisDecodeAlloc( pcCaret );	//	����Ă���
		StringCchLength( ptCont, STRSAFE_MAX_CCH, &cchItem );

		pfPageLoad( ptName, ptCont, cchItem );

		iNumber++;

		FREE( ptCont );
		FREE( ptName );

		pcCaret = pcEnd;

	}while( *pcCaret  );	//	�f�[�^�L����胋�[�v�ŒT��

	return iNumber;
}
//-------------------------------------------------------------------------------------------------


/*!
	�Ŗ����Z�b�g����E�t�@�C���R�A����
	@param[in]	ptName	�Z�b�g����Ŗ��̂ւ̃|�C���^�[
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocPageNameSet( LPTSTR ptName )
{
	StringCchCopy( (*gitFileIt).vcCont.at( gixFocusPage ).atPageName, SUB_STRING, ptName );

	PageListNameSet( gixFocusPage, ptName );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�y�[�W�ǉ������E�t�@�C���R�A����
	@param[in]	iAdding	���̎w��y�[�W�̎��ɒǉ��E-1�Ŗ��[�ɒǉ�
	@return	INT	�V�K�쐬�����y�[�W�ԍ�
*/
INT DocPageCreate( INT iAdding )
{
	INT_PTR		iTotal, iNext;
	UINT_PTR	iAddPage = 0;
	INT		i;

	ONELINE	stLine;
	ONEPAGE	stPage;

	PAGE_ITR	itPage;

#ifdef DO_TRY_CATCH
	try{
#endif

	ZeroONELINE( &stLine  );	//	�V�K�쐬������A��s�ڂ��O�����Șg�����

	//	��������ZeroONEPAGE�Ƃ��ɂ܂Ƃ߂邩
	ZeroMemory( stPage.atPageName, sizeof(stPage.atPageName) );
//	stPage.dDotCnt = 0;
	stPage.dByteSz = 0;
	stPage.ltPage.clear(  );
	stPage.ltPage.push_back( stLine );	//	�P�ł̘g�������
	stPage.dSelLineTop    =  -1;		//	�����́|�P�𒍈�
	stPage.dSelLineBottom =  -1;		//	
	stPage.ptRawData = NULL;
	SqnInitialise( &(stPage.stUndoLog) );

	//	���̕ł̎��ɍ쐬
	iTotal = DocNowFilePageCount(  );

	if( 0 <= iAdding )
	{
		iNext = iAdding + 1;	//	���̕�
		if( iTotal <= iNext ){	iNext =  -1;	}	//	�S�ł�葽���Ȃ疖�[�w��
	}
	else
	{
		iNext = -1;
	}

	if( 0 >  iNext )	//	�����ɒǉ�
	{
		(*gitFileIt).vcCont.push_back( stPage  );	//	�t�@�C���\���̂ɒǉ�

		iAddPage = DocNowFilePageCount( );
		iAddPage--;	//	���[�ɒǉ������񂾂���A�������ā|�P������O�C���f�b�N�X�ԍ�
	}
	else
	{
		itPage = (*gitFileIt).vcCont.begin(  );
		for( i = 0; iNext >  i; i++ ){	itPage++;	}
		(*gitFileIt).vcCont.insert( itPage, stPage );

		iAddPage = iNext;
	}

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return ETC_MSG( err.what(), 0 );	}
	catch( ... ){	return  ETC_MSG( ("etc error"), 0 );	}
#endif

	return iAddPage;	//	�ǉ������y�[�W�ԍ�
}
//-------------------------------------------------------------------------------------------------

/*!
	�ł��폜�E�t�@�C���R�A����
	@param[in]	iPage	�폜����ł̔ԍ�
	@param[in]	iBack	�|�P�����@�O�`�폜�������ƈړ�����Ŏw��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocPageDelete( INT iPage, INT iBack )
{
	INT	i, iNew;
	PAGE_ITR	itPage;

	if( 1 >= DocNowFilePageCount( ) )	return E_ACCESSDENIED;

#ifdef DO_TRY_CATCH
	try{
#endif

	//	�����Ńo�b�N�A�b�v���H

	//	�X���ʒu�܂ŃC�e���[�^�������Ă���
	itPage = (*gitFileIt).vcCont.begin(  );
	for( i = 0; iPage > i; i++ ){	itPage++;	}

	FREE( itPage->ptRawData );

	SqnFreeAll( &(itPage->stUndoLog)  );	//	�A���h�D���O�폜
	(*gitFileIt).vcCont.erase( itPage  );	//	��������폜
	gixFocusPage = -1;	//	�őI�𖳌��ɂ���


	PageListDelete( iPage );

	if( 0 <= iBack )	//	�߂��w��
	{
		iNew = iBack;
	}
	else
	{
		iNew = iPage - 1;	//	�폜�������O�̕ł�
		if( 0 > iNew )	iNew = 0;
	}

	DocPageChange( iNew );	//	�폜������ňړ�

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return ETC_MSG( err.what(), E_FAIL );	}
	catch( ... ){	return  ETC_MSG( ("etc error"), E_FAIL );	}
#endif

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�f�B���C���Ă�ł̓Ǎ�
	@param[in]	itFile	�Ώۂ̃t�@�C��
	@param[in]	iPage	���̃t�@�C���̕ŁE�������Ӗ��Ȃ�
	@return		��O���[�h�����@�O���[�h�ςł�����
*/
UINT DocDelayPageLoad( FILES_ITR itFile, INT iPage )
{
	INT	dmyX = 0, dmyY = 0;
	UINT_PTR	cchSize;


	if( itFile->vcCont.at( iPage ).ptRawData )
	{
		TRACE( TEXT("PAGE DELAY LOAD [%d]"), iPage );

		StringCchLength( itFile->vcCont.at( iPage ).ptRawData, STRSAFE_MAX_CCH, &cchSize );

		//	�����ŁA�{����ǂݍ���
		if( 0 < cchSize )	//	��s�łȂ��̂Ȃ�
		{
			DocStringAdd( &dmyX, &dmyY, itFile->vcCont.at( iPage ).ptRawData, cchSize );	//	���̒��ŉ��s�Ƃ��ʓ|����
			//	����ł́A���̃y�[�W���[�h��p�ɂȂ��Ă�
		}

		//	�A���h�D�͈�U���Z�b�g���ׂ����ŊJ���������Ȃ̂�
		//	�ύX��ON�Ȃ��Ă������

	//	DocPageParamGet( NULL, NULL );	//	�Čv�Z�����Ⴄ�������ǉ��ł���Ă�̂Ŗ�薳��

		FREE( itFile->vcCont.at( iPage ).ptRawData  );	//	NULL���ۂ�������̂Œ���

#ifdef FIND_STRINGS
#ifdef SEARCH_HIGHLIGHT
		//	�������e�������Ă���n�C���C�c����
		FindDelayPageReSearch( iPage );
#endif
#endif

	}
	else
	{
		return 0;
	}

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�y�[�W��ύX�E�t�@�C���R�A����
	@param[in]	dPageNum	�ύX�������Ŕԍ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocPageChange( INT dPageNum )
{
	INT	iPrePage;

	//	���̕\�����e�j���Ƃ����낢�날��
#ifdef DO_TRY_CATCH
	try{
#endif

	ViewSelPageAll( -1 );	//	�͈͑I����j��

	iPrePage = gixFocusPage;
	gixFocusPage = dPageNum;	//	��ɕύX����

	(*gitFileIt).dNowPage = dPageNum;	//	�L�^

	//	�܂��W�J����ĂȂ��Ȃ�
	DocDelayPageLoad( gitFileIt, dPageNum );

	PageListViewChange( dPageNum, iPrePage );

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (HRESULT)ETC_MSG( err.what(), E_UNEXPECTED );	}
	catch( ... ){	return (HRESULT)ETC_MSG( ("etc error") , E_UNEXPECTED );	}
#endif

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ňꗗ�ɕ\��������e�𑗂�B
	@param[in]	dPage	�Ŏw��E�����Ȃ猻�݂̕�
	@param[in]	bMode	��O�X�e�[�^�X�o�[�\���E�O�X�e�[�^�X�o�[����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocPageInfoRenew( INT dPage, UINT bMode )
{
	UINT_PTR	dLines;
	UINT		dBytes;
//	TCHAR		atBuff[SUB_STRING];

	if( 0 > dPage ){	dPage = gixFocusPage;	}


	dBytes = gitFileIt->vcCont.at( dPage ).dByteSz;
	
	if( bMode )	//	�X�e�[�^�X�o�[�Ƀo�C�g����\������
	{
		MainSttBarSetByteCount( dBytes );
	}

	if( gitFileIt->vcCont.at( dPage ).ptRawData )
	{
		dLines = gitFileIt->vcCont.at( dPage ).iLineCnt;
	}
	else
	{
		dLines = gitFileIt->vcCont.at( dPage ).ltPage.size( );
	}

	PageListInfoSet( dPage, dBytes, dLines );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��s�̃e�L�X�g���w�肵������������Q�b�g����
	�|�C���^�����n���Ă������Ń������m�ۂ���B�J���͌Ă񂾑���
	@param[in]	rdLine		�Ώۂ̍s�ԍ��E��΂O�C���f�b�N�X��
	@param[in]	iStart		�J�n�����ʒu�O�C���f�b�N�X�E���̕�������J�n�E�펞�O�ł悢�H
	@param[out]	*pstTexts	�����ƃX�^�C�����i�[����o�b�t�@����邽�߂̃|�C���^�[�ւ̃|�C���^�[�ENULL�Ȃ�K�v�����������Ԃ�
	@param[out]	pchLen		�m�ۂ����������ENULL�^�[�~�l�[�^�̓m�[�J���E�o�C�g����Ȃ���
	@param[out]	pdFlag		������ɂ��āE���ʂ̂Ƃ��A���󔒂Ƃ��ENULL�s��
	@return					������̎g�p�h�b�g��
*/
INT DocLineDataGetAlloc( INT rdLine, INT iStart, LPLETTER *pstTexts, PINT pchLen, PUINT pdFlag )
{
	INT		iSize, i = 0, j, dotCnt;
	INT_PTR	iCount, iLines;

	//	�n�_�ƏI�_���g����悤�ɂ���	//	�|�P�Ȃ疖�[

	LINE_ITR	itLine;

	iLines = DocNowFilePageLineCount( );
	if( iLines <=  rdLine )	return -1;


	itLine = gitFileIt->vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, rdLine );

	iCount = itLine->vcLine.size( );
	*pdFlag = 0;

	if( 0 == iCount )	//	������̒��g���Ȃ�
	{
		*pchLen = 0;
		dotCnt  = 0;
	}
	else
	{
		if( iStart >= iCount )	return 0;	//	�ʂ�߂���

		iSize = iCount - iStart;	//	������������
		//	�F�����̕K�v������Ƃ���܂łƂ��A��򂸂Ŗʓ|����悤��
		*pchLen = iSize;
		iSize++;	//	NULL�^�[�~�l�[�^�ׂ̈ɑ��₷

		if( !pstTexts )	return 0;	//	�����Ƃ���Ȃ��Ȃ炱���ŏI���

		*pstTexts = (LPLETTER)malloc( iSize * sizeof(LETTER) );
		if( !( *pstTexts ) ){	TRACE( TEXT("malloc error") );	return 0;	}

		ZeroMemory( *pstTexts, iSize * sizeof(LETTER) );

		dotCnt = 0;
		for( i = iStart, j = 0; iCount > i; i++, j++ )
		{
			(*pstTexts)[j].cchMozi = itLine->vcLine.at( i ).cchMozi;
			(*pstTexts)[j].rdWidth = itLine->vcLine.at( i ).rdWidth;
			(*pstTexts)[j].mzStyle = itLine->vcLine.at( i ).mzStyle;

			dotCnt += itLine->vcLine.at( i ).rdWidth;
		}

		//	���[��space���ǂ����m�F
		if( iswspace( itLine->vcLine.at( iCount-1 ).cchMozi ) )
		{	*pdFlag |= CT_LASTSP;	}
	}

	if( iLines - 1 >  rdLine )	*pdFlag |= CT_RETURN;	//	���̍s������Ȃ���s
	else						*pdFlag |= CT_EOF;		//	�Ȃ��Ȃ炱�̍s���[��EOF

	//	���s�̏�Ԃ��m��
	*pdFlag |= itLine->dStyle;

	return dotCnt;
}
//-------------------------------------------------------------------------------------------------


/*!
	�y�[�W�S�̂��m�ۂ���Efree�͌Ă񂾕��ł��
	@param[in]	bStyle	�P���j�R�[�h���V�t�gJIS
	@param[out]	*pText	�m�ۂ����̈��Ԃ��E���C�h�������}���`�����ɂȂ�ENULL���ƕK�v�o�C�g����Ԃ��̂�
	@return				�m�ۂ����o�C�g���ENULL�^�[�~�l�[�^���܂�
*/
INT DocPageGetAlloc( UINT bStyle, LPVOID *pText )
{
	return DocPageTextGetAlloc( gitFileIt, gixFocusPage, bStyle, pText, FALSE );
}
//-------------------------------------------------------------------------------------------------

/*!
	�y�[�W�S�̂𕶎���Ŋm�ۂ���Efree�͌Ă񂾕��ł��
	@param[in]	itFile	�m�ۂ���t�@�C��
	@param[in]	dPage	�m�ۂ���Ŕԍ�
	@param[in]	bStyle	�P���j�R�[�h���V�t�gJIS�ŁA��`���ǂ���
	@param[out]	pText	�m�ۂ����̈��Ԃ��E���C�h�������}���`�����ɂȂ�ENULL���ƕK�v�o�C�g����Ԃ��̂�
	@param[in]	bCrLf	���[�ɉ��s�t���邩
	@return				�m�ۂ����o�C�g���ENULL�^�[�~�l�[�^�܂�
*/
INT DocPageTextGetAlloc( FILES_ITR itFile, INT dPage, UINT bStyle, LPVOID *pText, BOOLEAN bCrLf )
{
	UINT_PTR	iLines, iLetters, j, iSize;
	UINT_PTR	i;
	UINT_PTR	cchSize;

	LPSTR		pcStr;

	string	srString;
	wstring	wsString;

	LINE_ITR	itLines;

#ifdef DO_TRY_CATCH
	try{
#endif

	srString.clear( );
	wsString.clear( );

	//	�f�t�H�I��
	if( 0 > dPage ){	dPage = gixFocusPage;	}


	if( itFile->vcCont.at( dPage ).ptRawData )	//	���f�[�^��ԂȂ�
	{
		if( bStyle & D_UNI )	//	���j�R�[�h�ł���
		{
			wsString = wstring( itFile->vcCont.at( dPage ).ptRawData );
			if( bCrLf ){	wsString += wstring( CH_CRLFW );	}
		}
		else	//	ShiftJIS�ł���
		{
			pcStr = SjisEncodeAlloc( itFile->vcCont.at( dPage ).ptRawData );	//	�y�[�W�S�̂𕶎���Ŋm��
			if( pcStr )
			{
				srString = string( pcStr );
				if( bCrLf ){	srString +=  string( CH_CRLFA );	}

				FREE( pcStr );
			}
		}
	}
	else	//	���[�h�ς݂Ȃ�
	{
		//	�S�����𒸂�
		iLines = itFile->vcCont.at( dPage ).ltPage.size( );

		for( itLines = itFile->vcCont.at( dPage ).ltPage.begin(), i = 0;
		itLines != itFile->vcCont.at( dPage ).ltPage.end();
		itLines++, i++ )
		{
			iLetters = itLines->vcLine.size( );

			for( j = 0; iLetters > j; j++ )
			{
				srString +=  string( itLines->vcLine.at( j ).acSjis );
				wsString += itLines->vcLine.at( j ).cchMozi;
			}

			if( !(1 == iLines && 0 == iLetters) )	//	��s���땶���͋�ł���
			{
				if( iLines > (i+1) )	//	�Ƃ肠�����t�@�C�����[���s�͂����ł͕t���Ȃ�
				{
					srString +=  string( CH_CRLFA );
					wsString += wstring( CH_CRLFW );
				}
			}
		}

		if( bCrLf )
		{
			srString +=  string( CH_CRLFA );
			wsString += wstring( CH_CRLFW );
		}
	}

	if( bStyle & D_UNI )
	{
		cchSize = wsString.size(  ) + 1;	//	NULL�^�[�~�l�[�^
		iSize = cchSize * sizeof(TCHAR);	//	���j�R�[�h�Ȃ̂Ńo�C�g���͂Q�{�ł���

		if( pText )
		{
			*pText = (LPTSTR)malloc( iSize );
			ZeroMemory( *pText, iSize );
			StringCchCopy( (LPTSTR)(*pText), cchSize, wsString.c_str( ) );
		}
	}
	else
	{
		iSize = srString.size( ) + 1;	//	NULL�^�[�~�l�[�^

		if( pText )
		{
			*pText = (LPSTR)malloc( iSize );
			ZeroMemory( *pText, iSize );
			StringCchCopyA( (LPSTR)(*pText), iSize, srString.c_str( ) );
		}
	}

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (INT)ETC_MSG( err.what(), 0 );	}
	catch( ... ){	return (INT)ETC_MSG( ("etc error") , 0 );	}
#endif

	return (INT)iSize;
}
//-------------------------------------------------------------------------------------------------

/*!
	���ݕł̎w��s�𕶎���Ŋm�ۂ���Efree�͌Ă񂾕��ł��E�t�@�C���ƕŎw��ł����ق���������
	@param[in]	itFile	�m�ۂ���t�@�C��
	@param[in]	dPage	�m�ۂ���Ŕԍ�
	@param[in]	bStyle	���j�R�[�h���V�t�gJIS
	@param[in]	dTarget	�s�ԍ��O�C���f�b�N�X
	@param[out]	pText	�m�ۂ����̈��Ԃ��E���C�h�������}���`�����ɂȂ�ENULL���ƕK�v�o�C�g����Ԃ��̂�
	@return		�m�ۂ����o�C�g���ENULL�^�[�~�l�[�^�܂�
*/
INT DocLineTextGetAlloc( FILES_ITR itFile, INT dPage, UINT bStyle, UINT dTarget, LPVOID *pText )
{
	UINT_PTR	dLines;
	UINT_PTR	dLetters, j, iSize;
	UINT_PTR	cchSize;

	string	srString;
	wstring	wsString;

	LINE_ITR	itLines;

	//	�ʒu�m�F
	dLines = itFile->vcCont.at( dPage ).ltPage.size( );
	if( dLines <= dTarget ){	return 0;	}

	itLines = itFile->vcCont.at( dPage ).ltPage.begin();
	std::advance( itLines, dTarget );

	dLetters = itLines->vcLine.size( );
	for( j = 0; dLetters > j; j++ )
	{
		srString +=  string( itLines->vcLine.at( j ).acSjis );
		wsString +=  itLines->vcLine.at( j ).cchMozi;
	}


	if( bStyle & D_UNI )
	{
		cchSize = wsString.size(  ) + 1;	//	NULL�^�[�~�l�[�^
		iSize = cchSize * sizeof(TCHAR);	//	���j�R�[�h�Ȃ̂Ńo�C�g���͂Q�{�ł���

		if( pText )
		{
			*pText = (LPTSTR)malloc( iSize );
			ZeroMemory( *pText, iSize );
			StringCchCopy( (LPTSTR)(*pText), cchSize, wsString.c_str( ) );
		}
	}
	else
	{
		iSize = srString.size( ) + 1;	//	NULL�^�[�~�l�[�^

		if( pText )
		{
			*pText = (LPSTR)malloc( iSize );
			ZeroMemory( *pText, iSize );
			StringCchCopyA( (LPSTR)(*pText), iSize, srString.c_str( ) );
		}
	}
	return (INT)iSize;
}
//-------------------------------------------------------------------------------------------------

#if 0
/*!
	�y�[�W�S�̂��m�ۂ���Efree�͌Ă񂾕��ł��
	@param[in]	bStyle	�P���j�R�[�h���V�t�gJIS
	@param[out]	*pText	�m�ۂ����̈��Ԃ��E���C�h�������}���`�����ɂȂ�ENULL���ƕK�v�o�C�g����Ԃ��̂�
	@return				�m�ۂ����o�C�g���ENULL�^�[�~�l�[�^���܂�
*/
INT DocPageTextAllGetAlloc( UINT bStyle, LPVOID *pText )
{
	//	SJIS�̏ꍇ�́A���j�R�[�h������&#dddd;�Ŋm�ۂ����

	UINT_PTR	iLines, i, iLetters, j;
	UINT_PTR	cchSize;
	INT_PTR		iSize;

	LPTSTR		ptData;
	LPSTR		pcStr;

	string	srString;	//	���j�R�[�h�E�V�t�gJIS�Ŋm��
	wstring	wsString;

	LINE_ITR	itLine;


	srString.clear( );
	wsString.clear( );

	if( gitFileIt->vcCont.at( gixFocusPage ).ptRawData )	//	���f�[�^��ԂȂ�
	{
		ptData = (*gitFileIt).vcCont.at( gixFocusPage ).ptRawData;
		StringCchLength( ptData, STRSAFE_MAX_CCH, &cchSize );

		if( bStyle & D_UNI )	//	���j�R�[�h�ł���
		{
			iSize = (cchSize+1) * sizeof(TCHAR);	//	NULL�^�[�~�l�[�^������

			if( pText )
			{
				*pText = (LPTSTR)malloc( iSize );
				ZeroMemory( *pText, iSize );
				StringCchCopy( (LPTSTR)(*pText), cchSize, ptData );
			}
		}
		else
		{
			pcStr = SjisEncodeAlloc( ptData );
			if( pcStr )
			{
				StringCchLengthA( pcStr, STRSAFE_MAX_CCH, &cchSize );
				iSize = cchSize + 1;	//	NULL�^�[�~�l�[�^������

				if( pText ){	*pText =  pcStr;	}
				else{	FREE( pcStr );	}
			}
		}
	}
	else
	{
		//	�y�[�W�S�̂̍s��
		iLines = DocNowFilePageLineCount( );

		itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();

		for( i = 0; iLines > i; i++, itLine++ )
		{
			//	�e�s�̕�����
			iLetters = itLine->vcLine.size( );

			if( bStyle & D_UNI )
			{
				for( j = 0; iLetters > j; j++ )
				{
					wsString += itLine->vcLine.at( j ).cchMozi;
				}

				if( iLines > (i+1) )	wsString += wstring( CH_CRLFW );
			}
			else
			{
				for( j = 0; iLetters > j; j++ )
				{
					srString +=  string( itLine->vcLine.at( j ).acSjis );
				}

				if( iLines > (i+1) )	srString +=  string( CH_CRLFA );
			}
		}

		if( bStyle & D_UNI )	//	���j�R�[�h�ł���
		{
			cchSize = wsString.size(  ) + 1;	//	NULL�^�[�~�l�[�^������
			iSize = cchSize * sizeof(TCHAR);	//	���j�R�[�h�Ȃ̂Ńo�C�g���͂Q�{�ł���

			if( pText )
			{
				*pText = (LPTSTR)malloc( iSize );
				ZeroMemory( *pText, iSize );
				StringCchCopy( (LPTSTR)(*pText), cchSize, wsString.c_str( ) );
			}
		}
		else
		{
			iSize = srString.size( ) + 1;	//	NULL�^�[�~�l�[�^������

			if( pText )
			{
				*pText = (LPSTR)malloc( iSize );
				ZeroMemory( *pText, iSize );
				StringCchCopyA( (LPSTR)(*pText), iSize, srString.c_str( ) );
			}
		}
	}

	return iSize;
}
//-------------------------------------------------------------------------------------------------
#endif

/*!
	�w�肳�ꂽ�y�[�W�S�̂��v���r���[�p��SJIS�Ŋm�ۂ���Efree�͌Ă񂾕��ł��
	@param[in]	iPage	�^�[�Q�b�g�Ŕԍ�
	@param[out]	pdBytes	�m�ۂ����o�C�g���Ԃ��ENULL�^�[�~�l�[�^���܂�
	@return				�m�ۂ����̈�ESJIS������ł���
*/
LPSTR DocPageTextPreviewAlloc( INT iPage, PINT pdBytes )
{
	//	SJIS�̏ꍇ�́A���j�R�[�h������&#dddd;�Ŋm�ۂ����

	UINT_PTR	iLines, i, iLetters;
	INT_PTR		iSize;
	LPSTR	pcText = NULL;
	CHAR	acEntity[10];

	string	srString;	//	�V�t�gJIS�Ŋm��
	LINE_ITR	itLine, itLineEnd;
	LETR_ITR	itLtr;

	TCHAR	atEntity[10];
	wstring	widString;
	LPTSTR	ptCaret;

	srString.clear( );

	if( pdBytes )	*pdBytes = 0;

	if( DocRangeIsError( gitFileIt, iPage, 0 ) )	return NULL;

	widString.clear();

	if(  (*gitFileIt).vcCont.at( iPage ).ptRawData )	//	���f�[�^��ԂȂ�
	{
		ptCaret = (*gitFileIt).vcCont.at( iPage ).ptRawData;
		StringCchLength( ptCaret, STRSAFE_MAX_CCH, &iLetters );

		for( i = 0; iLetters > i; i++ )
		{
			if( HtmlEntityCheckW( ptCaret[i], atEntity, 10 ) )
			{
				widString += wstring( atEntity );
			}
			else if( TEXT('\r') == ptCaret[i] )
			{
				widString += wstring( TEXT("<br>") );
				i++;	//	0x0A���i�߂�
			}
			else
			{
				widString += ptCaret[i];
			}
		}

		widString += wstring( TEXT("<br>") );	//	�����ɉ��s�����Ă����H

		pcText = SjisEncodeAlloc( widString.c_str() );	//	Preview�p
		iSize = strlen( pcText );
	}
	else
	{
		//	�y�[�W�S�̂̍s��
		iLines    = (*gitFileIt).vcCont.at( iPage ).ltPage.size( );

		itLine    = (*gitFileIt).vcCont.at( iPage ).ltPage.begin( );
		itLineEnd = (*gitFileIt).vcCont.at( iPage ).ltPage.end( );

		for( i = 0; itLine != itLineEnd; i++, itLine++ )
		{
			//	�e�s�̕�����
			iLetters = itLine->vcLine.size( );

			for( itLtr = itLine->vcLine.begin(); itLtr != itLine->vcLine.end(); itLtr++ )
			{
				//	HTML�I�Ƀ��o�C�������G���e�B�e�B����
				if( HtmlEntityCheckA( itLtr->cchMozi, acEntity, 10 ) )
				{
					srString +=  string( acEntity );
				}
				else
				{
					srString +=  string( itLtr->acSjis );
				}
			}

	//�S�s�ɉ��s�����Ă��܂�Ȃ��H
			srString +=  string( "<br>" );
		}

		iSize = srString.size( ) + 1;	//	NULL�^�[�~�l�[�^������

		pcText = (LPSTR)malloc( iSize );
		ZeroMemory( pcText, iSize );
		StringCchCopyA( pcText, iSize, srString.c_str( ) );
	}

	if( pdBytes )	*pdBytes = iSize;

	return pcText;
}
//-------------------------------------------------------------------------------------------------


/*!
	���݂̃��j�R�[�h���l�Q�Ƃ̋�ɍ��킹�ĕ�������`�F�C���W
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT UnicodeRadixExchange( LPVOID pVoid )
{
	INT_PTR	iPage, iLine, iMozi, dP, dL, dM;
	TCHAR	cchMozi;
	CHAR	acSjis[10];

	LINE_ITR	itLine;

	iPage = DocNowFilePageCount(  );

	for( dP = 0; iPage >  dP; dP++ )	//	�S��
	{
		iLine = (*gitFileIt).vcCont.at( dP ).ltPage.size(  );

		itLine = (*gitFileIt).vcCont.at( dP ).ltPage.begin();
		for( dL = 0; iLine >  dL; dL++, itLine++ )	//	�S�s
		{
			iMozi = itLine->vcLine.size(  );

			for( dM = 0; iMozi >  dM; dM++ )	//	�S��
			{
				if( itLine->vcLine.at( dM ).mzStyle & CT_CANTSJIS )
				{
					cchMozi = itLine->vcLine.at( dM ).cchMozi;
					if( gbUniRadixHex ){	StringCchPrintfA( acSjis, 10, ("&#x%X;"), cchMozi );	}
					else{					StringCchPrintfA( acSjis, 10, ("&#%d;"),  cchMozi );	}

					StringCchCopyA( itLine->vcLine.at( dM ).acSjis, 10, acSjis );
//	TODO:	�o�C�g���Čv�Z���K�v
				}
			}
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�y�[�W���������E�J�[�\���ʒu�̎��̍s����I���܂ł����̕ł�
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	hInst	�����n���h��
	@param[in]	iNow	���̍s
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocPageDivide( HWND hWnd, HINSTANCE hInst, INT iNow )
{
	INT	iDivLine = iNow + 1;
	INT	iLines, mRslt, iNewPage;
//	INT_PTR	iTotal, iNext;
	ONELINE	stLine;
	LINE_ITR	itLine, itEnd;

	ZeroONELINE( &stLine );

	//mRslt = MessageBox( hWnd, TEXT("��������������畜�A�ł��Ȃ���E�E�E\r\n�{���Ƀo�����Ă����H"), TEXT("�m�F�ł�"), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 );
	mRslt = MessageBoxCheckBox( hWnd, hInst, 1 );
	if( IDNO == mRslt ){	return  E_ABORT;	}

//�����́A�A���h�D�����Z�b�g���ׂ����v�͍폜�Ȃ̂ŁA���Z�b�g���Ȃ��Ă�����������
//���̕ł̊Y���������폜�����Ⴄ

	iLines = DocNowFilePageLineCount(  );//DocPageParamGet( NULL, NULL );	//	�s���m��

	if( iLines <= iDivLine )	return E_OUTOFMEMORY;

	//	���̕ł̎��ɍ쐬
	//iTotal = DocNowFilePageCount(  );
	//iNext = gixFocusPage + 1;	//	���̕�
	//if( iTotal <= iNext ){	iNext =  -1;	}	//	�S�ł�葽���Ȃ疖�[�w��

	iNewPage = DocPageCreate( gixFocusPage );	//	�V��
	PageListInsert( iNewPage  );	//	�y�[�W���X�g�r���[�ɒǉ�

	//	��̈�s������Ă�̂ŁA�폜���Ă���
	(*gitFileIt).vcCont.at( iNewPage ).ltPage.clear(  );

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin( );
	std::advance( itLine, iDivLine );	//	�Y���s�܂Ői�߂�

	itEnd  = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.end( );	//	���[�ʒu�m��

	std::copy( itLine, itEnd, back_inserter( (*gitFileIt).vcCont.at( iNewPage ).ltPage ) );

	(*gitFileIt).vcCont.at( gixFocusPage ).ltPage.erase( itLine, itEnd );

	SqnFreeAll( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog) );	//	�A���h�D���O�폜

	//	�o�C�g���Ƃ��̎�蒼��
	DocPageByteCount( gitFileIt, gixFocusPage, NULL, NULL );
	DocPageInfoRenew( gixFocusPage, TRUE );

	DocPageByteCount( gitFileIt, iNewPage, NULL, NULL );
	DocPageInfoRenew( iNewPage, FALSE );

	ViewRedrawSetLine( -1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�z�b�g�L�[�ɂ�铊���̒���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocThreadDropCopy( VOID )
{
	CHAR	acBuf[260];
	TCHAR	atTitle[64], atInfo[256];
	INT	cbSize, maxPage;//, dFocusBuf;
	LPVOID	pcString = NULL;

//	dFocusBuf = gixFocusPage;	//	���ݕł���U�Ҕ�������
//	gixFocusPage = gixDropPage;	//	�����p�łɂ���

//	cbSize = DocPageTextAllGetAlloc( D_SJIS, &pcString );
	cbSize = DocPageTextGetAlloc( gitFileIt, gixDropPage, D_SJIS, &pcString, FALSE );

//	gixFocusPage = dFocusBuf;	//	�I�������߂�

	TRACE( TEXT("%d �ł��R�s�["), gixDropPage );

	DocClipboardDataSet( pcString, cbSize, D_SJIS );

	ZeroMemory( acBuf, sizeof(acBuf) );
	StringCchCopyNA( acBuf, 260, (LPCSTR)pcString, 250 );
	ZeroMemory( atInfo, sizeof(atInfo) );
	MultiByteToWideChar( CP_ACP, 0, acBuf, (INT)strlen(acBuf), atInfo, 256 );

	StringCchPrintf( atTitle, 64, TEXT("%d �ł��R�s�[������"), gixDropPage + 1 );

	NotifyBalloonExist( atInfo, atTitle, NIIF_INFO );

	FREE( pcString );

	gixDropPage++;	//	���̕ł�

	maxPage = DocNowFilePageCount(  );
	if( maxPage <= gixDropPage )	gixDropPage = 0;
	//	�ŏI�ł܂ŃC�b����擪�ɖ߂�


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I��͈͂̃e�L�X�g��Ŗ��̂ɂ���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocSelText2PageName( VOID )
{

	INT	cbSize;
	LPVOID	pString = NULL;
	LPTSTR	ptText;
	UINT_PTR	cchSize, d;

	if( !( IsSelecting( NULL ) ) )	return  E_ABORT;	//	�I�����ĂȂ��Ȃ牽�����Ȃ�

	cbSize = DocSelectTextGetAlloc( D_UNI, &pString, NULL );	//	�I��͈͂���������
	TRACE( TEXT("BYTE:%d"), cbSize );

	ptText = (LPTSTR)pString;
	StringCchLength( ptText, STRSAFE_MAX_CCH, &cchSize );

	for( d = 0; cchSize > d; d++ )	//	���s�J�b�g
	{
		if( 0x0D == ptText[d] )
		{
			ptText[d] = NULL;
			break;
		}
	}

	PageListNameRewrite( ptText );

	FREE( pString );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

