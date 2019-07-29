/*! @file
	@brief �I��͈̖͂ʓ|�݂܂�
	���̃t�@�C���� DocSelect.cpp �ł��B
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
//-------------------------------------------------------------------------------------------------

extern FILES_ITR	gitFileIt;	//	�����Ă�t�@�C���̖{�́E�C�e���[�^���\���̂ƌ��Ȃ�
extern INT		gixFocusPage;	//	���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X

extern  UINT	gbUniPad;		//	�p�f�B���O�Ƀ��j�R�[�h�������āA�h�b�g�������Ȃ��悤�ɂ���
extern  UINT	gbCrLfCode;		//	���s�R�[�h�F�O������΁E��O�x�x 

static INT		gdSelByte;		//!<	�I��͈͂̃o�C�g��
//-------------------------------------------------------------------------------------------------


INT		DocLetterSelStateToggle( INT, INT, INT );
VOID	DocSelectedByteStatus( VOID );
//-------------------------------------------------------------------------------------------------

/*!
	�I��͈͂�����J�n�s�ƏI���s��o�^
	@param[in]	dTop	�I��͈͊J�n�s
	@param[in]	dBottom	�I��͈͏I���s
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocSelRangeSet( INT dTop, INT dBottom )
{
	TRACE( TEXT(" �I�������W�Z�b�g[%d - %d]"), dTop, dBottom );

	(*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop    = dTop;
	(*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom = dBottom;

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I��͈͂�����J�n�s�ƏI���s���擾
	@param[in]	pdTop	�I��͈͊J�n�s�����o�b�t�@���̂ۂ���
	@param[in]	pdBtm	�I��͈͏I���s�����o�b�t�@���̂ۂ���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocSelRangeGet( PINT pdTop, PINT pdBtm )
{
	if( pdTop ){	*pdTop = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop;	}
	if( pdBtm ){	*pdBtm = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom;	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I��͈͂�T���Đ��l���Đݒ肷��
	@param[in]	pdTop	�I��͈͊J�n�s�����o�b�t�@���̂ۂ��񂽁ENULL��
	@param[in]	pdBtm	�I��͈͏I���s�����o�b�t�@���̂ۂ��񂽁ENULL��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocSelRangeReset( PINT pdTop, PINT pdBtm )
{
	INT	iTop, iEnd, iLine;
	LINE_ITR	itLine, itLnEnd;
	LETR_ITR	itLtr;

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	itLnEnd = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.end();

	iTop = -1;	iEnd = -1;
	for( iLine = 0; itLnEnd != itLine; itLine++, iLine++ )
	{
		for( itLtr = itLine->vcLine.begin(); itLine->vcLine.end() != itLtr; itLtr++ )
		{
			if( CT_SELECT & itLtr->mzStyle )
			{
				if( 0 > iTop )	iTop = iLine;
				iEnd = iLine;

				break;	//	��ł��q�b�g����Ί֌W�Ȃ�
			}
		}
	}

	(*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop    = iTop;
	(*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom = iEnd;

	if( pdTop ){	*pdTop = iTop;	}
	if( pdBtm ){	*pdBtm = iEnd;	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I���o�C�g�����w�肵���l�ɏ��������E�����N�����[�p
	@param[in]	iBytes	�V�����l
*/
VOID DocSelByteSet( INT iBytes )
{
	gdSelByte = iBytes;
}
//-------------------------------------------------------------------------------------------------

/*!
	�����ꂽ�h�b�g�ʒu�̒���̕����̑I����Ԃ̊m�F
	@param[in]	nowDot	�Ώۂ̃h�b�g�ʒu
	@param[in]	rdLine	�Ώۂ̍s�ԍ��E�h�L�������g�̂O�C���f�b�N�X
	@return		��O�I����ԁ@�O�I�����ĂȂ�
*/
UINT DocLetterSelStateGet( INT nowDot, INT rdLine )
{
	UINT	dStyle;
	INT		iLetter;
	INT_PTR	iLines, iLength;

	LINE_ITR	itLine;

	iLines = DocNowFilePageLineCount( );
	if( iLines <= rdLine )	return 0;

	iLetter = DocLetterPosGetAdjust( &nowDot, rdLine, 0 );	//	���̕����ʒu���m�F

	//	����̕������m�F
	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, rdLine );
	if( (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.end() == itLine ){	return 0;	}

	iLength = itLine->vcLine.size();
	if( iLength <= iLetter )	return 0;

	//	�t���O����
	dStyle  = itLine->vcLine.at( iLetter ).mzStyle;

	if( dStyle & CT_SELECT )	return 1;

	return 0;
}
//-------------------------------------------------------------------------------------------------

