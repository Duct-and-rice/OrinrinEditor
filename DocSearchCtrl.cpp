/*! @file
	@brief �t�@�C�������╶���񌟍��̏��������܂�
	���̃t�@�C���� DocSearchCtrl.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/11/15
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
Ctrl+F�Ō������I�[�|��

Ctrl�{�e�R�ŁA�I��͈͂������͈͂ɁE�͈͂Ȃ������疳��
�e�R�Ŏ��̌���������̂���łփW�����v
�e�T�Ń��t���b�V�����ȈՑΉ�


����������A�t�@�C�����E�Ŕԍ��E�ʒu��S���o���Ă����K�v�����遃�s�v�H

�C�e���[�^�Aerase�̕Ԃ�l�́A�폜������̎��̈ʒu

F3�W�����v�̋L�^�A�ňړ�������A���̕łɍ��킹��
�V�K���������炻��ɍ��킹��

�����ʒu�ɕ�����ǉ��폜�����Ƃ��̏����́H
�땶���ǉ��폜�E���̍s����������x�T�[�`����΂���
���s�܂ł͂�������A�ĕ`��͈͒���

�y�[�X�g�Ƃ��A�I���폜�݂����ȑ�Z�͂�������
�폜��,�I�������̃J�[�\���ʒu�ł����邩�H�y�[�X�g�́A�n�߂��s��,�y�[�X�g�͈͍s�����ŁH

*/

#ifdef FIND_STRINGS



//	�q�b�g�ʒu���L�^
//typedef struct tagFINDPOSITION
//{
//	LPARAM	dUnique;	//!<	�t�@�C���ʂ��ԍ��E�P�C���f�b�N�X
//	INT		iPage;		//!<	�����Ă��
//	INT		iLine;		//!<	�Y���s
//	INT		iCaret;		//!<	�s���ł̕����ʒu
//
//} FINDPOSITION, *LPFINDPOSITION;

//-------------------------------------------------------------------------------------------------

extern list<ONEFILE>	gltMultiFiles;	//	�����t�@�C���ێ�
//�C�e���[�^��typedef�̓w�b�_��

extern FILES_ITR	gitFileIt;			//		�����Ă�t�@�C���̖{��

extern INT		gixFocusPage;			//		���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X

EXTERNED HWND	ghFindDlg;				//!<	�����_�C�����O�̃n���h��


static TCHAR	gatLastPtn[MAX_PATH];	//!<	�ŐV�̌�����������o���Ă���

static TCHAR	atSetPattern[MAX_PATH];	//!<	�����J�n����������E�����{�^���A�ł����玟�X�i�ނ̔��f�Ɏg��
static INT		giSetRange;				//!<	�����J�n�����Ƃ��́A�����͈�
static BOOLEAN	gbSetModCrlf;			//!<	�����J�n�����Ƃ��́A�����Ή�

//static INT		giCrLfCnt;				//!<	���������񒆂ɉ��s���������邩

static  UINT	gdNextStart;			//!<	����̌����I�[�ʒu�����̌����J�n�ʒu
static   INT	giSearchPage;			//!<	�������Ă�y�[�W�B�y�[�W�n�茟���p

//static FINDPOSITION	gstFindPos;			//!<	�����W�����v�ʒu

//static list<FINDPOSITION>	gltFindPosition;	//!<	�������ʕێ�
//-------------------------------------------------------------------------------------------------


INT_PTR		CALLBACK FindStrDlgProc( HWND, UINT, WPARAM, LPARAM );	//!<	
HRESULT		FindExecute( HWND );									//!<	
INT_PTR		FindPageSearch( LPTSTR, INT, FILES_ITR );				//!<	

UINT_PTR	SearchPatternStruct( LPTSTR, UINT_PTR, LPTSTR, BOOLEAN );

#ifdef SEARCH_HIGHLIGHT
INT		FindPageHighlightOff( INT , FILES_ITR );				//!<	
HRESULT	FindPageHighlightSet( INT, INT, INT, FILES_ITR );		//!<	
HRESULT	FindLineHighlightOff( UINT , LINE_ITR );				//!<	
#endif
HRESULT		FindPageSelectSet( INT, INT, INT, FILES_ITR );			//!<	

//-------------------------------------------------------------------------------------------------


