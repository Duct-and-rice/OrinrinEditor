/*! @file
	@brief �Ȃ񂩒����Ƃ�����������
	���̃t�@�C���� DocAdjustant.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/05/10
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
�P�h�b�g���炵
�ڕW�̕����P�P�Ŋ����āA�]��Q�b�g
�P�`�T�Ȃ甼�pSP�ǉ��A�U�`�P�O�Ȃ�S�pSP�ǉ����āA
�ڕW�̃h�b�g���ɂȂ�܂őS�pSP(11�h�b�g)�𔼊pSP�Q��(10�h�b�g)�ɒu�������Ă���
��������Ȃ��Ȃ����肵���瑀��s�E�R�W�ȉ��Ŕ�������
�K�v�ȑS�p���pSP�����܂�����A�������������ɂ����āA�����Ɉ������
���Ȃ���������Ă����B

�E����
�Œᕝ���Q�Q�ŁA���Ƃ̓p�^�[���B�͈͓��̈�Ԓ��������x�[�X�O�Ƃ��A
��������̂ւ��ݗʂ��O�`�P�O�ŋ��߂�B����ȏ�͑SSP�Ŗ��߂邾��
�v�Z�o���Ȃ����H�Ƃ肠�����X�y�[�X�݂̂Ŗ��߂āA���p�������悤�Ȃ�
�h�b�g�t���Ă�蒼���Ƃ��B�������[�v�ɒ���

��L��A�ɗ͑S�p���pSP���g���A�A���������p������������A�ǂ����Ă�����Ȃ�
�悤�Ȃ�A���j�R�[�h�̓���X�y�[�X���g���āA�s���I�h��A�����p���Ȃ����悤�ɁH

��������
��h�b�g�����߂āA�����ɗp����X�y�[�X�G���A�ɃJ�[�\�������Ă�
���̍s�̃Y�����m�F���ĕK�v�Ȃ��炵���s���B
���̂Ƃ��̓��j�R�[�h���g������A�s���I�h���g�����肵�ĘA�����p��ǂ��o��
��ʒu��RULER�ɐԂŎ����Ƃ�

�X�y�[�X�p�f�B���O�̂�����
�E�A�����p
�E�s���I�h
�E���j�R�[�h
*/

//-------------------------------------------------------------------------------------------------


extern FILES_ITR	gitFileIt;	//!<	�����Ă�t�@�C���̖{��
extern INT		gixFocusPage;	//!<	���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X

extern  UINT	gbUniPad;		//!<	�p�f�B���O�Ƀ��j�R�[�h�������āA�h�b�g�������Ȃ��悤�ɂ���

static INT		gdDiffLock;		//!<	���꒲���̊�h�b�g�l
//-------------------------------------------------------------------------------------------------

//	�E�����p�󔒃p���[��
CONST  TCHAR	gaatDotPtrnPeriod[11][9] = {
	{ TEXT("�@�@") },		//	22	0
	{ TEXT("�@....") },		//	23	1
	{ TEXT(" �@ .") },		//	24	2
	{ TEXT("�@�@.") },		//	25	3
	{ TEXT("�@.....") },	//	26	4
	{ TEXT("�@�@ ") },		//	27	5
	{ TEXT("�@�@..") },		//	28	6
	{ TEXT("�@......") },	//	29	7
	{ TEXT("�@�@ .") },		//	30	8
	{ TEXT("�@�@...") },	//	31	9
	{ TEXT("�@ �@ ") }		//	32	10
};
#define RIGHT_WALL	TEXT('|')	//	�E�����p�Ǖ���

//	�E�����p�󔒃p���[�����j�R�[�h	20120315
CONST  TCHAR	gaatDotPtrnUnic[11][6] = {
	{ TEXT("�@�@") },				//	22	0	�SSP�@�SSP
	{ 0x3000,0x3000,0x200A },		//	23	1	�SSP�@�SSP�@1dot
	{ 0x3000,0x3000,0x2009 },		//	24	2	�SSP�@�SSP�@2dot
	{ 0x3000,0x3000,0x2006 },		//	25	3	�SSP�@�SSP�@3dot
	{ 0x3000,0x3000,0x2005 },		//	26	4	�SSP�@�SSP�@4dot
	{ TEXT("�@�@ ") },				//	27	5	�SSP�@�SSP�@��SP
	{ 0x3000,0x0020,0x3000,0x200A },//	28	6	�SSP�@��SP�@�SSP�@1dot
	{ 0x3000,0x0020,0x3000,0x2009 },//	29	7	�SSP�@��SP�@�SSP�@2dot
	{ 0x3000,0x0020,0x3000,0x2006 },//	30	8	�SSP�@��SP�@�SSP�@3dot
	{ 0x3000,0x0020,0x3000,0x2005 },//	31	9	�SSP�@��SP�@�SSP�@4dot
	{ TEXT("�@ �@ ") }				//	32	10	�SSP�@��SP�@�SSP�@��SP
};



//	�p�f�B���O�p�󔒃p���[���E�񃆃j�R�[�h
CONST TCHAR gaatPaddingSpDot[34][9] = {
	{ TEXT("")    },		//	0
	{ TEXT(".")   },		//	1	3
	{ TEXT(".")   },		//	2	3
	{ TEXT(".")   },		//	3	3
	{ TEXT(".")   },		//	4	3
	{ TEXT(" ")   },		//	5		TEXT("..")	6
	{ TEXT("..")  },		//	6
	{ TEXT("..")  },		//	7	6
	{ TEXT(". ")  },		//	8
	{ TEXT("...") },		//	9
	{ TEXT("�@")  },		//	10	11
	{ TEXT("�@")  },		//	11
	{ TEXT("�@")  },		//	12	11	{ TEXT("....") }
	{ TEXT(".�@") },		//	13	14
	{ TEXT(".�@") },		//	14
	{ TEXT("�@ ") },		//	15	16	{ TEXT(".....") }
	{ TEXT("�@ ")  },		//	16
	{ TEXT(".�@.") },		//	17
	{ TEXT(". �@") },		//	18	19	{ TEXT("......") }
	{ TEXT(". �@") },		//	19
	{ TEXT("..�@.") },		//	20
	{ TEXT(" �@ ") },		//	21	{ TEXT(".......") }
	{ TEXT("�@�@") },		//	22
	{ TEXT("�@�@")  },		//	23	22	{ TEXT("..�@..") }
	{ TEXT("�@ . ") },		//	24
	{ TEXT(".�@�@") },		//	25
	{ TEXT("�@ �@") },		//	26	27	{ TEXT("...�@..") }
	{ TEXT("�@ �@") },		//	27
	{ TEXT(".�@�@.")  },	//	28
	{ TEXT(".�@ �@")  },	//	29	30	{ TEXT("...�@...") }
	{ TEXT(".�@ �@")  },	//	30
	{ TEXT(".�@.�@.") },	//	31
	{ TEXT("�@ �@ ")  },	//	32
	{ TEXT("�@�@�@") }		//	33
};

//	�p�f�B���O�p�󔒃p���[���E���j�R�[�h
CONST TCHAR gaatPaddingSpDotW[11][3] = {
	{ TEXT("") },		//	0	0	0
	{ 8202 },			//	1	1	0
	{ 8201 },			//	2	1	0
	{ 8198 },			//	3	1	0
	{ 8197 },			//	4	1	0
	{ TEXT(' ') },		//	5	0	1
	{ 8202, TEXT(' ') },//	6	1	1
	{ 8201, TEXT(' ') },//	7	1	1
	{ 8194 },			//	8	1	0
	{ 8197, TEXT(' ') },//	9	1	1
	{ 8199 }			//	10	1	0
};						//		U	H

//-------------------------------------------------------------------------------------------------


UINT	SpaceWidthAdjust( INT, PINT, PINT );	//!<	
LPTSTR	SpaceStrAlloc( INT, INT );	//!<	

UINT	DocSpaceDifference( UINT, PINT, INT, UINT );	//!<	

HRESULT	DocRightGuideSet( INT, INT );	//!<	

LPTSTR	DocPaddingSpace( INT , PINT, PINT );	//!<	
//-------------------------------------------------------------------------------------------------


/*!
	�g���X�y�[�X�̐����A���Z���Ē���
	@param[in]		dDot	�ڕW�h�b�g��
	@param[in,out]	pZen	�S�p�r�o�����󂯂āA������������Ԃ�
	@param[in,out]	pHan	���p�r�o�����󂯂āA������������Ԃ�
	@return			UINT	��O�����n�j�E�O���s
*/
UINT SpaceWidthAdjust( INT dDot, PINT pZen, PINT pHan )
{
	INT		dZen, dHan, size;

	dZen = *pZen;
	dHan = *pHan;

	do
	{
		size = (dZen * SPACE_ZEN) + (dHan * SPACE_HAN);

		if( dDot == size )
		{
			*pZen = dZen;
			*pHan = dHan;
			return 1;
		}

		dZen--;		//	�SSP:11dot�A��SP:5dot�Ȃ̂ŁA
		dHan += 2;	//	�SSP���炵�Ĕ�SP�󑝂₷��1dot�k��
	}
	while(  0 <= dZen );	//	�SSP�������Ȃ�ƃA�E�g

	return 0;
}
//-------------------------------------------------------------------------------------------------

