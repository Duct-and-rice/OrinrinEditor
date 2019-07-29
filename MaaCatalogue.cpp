/*! @file
	@brief MLT�t�@�C����ǂ�ŁA���g���������ɓW�J���܂�
	���̃t�@�C���� MaaCatalogue.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/06/22
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
#include "MaaTemplate.h"
//-------------------------------------------------------------------------------------------------

/*

AA��1�s��������2�s�ŁA�����e���ȉ��̕�����ȊO����n�܂�ꍇ�A
����AA�������o���Ƃ��ĔF�����Amenu���ڂɒǉ����܂��B

Last Mod
�ŏI�X
�@�@�i�S�p��2�j
�@ �i�S�p�󔒁{���p�󔒁j
:
_
�i
(
.
/

�������A�u��؂�.mlt�v�E�u�����o���E�͂ݗp.mlt�v�ɂ��ẮA�ȉ��̕����񂩂�n�܂�ꍇ�̂݁A
����AA�������o���Ƃ��ĔF���Amenu���ڂɒǉ����܂��B

�y

�y�@�����Q�O�����ȏ�͔F�����Ȃ��Ƃ��̃��W�b�N�K�v��
*/


#define MLT_SEPARATER	("[SPLIT]")
#define AST_SEPARATER	("[AA]")

//	�\���̒�`�EOrinrinEditor.h�ֈړ�

//-------------------------------------------------------------------------------------------------

static  vector<AAMATRIX>	gvcArts;	//!<	�J����AA�̕ێ�

static TCHAR	gatOpenFile[MAX_PATH];	//!<	�J�����t�@�C����

static TCHAR	gatBkUpDir[MAX_PATH];	//!<	MLT�������������̃o�b�N�A�b�v�쐬
//-------------------------------------------------------------------------------------------------


DWORD	AacInflateMlt( LPSTR, DWORD );
DWORD	AacInflateAst( LPSTR, DWORD );

UINT	AacTitleCheck( LPAAMATRIX );

LRESULT	CALLBACK AacFavInflate( UINT, UINT, UINT, LPCVOID );

#ifdef MAA_IADD_PLUS
UINT	AacItemBackUpCreate( LPVOID );
HRESULT	AacItemOutput( HWND );
#endif
//-------------------------------------------------------------------------------------------------

/*!
	�o�b�N�A�b�v�f�B���N�g���[���m�ہE�N�����Ɉ��Ă΂�邾��
	@param[in]	ptCurrent	��f�B���N�g��
*/
VOID AacBackupDirectoryInit( LPTSTR ptCurrent )
{
	StringCchCopy( gatBkUpDir, MAX_PATH, ptCurrent );
	PathAppend( gatBkUpDir, BACKUP_DIR );
	CreateDirectory( gatBkUpDir, NULL );	//	�߂Ƀf�B���N�g�����������生�������s���邾���Ȃ̂Ŗ�薳��

	return;
}
//-------------------------------------------------------------------------------------------------



/*!
	MLT�t�@�C�������󂯎���āA�W�J������i�߂�
	@param[in]	hWnd		�e�E�C���h�E�n���h��
	@param[in]	ptFileName	�t�@�C����
	@return		DWORD		�m�ۂ�����
*/
DWORD AacAssembleFile( HWND hWnd, LPTSTR ptFileName )
{
	HANDLE	hFile;
	DWORD	rdFileSize, readed, rdCount;
	BOOL	isAST;
	LPSTR	pcFullBuff;

	hFile = CreateFile( ptFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE == hFile ){	return 0;	}

	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );

	rdFileSize = GetFileSize( hFile, NULL );	//	�S�f�����ɑΉ����H

	StringCchCopy( gatOpenFile , MAX_PATH, ptFileName );	//	�t�@�C�����L�^

	//	AST�ł��邩�H
	isAST = PathMatchSpec( ptFileName , TEXT("*.ast") );

	//	�S�̃o�b�t�@�����
	pcFullBuff = (LPSTR)malloc( rdFileSize + 1 );
	if( !(pcFullBuff) ){	return 0;	}

	ZeroMemory( pcFullBuff, rdFileSize + 1 );
	//	���g�Ǎ�
	ReadFile( hFile, pcFullBuff, rdFileSize, &readed, NULL );

	CloseHandle( hFile );	//	���e�m�ۂ���������Ă���

	AacMatrixClear(   );	//	�����̓��e�S�j��

//	AST�͂����œW�J����΂���
	//	�W�J��������
	if( isAST ){	rdCount = AacInflateAst( pcFullBuff, readed );	}
	else{			rdCount = AacInflateMlt( pcFullBuff, readed );	}