/*!
	�����ꂽ�h�b�g�ʒu�̒���̕����̑I����Ԃ�ON/OFF���āA�Y�������̕���Ԃ��E�P�Ƃł͌Ă΂�Ȃ��H
	@param[in]	nowDot	�Ώۂ̃h�b�g�ʒu
	@param[in]	rdLine	�Ώۂ̍s�ԍ��E�h�L�������g�̂O�C���f�b�N�X
	@param[in]	dForce	�O�l�𗯁@�{�I����ԁ@�[�I������
	@return		�Y�������̃h�b�g��
*/
INT DocLetterSelStateToggle( INT nowDot, INT rdLine, INT dForce )
{
	UINT	dStyle, maeSty;
	INT		dLtrDot = 0, iLetter, dByte;
	INT_PTR	iLines, iLength;

	LINE_ITR	itLine;

	iLines = DocNowFilePageLineCount( );
	if( iLines <= rdLine )	return 0;

	iLetter = DocLetterPosGetAdjust( &nowDot, rdLine, 0 );	//	���̕����ʒu���m�F

	//	����̕����̕����m�F
	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, rdLine );
	if( (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.end() == itLine ){	return 0;	}

	iLength = itLine->vcLine.size();
	if( iLength <= iLetter )	return 0;

	dLtrDot = itLine->vcLine.at( iLetter ).rdWidth;
	dByte   = itLine->vcLine.at( iLetter ).mzByte;


	//	�t���O����
	dStyle  = itLine->vcLine.at( iLetter ).mzStyle;
	maeSty = dStyle;
	if( 0 == dForce ){		dStyle ^=  CT_SELECT;	}
	else if( 0 < dForce ){	dStyle |=  CT_SELECT;	}
	else if( 0 > dForce ){	dStyle &= ~CT_SELECT;	}
	itLine->vcLine.at( iLetter ).mzStyle = dStyle;

	TRACE( TEXT("L[%d] D[%d] B[%d] f[0x%X]"), rdLine, dLtrDot, dByte, dStyle );

	if( maeSty != dStyle )	//	�t���O���삳��Ă���
	{
		if( CT_SELECT & dStyle )	gdSelByte += dByte;
		else						gdSelByte -= dByte;

		if( 0 >  gdSelByte )	gdSelByte = 0;
		//	�O�����ɂȂ�����{���͂�������
	}

	return dLtrDot;	//	�h�b�g���߂��Ăn�j
}
//-------------------------------------------------------------------------------------------------

/*!
	�w�肳�ꂽ�s�̎n�_�I�_�h�b�g�ʒu�̋�Ԃ̕����̑I����Ԃ�ON/OFF����
	@param[in]	dBgnDot	�J�n�h�b�g�ʒu�E�}�C�i�X�Ȃ�O
	@param[in]	dEndDot	�I���h�b�g�ʒu�E�}�C�i�X�Ȃ�s���[
	@param[in]	rdLine	�Ώۂ̍s�ԍ��E�h�L�������g�̂O�C���f�b�N�X
	@param[in]	dForce	�O�l�𗯁@�{�I����ԁ@�[�I������
	@return		�Y�������̃h�b�g��
*/
INT DocRangeSelStateToggle( INT dBgnDot, INT dEndDot, INT rdLine, INT dForce )
{
	UINT_PTR	iLines;
	INT	dLtrDot = 0, dMaxDots, dDot;
	RECT	rect;

	iLines = DocNowFilePageLineCount( );
	if( (INT)iLines <=  rdLine )	return 0;

	dMaxDots = DocLineParamGet( rdLine, NULL, NULL );
	//	�͈͒���
	if( 0 > dBgnDot )	dBgnDot = 0;
	if( 0 > dEndDot )	dEndDot = dMaxDots;

	for( dDot = dBgnDot; dEndDot > dDot;  )
	{
		dDot += DocLetterSelStateToggle( dDot, rdLine, dForce );
	}
//����ς̃A���̌v�Z���w���E�t���O�̌��ˍ����Ƃ�
	dLtrDot = dDot - dBgnDot;

	rect.left   = dBgnDot;
	rect.top    = rdLine * LINE_HEIGHT;
	rect.right  = dEndDot;
	rect.bottom = rect.top + LINE_HEIGHT;

//	ViewRedrawSetLine( rdLine );
	ViewRedrawSetRect( &rect );

	DocSelectedByteStatus(  );

	return dLtrDot;	//	�h�b�g���߂��Ăn�j
}
//-------------------------------------------------------------------------------------------------

