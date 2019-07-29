/*! @file
	@brief ���[�U��`�}���̖ʓ|���܂�
	���̃t�@�C���� UserDefinition.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/06/16
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

//	���[�U��`���
typedef struct tagUSERITEMS
{
	TCHAR	atItemName[MAX_STRING];

	vector<ONELINE>	vcUnits;	//!<	�S��

} USERITEMS, *LPUSERITEMS;
//-------------------------------------------------------------------------------------------------

extern INT		gdDocLine;		//!<	�L�����b�g�̂x�s���E�h�L�������g�ʒu


static TCHAR	gatUsDfPath[MAX_PATH];	//!<	

static  UINT	gdItemCnt;	//!<	�o�^����Ă���A�C�e����

static USERITEMS	gstUserItem[USER_ITEM_MAX];	//!<	���[�U�A�C�e���̕ێ�
//-------------------------------------------------------------------------------------------------

UINT	CALLBACK UserDefItemLoad( LPTSTR, LPCTSTR, INT );	//!<	���[�U��`�̓��e���Ԃ�����

HRESULT	UserDefAppendMenu( HWND );	//!<	���[�U��`�̓��e�����j���[�ɒǉ�
//-------------------------------------------------------------------------------------------------


/*!
	���J���Ă�USER��`�̓��e��S�Ĕj�����ĐV�����̍����Ԃɂ���
	@param[in]	hWnd	���C���E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT UserDefObliterate( HWND hWnd )
{
	UINT_PTR	i, iLine;
	UINT_PTR	j;

	for( i = 0; USER_ITEM_MAX > i; i++ )
	{
		iLine = gstUserItem[i].vcUnits.size( );
		for( j = 0; iLine > j; j++ )
		{
			gstUserItem[i].vcUnits.at( j ).vcLine.clear(   );	//	�e�s�̒��g�S����
		}
		gstUserItem[i].vcUnits.clear(  );	//	�s��S����

		ZeroMemory( gstUserItem[i].atItemName, sizeof(gstUserItem[i].atItemName) );
	}

	gdItemCnt = 0;

	//	���j���[�̓��e���A�����ɂ��Ƃ���

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���[�U��`�̃A����ǂݍ���
	@param[in]	hWnd	���C���E�C���h�E�n���h��
	@param[in]	bFirst	���񂪍ŏ��ł��邩
	@return	��O���g��������������@�O�t�@�C���Ȃ�����
*/
INT UserDefInitialise( HWND hWnd, UINT bFirst )
{
	CONST WCHAR rtHead = 0xFEFF;	//	���j�R�[�h�e�L�X�g�w�b�_
	WCHAR	rtUniBuf;

	HANDLE	hFile;
	DWORD	readed;

	LPVOID	pBuffer;	//	������o�b�t�@�p�|�C���^�[
	INT		iByteSize;

	LPTSTR	ptString;
	LPSTR	pcText;
	UINT	cchSize;

	if( bFirst )	//	�ŏ��Ȃ�p�X����Ă���
	{
		ZeroMemory( gatUsDfPath, sizeof(gatUsDfPath) );
		StringCchCopy( gatUsDfPath, MAX_PATH, ExePathGet() );
		PathAppend( gatUsDfPath, TEMPLATE_DIR );
		PathAppend( gatUsDfPath, USER_ITEM_FILE );
	}

	hFile = CreateFile( gatUsDfPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE == hFile ){	return 0;	}

	//	���̓��e�j��
	UserDefObliterate( hWnd );

	iByteSize = GetFileSize( hFile, NULL );
	pBuffer = malloc( iByteSize + 2 );
	ZeroMemory( pBuffer, iByteSize + 2 );
	//	���g����������
	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
	ReadFile( hFile, pBuffer, iByteSize, &readed, NULL );
	CloseHandle( hFile );	//	���e�S����荞�񂾂���J��

	//	���j�R�[�h�`���b�N
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
		ptString = SjisDecodeAlloc( pcText );	//	SJIS�̓��e�����j�R�[�h�ɂ���

		FREE( pBuffer );	//	�������ŊJ��
		pBuffer = ptString;	//	�|�C���g����Ƃ����ύX
	}

	StringCchLength( ptString, STRSAFE_MAX_CCH, &cchSize );

	//	AST����Ȃ���_��
	if( !( StrCmpN( AST_SEPARATERW, ptString, 4 ) ) )
	{
		DocStringSplitAST( ptString , cchSize, UserDefItemLoad );
	}

	FREE( pBuffer );

	UserDefAppendMenu( hWnd );

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	���e���Ԃ�����
	@param[in]	ptName	���ڂ̖��O
	@param[in]	ptCont	���ڂ̓��e
	@param[in]	cchSize	���e�̕�����
	@return	��O����肵���@�O���Ȃ�����
*/
UINT CALLBACK UserDefItemLoad( LPTSTR ptName, LPCTSTR ptCont, INT cchSize )
{
	//	�g���̂͂���
	//	���O���Z�b�g���Ă���

	//	�͂ݏo�����͉������Ȃ���
	if( USER_ITEM_MAX <= gdItemCnt )	return 0;

	if( ptName )
	{
		StringCchCopy( gstUserItem[gdItemCnt].atItemName, MAX_STRING, ptName );
	}
	else
	{
		StringCchPrintf( gstUserItem[gdItemCnt].atItemName, MAX_STRING, TEXT("(���[�U�A�C�e�� No.%d"), gdItemCnt + 1 );
	}

	if( 0 < cchSize )	//	��s�łȂ��̂Ȃ�
	{
		UserDefSetString( &(gstUserItem[gdItemCnt].vcUnits), ptCont, cchSize );	//	���̒��ŉ��s�Ƃ��ʓ|����
	}

	gdItemCnt++;

	return 1;
}
//-------------------------------------------------------------------------------------------------


/*!
	�ێ��\���̂ɕ�������L�^����
	@param[in]	*pvcUnits	�ێ��x�N�[�^�ւ̃|�C���^�[
	@param[in]	ptText		�L�^���镶����
	@param[in]	cchSize		������
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT UserDefSetString( vector<ONELINE> *pvcUnits, LPCTSTR ptText, UINT cchSize )
{
	UINT	i;
	INT		yLine;
	ONELINE	stLine;
	LETTER	stLetter;

	ZeroONELINE( &stLine );

	pvcUnits->push_back( stLine );	//	�딭��

	yLine = 0;
	for( i = 0; cchSize > i; i++ )
	{
		if( CC_CR == ptText[i] && CC_LF == ptText[i+1] )	//	���s�ł�������
		{
			pvcUnits->push_back( stLine );

			i++;		//	0x0D,0x0A������A�땶����΂��̂��|�C���g
			yLine++;	//	���s��������Focus�͎��̍s��
		}
		else if( CC_TAB == ptText[i] )
		{
			//	�^�u�͑}�����Ȃ�
		}
		else
		{
			DocLetterDataCheck( &stLetter, ptText[i] );	//	�ێ��\���̂ɕ�������L�^����Ƃ�

			pvcUnits->at( yLine ).vcLine.push_back( stLetter );

			pvcUnits->at( yLine ).iDotCnt += stLetter.rdWidth;
			pvcUnits->at( yLine ).iByteSz += stLetter.mzByte;
		}
	}

	//	�����Ƃ��̋󔒂��܂߂ăe���v�����ȂƎv���Ă�

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���[�U��`�̓��e�����j���[�ɒǉ�
	@param[in]	hWnd	���C���E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT UserDefAppendMenu( HWND hWnd )
{
	HMENU	hMenu, hSubMenu;

#pragma message("���j���[�\���ς�����烆�[�U��`�̈ʒu�ł��邱�����ύX")
	hMenu = GetMenu( hWnd );
	hSubMenu = GetSubMenu( hMenu, 2 );
	hMenu = hSubMenu;
	hSubMenu = GetSubMenu( hMenu, 10 );

	UserDefMenuWrite( hSubMenu, 1 );

	DeleteMenu( hSubMenu, IDM_USERINS_NA, MF_BYCOMMAND );	//	Dummy�p

	DrawMenuBar( hWnd );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���[�U��`���j���[�̒��g����������
	@param[in]	hMenu	�������郁�j���[�̃n���h��
	@param[in]	bMode		��O���j���[�L�[�t����@�O�t���Ȃ�
*/
HRESULT UserDefMenuWrite( HMENU hMenu, UINT bMode )
{
	UINT	i;
	TCHAR	atBuffer[MAX_PATH];

	for( i = 0; gdItemCnt > i; i++ )
	{
		if( bMode )
		{
			StringCchPrintf( atBuffer, MAX_PATH, TEXT("%s(&%c)"), gstUserItem[i].atItemName, i+'A' );
		}
		else
		{
			StringCchCopy( atBuffer, MAX_PATH, gstUserItem[i].atItemName );
		}
		AppendMenu( hMenu, MF_STRING, (IDM_USERINS_ITEM_FIRST + i), atBuffer );
	}
	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e���̖��O����������
	@param[in]	dNumber	�A�C�e���ԍ��O�C���f�b�N�X
	@param[out]	ptNamed	���O�����o�b�t�@�ւ̃|�C���^�[
	@param[in]	cchSize	�o�b�t�@�̕������E�o�C�g����Ȃ���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT UserDefItemNameget( UINT dNumber, LPTSTR ptNamed, UINT_PTR cchSize )
{
	//	�͂ݏo���m�F
	if( gdItemCnt <= dNumber )	return E_OUTOFMEMORY;

	StringCchCopy( ptNamed, cchSize, gstUserItem[dNumber].atItemName );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ΏۃA�C�e���̑Ώۍs�̓��e���e�L�X�g�Ŋm�ہE���s�͊܂܂�
	@param[in]	idNum	�A�C�e���ԍ��O�`�P�T
	@param[in]	uLine	�s�ԍ�
	@return		LPTSTR	�m�ۂ������e�E�J���͌Ă񂾑����ӔC���Ă�
*/
LPTSTR UserDefTextLineAlloc( UINT idNum, INT uLine )
{
	INT_PTR	iLines, iLetters, i, cchSz;
	LPTSTR	ptText;
	LETR_ITR	itLetter;


	if( gdItemCnt <= idNum )	return NULL;

	iLines = gstUserItem[idNum].vcUnits.size( );
	if( iLines <= uLine )	return NULL;

	iLetters = gstUserItem[idNum].vcUnits.at( uLine ).vcLine.size( );

	cchSz = iLetters + 1;

	ptText = (LPTSTR)malloc( cchSz * sizeof(TCHAR) );
	ZeroMemory( ptText, cchSz * sizeof(TCHAR) );

//	for( i = 0; iLetters > i; i++ ){	ptText[i] = gstUserItem[idNum].vcUnits.at( uLine ).vcLine.at( i ).cchMozi;	}
	for( itLetter = gstUserItem[idNum].vcUnits.at( uLine ).vcLine.begin(), i = 0;
		itLetter != gstUserItem[idNum].vcUnits.at( uLine ).vcLine.end(); i++, itLetter++ )
	{	ptText[i] = itLetter->cchMozi;	}

	ptText[i] = NULL;

	return ptText;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I�����ꂽ���[�U��`�A�C�e���̏���
	@param[in]	hWnd	�������C���E�C���h�E�n���h��
	@param[in]	idNum	�A�C�e���ԍ��O�`�P�T
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT UserDefItemInsert( HWND hWnd, UINT idNum )
{
	INT		iLines, yLine, iMinus, i, dmyDot;
	INT_PTR	dNeedLine;
	LPTSTR	ptText;
	BOOLEAN	bFirst = TRUE;


	//	�͂ݏo������A�E�c�I
	if( gdItemCnt <= idNum )	return E_OUTOFMEMORY;

	//	���̃J�[�\���s����A�s�擪�ɁA�e�s�̓��e��}�����Ă���
	yLine = gdDocLine;

	dNeedLine = gstUserItem[idNum].vcUnits.size( );

	//	�܂��͕ōs�������ʂ�
	iLines = DocPageParamGet( NULL , NULL );	//	�s���m�F�E����ւ��Ă����邩
	//	�S�̍s�����A�ǉ��s��������������A���s���₷
	if( iLines < (dNeedLine + yLine) )
	{
		iMinus = (dNeedLine + yLine) - iLines;	//	�ǉ�����s��

		DocAdditionalLine( iMinus, &bFirst );//	bFirst = FALSE;

		//	���̕ł̍s����蒼��
		iLines = DocPageParamGet( NULL , NULL );	//	�Čv�Z���邩�H
	}

	for( i = 0; dNeedLine > i; i++, yLine++ )
	{
		ptText = UserDefTextLineAlloc( idNum, i );

		dmyDot = 0;
		DocInsertString( &dmyDot, &yLine, NULL, ptText, 0, bFirst );	bFirst = FALSE;

		FREE(ptText);
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

