/*! @file
	@brief �A�v���S�̂Ŏg���萔�┟���ł�
	���̃t�@�C���� OrinrinCollector.h �ł��B
	@author	SikigamiHNQ
	@date	2011/10/06
*/

/*
Orinrin Collector : Clipboard Auto Stocker for Japanese Only
Copyright (C) 2011 - 2012 Orinrin/SikigamiHNQ

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.
*/
//-------------------------------------------------------------------------------------------------

#pragma once

#define STRICT

#include "resource.h"
//-------------------------------------------------------------------------------------------------

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾
ATOM	InitWndwClass( HINSTANCE );
BOOL	InitInstance( HINSTANCE, INT );
LRESULT	CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

BOOLEAN	Cls_OnCreate( HWND, LPCREATESTRUCT );		//!<	�{�̂� WM_CREATE �̏����E�Œ�Edit�Ƃ�����
VOID	Cls_OnCommand( HWND , INT, HWND, UINT );	//!<	�{�̂� WM_COMMAND �̏���
VOID	Cls_OnPaint( HWND );						//!<	�{�̂� WM_PAINT �̏����E�g���`��Ƃ�
VOID	Cls_OnDestroy( HWND );						//!<	�{�̂� WM_DESTROY �̏����EBRUSH�Ƃ��̃I�u�W�F�N�g�̔j���Y��Ȃ��悤��
HBRUSH	Cls_OnCtlColor( HWND , HDC, HWND, INT );	//!<	
VOID	Cls_OnHotKey( HWND, INT, UINT, UINT );		//!<	
VOID	Cls_OnDrawClipboard( HWND );				//!<	
VOID	Cls_OnChangeCBChain( HWND, HWND, HWND );	//!<	

VOID		WndTagSet( HWND, LONG_PTR );			//!<	
LONG_PTR	WndTagGet( HWND );						//!<	

BOOLEAN	SelectFileDlg( HWND, LPTSTR, UINT_PTR );	//!<	

HRESULT	ToolTipSetting( HWND, UINT, LPTSTR );		//!<	

HRESULT	TasktrayIconAdd( HWND );					//!<	
VOID	TaskTrayIconEvent( HWND, UINT, UINT );		//!<	
HRESULT	TaskTrayIconCaptionChange( HWND );			//!<	
HRESULT	TaskTrayIconBalloon( HWND, LPTSTR, LPTSTR, DWORD );	//!<	

INT		InitParamValue( UINT, UINT, INT );			//!<	
HRESULT	InitClipStealOpen( UINT, UINT, LPTSTR );	//!<	
HRESULT	InitSettingSave( HWND, UINT );				//!<	

HRESULT	FileListViewInit( HWND );				//!<	
INT		FileListViewAdd( HWND, LPTSTR );		//!<	
HRESULT	FileListViewGet( HWND, INT, LPTSTR );	//!<	
INT		FileListViewDelete( HWND  );			//!<	

BOOLEAN	FileTypeCheck( LPTSTR );			//!<	

HMENU	CreateFileSelMenu( HWND, UINT );	//!<	

HRESULT	ClipStealDoing( HWND  );		//!<	
LPTSTR	ClipboardDataGet( LPVOID  );	//!<	

UINT	RegHotModExchange( UINT , BOOLEAN );	//!<	

LPTSTR	SjisDecodeAlloc( LPSTR );		//!<	
LPSTR	SjisEncodeAlloc( LPCTSTR );		//!<	
LPSTR	SjisEntityExchange( LPCSTR );	//!<	
TCHAR	UniRefCheck( LPSTR );			//!<	