/*!
	�w�肳�ꂽ�s�̉��s�̑I����Ԃ�ON/OFF����
	@param[in]	rdLine	�Ώۂ̍s�ԍ��E�h�L�������g�̂O�C���f�b�N�X
	@param[in]	dForce	�O�l�𗯁@�{�I����ԁ@�[�I������
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocReturnSelStateToggle( INT rdLine, INT dForce )
{
	UINT_PTR	iLines;
	UINT		dStyle, maeSty;
	INT			iLnDot, dByte;
	RECT		rect;

	LINE_ITR	itLine;

	iLines = DocNowFilePageLineCount( );
	if( (INT)iLines <=  rdLine )	return E_OUTOFMEMORY;

	iLnDot = DocLineParamGet( rdLine, NULL, NULL );
	//	�t���O����
	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, rdLine );

	dStyle = itLine->dStyle;
	maeSty = dStyle;
	if( 0 == dForce ){		dStyle ^=  CT_SELRTN;	}
	else if( 0 < dForce ){	dStyle |=  CT_SELRTN;	}
	else if( 0 > dForce ){	dStyle &= ~CT_SELRTN;	}
	itLine->dStyle = dStyle;
	if( maeSty != dStyle )	//	�t���O���삳��Ă���
	{
		if( gbCrLfCode )	dByte = YY2_CRLF;
		else				dByte = STRB_CRLF;

		if( CT_SELRTN & dStyle )	gdSelByte += dByte;
		else						gdSelByte -= dByte;

		if( 0 >  gdSelByte )	gdSelByte = 0;
		//	�O�����ɂȂ�����{���͂�������

		DocSelectedByteStatus(  );
	}

	rect.left   = iLnDot;
	rect.top    = rdLine * LINE_HEIGHT;
	rect.right  = iLnDot + 20;	//	���Ԃ񂱂ꂭ�炢
	rect.bottom = rect.top + LINE_HEIGHT;

//	ViewRedrawSetLine( rdLine );
	ViewRedrawSetRect( &rect );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�y�[�W�S�̂̑I����Ԃ�ON/OFF����
	@param[in]	dForce	�O�ف@�{�I����ԁ@�[�I������
	@return		�S�̕�����
*/
INT DocPageSelStateToggle( INT dForce )
{
	UINT_PTR	iLines, ln, iLetters, mz;
	UINT		dStyle;
	INT			iTotal, iDot, iWid;
	RECT		inRect;

	LINE_ITR	itLine;

	if( 0 == dForce )	return 0;	//	�O�Ȃ珈�����Ȃ�

	if( 0 > gixFocusPage )	return 0;	//	����ȏ󋵉��ł͏������Ȃ�

	iTotal = 0;

	iLines = DocNowFilePageLineCount( );

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	for( ln = 0; iLines > ln; ln++, itLine++ )
	{
		iDot = 0;	//	�����܂ł̃h�b�g�������ߍ���
		inRect.top    = ln * LINE_HEIGHT;
		inRect.bottom = inRect.top + LINE_HEIGHT;

		iLetters = itLine->vcLine.size( );	//	���̍s�̕������m�F����

		//	�땶�����A�S�����`�F�L���Ă���
		for( mz = 0; iLetters > mz; mz++ )
		{
			//	���O�̏��
			dStyle = itLine->vcLine.at( mz ).mzStyle;
			iWid   = itLine->vcLine.at( mz ).rdWidth;

			inRect.left  = iDot;
			inRect.right = iDot + iWid;

			if( 0 < dForce )
			{
				itLine->vcLine.at( mz ).mzStyle |=  CT_SELECT;
				if( !(dStyle & CT_SELECT) ){	ViewRedrawSetRect( &inRect );	}
			}
			else
			{
				itLine->vcLine.at( mz ).mzStyle &= ~CT_SELECT;
				if( dStyle & CT_SELECT ){	ViewRedrawSetRect( &inRect );	}
			}

			iDot += iWid;
			iTotal++;
		}

		//	��s�I������疖���󋵊m�F�B���s�E�{�����[�ɉ��s�͂Ȃ��E�I���̂Ƃ��̂�
		dStyle = itLine->dStyle;
		inRect.left  = iDot;
		inRect.right = iDot + 20;	//	���s�`��G���A�E��̂��ꂭ�炢
		if( 0 < dForce )
		{
			if( iLines > ln+1 )
			{
				itLine->dStyle |=  CT_SELRTN;
				if( !(dStyle & CT_SELRTN) ){	ViewRedrawSetRect( &inRect );	}
			}
		}
		else
		{
			itLine->dStyle &=  ~CT_SELRTN;
			if( dStyle & CT_SELRTN ){	ViewRedrawSetRect( &inRect );	}
		}
	}


	if( 0 < dForce )	//	�őS�̂̃o�C�g�����̂��̂��A��I���Ȃ̂łO
	{
		DocSelRangeSet(  0, iLines - 1 );
		DocPageParamGet( NULL, &gdSelByte );
	}
	else
	{
		DocSelRangeSet( -1, -1 );
		gdSelByte = 0;
	}
	DocSelectedByteStatus(  );

//	ViewRedrawSetLine( -1 );	//	��ʕ\���X�V

	return iTotal;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I������Ă�o�C�g�����X�e�[�^�X�o�[�ɕ\��
*/
VOID DocSelectedByteStatus( VOID )
{
	TCHAR	atBuffer[MIN_STRING];
	
	if( gdSelByte )
	{
		StringCchPrintf( atBuffer, MIN_STRING, TEXT("SEL %d Bytes"), gdSelByte );
		MainStatusBarSetText( SB_SELBYTE, atBuffer );
	}
	else
	{
		MainStatusBarSetText( SB_SELBYTE, TEXT("") );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I������Ă���Ƃ����S�폜�����Ⴄ
	@param[in]	pdDot	�L�����b�g�h�b�g�ʒu�E����������K�v������
	@param[in]	pdLine	�s�ԍ��E����������K�v������
	@param[in]	bSqSel	��`�I�����Ă�̂��ǂ����ED_SQUARE
	@param[in]	bFirst	�A���h�D�p�E���ꂪ�ŏ��̃A�N�V������
	@return	��O���s�������@�O��s�̂�
*/
INT DocSelectedDelete( PINT pdDot, PINT pdLine, UINT bSqSel, BOOLEAN bFirst )
{
//	UINT_PTR	iLines;
	UINT_PTR	iMozis;
	INT			i, j, dBeginX = 0, dBeginY = 0, cbSize;
	INT			iLct, k, bCrLf;
	LPTSTR		ptText;
	LPPOINT		pstPt;

	LETR_ITR	itLtr, itEnd, itHead, itTail;
	LINE_ITR	itLine;

#ifdef DO_TRY_CATCH
	try{
#endif

	bSqSel &= D_SQUARE;	//	��`�r�b�g�����c��

	//	�y�[�W�S�̂̍s��
//	iLines = DocNowFilePageLineCount( );
	i = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop;
	j = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom;
	TRACE( TEXT("�͈͍폜[T%d - B%d]"), i, j );
	if( 0 > i ){	return 0;	}	//	�I��͈͂���������


//�A���h�D�o�b�t�@�����O�̏���
	iLct = j - i + 1;	//	�܂܂��s�Ȃ̂ŁA������̒���
	cbSize = DocSelectTextGetAlloc( D_UNI | bSqSel, (LPVOID *)(&ptText), NULL );
	pstPt = (LPPOINT)malloc( iLct * sizeof(POINT) );
	ZeroMemory( pstPt, iLct * sizeof(POINT) );
	k = iLct - 1;

	bCrLf = iLct - 1;

	dBeginY = i;	//	�I�����̂���s

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, j );

	for( ; i <= j; j--, k--, itLine-- )//begin�𒴂�����A�E�c�I
	{
		//	continue�͎g���Ȃ��E

		iMozis = itLine->vcLine.size( );
		if( 0 < iMozis )
		{
			itLtr = itLine->vcLine.begin(  );
			itEnd = itLine->vcLine.end(  );
			itHead = itEnd;
			itTail = itEnd;

			dBeginX = 0;

			//	�ŏ��̑I�𕔕�������
			for( ; itLtr != itEnd; itLtr++ )
			{
				if( CT_SELECT & itLtr->mzStyle )
				{
					itHead =  itLtr;
					break;
				}

				dBeginX += itLtr->rdWidth;	//	�Ӗ�������͍̂Ō�̂Ƃ���Ȃ̂ŁA�펞�㏑���ł���
			}

			//	�I������ĂȂ����܂Ō���
			for( ; itLtr != itEnd; itLtr++ )
			{
				if( !(CT_SELECT & itLtr->mzStyle) )
				{
					itTail =  itLtr;
					break;
				}
			}
		}

		pstPt[k].x = dBeginX;
		pstPt[k].y = j;

		if( 0 < iMozis )
		{
			//	�Y���͈͂��폜�E���[�́A�Y�������̒��O�܂ł��ΏہE���[���͔̂�Ώ�
			itLine->vcLine.erase( itHead, itTail );
		}

		//	���s���܂܂�Ă�����
		if( CT_SELRTN & itLine->dStyle ){	DocLineCombine( j );	}

		DocLineParamGet( j, NULL, NULL );	//	�o�C�g���Čv�Z

		//	��`�̏ꍇ�́A�e�s���ɖʓ|�݂Ȃ�����
		if( D_SQUARE & bSqSel ){	DocBadSpaceCheck( j );	}

		//	���s�T�N��Ƃ���ɂ��at�������ɂȂ�H
	
	//	iLines = DocNowFilePageLineCount( );	//	�y�[�W�S�̂̍s���Đݒ�H

		if( (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin() == itLine )	break;
		//	�ʒu�I�ɖ��[�������烋�[�v�����ɏI���
	}

	ViewSelPageAll( -1 );	//	�I��͈͖����Ȃ�

	//	�J�[�\���ʒu�ړ����Ȃ�����
	*pdDot = dBeginX;	*pdLine = dBeginY;

	//	�ŏI�I�Ɏc���Ă���s�̃`�F�b�N��������΂���
	if( !(D_SQUARE & bSqSel)  ){	DocBadSpaceCheck( dBeginY );	}

	if( bSqSel ){	SqnAppendSquare( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), DO_DELETE, ptText, pstPt, iLct , bFirst );	}
	else{		SqnAppendString( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), DO_DELETE, ptText, dBeginX, dBeginY, bFirst );	}

	FREE( ptText );

	FREE( pstPt );

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return (INT)ETC_MSG( err.what(), 0 );	}
	catch( ... ){	return (INT)ETC_MSG( ("etc error"), 0 );	}
#endif

	return bCrLf;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I��͈͂��w�蕶����œh��Ԃ�
	@param[in]	ptBrush	�u���V������ENULL�Ȃ��
	@param[in]	pdDot	�L�����b�g�h�b�g�ʒu�E����������K�v������
	@param[in]	pdLine	�s�ԍ��E����������K�v������
	@return	��O�h�����@�O���ĂȂ�
*/
INT DocSelectedBrushFilling( LPTSTR ptBrush, PINT pdDot, PINT pdLine )
{
	UINT_PTR	iMozis;
	UINT_PTR	cchSize;
	INT			i, j, dBeginX = 0, dBeginY = 0;
	INT			iLct, dTgtDot, dBgnDot, dNowDot;
	BOOLEAN		bFirst;

	LPTSTR		ptReplc = NULL, ptDeled;
//	INT			dZenSp, dHanSp, dUniSp;

	wstring		wsBuffer;
	LETR_ITR	itLtr, itEnd, itHead, itTail;

	LINE_ITR	itLine;

	bFirst = TRUE;

	i = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop;
	j = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom;
	TRACE( TEXT("�͈͊m�F[T%d - B%d]"), i, j );
	if( 0 > i ){	return 0;	}	//	�I��͈͂���������

	dBeginY = i;	//	�I�����̂���s
	dBeginX = 0;

	//	��s���������Ă���
	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, i );

	for( iLct = i; j >= iLct; iLct++, itLine++ )
	{
		//	�������m�F����
		iMozis = itLine->vcLine.size( );
		if( 0 < iMozis )
		{
			itLtr = itLine->vcLine.begin(  );
			itEnd = itLine->vcLine.end(  );
			itHead = itEnd;
			itTail = itEnd;

			dBgnDot = 0;
			dTgtDot = 0;

			//	�ŏ��̑I�𕔕�������
			for( ; itLtr != itEnd; itLtr++ )
			{
				if( CT_SELECT & itLtr->mzStyle )
				{
					itHead =  itLtr;
					dTgtDot = itLtr->rdWidth;
					itLtr++;	//	���̕������Q��
					break;
				}

				dBgnDot += itLtr->rdWidth;
			}
			if( iLct == i ){	dBeginX = dBgnDot;	}	//	�Ӗ�������͍̂Ō�̂Ƃ���

			//	�I������ĂȂ����܂Ō���
			for( ; itLtr != itEnd; itLtr++ )
			{
				if( !(CT_SELECT & itLtr->mzStyle) )
				{
					itTail =  itLtr;
					break;
				}
				dTgtDot += itLtr->rdWidth;	//	�h�b�g�����m�F
			}

			//	���Ă͂߂�A�����v�Z����
			if( ptBrush )
			{
				ptReplc = BrushStringMake( dTgtDot, ptBrush );
			}
			else	//	�󔒎w��Ƃ�������
			{
				ptReplc = DocPaddingSpaceMake( dTgtDot );
			}

			//	�����ŁA���ߕ����񂪍쐬�s�Ȃ�A���̍s�̏����͔�΂�
			if( !(ptReplc) )	continue;


			//	�Y�������̓��e���L�^���A���h�D�p
			wsBuffer.clear();
			for( itLtr = itHead; itLtr != itTail; itLtr++ )
			{
				wsBuffer += itLtr->cchMozi;
			}

			cchSize = wsBuffer.size( ) + 1;
			ptDeled = (LPTSTR)malloc( cchSize * sizeof(TCHAR) );
			StringCchCopy( ptDeled, cchSize, wsBuffer.c_str( ) );

			//	�Y���������폜
			itLine->vcLine.erase( itHead, itTail );
			//	�u���V������Ŗ��߂�
			StringCchLength( ptReplc, STRSAFE_MAX_CCH, &cchSize );
			dNowDot = dBgnDot;
			DocStringAdd( &dNowDot, &iLct, ptReplc, cchSize );

			SqnAppendString( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), DO_DELETE, ptDeled, dBgnDot, iLct, bFirst );	bFirst = FALSE;
			SqnAppendString( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), DO_INSERT, ptReplc, dBgnDot, iLct, bFirst );

			FREE( ptDeled );

			FREE( ptReplc );

			ViewRedrawSetLine( iLct );
		}

	}

	ViewSelPageAll( -1 );	//	�I��͈͖����Ȃ�

	//	�J�[�\���ʒu�ړ����Ȃ�����
	*pdDot = dBeginX;	*pdLine = dBeginY;

	return 1;
}
//-------------------------------------------------------------------------------------------------

