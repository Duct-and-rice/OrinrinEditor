/*! @file
	@brief ���E���]�A�㉺���]���܂�
	���̃t�@�C���� DocInverseCtrl.cpp �ł��B
	@author	SikigamiHNQ
	@date	2012/04/11
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

//	���]�p�p�[�c�́A�K�v�ɂȂ����烍�[�h����悤�ɂ���



#define IV_MIRROR	1
#define IV_UPSET	0


typedef struct tagINVERSEPARTS
{
	TCHAR	atSrcStr[MIN_STRING];	//!<	��������
	TCHAR	atDestStr[MIN_STRING];	//!<	�ϊ��㕶����

} INVERSEPARTS, *LPINVERSEPARTS;
//-------------------------------------------------------------------------------------------------

extern list<ONEFILE>	gltMultiFiles;	//	�����t�@�C���ێ�
extern FILES_ITR	gitFileIt;		//	�����Ă�t�@�C���̖{��
extern INT			gixFocusPage;	//	���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X

static  vector<INVERSEPARTS>	gvcMirrorParts;	//!<	���E���]�p���փp�[�c
static  vector<INVERSEPARTS>	gvcUpsetParts;	//!<	�㉺���]�p���փp�[�c
typedef vector<INVERSEPARTS>::iterator	PARTS_ITR;
//-------------------------------------------------------------------------------------------------


HRESULT	DocMirrorTranceLine( INT, INT );
HRESULT	DocMirrorTranceBox( INT, INT );

HRESULT	DocUpsetTranceLine( INT, INT );
HRESULT	DocUpsetTranceBox( INT, INT );

LPTSTR	SeledTextAlloc( LINE_ITR, PINT, PINT );

HRESULT	InversePartsLoad( UINT );
UINT	InversePartsCheck( UINT, LPCTSTR, LPTSTR, UINT_PTR );
//-------------------------------------------------------------------------------------------------

/*!
	�������Ɣj��
	@param[in]	dMode	��O�������@�O�j��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocInverseInit( UINT dMode )
{

	if( dMode )
	{


	}
	else
	{
		gvcMirrorParts.clear( );
		gvcUpsetParts.clear(  );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

#define INV_ITEMS	3
/*!
	���]�p�[�c�����[�h
	@param[in]	dMode	��O���E�@�O�㉺
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT InversePartsLoad( UINT dMode )
{
	CONST WCHAR rtHead = 0xFEFF;	//	���j�R�[�h�e�L�X�g�w�b�_
	WCHAR	rtUniBuf;

	HANDLE	hFile;
	DWORD	readed;

	LPVOID	pBuffer;	//	������o�b�t�@�p�|�C���^�[
	INT		iByteSize;

	LPTSTR	ptString;
	LPSTR	pcText;
	UINT	cchSize, cchLen;
	TCHAR	atFileName[MAX_PATH];

	TCHAR	atBuff[INV_ITEMS][MIN_STRING];
	INT		nYct, nXct;
	UINT	caret, dItem;
	//BOOLEAN	bEmpty = FALSE;


	INVERSEPARTS	stData;

	UINT_PTR	loop;
	list<INVERSEPARTS>	ltParts;	//	�p�[�c�Ǎ��o�b�t�@
	list<INVERSEPARTS>::iterator	itParts, itPtPos;	//	�o�b�t�@�̃C�e���[�^


	StringCchCopy( atFileName, MAX_PATH, ExePathGet( ) );
	PathAppend( atFileName, TEMPLATE_DIR );

	if( dMode  )	//	���E
	{
		if( 1 <= gvcMirrorParts.size( ) )	return S_FALSE;

		PathAppend( atFileName, AA_MIRROR_FILE );
	}
	else	//	�㉺
	{
		if( 1 <= gvcUpsetParts.size(  ) )	return S_FALSE;

		PathAppend( atFileName, AA_UPSET_FILE );
	}

	hFile = CreateFile( atFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE == hFile ){	return E_HANDLE;	}

	iByteSize = GetFileSize( hFile, NULL );
	pBuffer = malloc( iByteSize + 2 );
	ZeroMemory( pBuffer, iByteSize + 2 );

	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
	ReadFile( hFile, pBuffer, iByteSize, &readed, NULL );
	CloseHandle( hFile );	//	���e�S����荞�񂾂���J��

	//	���j�R�[�h�`�F�b�N
	CopyMemory( &rtUniBuf, pBuffer, 2 );
	if( rtHead == rtUniBuf )	//	���j�R�[�h�w�b�_�������
	{	//	���ʂ̓��j�R�[�h����Ȃ�
		ptString = (LPTSTR)pBuffer;
		ptString++;	//	���j�R�[�h�w�b�_���i�߂Ă���
	}
	else	//	����SJIS�ł���Ȃ�
	{
		pcText = (LPSTR)pBuffer;
		ptString = SjisDecodeAlloc( pcText );	//	SJIS�̓��e�����j�R�[�h�ɂ���

		FREE( pBuffer );	//	�������ŊJ��
		pBuffer = ptString;	//	�|�C���g����Ƃ����ύX
	}

	StringCchLength( ptString, STRSAFE_MAX_CCH, &cchSize );

	//	�捞
	ZeroMemory( atBuff, sizeof(atBuff) );

	//�X�^�C���b�V���ɖ{�̓Ǎ�
	dItem = 0;
	for( nYct = 0, nXct = 0, caret = 0; caret <= cchSize; caret++, nXct++ )
	{
		if( MIN_STRING <= nXct ){	nXct = MIN_STRING - 1;	}
		if( INV_ITEMS  <= nYct ){	nYct = INV_ITEMS - 1;	}
 
		atBuff[nYct][nXct] = ptString[caret];

		//	���s�܂ł��P�Z�b�g�Ƃ��A�S�p�󔒂ŋ�؂��ēǂ�
		if( (TEXT('\r') == ptString[caret] && TEXT('\n') == ptString[caret + 1]) || 0x0000 == ptString[caret] )
		{
			//bEmpty = FALSE;
			//if(0 == nXct && 0 == nYct ){	bEmpty = TRUE;	}	//	���X�g���s����Ȃ炱���ɓ���E�����񖳂��Ȃ牽�����Ȃ�

			atBuff[nYct][nXct] = 0x0000;	//	������̏I�_�̓k��
			nXct = -1;	//	���[�v���Ł{�{����邽�߁A�O�ɂ����悤�ɂ��Ă���
			nYct = 0;	//	���̃G�������g�ɃX�^�C���b�V���Ɉڂ�
			caret++;	//	���s�R�[�h�����G���K���g�ɐi�߂�

			//if( (0x0000 != atBuff[0][0]) && (TEXT('\r') != atBuff[0][0]) && !(bEmpty) )	//	���e������̂Ȃ�
			if( (0x0000 != atBuff[0][0]) && (0 != dItem) )
			{
				//	��s�ڂ̓w�b�_�ł���

				ZeroMemory( &stData, sizeof(INVERSEPARTS) );	//	�N�����i�b�v
				StringCchCopy( stData.atSrcStr,  MIN_STRING, atBuff[0] );
				StringCchCopy( stData.atDestStr, MIN_STRING, atBuff[1] );
				ltParts.push_back( stData );	//	�܂��o�b�t�@��

				//	���Ό������K�v
				ZeroMemory( &stData, sizeof(INVERSEPARTS) );
				StringCchCopy( stData.atSrcStr,  MIN_STRING, atBuff[1] );
				StringCchCopy( stData.atDestStr, MIN_STRING, atBuff[0] );
				ltParts.push_back( stData );	//	�܂��o�b�t�@��

			}//���e���L�邩�ǂ���

			dItem++;
			ZeroMemory( atBuff , sizeof(atBuff) );	//	�N�����i�b�v
		}

		if( TEXT('�@') == ptString[caret] )
		{
			atBuff[nYct][nXct] = 0x0000;	//	������̏I�_�̓k��
			nXct = -1;	//	���[�v���Ł{�{����邽�߁A�O�ɂ����悤�ɂ��Ă���
			nYct++;		//	����Parameter�̂��߂�
		}
	}

	FREE( pBuffer );

	//	�������̏��Ԃɕ��ג����ׂ�
	cchSize = 0;

	loop = ltParts.size();
	while( loop )	//	�S�̂��݂Ȃ�����
	{
		itParts = ltParts.begin();	//	�Ƃ肠�����P��
		StringCchLength( itParts->atSrcStr, MIN_STRING , &cchSize );	//	�������m�F

		for( itPtPos = ltParts.begin(); ltParts.end() != itPtPos; itPtPos++ )
		{
			StringCchLength( itPtPos->atSrcStr, MIN_STRING, &cchLen );
			if( cchSize <  cchLen ){	itParts = itPtPos;	}	//	����������������ύX
		}

		ZeroMemory( &stData, sizeof(INVERSEPARTS) );	//	�N�����i�b�v
		StringCchCopy( stData.atSrcStr,  MIN_STRING, itParts->atSrcStr );
		StringCchCopy( stData.atDestStr, MIN_STRING, itParts->atDestStr );
		if( dMode ){	gvcMirrorParts.push_back( stData );	}	//	���E
		else{			gvcUpsetParts.push_back( stData );	}	//	�㉺

		ltParts.erase( itParts );	//	�L�^�����炻��͏���

		loop = ltParts.size();	//	�c�肪���邩
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��s�̑I��͈͂��e�L�X�g�Ŋm�ۂ���Efree�͌Ă񂾕��ł��
	@param[in]	itLine	�w��s�̃C�e���[�^
	@param[out]	*piDot	�I��͈͊J�n�ʒu�h�b�g��Ԃ�
	@param[out]	*piMozi	�I��͈͂̃��j�R�[�h��������Ԃ�
	@return				�m�ۂ������j�R�[�h������
*/
LPTSTR SeledTextAlloc( LINE_ITR itLine, PINT piDot, PINT piMozi )
{
	UINT_PTR	j, dLetters;
	INT			iDot, iMozi;
	INT			iSelDot;
	LPTSTR		ptString = NULL;

	UINT_PTR	cchSz;

	wstring	wsSrcBuff;

	//	���̍s�̑I��͈͂��m�ۂ���
	dLetters = itLine->vcLine.size( );

	wsSrcBuff.clear();
	iSelDot = -1;	//	�I���J�n�h�b�g���L�^
	iDot = 0;
	iMozi = 0;

	for( j = 0; dLetters > j; j++ )
	{
		//	�I������Ă��镔���𕶎���Ɋm��
		if( CT_SELECT & itLine->vcLine.at( j ).mzStyle )
		{
			wsSrcBuff += itLine->vcLine.at( j ).cchMozi;

			iMozi++;
			if( 0 > iSelDot )	iSelDot = iDot;
		}

		iDot +=  itLine->vcLine.at( j ).rdWidth;	//	�����܂ł̃h�b�g���m�F
	}

	cchSz = wsSrcBuff.size();
	ptString = (LPTSTR)malloc( (cchSz+2) * sizeof(TCHAR) );
	if( ptString )	StringCchCopy( ptString, (cchSz+2), wsSrcBuff.c_str() );

	if( 0 > iSelDot )	iSelDot = 0;

	if( piDot  )	*piDot  = iSelDot;
	if( piMozi )	*piMozi = iMozi;


	return ptString;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I��͈͂̂`�`�𔽓]����
	@param[in]	dStyle	��`���ǂ���
	@param[in]	dMode	��O���E�@�O�㉺
	@param[in]	pXdot	���̃h�b�g�ʒu���󂯂Ė߂�
	@param[in]	dLine	���̍s��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocInverseTransform( UINT dStyle, UINT dMode, PINT pXdot, INT dLine )
{
	INT_PTR	iLines;
	INT		iTop, iBtm, iInX;

#ifdef DO_TRY_CATCH
	try{
#endif

	iLines = DocNowFilePageLineCount( );	//	�y�[�W�S�̂̍s��

	//	�J�n�n�_����J�n	//	D_SQUARE
	iTop = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop;
	iBtm = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom;
	if( 0 >  iTop ){	iTop = 0;	}
	if( 0 >  iBtm ){	iBtm = iLines - 1;	}

	//	���[���m�F�E���e���Ȃ��Ȃ�A�g�p�s�߂�
	iInX = DocLineParamGet( iBtm, NULL, NULL );
	if( 0 == iInX ){	 iBtm--;	}

	//	�͂ݏo���Ȃ���
	if( iLines <= iTop || iLines <= iBtm )	return E_OUTOFMEMORY;

	//	��`�Ȃ�A�e�s���ɔ��]�����E�S�̂Ȃ�A�l�`�w���ɍ��킹��
	if( dStyle & D_SQUARE )
	{
		if( dMode ){	DocMirrorTranceBox( iTop, iBtm );	}	//	���E
		else{			DocUpsetTranceBox( iTop, iBtm );	}	//	�㉺
	}
	else
	{
		if( dMode ){	DocMirrorTranceLine( iTop, iBtm );	}	//	���E
		else{			DocUpsetTranceLine( iTop, iBtm );	}	//	�㉺
	}
	ViewSelPageAll( -1 );

	DocLetterPosGetAdjust( pXdot, dLine, 0 );	//	�J�[�\���ʒu��K���ɕ␳
	ViewDrawCaret( *pXdot, dLine, 1 );


#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (HRESULT)ETC_MSG( err.what(), E_UNEXPECTED );	}
	catch( ... ){	return (HRESULT)ETC_MSG( ("etc error") , E_UNEXPECTED );	}
#endif

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ʏ�I���̂Ƃ��̍��E���]����
	@param[in]	iTop	�J�n�s
	@param[in]	iBtm	�I���s
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocMirrorTranceLine( INT iTop, INT iBtm )
{
	INT_PTR		iLns;
	INT			iPadd, baseDot, iBytes;
	INT			iDot, iGyou, iMzDot;
	LPTSTR		ptPadd;
	LPTSTR		ptInvStr;
	LPTSTR		ptString = NULL;

	UINT_PTR	cchSz;
	UINT		d;
	TCHAR		atBuff[MIN_STRING];

	BOOLEAN		bFirst = TRUE;

	wstring	wsInvBuff;

	LINE_ITR	itLine;


#ifdef DO_TRY_CATCH
	try{
#endif

	InversePartsLoad( IV_MIRROR );	//	���E�̃p�[�c�m�F

	//	�I��͈͓��ł����Ƃ������h�b�g�����m�F
	baseDot = DocPageMaxDotGet( iTop, iBtm );

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, iTop );

	//	���E���]�Ȃ�A��s�����Ă����΂���
	for( iLns = iTop; iBtm >= iLns; iLns++ )
	{
		wsInvBuff.clear(  );

		iMzDot = DocLineParamGet( iLns, NULL, NULL );
		if( 0 >= iMzDot )	continue;	//	���̍s�̓��e���Ȃ��Ȃ牽�����Ȃ��ł���

		iPadd = baseDot - iMzDot;	//	���߂ɕK�v�ȕ��m��
		ptPadd = DocPaddingSpaceMake( iPadd );	//	�����܂ł𖄂߂�

		//	�Y���s���m�ۂ���
		iBytes = DocLineTextGetAlloc( gitFileIt, gixFocusPage, D_UNI, iLns, (LPVOID *)(&ptString) );
		if( 0 < iBytes )
		{
			StringCchLength( ptString, STRSAFE_MAX_CCH, &cchSz );
			for( d = 0; cchSz > d; )	//	���ԂɌ��Ă���
			{
				d += InversePartsCheck( IV_MIRROR, &(ptString[d]), atBuff, MIN_STRING );
				wsInvBuff.insert( 0, atBuff );
			}
		}
		FREE( ptString );

		//	��������Ɠ���ւ�
		cchSz = wsInvBuff.size() + 2;	//	�g���񂵒���
		ptInvStr = (LPTSTR)malloc( cchSz * sizeof(TCHAR) );
		StringCchCopy( ptInvStr, cchSz, wsInvBuff.c_str() );

		DocLineErase( iLns, &bFirst );	//	�悸���̍s�̓��e���폜����
		iDot = 0;	iGyou = iLns;
		if( ptPadd ){	DocInsertString( &iDot, &iGyou, NULL, ptPadd, 0, bFirst );	bFirst  = FALSE;	}
		DocInsertString( &iDot, &iGyou, NULL, ptInvStr, 0, bFirst );	bFirst  = FALSE;
		//	���ߕ�����������ŁA�Ђ�����Ԃ�����������������߂΂���

		FREE( ptPadd );
		FREE( ptInvStr );
	}
	//���������擪���猩�Ă����B������Ƃ��Ĉ����r���A�q�b�g������
	//����ւ���DEST�̐擪�ɋL�^�B���X�擪�ɓ����΍��E���]�����̂��ł���B
	//�q�b�g���Ȃ������猳���������̂܂܋L�^�B

	//	���[�󔒍폜���K�v

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (HRESULT)ETC_MSG( err.what(), E_UNEXPECTED );	}
	catch( ... ){	return (HRESULT)ETC_MSG( ("etc error") , E_UNEXPECTED );	}
#endif

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	��`�I���̂Ƃ��̍��E���]����
	@param[in]	iTop	�J�n�s
	@param[in]	iBtm	�I���s
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocMirrorTranceBox( INT iTop, INT iBtm )
{
	INT_PTR		iLns;
	INT			iGyou;
	INT			iSelDot, iMozi;
	LPTSTR		ptInvStr;
	LPTSTR		ptString = NULL;

	UINT_PTR	cchSz;
	UINT		d;
	TCHAR		atBuff[MIN_STRING];

	BOOLEAN		bFirst = TRUE;

	wstring	wsInvBuff, wsSrcBuff;

	LINE_ITR	itLine;


#ifdef DO_TRY_CATCH
	try{
#endif

	InversePartsLoad( IV_MIRROR );	//	���E�̃p�[�c�m�F

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, iTop );

	//	���E���]�Ȃ�A��s�����Ă����΂���
	for( iLns = iTop; iBtm >= iLns; iLns++ )
	{
		wsInvBuff.clear(  );

		//	���̍s�̑I��͈͂��m�ۂ���
		ptString = SeledTextAlloc( itLine, &iSelDot, &iMozi );	itLine++;
		StringCchLength( ptString, STRSAFE_MAX_CCH, &cchSz );

		//	���e���t�]������
		if( 0 <  cchSz )	//	���̍s�̒��g����������
		{
			for( d = 0; cchSz > d; )
			{
				d += InversePartsCheck( IV_MIRROR, &(ptString[d]), atBuff, MIN_STRING );
				wsInvBuff.insert( 0, atBuff );
			}

			//	�����e�폜���ăA���h�D�L�^
			DocStringErase( iSelDot, iLns, NULL, iMozi );
			SqnAppendString( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog) , DO_DELETE, ptString, iSelDot, iLns, bFirst );	bFirst = FALSE;
			FREE( ptString );	//	�A���h�D�L�^���Ă���폜�Z��

			cchSz = wsInvBuff.size() + 2;	//	�g���񂵒���
			ptInvStr = (LPTSTR)malloc( cchSz * sizeof(TCHAR) );
			StringCchCopy( ptInvStr, cchSz, wsInvBuff.c_str() );

			//	�Ђ�����Ԃ����������}��
			iGyou = iLns;
			DocInsertString( &iSelDot, &iGyou, NULL, ptInvStr, 0, FALSE );

			FREE( ptInvStr );
		}
	}