#pragma message ("�������ǉ�������͖��[��0x0D���������Ă�E��΂��ēǂݍ��ޏ�����")

	free( pcFullBuff );	//	�J���Y��Ȃ��悤�ɒ��ӃZ��

	return rdCount;
}
//-------------------------------------------------------------------------------------------------

/*!
	AST�̑S�̃o�b�t�@���󂯎���āA�������ĕێ�
	@param[in]	pcTotal	�S�̃o�b�t�@
	@param[in]	cbTotal	�o�C�g��
	@return		DWORD	�m�ۂ�����
*/
DWORD AacInflateAst( LPSTR pcTotal, DWORD cbTotal )
{
	LPSTR	pcCaret;	//	�Ǎ��J�n�E���݈ʒu
	LPSTR	pcStart;	//	�Z�p���[�^�̒��O
	LPSTR	pcEnd;
	//LPSTR	pcCheck;
	UINT	iNumber;	//	�ʂ��ԍ��J�E���g

	UINT	cbItem;
	BOOLEAN	bLast;
//	CHAR	acName[MAX_STRING];

	AAMATRIX	stAAbuf;//	���AA�̕ێ��E�x�N�^�[�ɓ����


	bLast = FALSE;
	iNumber = 0;	//	�ʂ��ԍ��O�C���f�b�N�X

	pcCaret = StrStrA( pcTotal, AST_SEPARATER );	//	�ŏ��̃Z�p���[�^�܂Ői�߂�
	if( !pcCaret )	return 0;	//	���̎��_�Ŗ��������炨������

	do	//	ptCaret��[AA]�ɂȂ��Ă�͂�
	{
		ZeroMemory( &stAAbuf, sizeof(AAMATRIX) );

		stAAbuf.ixNum = iNumber;

		pcStart = NextLineA(  pcCaret );	//	���̍s���炪�{��

		pcCaret += 5;	//	[AA][
		cbItem  = pcStart - pcCaret;	//	���O�����̕�����
		cbItem -= 3;	//	]rn

	
		if( 0 < cbItem )	//	���O�m��
		{
			StringCchCopyNA( stAAbuf.acAstName, MAX_STRING, pcCaret, cbItem );
			AaTitleAddString( iNumber , stAAbuf.acAstName );	//	���o���ǉ�
		}

		pcCaret = pcStart;	//	�{�̕���

		pcEnd = StrStrA( pcCaret, AST_SEPARATER );	//	�Z�p���[�^��T��
		//	���̎��_��pcEnd�͎���[AA]�������Ă�E��������NULL(�Ō�̃R�})
		if( !pcEnd )	//	������Ȃ������灁���ꂪ�Ō�Ȃ灁NULL
		{
			pcEnd = pcTotal + cbTotal;	//	CHAR�T�C�Y�Ōv�Z�����H
			bLast = TRUE;
		}
		stAAbuf.cbItem = pcEnd - pcCaret;	//	CHAR�P�ʂł��邩

		//����	���[��0x0D�P�Ƃ��O��
		//pcCheck = pcEnd;	pcCheck--;
		//if( 0x0D == *pcCheck )	stAAbuf.cbItem--;

		//	�ŏI�łłȂ��ꍇ�͖��[�̉��s������
		//if( !(bLast) && 0 < cbItem ){	cbItem -= CH_CRLF_CCH;	}

		stAAbuf.pcItem = (LPSTR)malloc( stAAbuf.cbItem + 2 );
		ZeroMemory( stAAbuf.pcItem, stAAbuf.cbItem + 2 );

		if( 0 >= stAAbuf.cbItem )
		{
			stAAbuf.cbItem = 2;
			StringCchCopyA( stAAbuf.pcItem, stAAbuf.cbItem, (" ") );
		}
		else
		{
			CopyMemory( stAAbuf.pcItem, pcCaret, stAAbuf.cbItem );
		}

		gvcArts.push_back( stAAbuf );	//	�x�N�^�[�ɒǉ�
		//	������ stAAbuf.pcItem ���t���[���Ă͂����Ȃ��Evector�Ŏg���Ă���

		iNumber++;

		pcCaret = pcEnd;

	}while( *pcCaret );	//	�f�[�^�L����胋�[�v�ŒT��

	return iNumber;
}
//-------------------------------------------------------------------------------------------------

