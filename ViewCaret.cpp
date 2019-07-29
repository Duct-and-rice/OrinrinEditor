/*! @file
	@brief �L�����b�g�̊Ǘ������܂�
	���̃t�@�C���� ViewCaret.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/04/15
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

/*
�L�����b�g(�J���b�g)�̓��b�Z�[�W�L���[�ɑ΂��ĂP�쐬�ł���
�Q�ȏ�̃L�����b�g�������ɕ\�������ƁAUI��D�܂����Ȃ�
�t�H�[�J�X�̗L�薳���Ƃ����֌W�ɍ쐬�j���ł��Ă��܂�
Windows�̎w�j�ɏ]����
�@�P�D�t�H�[�J�X���������Ƃ��B
�@�Q�D�A�N�e�B�u�łȂ��Ȃ����Ƃ��B
�̗��^�C�~���O���ɃL�����b�g���u��\���v�ɂ��ׂ�
*/

//-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "OrinrinEditor.h"
//-------------------------------------------------------------------------------------------------

#define CARET_WIDTH	2
//-------------------------------------------------------------------------------------------------

extern HWND		ghPrntWnd;		//!<	�e�E�C���h�E�n���h��
extern HWND		ghViewWnd;		//!<	�`��E�C���h�E�̃n���h��


extern INT		gdDocXdot;		//!<	�L�����b�g�̂w�h�b�g�E�h�L�������g�ʒu
extern INT		gdDocLine;		//!<	�L�����b�g�̂x�s���E�h�L�������g�ʒu
extern INT		gdDocMozi;		//!<	�L�����b�g�̍����̕�����

//	��ʃT�C�Y���m�F���āA�ړ��ɂ��X�N���[���̖ʓ|�݂�
extern INT		gdHideXdot;		//!<	���̉B�ꕔ��
extern INT		gdViewTopLine;	//!<	�\�����̍ŏ㕔�s�ԍ�
extern SIZE		gstViewArea;	//!<	�\���̈�̃T�C�Y�E���[���[���̗̈�͖���
extern INT		gdDispingLine;	//!<	�����Ă�s���E���r���[�Ɍ����Ă閖�[�͊܂܂Ȃ�


static HBITMAP	ghbmpCaret;		//!<	�L�����b�g�p�̃r�b�g�}�b�v

static BOOLEAN	gbCaretShow;	//!<	�L�����b�g�\�����Ă邩�H
//-------------------------------------------------------------------------------------------------


HRESULT	ImeInputBoxPosSet( VOID );

HRESULT	ViewCaretFrameOutCheck( INT, INT, UINT );
//-------------------------------------------------------------------------------------------------