#pragma message ("�w��s�I��͈͊m�ہA�K�v�ɂȂ�������")
#if 0
/*!
	�w��s�̑I��͈͂��e�L�X�g�Ŋm�ۂ���
	@param[in]	itLine	�w��s�̃C�e���[�^
	@param[in]	bStyle	���j�R�[�h���V�t�gJIS��
	@param[out]	*pText	�m�ۂ����̈��Ԃ��E���C�h�������}���`�����ɂȂ�ENULL���ƕK�v�o�C�g����Ԃ��̂�
	@param[out]	*piDot	�I��͈͊J�n�ʒu�h�b�g��Ԃ�
	@param[out]	*piMozi	�I��͈͂̃��j�R�[�h��������Ԃ�
	@return				�m�ۂ����o�C�g���ENULL�^�[�~�l�[�^���܂�
*/
INT DocSelectLineSelTextAlloc( LINE_ITR itLine, UINT bStyle, LPVOID *pText, PINT piDot, PINT piMozi )
{

	return 0;
}
//-------------------------------------------------------------------------------------------------
#endif

/*!
	�y�[�W�S�̂���A�I������Ă��镶������m�ۂ���Efree�͌Ă񂾕��ł��
	@param[in]	bStyle	�P���j�R�[�h���V�t�gJIS�ŁA��`���ǂ���
	@param[out]	*pText	�m�ۂ����̈��Ԃ��E���C�h�������}���`�����ɂȂ�ENULL���ƕK�v�o�C�g����Ԃ��̂�
	@param[out]	*pstPt	�I��͈͂̍s�ԍ��ƊJ�n�h�b�g�ʒu�����������ĕԂ��E�J���͌Ă񂾂ق��ł��ENULL�Ȃ牽�����Ȃ�
	@return				�m�ۂ����o�C�g���ENULL�^�[�~�l�[�^���܂�
*/
INT DocSelectTextGetAlloc( UINT bStyle, LPVOID *pText, LPPOINT *pstPt )
{
	//	�w��s�̎w��͈͂��R�s�[����悤�ɂ���΂���
	//	SJIS�̏ꍇ�́A���j�R�[�h������&#ddddd;�Ŋm�ۂ����
	//	������������&#xhhhh;��������Ȃ�

	UINT_PTR	iLines, i, j, iLetters;
	INT_PTR		iSize, cchSz;
	INT			d, k, m, iLn;
	BOOLEAN		bNoSel;
	LPPOINT		pstPoint = NULL;

	string	srString;	//	���j�R�[�h�E�V�t�gJIS�Ŋm��
	wstring	wsString;

	LINE_ITR	itLine;

	srString.clear( );
	wsString.clear( );

	//	�y�[�W�S�̂̍s��
	iLines = DocNowFilePageLineCount( );
	//	�J�n�n�_����J�n
	d = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop;
	k = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom;
	TRACE( TEXT("�I����e�m��[%d - %d]"), d, k );
	if( 0 > d ){	d = 0;	}
	if( 0 > k ){	k = iLines -  1;	}

	if( pstPt )
	{
		iLn = k - d + 1;	//	�s�̐�
		if( 0 < iLn ){	pstPoint = (LPPOINT)malloc( iLn * sizeof(POINT) );	}
		*pstPt = pstPoint;
	}

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, d );

	for( m = 0, i = d; iLines > i; i++, m++, itLine++ )
	{
		if( pstPoint  ){	pstPoint[m].x = 0;	pstPoint[m].y = i;	}

		//	�e�s�̕�����
		iLetters = itLine->vcLine.size( );

		bNoSel = TRUE;
		for( j = 0; iLetters > j; j++ )
		{
			//	�I������Ă��镔���𕶎���Ɋm��
			if( CT_SELECT & itLine->vcLine.at( j ).mzStyle )
			{
				bNoSel = FALSE;

				if( bStyle & D_UNI )	wsString += itLine->vcLine.at( j ).cchMozi;
				else	srString +=  string( itLine->vcLine.at( j ).acSjis );
			}

			if( bNoSel && pstPt )	pstPoint[m].x += itLine->vcLine.at( j ).rdWidth;
		}

		if( bStyle & D_SQUARE  )	//	��`�̂Ƃ��͗e�͂Ȃ����s
		{
			if( bStyle & D_UNI )	wsString += wstring( CH_CRLFW );
			else					srString +=  string( CH_CRLFA );
		}
		else
		{
			//	���s���܂܂�Ă����炻�̕��m��
			if( CT_SELRTN & itLine->dStyle )
			{
				if( bStyle & D_UNI )	wsString += wstring( CH_CRLFW );
				else					srString +=  string( CH_CRLFA );
			}
		}

		//	�I��͈͖��[�܂ŃC�b���炨���܂�
		if( (INT)i == (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom )	break;
	}


	if( bStyle & D_UNI )	//	���j�R�[�h�ł���
	{
		cchSz = wsString.size( ) + 1;	//	NULL�^�[�~�l�[�^������
		iSize = cchSz * sizeof(TCHAR);	//	���j�R�[�h�Ȃ̂Ńo�C�g���͂Q�{�ł���

		if( pText )
		{
			*pText = (LPTSTR)malloc( iSize );
			ZeroMemory( *pText, iSize );
			StringCchCopy( (LPTSTR)(*pText), cchSz, wsString.c_str( ) );
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

	return iSize;
}
//-------------------------------------------------------------------------------------------------

/*!
	���o�Ώۗ̈�����o��
	@param[in]	hInst	�����l
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocExtractExecute( HINSTANCE hInst )
{
	INT	dOffDot, dCount;
	BOOLEAN	bLnFirst, bMzFirst, bIsVoid;
	LPTSTR	ptSpace, ptString;
	UINT_PTR	cch;//, i;

	LINE_ITR	itLnFirst, itLnLast, itLnErate, itLnEnd;
	LETR_ITR	itMozi, itMzEnd;

	wstring	wsBuffer;


	if( 0 >= DocNowFilePageCount( ) )	return S_FALSE;

	//	�J�n�s�ƏI�~�s�E�I�t�Z�b�g�ʂ�����
	itLnErate = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	itLnEnd   = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.end();
	itLnFirst = itLnErate;
	itLnLast  = itLnEnd;

	dOffDot = DocPageMaxDotGet( -1, -1 );	//	MAX�ʒu�������ɂ��Ƃ��΂���

	bLnFirst = TRUE;

	for( ; itLnEnd != itLnErate; itLnErate++ )	//	�s�T�[�`
	{
		itMozi  = itLnErate->vcLine.begin();
		itMzEnd = itLnErate->vcLine.end();

		dCount = 0;

		for( ; itMzEnd != itMozi; itMozi++ )	//	�����T�[�`
		{
			if( CT_SELECT & itMozi->mzStyle )	//	�I����Ԕ���
			{
				if( bLnFirst )	//	�ŏ��̍s���������ł����
				{
					itLnFirst = itLnErate;	//	���̍s���L�^����
					bLnFirst = FALSE;
				}
				itLnLast  = itLnErate;	//	�I����Ԃ�����̂ŏI�~�s���X�V

				//	�����܂ł̃I�t�Z�b�g�ʂ�肳��ɏ�������΍X�V
				if( dOffDot > dCount )	dOffDot = dCount;

				break;	//	���̍s�Ɉړ�
			}

			dCount += itMozi->rdWidth;	//	�����܂ł̃h�b�g�������ߍ���

		}
	}
	if( itLnLast != itLnEnd )	 itLnLast++;	//	�I�~�̎��̍s�������Ă���

	if( bLnFirst )	return  S_FALSE;	//	�I��͈͂��Ȃ������玀�ɂ܂�

	wsBuffer.clear();

	//	�J�n�s������e���m�ۂ��Ă���
	for( itLnErate = itLnFirst; itLnLast != itLnErate; itLnErate++ )	//	�s�T�[�`
	{
		itMozi  = itLnErate->vcLine.begin();
		itMzEnd = itLnErate->vcLine.end();

		bMzFirst = TRUE;
		bIsVoid  = FALSE;
		dCount   = 0;

		for( ; itMzEnd != itMozi; itMozi++ )	//	�����T�[�`
		{
			if( CT_SELECT & itMozi->mzStyle )	//	�I����Ԕ���
			{
				if( bIsVoid )	//	���O�܂Ŗ��I�����
				{
					if( bMzFirst )	//	�ŏ��̋󔒕����ł����
					{
						dCount -= dOffDot;	//	�I�t�Z�b�g����
						if( 0 > dCount )	dCount = 0;
						bMzFirst = FALSE;
					}

					//	����Space�����E�s�Ȃ�NULL���Ԃ�
					ptSpace = DocPaddingSpaceMake( dCount );
					if( ptSpace )
					{
						wsBuffer += ptSpace;
						//StringCchLength( ptSpace, STRSAFE_MAX_CCH, &cch );
						//for( i = 0; cch > i; i++ )
						//{
						//	wsBuffer += (ptSpace[i]);
						//}
						FREE(ptSpace);
					}
				}

				wsBuffer += itMozi->cchMozi;
				dCount =  0;	//	���Z�b�g
				bIsVoid = FALSE;
			}
			else
			{
				dCount += itMozi->rdWidth;	//	�����܂ł̃h�b�g�������ߍ���
				bIsVoid = TRUE;
			}
		}

		wsBuffer += CH_CRLFW;	//	���s�ǉ�
	}
	//	���̎��_�ŁAwsBuffer�ɑS�̂������Ă���͂�

	cch = wsBuffer.size( ) + 1;
	ptString = (LPTSTR)malloc( cch * sizeof(TCHAR) );
	StringCchCopy( ptString, cch, wsBuffer.c_str( ) );

	if( hInst )	//	�������Ă�Ȃ烌�C���{�b�N�X��
	{
		LayerBoxVisibalise( hInst, ptString, 0x00 );
	}
	else	//	�Ȃ��Ȃ�N���b�y�{�[�h
	{
		DocClipboardDataSet( ptString, cch * sizeof(TCHAR), D_UNI );
	}


	FREE(ptString);

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