#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (HRESULT)ETC_MSG( err.what(), E_UNEXPECTED );	}
	catch( ... ){	return (HRESULT)ETC_MSG( ("etc error") , E_UNEXPECTED );	}
#endif

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ʏ�I���̂Ƃ��̏㉺���]����
	@param[in]	iTop	�J�n�s
	@param[in]	iBtm	�I���s
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocUpsetTranceLine( INT iTop, INT iBtm )
{

	INT_PTR		iLns;
	INT			iBytes;
	INT			iDot, iGyou;
	LPTSTR		ptInvStr;
	LPTSTR		ptString = NULL;

	UINT_PTR	cchSz, d, dL;
	TCHAR		atBuff[MIN_STRING];

	BOOLEAN		bFirst = TRUE;

	LINE_ITR	itLine;

	wstring	wsInvBuff;
	vector<wstring>	vcUpset;	//	�ϊ����ʂ̈ꎞ�ۑ�

#ifdef DO_TRY_CATCH
	try{
#endif

	InversePartsLoad( IV_UPSET );	//	�㉺�̃p�[�c�m�F

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, iTop );	//	�J�n�s�܂ňړ�

	iLns = (iBtm - iTop) + 1;	//	�S�̍s��
	vcUpset.resize( iLns  );	//	��Ɋm�ۂ��Ă���

	//	�ォ�珈�����Ă���
	for( iLns = iTop, dL = 0; iBtm >= iLns; iLns++, dL++ )
	{
		vcUpset.at( dL ).clear( );

		//	�Y���s���m�ۂ���
		iBytes = DocLineTextGetAlloc( gitFileIt, gixFocusPage, D_UNI, iLns, (LPVOID *)(&ptString) );
		if( 0 < iBytes )
		{
			//	�O���珇�Ԃɔ��]�����Ɠ���ւ��Ă���
			StringCchLength( ptString, STRSAFE_MAX_CCH, &cchSz );
			for( d = 0; cchSz > d; )
			{
				d += InversePartsCheck( IV_UPSET, &(ptString[d]), atBuff, MIN_STRING );
				vcUpset.at( dL ) += wstring(atBuff);	//wsInvBuff.push_back( atBuff );
				//	�Ƃ肠�������Ԃɂ���Ă����āA�������ގ���reverse����΂���
			}
		}
		FREE( ptString );
	}

	for( iLns = iTop, dL = vcUpset.size()-1; iBtm >= iLns; iLns++, dL-- )
	{
		DocLineErase( iLns, &bFirst );	//	�悸���̍s�̓��e���폜����

		cchSz = vcUpset.at( dL ).size( );	//	���e������Ώ�������
		if( 0 < cchSz )
		{
			cchSz += 2;
			ptInvStr = (LPTSTR)malloc( cchSz * sizeof(TCHAR) );
			StringCchCopy( ptInvStr, cchSz, vcUpset.at( dL ).c_str( ) );

			iDot = 0;	iGyou = iLns;
			DocInsertString( &iDot, &iGyou, NULL, ptInvStr, 0, bFirst );	bFirst  = FALSE;

			FREE( ptInvStr );
		}
	}

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (HRESULT)ETC_MSG( err.what(), E_UNEXPECTED );	}
	catch( ... ){	return (HRESULT)ETC_MSG( ("etc error") , E_UNEXPECTED );	}
