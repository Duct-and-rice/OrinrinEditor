/*! @file
	@brief �͈͑I���̊Ǘ������܂�
	���̃t�@�C���� ViewSelect.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/04/22
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

//	TODO:	ALT�����Ȃ���J�[�\���A�}�E�X���������炻�̂Ƃ�������`�I���ɂȂ�悤�ɂ���


extern  HWND	ghPrntWnd;		//	�e�E�C���h�E�n���h��
extern  HWND	ghViewWnd;		//	���̃E�C���h�E�̃n���h��

extern INT		gdDocXdot;		//	�L�����b�g�̂w�h�b�g�E�h�L�������g�ʒu
extern INT		gdDocLine;		//	�L�����b�g�̂x�s���E�h�L�������g�ʒu

//	��ʃT�C�Y���m�F���āA�ړ��ɂ��X�N���[���̖ʓ|�݂�
extern INT		gdHideXdot;		//	���̉B�ꕔ��
extern INT		gdViewTopLine;	//	�\�����̍ŏ㕔�s�ԍ�
extern SIZE		gstViewArea;	//	�\���̈�̃T�C�Y�E���[���[���̗̈�͖���
extern INT		gdDispingLine;	//	�����Ă�s���E���r���[�Ɍ����Ă閖�[�͊܂܂Ȃ�

//	�����̃L�[�̋�́AGetKeyState��������GetKeyboardState���g���΂���
extern BOOLEAN	gbShiftOn;		//	�V�t�g��������Ă���
extern BOOLEAN	gbCtrlOn;		//	�R���g���[����������Ă���
extern BOOLEAN	gbAltOn;		//	�A���^��������Ă���

extern BOOLEAN	gbExtract;	

extern POINT	gstCursor;		//	�������l�����Ȃ��ACursor�̃h�b�g���s�ʒu�E���ϐ��ł����̂��낤��

//	�I��͈͂̎n�_�I�_�E�l�͂w�h�b�g�̂x�s���ŁE��`�p�ɂ���
static POINT	gstSqSelBegin;	//!<	�n�_
static POINT	gstSqSelEnd;	//!<	�I�_

static POINT	gstSelBgnOrig;	//!<	�͈͑I�����J�n�����n�_
static POINT	gstSelEndOrig;	//!<	�͈͑I�����I�������n�_

static POINT	gstPrePos;		//!<	���O�̑I���ʒu


static BOOLEAN	gbSelecting;	//!<	�I�𑀍쒆���H
EXTERNED UINT	gbSqSelect;		//!<	��`�I�𒆂ł��� D_SQUARE
//-------------------------------------------------------------------------------------------------

HRESULT	ViewSelStateChange( UINT );
HRESULT	ViewSqSelAdjust( INT );
//-------------------------------------------------------------------------------------------------

/*!
	�I�𑀍쒆�ł��邩�ǂ����̖₢���킹
	@param[out]	pSqSel	��`�I�𒆂ł��邩�ǂ����ENULL��
	@return	BOOLEAN	��O�I�𒆂ł���@�O�I�����ĂȂ�
*/
BOOLEAN IsSelecting( PUINT pSqSel )
{
	if( pSqSel )	*pSqSel = gbSqSelect;

	return gbSelecting;
}
//-------------------------------------------------------------------------------------------------