/*!
	�����_�C�����O���J���E���[�_���X��
	@param[in]	hInst	�A�|���P�[�V�����̎���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@retval HRESULT	�I����ԃR�[�h
*/
HRESULT FindDialogueOpen( HINSTANCE hInst, HWND hWnd )
{

	if( !(hInst) || !(hWnd) )	//	�ϐ����������Ă�������
	{
		gdNextStart = 0;
		giSearchPage = 0;

		ZeroMemory( gatLastPtn, sizeof(gatLastPtn) );
	//	giCrLfCnt = 0;

		return S_OK;
	}

	if( ghFindDlg )
	{
		SetForegroundWindow( ghFindDlg );
		return S_FALSE;
	}

	ghFindDlg = CreateDialogParam( hInst, MAKEINTRESOURCE(IDD_FIND_STRING_DLG), hWnd, FindStrDlgProc, 0 );

	ShowWindow( ghFindDlg, SW_SHOW );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�����_�C�����O�̃v���V�[�W��
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK FindStrDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	HWND	hWorkWnd;
	UINT	id;
//	HWND	hWndChild;


	switch( message )
	{
		default:	break;

		case WM_INITDIALOG:
#ifndef SEARCH_HIGHLIGHT
			EnableWindow( GetDlgItem(hDlg,IDB_FIND_CLEAR), FALSE );
			ShowWindow( GetDlgItem(hDlg,IDB_FIND_CLEAR), SW_HIDE );
#endif
			ZeroMemory( atSetPattern, sizeof(atSetPattern) );
			giSetRange = 0;
			gbSetModCrlf = 0;
			gdNextStart = 0;
			giSearchPage = 0;

			//	�R���{�{�b�N�X�ɍ��ړ����
			hWorkWnd = GetDlgItem( hDlg, IDCB_FIND_TARGET );
			ComboBox_InsertString( hWorkWnd, 0, TEXT("�����Ă��") );
			ComboBox_InsertString( hWorkWnd, 1, TEXT("���̃t�@�C���S��") );
	//		ComboBox_InsertString( hWorkWnd, 2, TEXT("�J���Ă���S�Ẵt�@�C��") );������
			ComboBox_SetCurSel(  hWorkWnd, giSetRange );	//	���̌������[�h�𔽉f����
			//	�o���Ƃ��̂͂��Ƃł���

			hWorkWnd = GetDlgItem( hDlg, IDE_FIND_TEXT );
			Edit_SetText( hWorkWnd, gatLastPtn );	//	���̌������e������Γ]�ʂ���
			SetFocus( hWorkWnd );


			return (INT_PTR)FALSE;


		case WM_COMMAND:
			id = LOWORD(wParam);
			hWorkWnd = GetDlgItem( hDlg, IDE_FIND_TEXT );
			switch( id )
			{
				case IDCANCEL:	DestroyWindow( hDlg );	return (INT_PTR)TRUE;
				case IDOK:		FindExecute( hDlg );	return (INT_PTR)TRUE;	//	��������

				case IDM_PASTE:	SendMessage( hWorkWnd, WM_PASTE, 0, 0 );	return (INT_PTR)TRUE;
				case IDM_COPY:	SendMessage( hWorkWnd, WM_COPY,  0, 0 );	return (INT_PTR)TRUE;
				case IDM_CUT:	SendMessage( hWorkWnd, WM_CUT,   0, 0 );	return (INT_PTR)TRUE;
				case IDM_UNDO:	SendMessage( hWorkWnd, WM_UNDO,  0, 0 );	return (INT_PTR)TRUE;

#ifdef SEARCH_HIGHLIGHT
				case IDB_FIND_CLEAR:	FindHighlightOff(  );	return (INT_PTR)TRUE;
#endif
				default:	break;
			}
			break;


		case WM_CLOSE:
			DestroyWindow( hDlg );
			return (INT_PTR)TRUE;

		case WM_DESTROY:
			ghFindDlg = NULL;
			ViewFocusSet(  );	//	�t�H�[�J�X��`��ɖ߂�
			return (INT_PTR)TRUE;

	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�N�Z�����[�^�Œ��ڌ����w��
*/
HRESULT FindDirectly( HINSTANCE hInst, HWND hWnd, INT dCommand )
{
	 BOOLEAN	bOnCrLf = FALSE;
	 INT		cbSize;
	UINT_PTR	cchSize, d;
	LPTSTR		ptText;

	TCHAR	atGetPttn[MAX_PATH];	//	�I����e���m��

	if( IDM_FIND_JUMP_NEXT == dCommand )	//	�e�R�Œʏ팟��
	{
		FindExecute( NULL );
	}
	else if( IDM_FIND_TARGET_SET == dCommand )	//	Ctrl+�e�R�őI��͈͂����񂳂��L�[���[�h�ɂ��Č���
	{
		ZeroMemory( atGetPttn, sizeof(atGetPttn) );

		//	���I���Ȃ牽�����Ȃ��E�Ԃ�l�̃o�C�g�T�C�Y�ɂ̓k���^�[�~�l�[�^�܂ނ̂Œ���
		cbSize = DocSelectTextGetAlloc( D_UNI, (LPVOID *)(&ptText), NULL );
		StringCchLength( ptText, STRSAFE_MAX_CCH, &cchSize );
		if(  0 == cchSize ){		FREE(ptText);	return  E_ABORT;	}
		if( MAX_PATH <= cchSize ){	FREE(ptText);	return  E_ABORT;	}

		//	���s�܂݃`�F�b�N
		for( d = 0; cchSize > d; d++ )
		{
			if( 0x000D == ptText[d] && 0x000A ==ptText[d+1] )
			{
				atGetPttn[d] = TEXT('\\');	d++;
				atGetPttn[d] = TEXT('n');

				bOnCrLf = TRUE;
			}
			else
			{
				atGetPttn[d] = ptText[d];
			}
		}

		//	���������������������
		StringCchCopy( gatLastPtn, MAX_PATH, atGetPttn );
		gbSetModCrlf = bOnCrLf;	//	���s�`�F�b�N
		giSetRange   = 1;	//	���������W�̓t�@�C���S�̌Œ�ɂ����Ⴄ
		gdNextStart  = 0;	//	�V�K�����ł���
		giSearchPage = 0;

		FindExecute( NULL );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�������s
	@param[in]	hDlg	�_�C�����O�n���h��
*/
HRESULT FindExecute( HWND hDlg )
{
//	HWND	hWorkWnd;
//	UINT		dStartPage;
	UINT_PTR	cchSzPtn;
	 INT_PTR	iPage;
	 INT_PTR	iFindTop;
	 INT		dRange;	//	�����͈́@�O�Ł@�P�t�@�C�� �@�L�����Z�����Q�S�J���t�@�C��
	 BOOLEAN	bModCrlf;

//	 BOOLEAN	bSequenSearch = FALSE;	//	�A���T�[�`���ł��邩

	TCHAR	atPattern[MAX_PATH], atBuf[MAX_PATH];


	if( hDlg )
	{
		//�����p���[���m��
		Edit_GetText( GetDlgItem(hDlg,IDE_FIND_TEXT), atBuf, MAX_PATH );
		if( !(atBuf[0]) )	return  E_ABORT;	//	�󕶎���Ȃ牽�����Ȃ�

		//	���������s�A���������ɂ��邩
		bModCrlf = IsDlgButtonChecked( hDlg, IDCB_MOD_CRLF_YEN );

		//	�����͈�	�O�Ł@�P�t�@�C��
		dRange = ComboBox_GetCurSel( GetDlgItem(hDlg,IDCB_FIND_TARGET) );

		//	�����������S�ē����Ȃ�A��������
		if( !StrCmp( atSetPattern, atBuf ) && (gbSetModCrlf == bModCrlf) && (giSetRange == dRange) )
		{
	//		bSequenSearch = TRUE;
		}
		else	//	�Ⴄ�Ȃ�擪����
		{
			gdNextStart = 0;
			giSearchPage = 0;
		}

		StringCchCopy( atSetPattern, MAX_PATH, atBuf );
		gbSetModCrlf = bModCrlf;
		giSetRange = dRange;

		StringCchCopy( gatLastPtn, MAX_PATH, atBuf );	//	���Ƀ_�C�����O�J�������̕\���p
	}
	else	//	�e�R�Œ��ŗ���
	{
		if( 0 == gatLastPtn[0] )	return  E_ABORT;	//	�������Ȃ�

		//	���O�̐ݒ�𗬗p	
		StringCchCopy( atBuf, MAX_PATH, gatLastPtn );
		bModCrlf = gbSetModCrlf;
		dRange = giSetRange;
	}

	//	�����p���[���m��
	cchSzPtn = SearchPatternStruct( atPattern, MAX_PATH, atBuf, bModCrlf );
#if 0
	ZeroMemory( atPattern, sizeof(atPattern) );
//	giCrLfCnt = 0;	//	���s�J�E���g�E�ʏ�0����
	if( bModCrlf )	//	�G�X�P�[�v�V�[�P���X��W�J
	{
		for( d = 0, h = 0; MAX_PATH > d; d++, h++ )
		{
			atPattern[h] = atBuf[d];
			if( 0x005C == atBuf[d] )	//	0x005C�́�
			{
				d++;
				if( TEXT('n') ==  atBuf[d] )	//	���s�w���ł���ꍇ
				{
					atPattern[h] = TEXT('\r');	h++;
					atPattern[h] = TEXT('\n');
				//	giCrLfCnt++;	//	���s�J�E���g���₷
				}
			}
			if( 0x0000 == atBuf[d] )	break;
		}
	}
	else
	{
		StringCchCopy( atPattern, MAX_PATH, atBuf );
	}

	StringCchLength( atPattern, MAX_PATH, &cchSzPtn );
#endif

#ifdef SEARCH_HIGHLIGHT
#error �@�\�������̂ŁA�n�C���C�g�͎g�p�s�ł���
		FindHighlightOff(  );	//	��̃p���[���j��
#endif


	if( dRange )	//	�S�Ō�������������肵��
	{
		iPage = DocNowFilePageCount(  );	//	�Ő��m��

		do{
			iFindTop = FindPageSearch( atPattern, giSearchPage, gitFileIt );	//	�Ώەł�����
			if(  0 <= iFindTop )	//	����
			{
				if( giSearchPage != gixFocusPage )	//	�Ȃ��łłȂ��Ȃ�Y���̕łɈړ�����
				{
					DocPageChange( giSearchPage );	//	�ňړ��EgixFocusPage �����������
				}
				FindPageSelectSet( iFindTop, cchSzPtn, gixFocusPage, gitFileIt );	//	���̏ꏊ�ɃJ�[�\���W�����v���đI����Ԃɂ���
				gdNextStart = iFindTop + cchSzPtn;
				break;
			}
			else	//	���̕łɂ͖�������
			{
				 giSearchPage++;	//	���̕łɈړ�����
				gdNextStart = 0;	//	���͂܂��擪���猟��
				if( iPage <=  giSearchPage )	//	���[�������������
				{
					giSearchPage = 0;
					break;
				}
			}

		}while( 0 > iFindTop );	//	������܂ŕňړ�����
	}
	else	//	�Ȃ��ł̂�
	{
		iFindTop = FindPageSearch( atPattern, gixFocusPage, gitFileIt );	//	�P�Ō���
		if( 0 <=  iFindTop )	//	�擪����̕�����
		{
			FindPageSelectSet( iFindTop, cchSzPtn, gixFocusPage, gitFileIt );	//	���̏ꏊ�ɃJ�[�\���W�����v���đI����Ԃɂ���
			gdNextStart = iFindTop + cchSzPtn;
		}
		else{	gdNextStart = 0;	}	//	�擪����
	}


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�����p�^�[�����m�肷��
	@param[out]	ptDest		�p���[�������o�b�t�@
	@param[in]	cchSize		�o�b�t�@�T�C�Y
	@param[in]	ptSource	��������
	@param[in]	bCrLf		�G�X�P�[�v�V�[�P���X��W�J���邩
	@return	������
*/
UINT_PTR SearchPatternStruct( LPTSTR ptDest, UINT_PTR cchSize, LPTSTR ptSource, BOOLEAN bCrLf )
{
	UINT_PTR	d, h;
	UINT_PTR	cchSzPtn;


	ZeroMemory( ptDest, sizeof(TCHAR) * cchSize );

//	giCrLfCnt = 0;	//	���s�J�E���g�E�ʏ�0����

	if( bCrLf )	//	�G�X�P�[�v�V�[�P���X��W�J
	{
		for( d = 0, h = 0; cchSize > d; d++, h++ )
		{
			ptDest[h] = ptSource[d];
			if( 0x005C == ptSource[d] )	//	0x005C�́�
			{
				d++;
				if( TEXT('n') ==  ptSource[d] )	//	���s�w���ł���ꍇ
				{
					ptDest[h] = TEXT('\r');	h++;
					ptDest[h] = TEXT('\n');
				//	giCrLfCnt++;	//	���s�J�E���g���₷
				}
			}
			if( 0x0000 == ptSource[d] )	break;
		}
	}
	else
	{
		StringCchCopy( ptDest, cchSize, ptSource );
	}

	StringCchLength( ptDest, cchSize, &cchSzPtn );


	return cchSzPtn;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��p���[�����A�w��t�@�C���́A�w��łŌ���
	@param[in]	ptPattern	�����p���[���ENULL�͕s��
	@param[in]	iTgtPage	�Ώە�
	@param[in]	itFile		�t�@�C���C�e���[�^
	@return	���q�b�g�Ȃ�|�P�A�q�b�g�Ȃ�A�擪����̕������E���s�͂Q��������
*/
INT_PTR FindPageSearch( LPTSTR ptPattern, INT iTgtPage, FILES_ITR itFile )
{
	 INT		dCch;//, dLeng;
	 INT_PTR	iRslt;
	 INT_PTR	dBytes;
	UINT_PTR	cchSize, cchSzPtn;
	LPTSTR		ptPage, ptCaret, ptFind = NULL;

//	TCHAR	ttBuf;

	TRACE( TEXT("PAGE[%d] SEARCH"), iTgtPage );

	if( !(ptPattern)  ){	return -1;	}	//	NULL�s��

	StringCchLength( ptPattern, MAX_PATH, &cchSzPtn );

	//	�f�B���C���Ă���΃`�F�b�N���Ȃ��Ă�������Ȃ킯�Ȃ�
//	if( PageIsDelayed( itFile, iTgtPage ) ){	return   -1;	}


	//	�őS�̊m��
	dBytes = DocPageTextGetAlloc( itFile, iTgtPage, D_UNI, (LPVOID *)(&ptPage), FALSE );
	StringCchLength( ptPage, STRSAFE_MAX_CCH, &cchSize );

	ptCaret = ptPage;

	ptCaret += gdNextStart;	//	���O�̌����ʒu�܂ŃI�t�Z�b�g

	iRslt = -1;

#ifdef SEARCH_HIGHLIGHT
	//	��������
	dLeng = 0;
	do
	{
		ptFind = FindStringProc( ptCaret, ptPattern, &dCch );	//	�����{�́E�G�f�B�^��
		if( !(ptFind) ){	break;	}	//	�q�b�g���Ȃ�����

		dLeng += dCch;	//	�����ʒu�E�O�C���f�b�N�X
		ttBuf = ptPage[dLeng];

		//	�q�b�g���������ɐF��t����
		FindPageHighlightSet( dLeng, cchSzPtn, iTgtPage, itFile );	//	���ŊY���̈�̍ĕ`��܂Ŏw�肵�Ă�

		hRslt = S_OK;

		ptCaret = ptFind;
		ptCaret++;

		dLeng++;

	}while( *ptCaret );
#else
	ptFind = FindStringProc( ptCaret, ptPattern, &dCch );	//	�����{�́E�G�f�B�^��
	if( ptFind )	//	�Ȃ񂩂�����
	{
		dCch += gdNextStart;	//	�I�t�Z�b�g�ʂ𑫂��Ă���

	//	FindPageSelectSet( dCch, cchSzPtn, iTgtPage, itFile );	//	���̏ꏊ�ɃJ�[�\���W�����v���đI����Ԃɂ���
		//�O�ł��悤�ɂ���

	//	iRslt = dCch + cchSzPtn;
		iRslt = dCch;
	}

#endif

	FREE(ptPage);

	return iRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��t�@�C���̎w��ł̎w�蕶���ʒu����w�蕶������I����Ԃɂ���B���s�R�[�h�܂ށB
	@param[in]	iOffset	�Ő擪����̕������E���s�R�[�h�܂ށB
	@param[in]	iRange	�����Ώۂ̕�����
	@param[in]	iPage	�Ώە�
	@param[in]	itFile	�t�@�C���C�e���[�^
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FindPageSelectSet( INT iOffset, INT iRange, INT iPage, FILES_ITR itFile )
{
	UINT_PTR	ln, iLetters;//, iLines;
	 INT_PTR	dMozis;
	 INT		iTotal, iDot, iLnTop, iSlide, mz, iNext, iWid = 0;
	 INT		iEndTotal, iEndOffset;
//	RECT		inRect;

	LINE_ITR	itLine, itLnEnd;

	itLine  = itFile->vcCont.at( iPage ).ltPage.begin();
	itLnEnd = itFile->vcCont.at( iPage ).ltPage.end();

	iEndOffset = iOffset + iRange;
	iEndTotal = 0;
	iTotal = 0;
	iLnTop = 0;
	for( ln = 0; itLnEnd != itLine; itLine++, ln++ )
	{
		dMozis = itLine->vcLine.size( );	//	���̍s�̕������m�F����
		iLetters = dMozis + 2;	//	���s�R�[�h

		iTotal += iLetters;

		if( iOffset < iTotal )	//	�s���[�܂ł̕��������I�t�Z�b�g��������������A���̍s�Ɋ܂܂��
		{
			iSlide = iOffset - iLnTop;	//	���̍s�擪����̕�����
			//	�������s���猟����������AiSlide = dMozis �ɂȂ�
			iNext = 0;	//	���s���L��ꍇ�̎c�蕶����

			//	�����ŉ��s�̊������ݏ󋵂��m�F���āA���̍s���Ԃ�Ƃ��`�F�b�N�H
			//if( dMozis < (iSlide + iRange) )	//	�����Ȃ�A���s���͂ݏo���Ă�
			//{
			//	iNext  = iRange;
			//	iRange = dMozis - iSlide;	//	�c�蕶����
			//	iNext -= iRange;	//	���s���܂߂��c�蕶����
			//}

			iDot = 0;	//	�����܂ł̃h�b�g�������ߍ���
			for( mz = 0; iSlide > mz; mz++ )	//	�Y�������܂Ői�߂ăh�b�g�����߂Ƃ�
			{
				//	�������s���猟���Ȃ炱�ꂪ����
				if( dMozis <=  mz ){	iDot += iWid;	break;	}

				iDot += itLine->vcLine.at( mz ).rdWidth;

				iWid  = itLine->vcLine.at( mz ).rdWidth;	//	���̕����̕�
			}

			//	�Y���͈͂�I����Ԃɂ���
			DocPageSelStateToggle(  FALSE );	//	��U�I����Ԃ͉���

			ViewPosResetCaret( iDot, ln );	//	�J�[�\���������Ɉړ�
			ViewSelMoveCheck( FALSE );	//	�͈͑I���J�n���
			ViewSelPositionSet( NULL );

			//	�͈͑I���AViewSelAreaSelect ���Q�l����

			break;
		}

		iLnTop += iLetters;

		iEndTotal += iLetters;	//	�������e�̏I�[���o�p
	}

	//	���[�ʒu����J�n
	for( ; itLnEnd != itLine; itLine++, ln++ )	//	�I�t�Z�b�g�������ꂽ�s����J�n
	{
		dMozis = itLine->vcLine.size( );	//	���̍s�̕������m�F����
		iLetters = dMozis + 2;	//	���s�R�[�h

		iEndTotal += iLetters;

		if( iEndOffset < iEndTotal )	//	�s���[�܂ł̕��������I�t�Z�b�g��������������A���̍s�Ɋ܂܂��
		{
			iSlide = iEndOffset - iLnTop;	//	���̍s�擪����̕�����

			iWid = 0;
			iDot = 0;	//	�����܂ł̃h�b�g�������ߍ���
			for( mz = 0; iSlide > mz; mz++ )	//	�Y�������܂Ői�߂ăh�b�g�����߂Ƃ�
			{
				//	�������s���猟���Ȃ炱�ꂪ����
				if( dMozis <=  mz ){	iDot += iWid;	break;	}

				iDot += itLine->vcLine.at( mz ).rdWidth;

				iWid  = itLine->vcLine.at( mz ).rdWidth;	//	���̕����̕�
			}

			ViewPosResetCaret( iDot, ln );	//	�J�[�\���������Ɉړ�
			//	ViewDrawCaret( gdDocXdot, gdDocLine, 1 );	//	�����ŃL�����b�g���ړ�

			ViewSelMoveCheck( TRUE );
			ViewSelPositionSet( NULL );	//	�ړ������ʒu���L�^

			break;
		}

		iLnTop += iLetters;
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------



#ifdef SEARCH_HIGHLIGHT
/*!
	�w��t�@�C���̎w��ł̎w�蕶���ʒu����w�蕶�������n�C���C�g�w��ɂ���B���s�R�[�h�܂ށB
	@param[in]	iOffset	�Ő擪����̕������E���s�R�[�h�܂ށB
	@param[in]	iRange	�n�C���C�g������
	@param[in]	iPage	�Ώە�
	@param[in]	itFile	�t�@�C���C�e���[�^
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FindPageHighlightSet( INT iOffset, INT iRange, INT iPage, FILES_ITR itFile )
{
	UINT_PTR	ln, iLetters;//, iLines;
	INT_PTR		dMozis;
	INT			iTotal, iDot, iLnTop, iSlide, mz, iNext, iWid = 0;
	RECT		inRect;

	LINE_ITR	itLine, itLnEnd;

	itLine  = itFile->vcCont.at( iPage ).ltPage.begin();
	itLnEnd = itFile->vcCont.at( iPage ).ltPage.end();

	iTotal = 0;
	iLnTop = 0;
	for( ln = 0; itLnEnd != itLine; itLine++, ln++ )
	{
		inRect.top    = ln * LINE_HEIGHT;
		inRect.bottom = inRect.top + LINE_HEIGHT;
		inRect.left   = 0;
		inRect.right  = 0;

		dMozis = itLine->vcLine.size( );	//	���̍s�̕������m�F����
		iLetters = dMozis + 2;	//	���s�R�[�h

		iTotal += iLetters;

		if( iOffset < iTotal )	//	�s���[�܂ł̕��������I�t�Z�b�g��������������A���̍s�Ɋ܂܂��
		{
			iSlide = iOffset - iLnTop;	//	���̍s�擪����̕�����
			//	�������s���猟����������AiSlide = dMozis �ɂȂ�
			iNext = 0;	//	���s���L��ꍇ�̎c�蕶����

			//	�����ŉ��s�̊������ݏ󋵂��m�F���āA���̍s���Ԃ�Ƃ��`�F�b�N�H
			if( dMozis < (iSlide + iRange) )	//	�����Ȃ�A���s���͂ݏo���Ă�
			{
				iNext  = iRange;
				iRange = dMozis - iSlide;	//	�c�蕶����
				iNext -= iRange;	//	���s���܂߂��c�蕶����
			}

			iDot = 0;	//	�����܂ł̃h�b�g�������ߍ���
			for( mz = 0; iSlide > mz; mz++ )	//	�Y�������܂Ői�߂ăh�b�g�����߂Ƃ�
			{
				//	�������s���猟���Ȃ炱�ꂪ����
				if( dMozis <=  mz ){	iDot += iWid;	break;	}

				iDot += itLine->vcLine.at( mz ).rdWidth;

				iWid  = itLine->vcLine.at( mz ).rdWidth;	//	���̕����̕�
			}

			for(  ; (iSlide+iRange) > mz; mz++ )
			{
				//	���s�܂ł߂荞�ނȂ炱��������
				if( dMozis <=  mz ){	break;	}

				iWid = itLine->vcLine.at( mz ).rdWidth;

				inRect.left  = iDot;
				inRect.right = iDot + iWid;

				itLine->vcLine.at( mz ).mzStyle |= CT_FINDED;
				ViewRedrawSetRect( &inRect );

				iDot += iWid;
			}

			if( 0 <	 iNext )	//	���s������Ȃ琔�l������
			{
				inRect.right += 20;

				itLine->dStyle |= CT_FINDRTN;
				ViewRedrawSetRect( &inRect );

				iNext -=  2;	//	���s�������ւ炵��

			}

			//	�K�v�ɉ����āA�����ŏI������莟�̍s�̏����ɂ�������
			if( 0 >= iNext ){	break;	}
			else
			{
				iOffset += (2 + iRange);	//	���s�������{���̍s�Ŏg�����������i�߂�
				iRange  = iNext;	//	�c�蕶��������
			}
		}

		iLnTop += iLetters;
	}


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�S���n�C���C�g�n�e�e
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT FindHighlightOff( VOID )
{
	UINT_PTR	dPage, d;

	dPage = DocNowFilePageCount( );

	for( d = 0; dPage > d; d++ )
	{
		FindPageHighlightOff( d, gitFileIt );
	}

	//	���ݓ��e��������
	FindDialogueOpen( NULL, NULL );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��t�@�C���̎w��ł̌����n�C���C�g��OFF����
	@param[in]	iPage	�Ώە�
	@param[in]	itFile	�t�@�C���C�e���[�^
	@return		���ɂȂ�
*/
INT FindPageHighlightOff( INT iPage, FILES_ITR itFile )
{
	UINT_PTR	ln;//, iLines, iLetters, mz;
//	UINT		dStyle;
//	INT			iDot, iWid;
//	RECT		inRect;

	LINE_ITR	itLine, itLnEnd;


	if( 0 >  iPage )	return 0;	//	����ȏ󋵉��ł͏������Ȃ�

	ZeroMemory( gatLastPtn, sizeof(gatLastPtn) );

	itLine  = itFile->vcCont.at( iPage ).ltPage.begin();
	itLnEnd = itFile->vcCont.at( iPage ).ltPage.end();

	for( ln = 0; itLnEnd != itLine; itLine++, ln++ )
	{
		FindLineHighlightOff( ln, itLine );	//	����REDRAW�������Ă�
	}

//	ViewRedrawSetLine( -1 );	//	��ʕ\���X�V

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��s�̌����n�C���C�g��OFF����
	@param[in]	iLine	�s���E�O�C���f�b�N�X
	@param[in]	itFile	�s�C�e���[�^
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT FindLineHighlightOff( UINT iLine, LINE_ITR itLine )
{
	UINT_PTR	iLetters, mz;//, iLines, ln;
	UINT		dStyle;
	INT			iDot, iWid;
	RECT		inRect;


	iDot = 0;	//	�����܂ł̃h�b�g�������ߍ���
	inRect.top    = iLine * LINE_HEIGHT;
	inRect.bottom = inRect.top + LINE_HEIGHT;

	iLetters = itLine->vcLine.size();
	//	�땶�����A�S�����`�F�L���Ă���
	for( mz = 0; iLetters > mz; mz++ )
	{
		//	���O�̏��
		dStyle = itLine->vcLine.at( mz ).mzStyle;
		iWid   = itLine->vcLine.at( mz ).rdWidth;

		inRect.left  = iDot;
		inRect.right = iDot + iWid;

		itLine->vcLine.at( mz ).mzStyle &= ~CT_FINDED;
		if( dStyle & CT_FINDED )	ViewRedrawSetRect( &inRect );

		iDot += iWid;
	}

	//	��s�I������疖���󋵊m�F�B���s�E�{�����[�ɉ��s�͂Ȃ��E�I���̂Ƃ��̂�
	dStyle = itLine->dStyle;
	inRect.left  = iDot;
	inRect.right = iDot + 20;	//	���s�`��G���A�E��̂��ꂭ�炢

	itLine->dStyle &=  ~CT_FINDRTN;
	if( dStyle & CT_FINDRTN )	ViewRedrawSetRect( &inRect );


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

#endif

#if 0
/*!
	�����ʒu�փW�����v
	@param[in]	dMode	�O���ց@�P�O��
	@param[in]	pxDot	�J�[�\���h�b�g�ʒu�E����������߂�
	@param[in]	pyLine	�J�[�\���s�E����������߂�
	@param[in]	pMozi	�L�����b�g�̍����̕������E����������߂�
	@return		INT		���F�G���[�@�O�F�q�b�g�����@�P�`�F�q�b�g����
*/
INT FindStringJump( UINT dMode, PINT pXdot, PINT pYline, PINT pMozi )
{
	INT			iXdot, iYline, iMozi;
	BOOLEAN		bStart, bBegin;
	INT_PTR		dTotalPage, dTotalLine;
	PAGE_ITR	itPage;	//	�ł����Ɍ��Ă���
	LINE_ITR	itLine;	//	�s�����Ɍ��Ă���
	LETR_ITR	itMozi;	//	���������Ɍ��Ă���

	if( !(pXdot) || !(pYline) || !(pMozi) ){	return -1;	}

	//	�ł͂ݏo���`�F�L
	dTotalPage = DocNowFilePageCount();
	if( !(0 <= gixFocusPage && gixFocusPage < dTotalPage) ){	return -1;	}
	//	���̕łɒ���
	itPage = gitFileIt->vcCont.begin();
	std::advance( itPage, gixFocusPage );

	iXdot  = *pXdot;
	iYline = *pYline;
	iMozi  = *pMozi;


	//	�s�͂ݏo���`�F�L
	dTotalLine = itPage->ltPage.size();
	if( !(0 <= iYline && iYline < dTotalLine) ){	return -1;	}
	//	���̍s�ɒ���
	itLine = itPage->ltPage.begin();
	std::advance( itLine, iYline );

	itMozi = itLine->vcLine.begin();
	std::advance( itMozi, iMozi );

	bStart = TRUE;	bBegin = TRUE;
	for( ; itPage->ltPage.end() != itLine; itLine++ )
	{
		//	�ŏ��͂��̂܂܁A���񂩂�͂��̍s�̐擪�������猩�Ă���
		if( bStart ){	bStart =  FALSE;	}
		else{	itMozi = itLine->vcLine.begin();	}

		for( ;itLine->vcLine.end() != itMozi; itMozi++ )
		{
			if( itMozi->mzStyle & CT_FINDED )	//	�q�b�g
			{
				if( !(bBegin)  )	//	�����J�n�ʒu�͖̂�������
				{
					*pXdot  = iXdot;
					*pYline = iYline;
					*pMozi  = iMozi;

					ViewDrawCaret( iXdot, iYline, TRUE );	//	�L�����b�g���ړ�

					return 1;
					//	�ł܂����͂ǂ������
				}
			}
			else{	bBegin =  FALSE;	}
			//	

			iXdot += itMozi->rdWidth;
			iMozi++;
		}

		iYline++;	//	�s�ԍ��͑��₷
		iXdot = 0;
		iMozi = 0;
	}

	//	�ł܂����͂ǂ������

	return 0;
}
//-------------------------------------------------------------------------------------------------
#endif

#ifdef SEARCH_HIGHLIGHT
/*!
	���ł��Č����E��ʍĕ`��Ƃ�
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT FindNowPageReSearch( VOID )
{
	FindHighlightOff(  );

	FindPageSearch( gatLastPtn, gixFocusPage, gitFileIt );	//	�d��

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�x���Ǎ��ł��J���Ƃ��ɁA�����n�C���C�c�̖ʓ|����
	@param[in]	iTgtPage	�ΏەŔԍ�
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT FindDelayPageReSearch( INT iTgtPage )
{

	//	�S�̌����łȂ����A���������񂪋�Ȃ疳�����Ă�낵
	if(  1 != giSetRange || NULL == gatLastPtn[0] ){	return  E_ABORT;	}


	//	�Ƃ肠������Search
	FindPageSearch( NULL, iTgtPage, gitFileIt );


	return S_OK;
}
//-------------------------------------------------------------------------------------------------
#endif

#if 0
/*!
	�������}���폜���ꂽ�Ƃ��ɁA���̕�����Check������
	@param[in]	iTgtLine	�Ώۍs�ԍ�
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT FindTextModifyLine( INT iTgtLine )
{
//�Y���s�Ɖ��s���y�Ԕ͈͂̍s�̂݁A��������`�F�b�N�O���āA�Y���͈͂̍s�����`�F�L
//�R�s�y���ꂽ�ꍇ�́H�@�}���폜�������s���Ă���`�F�b�N����Ȃ�A�e�����咍��

	//	giCrLfCnt

	//�s���m�F�E�͂ݏo���Ȃ��悤��


	return S_OK;
}
//-------------------------------------------------------------------------------------------------
#endif

#endif	//	FIND_STRINGS
