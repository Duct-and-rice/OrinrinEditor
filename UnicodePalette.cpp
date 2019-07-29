/*! @file
	@brief ���j�R�[�h�ꗗ�̃_�C�����O�̖ʓ|���܂�
	���̃t�@�C���� UnicodePalette.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/05/20
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

#include "UnicodePalette.h"
//-------------------------------------------------------------------------------------------------

//	TODO:	�g�������c�̗����ق�����




#ifdef UNICODE
#define LPNMLVDISPINFO	LPNMLVDISPINFOW
#else
#define LPNMLVDISPINFO	LPNMLVDISPINFOA
#endif
//-------------------------------------------------------------------------------------------------

static  HWND	ghMainWnd;		//!<	
static  HWND	ghUniPltWnd;	//!<	
static  HWND	ghGroupSelWnd;	//!<	
static  HWND	ghUniLvWnd;		//!<	

static WNDPROC	gpfOrgUniListProc;	//!<	

static HFONT	ghLvFont, ghPanelFont;	//!<	�\���ɗp����t�H���g

static TCHAR	gtSelMozi;	//!<	

static  UINT	gSelRow;	//!<	
static INT		gSelClm;	//!<	

//	�g�p���O�E�P�U�ێ�
#define UNIUSELOG_MAX	16
static list<TCHAR>	gltUseMozi;
typedef list<TCHAR>::iterator	UUSE_LITR;
//-------------------------------------------------------------------------------------------------

HRESULT UniUseLogging( HWND, TCHAR );

INT_PTR	CALLBACK UniPaletteDlgProc( HWND, UINT, WPARAM, LPARAM );	//!<	

INT_PTR	Uni_OnInitDialog( HWND , HWND, LPARAM );			//!<	
INT_PTR	Uni_OnCommand( HWND , INT, HWND, UINT );			//!<	
INT_PTR	Uni_OnClose( HWND );								//!<	
INT_PTR	Uni_OnNotify( HWND , INT, LPNMHDR );				//!<	
INT_PTR	Uni_OnDrawItem( HWND , CONST LPDRAWITEMSTRUCT );	//!<	

LRESULT	CALLBACK gpfUniListProc( HWND, UINT, WPARAM, LPARAM );	//!<	
//-------------------------------------------------------------------------------------------------

/*!
	���j�R�[�h�p���b�g�̏�����
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	bMode	��O�쐬�@�O�j��
*/
HRESULT UniDlgInitialise( HWND hWnd, UINT dMode )
{
	ULONG	d;
	TCHAR	tMozi;
	TCHAR	atBuff[MAX_PATH], atStr[MIN_STRING];
	LPTSTR	ptEnd, ptBuff;
	UUSE_LITR	itMous;

	ZeroMemory( atBuff, sizeof(atBuff) );


	if( dMode )
	{
		gltUseMozi.clear();
		//	���e�ǂݏo���EXXXXX,XXXXX,XXXXX�@�݂����Ȋ����ŕۑ�
		InitParamString( INIT_LOAD, VS_UNI_USE_LOG, atBuff );
		if( 0 != atBuff[0] )
		{
			ptBuff = &(atBuff[0]);	//	�J�n
			for( d = 0; UNIUSELOG_MAX > d; d++ )
			{
				tMozi = (TCHAR)_tcstoul( ptBuff, &ptEnd , 10 );	//	

				gltUseMozi.push_front( tMozi );

				if( 0 == *ptEnd ){	break;	}	//	���[�܂ŃC�b��
				ptBuff =  ptEnd + 1;	//	���ɐi��
			}
		}
	}
	else
	{
		for( d = 0, itMous = gltUseMozi.begin(); itMous != gltUseMozi.end(); itMous++, d++ )
		{
			StringCchPrintf( atStr, MIN_STRING, TEXT("%d"), *itMous );

			if( 0 != d ){	StringCchCat( atBuff, MAX_PATH, TEXT(",") );	}
			StringCchCat( atBuff, MAX_PATH, atStr );
		}

		InitParamString( INIT_SAVE, VS_UNI_USE_LOG, atBuff );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g�p����������X�g�ɒǉ�
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	tAddMozi	�g��������
*/
HRESULT UniUseLogging( HWND hDlg, TCHAR tAddMozi )
{
	ULONG_PTR	dSize;
	UUSE_LITR	itUse;

	//	�ߋL�^���m�F
	for( itUse = gltUseMozi.begin(); itUse != gltUseMozi.end(); itUse++ )
	{
		if( *itUse == tAddMozi )	//	�q�b�g������A��������U����
		{
			gltUseMozi.erase( itUse );
			break;
		}
	}

	gltUseMozi.push_front( tAddMozi );	//	�g��������擪�ɂ����

	dSize = gltUseMozi.size();
	if( UNIUSELOG_MAX < dSize )
	{
		gltUseMozi.pop_back(  );	//	��ԌÂ��̂𔲂�
	}

	InvalidateRect( GetDlgItem( hDlg, IDS_UNI_USE_LOG ), NULL, TRUE );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g�p�ꗗ����A�g�����̂���������
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	hWndCtl	�N���C�����g�n���h��
	@param[in]	x		�N���C�����g�w���W
	@param[in]	y		�N���C�����g�x���W
*/
HRESULT UniUseFromLog( HWND hDlg, HWND hWndCtl, LONG x, LONG y )
{
	LONG_PTR	useCnt;
	LONG	iWidth, iPos;
	RECT	rect;
	TCHAR	tMozi;
	UUSE_LITR	itUse;


	GetClientRect( GetDlgItem( hDlg, IDS_UNI_USE_LOG ), &rect );
	iWidth = rect.bottom;

	iPos = x / iWidth;	//	�؂�̂Ăɂ���΂O�C���f�b�N�X����
	useCnt = gltUseMozi.size();
	if( useCnt <= iPos )	return E_OUTOFMEMORY;	//	�͂ݏo������_���b�E�E�E�I

	itUse = gltUseMozi.begin();
	std::advance( itUse, iPos );	//	�ړI�܂ŃY������

	tMozi = *itUse;

	Evw_OnChar( ghMainWnd, tMozi, 0 );
	UniUseLogging( hDlg, tMozi );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���j�R�[�h�p���b�g�_�C�����O�[���J������
	@param[in]	hInst	�C���X�^���X�n���h��
	@param[in]	hWnd	�e�E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT UniDialogueEntry( HINSTANCE hInst, HWND hWnd )
{
	ghMainWnd = hWnd;

	gSelRow   = 0xFFFFFFFF;
	gSelClm   = -1;

	if( ghUniPltWnd ){	PostMessage( ghUniPltWnd, WM_CLOSE, 0 , 0 );	return S_OK;	}

	gtSelMozi = NULL;

	//	�_�C�����O�[�̓��[�_���X�ł����
	ghUniPltWnd = CreateDialogParam( hInst, MAKEINTRESOURCE(IDD_UNICODE_PALETTE_DLG_2), hWnd, UniPaletteDlgProc, 0 );

	if( ghUniPltWnd )
	{
		MenuItemCheckOnOff( IDM_UNI_PALETTE, TRUE );
		ShowWindow( ghUniPltWnd, SW_SHOW );
		//UpdateWindow( ghUniPltWnd );
	}

	return ghUniPltWnd ? S_OK : E_HANDLE;
}
//-------------------------------------------------------------------------------------------------

/*!
	���j�R�[�h�p���b�g�_�C�����O�[�̃E�C���h�E�v���V�[�W��
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�ɑ΂��ĉ������Ȃ�����
	@retval no0	�Ȃ񂩏�������
*/
INT_PTR CALLBACK UniPaletteDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		default:	break;

		case WM_INITDIALOG:	return Uni_OnInitDialog( hDlg, (HWND)(wParam), lParam );
		case WM_COMMAND:	return Uni_OnCommand( hDlg, (INT)(LOWORD(wParam)), (HWND)(lParam), (UINT)HIWORD(wParam) );
		case WM_CLOSE:		return Uni_OnClose( hDlg  );
		case WM_NOTIFY:		return Uni_OnNotify( hDlg, (INT)(wParam), (LPNMHDR)(lParam) );
		case WM_DRAWITEM:	return Uni_OnDrawItem( hDlg, (LPDRAWITEMSTRUCT)(lParam) );
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�_�C�����O�N�����̏�����
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	hWndFocus	�Ȃ񂾂���
	@param[in]	lParam		�_�C�����O�I�[�|������Ƃ��ɌĂяo�������n�����l
	@return		FALSE��Ԃ��ƁA�����t�H�[�J�X�R���g���[���̕ύX���L���ɂȂ�
*/
INT_PTR Uni_OnInitDialog( HWND hDlg, HWND hWndFocus, LPARAM lParam )
{
	INT		iTopIdx;
	UINT	i;
	LOGFONT	stFont;
	LVCOLUMN	stLvColm;
	RECT		rect;


	ghGroupSelWnd = GetDlgItem( hDlg, IDCB_UNI_GROUPSEL );

	for( i = 0; UNIGROUP_MAX > i; i++ )
	{
		ComboBox_AddString( ghGroupSelWnd, gstUniGroupName[i].ptNameStr );
	}
	ComboBox_SetCurSel( ghGroupSelWnd, 0 );

	ViewingFontGet( &stFont );	//	gstUniFont

	stFont.lfHeight = 48;
	ghPanelFont = CreateFontIndirect( &stFont );
	SetWindowFont( GetDlgItem(hDlg,IDS_UNI_LETTER_IMG), ghPanelFont, TRUE );

	stFont.lfHeight = 28;
	ghLvFont = CreateFontIndirect( &stFont );

	ghUniLvWnd = GetDlgItem( hDlg, IDLV_UNI_CATALOGUE );
	ListView_SetExtendedListViewStyle( ghUniLvWnd, LVS_EX_GRIDLINES | LVS_EX_LABELTIP );
	SetWindowFont( ghUniLvWnd, ghLvFont, TRUE );

	//	�T�u�N���X��	
	gpfOrgUniListProc = SubclassWindow( ghUniLvWnd, gpfUniListProc );

	ZeroMemory( &stLvColm, sizeof(LVCOLUMN) );
	stLvColm.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	stLvColm.fmt = LVCFMT_CENTER;
	stLvColm.pszText = TEXT("Z");	stLvColm.cx =  0;	stLvColm.iSubItem = 0x00;	ListView_InsertColumn( ghUniLvWnd, 0x00, &stLvColm );
	stLvColm.pszText = TEXT("0");	stLvColm.cx = 40;	stLvColm.iSubItem = 0x01;	ListView_InsertColumn( ghUniLvWnd, 0x01, &stLvColm );
	stLvColm.pszText = TEXT("1");	stLvColm.cx = 40;	stLvColm.iSubItem = 0x02;	ListView_InsertColumn( ghUniLvWnd, 0x02, &stLvColm );
	stLvColm.pszText = TEXT("2");	stLvColm.cx = 40;	stLvColm.iSubItem = 0x03;	ListView_InsertColumn( ghUniLvWnd, 0x03, &stLvColm );
	stLvColm.pszText = TEXT("3");	stLvColm.cx = 40;	stLvColm.iSubItem = 0x04;	ListView_InsertColumn( ghUniLvWnd, 0x04, &stLvColm );
	stLvColm.pszText = TEXT("4");	stLvColm.cx = 40;	stLvColm.iSubItem = 0x05;	ListView_InsertColumn( ghUniLvWnd, 0x05, &stLvColm );
	stLvColm.pszText = TEXT("5");	stLvColm.cx = 40;	stLvColm.iSubItem = 0x06;	ListView_InsertColumn( ghUniLvWnd, 0x06, &stLvColm );
	stLvColm.pszText = TEXT("6");	stLvColm.cx = 40;	stLvColm.iSubItem = 0x07;	ListView_InsertColumn( ghUniLvWnd, 0x07, &stLvColm );
	stLvColm.pszText = TEXT("7");	stLvColm.cx = 40;	stLvColm.iSubItem = 0x08;	ListView_InsertColumn( ghUniLvWnd, 0x08, &stLvColm );
	stLvColm.pszText = TEXT("8");	stLvColm.cx = 40;	stLvColm.iSubItem = 0x09;	ListView_InsertColumn( ghUniLvWnd, 0x09, &stLvColm );
	stLvColm.pszText = TEXT("9");	stLvColm.cx = 40;	stLvColm.iSubItem = 0x0A;	ListView_InsertColumn( ghUniLvWnd, 0x0A, &stLvColm );
	stLvColm.pszText = TEXT("A");	stLvColm.cx = 40;	stLvColm.iSubItem = 0x0B;	ListView_InsertColumn( ghUniLvWnd, 0x0B, &stLvColm );
	stLvColm.pszText = TEXT("B");	stLvColm.cx = 40;	stLvColm.iSubItem = 0x0C;	ListView_InsertColumn( ghUniLvWnd, 0x0C, &stLvColm );
	stLvColm.pszText = TEXT("C");	stLvColm.cx = 40;	stLvColm.iSubItem = 0x0D;	ListView_InsertColumn( ghUniLvWnd, 0x0D, &stLvColm );
	stLvColm.pszText = TEXT("D");	stLvColm.cx = 40;	stLvColm.iSubItem = 0x0E;	ListView_InsertColumn( ghUniLvWnd, 0x0E, &stLvColm );
	stLvColm.pszText = TEXT("E");	stLvColm.cx = 40;	stLvColm.iSubItem = 0x0F;	ListView_InsertColumn( ghUniLvWnd, 0x0F, &stLvColm );
	stLvColm.pszText = TEXT("F");	stLvColm.cx = 40;	stLvColm.iSubItem = 0x10;	ListView_InsertColumn( ghUniLvWnd, 0x10, &stLvColm );

	ListView_SetItemCountEx( ghUniLvWnd, 4096, LVSICF_NOINVALIDATEALL );

	iTopIdx = InitParamValue( INIT_LOAD, VL_UNILISTLAST, 0 );
	ListView_GetItemRect( ghUniLvWnd, 0, &rect, LVIR_BOUNDS );
	iTopIdx *= rect.bottom;
	ListView_Scroll( ghUniLvWnd, 0, iTopIdx );


	SetFocus( ghUniLvWnd );

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�_�C�����O��COMMAND���b�Z�[�W�̎󂯎��
	@param[in]	hDlg		�_�C�����O�[�n���h��
	@param[in]	id			���b�Z�[�W�𔭐��������q�E�C���h�E�̎��ʎq	LOWORD(wParam)
	@param[in]	hWndCtl		���b�Z�[�W�𔭐��������q�E�C���h�E�̃n���h��	lParam
	@param[in]	codeNotify	�ʒm���b�Z�[�W	HIWORD(wParam)
	@return		�������������񂩂�����
*/
INT_PTR Uni_OnCommand( HWND hDlg, INT id, HWND hWndCtl, UINT codeNotify )
{
	INT	dSel, tIdx, dPos;
	RECT	rect;
	POINT	point;

	switch( id )
	{
		default:	break;

		case IDCB_UNI_GROUPSEL:
			if( CBN_SELCHANGE == codeNotify )	//	�R���{�b�N�X�őI�������ꂽ����
			{
				tIdx = ListView_GetTopIndex( ghUniLvWnd );
				ListView_GetItemRect( ghUniLvWnd, tIdx, &rect, LVIR_BOUNDS );
				TRACE( TEXT("%d %d %d"), tIdx, rect.top, rect.bottom );

				dSel = ComboBox_GetCurSel( hWndCtl );
				dPos = gstUniGroupName[dSel].dCode;
				TRACE( TEXT("%s[%u]"), gstUniGroupName[dSel].ptNameStr, dPos );
				dPos /= 16;	//	��s������P�U�����Ȃ̂�
				dPos -= tIdx;
				dPos *= rect.bottom;
				TRACE( TEXT("%d"), dPos );
				ListView_Scroll( ghUniLvWnd, 0, dPos );
				SetFocus( ghUniLvWnd );
			}
			return (INT_PTR)TRUE;


		case IDB_UNI_DECIDE:
			if( gtSelMozi )
			{
				Evw_OnChar( ghMainWnd, gtSelMozi, 0 );
				UniUseLogging( hDlg, gtSelMozi );
			}
			return (INT_PTR)TRUE;

		case IDB_UNI_COPY_CLIP:	
			if( gtSelMozi )
			{
				DocClipLetter( gtSelMozi  );
				UniUseLogging( hDlg, gtSelMozi );
			}
			return (INT_PTR)TRUE;

		case IDS_UNI_USE_LOG:
			if( STN_DBLCLK == codeNotify )	//	�_�u���N���b�N
			{
				GetCursorPos( &point );
				ScreenToClient( hWndCtl, &point );
				UniUseFromLog( hDlg, hWndCtl, point.x, point.y );
			}
			return (INT_PTR)TRUE;

	//	case IDOK:
		case IDCANCEL:
			return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E����Ƃ�
	@param[in]	hDlg		�_�C�����[�O�n���h��
	@return		�������������񂩂�����
*/
INT_PTR Uni_OnClose( HWND hDlg )
{
	INT	iTopIdx;

	iTopIdx = ListView_GetTopIndex( ghUniLvWnd );
	InitParamValue( INIT_SAVE, VL_UNILISTLAST, iTopIdx );

	DeleteFont( ghLvFont  );
	DeleteFont( ghPanelFont );
	DestroyWindow( hDlg );

	ghUniPltWnd = NULL;

	MenuItemCheckOnOff( IDM_UNI_PALETTE, FALSE );

	return (INT_PTR)TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�m�[�e�B�t�@�C���b�Z�[�W�̏���
	@param[in]	hDlg		�_�C�����[�O�n���h��
	@param[in]	idFrom		NOTIFY�𔭐��������R���g���[���̂h�c
	@param[in]	pstNmhdr	NOTIFY�̏ڍ�
	@return		�������������񂩂�����
*/
INT_PTR Uni_OnNotify( HWND hDlg, INT idFrom, LPNMHDR pstNmhdr )
{
	INT		iItem, iSubItem, i;//, tIdx;
	DWORD	iNumber;
	TCHAR	ch[2], atBuff[MIN_STRING];
	RECT	rect;

	LVHITTESTINFO		stHitTestInfo;

	LPNMLISTVIEW		pstListView;
	LPNMLVDISPINFO		pstDispInfo;
	LPNMLVCUSTOMDRAW	pstCustomDraw;

	if( LVN_GETDISPINFO == pstNmhdr->code )	//	���z���X�g�r���[�̏���
	{
		pstDispInfo = (LPNMLVDISPINFO)pstNmhdr;

		iItem = pstDispInfo->item.iItem;

		if( pstDispInfo->item.mask & LVIF_TEXT )
		{
			iSubItem = pstDispInfo->item.iSubItem;

			if( 0 != iSubItem )
			{
				iNumber = iItem * 16 + (iSubItem - 1);
				ch[0] = (TCHAR)iNumber;
				ch[1] = 0;

				StringCchPrintf( pstDispInfo->item.pszText, pstDispInfo->item.cchTextMax, TEXT("%c"), ch[0] );
			}
			else
			{
				ListView_GetItemRect( ghUniLvWnd, iItem, &rect, LVIR_BOUNDS );

			//	TRACE( TEXT("DISP[%d][%d]"), tIdx, iItem );

				StringCchCopy( pstDispInfo->item.pszText, pstDispInfo->item.cchTextMax, TEXT("") );
			}
		}

		return (INT_PTR)TRUE;
	}

	pstListView = (LPNMLISTVIEW)pstNmhdr;

	if( NM_CLICK == pstListView->hdr.code )	//	�N���b�P�����z�̏���
	{
		//	FULLROWSELECT���ĂȂ��Ǝ擾�o���Ȃ��̂ŁA���̂悤�ɂ��ăQ�b�g����
		stHitTestInfo.pt = pstListView->ptAction;
		ListView_SubItemHitTest( ghUniLvWnd, &stHitTestInfo );

		gSelRow = stHitTestInfo.iItem;
		gSelClm = stHitTestInfo.iSubItem;

		iNumber = gSelRow * 16 + (gSelClm - 1);
		ch[0] = (TCHAR)iNumber;
		ch[1] = 0;

		gtSelMozi = (TCHAR)iNumber;

		Static_SetText( GetDlgItem(hDlg,IDS_UNI_LETTER_IMG), ch );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("0x%04X"), iNumber );
		Edit_SetText( GetDlgItem(hDlg,IDE_UNI_HEX_CODE), atBuff );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), iNumber );
		Edit_SetText( GetDlgItem(hDlg,IDE_UNI_DEC_CODE), atBuff );
		Edit_SetText( GetDlgItem(hDlg,IDE_UNI_LETTER_NAME), TEXT("") );
		for( i = 0; UNINAME_MAX > i; i++ )
		{
			if( iNumber == gstUniMoziName[i].dCode )
			{
				Edit_SetText( GetDlgItem(hDlg,IDE_UNI_LETTER_NAME), gstUniMoziName[i].ptNameStr );
				break;
			}

			//	Sort�ςȂ̂ł���ł���
			if( iNumber <  gstUniMoziName[i].dCode )	break;
		}

		InvalidateRect( ghUniLvWnd, NULL, TRUE );
		//UpdateWindow( ghUniLvWnd );
		return (INT_PTR)TRUE;
	}

	if( NM_DBLCLK == pstListView->hdr.code )	//	�_�{���N���b�P�����z�̏���
	{
		//	�m�葀��Ƃ��Ĉ���
		if( gtSelMozi )
		{
			Evw_OnChar( ghMainWnd, gtSelMozi, 0 );
			UniUseLogging( hDlg, gtSelMozi );
		}
	}

	//	�I�����F������̂̓J�X�^���h���[
	if( NM_CUSTOMDRAW == pstListView->hdr.code )
	{
		pstCustomDraw = (LPNMLVCUSTOMDRAW)pstListView;


		if( CDDS_PREPAINT == pstCustomDraw->nmcd.dwDrawStage || 
			CDDS_ITEMPREPAINT == pstCustomDraw->nmcd.dwDrawStage )
		{
			SetWindowLong( hDlg, DWL_MSGRESULT, (long)CDRF_NOTIFYSUBITEMDRAW );
			return TRUE;
		}

		if( (CDDS_ITEMPREPAINT|CDDS_SUBITEM) == pstCustomDraw->nmcd.dwDrawStage )
		{
			//if( 0 == pstCustomDraw->iSubItem )
			//{
			//	TRACE( TEXT("CSTM [%d %d]"), pstCustomDraw->nmcd.rc.top, pstCustomDraw->nmcd.dwItemSpec );
			//}

			if( (gSelClm == pstCustomDraw->iSubItem) && (gSelRow == pstCustomDraw->nmcd.dwItemSpec) )
			{
				pstCustomDraw->clrTextBk = GetSysColor( COLOR_HIGHLIGHT );
				pstCustomDraw->clrText   = GetSysColor( COLOR_HIGHLIGHTTEXT );
				SetWindowLong( hDlg, DWL_MSGRESULT, (long)CDRF_NEWFONT);
			}
			else
			{
				pstCustomDraw->clrTextBk = GetSysColor( COLOR_WINDOW );
				pstCustomDraw->clrText   = GetSysColor( COLOR_WINDOWTEXT );
				SetWindowLong( hDlg, DWL_MSGRESULT, (long)CDRF_NEWFONT);
			}
			return TRUE;
		}

		return (INT_PTR)TRUE;	//	CDRF_DODEFAULT
	}
/*
���X�g�r���[�ŁA�w��s�̔w�i�F��ύX����ɂ́A�J�X�^���h���[���s���܂��B
�J�X�^���h���[�́A�_�u���N���b�N�̎擾�Ő����������Ɠ����ŁA
WM_NOTIFY���b�Z�[�W����NM_CUSTOMDRAW�𔻕ʂ��܂��B
�X�ɁA���X�g�r���[�p�̃J�X�^���h���[�\���̂ŁA�X�e�[�W���̏������s���܂��B
�ECDDS_PREPAINT    �E�E�E�`��O�ʒm
�ECDDS_ITEMPREPAINT�E�E�E���ږ��̕`��O�ʒm
�g�p���Ȃ��X�e�[�W�̏ꍇ�́ACDRF_DODEFAULT��߂�l�Ƃ��ă��^�[�����܂��B
����́A�R���g���[���ɕ`���C�������Ƃ��Ӗ����܂��B
���̃X�e�[�W������������́A�����������Ƃ�ʒm����K�v����̂ł����A
�����̗�Ŏg�p���Ă���̂̓_�C�A���O�ł���ׁA�ʒm��Ԃ��܂���B
�ł��̂ŁASetWindowLong�֐����g�p���ĕԂ��܂��B

�ȉ��ɁANMLVCUSTOMDRAW�\���̂̐����������܂��B

��NMLVCUSTOMDRAW�\���́�
�ENMHDR  hdr         NMHDR�\���̂�\���܂��B
�EDWORD  dwDrawStage ���݂̕`��X�e�[�W��\���܂��B
�EHDC    hdc         �R���g���[���̃f�o�C�X�R���e�L�X�g�n���h����\���܂��B
�ERECT   rc          �`�悳��悤�Ƃ��Ă���̈��RECT�\���̂�\���܂��B
�EDWORD  dwItemSpec  �A�C�e���ԍ���\���܂��B
�EUINT   uItemState  ���݂̃A�C�e���̏�Ԃ�\���܂��B
�ELPARAM lItemlParam �A�v���P�[�V������`�̃f�[�^��\���܂��B

HWND             hList;
LPNMHDR          lpnmhdr;
LPNMLVCUSTOMDRAW lplvcd;

case WM_NOTIFY:
	lpnmhdr = (LPNMHDR)lp;
	if( lpnmhdr->hwndFrom == GetDlgItem(hWnd, ���X�g�r���[��ID) ){
		switch( lpnmhdr->code ){
			case NM_CUSTOMDRAW:
				lplvcd = (LPNMLVCUSTOMDRAW)lp;

				if( lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT ){
					SetWindowLong( hWnd, DWL_MSGRESULT, (long)CDRF_NOTIFYITEMDRAW );
				return( TRUE );
		}
		if( lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT ){
			lplvcd->clrTextBk = RGB(0xCC, 0xCC, 0xCC);
			SetWindowLong( hWnd, DWL_MSGRESULT, (long)CDRF_NEWFONT);
		return( TRUE );
	}
return( CDRF_DODEFAULT );
*/

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�X�^�e�B�b�N�̃I�[�i�[�h���[�̏���
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	pstDrawItem	�I�[�i�h���[�f�[�^
*/
INT_PTR Uni_OnDrawItem( HWND hDlg, CONST LPDRAWITEMSTRUCT pstDrawItem )
{
	LONG	iWidth;
	RECT	rect;
	HFONT	hOldFnt;
	TCHAR	atUnic[2];
	UUSE_LITR	itUse;

	//	�֌W�Ȃ��͕̂��u
	if( IDS_UNI_USE_LOG != pstDrawItem->CtlID ){	return (INT_PTR)FALSE;	}

	hOldFnt = SelectFont( pstDrawItem->hDC , ghLvFont );	//	�t�H���g��������

	SetBkMode( pstDrawItem->hDC, TRANSPARENT );
	rect = pstDrawItem->rcItem;
	iWidth = rect.bottom;	//	�����`�̕`��G���A�Ƃ���

	FillRect( pstDrawItem->hDC, &(pstDrawItem->rcItem), GetSysColorBrush( COLOR_WINDOW ) );

	atUnic[0] = 0;	atUnic[1] = 0;
	rect.right = iWidth;	//	�ŏ��̃G���A�ݒ�
	for( itUse = gltUseMozi.begin(); itUse != gltUseMozi.end(); itUse++ )
	{
		atUnic[0] =  *itUse;	//	�땶���Â`�悵�Ă���
		DrawText( pstDrawItem->hDC, atUnic, 1, &(rect), DT_CENTER | DT_VCENTER | DT_SINGLELINE );

		rect.left  += iWidth;	//	�`��ʒu���V�t�g
		rect.right += iWidth;
	}

	SelectFont( pstDrawItem->hDC, hOldFnt );	//	�����؂���

	return (INT_PTR)TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	���j�R�[�h�ꗗ�̃T�u�N���X�v���V�[�W��
	@param[in]	hWnd	���X�g�̃n���h��
	@param[in]	msg		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@return		LRESULT	�������ʂƂ�
*/
LRESULT CALLBACK gpfUniListProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	INT	tIdx;
	TCHAR	atBuffer[MIN_STRING];

	switch( msg )
	{
		case WM_PAINT:	//	���X�g�̏������������Ȃ킿�\�����e���ύX���ꂽ�Ǝv���
			tIdx = ListView_GetTopIndex( hWnd );
		//	TRACE( TEXT("UPSC_WM_PAINT [%d]"), tIdx );
			StringCchPrintf( atBuffer, MIN_STRING, TEXT("0x%04X"),  tIdx    * 0x10 );	SetDlgItemText( ghUniPltWnd, IDS_UA_ALPHA,   atBuffer );
			StringCchPrintf( atBuffer, MIN_STRING, TEXT("0x%04X"), (tIdx+1) * 0x10 );	SetDlgItemText( ghUniPltWnd, IDS_UA_BRAVO,   atBuffer );
			StringCchPrintf( atBuffer, MIN_STRING, TEXT("0x%04X"), (tIdx+2) * 0x10 );	SetDlgItemText( ghUniPltWnd, IDS_UA_CHARLIE, atBuffer );
			StringCchPrintf( atBuffer, MIN_STRING, TEXT("0x%04X"), (tIdx+3) * 0x10 );	SetDlgItemText( ghUniPltWnd, IDS_UA_DELTA,   atBuffer );
			StringCchPrintf( atBuffer, MIN_STRING, TEXT("0x%04X"), (tIdx+4) * 0x10 );	SetDlgItemText( ghUniPltWnd, IDS_UA_ECHO,    atBuffer );
			StringCchPrintf( atBuffer, MIN_STRING, TEXT("0x%04X"), (tIdx+5) * 0x10 );	SetDlgItemText( ghUniPltWnd, IDS_UA_FOXTROT, atBuffer );
			StringCchPrintf( atBuffer, MIN_STRING, TEXT("0x%04X"), (tIdx+6) * 0x10 );	SetDlgItemText( ghUniPltWnd, IDS_UA_GOLF,    atBuffer );
			StringCchPrintf( atBuffer, MIN_STRING, TEXT("0x%04X"), (tIdx+7) * 0x10 );	SetDlgItemText( ghUniPltWnd, IDS_UA_HOTEL,   atBuffer );
			break;

		default:	break;
	}

	return CallWindowProc( gpfOrgUniListProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