/*!
	�J�[�\�����삵���Ƃ��Ƃ��A�}�E�X�N���b�N�ŃJ�[�\���ړ������Ƃ��Ƃ��ɌĂ΂��
	@param[in]	pVoid	�Ȃɂ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewSelPositionSet( LPVOID pVoid )
{
	INT	iBgn, iEnd, iDmy = 0;

	//	���[���ĕ`��E�w�ړ������Ȃ�`��̕K�v�͖���
	if( (gstPrePos.x != gdDocXdot) )
	{
		iBgn =  gstPrePos.x;	if( 0 > iBgn )	iBgn = 0;
		iEnd =  gstPrePos.x + 1;
		ViewPositionTransform( &iBgn, &iDmy, 1 );
		ViewPositionTransform( &iEnd, &iDmy, 1 );
		ViewRulerRedraw( iBgn, iEnd );	//	�X�V!?�͈͂������

		iBgn =  gdDocXdot;	if( 0 > iBgn )	iBgn = 0;
		iEnd =  gdDocXdot + 1;
		ViewPositionTransform( &iBgn, &iDmy, 1 );
		ViewPositionTransform( &iEnd, &iDmy, 1 );
		ViewRulerRedraw( iBgn, iEnd );	//	�X�V!?�͈͂������
	}

	gstPrePos.x = gdDocXdot;
	gstPrePos.y = gdDocLine;

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	��`�I�����[�h��ON/OFF�g�O��
	@param[in]	bMode	��O���j���[����@�O�͈͑I�������̒�����
	@param[in]	pVoid	�Ȃɂ�
	@return	UINT	��O��`���[�h�n�m�@�O��`���[�h�n�e�e
*/
UINT ViewSqSelModeToggle( UINT bMode, LPVOID pVoid )
{
	POINT	point;

	TRACE( TEXT("��`�I��ON/OFF") );

	//	�I�𓮍쒆�̓��[�h�ύX���Ȃ�
	if( gbSelecting )	return gbSqSelect;	//	�Ȃ���RETURN�������Ă��H

	if( bMode )	//	20120313
	{
		gbSqSelect ^= D_SQUARE;
	}
	else
	{
		//gbSqSelect &= ~D_SQUARE;	//	��U��������
		if( gbAltOn ){	gbSqSelect |=  D_SQUARE;	}
		//	20120323	Alt������Ă���ON�A�Ⴄ�Ȃ�f�ʂ�
	}

	//	�J�n���Ă��I�����Ă�����������͕̂ς��Ȃ�
	gstSqSelBegin.x = -1;
	gstSqSelBegin.y = -1;
	gstSqSelEnd.x   = -1;
	gstSqSelEnd.y   = -1;

	MenuItemCheckOnOff( IDM_SQSELECT , gbSqSelect );	//	���j���[�`�F�b�N

	OperationOnStatusBar(  );

	//	�J�[�\����ύX���Ă݂�E��`�Ȃ�N���X��
	if( D_SQUARE & gbSqSelect )
	{
		SetClassLongPtr( ghViewWnd, GCLP_HCURSOR, (LONG_PTR)(LoadCursor( NULL, IDC_CROSS ) ) );
	}
	else
	{
		SetClassLongPtr( ghViewWnd, GCLP_HCURSOR, (LONG_PTR)(LoadCursor( NULL, IDC_IBEAM ) ) );
	}
	GetCursorPos( &point );
	SetCursorPos( point.x, point.y );

	return gbSqSelect;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I��͈͊m�F���āA�n�_�I�_�������������������
	@param[in]	dMode	��F���ɍl���Ȃ��@���F�I���J�n�������[�h�E�ł��g���ĂȂ�
	@return	��O�n�_�I�_���قȂ�@�O����
*/
UINT ViewSelRangeCheck( UINT dMode )
{

	//	�n�_�I�_�������ʒu�������I�����Ă��Ȃ�
	if( gstSelBgnOrig.x == gstSelEndOrig.x && gstSelBgnOrig.y == gstSelEndOrig.y )
	{
		TRACE( TEXT("�͈͏��łɂ��I������") );
		if( IsSelecting( NULL ) )
		{
			TRACE( TEXT("�I�𒆂ł�������͈͉���") );
			ViewSelPageAll( -1 );	//	���̒���DocSelRangeSet
		}

		//	��`�p
		gstSqSelBegin.x = -1;
		gstSqSelBegin.y = -1;
		gstSqSelEnd.x   = -1;
		gstSqSelEnd.y   = -1;

		gstSelBgnOrig.x = -1;
		gstSelBgnOrig.y = -1;
		gstSelEndOrig.x = -1;
		gstSelEndOrig.y = -1;

		return 0;
	}

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I�����邩�ǂ����m�F���āA�n�_�Ƃ�����邩���E�L�[�}�E�X�������痈��
	�L�����b�g�̈ړ������̑O�ɁA���O�̈ʒu���o���Ă����K�v������E���������_
	@param[in]	dMode	��F���ɍl���Ȃ��@���F�I���J�n�������[�h
	@return	HRESULT		�I����ԃR�[�h
*/
HRESULT ViewSelMoveCheck( UINT dMode )
{

	if( gbExtract && dMode )	//	���o���[�h�Ȃ�
	{
		ViewSelStateChange( FALSE );

		return S_OK;
	}

	if( gbSelecting )
	{
		if( gbShiftOn || dMode )	//	�V�t�g������Ă邩�A�h���b�O�I�𒆂ł���
		{
			ViewSelStateChange( FALSE );

			ViewSelRangeCheck( dMode );
		}
		else
		{
			TRACE( TEXT("������ɂ��I������") );
			ViewSelPageAll( -1 );

			//	��`�p
			gstSqSelBegin.x = -1;
			gstSqSelBegin.y = -1;
			gstSqSelEnd.x   = -1;
			gstSqSelEnd.y   = -1;

			gstSelBgnOrig.x = -1;
			gstSelBgnOrig.y = -1;
			gstSelEndOrig.x = -1;
			gstSelEndOrig.y = -1;
		}
	}
	else
	{
		//	���I����ԂŁA�V�t�g�I�T���Ȃ���J�[�\���̈ړ�����������
		//	�������̓h���b�O�I���Ȃ�
		if( gbShiftOn || dMode )
		{
			//	ALT�����Ȃ���I���J�n������A��`�I����Toggle����
		//	if( gbAltOn )	//	20120313
				ViewSqSelModeToggle( 0, NULL );

			TRACE( TEXT("STATE[%d %d %d]"), gbShiftOn, gbAltOn, dMode );

			//	��`�p
			gstSqSelBegin = gstPrePos;	//	�`��ʒu�H
			gstSqSelEnd.x = gdDocXdot;
			gstSqSelEnd.y = gdDocLine;

			gstSelBgnOrig = gstSqSelBegin;
			gstSelEndOrig = gstSqSelEnd;

			gbSelecting   = TRUE;	//	�I�������J�n
			TRACE( TEXT("�I�������J�n[%d:%d]"), gstSqSelBegin.x, gstSqSelBegin.y );

			ViewSelStateChange( TRUE );
		}

	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�y�[�W�S�̂̑I����Ԃ�ON/OFF����
	@param[in]	dForce	�O�ف@�{�I����ԁ@�[�I������
	@return		�S�̕�����
*/
INT ViewSelPageAll( INT dForce )
{
	TRACE( TEXT("�S�I��[%d]"), dForce );

	if( 0 < dForce )		gbSelecting =  TRUE;	//	�I�������J�n
	else if( 0 > dForce )	gbSelecting = FALSE;	//	�I�������I��
	else					return 0;	//	�O�Ȃ珈�����Ȃ�

	return DocPageSelStateToggle( dForce );
}
//-------------------------------------------------------------------------------------------------

/*!
	�X�V���ꂽ�I��͈͂́A�I����Ԃ�ON/OFF����
	@param[in]	dFirst	�I���J�n�����Ƃ����ǂ����E�g���ĂȂ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewSelStateChange( UINT dFirst )
{
	//	���O�̃J�[�\���ʒu����A���̑I��͈�END�ʒu�̊Ԃ̕���
	//	����͔͈͍X�V���ꂽ�͈͂Ɋ܂ނ�̏���
	INT		dBeginDot, dEndDot, dStep = 0;
	INT		dBaseLine, dJpLn;
//	LONG	dBuffer;

	//	���O�̏�Ԃ���s�܂������������ǂ���-
	if( gstPrePos.y != gdDocLine )
	{
		dStep = gdDocLine - gstPrePos.y;	//	�}�C�i�X�����ɒ��ӃZ��
		
		TRACE( TEXT("�I���ōs�܂��������FD[%d] L[%d] St[%d]"), gdDocXdot, gdDocLine, dStep );

		//	���X�L�����b�g�̂������s�̏���
		//	�t�����ւ̑I�������͂����ł���Ă�����͂�
		if( 0 <  dStep )	//	�����Ɍ�������
		{
			dBeginDot =  gstPrePos.x;	//	�L�����b�g�ʒu����
			dEndDot   =  -1;	//	�s�I�[�܂�
		}
		else	//	�����O�Ȃ炻�����������܂ŗ��Ȃ����炨��
		{
			dBeginDot = 0;
			dEndDot   = gstPrePos.x;
		}

		gstSqSelEnd.x =  gdDocXdot;	//	����Ɣ͈͎w��Ɏg��

		dBaseLine = gstPrePos.y;	//	���X�L�����b�g�̑��݂��Ă����s
	}
	else	//	�܂����łȂ�
	{
		if( gstPrePos.x < gdDocXdot )	//	�����Ɍ�������
		{
			dBeginDot = gstPrePos.x;
			dEndDot   = gdDocXdot;
		}
		else	//	�K���Ɍ�������
		{
			dBeginDot = gdDocXdot;
			dEndDot   = gstPrePos.x;
		}


		dBaseLine = gdDocLine;	//	���X�L�����b�g�̑��݂��Ă����s
	}

	gstSelEndOrig.x = gdDocXdot;
	gstSelEndOrig.y = gdDocLine;

	//	�I��͈́A����E�������EOrig�ʒu�͐�ΓI�ȓ��e�̂͂��EOrig�l�����ɂ���΂������H
	if( gstSelBgnOrig.y >  gstSelEndOrig.y )	//	�J�n����O�ɃC�b����
	{
		gstSqSelBegin.y = gdDocLine;
		gstSqSelEnd.y   = gstSelBgnOrig.y;
	}
	else	//	�ʏ�Ȃ�
	{
		gstSqSelBegin.y = gstSelBgnOrig.y;
		gstSqSelEnd.y   = gdDocLine;
	}

	if( gstSelBgnOrig.x >  gstSelEndOrig.x )	//	�J�n����O�ɃC�b����
	{
		gstSqSelBegin.x = gdDocXdot;	//	��`�p�H
		gstSqSelEnd.x   = gstSelBgnOrig.x;
	}
	else	//	�ʏ�Ȃ�
	{
		gstSqSelBegin.x = gstSelBgnOrig.x;
		gstSqSelEnd.x   = gdDocXdot;
	}


	//	�J�n�ʒu�����ނƂ��A���������Ȃ��悤�ɏC��
	//	��[�A���[������Ȃ��悤�ɒ��ӃZ��

	TRACE( TEXT("[%d:%d][%d:%d]"), gstSqSelBegin.x, gstSqSelBegin.y, gstSqSelEnd.x, gstSqSelEnd.y );

	DocSelRangeSet( gstSqSelBegin.y, gstSqSelEnd.y );

	if( gbSqSelect )	//	��`�̎��͐�p�ɏ�������
	{
		ViewSqSelAdjust( dBaseLine );
	}
	else
	{
		//	���X�L�����b�g�̂������s�̑I���𒲐�
		DocRangeSelStateToggle( dBeginDot, dEndDot, dBaseLine, 0 );

		//	�s�܂����������Ă���
		if( 1 <= dStep )
		{
			DocReturnSelStateToggle( dBaseLine, 0 );

			//	�ԂɂP�s�ȏ㑶�݂��Ă��炻�����𖄂߂�
			for( dJpLn = (dBaseLine + 1); gdDocLine > dJpLn; dJpLn++ )
			{
				DocReturnSelStateToggle( dJpLn, 0 );
				DocRangeSelStateToggle( 0, -1, dJpLn, 0 );
			}

			DocRangeSelStateToggle( 0, gdDocXdot, gdDocLine, 0 );
		}

		if( -1 >= dStep )
		{
		//	DocReturnSelStateToggle( dBaseLine, 0 );	//	�����ɂ͗v��Ȃ��H

			//	�ԂɂP�s�ȏ㑶�݂��Ă��炻�����𖄂߂�
			for( dJpLn = (gdDocLine + 1); dBaseLine > dJpLn; dJpLn++ )
			{
				DocReturnSelStateToggle( dJpLn, 0 );
				DocRangeSelStateToggle( 0, -1, dJpLn, 0 );
			}

			DocReturnSelStateToggle( gdDocLine, 0 );
			DocRangeSelStateToggle( gdDocXdot, -1, gdDocLine, 0 );
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I���J�n�n�_���O�ɑI�����Ă��邩
	@param[in]	line	���ݍs
	@return	UINT		��O����I����I
*/
UINT ViewSelBackCheck( INT line )
{
	//	�I��͈́A����E�������EOrig�ʒu�͐�ΓI�ȓ��e�̂͂��EOrig�l�����ɂ���΂������H

	TRACE( TEXT("LINE[%d] ST[%d]"), line, gstSelBgnOrig.y );

	//	�J�n�����̍s�C�b����
	if( gstSelBgnOrig.y >= line ){	return TRUE;	}

	return FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	��`�I���̒���
	@param[in]	dBaseLine	���X�L�����b�g�̂������s�ԍ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewSqSelAdjust( INT dBaseLine )
{
	INT	i, xDotBegin, xDotEnd, xDotLast;
	//	�����Ɨǂ������Ȃ���

#pragma message ("�����ŁA�I��͈͑S�̂̏��������x���s���Ă���̂ŏd����")

	//	�}�E�X�N���b�N�Ƃ��ŁA�s�P�ʂőI�����ύX���ꂽ�ꍇ
	//	�オ�J���Ă�E�J���Ă鏊�̑I������
	if( dBaseLine < gstSqSelBegin.y )
	{
		for( i = dBaseLine; gstSqSelBegin.y > i; i++ )
		{
			DocRangeSelStateToggle( 0, -1, i, -1 );
		}
	}
	//	�����J���Ă�
	if( gstSqSelEnd.y < dBaseLine )
	{
		for( i = gstSqSelEnd.y + 1; dBaseLine >= i; i++ )
		{
			DocRangeSelStateToggle( 0, -1, i, -1 );
		}
	}

	for( i = gstSqSelBegin.y; gstSqSelEnd.y >= i; i++ )
	{
		xDotBegin = gstSqSelBegin.x;
		DocLetterPosGetAdjust( &xDotBegin, i, 0 );	//	�e�s�̃L�����b�g�ʒu�̒���

		//	20110720	�I�[�ʒu�ǂ��ɂ��Ȃ�H
	//	xDotEnd = gstSqSelEnd.x;	//��
		//���[�ʒu�ƃJ�[�\���ʒu���ׂāA��艓�������̗p�E�t�����֑Ώ����邽�߂Ɋm�F���K�v
		xDotEnd = (gstSqSelEnd.x < gstCursor.x) ? gstCursor.x : gstSqSelEnd.x;
		DocLetterPosGetAdjust( &xDotEnd, i, 0 );

		//	���[�m�F
		xDotLast = DocLineParamGet( i, NULL, NULL );

		if( 0 < xDotBegin )	//	�擪����I��͈͒��O�܂�
		{
			DocRangeSelStateToggle( 0, xDotBegin, i, -1 );
		}

		//	�I��͈�
		DocRangeSelStateToggle( xDotBegin, xDotEnd, i, 1 );

		if( xDotEnd < xDotLast )	//	�I��͈͂̏I��肩�疖�[�܂�
		{
			DocRangeSelStateToggle( xDotEnd, -1, i, -1 );
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�J�[�\���̂���A�󔒗�������͕������I����Ԃɂ���
	@param[in]	pVoid	�Ȃɂ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewSelAreaSelect( LPVOID pVoid )
{
	INT		iBeginDot, iEndDot, iStCnt, iCount;
	INT		iRangeDot;
	BOOLEAN	bIsSpase;

	DocPageSelStateToggle(  FALSE );	//	��U�I����Ԃ͉���

	iRangeDot = DocLineStateCheckWithDot( gdDocXdot, gdDocLine, &iBeginDot, &iEndDot, &iStCnt, &iCount, &bIsSpase );
															//	�n�_�h�b�g�E�I�_�h�b�g�E�J�n�n�_�̕������E�Ԃ̕������E�Y���̓X�y�[�X�ł��邩
	gdDocXdot = iBeginDot;	//	�I��͈͂Ƃ��Ĉړ�����
	ViewSelMoveCheck( FALSE );
	ViewSelPositionSet( NULL );

	//	�h���b�O�ړ���͋[�I�ɍs��

	gdDocXdot = iEndDot;	//	�I��͈͂Ƃ��Ĉړ�����

	ViewDrawCaret( gdDocXdot, gdDocLine, 1 );	//	�����ŃL�����b�g���ړ�

	ViewSelMoveCheck( TRUE );
	ViewSelPositionSet( NULL );	//	�ړ������ʒu���L�^

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