/*!
	�S�p�X�y�[�X�A���p�X�y�[�X�̌����󂯂āA������������ĕԂ�
	@param[in]	dZen	�S�p�X�y�[�X�̌�
	@param[in]	dHan	���p�X�y�[�X�̌�
	@return		LPTSTR	�쐬�����X�y�[�X�̕�����
*/
LPTSTR SpaceStrAlloc( INT dZen, INT dHan )
{
	INT		cchSize, i;
	LPTSTR	ptStr;

	cchSize = dZen + dHan;	//	�K�v��	��NULL���݂˂������₷
	ptStr = (LPTSTR)malloc( (cchSize + 1) * sizeof(TCHAR) );
	if( !ptStr )	return NULL;
	ZeroMemory( ptStr, (cchSize + 1) * sizeof(TCHAR) );

	for( i = (cchSize - 1); 0 <= i; )
	{
		if( 0 < dHan )
		{
			ptStr[i--] = TEXT(' ');
			dHan--;
			if( 0 >  i )	break;
		}

		if( 0 < dZen )
		{
			ptStr[i--] = TEXT('�@');
			dZen--;
			if( 0 >  i )	break;
		}
	}

	return ptStr;
}
//-------------------------------------------------------------------------------------------------

/*!
	�h�b�g�����󂯂āA�����Ɏ��܂�悤�ȃX�y�[�X�̑g�ݍ��킹���v�Z�E���j�R�[�h�󔒂��g��
	@param[in]	dTgtDot	�쐬����h�b�g��
	@param[out]	pdZenSp	�g�p�����S�p�X�y�[�X�̌������ENULL�ł��n�j
	@param[out]	pdHanSp	�g�p�������p�X�y�[�X�̌������ENULL�ł��n�j
	@param[out]	pdUniSp	�g�p�������j�R�[�h�̌������ENULL�ł��n�j
	@return		LPTSTR	�쐬�����X�y�[�X�̕�����E�J���͌Ă񂾕��Ŗʓ|����
*/
LPTSTR DocPaddingSpaceUni( INT dTgtDot, PINT pdZenSp, PINT pdHanSp, PINT pdUniSp )
{
	INT		dZen, dHan, dUni;
	INT		iCnt, iRem;
	INT		cchSize, i;
	LPTSTR	ptStr = NULL;

	//	���O���ƍ��Ȃ�
	if( 0 >= dTgtDot )	return NULL;

	iCnt =  dTgtDot / SPACE_ZEN;	//	�S�p�ŏo���邾�����߂�
	iRem =  dTgtDot % SPACE_ZEN;	//	�]�邩�H

	dZen = iCnt;	//	�Ƃ肠�����K�v��

	switch( iRem )	//	���ꂼ��̕K�v���m��
	{
		case  1:	dUni = 1;	dHan = 0;	break;
		case  2:	dUni = 1;	dHan = 0;	break;
		case  3:	dUni = 1;	dHan = 0;	break;
		case  4:	dUni = 1;	dHan = 0;	break;
		case  5:	dUni = 0;	dHan = 1;	break;
		case  6:	dUni = 1;	dHan = 1;	break;
		case  7:	dUni = 1;	dHan = 1;	break;
		case  8:	dUni = 1;	dHan = 0;	break;
		case  9:	dUni = 1;	dHan = 1;	break;
		case 10:	dUni = 1;	dHan = 0;	break;
		default:	dUni = 0;	dHan = 0;	break;
	}

	cchSize = dZen + dHan + dUni;	//	�K�v��	��NULL���݂˂������₷
	ptStr = (LPTSTR)malloc( (cchSize + 1) * sizeof(TCHAR) );
	if( !ptStr )	return NULL;
	ZeroMemory( ptStr, (cchSize + 1) * sizeof(TCHAR) );

	for( i = 0; dZen > i; i++ ){	ptStr[i] = TEXT('�@');	}

	switch( iRem )	//	�e������ǉ�
	{
		case  1:	ptStr[i++] = gaatPaddingSpDotW[1][0];	break;
		case  2:	ptStr[i++] = gaatPaddingSpDotW[2][0];	break;
		case  3:	ptStr[i++] = gaatPaddingSpDotW[3][0];	break;
		case  4:	ptStr[i++] = gaatPaddingSpDotW[4][0];	break;
		case  5:	ptStr[i++] = gaatPaddingSpDotW[5][0];	break;
		case  6:	ptStr[i++] = gaatPaddingSpDotW[6][0];	ptStr[i++] = gaatPaddingSpDotW[6][1];	break;
		case  7:	ptStr[i++] = gaatPaddingSpDotW[7][0];	ptStr[i++] = gaatPaddingSpDotW[7][1];	break;
		case  8:	ptStr[i++] = gaatPaddingSpDotW[8][0];	break;
		case  9:	ptStr[i++] = gaatPaddingSpDotW[9][0];	ptStr[i++] = gaatPaddingSpDotW[9][1];	break;
		case 10:	ptStr[i++] = gaatPaddingSpDotW[10][0];	break;
		default:	break;
	}

	if( pdZenSp )	*pdZenSp = dZen;
	if( pdHanSp )	*pdHanSp = dHan;
	if( pdUniSp )	*pdUniSp = dUni;

	return ptStr;
}
//-------------------------------------------------------------------------------------------------

/*!
	�h�b�g�����󂯂āA�����Ɏ��܂�悤�ȃX�y�[�X�̑g�ݍ��킹���v�Z
	@param[in]	dTgtDot	�쐬����h�b�g��
	@param[out]	pdZenSp	�g�p�����S�p�X�y�[�X�̌������ENULL�ł��n�j
	@param[out]	pdHanSp	�g�p�������p�X�y�[�X�̌������ENULL�ł��n�j
	@return		LPTSTR	�쐬�����X�y�[�X�̕�����E�J���͌Ă񂾕��Ŗʓ|����E�쐬�s�Ȃ�NULL
*/
LPTSTR DocPaddingSpace( INT dTgtDot, PINT pdZenSp, PINT pdHanSp )
{
	INT		dZen, dHan;
	INT		iCnt, iRem;
	UINT	dRslt;
	LPTSTR	ptStr = NULL;

	//	���O���ƍ��Ȃ�
	if( 0 >= dTgtDot )	return NULL;

	iCnt =  dTgtDot / SPACE_ZEN;	//	�S�p�ŏo���邾�����߂�
	iRem =  dTgtDot % SPACE_ZEN;	//	�]�邩�H

	dZen = iCnt;	//	�Ƃ肠�����K�v��

	if( 1 <= iRem && iRem <= 5 )	//	���p�Ŗ��߂�
	{
		dHan = 1;
	}
	else if( 6 <= iRem && iRem <= 10 )	//	�S�p�Ŗ��߂�
	{
		dHan = 0;
		dZen++;
	}
	else	//	�҂����肾����
	{
		dHan = 0;
	}

	//	������
	dRslt = SpaceWidthAdjust( dTgtDot, &dZen, &dHan );

	if( dRslt )
	{
		if( pdZenSp )	*pdZenSp = dZen;
		if( pdHanSp )	*pdHanSp = dHan;

		//	�������m�ۂ��ĕ�������
		ptStr = SpaceStrAlloc( dZen, dHan );

		return ptStr;
	}

	return NULL;
}
//-------------------------------------------------------------------------------------------------

