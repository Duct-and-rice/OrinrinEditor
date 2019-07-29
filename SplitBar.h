/*! @file
	@brief �X�v���b�g�o�[�Ŏg���萔�┟���ł��B
	���̃t�@�C���� SplitBar.h �ł��B
	@author	SikigamiHNQ
	@date	2011/00/00
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

#pragma once

#define STRICT
//-------------------------------------------------------------------------------------------------

#define SPLITBAR_CLASS		TEXT("CSplitBar")	//	�X�v���b�g�o�[�N���X��
#define SPLITBAR_WIDTH		4	//	�X�v���b�g�o�[�̕�
#define SPLITBAR_LEFTLIMIT	120	//	�X�v���b�g�o�[�̍��ʒu���~�b�g

#define SIZE_SPLITBAR_MOVED	0xFFFF	//	�X�v���b�g�o�[���������ꂽ
//-------------------------------------------------------------------------------------------------

ATOM	SplitBarClass( HINSTANCE );
HWND	SplitBarCreate( HINSTANCE, HWND, INT, INT, INT );
VOID	SplitBarPosGet( HWND, LPRECT );
LONG	SplitBarResize( HWND, LPRECT );
