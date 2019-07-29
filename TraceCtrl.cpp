/*! @file
	@brief �g���X�@�\�̖ʓ|���܂��E�摜�ۑ��̖ʓ|�����܂�
	���̃t�@�C���� TraceCtrl.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/07/19
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


//	�摜�ۑ��@�\�ɂ��g�����璍��
#define IMGCTL_RUNTIME
#include "imgctl.h"
//-------------------------------------------------------------------------------------------------

/*
�X���C�_�ƃX�N���[���o�[�̃_�C�����O���\�[�X�̎g����
    CONTROL         "",IDC_SLIDER1,"msctls_trackbar32",TBS_BOTH | TBS_NOTICKS | WS_TABSTOP,14,66,100,15
    SCROLLBAR       IDC_SCROLLBAR1,134,68,82,11
*/
#define TRC_SCROLLBAR

#define TRC_POSITION_RANGE	2000	//	�ʒu���킹�|�P�O�O�O�`�P�O�O�O
#define TRC_POSITION_OFFSET	1000	//	���ꂪ��ʒu

#define TRC_CONTRA_RANGE	 510	//	�R���g���X�g�́|�Q�T�T�`�Q�T�T
#define TRC_CONTRA_OFFSET	 255	//	����łO�i���얳���j

#define TRC_GAMMA_RANGE		3000	//	�K���}�@�O�`�R�D�O
#define TRC_GAMMA_OFFSET	1000	//	����łP�D�O�i���얳���j�E���̒l���̂܂܎w��ł���

#define TRC_GRAYMOPH_RANGE	 255	//	�W�F���@�O�`�Q�T�T

#define TRC_ZOOM_RANGE		 250	//	�g��k���͂T�O���`�R�O�O��
#define TRC_ZOOM_OFFSET		  50	//	����łP�O�O���i���얳���j�E�����ĂP�O�O�Ŋ���

#define TRC_TURN_RANGE		 359	//	��T����΂���
//-------------------------------------------------------------------------------------------------

static HMODULE		ghImgctl;		//!<	ImgCtl.dll�̃n���h��

TODIB		gpifToDIB;		//!<	�t�@�C��������C���[�W�Q�b�g
DELETEDIB	gpifDeleteDIB;	//!<	�摜�f�[�^��j������
HEADDIB		gpifHeadDIB;	//!<	�摜�̃f�[�^���Q�b�c
COPYDIB		gpifCopyDIB;	//!<	
TURNDIBEX	gpifTurnDIBex;	//!<	�摜��C�ӂ̊p�x�ɉ�]����
GRAYDIB		gpifGrayDIB;	//!<	�摜��W�F������
GAMMADIB	gpifGammaDIB;	//!<	�K���}�␳
CONTRASTDIB	gpifContrastDIB;//!<	�R���g���X�g�␳

DIBTODCEX	gpifDIBtoDCex;	//!<	�摜�f�[�^���f�B�o�C�X�R���e�L�X�c�ɂ؂�����

DCTODIB		gpifDCtoDIB;	//!<	�f�o�C�X�R���e�L�X�g���摜�ł�����
DIBTOBMP	gpifDIBtoBMP;	//!<	
//DIBTOJPG	gpifDIBtoJPG;	//!<	
DIBTOPNG	gpifDIBtoPNG;	//!<	�摜�f�[�^��PNG�`���ŃA�E�c

static  HDIB	ghOutDib;				//!<	�t�@�C���o�͗p
static  HDIB	ghImgDib, ghOrigDib;	//!<	�g���X�\���p
static  SIZE	gstImgSize;				//!<	�ǂݍ��񂾉摜�̃T�C�Y

static HBRUSH	gMoziClrBrush;	//!<	�����F�u���V

static  HWND	ghTraceDlg;		//!<	�g���X�@�\���[�_���X�_�C�����O�n���h��

static BOOLEAN	gbThumbUse;		//!<	�摜�I�[�|���_�C�����[�O�ŃT���l�[����L����

//	�S�ăX���C�_�̐��l���L�^����
static TRACEPARAM	gstTrcPrm;

static BOOLEAN	gbOnView;		//!<	��O�ŕ\��
//-------------------------------------------------------------------------------------------------

INT_PTR	CALLBACK TraceCtrlDlgProc( HWND, UINT, WPARAM, LPARAM );

HRESULT	TraceImageFileOpen( HWND );
HRESULT	TraceMoziColourChoice( HWND );

HRESULT	TraceRedrawIamge( VOID );

INT_PTR	TraceOnScroll( HWND, HWND, UINT, INT );
//-------------------------------------------------------------------------------------------------