/*!
	�h�b�g���𑽏��O�サ�Ă��������疄�߂�
	@param[in]	dTgtDot	�쐬����h�b�g��
	@param[out]	pdZenSp	�g�p�����S�p�X�y�[�X�̌������ENULL�ł��n�j
	@param[out]	pdHanSp	�g�p�������p�X�y�[�X�̌������ENULL�ł��n�j
	@return		LPTSTR	�쐬�����X�y�[�X�̕�����E�J���͌Ă񂾕��Ŗʓ|����E�쐬�s�Ȃ�NULL
*/
LPTSTR DocPaddingSpaceWithGap( INT dTgtDot, PINT pdZenSp, PINT pdHanSp )
{
	INT		cchSize, i;
	LPTSTR	ptStr = NULL;

	if( 16 <= dTgtDot )	//	���𑝂₵�Ȃ�����܂�͈͂�������
	{
		i = 0;

		do
		{
			if( 22 < i )	return NULL;	//	�������[�v�j�~�B���v�Ǝv�����ǁB

			ptStr = DocPaddingSpace( dTgtDot, pdZenSp, pdHanSp );
			dTgtDot++;	i++;

		}while( !(ptStr) );

		return ptStr;
	}


	//	�G���A������������̂ŗ�O
	cchSize = 1;	//	�K�v��	��NULL���݂˂������₷
	ptStr = (LPTSTR)malloc( (cchSize + 1) * sizeof(TCHAR) );
	if( !ptStr )	return NULL;

	if( 7 >= dTgtDot )	//	���p��Ŗڂ��Ԃ�
	{
		ptStr[0] = TEXT(' ');
		if( pdZenSp )	*pdZenSp = 0;
		if( pdHanSp )	*pdHanSp = 1;
	}
	else if( 8 <= dTgtDot && dTgtDot <= 15 )	//	�S�p��ł��܂���
	{
		ptStr[0] = TEXT('�@');
		if( pdZenSp )	*pdZenSp = 1;
		if( pdHanSp )	*pdHanSp = 0;
	}
	ptStr[1] = 0x0000;

	return ptStr;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��h�b�g�ʒu���܂܂�Ă���A�X�y�[�X����X�y�[�X�̕����̎n�_�I�_�h�b�g���m��
	@param[in]	dDot	�w��h�b�g�E����̍��̕����Ŕ��f
	@param[in]	rdLine	�s��
	@param[out]	pLeft	�n�_�h�b�g�E�s������
	@param[out]	pRight	�I�_�h�b�g�E�s������
	@param[out]	pStCnt	�J�n�n�_�̕������ENULL�ł���
	@param[out]	pCount	�Ԃ̕������ENULL�ł���
	@param[out]	pIsSp	�Y���̓X�y�[�X�ł��邩�H
	@return		�Y���͈͂̃h�b�g��
*/
INT DocLineStateCheckWithDot( INT dDot, INT rdLine, PINT pLeft, PINT pRight, PINT pStCnt, PINT pCount, PBOOLEAN pIsSp )
{
	UINT_PTR	iCount;
	INT		bgnDot, endDot;
	INT		iBgnCnt, iRngCnt;
	TCHAR	ch, chb;
	UINT	dMozis;
	INT		bSpace;
	LETR_ITR	itMozi, itHead, itTail, itTemp;

	LINE_ITR	itLine;

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, rdLine );

	if( !(pLeft) || !(pRight) || !(pIsSp) ){	return 0;	}

	itMozi = itLine->vcLine.begin( );
	iCount = itLine->vcLine.size( );	//	���̍s�̕������m�F

	//	���g�������Ȃ�G���[
	if( 0 >= iCount ){	*pIsSp =  FALSE;	*pLeft =  0;	*pRight = 0;	return 0;	}

	dMozis = DocLetterPosGetAdjust( &dDot , rdLine, 0 );	//	���̕����ʒu���m�F

	if( 1 <= dMozis ){	itMozi += (dMozis-1);	}	//	�L�����b�g�̈ʒu�̍������Ŕ���
	//	�ŏ�����擪�Ȃ�Ȃɂ����Ȃ��Ă���
	ch = itMozi->cchMozi;
	bSpace = iswspace( ch );


	//	���̏ꏊ���瓪�����ɒH���āA�r�؂�ڂ�T��
	itHead = itLine->vcLine.begin( );

	for( ; itHead != itMozi; itMozi-- )
	{
		chb = itMozi->cchMozi;
		if( iswspace( chb ) != bSpace ){	itMozi++;	break;	}
	}
	//	�擪�܂ŃC�b��������ꍇ�E���ꂪ�����Ă�
	if( itHead == itMozi )
	{
		chb = itMozi->cchMozi;
		if( iswspace( chb ) != bSpace ){	itMozi++;	}
	}
	//	��ƈقȂ镶���Ƀq�b�g�������A�擪�ʒu�ł���

	//	�擪����A�q�b�g�ʒu�܂ŒH���ăh�b�g���ƕ������m�F
	bgnDot = 0;
	iBgnCnt = 0;
	for( itTemp = itHead; itTemp != itMozi; itTemp++ )
	{
		bgnDot += itTemp->rdWidth;	//	���������₵��
		iBgnCnt++;	//	�����������₷
	}//�����ŏ�����擪�Ȃ痼���O�̂܂�

	itTail = itLine->vcLine.end( );

	//	���̏ꏊ����A�����O���[�v�̏��܂Ŋm�F
	endDot = bgnDot;
	iRngCnt = 0;
	for( ; itTemp != itTail; itTemp++ )
	{
		chb = itTemp->cchMozi;	//	�����^�C�v�ł���ԉ��Z������
		if( iswspace( chb ) != bSpace ){	break;	}

		endDot += itTemp->rdWidth;
		iRngCnt++;
	}

	*pLeft  = bgnDot;
	*pRight = endDot;
	*pIsSp  = bSpace ? TRUE : FALSE;

	if( pCount )	*pCount = iRngCnt;
	if( pStCnt )	*pStCnt = iBgnCnt;

	return (endDot - bgnDot);
}
//-------------------------------------------------------------------------------------------------

/*!
	���݂̃h�b�g�ʒu���܂�ł���󔒃G���A���P�h�b�g�����炷
	@param[in]	vk		�����E�E������
	@param[in]	pXdot	���̃h�b�g�ʒu���󂯂Ė߂�
	@param[in]	dLine	���̍s��
	@param[in]	dFirst	�A���h�D�O���[�v�E��O�Ȃ�ŏ��̈ꔭ�@�O����
	@return	UINT	��O�Y���l�@�O���s
*/
UINT DocSpaceDifference( UINT vk, PINT pXdot, INT dLine, UINT dFirst )
{
	INT			dTgtDot, dNowDot;
	INT			dBgnDot, dEndDot;
	INT			dBgnCnt, dRngCnt;
	UINT_PTR	cchSize;
	BOOLEAN		bIsSpace;
	LPTSTR		ptSpace;//, ptOldSp;
	INT			dZenSp, dHanSp, dUniSp;
	INT			iDots, iBytes;

	wstring		wsBuffer;
	LETR_ITR	vcLtrBgn, vcLtrEnd, vcItr;

	LINE_ITR	itLine;

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, dLine );

	dNowDot = *pXdot;

	if( 0 == dNowDot )	//	�O�̏ꍇ�͋����Ɉړ�
	{
		dNowDot = itLine->vcLine.at( 0 ).rdWidth;
	}

	dTgtDot = DocLineStateCheckWithDot( dNowDot, dLine, &dBgnDot, &dEndDot, &dBgnCnt, &dRngCnt, &bIsSpace );
	if( !(bIsSpace) )	return 0;	//	��X�y�[�X�G���A�͈Ӗ�������

	if( VK_RIGHT == vk )		dTgtDot++;	//	�E�Ȃ瑝�₷���Ă���
	else if( VK_LEFT == vk )	dTgtDot--;
	else	return 0;	//	�֌W�Ȃ��̂̓A�E�c

	//	���Ă͂߂�A�����v�Z����
	ptSpace = DocPaddingSpace( dTgtDot, &dZenSp, &dHanSp );
	if( gbUniPad )
	{
		//	�쐬�s�������蔼�p����������A���j�R�[�h�g���č�蒼��
		if( !(ptSpace) || (dZenSp < dHanSp) )	//	(dZenSp + 1)
		{
			FREE(ptSpace);
			ptSpace = DocPaddingSpaceUni( dTgtDot, &dZenSp, &dHanSp, &dUniSp );
		}
	}

	if( !(ptSpace) )	return 0;	//	�쐬�s�������ꍇ


	StringCchLength( ptSpace, STRSAFE_MAX_CCH, &cchSize );

	vcLtrBgn  = itLine->vcLine.begin( );
	vcLtrBgn += dBgnCnt;	//	�Y���ʒu�܂ňړ�����
	vcLtrEnd  = vcLtrBgn;
	vcLtrEnd += dRngCnt;	//	���̃G���A�̏I�[���m�F

	iDots = 0;	iBytes = 0;
	wsBuffer.clear();
	for( vcItr = vcLtrBgn; vcLtrEnd != vcItr; vcItr++ )
	{
		wsBuffer += vcItr->cchMozi;
		iDots    += vcItr->rdWidth;
		iBytes   += vcItr->mzByte;
	}

	//	�Y����������U�폜�E�A���h�D���h�D����Ȃ���e���L�^����K�v������
	itLine->vcLine.erase( vcLtrBgn, vcLtrEnd );
	itLine->iByteSz -= iBytes;	if( 0 > itLine->iByteSz ){	itLine->iByteSz = 0;	}
	itLine->iDotCnt -= iDots;	if( 0 > itLine->iDotCnt ){	itLine->iDotCnt = 0;	}

//	DocLineParamGet( yLine, NULL, NULL );	//	�s���e�̍Čv�Z
//���Ƃ̔������ŌĂ΂�܂����Ă�

	//	Space�������ǉ�
	dNowDot = dBgnDot;
	DocStringAdd( &dNowDot, &dLine, ptSpace, cchSize );

	*pXdot = dNowDot;

//	cchSize = wsBuffer.size( ) + 1;
//	ptOldSp = (LPTSTR)malloc( cchSize * sizeof(TCHAR) );
//	StringCchCopy( ptOldSp, cchSize, wsBuffer.c_str( ) );

	SqnAppendString( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), DO_DELETE, wsBuffer.c_str( ), dBgnDot, dLine, dFirst );
	SqnAppendString( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), DO_INSERT, ptSpace, dBgnDot, dLine, FALSE );	//	���ڂȂ̂Ŋm��ł�낵

//	FREE( ptOldSp );

	FREE( ptSpace );

	return dTgtDot;
}
//-------------------------------------------------------------------------------------------------