/*!
	�L�����b�g�����
	@param[in]	hWnd	�E�C���h�E�̃n���h��
	@param[in]	clrMain	�L�����b�g�̐F
	@param[in]	clrBack	�L�����b�g�̔w�i�E���g�p�ɂ��Ă݂�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewCaretCreate( HWND hWnd, COLORREF clrMain, COLORREF clrBack )
{
	HDC		hdcMem, hdc = GetDC( hWnd );
	HBITMAP	hBmpOld;
	HBRUSH	hBrushCaret, hBrushBack, hBrushOld;


//�e�X�g
	clrBack  = ~(clrMain);	//	���]�F������
	clrBack &= 0x00FFFFFF;	//	�֌W�Ȃ��Ƃ���̓N�����[���Ă���

	//	�K�v�Ȃ�A�ȑO�̃r�b�g�}�b�v��j������E�X����O�̏�����


	//	�L�����b�gBMP���쐬�E�����̂��ǂݍ���ł��ǂ�
	ghbmpCaret  = CreateCompatibleBitmap( hdc, CARET_WIDTH, LINE_HEIGHT );
	hdcMem      = CreateCompatibleDC( hdc );
	hBmpOld     = SelectBitmap( hdcMem, ghbmpCaret );

	hBrushCaret = CreateSolidBrush( clrMain );
	hBrushBack  = CreateSolidBrush( clrBack );

	hBrushOld   = SelectBrush( hdcMem, hBrushCaret );
	PatBlt( hdcMem, 0, 0, CARET_WIDTH, LINE_HEIGHT, PATCOPY );
	SelectBrush( hdcMem, hBrushBack );
	PatBlt( hdcMem, 0, 0, CARET_WIDTH, LINE_HEIGHT, PATINVERT );

	SelectBrush( hdcMem, hBrushOld );
	SelectBitmap( hdcMem, hBmpOld );

	DeleteBrush( hBrushCaret );
	DeleteBrush( hBrushBack );
	DeleteDC( hdcMem );

	ReleaseDC( hWnd, hdc );

	//	�L�����b�g���쐬����
	CreateCaret( hWnd, ghbmpCaret, CARET_WIDTH, LINE_HEIGHT );

	gbCaretShow = FALSE;


	//	�t�H�[�J�X��������������Ă����Ƃ��̂��z�����肢�܂�

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�L�����b�g�̔j��
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT ViewCaretDelete( VOID )
{
	if( ghbmpCaret )
	{
		DestroyCaret(  );
		DeleteObject( ghbmpCaret );
	}
	ghbmpCaret = NULL;

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�L�����b�g�̍ĕ\��
	@return	BOOL	��O�\������Ă��@�O�����Ă�
*/
BOOL ViewShowCaret( VOID )
{
	BOOL	bRslt;

	bRslt = ShowCaret( ghViewWnd  );

	if( !(bRslt) )
	{
		bRslt = CreateCaret( ghViewWnd, ghbmpCaret, CARET_WIDTH, LINE_HEIGHT );
		TRACE( TEXT("CARET reset %u"), bRslt );
		gbCaretShow = FALSE;
		ViewDrawCaret( gdDocXdot, gdDocLine, 1 );	//	�ʒu�����߂�
	}

//	ImeInputBoxPosSet(  );

	return bRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	�L�����b�g�̔�\��
*/
VOID ViewHideCaret( VOID )
{
	HideCaret( ghViewWnd  );
	gbCaretShow = FALSE;

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	Caret���A�F��ς��čč\�z����
	@param[in]	crtColour	�ύX����F
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT ViewCaretReColour( COLORREF crtColour )
{

	ViewCaretDelete(  );	//	�܂������̃��c��j��

	ViewCaretCreate( ghViewWnd, crtColour, 0 );

	ViewShowCaret(  );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�L�����b�g�̈ʒu�������ύX
	@param[in]	xDot	�`��ʒu�̉��h�b�g��
	@param[in]	yLine	�`�悷��s��
	@return		��O�g���ł���@�O�͂ݏo���Ă�
*/
BOOLEAN ViewPosResetCaret( INT xDot, INT yLine )
{
	if( 0 > xDot )	xDot  = 0;
	if( 0 > yLine )	yLine = 0;

	gdDocLine = yLine;
	gdDocMozi = DocLetterPosGetAdjust( &xDot, yLine, 0 );	//	���̕����ʒu���m�F
	gdDocXdot = xDot;

	return ViewDrawCaret( xDot, yLine, TRUE );
}
//-------------------------------------------------------------------------------------------------

/*!
	�L�����b�g�̈ʒu��ύX����
	@param[in]	rdXdot	�`��ʒu�̉��h�b�g��
	@param[in]	rdLine	�`�悷��s��
	@param[in]	bOnScr	��ʂ���͂ݏo���悤�Ȃ�X�N���[�������邩�H
	@return		��O�g���ł���@�O�͂ݏo���Ă�
*/
BOOLEAN ViewDrawCaret( INT rdXdot, INT rdLine, BOOLEAN bOnScr )
{
	INT		dX, dY, loop;
	BOOLEAN	bRslt, fRslt, cRslt;
	POINT	stCaret;

	dX = rdXdot;
	dY = rdLine * LINE_HEIGHT;


	stCaret.x = rdXdot;
	stCaret.y = rdLine;
	DocCaretPosMemory( INIT_SAVE, &stCaret );	//	�t�@�C������Caret�ʒu�ۑ�


	//	�ʒu���́A�h�L�������g�I�Ȉʒu�ő��삵�āA�r���[����Ƃ��������낢�낷��
	//	�����ŁA�J�[�\���ʒu����ʂ���͂ݏo�������Ȃ�A�K�؂ȕ����ɃX�N���[��
	//	�ʒu�����C��

	//	�X�N���[���o�[����̑���Ȃ�A�L�����b�g����ʂ���͂ݏo���Ă��悢
	if( bOnScr )	ViewCaretFrameOutCheck( dX, dY, 1 );
	
	ViewPositionTransform( &dX, &dY, 1 );

//�L�����b�g�ʒu���}�C�i�X�ɂȂ�悤�Ȃ�A��\���ɂ���

	//gdViewXdot = dX;
	//gdViewLine = dY;	//	�g���ĂȂ��H

	fRslt = ViewIsPosOnFrame( dX , dY );	//	�ʒu�m�F
	if( fRslt )
	{
		bRslt = SetCaretPos( dX, dY );	//	�ړ�

		if( !(gbCaretShow) )
		{
			for( loop = 0; 10 > loop; loop++ )
			{
				cRslt = ShowCaret( ghViewWnd  );	//	�\������
				if( cRslt )	break;
			}
		}
		gbCaretShow = TRUE;
	}
	else
	{
		if( gbCaretShow )
		{
			for( loop = 0; 10 > loop; loop++ )
			{
				cRslt = HideCaret( ghViewWnd  );	//	����
				if( cRslt )	break;
			}
		}
		gbCaretShow = FALSE;
	}


	ViewNowPosStatus(  );

	ImeInputBoxPosSet(  );	//	IME���͘g�̈ʒu��ύX

	return fRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	���[���[�Ƃ��l������O�̕`��ʒu�h�b�g���󂯎���āA
	�͂ݏo���悤�Ȃ�X�N���[���ʂ𑀍삷��E����
	@param[in]	dDotX	�`��ʒu�̉��h�b�g���E�����ł͐��l�󂯎�邾��
	@param[in]	dDotY	�`��ʒu�̏c�h�b�g��
	@param[in]	dummy	�Ƃ��ɂȂ��H
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewCaretFrameOutCheck( INT dDotX, INT dDotY, UINT dummy )
{
	BOOLEAN	bRedraw = FALSE;
	INT	opX, opY;
	INT	bkWid;

	//	���ւ͂ݏo��
	if( gdHideXdot > dDotX )	//	�B��ʒu��肳��ɍ��ւ�������
	{
		gdHideXdot = dDotX;

		bRedraw = TRUE;
	}

	//	�E�ւ͂ݏo��
	opX = dDotX - gdHideXdot;
	if( gstViewArea.cx < (opX + EOF_WIDTH) )
	{
		bkWid = (opX + EOF_WIDTH) - gstViewArea.cx;

		gdHideXdot += bkWid;	//	���B��̈�𑝂₷

		bRedraw = TRUE;
	}


	//	��ւ͂ݏo��
	if( gdViewTopLine > gdDocLine )
	{
		assert( gdViewTopLine );	//	���̒i�K�łO�͂��蓾�Ȃ�
		gdViewTopLine = gdDocLine;

		bRedraw = TRUE;
	}

	//	���ւ͂ݏo��
	opY = gdDocLine - gdViewTopLine;
	if( gdDispingLine <= opY )
	{
		gdViewTopLine = gdDocLine - gdDispingLine + 1;

		bRedraw = TRUE;
	}

	if( bRedraw )	ViewRedrawSetLine( -1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------



/*!
	IME���͘g�̈ʒu��ύX
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ImeInputBoxPosSet( VOID )
{
	COMPOSITIONFORM	stCompForm;
	HIMC	hImc;
	POINT	stPoint;

	hImc = ImmGetContext( ghViewWnd );	//	IME�n���h���m��

	if( hImc )	//	�m�ۏo������
	{
		GetCaretPos( &stPoint );	//	���̃L�����b�g�ʒu���m�F����
		stCompForm.dwStyle = CFS_POINT;	//	�ʒu�ύX��L���ɂ���
		stCompForm.ptCurrentPos.x = stPoint.x;
		stCompForm.ptCurrentPos.y = stPoint.y;

	//	TRACE( TEXT("CARET [%d x %d]"), stPoint.x, stPoint.y );

		ImmSetCompositionWindow( hImc, &stCompForm );	//	�ʒu�ύX

		ImmReleaseContext( ghViewWnd , hImc );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���̃L�����b�g�ʒu���Q�b�c����
	@param[in]	pXdot	�L�����b�g�̂w�h�b�g
	@param[in]	pYline	�L�����b�g�̂x�s��
	@return	INT	�L�����b�g�̍����̕�����
*/
INT ViewCaretPosGet( PINT pXdot, PINT pYline )
{
	if( pXdot ){	*pXdot  = gdDocXdot;	}
	if( pYline ){	*pYline = gdDocLine;	}

	return gdDocMozi;
}
//-------------------------------------------------------------------------------------------------