/*!
	MLT�̑S�̃o�b�t�@���󂯎���āA�������ĕێ�
	@param[in]	pcTotal	�S�̃o�b�t�@
	@param[in]	cbTotal	�o�C�g��
	@return		DWORD	�m�ۂ�����
*/
DWORD AacInflateMlt( LPSTR pcTotal, DWORD cbTotal )
{
	LPSTR	pcCaret;	//	�Ǎ��J�n�E���݈ʒu
	LPSTR	pcEnd;		//	���AA�̖��[�ʒu�E�Z�p���[�^�̒��O
	//LPSTR	pcCheck;
	DWORD	iNumber;	//	�ʂ��ԍ��J�E���g
	AAMATRIX	stAAbuf;//	���AA�̕ێ��E�x�N�^�[�ɓ����

	pcCaret = pcTotal;	//	�܂��͍ŏ�����

	iNumber = 0;	//	�ʂ��ԍ��O�C���f�b�N�X
	//	�n�_�ɂ̓Z�p���[�^�������̂Ƃ݂Ȃ��B�A������Z�p���[�^�́A���s�݂̂Ƃ��Ĉ���

	do
	{
		ZeroMemory( &stAAbuf, sizeof(AAMATRIX) );

		stAAbuf.ixNum = iNumber;

		pcEnd = StrStrA( pcCaret, MLT_SEPARATER );	//	�Z�p���[�^��T��
		if( !pcEnd )	//	������Ȃ������灁���ꂪ�Ō�Ȃ灁NULL
		{
			pcEnd = pcTotal + cbTotal;
		}
		stAAbuf.cbItem = pcEnd - pcCaret;	//	�o�C�g���Ȃ̂ł���ł����͂�

		//����	���[��0x0D�P�Ƃ��O��
		//if( 0 != stAAbuf.cbItem )
		//{
		//	pcCheck = pcEnd;	pcCheck--;
		//	if( 0x0D == *pcCheck )	stAAbuf.cbItem--;
		//}

		stAAbuf.pcItem = (LPSTR)malloc( stAAbuf.cbItem + 2 );
		ZeroMemory( stAAbuf.pcItem, stAAbuf.cbItem + 2 );

		if( 0 >= stAAbuf.cbItem )
		{
			stAAbuf.cbItem = 2;
			StringCchCopyA( stAAbuf.pcItem, stAAbuf.cbItem, (" ") );
		}
		else
		{
			CopyMemory( stAAbuf.pcItem, pcCaret, stAAbuf.cbItem );
		}

		AacTitleCheck( &stAAbuf );	//	���e�m�F���Ăł������Ȃ猩�o���ɂ���

		gvcArts.push_back( stAAbuf );	//	�x�N�^�[�ɒǉ�
		//	������ stAAbuf.pcItem ���t���[���Ă͂����Ȃ��Evector�Ŏg���Ă���

		iNumber++;

		pcCaret = NextLineA( pcEnd );

	}while( *pcCaret );	//	�f�[�^�L����胋�[�v�ŒT��


	return iNumber;
}
//-------------------------------------------------------------------------------------------------