/*!
	���݂̃h�b�g�ʒu���܂�ł���󔒃G���A���P�h�b�g���炷�V�[�P���X
	@param[in]	vk		�����E�E������
	@param[in]	pXdot	���̃h�b�g�ʒu���󂯂Ė߂�
	@param[in]	dLine	���̍s��
	@return	INT	��O�Y���l�@�O���s
*/
INT DocSpaceShiftProc( UINT vk, PINT pXdot, INT dLine )
{
	INT		dDot, dMozi, dPreByte;

	//	20110720	�O�����ő��삷��Ƃ��ځ[�񂷂�̂Ŋm�F���Ă���
	dDot = DocLineParamGet( dLine, &dMozi, &dPreByte );
	if( 0 >= dMozi )	return 0;

	dDot = DocSpaceDifference( vk, pXdot, dLine, TRUE );

	DocLetterPosGetAdjust( pXdot, dLine, 0 );	//	���̒���DocLineParamGet�Ńo�C�g�����v�Z����Ă�

	ViewRedrawSetLine( dLine );

	ViewDrawCaret( *pXdot, dLine, 1 );

	return dDot;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�������̖ʓ|����
	@param[in]	pVoid	�Ȃɂ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT	DocRightGuideline( LPVOID pVoid )
{
	INT	iTop, iBottom, i;

	TRACE( TEXT("�E������") );

	iTop    = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop;
	iBottom = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom;

	DocRightGuideSet( iTop, iBottom );

	ViewSelPageAll( -1 );	//	�I��͈͖����Ȃ�

	if( 0 > iTop || 0 > iBottom ){	ViewRedrawSetLine( -1 );	}
	else{	for( i =  iTop; iBottom >= i; i++ ){	ViewRedrawSetLine(  i );	}	}


	DocPageInfoRenew( -1, 1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��͈͂ɉE��������t����
	@param[in]	dTop	�J�n�s�E�܂ށE�|�P�ōŏ�����
	@param[in]	dBottom	�I���s�E�܂ށE�|�P�ōŌ�܂�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocRightGuideSet( INT dTop, INT dBottom )
{
	//	�������I�������A�Ă񂾕��őI��͈͂̉����Ɖ�ʍX�V���邱��

	UINT_PTR	iLines, cchSize;
	INT			baseDot, i, j, iMz, nDot, sDot, lDot, iUnt, iPadot;
	TCHAR		ch, atBuffer[MAX_PATH];
	LPTSTR		ptBuffer;
	BOOLEAN		bFirst;
	wstring		wsBuffer;

	//	�͈͊m�F
	iLines = DocNowFilePageLineCount( );
	if( 0 > dTop )		dTop = 0;
	if( 0 > dBottom )	dBottom = iLines - 1;

	//	
	ZeroMemory( atBuffer, sizeof(atBuffer) );
	atBuffer[0] = RIGHT_WALL;
	InitParamString( INIT_LOAD, VS_RGUIDE_MOZI, atBuffer );

	//	��Ԓ����ƂƂ�����m�F
	baseDot = DocPageMaxDotGet( dTop, dBottom );

	bFirst = TRUE;
	//	�e�s���ɒǉ����銴����
	for( i = dTop; dBottom >= i; i++ )
	{
		nDot = DocLineParamGet( i , NULL, NULL );	//	�Ăяo���Β��Ŗʓ|�݂Ă����
		sDot = baseDot - nDot;
		iUnt = sDot / SPACE_ZEN;	//	���߂镪
		sDot = sDot % SPACE_ZEN;	//	�͂ݏo���h�b�g�m�F
		//	�ϐ��g���񂵒���

		iPadot = nDot;
		wsBuffer.clear( );	//	�A���h�D�o�b�t�@�p�L�^

		for( j = 0; iUnt > j; j++ )
		{
			ch = TEXT('�@');	//	�����̂͑S�p�󔒊m��
			wsBuffer += ch;
			lDot  = DocInputLetter( nDot, i, ch );
			nDot += lDot;
		}

		//	20120315	���j�R�[�h���[�h�Ȃ��ɂ�ɂ��Ƃ���
		if( gbUniPad  ){	iMz = lstrlen( gaatDotPtrnUnic[sDot]  );	}
		else{				iMz = lstrlen( gaatDotPtrnPeriod[sDot] );	}

		//	�������܂ł̋󔒂𖄂߂�
		for( j = 0; iMz > j; j++ )
		{
			if( gbUniPad  ){	ch = gaatDotPtrnUnic[sDot][j];	}	//	20120315
			else{			ch = gaatDotPtrnPeriod[sDot][j];	}

			wsBuffer += ch;
			lDot  = DocInputLetter( nDot, i, ch );
			nDot += lDot;
		}

		//	�������[�������ꍞ��
		wsBuffer += atBuffer[0];
		lDot  = DocInputLetter( nDot, i, atBuffer[0] );
		nDot += lDot;

		DocBadSpaceCheck( i );	//	�����ŋ󔒃`�F�L

		//	���ꂽ�����𓝍����ăA���h�D�o�b�t�@�����O
		cchSize = wsBuffer.size( ) + 1;
		ptBuffer = (LPTSTR)malloc( cchSize * sizeof(TCHAR) );
		StringCchCopy( ptBuffer, cchSize, wsBuffer.c_str(  ) );

		SqnAppendString( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), DO_INSERT, ptBuffer, iPadot, i, bFirst );
		bFirst = FALSE;

		FREE( ptBuffer );
	}


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���꒲���p�̊�h�b�g�l���Z�b�g
	@param[in]	yLine	�Ώۍs
	@return		INT		�ݒ肵���h�b�g�l
*/
INT DocDiffAdjBaseSet( INT yLine )
{
	INT	dDot = 0;
	TCHAR	atMessage[MAX_STRING];

	TRACE( TEXT("����������_���b�N�I��") );

	dDot = DocLineParamGet( yLine, NULL, NULL );

	gdDiffLock = dDot;

	StringCchPrintf( atMessage, MAX_STRING, TEXT("������ʒu�� %d �h�b�g�ɐݒ肵���ł���"), dDot );
	NotifyBalloonExist( atMessage, TEXT("���b�N�I��"), NIIF_INFO );

	return dDot;
}
//-------------------------------------------------------------------------------------------------

/*!
	���꒲�������s����
	@param[in]	pxDot	�����ʒu
	@param[in]	yLine	�Ώۍs
	@return		INT		���������h�b�g��
*/
INT DocDiffAdjExec( PINT pxDot, INT yLine )
{
	INT			dMotoDot = 0;
	INT			dBgnDot, dEndDot, dBgnCnt, dRngCnt, iSabun, dTgtDot, nDot;
	UINT_PTR	cchSize, cchPlus;
	BOOLEAN		bIsSpace;
	LPTSTR		ptPlus, ptBuffer;

	wstring		wsDelBuf, wsAddBuf;
	LETR_ITR	vcLtrBgn, vcLtrEnd, vcItr;

	LINE_ITR	itLine;

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, yLine );

	//	�����l�̏󋵂��m�F
	dTgtDot = DocLineStateCheckWithDot( *pxDot, yLine, &dBgnDot, &dEndDot, &dBgnCnt, &dRngCnt, &bIsSpace );
	if( !(bIsSpace) )
	{
		NotifyBalloonExist( TEXT("�A������X�y�[�X�̕����ɃJ�[�\���𓖂Ă�ł���"), TEXT("�����s�\"), NIIF_ERROR );
		return 0;
	}
	//�����݂̋󔒕����m�F


	//	�Ώۍs�̒������m�F
	dMotoDot = DocLineParamGet( yLine, NULL, NULL );
	iSabun = gdDiffLock - dMotoDot;	//	�����m�F�E�}�C�i�X�Ȃ�͂ݏo���Ă�

//�܂��S�p���p�Ŗ��߂āA���p�������悤�Ȃ�s���I�h�t���čČv�Z

	dTgtDot += iSabun;	//	�ύX��̃h�b�g��

	if( 41 > dTgtDot )	//	���j�R�[�h�g���Ȃ�m�F���Ȃ��Ă����v�H
	{
		NotifyBalloonExist( TEXT("�������������Ȃ��ƒ����ł��Ȃ��ł���"), TEXT("������"), NIIF_ERROR );
		return 0;
	}

	//���ߕ�����쐬
	ptPlus = DocPaddingSpaceWithPeriod( dTgtDot, NULL, NULL, NULL, FALSE );

	if( !(ptPlus) )
	{
		NotifyBalloonExist( TEXT("�����o���Ȃ������ł���"), TEXT("�����������s"), NIIF_ERROR );
		return 0;
	}

	StringCchLength( ptPlus, STRSAFE_MAX_CCH, &cchPlus );


	vcLtrBgn  = itLine->vcLine.begin( );
	vcLtrBgn += dBgnCnt;	//	�Y���ʒu�܂ňړ�����
	vcLtrEnd  = vcLtrBgn;
	vcLtrEnd += dRngCnt;	//	���̃G���A�̏I�[���m�F

	wsDelBuf.clear();
	for( vcItr = vcLtrBgn; vcLtrEnd != vcItr; vcItr++ ){	wsDelBuf +=  vcItr->cchMozi;	}

	//	�Y���������폜
	itLine->vcLine.erase( vcLtrBgn, vcLtrEnd );
	nDot = dBgnDot;

	cchSize = wsDelBuf.size( ) + 1;
	ptBuffer = (LPTSTR)malloc( cchSize * sizeof(TCHAR) );
	StringCchCopy( ptBuffer, cchSize, wsDelBuf.c_str( ) );
	SqnAppendString( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), DO_DELETE, ptBuffer, dBgnDot, yLine, TRUE );
	FREE( ptBuffer );

//�����ŕ�����ǉ�
	DocStringAdd( &nDot, &yLine, ptPlus, cchPlus );
	SqnAppendString( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), DO_INSERT, ptPlus, dBgnDot, yLine, FALSE );
	FREE(ptPlus);

//�������̈ʒu���킹���Ă���
	*pxDot = nDot;

	DocLetterPosGetAdjust( pxDot, yLine, 0 );

	ViewRedrawSetLine( yLine );

	ViewDrawCaret( *pxDot, yLine, 1 );

	return iSabun;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w�肳�ꂽ�h�b�g�����A�s���I�h���g�����Y��Ɋm�ۂ���E�P�X���܂łȂ璲���ł���E����͂���ŕK�v
	@param[in]	dTgtDot	�쐬����h�b�g��
	@param[out]	pdZen	�g�p�����S�p�X�y�[�X�̌������ENULL�ł��n�j
	@param[out]	pdHan	�g�p�������p�X�y�[�X�̌������ENULL�ł��n�j
	@param[out]	pdPrd	�g�p�����s���I�h�̌������ENULL�ł��n�j
	@param[in]	bFull	�Œ�e�[�u�������ċ����ɂ��킹��H
	@return		LPTSTR	�쐬�����X�y�[�X�̕�����E�J���͌Ă񂾕��Ŗʓ|����E�쐬�s�Ȃ�NULL
*/
LPTSTR DocPaddingSpaceWithPeriod( INT dTgtDot, PINT pdZen, PINT pdHan, PINT pdPrd, BOOLEAN bFull )
{
	INT	dZenSp, dHanSp, dPrdSp, m, dPre;
	LPTSTR	ptSpace = NULL, ptPlus = NULL;
	UINT	cchSize, cchPlus;


	dPre = dTgtDot;
	dPrdSp = 0;

	do{
		dZenSp =  0;	dHanSp =  0;
		ptSpace = DocPaddingSpace( dTgtDot, &dZenSp, &dHanSp );

		//	�쐬�s�������ꍇ	���p�������Ă��s��
		if( !(ptSpace) || (dZenSp < dHanSp) )	//	(dZenSp + 1)
		{
			FREE(ptSpace);
			if( gbUniPad )	//	��肭�����Ȃ��悤�Ȃ�A���j�R�[�h�g���Ă�蒼��
			{
				ptSpace = DocPaddingSpaceUni( dTgtDot, &dZenSp, &dHanSp, NULL );
				break;
			}
			else
			{
				dPrdSp++;	dTgtDot -= 3;	//	�s���I�h�͂R�h�b�g
			}
		}
		else	//	��薳��������Ȃ炨��
		{
			break;
		}

	}while( dTgtDot >= 19 );	//	����ȏ�͕s�H

	if( !(ptSpace) && bFull )	//	�܂�����ł��ĂȂ��A�Œ�e�[�u���g���Ȃ�
	{
		dPrdSp = 0;
		dTgtDot = dPre;

		StringCchLength( gaatPaddingSpDot[dTgtDot], STRSAFE_MAX_CCH, &cchSize );

		cchSize += 1;
		ptSpace = (LPTSTR)malloc( cchSize * sizeof(TCHAR) );
		ZeroMemory( ptSpace, cchSize * sizeof(TCHAR) );

		StringCchCopy( ptSpace, cchSize, gaatPaddingSpDot[dTgtDot] );
	}

	if( ptSpace )
	{
		StringCchLength( ptSpace, STRSAFE_MAX_CCH, &cchSize );
		
		//	�s���I�h����ăT�C�Y����
		cchPlus = cchSize + dPrdSp + 1;
		ptPlus = (LPTSTR)malloc( cchPlus * sizeof(TCHAR) );
		ZeroMemory( ptPlus, cchPlus * sizeof(TCHAR) );

		StringCchCopy( ptPlus, cchPlus, ptSpace );
		FREE(ptSpace);	//	�X�y�[�X�����܂�

		//	�����s���I�h��������O��ɂ���悤�ɂ�����
		for( m = 0; dPrdSp > m; m++ ){	StringCchCat( ptPlus , cchPlus, TEXT(".") );	}
	}


	if( pdZen  )	*pdZen = dZenSp;
	if( pdHan  )	*pdHan = dHanSp;
	if( pdPrd  )	*pdPrd = dPrdSp;

	return ptPlus;
}
//-------------------------------------------------------------------------------------------------


/*!
	�s���ɁA����(��ɋ�)��ǉ�
	@param[in]	ch		�ǉ����镶��
	@param[in]	pXdot	���̃h�b�g�ʒu���󂯂Ė߂��E�폜�Ɋ������܂ꂽ�Ή�
	@param[in]	dLine	���̍s��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocTopLetterInsert( TCHAR ch, PINT pXdot, INT dLine )
{
	UINT_PTR	iLines;
	INT			iTop, iBottom, i, xDot = 0;
	BOOLEAN		bFirst = TRUE, bSeled = FALSE;

	TRACE( TEXT("�s���󔒂�ǉ�") );
	
	//	�͈͊m�F
	iLines  = DocNowFilePageLineCount( );
	iTop    = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop;
	iBottom = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom;
	if( 0 <= iTop &&  0 <= iBottom )	bSeled = TRUE;

	if( 0 > iTop )		iTop = 0;
	if( 0 > iBottom )	iBottom = iLines - 1;

	//	�I��͈͂́A���삵���s�S�̂�I����Ԃɂ���

	for( i = iTop; iBottom >= i; i++ )	//	�͈͓��̊e�s�ɂ���
	{
		//	�擪�ʒu�ɕ����������B
		xDot = DocInputLetter( 0, i, ch );

		SqnAppendLetter( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), DO_INSERT, ch, 0, i, bFirst );
		bFirst = FALSE;

		if( bSeled )
		{
			DocRangeSelStateToggle( -1, -1, i, 1 );	//	�Y���s�S�̂�I����Ԃɂ���
			DocReturnSelStateToggle( i, 1 );	//	���s���I����
		}

		DocBadSpaceCheck( i );
		ViewRedrawSetLine( i );
	}

	//	�L�����b�g�ʒu����Ă���K���ɒ���
	*pXdot += xDot;
	DocLetterPosGetAdjust( pXdot, dLine, 0 );	//	�L�����b�g�ʒu�K���ɒ���
	ViewDrawCaret( *pXdot, dLine, 1 );

	DocPageInfoRenew( -1, 1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�s���S�p�󔒋y�у��j�R�[�h�󔒂��폜����
	@param[in]	pXdot	���̃h�b�g�ʒu���󂯂Ė߂��E�폜�Ɋ������܂ꂽ�Ή�
	@param[in]	dLine	���̍s��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocTopSpaceErase( PINT pXdot, INT dLine )
{
	UINT_PTR	iLines;
	INT			iTop, iBottom, i;
	BOOLEAN		bFirst = TRUE, bSeled = FALSE;
	TCHAR		ch;

	LETR_ITR	vcLtrItr;

	LINE_ITR	itLine;

	//	�͈͊m�F
	iLines  = DocNowFilePageLineCount( );
	iTop    = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop;
	iBottom = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom;
	if( 0 <= iTop &&  0 <= iBottom )	bSeled = TRUE;

	if( 0 > iTop )		iTop = 0;
	if( 0 > iBottom )	iBottom = iLines - 1;

	TRACE( TEXT("�s���󔒂��폜") );
	//	�I��͈͂́A���삵���s�S�̂�I����Ԃɂ���

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, iTop );	//	�ʒu���킹

	for( i = iTop; iBottom >= i; i++, itLine++ )	//	�͈͓��̊e�s�ɂ���
	{
		//	����������Ȃ瑀�삷��
		if( 0 != itLine->vcLine.size(  ) )
		{
			vcLtrItr = itLine->vcLine.begin( );
			ch = vcLtrItr->cchMozi;

			//	�󔒂����p�ł͂Ȃ�
			if( ( iswspace( ch ) && TEXT(' ') != ch ) )
			{
				SqnAppendLetter( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), DO_DELETE, ch, 0, i, bFirst );
				bFirst = FALSE;

				DocIterateDelete( vcLtrItr, i );
			}
		}

		if( bSeled )
		{
			DocRangeSelStateToggle( -1, -1, i, 1 );	//	�Y���s�S�̂�I����Ԃɂ���
			DocReturnSelStateToggle( i, 1 );	//	���s���I����
		}

		DocBadSpaceCheck( i );	//	��Ԃ����Z�b�g
		ViewRedrawSetLine( i );
	}

	//	�L�����b�g�ʒu����Ă���K���ɒ���
	*pXdot = 0;
	DocLetterPosGetAdjust( pXdot, dLine, 0 );	//	�L�����b�g�ʒu�K���ɒ���
	ViewDrawCaret( *pXdot, dLine, 1 );


	DocPageInfoRenew( -1, 1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------
/*!
	�s���������폜����B�������󔒂�������폜���Ȃ�
	@param[in]	pXdot	���̃h�b�g�ʒu���󂯂Ė߂��E�폜�Ɋ������܂ꂽ�Ή�
	@param[in]	dLine	���̍s��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocLastLetterErase( PINT pXdot, INT dLine )
{
	UINT_PTR	iLines;
	INT			iTop, iBottom, i, xDot = 0;
	TCHAR		ch;
	BOOLEAN		bFirst = TRUE, bSeled = FALSE;
	RECT		rect;

	LETR_ITR	vcLtrItr;

	LINE_ITR	itLine;

	//	�͈͊m�F
	iLines  = DocNowFilePageLineCount( );
	iTop    = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop;
	iBottom = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom;
	if( 0 <= iTop &&  0 <= iBottom )	bSeled = TRUE;

	if( 0 > iTop )		iTop = 0;
	if( 0 > iBottom )	iBottom = iLines - 1;

	TRACE( TEXT("�s�������폜") );
	//	�I�����Ă�ꍇ�́A����s��S�I����Ԃɂ���
	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, iTop );	//	�ʒu���킹

	for( i = iTop; iBottom >= i; i++, itLine++ )	//	�͈͓��̊e�s�ɂ���
	{
		//	����������Ȃ瑀�삷��
		if( 0 != itLine->vcLine.size( ) )
		{
			vcLtrItr = itLine->vcLine.end( );
			vcLtrItr--;	//	�I�[�̈�O�����[����
			ch = vcLtrItr->cchMozi;

			rect.top    = i * LINE_HEIGHT;
			rect.bottom = rect.top + LINE_HEIGHT;

			if( !( iswspace( ch ) ) )
			{
				xDot  = DocLineParamGet( i, NULL, NULL );

				xDot -= vcLtrItr->rdWidth;

				SqnAppendLetter( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), DO_DELETE, ch, xDot, i, bFirst );
				bFirst = FALSE;

				DocIterateDelete( vcLtrItr, i );

				rect.left  = xDot;
				rect.right = xDot + 40;	//	�땶���{���s�E�K���ł�낵

				ViewRedrawSetRect( &rect );	//	���[��������������΂����H

				DocBadSpaceCheck( i );	//	�ǂ��Ȃ��X�y�[�X�𒲂ׂĂ���
			}
		}

		if( bSeled )
		{
			DocRangeSelStateToggle( -1, -1, i , 1 );	//	�Y���s�S�̂�I����Ԃɂ���
			DocReturnSelStateToggle( i, 1 );	//	���s���I����
		}
	}

	//	�L�����b�g�ʒu�K���ɒ���
	*pXdot = 0;
	DocLetterPosGetAdjust( pXdot, dLine, 0 );	//	�L�����b�g�ʒu�K���ɒ���
	ViewDrawCaret( *pXdot, dLine, 1 );

	DocPageInfoRenew( -1, 1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�s���󔒍폜�̖ʓ|����E�I���s�Ƃ�
	@param[in]	pXdot	���̃h�b�g�ʒu���󂯂Ė߂��E�폜�Ɋ������܂ꂽ�Ή�
	@param[in]	dLine	���̍s��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocLastSpaceErase( PINT pXdot, INT dLine )
{
	UINT_PTR	iLines;
	INT			iTop, iBottom, i, xDelDot, xMotoDot;
	BOOLEAN		bFirst = TRUE;
	LPTSTR		ptBuffer = NULL;
	RECT		rect;

	LINE_ITR	itLine;

	TRACE( TEXT("�s���󔒍폜") );

	//	�͈͊m�F
	iLines = DocNowFilePageLineCount( );
	iTop    = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop;
	iBottom = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom;

	if( 0 > iTop )		iTop = 0;
	if( 0 > iBottom )	iBottom = iLines - 1;


	ViewSelPageAll( -1 );	//	�I��͈͖����Ȃ�

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, iTop );	//	�ʒu���킹

	for( i = iTop; iBottom >= i; i++, itLine++ )
	{
		xMotoDot = itLine->iDotCnt;
		ptBuffer = DocLastSpDel( &(itLine->vcLine) );
		xDelDot  = DocLineParamGet( i, NULL, NULL );	//	�T�N������̍s���[���Ȃ킿�폜�ʒu
		
		if( ptBuffer  )
		{
			SqnAppendString( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), DO_DELETE, ptBuffer, xDelDot, i , bFirst );
			bFirst = FALSE;
		}

		FREE( ptBuffer );

		DocBadSpaceCheck( i );	//	��Ԃ����Z�b�g�E���ōs�����ł������H

		rect.top    = i * LINE_HEIGHT;
		rect.bottom = rect.top + LINE_HEIGHT;
		rect.left   = xDelDot;	//	������獶���ɂȂ�
		rect.right  = xMotoDot + 20;	//	�������{���s�}�[�N
		ViewRedrawSetRect( &rect );
//		ViewRedrawSetLine( i );	//	�ĕ`��COMMANDO
	}

	//	�L�����b�g�ʒu����Ă���K���ɒ���
	DocLetterPosGetAdjust( pXdot, dLine, 0 );	//	�L�����b�g�ʒu�K���ɒ���
	ViewDrawCaret( *pXdot, dLine, 1 );

	DocPageInfoRenew( -1, 1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�s���󔒍폜�E�R�A����
	@param[in]	*vcTgLine	�Y������s�̃x�N�^�[�ւ̃|�C���^�`
	@return		�폜����������E�Ă񂾕���free����
*/
LPTSTR DocLastSpDel( vector<LETTER> *vcTgLine )
{
	UINT_PTR	cchSize;
	LPTSTR		ptBuffer = NULL;
	wstring		wsDelBuf;
	LETR_ITR	itLtr, itDel;

	if( 0 >= vcTgLine->size( ) )	return NULL;

	itLtr = vcTgLine->end( );
	itLtr--;

	//	��������t�Ɍ��Ă���
	for( ; itLtr != vcTgLine->begin(); itLtr-- )
	{
		if( !( iswspace( itLtr->cchMozi ) ) )	//	�󔒂���Ȃ��Ȃ�����
		{
			itLtr++;	//	���̎����炪�^�[�Q�b�g
			break;
		}
	}
	if( itLtr == vcTgLine->begin() )	//	�擪�����m�F
	{
		//	�󔒂���Ȃ��Ȃ����炻�̎����炪�^�[�Q�b�g
		if( !( iswspace( itLtr->cchMozi ) ) ){	itLtr++;	}
	}

	//	�󔒃G���A���Ȃ��Ȃ���ɂ��邱�Ƃ͂Ȃ�
	if( itLtr == vcTgLine->end( ) ){	return NULL;	}

	wsDelBuf.clear();
	for( itDel = itLtr; vcTgLine->end( ) != itDel; itDel++ ){	wsDelBuf +=  itDel->cchMozi;	}

	cchSize = wsDelBuf.size( ) + 1;
	ptBuffer = (LPTSTR)malloc( cchSize * sizeof(TCHAR) );
	StringCchCopy( ptBuffer, cchSize, wsDelBuf.c_str( ) );

	//	�Y���������폜
	vcTgLine->erase( itLtr, vcTgLine->end( ) );

	return ptBuffer;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��͈͂��폜����
	@param[in]		xDot		�Ώۃh�b�g
	@param[in]		yLine		�Ώۍs
	@param[in]		dBgnMozi	�J�n�����ʒu
	@param[in]		dEndMozi	�I�[�����ʒu
	@param[in,out]	pFirst		�A���h�D�L�^�p
	@return	UINT	�X�������̕�����
*/
UINT DocRangeDeleteByMozi( INT xDot, INT yLine, INT dBgnMozi, INT dEndMozi, PBOOLEAN pFirst )
{
	UINT_PTR	cchSize;
	INT			iBytes;
	LPTSTR		ptBuffer;
	LETR_ITR	vcLtrBgn, vcLtrEnd, vcItr;
	wstring		wsDelBuf;

	LINE_ITR	itLine;

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, yLine );

	vcLtrBgn  = itLine->vcLine.begin( );
	vcLtrEnd  = itLine->vcLine.begin( );
	vcLtrBgn += dBgnMozi;	//	�Y���ʒu�܂ňړ�����
	vcLtrEnd += dEndMozi;	//	���̃G���A�̏I�[���m�F

	wsDelBuf.clear();
	iBytes = 0;
	for( vcItr = vcLtrBgn; vcLtrEnd != vcItr; vcItr++ )
	{
		wsDelBuf += vcItr->cchMozi;
		iBytes   += vcItr->mzByte;
	}

	//	�Y���������폜
	itLine->vcLine.erase( vcLtrBgn, vcLtrEnd );
	itLine->iByteSz -= iBytes;
	//	�A���h�D�o�b�t�@�쐬
	cchSize = wsDelBuf.size( ) + 1;
	ptBuffer = (LPTSTR)malloc( cchSize * sizeof(TCHAR) );
	StringCchCopy( ptBuffer, cchSize, wsDelBuf.c_str( ) );
	SqnAppendString( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), DO_DELETE, ptBuffer, xDot, yLine, *pFirst );
	FREE( ptBuffer );	*pFirst = FALSE;

	return (UINT)(cchSize - 1);
}
//-------------------------------------------------------------------------------------------------

/*!
	�S�̖��͑I��͈͂��E�Ɋ񂹂�
	@param[in]	pXdot	���̃h�b�g�ʒu���󂯂Ė߂�
	@param[in]	dLine	���̍s��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocRightSlide( PINT pXdot, INT dLine )
{
	UINT_PTR	iLines;
	INT			iTop, iBottom, i;
	INT			dSliDot, dRitDot, dPaDot, dInBgn;
	INT			dMozi, dLefDot, dAdDot;
	BOOLEAN		bFirst = TRUE;
	LPTSTR		ptBuffer = NULL;

	LINE_ITR	itLine;

	TRACE( TEXT("�E��") );

	//	�E�񂹌��E�m�F
	dSliDot = InitParamValue( INIT_LOAD, VL_RIGHT_SLIDE, 790 );

	//	�͈͊m�F
	iLines = DocNowFilePageLineCount( );
	iTop    = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop;
	iBottom = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom;

	if( 0 > iTop )		iTop = 0;
	if( 0 > iBottom )	iBottom = iLines - 1;


	ViewSelPageAll( -1 );	//	�I��͈͖����Ȃ�

	dRitDot = DocPageMaxDotGet( iTop, iBottom );	//	��ԉE�̃h�b�g�m�F

	dPaDot = dSliDot - dRitDot;
	if( 0 > dPaDot )
	{
		NotifyBalloonExist( TEXT("�͂ݏo���Ă�݂���"), TEXT("���s"), NIIF_ERROR );
		return E_FAIL;
	}

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, iTop );	//	�ʒu���킹

	for( i = iTop; iBottom >= i; i++, itLine++ )
	{
		dAdDot = dPaDot;
		//	�s���̊J����Ԃ��m�F
		dLefDot = LayerHeadSpaceCheck( &(itLine->vcLine), &dMozi );
		if( 0 < dLefDot )
		{
			dAdDot += dLefDot;
			//	��O�ɋ󔒂�����Ȃ�A���̕��܂߂Ă��炵�p�X�y�[�X���v�Z
			DocRangeDeleteByMozi( 0, i, 0, dMozi, &bFirst );	bFirst = FALSE;
		}

		//	�擪���炤�߂��Ⴄ
		dInBgn = 0;
		ptBuffer = DocPaddingSpaceWithPeriod( dAdDot, NULL, NULL, NULL, TRUE );
		DocInsertString( &dInBgn, &i, NULL, ptBuffer, 0, bFirst );	bFirst = FALSE;
		FREE(ptBuffer);

		ViewRedrawSetLine( i );
	}

	//	�L�����b�g�ʒu�K���ɒ���
	*pXdot = 0;
	DocLetterPosGetAdjust( pXdot, dLine, 0 );	//	�L�����b�g�ʒu�K���ɒ���
	ViewDrawCaret( *pXdot, dLine, 1 );

	DocPageInfoRenew( -1, 1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���j�R�[�h�̎g�p�s�g�p�Ƃ��l�������ߋ󔒂����
	@param[in]	dTgtDot	�쐬����h�b�g��
	@return		LPTSTR	�쐬�����X�y�[�X�̕�����E�J���͌Ă񂾕��Ŗʓ|����
*/
LPTSTR DocPaddingSpaceMake( INT dTgtDot )
{
	LPTSTR	ptReplc = NULL;
	INT		dZenSp, dHanSp, dUniSp;

	//	���O���ƍ��Ȃ�
	if( 0 >= dTgtDot )	return NULL;

	if( gbUniPad )
	{
		ptReplc = DocPaddingSpace( dTgtDot, &dZenSp, &dHanSp );
		//	�쐬�s�������蔼�p����������A���j�R�[�h�g���č�蒼��
		if( !(ptReplc) || (dZenSp < dHanSp) )	//	(dZenSp + 1)
		{
			FREE(ptReplc);
			ptReplc = DocPaddingSpaceUni( dTgtDot, &dZenSp, &dHanSp, &dUniSp );
		}
	}
	else	//	���j�R�[�h�󔒎g��Ȃ��Ȃ�
	{
		ptReplc = DocPaddingSpaceWithGap( dTgtDot, &dZenSp, &dHanSp );
	}

	return ptReplc;
}
//-------------------------------------------------------------------------------------------------

/*!
	�`�`�S�̂��A�Pdot�����炷�B�����Ȃ�󔒂ɒu�������Ȃ���
	@param[in]	vk		�����E�E������
	@param[in]	pXdot	���̃h�b�g�ʒu���󂯂Ė߂�
	@param[in]	dLine	���̍s��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocPositionShift( UINT vk, PINT pXdot, INT dLine )
{
	UINT_PTR	iLines, cchSz;
	INT			iTop, iBottom, i;
	INT			wid, iDot, iLin, iMzCnt;
	INT			iTgtWid, iLefDot, iRitDot;
	BOOLEAN		bFirst = TRUE, bSeled = FALSE, bDone = FALSE;
	BOOLEAN		bRight;	//	��O�E�ց@�O����
	BOOLEAN		bIsSp;
	LPTSTR		ptRepl;
	TCHAR		ch, chOneSp;

	LPUNDOBUFF	pstUndoBuff;	

	LETR_ITR	vcLtrItr;
	LINE_ITR	itLine;



	chOneSp = gaatPaddingSpDotW[1][0];

	pstUndoBuff = &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog);


	TRACE( TEXT("�S�̂��炵") );

	if( VK_RIGHT == vk )		bRight = TRUE;
	else if( VK_LEFT == vk )	bRight = FALSE;
	else	return E_INVALIDARG;

	//	�͈͊m�F
	iLines  = DocNowFilePageLineCount( );
	iTop    = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop;
	iBottom = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom;
	if( 0 <= iTop &&  0 <= iBottom )	bSeled = TRUE;

	if( 0 > iTop )		iTop = 0;
	if( 0 > iBottom )	iBottom = iLines - 1;

	//	���̂܂܂��Ɨe�ʂ������E��U�I����Ԃ��������Čv�Z���Ȃ���
	if( bSeled ){	DocPageSelStateToggle( -1 );	}

	//	��s���ʓ|���Ă���
	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, iTop );	//	�ʒu���킹

	for( i = iTop; iBottom >= i; i++, itLine++ )
	{
		//	����������Ȃ瑀�삷��
		if( 0 != itLine->vcLine.size(  ) )
		{
			//	�擪�������m�F
			vcLtrItr = itLine->vcLine.begin( );
			ch  = vcLtrItr->cchMozi;
			wid = vcLtrItr->rdWidth;	//	������

			bDone = FALSE;

			if( !(iswspace(ch)) )	//	�󔒂ł͂Ȃ�
			{
				if( bRight )	//	�E���炵�Ȃ�
				{
					//	�擪��1dot�X�y�[�X�����΂���
					DocInputLetter( 0, i, chOneSp );
					SqnAppendLetter( pstUndoBuff, DO_INSERT, chOneSp, 0, i, bFirst );	bFirst = FALSE;
					bDone = TRUE;	//	�������������
				}
				else	//	���C�N�Ȃ�A�擪�������󔒂ɂ��Ē�������
				{
					ptRepl = DocPaddingSpaceMake( wid );	//	�K�v�ȋ󔒊m��
					StringCchLength( ptRepl, STRSAFE_MAX_CCH, &cchSz );
					//	���̕������폜
					SqnAppendLetter( pstUndoBuff, DO_DELETE, ch, 0, i, bFirst );	bFirst = FALSE;
					DocIterateDelete( vcLtrItr, i );
					//	�����Đ擪�ɋ󔒂��A�b�[�I
					iDot = 0;	iLin = i;
					DocStringAdd( &iDot, &iLin, ptRepl, cchSz );
					SqnAppendString( pstUndoBuff, DO_INSERT, ptRepl, 0, i, bFirst );	bFirst = FALSE;
					FREE(ptRepl);	//	�J���Y��Ȃ��悤��
				}
			}
			//	���̐�Begin�C�e���[�^����

			if( !(bDone) )	//	�������ł���Ȃ�E���̎��_�ŁA�擪�����͋󔒊m��
			{
				//	�󔒔͈͂��m�F
				iTgtWid = DocLineStateCheckWithDot( 0, i, &iLefDot, &iRitDot, NULL, &iMzCnt, &bIsSp );
				if( bRight )	iTgtWid++;	//	�����ɍ��킹��
				else			iTgtWid--;	//	�h�b�g�������߂�
				if( 0 > iTgtWid )	iTgtWid = 0;	//	�}�C�i�X�͖����Ǝv�����ǔO�̂���

				ptRepl = DocPaddingSpaceMake( iTgtWid );	//	�K�v�ȋ󔒊m��
				//	�^�[�Q�b�g�����O�Ȃ�NULL�Ȃ̂ŁA�擪�����̍폜�����ł���

				DocRangeDeleteByMozi( 0, i, 0, iMzCnt, &bFirst );	//	���̕����폜����

				if( ptRepl )	//	�K�v�ȕ���������
				{
					StringCchLength( ptRepl, STRSAFE_MAX_CCH, &cchSz );
					iDot = 0;	iLin = i;
					DocStringAdd( &iDot, &iLin, ptRepl, cchSz );
					SqnAppendString( pstUndoBuff, DO_INSERT, ptRepl, 0, i, bFirst );	bFirst = FALSE;
					FREE(ptRepl);	//	�J���Y��Ȃ��悤��
				}
			}

			if( bSeled )	//	�I����ԂŃ��b�Ă��̂Ȃ�A�I����Ԃ��ێ�����
			{
				DocRangeSelStateToggle( -1, -1, i , 1 );	//	�Y���s�S�̂�I����Ԃɂ���
				//	���̍s������Ȃ���s���I����
				if( iBottom > i )	DocReturnSelStateToggle( i, 1 );
			}

			DocBadSpaceCheck( i );	//	��Ԃ����Z�b�g
			ViewRedrawSetLine( i );
		}
	}

	if( bSeled )	//	�I��͈͂͂蒼��
	{
		(*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop    = iTop;
		(*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom = iBottom;
	}


	//	�L�����b�g�ʒu����
	iDot = 0;
	DocLetterPosGetAdjust( &iDot, dLine, 0 );	//	�L�����b�g�ʒu�K���ɒ���
	ViewDrawCaret( iDot, dLine, 1 );

	DocPageByteCount( gitFileIt, gixFocusPage, NULL, NULL );
	DocPageInfoRenew( -1, 1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�s�����p�󔒂����j�R�[�h�ɕϊ��E�t�@�C���R�A����
*/
HRESULT DocHeadHalfSpaceExchange( HWND hWnd )
{
	UINT_PTR	iLines;
	INT			iTop, iBottom, i;
	INT			xDot;
	BOOLEAN		bFirst = TRUE, bSeled = FALSE;
	TCHAR		ch;

	LETR_ITR	vcLtrItr;
	LINE_ITR	itLine;


	//	�͈͊m�F
	iLines  = DocNowFilePageLineCount( );
	iTop    = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop;
	iBottom = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom;
	if( 0 <= iTop &&  0 <= iBottom )	bSeled = TRUE;

	if( 0 > iTop )		iTop = 0;
	if( 0 > iBottom )	iBottom = iLines - 1;

	TRACE( TEXT("�s�����p�����j�R�[�h��") );

	ViewSelPageAll( -1 );	//	�I��͈͖����Ȃ�

	//	�e�ʌv�Z�A�o�b�h�󔒂̊m�F�ƍĕ`��K�v

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, iTop );	//	�s�̈ʒu���킹

	for( i = iTop; iBottom >= i; i++, itLine++ )	//	�͈͓��̊e�s�ɂ���
	{
		//	����������Ȃ瑀�삷��
		if( 0 != itLine->vcLine.size(  ) )
		{
			vcLtrItr = itLine->vcLine.begin( );
			ch = vcLtrItr->cchMozi;

			if( TEXT(' ') == ch )	//	���p�Ȃ�
			{
				//	��U�폜����
				SqnAppendLetter( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), DO_DELETE, ch, 0, i, bFirst );
				bFirst = FALSE;
				DocIterateDelete( vcLtrItr, i );

				//	�擪�ʒu��5dot���j�R�[�h�󔒂��A�b�[�I�B
				ch  = (TCHAR)0x2004;
				xDot = DocInputLetter( 0, i, ch );
				SqnAppendLetter( &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog), DO_INSERT, ch, 0, i, bFirst );
			}

			DocBadSpaceCheck( i );	//	��Ԃ����Z�b�g
			ViewRedrawSetLine( i );
		}
	}

	//	�L�����b�g�ʒu�͂���Ȃ�

	DocPageInfoRenew( -1, 1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

#ifdef DOT_SPLIT_MODE
/*!
	�L�����b�g�ʒu����A���E�ɂPdot�����炷�B�����Ȃ�󔒂ɒu�������Ȃ���
	@param[in]	vk		�����E�E������
	@param[in]	pXdot	���̃h�b�g�ʒu���󂯂Ė߂�
	@param[in]	dLine	���̍s��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocCentreWidthShift( UINT vk, PINT pXdot, INT dLine )
{
	UINT_PTR	iLines, cchSz;
	UINT		dRslt, dFirst;
	INT			iBaseDot, iTop, iBottom, iBufDot;
	INT			wid, iDot, iLin, iMzCnt;
	INT			iFnlDot;
	BOOLEAN		bSeled = FALSE;
	BOOLEAN		bRight;	//	��O�E�ց@�O����
	LPTSTR		ptRepl;
	TCHAR		ch, chOneSp;

	LPUNDOBUFF	pstUndoBuff;	

	LETR_ITR	vcLtrItr;
	LINE_ITR	itLine;

	chOneSp = gaatPaddingSpDotW[1][0];	//	��1dot�E�����Ԃɑ}��

	//���S�������󔒂Ȃ�A���̋󔒂�L�яk�݂�����B
	//�����ƕ����̊ԊJ����Ȃ�A���c�ɂȂ��Ă鎚�̍������J����悤�ɂ���
	//�ׂ��Ƃ��́A���c�����󔒂ɒu�������āA������k�߂�

	pstUndoBuff = &((*gitFileIt).vcCont.at( gixFocusPage ).stUndoLog);

	iBaseDot = *pXdot;	//	��_�A�Ȃ�ׂ������Ȃ��悤�ɂ��Ȃ�����
	TRACE( TEXT("���Ԃ��炵 %dDOT"), iBaseDot );

	iFnlDot = iBaseDot;

	if( VK_RIGHT == vk )		bRight = TRUE;
	else if( VK_LEFT == vk )	bRight = FALSE;
	else	return E_INVALIDARG;

	if(  0 == iBaseDot )	//	����O�Ȃ�A�S�̍��E���炵���Ă���
	{
		return DocPositionShift( vk, pXdot, dLine );
	}

	//	�͈͊m�F
	iLines  = DocNowFilePageLineCount( );
	iTop    = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop;
	iBottom = (*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom;
	if( 0 <= iTop &&  0 <= iBottom )	bSeled = TRUE;

	if( 0 > iTop )		iTop = 0;
	if( 0 > iBottom )	iBottom = iLines - 1;

	//	���̂܂܂��Ɨe�ʂ������E��U�I����Ԃ��������Čv�Z���Ȃ���
	if( bSeled ){	DocPageSelStateToggle( -1 );	}

	//	��s���ʓ|���Ă���
	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, iTop );	//	�ʒu���킹

//�Ȃ񂩎��X�A���󔒂��ł���

	dFirst = TRUE;
	//	���Ԃɏ������Ă���
	for( iLin = iTop; iBottom >= iLin; iLin++, itLine++ )
	{
		iDot = itLine->iDotCnt;
		if( iBaseDot >=  iDot ){	continue;	}
		//	����ʒu�ɖ����Ȃ��̂Ȃ�A��������K�v�͖���

		//	����J�n
		iDot = iBaseDot;	//	�����ʒu�m��
		iMzCnt = DocLetterPosGetAdjust( &iDot, iLin, -1 );	//	��ɍ������݂�
		//	���삷��ʒu�̕����m�F

		//	�Y���ʒu���󔒂Ȃ�A�L�яk�݌��p
		iBufDot = iDot;	//	�l�Y����̂ł��̂܂܎g���ƃC�P�Ȃ�
		dRslt = DocSpaceDifference( vk, &iBufDot, iLin, dFirst );	//	iBufDot�̓Y��������g���Ȃ�
		if( dRslt  )	//	�Y��������
		{
			if( iLin == dLine ){	iFnlDot =  iBaseDot;	}	//	���炵����̈ʒu�̖ʓ|����
			dFirst = FALSE;
		}
		else	//	�Ԃ�l�O�Ȃ�A�����Ȃ̂ŏ�����
		{
			vcLtrItr = itLine->vcLine.begin( );
			std::advance( vcLtrItr, iMzCnt );	//	���ڈʒu�̕����܂ňړ�����
			ch  = vcLtrItr->cchMozi;
			wid = vcLtrItr->rdWidth;	//	�Y���̕����̕����m�F

			if( bRight )	//	�E�ɓ�����
			{
				if( iswspace( ch ) )	//	�E���̕����͋󔒂ł�������
				{
					iBufDot = iDot + wid;	//	���̋󔒂����΂�
					DocSpaceDifference( vk, &iBufDot, iLin, dFirst );	//	iBufDot�͎g���Ȃ�
					if( iLin == dLine ){	iFnlDot =  iBaseDot;	}	//	���炵����̈ʒu�̖ʓ|����
				}
				else
				{
					SqnAppendLetter( pstUndoBuff, DO_INSERT, chOneSp, iDot, iLin, dFirst );
					DocInputLetter( iDot, iLin, chOneSp );	//	���̏ꏊ��1dot�X�y�[�X�����΂���
					if( iLin == dLine ){	iFnlDot = iDot +  1;	}	//	���炵����̈ʒu�̖ʓ|����
				}
				dFirst = FALSE;
			}
			else	//	���ɓ�����
			{
				if( iLin == dLine ){	iFnlDot =  iDot;	}	//	���炷�O�̈ʒu�̖ʓ|����

				//	���̕������폜
				SqnAppendLetter( pstUndoBuff, DO_DELETE, ch, iDot, iLin, dFirst );	dFirst = FALSE;
				DocIterateDelete( vcLtrItr , iLin );
				if( 2 <= wid )	//	�����P�Ȃ�A�폜�����ł���
				{
					ptRepl = DocPaddingSpaceMake( wid-1 );	//	�K�v�ȋ󔒊m��
					StringCchLength( ptRepl , STRSAFE_MAX_CCH, &cchSz );	//	�������m�F
					SqnAppendString( pstUndoBuff, DO_INSERT, ptRepl, iDot, iLin, dFirst );	dFirst = FALSE;
					DocStringAdd( &iDot, &iLin, ptRepl, cchSz );	//	�����Đ擪�ɋ󔒂��A�b�[�I
					FREE(ptRepl);	//	�J���Y��Ȃ��悤��
				}
			}
		}

		if( bSeled )	//	�I����ԂŃ��b�Ă��̂Ȃ�A�I����Ԃ��ێ�����
		{
			if( iLin == iTop )
			{
				iDot = iBaseDot;
				DocLetterPosGetAdjust( &iDot, iLin, 0 );	//	�L�����b�g�ʒu�K���ɒ���
				DocRangeSelStateToggle( iDot, -1, iLin , 1 );	//	�Y���s�S�̂�I����Ԃɂ���
			}
			else
			{
				DocRangeSelStateToggle( -1, -1, iLin , 1 );	//	�Y���s�S�̂�I����Ԃɂ���
			}

			//	���̍s������Ȃ���s���I����
			if( iBottom > iLin )	DocReturnSelStateToggle( iLin, 1 );
		}

		DocBadSpaceCheck( iLin );	//	��Ԃ����Z�b�g
		ViewRedrawSetLine( iLin );
	}

	if( bSeled )	//	�I��͈͂͂蒼��
	{
		(*gitFileIt).vcCont.at( gixFocusPage ).dSelLineTop    = iTop;
		(*gitFileIt).vcCont.at( gixFocusPage ).dSelLineBottom = iBottom;
	}

	//	�L�����b�g�ʒu����
	DocLetterPosGetAdjust( &iFnlDot, dLine, 0 );	//	�L�����b�g�ʒu�K���ɒ���
	*pXdot = iFnlDot;	//	�ʒu��߂�
	ViewDrawCaret( iFnlDot, dLine, 1 );
	//	�ĕ`��
	DocPageByteCount( gitFileIt, gixFocusPage, NULL, NULL );
	DocPageInfoRenew( -1, 1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------
#endif