#endif

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	��`�I���̂Ƃ��̏㉺���]����
	@param[in]	iTop	�J�n�s
	@param[in]	iBtm	�I���s
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocUpsetTranceBox( INT iTop, INT iBtm )
{
	INT_PTR		iLns;
	INT			iGyou;
	INT			iSelDot, iMozi;
	LPTSTR		ptInvStr;
	LPTSTR		ptString = NULL;

	UINT_PTR	cchSz, d, dL;
	TCHAR		atBuff[MIN_STRING];

	BOOLEAN		bFirst = TRUE;

	LINE_ITR	itLine, itStart;

	vector<wstring>	vcUpset;	//	�ϊ����ʂ̈ꎞ�ۑ�

#ifdef DO_TRY_CATCH
	try{
#endif
	InversePartsLoad( IV_UPSET );	//	�㉺�̃p�[�c�m�F

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, iTop );	//	�J�n�s�܂ňړ�
	itStart = itLine;

	iLns = (iBtm - iTop) + 1;	//	�S�̍s��
	vcUpset.resize( iLns  );	//	��Ɋm�ۂ��Ă���

	//	�ォ���s�����Ă���
	for( iLns = iTop, dL = 0; iBtm >= iLns; iLns++, dL++ )
	{
		vcUpset.at( dL ).clear( );

		//	���̍s�̑I��͈͂��m�ۂ���
		ptString = SeledTextAlloc( itLine, &iSelDot, &iMozi );	itLine++;
		StringCchLength( ptString, STRSAFE_MAX_CCH, &cchSz );

		//	���e���t�]������
		if( 0 <  cchSz )	//	���̍s�̒��g����������
		{
			//	�O���珇�Ԃɔ��]�����Ɠ���ւ��Ă���
			for( d = 0; cchSz > d; )
			{
				d += InversePartsCheck( IV_UPSET, &(ptString[d]), atBuff, MIN_STRING );
				vcUpset.at( dL ) += wstring(atBuff);	//wsInvBuff.push_back( atBuff );
				//	�Ƃ肠�������Ԃɂ���Ă����āA�������ގ���reverse����΂���
			}
		}
		FREE( ptString );
	}

	//	�ォ�����ւ��Ă���
	itLine = itStart;
	for( iLns = iTop, dL = vcUpset.size()-1; iBtm >= iLns; iLns++, dL-- )
	{
		//	���߂đI��͈͊m��
		ptString = SeledTextAlloc( itLine, &iSelDot, &iMozi );	itLine++;
		if( 0 != iMozi )
		{
			//	�����e�폜���ăA���h�D�L�^
			DocStringErase( iSelDot, iLns, NULL, iMozi );
			SqnAppendString( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog) , DO_DELETE, ptString, iSelDot, iLns, bFirst );	bFirst = FALSE;
			FREE( ptString );	//	�A���h�D�L�^���Ă���폜�Z��
		}

		cchSz = vcUpset.at( dL ).size() + 2;	//	�g���񂵒���
		ptInvStr = (LPTSTR)malloc( cchSz * sizeof(TCHAR) );
		StringCchCopy( ptInvStr, cchSz, vcUpset.at( dL ).c_str() );

		//	��������ɊY������ꏊ���Ȃ�������A���[�ɂ��Ă���
		if( 0 == iMozi ){	iSelDot = DocLineParamGet( iLns, NULL, NULL );	}

		//	�Ђ�����Ԃ����������}��
		iGyou = iLns;
		DocInsertString( &iSelDot, &iGyou, NULL, ptInvStr, 0 , bFirst );	bFirst = FALSE;
	
		FREE( ptInvStr );
	}


#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (HRESULT)ETC_MSG( err.what(), E_UNEXPECTED );	}
	catch( ... ){	return (HRESULT)ETC_MSG( ("etc error") , E_UNEXPECTED );	}
#endif

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	����������󂯂āA�ϊ����ʂ�߂�
	@param[in]	dMode		��O���E�@�O�㉺
	@param[in]	ptSource	��������
	@param[out]	ptOutput	�擪�����̕ϊ����ʂ�߂��o�b�t�@
	@param[in]	cchSz		�o�b�t�@�̕�����
	@return		UINT		��������́A�ϊ������������B�ʏ�P
*/
UINT InversePartsCheck( UINT dMode, LPCTSTR ptSource, LPTSTR ptOutput, UINT_PTR cchSz )
{
	UINT_PTR	dParts;
	UINT_PTR	cchPrt = 1;
	PARTS_ITR	itParts, itBegin, itEnd;

	if( dMode )
	{
		dParts  = gvcMirrorParts.size();
		itBegin = gvcMirrorParts.begin();
		itEnd   = gvcMirrorParts.end();
	}
	else
	{
		dParts  = gvcUpsetParts.size();
		itBegin = gvcUpsetParts.begin();
		itEnd   = gvcUpsetParts.end();
	}

	ZeroMemory( ptOutput, cchSz * sizeof(TCHAR) );

	ptOutput[0] = ptSource[0];	//	�f�t�H���g

	//	�ϊ��e�[�u���������Ȃ炻�̂܂܃R�s���Ė߂��΂���
	if( 0 == dParts ){	return 1;	}

	//	�S�������`�F�b�N
	for( itParts = itBegin; itEnd != itParts; itParts++ )
	{
		StringCchLength( itParts->atSrcStr, MIN_STRING, &cchPrt );
		//	�e�[�u���̕�����Ɣ�r���Ă���
		if( 0 == StrCmpN( ptSource, itParts->atSrcStr, cchPrt ) )
		{
			StringCchCopy( ptOutput, cchSz, itParts->atDestStr );
			break;
		}
		//	�q�b�g������A�ϊ��ꕶ������R�s�[���ďI���
	}

//	StringCchLength( ptOutput, cchSz, &cchPrt );	//	�����m�F�E�����Ⴂ����

	return cchPrt;
}
//-------------------------------------------------------------------------------------------------

