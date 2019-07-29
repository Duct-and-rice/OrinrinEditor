/*! @file
	@brief �g���X���[�h�̂Ƃ��A�Q�y�C���̊Ǘ����܂��B
	���̃t�@�C���� ViewTrace.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/11/29
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

//�������͉{���p�E�w�i�G��\������̂͂ǂ����ł�������悤�ɂ���̂���������

#include "stdafx.h"
#include "OrinrinEditor.h"
//-------------------------------------------------------------------------------------------------

#define VIEW_TRACE_CLASS	TEXT("TRACE_VIEW")
//-------------------------------------------------------------------------------------------------

static HINSTANCE	ghInst;		//!<	���݂̃C���^�[�t�F�C�X

extern  HWND	ghPrntWnd;		//!<	���C���E�C���h�E�n���h��
extern  HWND	ghViewWnd;		//!<	�ҏW�r���[�n���h��

static  HWND	ghVwTrcWnd;		//!<	���̃E�C���h�E�E�g���X�r���[�n���h��

//	��ʃT�C�Y���m�F���āA�ړ��ɂ��X�N���[���̖ʓ|�݂�
extern INT		gdHideXdot;		//!<	���̉B�ꕔ��
extern INT		gdViewTopLine;	//!<	�\�����̍ŏ㕔�s�ԍ�
extern SIZE		gstViewArea;	//!<	�\���̈�̃h�b�g�T�C�Y�E���[���[���̗̈�͖���
extern INT		gdDispingLine;	//!<	�����Ă�s���E���r���[�Ɍ����Ă閖�[�͊܂܂Ȃ�

//	�t�H���g�͕`�斈�Ƀf�o�C�X�R���e�L�X�g�Ɋ���t����K�v������
extern HFONT	ghAaFont;		//!<	AA�p�t�H���g
//-------------------------------------------------------------------------------------------------