/*!
	ImgCtl.dll�����[�h����
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	bMode	��O�쐬�@�O�j��
*/
INT TraceInitialise( HWND hWnd, UINT bMode )
{
	if( bMode )
	{
		ghTraceDlg = NULL;
		ghImgDib = NULL;
		ghOrigDib = NULL;

		ghImgctl = LoadLibrary( TEXT("imgctl.dll") );

//�g���XDIALOGUE�A�g��k�����Ȃ�

		gpifToDIB       = (TODIB)GetProcAddress( ghImgctl, "ToDIB" );
		gpifDeleteDIB   = (DELETEDIB)GetProcAddress( ghImgctl, "DeleteDIB" );
		gpifHeadDIB     = (HEADDIB)GetProcAddress( ghImgctl, "HeadDIB" );

		gpifCopyDIB     = (COPYDIB)GetProcAddress( ghImgctl, "CopyDIB" );

		gpifTurnDIBex   = (TURNDIBEX)GetProcAddress( ghImgctl, "TurnDIBex" );
		gpifGrayDIB     = (GRAYDIB)GetProcAddress( ghImgctl, "GrayDIB" );
		gpifGammaDIB    = (GAMMADIB)GetProcAddress( ghImgctl, "GammaDIB" );
		gpifContrastDIB = (CONTRASTDIB)GetProcAddress( ghImgctl, "ContrastDIB" );

		gpifDIBtoDCex   = (DIBTODCEX)GetProcAddress( ghImgctl, "DIBtoDCex" );
		//	����ex�łȂ��Ƃ�����

		//	�摜de�ۑ��p
		gpifDCtoDIB  = (DCTODIB)GetProcAddress( ghImgctl, "DCtoDIB" );
		gpifDIBtoBMP = (DIBTOBMP)GetProcAddress( ghImgctl, "DIBtoBMP" );
	//	gpifDIBtoJPG = (DIBTOJPG)GetProcAddress( ghImgctl, "DIBtoJPG" );
		gpifDIBtoPNG = (DIBTOPNG)GetProcAddress( ghImgctl, "DIBtoPNG" );



		gstTrcPrm.stOffsetPt.x = TRC_POSITION_OFFSET;
		gstTrcPrm.stOffsetPt.y = TRC_POSITION_OFFSET;

		gstTrcPrm.dContrast = TRC_CONTRA_OFFSET;
		gstTrcPrm.dGamma    = TRC_GAMMA_OFFSET;
		gstTrcPrm.dGrayMoph = 0;
		gstTrcPrm.dZooming  = TRC_ZOOM_OFFSET;
		gstTrcPrm.dTurning  = 0;

		gstTrcPrm.bUpset  = BST_UNCHECKED;
		gstTrcPrm.bMirror = BST_UNCHECKED;

		gstTrcPrm.dMoziColour = ViewMoziColourGet( NULL );

		InitTraceValue( INIT_LOAD, &gstTrcPrm );

		gbOnView = TRUE;

		gbThumbUse = TRUE;
	}
	else
	{
		if( ghImgDib  ){	gpifDeleteDIB( ghImgDib );	ghImgDib = NULL;	}
		if( ghOrigDib ){	gpifDeleteDIB( ghOrigDib );	ghOrigDib = NULL;	}

		if( ghOutDib  ){	gpifDeleteDIB( ghOutDib );	ghOutDib = NULL;	}

		if( ghImgctl )		FreeLibrary( ghImgctl );
		if( ghTraceDlg )	DestroyWindow( ghTraceDlg );
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------

/*!
	���[�_���X�g���X�_�C�����O���J��������݂�ł�����
	@param[in]	hInst	�A�v���̎���
	@param[in]	hWnd	���C���E�C���h�E�n���h���ł���悤��
*/
HRESULT TraceDialogueOpen( HINSTANCE hInst, HWND hWnd )
{
	HWND	hDktpWnd;
	LONG	x;
	RECT	rect, dtRect, trRect;

	if( !(ghImgctl) )	return E_HANDLE;	//	DLL����ł���A�E�c�I

	if( ghTraceDlg )
	{
		//SetForegroundWindow( ghTraceDlg );
		PostMessage( ghTraceDlg, WM_CLOSE, 0, 0 );
		return S_OK;
	}

	GetWindowRect( hWnd, &rect );	//	���C�����̃X�N���[�����W
	
	//	�X�N���[���o�[�ɂ��Ă݂�e�X�g
#ifdef TRC_SCROLLBAR
	ghTraceDlg = CreateDialogParam( hInst, MAKEINTRESOURCE(IDD_TRACEADJUST_DLG2), hWnd, TraceCtrlDlgProc, 0 );
#else
	ghTraceDlg = CreateDialogParam( hInst, MAKEINTRESOURCE(IDD_TRACEADJUST_DLG), hWnd, TraceCtrlDlgProc, 0 );
#endif
	GetClientRect( ghTraceDlg, &trRect );	//	�g���X���䑋�̃T�C�Y

	//	�f�B�X�N�g�b�v����͂ݏo���Ȃ��悤��
	hDktpWnd = GetDesktopWindow(  );
	GetWindowRect( hDktpWnd, &dtRect );

	x = dtRect.right - rect.right;	//	�E�̗]�T�m�F
	if( trRect.right >  x ){	rect.right = dtRect.right - trRect.right;	}
	//	����Ȃ��悤�Ȃ�A�\���ʒu���I�t�Z�b�g���Ă���

	SetWindowPos( ghTraceDlg, HWND_TOP, rect.right, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW );

	if( ghOrigDib ){	ViewRedrawSetLine( -1 );	}

	MenuItemCheckOnOff( IDM_TRACE_MODE_ON, TRUE );

	AppTitleTrace( TRUE );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g���X�R���g���[���_�C�����O�̃v���V�[�W��
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	message		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam		�ǉ��̏��P
	@param[in]	lParam		�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK TraceCtrlDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	HWND	hWorkWnd;
	UINT	id;
	HDC		hdc;
	HWND	hWndChild;
	RECT	rect;
//	LONG	lRslt;
	COLORREF	caretColour;

#ifdef TRC_SCROLLBAR
	SCROLLINFO	stSclInfo;
#endif

	switch( message )
	{
		default:	break;

		case WM_INITDIALOG:
			gMoziClrBrush = CreateSolidBrush( gstTrcPrm.dMoziColour );

			hWorkWnd = GetDlgItem( hDlg, IDSL_TRC_HRIZ_POS );
#ifdef TRC_SCROLLBAR
			ZeroMemory( &stSclInfo, sizeof(SCROLLINFO) );
			stSclInfo.cbSize = sizeof(SCROLLINFO);
			stSclInfo.fMask  = SIF_RANGE;

			stSclInfo.nMax   = TRC_POSITION_RANGE;	//	nMin���O
			SetScrollInfo( hWorkWnd, SB_CTL, &stSclInfo, TRUE );
#else
			SendMessage( hWorkWnd, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(0, TRC_POSITION_RANGE) );	//	
			SendMessage( hWorkWnd, TBM_SETPOS, TRUE , gstTrcPrm.stOffsetPt.x );	//	
			SendMessage( hWorkWnd, TBM_SETPAGESIZE, 0, 25 );
#endif
			TraceOnScroll( hDlg, hWorkWnd, TB_THUMBPOSITION, gstTrcPrm.stOffsetPt.x );

			hWorkWnd = GetDlgItem( hDlg, IDSL_TRC_VART_POS );
#ifdef TRC_SCROLLBAR
			stSclInfo.nMax   = TRC_POSITION_RANGE;	//	nMin���O
			SetScrollInfo( hWorkWnd, SB_CTL, &stSclInfo, TRUE );
#else
			SendMessage( hWorkWnd, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(0, TRC_POSITION_RANGE) );	//	
			SendMessage( hWorkWnd, TBM_SETPOS, TRUE , gstTrcPrm.stOffsetPt.y );	//	
			SendMessage( hWorkWnd, TBM_SETPAGESIZE, 0, 25 );
#endif
			TraceOnScroll( hDlg, hWorkWnd, TB_THUMBPOSITION, gstTrcPrm.stOffsetPt.y );

			hWorkWnd = GetDlgItem( hDlg, IDSL_TRC_CONTRAST );
#ifdef TRC_SCROLLBAR
			stSclInfo.nMax   = TRC_CONTRA_RANGE;	//	nMin���O
			SetScrollInfo( hWorkWnd, SB_CTL, &stSclInfo, TRUE );
#else
			SendMessage( hWorkWnd, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(0, TRC_CONTRA_RANGE) );	//	�R���g���X�g
			SendMessage( hWorkWnd, TBM_SETPOS, TRUE , gstTrcPrm.dContrast );	//	
			SendMessage( hWorkWnd, TBM_SETPAGESIZE, 0, 10 );
#endif
			TraceOnScroll( hDlg, hWorkWnd, TB_THUMBPOSITION, gstTrcPrm.dContrast );

			hWorkWnd = GetDlgItem( hDlg, IDSL_TRC_GAMMA );
#ifdef TRC_SCROLLBAR
			stSclInfo.nMax   = TRC_GAMMA_RANGE;	//	nMin���O
			SetScrollInfo( hWorkWnd, SB_CTL, &stSclInfo, TRUE );
#else
			SendMessage( hWorkWnd, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(0, TRC_GAMMA_RANGE) );	//	�K���}
			SendMessage( hWorkWnd, TBM_SETPOS, TRUE , gstTrcPrm.dGamma );	//	
			SendMessage( hWorkWnd, TBM_SETPAGESIZE, 0, 100 );
#endif
			TraceOnScroll( hDlg, hWorkWnd, TB_THUMBPOSITION, gstTrcPrm.dGamma );

			hWorkWnd = GetDlgItem( hDlg, IDSL_TRC_GRAYMOPH );
#ifdef TRC_SCROLLBAR
			stSclInfo.nMax   = TRC_GRAYMOPH_RANGE;	//	nMin���O
			SetScrollInfo( hWorkWnd, SB_CTL, &stSclInfo, TRUE );
#else
			SendMessage( hWorkWnd, TBM_SETRANGE, TRUE , (LPARAM)MAKELONG(0, TRC_GRAYMOPH_RANGE) );	//	�W�F��
			SendMessage( hWorkWnd, TBM_SETPOS, TRUE , gstTrcPrm.dGrayMoph );	//	
			SendMessage( hWorkWnd, TBM_SETPAGESIZE, 0, 10 );
#endif
			TraceOnScroll( hDlg, hWorkWnd, TB_THUMBPOSITION, gstTrcPrm.dGrayMoph );

			hWorkWnd = GetDlgItem( hDlg, IDSL_TRC_ZOOM );
#ifdef TRC_SCROLLBAR
			stSclInfo.nMax   = TRC_ZOOM_RANGE;	//	nMin���O
			SetScrollInfo( hWorkWnd, SB_CTL, &stSclInfo, TRUE );
#else
			SendMessage( hWorkWnd, TBM_SETRANGE, TRUE , (LPARAM)MAKELONG(0, TRC_ZOOM_RANGE) );	//	�g��k��
			SendMessage( hWorkWnd, TBM_SETPOS, TRUE, gstTrcPrm.dZooming );	//	�����Ă邩�H
			SendMessage( hWorkWnd, TBM_SETPAGESIZE, 0, 5 );
#endif
			TraceOnScroll( hDlg, hWorkWnd, TB_THUMBPOSITION, gstTrcPrm.dZooming );

			hWorkWnd = GetDlgItem( hDlg, IDSL_TRC_TURN );
#ifdef TRC_SCROLLBAR
			stSclInfo.nMax   = TRC_TURN_RANGE;	//	nMin���O
			SetScrollInfo( hWorkWnd, SB_CTL, &stSclInfo, TRUE );
#else
			SendMessage( hWorkWnd, TBM_SETRANGE, TRUE , (LPARAM)MAKELONG(0, TRC_TURN_RANGE) );	//	��]
			SendMessage( hWorkWnd, TBM_SETPOS, TRUE, gstTrcPrm.dTurning );	//	�����Ă邩�H
			SendMessage( hWorkWnd, TBM_SETPAGESIZE, 0, 5 );
#endif
			TraceOnScroll( hDlg, hWorkWnd, TB_THUMBPOSITION, gstTrcPrm.dTurning );

			CheckDlgButton( hDlg, IDCB_TRC_IMG_UPSET, gstTrcPrm.bUpset );

			CheckDlgButton( hDlg, IDCB_TRC_IMG_MIRROR, gstTrcPrm.bMirror );

			ViewRedrawSetLine( -1 );

			return (INT_PTR)TRUE;

		case WM_CTLCOLORSTATIC:
			hdc = (HDC)(wParam);
			hWndChild = (HWND)(lParam);
			id = GetDlgCtrlID( hWndChild );
			if( IDPL_TRC_MOZICOLOUR == id )	//	�^�[�Q�b�g�͈����
			{
				GetClientRect( hWndChild, &rect );
				FillRect( hdc, &rect, gMoziClrBrush );
				return (INT_PTR)TRUE;
			}
			break;


		case WM_VSCROLL:
		case WM_HSCROLL:
			return TraceOnScroll( hDlg, (HWND)(lParam), (UINT)(LOWORD(wParam)), (INT)(SHORT)HIWORD(wParam) );


		case WM_COMMAND:
			id = LOWORD(wParam);
			switch( id )
			{
				case IDB_TRC_IMAGEOPEN:		TraceImageFileOpen( hDlg );	return (INT_PTR)TRUE;
				case IDPL_TRC_MOZICOLOUR:	TraceMoziColourChoice( hDlg );	return (INT_PTR)TRUE;

				case IDCB_TRC_IMG_UPSET:	gstTrcPrm.bUpset  = IsDlgButtonChecked( hDlg, IDCB_TRC_IMG_UPSET  );	ViewRedrawSetLine( -1 );	break;
				case IDCB_TRC_IMG_MIRROR:	gstTrcPrm.bMirror = IsDlgButtonChecked( hDlg, IDCB_TRC_IMG_MIRROR );	ViewRedrawSetLine( -1 );	break;

				case IDB_TRC_VIEWTOGGLE:
					gbOnView = gbOnView ? FALSE : TRUE;
					SetDlgItemText( hDlg, IDB_TRC_VIEWTOGGLE, gbOnView ? TEXT("��\��") : TEXT("�\��") );
					ViewRedrawSetLine( -1 );
					ViewFocusSet(  );	//	20110728	�t�H�[�J�X��`��ɖ߂�
					break;

				case IDB_TRC_RESET:
					if( ghImgDib  ){	gpifDeleteDIB( ghImgDib );	ghImgDib = NULL;	}
					if( ghOrigDib ){	ghImgDib = gpifCopyDIB( ghOrigDib );	}

					gstTrcPrm.stOffsetPt.x = TRC_POSITION_OFFSET;
					gstTrcPrm.stOffsetPt.y = TRC_POSITION_OFFSET;

					gstTrcPrm.dContrast = TRC_CONTRA_OFFSET;
					gstTrcPrm.dGamma    = TRC_GAMMA_OFFSET;
					gstTrcPrm.dGrayMoph = 0;
					gstTrcPrm.dZooming  = TRC_ZOOM_OFFSET;
					gstTrcPrm.dTurning  = 0;

					CheckDlgButton( hDlg, IDCB_TRC_IMG_UPSET, BST_UNCHECKED );
					gstTrcPrm.bUpset  = BST_UNCHECKED;
					CheckDlgButton( hDlg, IDCB_TRC_IMG_MIRROR, BST_UNCHECKED );
					gstTrcPrm.bMirror = BST_UNCHECKED;

#ifdef TRC_SCROLLBAR
					SendDlgItemMessage( hDlg, IDSL_TRC_HRIZ_POS, SBM_SETPOS, gstTrcPrm.stOffsetPt.x, TRUE );
					SendDlgItemMessage( hDlg, IDSL_TRC_VART_POS, SBM_SETPOS, gstTrcPrm.stOffsetPt.y, TRUE );	//	
					SendDlgItemMessage( hDlg, IDSL_TRC_CONTRAST, SBM_SETPOS, gstTrcPrm.dContrast, TRUE );	//	�|�Q�T�T�`�Q�T�T�Ȃ̂�
					SendDlgItemMessage( hDlg, IDSL_TRC_GAMMA,    SBM_SETPOS, gstTrcPrm.dGamma, TRUE );		//	
					SendDlgItemMessage( hDlg, IDSL_TRC_GRAYMOPH, SBM_SETPOS, gstTrcPrm.dGrayMoph, TRUE );	//	
					SendDlgItemMessage( hDlg, IDSL_TRC_ZOOM,     SBM_SETPOS, gstTrcPrm.dZooming, TRUE  );	//	
					SendDlgItemMessage( hDlg, IDSL_TRC_TURN,     SBM_SETPOS, gstTrcPrm.dTurning, TRUE  );	//	
#else
					SendDlgItemMessage( hDlg, IDSL_TRC_HRIZ_POS, TBM_SETPOS, TRUE, gstTrcPrm.stOffsetPt.x );	//	
					SendDlgItemMessage( hDlg, IDSL_TRC_VART_POS, TBM_SETPOS, TRUE, gstTrcPrm.stOffsetPt.y );	//	
					SendDlgItemMessage( hDlg, IDSL_TRC_CONTRAST, TBM_SETPOS, TRUE, gstTrcPrm.dContrast );	//	�|�Q�T�T�`�Q�T�T�Ȃ̂�
					SendDlgItemMessage( hDlg, IDSL_TRC_GAMMA,    TBM_SETPOS, TRUE, gstTrcPrm.dGamma );		//	
					SendDlgItemMessage( hDlg, IDSL_TRC_GRAYMOPH, TBM_SETPOS, TRUE, gstTrcPrm.dGrayMoph );	//	
					SendDlgItemMessage( hDlg, IDSL_TRC_ZOOM,     TBM_SETPOS, TRUE, gstTrcPrm.dZooming  );	//	
					SendDlgItemMessage( hDlg, IDSL_TRC_TURN,     TBM_SETPOS, TRUE, gstTrcPrm.dTurning  );	//	
#endif

					TraceOnScroll( hDlg, GetDlgItem( hDlg, IDSL_TRC_HRIZ_POS ), TB_THUMBPOSITION, gstTrcPrm.stOffsetPt.x );
					TraceOnScroll( hDlg, GetDlgItem( hDlg, IDSL_TRC_VART_POS ), TB_THUMBPOSITION, gstTrcPrm.stOffsetPt.y );
					TraceOnScroll( hDlg, GetDlgItem( hDlg, IDSL_TRC_CONTRAST ), TB_THUMBPOSITION, gstTrcPrm.dContrast );
					TraceOnScroll( hDlg, GetDlgItem( hDlg, IDSL_TRC_GAMMA ),    TB_THUMBPOSITION, gstTrcPrm.dGamma );
					TraceOnScroll( hDlg, GetDlgItem( hDlg, IDSL_TRC_GRAYMOPH ), TB_THUMBPOSITION, gstTrcPrm.dGrayMoph );
					TraceOnScroll( hDlg, GetDlgItem( hDlg, IDSL_TRC_ZOOM ),     TB_THUMBPOSITION, gstTrcPrm.dZooming );
					TraceOnScroll( hDlg, GetDlgItem( hDlg, IDSL_TRC_TURN ),     TB_THUMBPOSITION, gstTrcPrm.dTurning );

					gbOnView = TRUE;

					ViewRedrawSetLine( -1 );
					ViewFocusSet(  );	//	20110728	�t�H�[�J�X��`��ɖ߂�
					break;

				//case IDCANCEL:	//	�v��Ȃ����ȁH
				//case IDOK:
				//	//	�Ȃ񂩏���
				//	DestroyWindow( hDlg );
				//	ghTraceDlg = NULL;
				//	return (INT_PTR)TRUE;

				default:	break;
			}
			break;


		case WM_CLOSE:
	//		if( ghImgDib  ){	gpifDeleteDIB( ghImgDib );	ghImgDib = NULL;	}
			DestroyWindow( hDlg );
			ghTraceDlg = NULL;
			ViewMoziColourGet( &caretColour );	//	�L�����b�g�F���ɖ߂�
			ViewCaretReColour( caretColour );
			ViewRedrawSetLine( -1 );	//	�`��̈�ĕ`��
			DeleteBrush( gMoziClrBrush );
			MenuItemCheckOnOff( IDM_TRACE_MODE_ON, FALSE );
			return (INT_PTR)TRUE;

		case WM_DESTROY:
			InitTraceValue( INIT_SAVE, &gstTrcPrm );
			ViewFocusSet(  );	//	20110728	�t�H�[�J�X��`��ɖ߂�
			AppTitleTrace( FALSE);	//	�^�C�g���o�[���ɖ߂�
			return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------



/*!
	�J���_�C�A���O�̃t�b�N�v���V�[�W��
*/
UINT_PTR CALLBACK ImageOpenDlgHookProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	static  HWND	chUseCbxWnd, chPanelWnd;
	static  HDIB	chThumbDib;		//	�T���l�p

	HWND	hWndCtl, hWndChild, hWnd;
	HDC		hDC;
	INT		idCtrl, id, cx, cy;
	UINT	codeNotify, state;
	TCHAR	atFile[MAX_PATH], atSpec[MAX_PATH];
	CHAR	acName[MAX_PATH];
	LPOFNOTIFY	pstOfNty;
	RECT	rect, dlgRect;
	POINT	stPoint;
	SIZE	stSize;

	//	�R�����_�C�����O�̈ʒu�͕ύX�o���Ȃ�

	switch( message )
	{
		case WM_INITDIALOG:
			TRACE( TEXT("HOOK INIT") );
			chThumbDib = NULL;

			chUseCbxWnd = GetDlgItem( hDlg, IDCB_TRC_DLG_USETHUMB );
			Button_SetCheck( chUseCbxWnd, gbThumbUse ? BST_CHECKED : BST_UNCHECKED );

			chPanelWnd  = GetDlgItem( hDlg, IDS_TRC_DLG_THUMBFRAME );
			return (INT_PTR)TRUE;


		case WM_CTLCOLORSTATIC:
			hDC = (HDC)(wParam);
			hWndChild = (HWND)(lParam);
			if( hWndChild == chPanelWnd )
			{
				GetClientRect( chPanelWnd, &rect );
				if( chThumbDib && gbThumbUse )
				{
					gpifDIBtoDCex( hDC, 0, 0, rect.right, rect.bottom, 
						chThumbDib, 0, 0, 0, 0, SRCCOPY );
				}
				else
				{
					FillRect( hDC, &rect, GetStockBrush( WHITE_BRUSH ) );
				}
			}
			return (INT_PTR)FALSE;


		case WM_COMMAND:
			id = LOWORD(wParam);
			hWndCtl = (HWND)lParam;
			codeNotify = HIWORD(wParam);
			if( IDCB_TRC_DLG_USETHUMB == id )
			{
				if( BST_CHECKED == Button_GetCheck( chUseCbxWnd ) )	gbThumbUse = TRUE;
				else	gbThumbUse = FALSE;
			}
			return (INT_PTR)TRUE;


		case WM_SIZE:
			state = (UINT)(wParam);
			cx = (INT)(SHORT)LOWORD(lParam);
			cy = (INT)(SHORT)HIWORD(lParam);
			hWnd = GetParent(hDlg);
			GetClientRect( hWnd, &dlgRect );
			TRACE( TEXT("%dx%d, %dx%d"), hDlg, hWnd, cx, cy, dlgRect.right, dlgRect.bottom );
			GetWindowRect( chPanelWnd, &rect );
			stPoint.x = rect.left;	stPoint.y = rect.top;
			ScreenToClient( hDlg, &stPoint );
			stSize.cx = (dlgRect.right  - stPoint.x) -  8;
			stSize.cy = (dlgRect.bottom - stPoint.y) - 40;
			SetWindowPos( chPanelWnd, HWND_TOP, 0, 0, stSize.cx, stSize.cy, SWP_NOMOVE | SWP_NOZORDER );
			InvalidateRect( chPanelWnd, NULL, TRUE );
			return (INT_PTR)TRUE;


		case WM_NOTIFY:
			idCtrl = (int)wParam;
			pstOfNty = (LPOFNOTIFY)lParam;
			if( CDN_SELCHANGE == pstOfNty->hdr.code )
			{
				//	pstOfNty->hdr.hwndFrom �́A�_�C�����O�S�̂�STATIC�݂����Ȃ��
				TRACE( TEXT("%X,%X,%d"), hDlg, pstOfNty->hdr.hwndFrom, pstOfNty->hdr.idFrom );

				CommDlg_OpenSave_GetSpec( pstOfNty->hdr.hwndFrom, atSpec, MAX_PATH );
				TRACE( TEXT("[SPEC]%s"), atSpec );	//	�t�@�C�����̂�

				CommDlg_OpenSave_GetFilePath( pstOfNty->hdr.hwndFrom, atFile, MAX_PATH );
				TRACE( TEXT("[FILE]%s"), atFile );	//	�t���p�X

				ZeroMemory( acName, sizeof(acName) );
				WideCharToMultiByte( CP_ACP, WC_NO_BEST_FIT_CHARS, atFile, MAX_PATH, acName, MAX_PATH, NULL, NULL );
				if( chThumbDib ){	gpifDeleteDIB( chThumbDib );	chThumbDib = NULL;	}
				chThumbDib = gpifToDIB( acName );

				InvalidateRect( chPanelWnd, NULL, TRUE );
			}
			return (INT_PTR)TRUE;

		case WM_DESTROY:
			if( chThumbDib ){	gpifDeleteDIB( chThumbDib );	}
			return (INT_PTR)TRUE;


		default:	break;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------


/*!
	�摜���J���ăA�b�[�I
	@param[in]	hDlg	�_�C�����O�n���h��
*/
HRESULT TraceImageFileOpen( HWND hDlg )
{
	BITMAPINFOHEADER	stBIH;

	OPENFILENAME	stOpenFile;
	BOOLEAN	bOpened;

	UINT_PTR	cchSize;
	TCHAR	atFilePath[MAX_PATH], atFileName[MAX_STRING];
	CHAR	acName[MAX_PATH];


	ZeroMemory( &stOpenFile, sizeof(OPENFILENAME) );

	ZeroMemory( atFilePath,  sizeof(atFilePath) );
	ZeroMemory( atFileName,  sizeof(atFileName) );

	stOpenFile.lStructSize     = sizeof(OPENFILENAME);
	stOpenFile.hInstance       = GetModuleHandle( NULL );
	stOpenFile.hwndOwner       = GetDesktopWindow(  );
	stOpenFile.lpstrFilter     = TEXT("�摜�t�@�C�� ( bmp, png, jpg, gif )\0*.bmp;*.png;*.jpg;*.jpeg;*.jpe;*.gif\0\0");
//	stOpenFile.nFilterIndex    = 1;
	stOpenFile.lpstrFile       = atFilePath;
	stOpenFile.nMaxFile        = MAX_PATH;
	stOpenFile.lpstrFileTitle  = atFileName;
	stOpenFile.nMaxFileTitle   = MAX_STRING;
//	stOpenFile.lpstrInitialDir = 
	stOpenFile.lpstrTitle      = TEXT("�摜�t�@�C�����w�肷��́[");
	stOpenFile.Flags           = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ENABLETEMPLATE | OFN_ENABLEHOOK | OFN_ENABLESIZING;
//	stOpenFile.lpstrDefExt     = TEXT("");

	stOpenFile.lpfnHook        = ImageOpenDlgHookProc;
	stOpenFile.lpTemplateName  = MAKEINTRESOURCE(IDD_IMAGE_OPEN_DLG);

	bOpened = GetOpenFileName( &stOpenFile );

	if( !(bOpened) ){	return  E_ABORT;	}	//	�L�����Z�����Ă��牽�����Ȃ�

	StringCchLength( atFilePath, MAX_PATH, &cchSize );

	ZeroMemory( acName, sizeof(acName) );
	WideCharToMultiByte( CP_ACP, WC_NO_BEST_FIT_CHARS, atFilePath, cchSize, acName, MAX_PATH, NULL, NULL );

	//	�O�̊J�����ςȂ�j��
	if( ghImgDib  ){	gpifDeleteDIB( ghImgDib );	ghImgDib = NULL;	}
	if( ghOrigDib ){	gpifDeleteDIB( ghOrigDib );	ghOrigDib = NULL;	}

	//	�Y�[�������͌��ɖ߂�
	gstTrcPrm.dZooming  = TRC_ZOOM_OFFSET;
	SendDlgItemMessage( hDlg, IDSL_TRC_ZOOM,     TBM_SETPOS, TRUE, gstTrcPrm.dZooming  );	//	
	TraceOnScroll( hDlg, GetDlgItem( hDlg, IDSL_TRC_ZOOM ),     TB_THUMBPOSITION, gstTrcPrm.dZooming );

	ghOrigDib = gpifToDIB( acName );

	if( !(ghOrigDib) )	return E_HANDLE;

	ghImgDib = gpifCopyDIB( ghOrigDib );

	ZeroMemory( &stBIH, sizeof(BITMAPINFOHEADER) );
	stBIH.biSize = sizeof(BITMAPINFOHEADER);
	gpifHeadDIB( ghImgDib, &stBIH );

	TRACE( TEXT("IMAGE [%dx%d]"), stBIH.biWidth, stBIH.biHeight );

	gstImgSize.cx = stBIH.biWidth;
	gstImgSize.cy = stBIH.biHeight;

	TraceRedrawIamge(  );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�����p�́A�F�I���_�C�����O�[���J���ăA�b�[�I
	@param[in]	hDlg	�_�C�����O�n���h��
*/
HRESULT TraceMoziColourChoice( HWND hDlg )
{
	CHOOSECOLOR	stChsColour;
	COLORREF	adColours[16];

	TRACE( TEXT("�F����") );

	ZeroMemory( adColours, sizeof(adColours) );
	adColours[0] = gstTrcPrm.dMoziColour;

	ZeroMemory( &stChsColour, sizeof(CHOOSECOLOR) );

	stChsColour.lStructSize  = sizeof(CHOOSECOLOR);
	stChsColour.hwndOwner    = hDlg;
	stChsColour.rgbResult    = gstTrcPrm.dMoziColour;	//	�_�C�A���O���ŏ��ɑI�����Ă��鏉���J���[���w�肵�܂��B���[�U�[���F��I�����ă_�C�A���O�������́A�I�������F���i�[����Ă��܂��B
	stChsColour.lpCustColors = adColours;	//	�_�C�A���O�������Ă����{�F���i�[����16�� COLORREF �z��ւ̃|�C���^���w�肵�܂��B���[�U�[�́A�쐬�����F���_�C�A���O�̃p���b�g�̈�Ɉꎞ�ۑ����邱�Ƃ��ł����̕ϐ��̔z��ɂ́A���̃_�C�A���O�̊�{�F���i�[����܂��B
	stChsColour.Flags        = CC_RGBINIT;

	if( ChooseColor( &stChsColour ) )	//	�F�_�C�����O�[�o��
	{
		gstTrcPrm.dMoziColour = stChsColour.rgbResult;

		DeleteBrush( gMoziClrBrush );
		gMoziClrBrush = CreateSolidBrush( gstTrcPrm.dMoziColour );

		ViewCaretReColour( gstTrcPrm.dMoziColour );

		InvalidateRect( GetDlgItem(hDlg,IDPL_TRC_MOZICOLOUR), NULL, TRUE );
		ViewRedrawSetLine( -1 );
		return S_OK;
	}

	return E_ABORT;
}
//-------------------------------------------------------------------------------------------------

/*!
	�e���炢���`�����삳�ꂽ
	@param[in]	hDlg	�_�C�����[�O�n���h��
	@param[in]	hWndCtl	�X���C�_�̃n���h��
	@param[in]	code	�X�N���[���R�[�h
	@param[in]	pos		�X�N���[���{�b�N�X�i�܂݁j�̈ʒu
	@return		����
*/
INT_PTR TraceOnScroll( HWND hDlg, HWND hWndCtl, UINT code, INT pos )
{
/*
#define TB_LINEUP               0
#define TB_LINEDOWN             1
#define TB_PAGEUP               2	���炢���G���A�N���b�N�EPOS���Ȃ�
#define TB_PAGEDOWN             3	���炢���G���A�N���b�N�EPOS���Ȃ�
#define TB_THUMBPOSITION        4	�c�}�~�������I�������N��
#define TB_THUMBTRACK           5	�c�}�~�������Ă�Ƃ��ɃN��
#define TB_TOP                  6
#define TB_BOTTOM               7
#define TB_ENDTRACK             8	����I�������N���EPOS���Ȃ�

#define SB_LINEUP           0
#define SB_LINEDOWN         1
#define SB_PAGEUP           2
#define SB_PAGEDOWN         3
#define SB_THUMBPOSITION    4
#define SB_THUMBTRACK       5
#define SB_TOP              6
#define SB_BOTTOM           7
#define SB_ENDSCROLL        8
*/
//	���ł���pos�̓X���C�_�l���̂܂܂ɂȂ��Ă���悤�ɒ��ӃZ��


	static BOOLEAN	bThumPos = FALSE;
	TCHAR	atBuffer[SUB_STRING];
	UINT	ctlID;
	INT		dDigi, dSyou;


	ctlID = GetDlgCtrlID( hWndCtl );

	ZeroMemory( atBuffer, sizeof(atBuffer) );

	//	�����̂Ƃ���pos�Ȃ��̂Ŏ��O�ňʒu�m�F
	if( TB_PAGEDOWN == code || TB_PAGEUP ==code || TB_ENDTRACK == code || TB_LINEUP == code || TB_LINEDOWN == code )
	{
#ifdef TRC_SCROLLBAR
		pos = SendMessage( hWndCtl, SBM_GETPOS, 0, 0 );
#else
		pos = SendMessage( hWndCtl, TBM_GETPOS, 0, 0 );
#endif
	}

	TRACE( TEXT("SLIDER[%d][%d]"), code, pos );

	if( TB_THUMBPOSITION ==  code ){	bThumPos = TRUE;	}


	if( (TB_ENDTRACK == code && !(bThumPos)) || TB_THUMBPOSITION ==  code )
	{
		if( ghImgDib  ){	gpifDeleteDIB( ghImgDib );	}
		if( ghOrigDib ){	ghImgDib = gpifCopyDIB( ghOrigDib );	}
	}

	switch( code )
	{
		default:	break;
		case SB_LINEDOWN:	pos++;		break;
		case SB_LINEUP:		pos--;		break;
	}

	switch( ctlID )
	{
		case IDSL_TRC_HRIZ_POS:
#ifdef TRC_SCROLLBAR
			switch( code )
			{
				default:	break;
				case SB_PAGEDOWN:	pos += 11;	break;
				case SB_PAGEUP:		pos -= 11;	break;
			}
			if( 0 > pos )	pos = 0;
			if( TRC_POSITION_RANGE < pos )	pos = TRC_POSITION_RANGE;
#endif
			gstTrcPrm.stOffsetPt.x = pos;
			StringCchPrintf( atBuffer, SUB_STRING, TEXT("%d"), pos - TRC_POSITION_OFFSET );
			Edit_SetText( GetDlgItem(hDlg,IDE_TRC_HRIZ_VALUE), atBuffer );
#ifdef TRC_SCROLLBAR
			SendDlgItemMessage( hDlg, IDSL_TRC_HRIZ_POS, SBM_SETPOS, pos, TRUE );
#endif
			break;


		case IDSL_TRC_VART_POS:
#ifdef TRC_SCROLLBAR
			switch( code )
			{
				default:	break;
				case SB_PAGEDOWN:	pos += 18;	break;
				case SB_PAGEUP:		pos -= 18;	break;
			}
			if( 0 > pos )	pos = 0;
			if( TRC_POSITION_RANGE < pos )	pos = TRC_POSITION_RANGE;
#endif
			gstTrcPrm.stOffsetPt.y = pos;
			StringCchPrintf( atBuffer, SUB_STRING, TEXT("%d"), pos - TRC_POSITION_OFFSET );
			Edit_SetText( GetDlgItem(hDlg,IDE_TRC_VART_VALUE), atBuffer );
#ifdef TRC_SCROLLBAR
			SendDlgItemMessage( hDlg, IDSL_TRC_VART_POS, SBM_SETPOS, pos, TRUE );
#endif
			break;


		case IDSL_TRC_CONTRAST:
#ifdef TRC_SCROLLBAR
			switch( code )
			{
				default:	break;
				case SB_PAGEDOWN:	pos += 10;	break;
				case SB_PAGEUP:		pos -= 10;	break;
			}
			if( 0 > pos )	pos = 0;
			if( TRC_CONTRA_RANGE < pos )	pos = TRC_CONTRA_RANGE;
#endif
			gstTrcPrm.dContrast = pos;
			StringCchPrintf( atBuffer, SUB_STRING, TEXT("%d"), pos - TRC_CONTRA_OFFSET );
			Edit_SetText( GetDlgItem(hDlg,IDE_TRC_CONTRAST_VALUE), atBuffer );
#ifdef TRC_SCROLLBAR
			SendDlgItemMessage( hDlg, IDSL_TRC_CONTRAST, SBM_SETPOS, pos, TRUE );
#endif
			break;


		case IDSL_TRC_GAMMA:
#ifdef TRC_SCROLLBAR
			switch( code )
			{
				default:	break;
				case SB_PAGEDOWN:	pos += 100;	break;
				case SB_PAGEUP:		pos -= 100;	break;
			}
			if( 0 > pos )	pos = 0;
			if( TRC_GAMMA_RANGE <  pos )	pos = TRC_GAMMA_RANGE;
#endif
			gstTrcPrm.dGamma = pos;
			dDigi = pos / 1000;
			dSyou = pos % 1000;
			StringCchPrintf( atBuffer, SUB_STRING, TEXT("%d.%03d"), dDigi, dSyou );
			Edit_SetText( GetDlgItem(hDlg,IDE_TRC_GAMMA_VALUE), atBuffer );
#ifdef TRC_SCROLLBAR
			SendDlgItemMessage( hDlg, IDSL_TRC_GAMMA, SBM_SETPOS, pos, TRUE );
#endif
			break;


		case IDSL_TRC_GRAYMOPH:
#ifdef TRC_SCROLLBAR
			switch( code )
			{
				default:	break;
				case SB_PAGEDOWN:	pos += 10;	break;
				case SB_PAGEUP:		pos -= 10;	break;
			}
			if( 0 > pos )	pos = 0;
			if( TRC_GRAYMOPH_RANGE < pos )	pos = TRC_GRAYMOPH_RANGE;
#endif
			gstTrcPrm.dGrayMoph = pos;
			StringCchPrintf( atBuffer, SUB_STRING, TEXT("%d"), gstTrcPrm.dGrayMoph );
			Edit_SetText( GetDlgItem(hDlg,IDE_TRC_GRAYMOPH_VALUE), atBuffer );
#ifdef TRC_SCROLLBAR
			SendDlgItemMessage( hDlg, IDSL_TRC_GRAYMOPH, SBM_SETPOS, pos, TRUE );
#endif
			break;


		case IDSL_TRC_ZOOM:
#ifdef TRC_SCROLLBAR
			switch( code )
			{
				default:	break;
				case SB_PAGEDOWN:	pos += 5;	break;
				case SB_PAGEUP:		pos -= 5;	break;
			}
			if( 0 > pos )	pos = 0;
			if( TRC_ZOOM_RANGE < pos )	pos = TRC_ZOOM_RANGE;
#endif
			gstTrcPrm.dZooming = pos;
			StringCchPrintf( atBuffer, SUB_STRING, TEXT("%d ��"), pos + TRC_ZOOM_OFFSET );
			Edit_SetText( GetDlgItem(hDlg,IDE_TRC_ZOOM_VALUE), atBuffer );
#ifdef TRC_SCROLLBAR
			SendDlgItemMessage( hDlg, IDSL_TRC_ZOOM, SBM_SETPOS, pos, TRUE );
#endif
			break;


		case IDSL_TRC_TURN:	//	��]����Ɖ摜�T�C�Y�ς��
#ifdef TRC_SCROLLBAR
			switch( code )
			{
				default:	break;
				case SB_PAGEDOWN:	pos += 5;	break;
				case SB_PAGEUP:		pos -= 5;	break;
			}
			if( 0 > pos )	pos = 0;
			if( TRC_TURN_RANGE < pos )	pos = TRC_TURN_RANGE;
#endif
			gstTrcPrm.dTurning = pos;
			StringCchPrintf( atBuffer, SUB_STRING, TEXT("%d"), gstTrcPrm.dTurning );
			Edit_SetText( GetDlgItem(hDlg,IDE_TRC_TURN_VALUE), atBuffer );
#ifdef TRC_SCROLLBAR
			SendDlgItemMessage( hDlg, IDSL_TRC_TURN, SBM_SETPOS, pos, TRUE );
#endif
			break;

		default:	return (INT_PTR)FALSE;
	}

	if( (TB_ENDTRACK == code || TB_THUMBPOSITION == code) &&  ghImgDib )	//	�K�p�����l���ł��邩�H
	{
		if( bThumPos && TB_ENDTRACK == code ){	bThumPos = FALSE;	}
		else{	TraceRedrawIamge(  );	}
	}

	//	20110728	�t�H�[�J�X��`��ɖ߂�
	if( TB_ENDTRACK == code ){	ViewFocusSet(  );	}

	return (INT_PTR)TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�C���[�W���ĕ`�悷��
*/
HRESULT TraceRedrawIamge( VOID )
{
	SHORT	dBuff;

	BITMAPINFOHEADER	stBIH;

	dBuff = gstTrcPrm.dContrast - TRC_CONTRA_OFFSET;
	gpifContrastDIB( ghImgDib, dBuff, dBuff, dBuff );
	gpifGammaDIB( ghImgDib, gstTrcPrm.dGamma, gstTrcPrm.dGamma, gstTrcPrm.dGamma );
	gpifGrayDIB( ghImgDib, gstTrcPrm.dGrayMoph );

	gpifTurnDIBex( ghImgDib, gstTrcPrm.dTurning * 1000, 0x00EEEEEE );

	ZeroMemory( &stBIH, sizeof(BITMAPINFOHEADER) );
	stBIH.biSize = sizeof(BITMAPINFOHEADER);
	gpifHeadDIB( ghImgDib, &stBIH );

	gstImgSize.cx = stBIH.biWidth;
	gstImgSize.cy = stBIH.biHeight;

	ViewRedrawSetLine( -1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�摜�̕\����\����ON/OFF����
*/
HRESULT TraceImgViewTglExt( VOID )
{
	if( !(ghTraceDlg) ){	return E_HANDLE;	}

	gbOnView = gbOnView ? FALSE : TRUE;
	SetDlgItemText( ghTraceDlg, IDB_TRC_VIEWTOGGLE, gbOnView ? TEXT("��\��") : TEXT("�\��") );
	ViewRedrawSetLine( -1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�����`��F�����m�点����
	@param[in]	pColour	�I������Ă���F������
	@retval	��O	�g���X��
	@retval	�O		�g���X���[�h�ɂȂ��Ă��Ȃ�
*/
UINT TraceMoziColourGet( LPCOLORREF pColour )
{
	if( !(ghTraceDlg) || !(pColour) ){	return FALSE;	}

	if( !(gbOnView) ){	return FALSE;	}

	*pColour = gstTrcPrm.dMoziColour;

	return TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�C���[�W�J���Ăĕ\���L���ȁE�r���[�`��ŌĂ΂��
	@param[in]	hdc		�`�悷��f�o�C�X�R���e�L�X�g
	@param[in]	iScrlX	�`��̈�̂w�X�N���[��Dot��
	@param[in]	iScrlY	�`��̈�̂x�X�N���[��Dot��
	@retval	��O		�摜�C���[�W������
	@retval	�O			�摜�͊J����Ă��Ȃ�
*/
UINT TraceImageAppear( HDC hdc, INT iScrlX, INT iScrlY )
{
	POINT	stBegin;
	SIZE	stStretch, stReverse;

	if( !(ghTraceDlg && ghImgDib) ){	return FALSE;	}

	if( !(gbOnView) ){	return FALSE;	}

	//	����̃I�t�Z�b�g���l�����ĕ`��Z��

	SetStretchBltMode( hdc, COLORONCOLOR );

	//	�ʒu���킹�Ɏg��
	stBegin.x = LINENUM_WID + (gstTrcPrm.stOffsetPt.x - TRC_POSITION_OFFSET);
	stBegin.y = RULER_AREA  + (gstTrcPrm.stOffsetPt.y - TRC_POSITION_OFFSET);
	//	�X�N���[�����̍l��
	stBegin.x -= iScrlX;
	stBegin.y -= iScrlY;

	//	�g��k���Ɏg��
	stStretch = gstImgSize;
	stStretch.cx *= (gstTrcPrm.dZooming + TRC_ZOOM_OFFSET);	stStretch.cx /= 100;
	stStretch.cy *= (gstTrcPrm.dZooming + TRC_ZOOM_OFFSET);	stStretch.cy /= 100;

	//	���]�Ɏg��
	stReverse = gstImgSize;
	if( BST_CHECKED == gstTrcPrm.bUpset )	stReverse.cy *= -1;
	if( BST_CHECKED == gstTrcPrm.bMirror )	stReverse.cx *= -1;

	gpifDIBtoDCex( hdc, stBegin.x, stBegin.y, stStretch.cx, stStretch.cy, 
		ghImgDib, 0, 0, stReverse.cx, stReverse.cy, SRCCOPY );

	return TRUE;
}
//-------------------------------------------------------------------------------------------------





/*!
	�f�o�C�X�R���e�L�X�g�̓��e���A�e�`���ŕۑ�����
	@param[in]	hDC		�ۑ����������e���͂����Ă�f�B�o�C�X�R���e�B�L�X�g
	@param[in]	ptName	�ۑ��������t���p�X�E�g���q�͎w��ɍ��킹�ďC�������Ⴄ�EMAX_PATH�ł��邱��
	@param[in]	iType	�t�@�C���^�C�v�w��@�P�F�a�l�o�@�Q�F�i�o�f�@�R�F�o�m�f
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ImageFileSaveDC( HDC hDC, LPTSTR ptName, INT iType )
{
	HDIB	hDIB;
	BOOL	bRslt;
	UINT_PTR	cchSize;

	CHAR	acOutName[MAX_PATH];


	ZeroMemory( acOutName, sizeof(acOutName) );

	hDIB = gpifDCtoDIB( hDC, 0, 0, 0, 0 );	//	����ŉ�ʑS��

	PathRemoveExtension( ptName );	//	�g���q���ځ`��

	switch( iType )
	{
		case  ISAVE_BMP:	//	BMP�ۑ��́A�Q�l�Ƃ��Q�T�U�F�ł���
			PathAddExtension( ptName, TEXT(".bmp") );
			StringCchLength( ptName, MAX_PATH, &cchSize );
			WideCharToMultiByte( CP_ACP, 0, ptName, cchSize, acOutName, MAX_PATH, NULL, NULL );
			bRslt = gpifDIBtoBMP( acOutName, hDIB );
			break;

		//case ISAVE_JPEG:
		//	PathAddExtension( ptName, TEXT(".jpg") );
		//	break;

		case  ISAVE_PNG:
			PathAddExtension( ptName, TEXT(".png") );
			StringCchLength( ptName, MAX_PATH, &cchSize );
			WideCharToMultiByte( CP_ACP, 0, ptName, cchSize, acOutName, MAX_PATH, NULL, NULL );
			bRslt = gpifDIBtoPNG( acOutName, hDIB, TRUE );
			break;

		default:	bRslt = FALSE;	break;
	}

	gpifDeleteDIB( hDIB );

	return bRslt ? S_OK : E_DRAW;
}
//-------------------------------------------------------------------------------------------------