/*!
	���o���ɂȂ蓾��Tango��T��
	@param[in]	pstItem	�`�F�b�N����^�[�Q�b�g���e
	@return		UINT	��O���o���������@�O�Ȃ�����
*/
UINT AacTitleCheck( LPAAMATRIX pstItem )
{
	LPSTR	pcCaret, pcEnd, pcOpen;
	LPSTR	pcLine;
	UINT	cbSize, d;

	if( 4 > pstItem->cbItem )	return 0;	//	�����������牽������

	//	�Ƃ肠�����擪��s�ڂɂ���
	pcCaret = pstItem->pcItem;

	//	�擪�ɂ���
	if( !( strncmp( pcCaret, "�y", 2 ) ) )	//	�u�����P�b�g�ł��邩
	{
		pcOpen = pcCaret + 2;	//	���e�J�n�ʒu

		cbSize  = pstItem->cbItem;
		cbSize -= 2;	//	��s�̏ꍇ�����z���Čv�Z

		pcEnd = NextLineA( pcCaret );
		//	���g��NULL�Ȃ�A���s������s�ł���
		if( *pcEnd )
		{
			pcCaret = pcOpen;
			cbSize = pcEnd - pcCaret;
		}
		//	�����ŁAcbSize�͈�s�̒����ɂȂ�
		pcLine = (LPSTR)malloc( cbSize+1 );
		ZeroMemory( pcLine, cbSize+1 );
		CopyMemory( pcLine, pcOpen, cbSize );
		pcCaret = pcLine;
		for( d = 0; cbSize > d; d++ )
		{
			if( !(strncmp( pcCaret, "�z", 2 ) ) )
			{
				*pcCaret = NULL;
				break;
			}
			pcCaret++;
		}
		//�����܂łŁApcLine�Ɂy�z�̕����񂪎c���Ă���͂�
		AaTitleAddString( pstItem->ixNum, pcLine );

		FREE(pcLine);
	}
	else
	{
		//	�擪������̕����ł���Ȃ猩�o���ł͂Ȃ�
		if( ':' ==  pcCaret[0] )	return 0;
		if( '_' ==  pcCaret[0] )	return 0;
		if( '(' ==  pcCaret[0] )	return 0;
		if( '.' ==  pcCaret[0] )	return 0;
		if( '/' ==  pcCaret[0] )	return 0;
		//	�擪������������̕�����ł���Ȃ猩�o���ł͂Ȃ�
		if( !( strncmp( pcCaret, "�i", 2 ) ) )	return 0;
		if( !( strncmp( pcCaret, "�@ ", 3 ) ) )	return 0;	//	�S�p�󔒁{���p��
		if( !( strncmp( pcCaret, "�@�@", 4 ) ) )	return 0;	//	�S�p��2��
		if( !( strncmp( pcCaret, "�ŏI�X", 4 ) ) )	return 0;
		if( !( strncmp( pcCaret, "Last Mod", 8 ) ) )	return 0;

		//	���o���ƌ��Ȃ��ē��e���Q�b�g����B
		cbSize = pstItem->cbItem;	//	��s�̏ꍇ

		pcEnd = NextLineA( pcCaret );
		//NULL�Ȃ�A���s������s�ł���
		if( *pcEnd )
		{
			cbSize = pcEnd - pcCaret;	//	���s������

			pcOpen = pcEnd;
			pcEnd  = NextLineA( pcOpen );	//	���s�m�F
			//	�����ŁA�R�s�ڈȍ~������Ό��o���Ƃ͌��Ȃ��Ȃ�
			if( *pcEnd )	return 0;
		}

		//	�����ŁAcbSize�͈�s�̒����ɂȂ�

		//	�����A�s�����ɒ���������A���o���Ƃ͌��Ȃ��Ȃ�
		if( 42 <= cbSize )	return 0;

		pcLine = (LPSTR)malloc( cbSize );
		ZeroMemory( pcLine, cbSize );
		CopyMemory( pcLine, pcCaret, cbSize-2 );	//	���s���J�b�g

		AaTitleAddString( pstItem->ixNum, pcLine );

		FREE(pcLine);
	}

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�m�ۂ��Ă�A�C�e������Ԃ�
*/
INT_PTR AacItemCount( UINT reserve )
{
	return gvcArts.size();
}
//-------------------------------------------------------------------------------------------------

/*!
	�ʂ��ԍ����󂯂āAHBITMAP�ƃT�C�Y��Ԃ�
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	iNumber	�ʂ��ԍ��O�C���f�b�N�X
	@param[out]	pstSize	�傫��
	@param[out]	pstArea	�h�b�g�����C��
	@return	HBITMAP	AA�̓��e�̃r�b�g�}�b�v��Ԃ��B�߂ɂ���Ȃ炻�̂܂܁A�A�C�e���Ȃ��Ȃ�NULL
*/
HBITMAP AacArtImageGet( HWND hWnd, INT iNumber, LPSIZE pstSize, LPSIZE pstArea )
{
	INT_PTR		iItems, i;
	MAAM_ITR	itArts;

	pstSize->cx = 0;
	pstSize->cy = 0;

	pstArea->cx = 0;
	pstArea->cy = 0;

	iItems = gvcArts.size( );
	if( iItems <= iNumber ){	return NULL;	}	//	�͂ݏo���̏ꍇ

	itArts = gvcArts.begin();
	for( i = 0; iNumber >  i; i++ ){	itArts++;	}


	//	�m�ۍς̏ꍇ	hThumbDC
	if( itArts->hThumbBmp )
	{
		pstSize->cx = itArts->stSize.cx;
		pstSize->cy = itArts->stSize.cy;

		pstArea->cx = itArts->iMaxDot;
		pstArea->cy = itArts->iLines;

		return	itArts->hThumbBmp;
	}


	//	���m�ۂ̏ꍇ
	DraughtAaImageing( hWnd, &(*itArts) );

	pstSize->cx = itArts->stSize.cx;
	pstSize->cy = itArts->stSize.cy;

	pstArea->cx = itArts->iMaxDot;
	pstArea->cy = itArts->iLines;

	return itArts->hThumbBmp;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ʂ��ԍ����󂯂āA���e��Ԃ�
	@param[in]	iNumber	�ʂ��ԍ��O�C���f�b�N�X
	@return	LPSTR	AA�̓��e���A�������m�ۂ��ēn���B�󂯎�������ŊJ������
*/
LPSTR AacAsciiArtGet( DWORD iNumber )
{
	size_t	items;
	LPSTR	pcBuff;
//	MAAM_ITR	itMaam;

	items = gvcArts.size( );
	if( items <= iNumber )	return NULL;

	pcBuff = (LPSTR)malloc( gvcArts.at( iNumber ).cbItem + 1 );
	ZeroMemory( pcBuff, gvcArts.at( iNumber ).cbItem + 1 );
	CopyMemory( pcBuff, gvcArts.at( iNumber ).pcItem, gvcArts.at( iNumber ).cbItem );

	return pcBuff;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ʂ��ԍ����󂯂āA���e�̍ő�h�b�g���ƍs���ƃo�C�g����߂�
	@param[in]	iNumber	�ʂ��ԍ��O�C���f�b�N�X
	@param[out]	piLine	�s���Ԃ�
	@param[out]	pBytes	�o�C�g���Ԃ��iEditor�̂݁j
	@return		�ő�h�b�g��
*/
INT AacArtSizeGet( DWORD iNumber, PINT piLine, PINT pBytes )
{
	INT	iDot, cx, cy, iByte = 0;
	size_t	items;
	LPSTR	pcConts;
	LPTSTR	ptString;

	items = gvcArts.size( );
	if( items <=  iNumber ){	return 0;	}

	if( 0 >= gvcArts.at( iNumber ).stSize.cx || 0 >= gvcArts.at( iNumber ).stSize.cy )
	{
		pcConts = (LPSTR)malloc( gvcArts.at( iNumber ).cbItem + 1 );
		if( pcConts )
		{
			TRACE( TEXT("AA Size Calculate[%d]"), iNumber );

			ZeroMemory( pcConts, gvcArts.at( iNumber ).cbItem + 1 );
			CopyMemory( pcConts, gvcArts.at( iNumber ).pcItem, gvcArts.at( iNumber ).cbItem );

			ptString = SjisDecodeAlloc( pcConts );
			FREE( pcConts );
			if( ptString )
			{
				cx = TextViewSizeGet( ptString, &cy );
#ifndef _ORRVW
				DocRawDataParamGet( ptString, NULL, &iByte );
#endif
				FREE( ptString );

				gvcArts.at( iNumber ).stSize.cx = cx;
				gvcArts.at( iNumber ).stSize.cy = cy;
				gvcArts.at( iNumber ).iByteSize = iByte;
			}
		}
	}

	iDot    = gvcArts.at( iNumber ).stSize.cx;
	if( piLine ){	*piLine = gvcArts.at( iNumber ).stSize.cy;	}
	if( pBytes ){	*pBytes = gvcArts.at( iNumber ).iByteSize;	}

	return iDot;
}
//-------------------------------------------------------------------------------------------------

/*!
	MLT�̕ێ����e��S�j��
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT AacMatrixClear( VOID )
{
	MAAM_ITR	itArts;

	//	��ɗ̈���J��
	for( itArts = gvcArts.begin(); itArts != gvcArts.end(); itArts++ )
	{
		FREE( itArts->pcItem );

		if( itArts->hThumbBmp )
		{
	//		SelectBitmap( itArts->hThumbDC, itArts->hOldBmp );
			DeleteBitmap( itArts->hThumbBmp );
	//		DeleteDC( itArts->hThumbDC );
		}
	}

	gvcArts.clear();	//	�����đS�j��

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g�p�̋敪�f�B���N�g�������󂯎���āA�r�p�k���璆�g���m�ۂ���
	@param[in]	hWnd		�e�E�C���h�E�n���h���ENULL�Ȃ�j��
	@param[in]	ptBlockName	�敪�f�B���N�g����
	@return		DWORD		�m�ۂ�����
*/
DWORD AacAssembleSql( HWND hWnd, LPCTSTR ptBlockName )
{
	DWORD	ixItems;

	ixItems = SqlFavCount( ptBlockName, NULL );

	AacMatrixClear(   );	//	�����̓��e�S�j��

	if( 0 == ixItems )	return 0;

	//	�R�[���o�b�N���Ē��g���x�N�^�[�Ɋm��
	SqlFavArtEnum( ptBlockName, AacFavInflate );

	return ixItems;
}
//-------------------------------------------------------------------------------------------------

/*!
	�R�[���o�b�N�ł��C�ɓ��胊�X�g�̒��g���Q�b�c
	@param[in]	dLength	�o�C�g��
	@param[in]	dummy	���g�p
	@param[in]	fake	���g�p
	@param[in]	pcConts	AA�{��
	@return		UINT	�m�ۂ�����
*/
LRESULT CALLBACK AacFavInflate( UINT dLength, UINT dummy, UINT fake, LPCVOID pcConts )
{
	UINT_PTR	iNumber;	//	�ʂ��ԍ�
	AAMATRIX	stAAbuf;	//	���AA�̕ێ��E�x�N�^�[�ɓ����

	iNumber = gvcArts.size( );	//	�����o�Ă���̂ŁA�O�C���f�b�N�X�ɂȂ�

	TRACE( TEXT("NUM[%d] byte[%d]"), iNumber, dLength );

	ZeroMemory( &stAAbuf, sizeof(AAMATRIX) );

	stAAbuf.ixNum = iNumber;

	stAAbuf.cbItem = dLength;	//	�o�C�g���Ȃ̂ł���ł����͂�
	stAAbuf.pcItem = (LPSTR)malloc( stAAbuf.cbItem + 2 );
	ZeroMemory( stAAbuf.pcItem, stAAbuf.cbItem + 2 );

	CopyMemory( stAAbuf.pcItem, pcConts, stAAbuf.cbItem );

	gvcArts.push_back( stAAbuf );	//	�x�N�^�[�ɒǉ�

	return 0;
}
//-------------------------------------------------------------------------------------------------



#ifdef MAA_IADD_PLUS

#ifndef _ORRVW


typedef struct tagITEMADDINFO
{
	LPTSTR	ptContent;			//!<	�{�����e
	TCHAR	atSep[MAX_STRING];	//!<	�Z�p���[�^���e
	INT		bType;				//!<	��OMLT�@�OAST

} ITEMADDINFO, *LPITEMADDINFO;
//--------------------------------------


/*!
	�A�C�e���ǉ��̖ʓ|����_�C�����O�[
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	message		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam		�ǉ��̏��P
	@param[in]	lParam		�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK AaItemAddDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	static LPITEMADDINFO	pstIaInfo;
	static LPTSTR	ptBuffer;
	UINT_PTR	cchSize;
	TCHAR	atName[MAX_PATH];
	INT		id;
	RECT	rect;

	switch( message )
	{
		case WM_INITDIALOG:
			pstIaInfo = (LPITEMADDINFO)(lParam);
			GetClientRect( hDlg, &rect );
			CreateWindowEx( 0, WC_BUTTON, TEXT("���̕�"),         WS_CHILD | WS_VISIBLE, 0, 0, 75, 23, hDlg, (HMENU)IDB_MAID_NOWPAGE, GetModuleHandle(NULL), NULL );
			CreateWindowEx( 0, WC_BUTTON, TEXT("�N���b�v�{�[�h"), WS_CHILD | WS_VISIBLE, 75, 0, 120, 23, hDlg, (HMENU)IDB_MAID_CLIPBOARD, GetModuleHandle(NULL), NULL ); 
			CreateWindowEx( 0, WC_EDIT,   TEXT(""),               WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 195, 0, rect.right-195-50, 23, hDlg, (HMENU)IDE_MAID_ITEMNAME, GetModuleHandle(NULL), NULL );
			CreateWindowEx( 0, WC_BUTTON, TEXT("�ǉ�"),           WS_CHILD | WS_VISIBLE, rect.right-50, 0, 50, 23, hDlg, (HMENU)IDB_MAID_ADDGO, GetModuleHandle(NULL), NULL );
			CreateWindowEx( 0, WC_EDIT,   TEXT(""),               WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_READONLY, 0, 23, rect.right, rect.bottom-23, hDlg, (HMENU)IDE_MAID_CONTENTS, GetModuleHandle(NULL), NULL );
			//	�Ȃ�ł킴�킴����������̂��v���o���Ȃ��B

			if( pstIaInfo->bType )
			{
				SetDlgItemText( hDlg, IDE_MAID_ITEMNAME, TEXT("���̂�AST�łȂ��Ǝg�p�ł��Ȃ���") );
				EnableWindow( GetDlgItem(hDlg,IDE_MAID_ITEMNAME), FALSE );
				StringCchCopy( pstIaInfo->atSep, MAX_STRING, TEXT("[SPLIT]\r\n") );
			}

			//	�Ƃ肠�����N���b�v�{�[�h�̒��g���Ƃ�
			ptBuffer = DocClipboardDataGet( NULL );
			if( !(ptBuffer) ){	DocPageGetAlloc( D_UNI , (LPVOID *)(&ptBuffer) );	}
			//	�g���Ȃ��V�����m�Ȃ�A���̕ł̓��e�������Ă��ĕ\��
			SetDlgItemText( hDlg, IDE_MAID_CONTENTS, ptBuffer );
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			id = LOWORD(wParam);
			switch( id )
			{
				case IDCANCEL:
					FREE(ptBuffer);
					EndDialog(hDlg, 0 );
					return (INT_PTR)TRUE;

				case IDB_MAID_ADDGO:	//	�ǉ����s
					if( ptBuffer )
					{
						StringCchLength( ptBuffer, STRSAFE_MAX_CCH, &cchSize );
						cchSize += 4;
						pstIaInfo->ptContent = (LPTSTR)malloc( cchSize * sizeof(TCHAR) );
						StringCchCopy( pstIaInfo->ptContent, cchSize, ptBuffer );
						//	�X�v���b�^�p�̉��s
						StringCchCat( pstIaInfo->ptContent, cchSize, CH_CRLFW );

						if( !(pstIaInfo->bType) )	//	AST�Ȃ�^�C�g������
						{
							GetDlgItemText( hDlg, IDE_MAID_ITEMNAME, atName, MAX_PATH );
					//		StringCchPrintf( pstIaInfo->atSep, MAX_STRING, TEXT("[AA][%s]\r\n"), atName );	//	���̂܂܂ł̓^�O�����d�ɕۑ�����Ă܂�
							StringCchCopy( pstIaInfo->atSep, MAX_STRING, atName );
						}
					}
					FREE(ptBuffer);
					EndDialog(hDlg, 1 );
					return (INT_PTR)TRUE;

				case IDB_MAID_CLIPBOARD:
					FREE(ptBuffer);
					ptBuffer = DocClipboardDataGet( NULL );
					SetDlgItemText( hDlg, IDE_MAID_CONTENTS, ptBuffer );
					return (INT_PTR)TRUE;

				case IDB_MAID_NOWPAGE:
					FREE(ptBuffer);
					DocPageGetAlloc( D_UNI , (LPVOID *)(&ptBuffer) );
					SetDlgItemText( hDlg, IDE_MAID_CONTENTS, ptBuffer );
					return (INT_PTR)TRUE;

				default:	break;
			}
			break;

		case WM_SIZE:
			GetClientRect( hDlg, &rect );
			MoveWindow( GetDlgItem(hDlg,IDE_MAID_ITEMNAME), 195, 0, rect.right-195-50, 23, TRUE );
			MoveWindow( GetDlgItem(hDlg,IDB_MAID_ADDGO),    rect.right-50, 0, 50, 23, TRUE );
			MoveWindow( GetDlgItem(hDlg,IDE_MAID_CONTENTS), 0, 23, rect.right, rect.bottom-23, TRUE );
			break;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ʂ��ԍ����󂯂āA������폜
	@param[in]	hWnd	�e�E�C���h�E�n���h��
	@param[in]	iNumber	�ʂ��ԍ��O�C���f�b�N�X�E�}�C�i�X�Ȃ牽�����Ȃ�
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT AacItemDelete( HWND hWnd, LONG iNumber )
{
	UINT		curSel;
	MAAM_ITR	itMaaItem;

	if( 0 > iNumber )	return E_OUTOFMEMORY;

	itMaaItem = gvcArts.begin();
	std::advance( itMaaItem , iNumber );	//	�ڕW�܂Ői�߂�

	//�f���[�g�m�F������

	gvcArts.erase( itMaaItem );	//	�f���[�g����

	AacItemBackUpCreate( NULL );	//	�o�b�N�A�b�v���Ă���

	AacItemOutput( hWnd );	//	�����Ńt�@�C�����o�͍X�V

	//	�ǉ����������烊���[�h����
	curSel = TabMultipleNowSel(  );
	AaItemsDoShow( hWnd, gatOpenFile, curSel );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ʂ��ԍ����󂯂āA�����̎�O�ɃA�C�e���ǉ�
	@param[in]	hWnd	�e�E�C���h�E�n���h��
	@param[in]	iNumber	�ʂ��ԍ��O�C���f�b�N�X�E�}�C�i�X�Ȃ疖���ɒǉ��Ƃ���
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT AacItemInsert( HWND hWnd, LONG iNumber )
{
	UINT_PTR	cbSize;
	LPSTR		pcName;
	INT			curSel;
	AAMATRIX	stAAbuf;	//	���AA�̕ێ��E�x�N�^�[�ɓ����
	ITEMADDINFO	stIaInfo;

	ZeroMemory( &stAAbuf, sizeof(AAMATRIX) );
	ZeroMemory( &stIaInfo, sizeof(ITEMADDINFO) );

	//	�g���q�m�F
	if( FileExtensionCheck( gatOpenFile, TEXT(".ast") ) ){	stIaInfo.bType =  0;	}
	else{	stIaInfo.bType =  1;	}

	if( DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MAA_IADD_DLG), hWnd, AaItemAddDlgProc, (LPARAM)(&stIaInfo) ) )
	{
		if( stIaInfo.ptContent )	//	���g���L���Ȃ珈������
		{
			//	���O�m��
			pcName = SjisEncodeAlloc( stIaInfo.atSep  );	//	�ϊ�
			StringCchLengthA( pcName, STRSAFE_MAX_CCH, &cbSize );
			StringCchCopyA( stAAbuf.acAstName, MAX_STRING, pcName );
			FREE( pcName );
			stAAbuf.cbItem = cbSize;
			//	�{�̊m��
			stAAbuf.pcItem = SjisEncodeAlloc( stIaInfo.ptContent  );

			if( 0 <= iNumber )	gvcArts.insert( gvcArts.begin() + iNumber, stAAbuf );
			else				gvcArts.push_back( stAAbuf );
		//	������ stAAbuf.pcItem ���t���[���Ă͂����Ȃ��Evector�Ŏg���Ă���

			FREE( stIaInfo.ptContent );

			AacItemBackUpCreate( NULL );	//	�o�b�N�A�b�v���Ă���

			AacItemOutput( hWnd );	//	�����Ńt�@�C�����o�͍X�V

			//�ǉ����������烊���[�h����E�R���{�b�N�X�N�����[�Ƃ�����
		//	AaTitleClear(  );	//	AaItemsDoShow�̒��ł���Ă邩�炱���ɂ͕s�v
			curSel = TabMultipleNowSel(  );
			AaItemsDoShow( hWnd, gatOpenFile, curSel );
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ύX���ʂ�ۑ�����O�ɁA�o�b�N�A�b�v�Ƃ��Ă���
*/
UINT AacItemBackUpCreate( LPVOID pVoid )
{
	TCHAR	atOutFile[MAX_PATH], atFileName[MAX_PATH];

	//	�t�@�C�����m��
	StringCchCopy( atFileName, MAX_PATH, PathFindFileName( gatOpenFile ) );
	//	�K���Ɋg���q�t���Ă���
	StringCchCat( atFileName, MAX_PATH, TEXT(".abk") );
	//	�p�X�����
	StringCchCopy( atOutFile, MAX_PATH, gatBkUpDir );
	PathAppend( atOutFile, atFileName );	//	Backup�t�@�C����

	//	�t�@�C�����R�s�[����
	CopyFile( gatOpenFile, atOutFile, FALSE );	//	�����̃t�@�C���͏㏑������

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�X�V�������e���A�E�g�v�b�g����
*/
HRESULT AacItemOutput( HWND hWnd )
{
	BOOLEAN	isAST;
	CHAR	acSep[MAX_STRING];

	HANDLE	hFile;
	DWORD	wrote;
	UINT_PTR	cbSize;

	INT_PTR	i, iPage;

	MAAM_ITR	itAamx;


	//	�g���q�m�F
	if( FileExtensionCheck( gatOpenFile, TEXT(".ast") ) ){	isAST = TRUE;	}
	else{	isAST = FALSE;	}

	hFile = CreateFile( gatOpenFile, GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE == hFile )
	{
		MessageBox( hWnd, TEXT("�t�@�C�����J���Ȃ�������"), TEXT("���ӂ���̂��m�点"), MB_OK | MB_ICONERROR );
		return E_HANDLE;
	}

	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );

	iPage = gvcArts.size();
	//	�S���o�͂��Ă���
	for( i = 0, itAamx = gvcArts.begin(); gvcArts.end() != itAamx; itAamx++, i++ )
	{
		//	��؂���o��	�]�v�ȉ��s���{���ɂ������̂��C��
		ZeroMemory( acSep, sizeof(acSep) );	cbSize = 0;
		if( isAST )	//	AST�t�@�C���ł���
		{
			StringCchPrintfA( acSep , MAX_STRING, ("[AA][%s]%s"), itAamx->acAstName, CH_CRLFA );
		}
		else	//	MLT�ł���BTXT��������Ȃ��B
		{
			if( 1 <= i )	//	�t�@�C���擪�ɂ͕s�v
			{
				StringCchPrintfA( acSep, MAX_STRING, ("%s%s"), MLT_SEPARATERA, CH_CRLFA );
			}
		}
		StringCchLengthA( acSep , MAX_STRING, &cbSize );	//	�����m�F�AMLT�̏ꍇ����
		if( cbSize ){	WriteFile( hFile, acSep, cbSize, &wrote, NULL );	}

		//	�{���o��
		StringCchLengthA( itAamx->pcItem, STRSAFE_MAX_CCH, &cbSize );
		WriteFile( hFile, itAamx->pcItem, cbSize, &wrote, NULL );
	}

	SetEndOfFile( hFile );
	CloseHandle( hFile );


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

#endif

#endif	//	MAA_IADD_PLUS
