/*! @file
	@brief ����̃��O�����܂��E�A���h�D���h�D�p
	���̃t�@�C���� DocOperateLog.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/05/21
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

extern FILES_ITR	gitFileIt;	//!<	�����Ă�t�@�C���̖{��
//#define gstFile	(*gitFileIt)	//!<	�C�e���[�^���\���̂ƌ��Ȃ�

extern INT		gixFocusPage;	//!<	���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X

static BOOLEAN	gbGroupUndo;	//!<	�^�Ȃ�O���[�v�A���h�D������
//-------------------------------------------------------------------------------------------------

INT	SqnUndoExec( LPUNDOBUFF, PINT, PINT );
INT	SqnRedoExec( LPUNDOBUFF, PINT, PINT );
//-------------------------------------------------------------------------------------------------

/*!
	�A���h�D�����s����̂����󂯂�
	@param[in,out]	pxDot	�h�b�g�ʒu
	@param[in,out]	pyLine	�s
	@return		INT			���s���������������ǂ���
*/
INT DocUndoExecute( PINT pxDot, PINT pyLine )
{
	INT	iRslt = 0;

	iRslt = SqnUndoExec( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), pxDot, pyLine );

	DocModifyContent( TRUE );

	return iRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	���h�D�����s����̂����󂯂�
	@param[in,out]	pxDot	�h�b�g�ʒu
	@param[in,out]	pyLine	�s
	@return		INT			���s���������������ǂ���
*/
INT DocRedoExecute( PINT pxDot, PINT pyLine )
{
	INT	iRslt = 0;

	iRslt = SqnRedoExec( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), pxDot, pyLine );

	DocModifyContent( TRUE );

	return iRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A���h�D�̓���ݒ����������
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT SqnSetting( VOID )
{
	gbGroupUndo = InitParamValue( INIT_LOAD, VL_GROUP_UNDO, 1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Ńf�[�^�̏������E�ō쐬���ɌĂ�
	@param[in]	pstBuff	������A���h�D�o�b�t�@
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT SqnInitialise( LPUNDOBUFF pstBuff )
{
	pstBuff->dNowSqn = 0;
	pstBuff->dTopSqn = 0;
	pstBuff->dGrpSqn = 0;

	pstBuff->vcOpeSqn.clear( );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�S���O�j��
	@param[in]	pstBuff	������A���h�D�o�b�t�@
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT SqnFreeAll( LPUNDOBUFF pstBuff )
{
	UINT_PTR	iCount, i;

	iCount = pstBuff->vcOpeSqn.size( );

	for( i = 0; iCount > i; i++ )
	{
		free( pstBuff->vcOpeSqn.at( i ).ptText );
	}

	SqnInitialise( pstBuff );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�o�b�t�@�r�����ǂ����m�F���āA�r���Ȃ炻���������폜
	@param[in]	pstBuff	������A���h�D�o�b�t�@
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT SqnNumberCheck( LPUNDOBUFF pstBuff )
{
	OPSQ_ITR	itOpe, itBuf;

	//	��������
	if( pstBuff->dNowSqn == pstBuff->dTopSqn )	return S_FALSE;

	//	��������
	if( 0 == pstBuff->dNowSqn ){	SqnFreeAll( pstBuff );	return  S_FALSE;	}

	TRACE( TEXT("UNDO BUF err %d %d"), pstBuff->dNowSqn, pstBuff->dTopSqn );

	itOpe = pstBuff->vcOpeSqn.end( );
	itOpe--;	//	�Ō�̂�����

	do
	{
		if( pstBuff->dNowSqn == itOpe->ixSequence ){	break;	}

		free( itOpe->ptText );
		itBuf = itOpe;
		itBuf--;
		pstBuff->vcOpeSqn.erase( itOpe );
		itOpe = itBuf;

	}
	while( itOpe != pstBuff->vcOpeSqn.begin( ) );

	pstBuff->dTopSqn = itOpe->ixSequence;

	TRACE( TEXT("UNDO BUF chk %d %d"), itOpe->ixSequence, pstBuff->dTopSqn );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�땶���ǉ��A���͍폜
	@param[in]	pstBuff	������A���h�D�o�b�t�@
	@param[in]	dCmd	������
	@param[in]	ch		�����E�폜�̂Ƃ��͖���
	@param[in]	xDot	���삵���h�b�g�ʒu
	@param[in]	yLine	���삵���s��
	@param[in]	bAlone	�P�Ƃ��ǂ����E�P�Ƃ�A�O���[�v�V�[�P���X�ԍ����C���N�����ċL�^
	@return		UINT	�O���[�v�ԍ�
*/
UINT SqnAppendLetter( LPUNDOBUFF pstBuff, UINT dCmd, TCHAR ch, INT xDot, INT yLine, UINT bAlone )
{
	UINT	uRslt;
	TCHAR	atBuffer[3];

	ZeroMemory( atBuffer, sizeof(atBuffer) );
	atBuffer[0] = ch;

	//	�땶���Œ�Ȃ̂ŁA�O���[�v�͏�ɌǓ�
	uRslt = SqnAppendString( pstBuff, dCmd, atBuffer, xDot, yLine, bAlone );

	DocModifyContent( TRUE );

	return uRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	��`�̕�����ǉ��A���͍폜���L�^
	@param[in]	pstBuff	������A���h�D�o�b�t�@
	@param[in]	dCmd	������
	@param[in]	ptStr	������
	@param[in]	pstPt	���삵���s�ʒu�ƃh�b�g�ʒu
	@param[in]	yLine	���삵���s��
	@param[in]	bAlone	�P�Ƃ��ǂ����E�P�Ƃ�A�O���[�v�V�[�P���X�ԍ����C���N�����ċL�^
	@return		UINT	��[�ԍ��H
*/
UINT SqnAppendSquare( LPUNDOBUFF pstBuff, UINT dCmd, LPCTSTR ptStr, LPPOINT pstPt, INT yLine, UINT bAlone )
{
	INT	i;
	UINT_PTR	cchMozi, cchSize;
	LPCTSTR		ptCaret, ptSprt;
	OPERATELOG	stOpe;

	//	�A���h�D�Ƃ��ōŐV�ʒu������Ă���A�������V�����̔j�����ĕt�������Ă���
	//	�����ŃV�[�P���X�ԍ��`�F�L
	SqnNumberCheck( pstBuff );

	//	Group�ԍ�����
	if( bAlone ){	pstBuff->dGrpSqn += 1;	}

	StringCchLength( ptStr, STRSAFE_MAX_CCH, &cchSize );

	ptCaret = ptStr;

	for( i = 0; yLine > i; i++ )	//	�s������
	{
		if( !( *ptCaret ) ){	break;	}

		pstBuff->dTopSqn += 1;	//	�V�[�P���X�͏펞�C���N��

		ZeroMemory( &stOpe, sizeof(OPERATELOG) );
		stOpe.dCommando  = dCmd;
		stOpe.ixSequence = pstBuff->dTopSqn;
		stOpe.ixGroup    = pstBuff->dGrpSqn;	//	

		stOpe.rdXdot  = pstPt[i].x;
		stOpe.rdYline = pstPt[i].y;

		ptSprt = StrStr( ptCaret, CH_CRLFW );	//	���s�̂Ƃ���܂�
		if( !(ptSprt) ){	ptSprt = ptStr + cchSize;	}
		//	���[�܂ŉ��s���Ȃ�������A���[�����̈ʒu������
		//	���[���s�^�����s�ɂȂ�͂�
		cchMozi = ptSprt - ptCaret;	//	�����܂ł̕��������߂�

		cchMozi++;	//	�����Ȱ���
		stOpe.cchSize = cchMozi;
		stOpe.ptText  = (LPTSTR)malloc( cchMozi * sizeof(TCHAR) );	//	�K�v���m��
		StringCchCopy( stOpe.ptText, cchMozi, ptCaret );

		pstBuff->vcOpeSqn.push_back( stOpe );

		ptCaret = NextLineW( ptSprt );	//	���̍s�̐擪�Ɉړ�
	}

	pstBuff->dNowSqn = pstBuff->vcOpeSqn.size( );

	DocModifyContent( TRUE );

	return pstBuff->dNowSqn;
}
//-------------------------------------------------------------------------------------------------

/*!
	������ǉ��A���͍폜���L�^
	@param[in]	pstBuff	������A���h�D�o�b�t�@
	@param[in]	dCmd	������
	@param[in]	ptStr	������
	@param[in]	xDot	���삵���h�b�g�ʒu
	@param[in]	yLine	���삵���s��
	@param[in]	bAlone	�P�Ƃ��ǂ����E�P�Ƃ�A�O���[�v�V�[�P���X�ԍ����C���N�����ċL�^
	@return		UINT	��[�ԍ��H
*/
UINT SqnAppendString( LPUNDOBUFF pstBuff, UINT dCmd, LPCTSTR ptStr, INT xDot, INT yLine, UINT bAlone )
{
	UINT_PTR	cchSize;
	OPERATELOG	stOpe;

	//	�A���h�D�Ƃ��ōŐV�ʒu������Ă���A�������V�����̔j�����ĕt�������Ă���
	//	�����ŃV�[�P���X�ԍ��`�F�L
	SqnNumberCheck( pstBuff );


	pstBuff->dTopSqn += 1;

	stOpe.dCommando  = dCmd;
	stOpe.ixSequence = pstBuff->dTopSqn;

	//	Group�ԍ�����
	if( bAlone ){	pstBuff->dGrpSqn += 1;	}
	stOpe.ixGroup    = pstBuff->dGrpSqn;	//	�O���[�v�ԍ��A�O�ɐ���Ȃ��悤�ɒ���

	stOpe.rdXdot  = xDot;
	stOpe.rdYline = yLine;

	//	���͂��������E�������͍폜��������
	StringCchLength( ptStr, STRSAFE_MAX_CCH, &cchSize );
	stOpe.cchSize = cchSize;	//	�������ɂ̓k���^�[�~�l�[�^���͊܂߂Ȃ�
	cchSize++;	//	�k���^�[�~�l�[�^��
	stOpe.ptText  = (LPTSTR)malloc( cchSize * sizeof(TCHAR) );	//	�K�v���m��
	StringCchCopy( stOpe.ptText, cchSize, ptStr );

	pstBuff->vcOpeSqn.push_back( stOpe );

	pstBuff->dNowSqn = pstBuff->vcOpeSqn.size( );

	DocModifyContent( TRUE );

	return pstBuff->dNowSqn;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A���h�D�����s
	@param[in]	pstBuff	������A���h�D�o�b�t�@
	@param[in]	pxDot	���삵���h�b�g�ʒu
	@param[in]	pyLine	���삵���s��
	@return		INT		���s�������ǂ���
*/
INT SqnUndoExec( LPUNDOBUFF pstBuff, PINT pxDot, PINT pyLine )
{
	OPSQ_ITR	itSqn;
	INT		xDot, yLine, iRslt = 0, dCrLf = 0, yPreLine = 0;
	UINT	dCmd, dGrp, dNow, cchSize;
	UINT	dPreGroup = 0;
	LPTSTR	ptStr;

	if( !(pstBuff) )	return 0;	//	���S�΍�

#ifdef DO_TRY_CATCH
	try{
#endif

	do
	{
		dNow  = pstBuff->dNowSqn;

		if( 0 >= dNow ){	return dCrLf;	}

		dNow--;	//	�ʒu���킹
		TRACE( TEXT("UNDO SQNUM:%u"), dNow );

		dCmd  = pstBuff->vcOpeSqn.at( dNow ).dCommando;
		dGrp  = pstBuff->vcOpeSqn.at( dNow ).ixGroup;
		xDot  = pstBuff->vcOpeSqn.at( dNow ).rdXdot;
		yLine = pstBuff->vcOpeSqn.at( dNow ).rdYline;
		
		if( 0 == dPreGroup )	//	�P��ڂ͏���������΂���
		{
			dPreGroup = dGrp;
			yPreLine = yLine;
		}
		else	//	�Q��ڈȍ~
		{
			//	�ʃO���[�v�Ȃ瑦���E
			if( dPreGroup != dGrp ){	break;	}

			//	�����s�ɂ킽���Ă���Ȃ�A�t���O�I�ɂn�m
			if( yPreLine != yLine && 0 == dCrLf ){	dCrLf = 1;	}
		}

		TRACE( TEXT("UNDO CMD:%u GRP:%u  D:%d, L:%d"), dCmd, dGrp, xDot, yLine );

		ptStr = pstBuff->vcOpeSqn.at( dNow ).ptText;
//OutOfRange�́A�n�_�h�b�g�������Ă�ꍇ������悤��
		switch( dCmd )
		{
			case  DO_INSERT:	//	�}���Ȃ�A�Y���͈͂��폜
				StringCchLength( ptStr, STRSAFE_MAX_CCH, &cchSize );
				iRslt = DocStringErase( xDot, yLine, NULL, cchSize );
				break;

			case  DO_DELETE:	//	�폜��������A����������A����΂���
				iRslt = DocStringAdd( &xDot, &yLine, pstBuff->vcOpeSqn.at( dNow ).ptText, pstBuff->vcOpeSqn.at( dNow ).cchSize );
				break;

			default:	TRACE( TEXT("�A���h�D�G���[�I�@[%u]���m�̃R�[�h"), dCmd );	return dCrLf;
		}

		*pxDot  = xDot;
		*pyLine = yLine;

		if( dCrLf < iRslt ){	dCrLf = iRslt;	}

		pstBuff->dNowSqn -= 1;

		if( !(gbGroupUndo) ){	break;	}	//	�O���[�v�A���h�D���Ȃ�

	}while( pstBuff->dNowSqn );

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return ETC_MSG( err.what(), 0 );	}
	catch( ... ){	return  ETC_MSG( ("etc error"), 0 );	}
#endif

	return dCrLf;
}
//-------------------------------------------------------------------------------------------------

/*!
	���h�D�����s
	@param[in]	pstBuff	������A���h�D�o�b�t�@
	@param[in]	pxDot	���삵���h�b�g�ʒu
	@param[in]	pyLine	���삵���s��
	@return		INT		���s�������ǂ���
*/
INT SqnRedoExec( LPUNDOBUFF pstBuff, PINT pxDot, PINT pyLine )
{
	OPSQ_ITR	itSqn;
	INT		xDot, yLine, iRslt = 0, dCrLf = 0, yPreLine = 0;
	UINT	dCmd, dGrp, dNow, cchSize;
	UINT	dPreGroup = 0;
	LPTSTR	ptStr;

#ifdef DO_TRY_CATCH
	try{
#endif

	do
	{
		dNow  = pstBuff->dNowSqn;
		if( dNow == pstBuff->vcOpeSqn.size( ) ){	return 0;	}

		//dNow++;	//	�ʒu���킹
		TRACE( TEXT("REDO SQNUM:%u"), dNow );

		dCmd  = pstBuff->vcOpeSqn.at( dNow ).dCommando;
		dGrp  = pstBuff->vcOpeSqn.at( dNow ).ixGroup;
		xDot  = pstBuff->vcOpeSqn.at( dNow ).rdXdot;
		yLine = pstBuff->vcOpeSqn.at( dNow ).rdYline;

		if( 0 == dPreGroup )	//	�P��ڂ͏���������΂���
		{
			dPreGroup = dGrp;
			yPreLine = yLine;
		}
		else	//	�Q��ڈȍ~
		{
			//	�ʃO���[�v�Ȃ瑦���E
			if( dPreGroup != dGrp ){	break;	}

			//	�����s�ɂ킽���Ă���Ȃ�A�t���O�I�ɂn�m
			if( yPreLine != yLine && 0 == dCrLf ){	dCrLf = 1;	}
		}

		TRACE( TEXT("REDO CMD:%u GRP:%u  D:%d, L:%d"), dCmd, dGrp, xDot, yLine );

		ptStr = pstBuff->vcOpeSqn.at( dNow ).ptText;

		switch( dCmd )	//	���h�D�̏ꍇ�͑f���ɏ�������΂���
		{
			case  DO_INSERT:
				iRslt = DocStringAdd( &xDot, &yLine, pstBuff->vcOpeSqn.at( dNow ).ptText, pstBuff->vcOpeSqn.at( dNow ).cchSize );
				break;

			case  DO_DELETE:
				StringCchLength( ptStr, STRSAFE_MAX_CCH, &cchSize );
				iRslt = DocStringErase( xDot, yLine, NULL, cchSize );
				break;

			default:	TRACE( TEXT("���h�D�G���[�I�@[%u]���m�̃R�[�h"), dCmd );	return dCrLf;
		}

		*pxDot  = xDot;
		*pyLine = yLine;

		if( dCrLf < iRslt ){	dCrLf = iRslt;	}

		pstBuff->dNowSqn += 1;

		if( !(gbGroupUndo) ){	break;	}	//	�O���[�v�A���h�D���Ȃ�

	}while( pstBuff->dNowSqn != pstBuff->vcOpeSqn.size( ) );

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return ETC_MSG( err.what(), 0 );	}
	catch( ... ){	return  ETC_MSG( ("etc error"), 0 );	}
#endif

	return dCrLf;
}
//-------------------------------------------------------------------------------------------------

