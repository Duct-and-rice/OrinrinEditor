/*! @file
	@brief ���C���̃R���e�L�X�g���j���[�̊Ǘ����܂�
	���̃t�@�C���� ContextCtrl.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/11/07
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

//	���[�U��`�Ƃ��g�Ƃ��́A�T�u���j���[�p�̃_�~�[�A�C�e����p�ӂ���

typedef struct tagCONTEXTITEM
{
	TCHAR	atString[MIN_STRING];		//!<	�\�����镶����
	UINT	dCommandoID;				//!<	�R�}���h�h�c�E0�ŃZ�p���[�^

} CONTEXTITEM, *LPCONTEXTITEM;
//-------------------------------------------------------------------------------------------------


//


CONST static CONTEXTITEM	gstContextItem[] =
{
/* 0*/	{  TEXT("�V�K�쐬"),	/*�t�@�C��*/		IDM_NEWFILE				},
/* 1*/	{  TEXT("�t�@�C���J��"),					IDM_OPEN				},
		{  TEXT("�㏑���ۑ�"),						IDM_OVERWRITESAVE		},
		{  TEXT("���O��t���ĕۑ�"),				IDM_RENAMESAVE			},
		{  TEXT("���ݕł��摜�ɂ��ĕۑ�"),			IDM_IMAGE_SAVE			},
/* 5*/	{  TEXT("��ʐݒ�"),						IDM_GENERAL_OPTION		},
		{  TEXT("�i�Z�p���[�^�j"),					0						},
		{  TEXT("���ɖ߂�"),	/*�ҏW*/			IDM_UNDO				},
		{  TEXT("��蒼��"),						IDM_REDO				},
		{  TEXT("�؂���"),						IDM_CUT					},
/*10*/	{  TEXT("Unicode�R�s�["),					IDM_COPY				},
		{  TEXT("SJIS�R�s�["),						IDM_SJISCOPY			},
		{  TEXT("�S�̂�SJIS�R�s�["),				IDM_SJISCOPY_ALL		},
		{  TEXT("�\�t"),							IDM_PASTE				},
		{  TEXT("�폜"),							IDM_DELETE				},
/*15*/	{  TEXT("�S�I��"),							IDM_ALLSEL				},
		{  TEXT("��`�I��"),						IDM_SQSELECT			},
		{  TEXT("��`�\�t"),						IDM_SQUARE_PASTE		},
		{  TEXT("���C���{�b�N�X"),					IDM_LAYERBOX			},
		{  TEXT("�������o���[�h"),					IDM_EXTRACTION_MODE		},
/*20*/	{  TEXT("���̍s�ȍ~��V�łɕ���"),			IDM_PAGEL_DIVIDE		},
		{  TEXT("�I�𕶎����Ŗ��ɂ���"),			IDM_PAGENAME_SELASSIGN	},
		{  TEXT("�i�Z�p���[�^�j"),					0						},
		{  TEXT("���j�R�[�h��"),	/*�}��*/		IDM_MN_UNISPACE			},//�T�u���j���[�_�~�[
		{  TEXT(" 1 dot SPACE"),					IDM_IN_01SPACE			},
/*25*/	{  TEXT(" 2 dot SPACE"),					IDM_IN_02SPACE			},
		{  TEXT(" 3 dot SPACE"),					IDM_IN_03SPACE			},
		{  TEXT(" 4 dot SPACE"),					IDM_IN_04SPACE			},
		{  TEXT(" 5 dot SPACE"),					IDM_IN_05SPACE			},
		{  TEXT(" 8 dot SPACE"),					IDM_IN_08SPACE			},
/*30*/	{  TEXT("10 dot SPACE"),					IDM_IN_10SPACE			},
		{  TEXT("16 dot SPACE"),					IDM_IN_16SPACE			},
		{  TEXT("�F�w��"),							IDM_MN_COLOUR_SEL		},//�T�u���j���[�_�~�[
		{  TEXT("��"),								IDM_INSTAG_WHITE		},
		{  TEXT("��"),								IDM_INSTAG_BLUE			},
/*35*/	{  TEXT("��"),								IDM_INSTAG_BLACK		},
		{  TEXT("��"),								IDM_INSTAG_RED			},
		{  TEXT("��"),								IDM_INSTAG_GREEN		},
		{  TEXT("�g�I��"),							IDM_MN_INSFRAME_SEL		},//�T�u���j���[�_�~�[
		{  TEXT("�g�i�P�j"),						IDM_INSFRAME_ALPHA		},
/*40*/	{  TEXT("�g�i�Q�j"),						IDM_INSFRAME_BRAVO		},
		{  TEXT("�g�i�R�j"),						IDM_INSFRAME_CHARLIE	},
		{  TEXT("�g�i�S�j"),						IDM_INSFRAME_DELTA		},
		{  TEXT("�g�i�T�j"),						IDM_INSFRAME_ECHO		},
		{  TEXT("�g�i�U�j"),						IDM_INSFRAME_FOXTROT	},
/*45*/	{  TEXT("�g�i�V�j"),						IDM_INSFRAME_GOLF		},
		{  TEXT("�g�i�W�j"),						IDM_INSFRAME_HOTEL		},
		{  TEXT("�g�i�X�j"),						IDM_INSFRAME_INDIA		},
		{  TEXT("�g�i�P�O�j"),						IDM_INSFRAME_JULIETTE	},
		{  TEXT("�g�i�P�P�j"),						IDM_INSFRAME_KILO		},
/*50*/	{  TEXT("�g�i�P�Q�j"),						IDM_INSFRAME_LIMA		},
		{  TEXT("�g�i�P�R�j"),						IDM_INSFRAME_MIKE		},
		{  TEXT("�g�i�P�S�j"),						IDM_INSFRAME_NOVEMBER	},
		{  TEXT("�g�i�P�T�j"),						IDM_INSFRAME_OSCAR		},
		{  TEXT("�g�i�P�U�j"),						IDM_INSFRAME_PAPA		},
/*55*/	{  TEXT("�g�i�P�V�j"),						IDM_INSFRAME_QUEBEC		},
		{  TEXT("�g�i�P�W�j"),						IDM_INSFRAME_ROMEO		},
		{  TEXT("�g�i�P�X�j"),						IDM_INSFRAME_SIERRA		},
		{  TEXT("�g�i�Q�O�j"),						IDM_INSFRAME_TANGO		},
		{  TEXT("�g�ҏW"),							IDM_INSFRAME_EDIT		},
/*60*/	{  TEXT("�g�}���{�b�N�X"),					IDM_FRMINSBOX_OPEN		},
		{  TEXT("�����`�`�ϊ��{�b�N�X"),			IDM_MOZI_SCR_OPEN		},
		{  TEXT("�c�����ϊ��{�b�N�X"),				IDM_VERT_SCRIPT_OPEN	},
		{  TEXT("���[�U��`�A�C�e��"),				IDM_MN_USER_REFS		},//�T�u���j���[�_�~�[
		{  TEXT("���[�U�A�C�e���i�P�j"),			IDM_USER_ITEM_ALPHA		},
/*65*/	{  TEXT("���[�U�A�C�e���i�Q�j"),			IDM_USER_ITEM_BRAVO		},
		{  TEXT("���[�U�A�C�e���i�R�j"),			IDM_USER_ITEM_CHARLIE	},
		{  TEXT("���[�U�A�C�e���i�S�j"),			IDM_USER_ITEM_DELTA		},
		{  TEXT("���[�U�A�C�e���i�T�j"),			IDM_USER_ITEM_ECHO		},
		{  TEXT("���[�U�A�C�e���i�U�j"),			IDM_USER_ITEM_FOXTROT	},
/*70*/	{  TEXT("���[�U�A�C�e���i�V�j"),			IDM_USER_ITEM_GOLF		},
		{  TEXT("���[�U�A�C�e���i�W�j"),			IDM_USER_ITEM_HOTEL		},
		{  TEXT("���[�U�A�C�e���i�X�j"),			IDM_USER_ITEM_INDIA		},
		{  TEXT("���[�U�A�C�e���i�P�O�j"),			IDM_USER_ITEM_JULIETTE	},
		{  TEXT("���[�U�A�C�e���i�P�P�j"),			IDM_USER_ITEM_KILO		},
/*75*/	{  TEXT("���[�U�A�C�e���i�P�Q�j"),			IDM_USER_ITEM_LIMA		},
		{  TEXT("���[�U�A�C�e���i�P�R�j"),			IDM_USER_ITEM_MIKE		},
		{  TEXT("���[�U�A�C�e���i�P�S�j"),			IDM_USER_ITEM_NOVEMBER	},
		{  TEXT("���[�U�A�C�e���i�P�T�j"),			IDM_USER_ITEM_OSCAR		},
		{  TEXT("���[�U�A�C�e���i�P�U�j"),			IDM_USER_ITEM_PAPA		},
/*80*/	{  TEXT("�e�łɒʂ��ԍ�������"),			IDM_PAGENUM_DLG_OPEN	},
		{  TEXT("�i�Z�p���[�^�j"),					0						},
		{  TEXT("�E������"),	/*���`*/			IDM_RIGHT_GUIDE_SET		},
		{  TEXT("�s���ɑS�p�󔒒ǉ�"),				IDM_INS_TOPSPACE		},
		{  TEXT("�s���󔒍폜"),					IDM_DEL_TOPSPACE		},
/*85*/	{  TEXT("�s���󔒍폜"),					IDM_DEL_LASTSPACE		},
		{  TEXT("�s�������폜"),					IDM_DEL_LASTLETTER		},
		{  TEXT("�I��͈͂��󔒂ɂ���"),			IDM_FILL_SPACE			},
		{  TEXT("�őS�̂��󔒂Ŗ��߂�"),			IDM_FILL_ZENSP			},
		{  TEXT("�s�����p�󔒂����j�R�[�h�ɕϊ�"),	IDM_HEADHALF_EXCHANGE	},
/*90*/	{  TEXT("���E���]"),						IDM_MIRROR_INVERSE		},
		{  TEXT("�㉺���]"),						IDM_UPSET_INVERSE		},
		{  TEXT("�E�Ɋ񂹂�"),						IDM_RIGHT_SLIDE			},
		{  TEXT("�P�h�b�g���₷"),					IDM_INCREMENT_DOT		},
		{  TEXT("�P�h�b�g���炷"),					IDM_DECREMENT_DOT		},
/*95*/	{  TEXT("�S�̂��P�h�b�g�E��"),				IDM_INCR_DOT_LINES		},
		{  TEXT("�S�̂��P�h�b�g����"),				IDM_DECR_DOT_LINES		},
		{  TEXT("�w��ʒu����E�֍L����"),			IDM_DOT_SPLIT_RIGHT		},
		{  TEXT("�w��ʒu���獶�ɋ��߂�"),			IDM_DOT_SPLIT_LEFT		},
		{  TEXT("��������b�N"),					IDM_DOTDIFF_LOCK		},
/*100*/	{  TEXT("�J�[�\���ʒu�Œ���"),				IDM_DOTDIFF_ADJT		},
		{  TEXT("�i�Z�p���[�^�j"),					0						},
		{  TEXT("�󔒂�\��"),	/*�\��*/			IDM_SPACE_VIEW_TOGGLE	},
		{  TEXT("�O���b�h����\��"),				IDM_GRID_VIEW_TOGGLE	},
		{  TEXT("�E�K�C�h����\��"),				IDM_RIGHT_RULER_TOGGLE	},
/*105*/	{  TEXT("�s�K�C�h����\��"),				IDM_UNDER_RULER_TOGGLE	},
		{  TEXT("�����s�e���v���[�g�\��"),			IDM_MAATMPLE_VIEW		},
		{  TEXT("�ňꗗ�\��"),						IDM_PAGELIST_VIEW		},
		{  TEXT("��s�e���v���[�g�\��"),			IDM_LINE_TEMPLATE		},
		{  TEXT("�h��ׂ��u���V�\��"),				IDM_BRUSH_PALETTE		},
/*110*/	{  TEXT("���j�R�[�h�\���J��"),				IDM_UNI_PALETTE			},
		{  TEXT("�g���X���[�h"),					IDM_TRACE_MODE_ON		},
		{  TEXT("�v���r���["),						IDM_ON_PREVIEW			},
		{  TEXT("�h���t�g�{�[�h���J��"),			IDM_DRAUGHT_OPEN		},
		{  TEXT("�T���l�C����\��"),				IDM_MAA_THUMBNAIL_OPEN	},
/*115*/	{  TEXT("�i�Z�p���[�^�j"),					0						},
		{  TEXT("�I��͈͂��h���t�g�{�[�h��"),		IDM_COPY_TO_DRAUGHT		},
		{  TEXT("�i�Z�p���[�^�j"),	0	/*�ȉ��A���e�Œ�H�̓���R�}���h*/	},
		{  TEXT("�I��ł𕡐�"),					IDM_PAGEL_DUPLICATE		},
		{  TEXT("�I��ł��폜"),					IDM_PAGEL_DELETE		},
/*120*/	{  TEXT("���̕łƓ���"),					IDM_PAGEL_COMBINE		},
		{  TEXT("�I��ł̎��ɐV�K�쐬"),			IDM_PAGEL_INSERT		},
		{  TEXT("�����ɕł�V�K�쐬"),				IDM_PAGEL_ADD			},
		{  TEXT("�ł����ֈړ�"),					IDM_PAGEL_DOWNSINK		},
		{  TEXT("�ł���ֈړ�"),					IDM_PAGEL_UPFLOW		},
/*125*/	{  TEXT("�Ŗ��̂̕ύX"),					IDM_PAGEL_RENAME		},
		{  TEXT("�g���X�摜�\��/��\��"),			IDM_TRC_VIEWTOGGLE		},
		{  TEXT("�e���v���O���[�v�ؑց�"),			IDM_TMPLT_GROUP_PREV	},
		{  TEXT("�e���v���O���[�v�ؑց�"),			IDM_TMPLT_GROUP_NEXT	},
		{  TEXT("���t�H�[�J�X�ؑց�"),				IDM_WINDOW_CHANGE		},
/*130*/	{  TEXT("���t�H�[�J�X�ؑց�"),				IDM_WINDOW_CHG_RVRS		},
		{  TEXT("�t�@�C�������"),				IDM_FILE_CLOSE			},
		{  TEXT("�t�@�C���ؑց�"),					IDM_FILE_PREV			},
		{  TEXT("�t�@�C���ؑց�"),					IDM_FILE_NEXT			},
		{  TEXT("�O�̕łֈړ�"),					IDM_PAGE_PREV			},
/*135*/	{  TEXT("���̕łֈړ�"),					IDM_PAGE_NEXT			},
		{  TEXT("�e���v���O���b�h����"),			IDM_TMPL_GRID_INCREASE	},
		{  TEXT("�e���v���O���b�h����"),			IDM_TMPL_GRID_DECREASE	},
		{  TEXT("�\����ʍĕ`��"),					IDM_NOW_PAGE_REFRESH	},
		{  TEXT("�i�������j�����񌟍�"),			IDM_FIND_DLG_OPEN		},
/*140*/	{  TEXT("�i�������j�����ݒ胊�Z�b�g"),		IDM_FIND_HIGHLIGHT_OFF	},
		{  TEXT("�i�������j���̌����ʒu"),			IDM_FIND_JUMP_NEXT		},
		{  TEXT("�i�������j�O�̌����ʒu"),			IDM_FIND_JUMP_PREV		},
		{  TEXT("�i�������j�V���������������"),	IDM_FIND_TARGET_SET		},
		{  TEXT("�i�Z�p���[�^�j"),					0						}
};

#define ALL_ITEMS	 117	//	�E�N���p�@�O�C���f�b�N�X
#define FULL_ITEMS	 145	//	�S�A�C�e��

//	�E�N���p�T�u�A�C�e���E�ʒu�����Y��Ȃ��悤��
#define CTS_UNISPACE	24
#define CTS_COLOURINS	33
#define CTS_FRAMEINS	39
#define CTS_USERITEM	64

//	�T�u�W�J����A�C�e���ɒ��ӃZ���E�R���e�L�X�g���j���[�ƃA�N�Z���L�[

//	�L�[�o�C���h�E���j���[�ɓ����ĂȂ��@�\�ɒ���

//�����ݒ�
#define DEF_ITEMS	32
const static UINT	gadDefItem[] =
{
	IDM_CUT,				//	�؂���
	IDM_COPY,				//	Unicode�R�s�[
	IDM_PASTE,				//	�\�t
	IDM_ALLSEL,				//	�S�I��
	0,
	IDM_COPY_TO_DRAUGHT,	//	�I��͈͂��h���t�g�{�[�h��
	0,
	IDM_SJISCOPY,			//	SJIS�R�s�[
	IDM_SJISCOPY_ALL,		//	�S�̂�SJIS�R�s�[
	0,
	IDM_SQSELECT,			//	��`�I��
	0,
	IDM_LAYERBOX,			//	���C���{�b�N�X
	IDM_FRMINSBOX_OPEN,		//	�g�}���{�b�N�X
	IDM_MOZI_SCR_OPEN,		//	�����`�`�ϊ��{�b�N�X
	0,
	IDM_RIGHT_GUIDE_SET,	//	�E������
	IDM_INS_TOPSPACE,		//	�s���ɑS�p�󔒒ǉ�
	IDM_DEL_TOPSPACE,		//	�s���󔒍폜
	IDM_DEL_LASTSPACE,		//	�s���󔒍폜
	IDM_DEL_LASTLETTER,		//	�s�������폜
	IDM_FILL_SPACE,			//	�I��͈͂��󔒂ɂ���
	IDM_RIGHT_SLIDE,		//	�E�Ɋ񂹂�
	0,
	IDM_INCR_DOT_LINES,		//	�S�̂��P�h�b�g�E��
	IDM_DECR_DOT_LINES,		//	�S�̂��P�h�b�g����
	0,
	IDM_SPACE_VIEW_TOGGLE,	//	�󔒂�\��	
	IDM_GRID_VIEW_TOGGLE,	//	�O���b�h����\��
	IDM_RIGHT_RULER_TOGGLE,	//	�E�K�C�h����\��
	0,
	IDM_PAGEL_DIVIDE		//	���̍s�ȍ~��V�łɕ���
};
//-------------------------------------------------------------------------------------------------








static HINSTANCE	ghInst;
static TCHAR		gatCntxIni[MAX_PATH];	//!<	

static HMENU	ghPopupMenu;	
static HMENU	ghUniSpMenu;
static HMENU	ghColourMenu;
static HMENU	ghFrameMenu;
static HMENU	ghUsrDefMenu;

static vector<CONTEXTITEM>	gvcCntxItem;
typedef vector<CONTEXTITEM>::iterator	CTXI_VITR;

static list<CONTEXTITEM>	gltCntxEdit;
typedef list<CONTEXTITEM>::iterator	CTXI_LITR;
//-------------------------------------------------------------------------------------------------

VOID	CntxEditBuild( VOID );

INT_PTR	CALLBACK CntxEditDlgProc( HWND, UINT, WPARAM, LPARAM );

VOID	CntxDlgLvInit( HWND );
VOID	CntxDlgAllListUp( HWND );
VOID	CntxDlgBuildListUp( HWND );

VOID	CntxDlgItemAdd( HWND );
VOID	CntxDlgItemDel( HWND );
VOID	CntxDlgItemSpinUp( HWND );
VOID	CntxDlgItemSpinDown( HWND );


INT_PTR	CALLBACK AccelKeyDlgProc( HWND, UINT, WPARAM, LPARAM );
INT_PTR	AccelKeyNotify( HWND, INT, LPNMHDR, list<ACCEL> * );

HRESULT	AccelKeyBindExistCheck( HWND, LPACCEL, list<ACCEL> * );
HRESULT	AccelKeyBindListMod( HWND, INT, LPACCEL, list<ACCEL> * );
HRESULT	AccelKeySettingReset( HWND, list<ACCEL> * );
HRESULT	AccelKeyListOutput( HWND );
HRESULT	AccelKeyBindString( LPACCEL, LPTSTR, UINT_PTR );
VOID	AccelKeyListInit( HWND, list<ACCEL> * );
BYTE	AccelHotModExchange( BYTE, BOOLEAN );
HRESULT	AccelKeyTableSave( list<ACCEL> * );



//-------------------------------------------------------------------------------------------------

/*!
	�N�����������EINI�f�B���N�g���i�[�Ƃ����������Ƃ�
	@param[in]	ptCurrent	��f�B���N�g��
	@param[in]	hInstance	���̃A�v���̎���
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT CntxEditInitialise( LPTSTR ptCurrent, HINSTANCE hInstance )
{
	UINT	dCount, cid;
	UINT	ams, ims;
	TCHAR	atKeyName[MIN_STRING];



	if( !(ptCurrent) || !(hInstance) )
	{
		if( ghPopupMenu ){	DestroyMenu( ghPopupMenu  );	}
		if( ghUniSpMenu ){	DestroyMenu( ghUniSpMenu  );	}
		if( ghColourMenu ){	DestroyMenu( ghColourMenu );	}
		if( ghFrameMenu ){	DestroyMenu( ghFrameMenu  );	}
		if( ghUsrDefMenu ){	DestroyMenu( ghUsrDefMenu );	}

		return S_OK;
	}



	ghInst = hInstance;

	StringCchCopy( gatCntxIni, MAX_PATH, ptCurrent );
	PathAppend( gatCntxIni, MZCX_INI_FILE );


	gvcCntxItem.clear();

	//	�������
	dCount = GetPrivateProfileInt( TEXT("Context"), TEXT("Count"), 0, gatCntxIni );
	if( 1 <= dCount )	//	�f�[�^�L��
	{
		for( ims = 0; dCount > ims; ims++ )
		{
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("CmdID%u"), ims );
			cid = GetPrivateProfileInt( TEXT("Context"), atKeyName, 0, gatCntxIni );

			for( ams = 0; ALL_ITEMS > ams; ams++ )
			{
				if( cid == gstContextItem[ams].dCommandoID )
				{
					gvcCntxItem.push_back( gstContextItem[ams] );
					break;
				}
			}
		}
	}
	else	//	�f�[�^�����Ȃ�f�t�H���j���[���\�z
	{
		for( ims = 0; DEF_ITEMS > ims; ims++ )
		{
			for( ams = 0; ALL_ITEMS > ams; ams++ )
			{
				if( gadDefItem[ims] == gstContextItem[ams].dCommandoID )
				{
					gvcCntxItem.push_back( gstContextItem[ams] );
					break;
				}
			}
		}
	}

	CntxEditBuild(  );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ł��������Ă�R���e�L�X�g���j���[���O���Ŏg��
*/
HMENU CntxMenuGet( VOID )
{
	return ghPopupMenu;
}
//-------------------------------------------------------------------------------------------------

/*!
	�R���e�L�X�g���j���[��g�ݗ��Ă�B
*/
VOID CntxEditBuild( VOID )
{
	UINT	d, e, num;
	TCHAR	atItem[MAX_STRING], atKey[MIN_STRING], atBuffer[SUB_STRING];
	CTXI_VITR	itMnItm;

	if( ghPopupMenu ){	DestroyMenu( ghPopupMenu  );	}	ghPopupMenu  = NULL;
	if( ghUniSpMenu ){	DestroyMenu( ghUniSpMenu  );	}	ghUniSpMenu  = NULL;
	if( ghColourMenu ){	DestroyMenu( ghColourMenu );	}	ghColourMenu = NULL;
	if( ghFrameMenu ){	DestroyMenu( ghFrameMenu  );	}	ghFrameMenu  = NULL;
	if( ghUsrDefMenu ){	DestroyMenu( ghUsrDefMenu );	}	ghUsrDefMenu = NULL;

	ghPopupMenu = CreatePopupMenu(  );

	e = 0;
	for( itMnItm = gvcCntxItem.begin(); gvcCntxItem.end() != itMnItm; itMnItm++ )
	{
		if( 0 >= itMnItm->dCommandoID )
		{
			AppendMenu( ghPopupMenu, MF_SEPARATOR, 0, NULL );
		}
		else
		{
			ZeroMemory( atBuffer, sizeof(atBuffer) );
			StringCchCopy( atItem, MAX_STRING, itMnItm->atString );	//	��ɋL�^

			if( IDM_INSFRAME_ALPHA <= itMnItm->dCommandoID && itMnItm->dCommandoID <= IDM_INSFRAME_TANGO )
			{	//	�g����										//	IDM_INSFRAME_ZULU
				num = itMnItm->dCommandoID - IDM_INSFRAME_ALPHA;
				FrameNameLoad( num, atBuffer, SUB_STRING );
				StringCchPrintf( atItem, MAX_STRING, TEXT("�g�F%s"), atBuffer );
			}
			else if( IDM_USER_ITEM_ALPHA <= itMnItm->dCommandoID && itMnItm->dCommandoID <= IDM_USER_ITEM_PAPA )
			{	//	���[�U�A�C�e������
				num = itMnItm->dCommandoID - IDM_USER_ITEM_ALPHA;
				UserDefItemNameget( num, atBuffer, SUB_STRING );
				StringCchPrintf( atItem, MAX_STRING, TEXT("���[�U�F%s"), atBuffer );
			}
			else
			{
				//	��������
			}

			if( 26 > e )
			{
				StringCchPrintf( atKey, MIN_STRING, TEXT("(&%c)"), 'A' + e );
				StringCchCat( atItem, MAX_STRING, atKey );
				e++;
			}

			switch( itMnItm->dCommandoID )
			{
				default:	AppendMenu( ghPopupMenu, MF_STRING, itMnItm->dCommandoID, atItem );	break;

				case IDM_MN_UNISPACE:
					ghUniSpMenu = CreatePopupMenu(  );
					for( d = 0; 8 > d; d++ ){	AppendMenu( ghUniSpMenu, MF_STRING, gstContextItem[CTS_UNISPACE+d].dCommandoID, gstContextItem[CTS_UNISPACE+d].atString );	}
					AppendMenu( ghPopupMenu, MF_POPUP, (UINT_PTR)ghUniSpMenu, atItem );
					break;

				case IDM_MN_COLOUR_SEL:
					ghColourMenu = CreatePopupMenu(  );
					for( d = 0; 5 > d; d++ ){	AppendMenu( ghColourMenu, MF_STRING, gstContextItem[CTS_COLOURINS+d].dCommandoID, gstContextItem[CTS_COLOURINS+d].atString );	}
					AppendMenu( ghPopupMenu, MF_POPUP, (UINT_PTR)ghColourMenu, atItem );
					break;

				case IDM_MN_INSFRAME_SEL:
					ghFrameMenu = CreatePopupMenu(  );
					for( d = 0; FRAME_MAX > d; d++ )
					{
					//	FrameNameLoad( d, atBuffer, SUB_STRING );
						AppendMenu( ghFrameMenu, MF_STRING, gstContextItem[CTS_FRAMEINS+d].dCommandoID, gstContextItem[CTS_FRAMEINS+d].atString );
					}
					FrameNameModifyPopUp( ghFrameMenu, 1 );
					AppendMenu( ghPopupMenu, MF_POPUP, (UINT_PTR)ghFrameMenu, atItem );
					break;

				case IDM_MN_USER_REFS:
					ghUsrDefMenu = CreatePopupMenu(  );
					UserDefMenuWrite( ghUsrDefMenu, 1 );
					AppendMenu( ghPopupMenu, MF_POPUP, (UINT_PTR)ghUsrDefMenu, atItem );
					break;
			}
		}
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ҏW�_�C�����O�J��
	@param[in]	hWnd	�E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT CntxEditDlgOpen( HWND hWnd )
{
	INT_PTR	iRslt, i;
	TCHAR	atKeyName[MIN_STRING], atBuff[MIN_STRING];
	CTXI_LITR	itEdit;

	iRslt = DialogBoxParam( ghInst, MAKEINTRESOURCE(IDD_CONTEXT_ITEM_DLG), hWnd, CntxEditDlgProc, 0 );
	if( IDOK == iRslt )
	{
		//	��U�Z�N�V��������ɂ���
		ZeroMemory( atBuff, sizeof(atBuff) );
		WritePrivateProfileSection( TEXT("Context"), atBuff, gatCntxIni );

		gvcCntxItem.clear();	i = 0;
		for( itEdit = gltCntxEdit.begin(); gltCntxEdit.end() != itEdit; itEdit++ )
		{
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("CmdID%d"), i );
			StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), itEdit->dCommandoID );
			WritePrivateProfileString( TEXT("Context"), atKeyName, atBuff, gatCntxIni );

			gvcCntxItem.push_back( *itEdit );
			i++;
		}

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), i );
		WritePrivateProfileString( TEXT("Context"), TEXT("Count"), atBuff, gatCntxIni );

		CntxEditBuild(  );

		return S_OK;
	}

	return E_ABORT;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ҏW�_�C�����O�{�b�N�X�̃��Z�[�W�n���h�������Ă΂�
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK CntxEditDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	INT		id;
	HWND	hWndCtl;
	UINT	codeNotify;

	CTXI_VITR	itMnItm;


	switch( message )
	{
		case WM_INITDIALOG:
			gltCntxEdit.clear();
			for( itMnItm = gvcCntxItem.begin(); gvcCntxItem.end() != itMnItm; itMnItm++ ){	gltCntxEdit.push_back( *itMnItm );	}
			CntxDlgLvInit( hDlg );
			CntxDlgAllListUp( hDlg );
			CntxDlgBuildListUp( hDlg );
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			id         = LOWORD(wParam);	//	���b�Z�[�W�𔭐��������q�E�C���h�E�̎��ʎq
			hWndCtl    = (HWND)lParam;		//	���b�Z�[�W�𔭐��������q�E�C���h�E�̃n���h��
			codeNotify = HIWORD(wParam);	//	�ʒm���b�Z�[�W
			switch( id )
			{
				case IDOK:		EndDialog( hDlg, IDOK );		return (INT_PTR)TRUE;
				case IDCANCEL:	EndDialog( hDlg, IDCANCEL );	return (INT_PTR)TRUE;

				case IDB_MENUITEM_ADD:		CntxDlgItemAdd( hDlg );	return (INT_PTR)TRUE;
				case IDB_MENUITEM_DEL:		CntxDlgItemDel( hDlg );	return (INT_PTR)TRUE;

				case IDB_MENUITEM_SPINUP:	CntxDlgItemSpinUp( hDlg );		return (INT_PTR)TRUE;
				case IDB_MENUITEM_SPINDOWN:	CntxDlgItemSpinDown( hDlg );	return (INT_PTR)TRUE;

				default:	break;
			}
			break;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e���̃��X�g�r���[�쐬
	@param[in]	hDlg	�_�C�����O�n���h��
*/
VOID CntxDlgLvInit( HWND hDlg )
{
	HWND	hLvWnd;
	LVCOLUMN	stLvColm;
	RECT	rect;


	hLvWnd = GetDlgItem( hDlg, IDLV_MENU_ALLITEM );
	GetClientRect( hLvWnd, &rect );

	ListView_SetExtendedListViewStyle( hLvWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP );

	ZeroMemory( &stLvColm, sizeof(LVCOLUMN) );
	stLvColm.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	stLvColm.fmt      = LVCFMT_LEFT;
	stLvColm.iSubItem = 0;

	stLvColm.pszText  = TEXT("���j���[�A�C�e��");
	stLvColm.cx       = rect.right - 23;
	ListView_InsertColumn( hLvWnd, 0, &stLvColm );


	hLvWnd = GetDlgItem( hDlg, IDLV_MENU_BUILDX );
	GetClientRect( hLvWnd, &rect );

	ListView_SetExtendedListViewStyle( hLvWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP );

	stLvColm.cx       = rect.right - 23;
	ListView_InsertColumn( hLvWnd, 0, &stLvColm );

}
//-------------------------------------------------------------------------------------------------

/*!
	�S�A�C�e�������X�g�r���[�Ƀu�b����
	@param[in]	hDlg	�_�C�����O�n���h��
*/
VOID CntxDlgAllListUp( HWND hDlg )
{
	HWND	hLvWnd;
	UINT	d;
	LVITEM	stLvi;
	TCHAR	atItem[SUB_STRING];


	hLvWnd = GetDlgItem( hDlg, IDLV_MENU_ALLITEM );

	ListView_DeleteAllItems( hLvWnd );

	ZeroMemory( &stLvi, sizeof(stLvi) );
	stLvi.mask    = LVIF_TEXT;
	stLvi.pszText = atItem;

	for( d = 0; ALL_ITEMS > d; d++ )
	{
		StringCchCopy( atItem, SUB_STRING, gstContextItem[d].atString );

		if( IDM_MN_UNISPACE == gstContextItem[d].dCommandoID || 
		IDM_MN_COLOUR_SEL   == gstContextItem[d].dCommandoID || 
		IDM_MN_INSFRAME_SEL == gstContextItem[d].dCommandoID || 
		IDM_MN_USER_REFS    == gstContextItem[d].dCommandoID )
		{
			StringCchCat( atItem, SUB_STRING, TEXT("�i�T�u���j���[�W�J�j") );
		}

		stLvi.iItem = d;
		ListView_InsertItem( hLvWnd, &stLvi );
	}
}
//-------------------------------------------------------------------------------------------------

/*!
	���j���[�\�������X�g�r���[�ɕ\��
	@param[in]	hDlg	�_�C�����O�n���h��
*/
VOID CntxDlgBuildListUp( HWND hDlg )
{
	HWND	hLvWnd;
	UINT	d;
	LVITEM	stLvi;
	TCHAR	atItem[SUB_STRING];

	CTXI_LITR	itMnItm;


	hLvWnd = GetDlgItem( hDlg, IDLV_MENU_BUILDX );

	ListView_DeleteAllItems( hLvWnd );

	ZeroMemory( &stLvi, sizeof(stLvi) );
	stLvi.mask    = LVIF_TEXT;
	stLvi.pszText = atItem;

	for( itMnItm = gltCntxEdit.begin(), d = 0; gltCntxEdit.end() != itMnItm; itMnItm++, d++ )
	{
		stLvi.iItem = d;
		if( 0 >= itMnItm->dCommandoID )
		{
			StringCchCopy( atItem, SUB_STRING, TEXT("---------------") );
		}
		else
		{
			StringCchCopy( atItem, SUB_STRING, itMnItm->atString );

			if( IDM_MN_UNISPACE == itMnItm->dCommandoID || 
			IDM_MN_COLOUR_SEL   == itMnItm->dCommandoID || 
			IDM_MN_INSFRAME_SEL == itMnItm->dCommandoID || 
			IDM_MN_USER_REFS    == itMnItm->dCommandoID )
			{
				StringCchCat( atItem, SUB_STRING, TEXT("�@�@[��") );
			}

		}

		ListView_InsertItem( hLvWnd, &stLvi );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e����ǉ�
	@param[in]	hDlg	�_�C�����O�n���h��
*/
VOID CntxDlgItemAdd( HWND hDlg )
{
	HWND	hListWnd, hBuildWnd;
	INT		iSel, iIns, iCount;
	CTXI_LITR	itMnItm;

	hListWnd  = GetDlgItem( hDlg, IDLV_MENU_ALLITEM );
	hBuildWnd = GetDlgItem( hDlg, IDLV_MENU_BUILDX );

	//	���̂ق�
	iSel = ListView_GetNextItem( hListWnd, -1, LVNI_ALL | LVNI_SELECTED );
	if( 0 > iSel )	 return;	//	�I�����ĂȂ�������I���

	iCount = ListView_GetItemCount( hBuildWnd );


	//	�I���������c�̎��ɂ����E���I���������Ȃ疖����
	iIns = ListView_GetNextItem( hBuildWnd, -1, LVNI_ALL | LVNI_SELECTED );
	if( (0 > iIns) || ((iIns+1) >= iCount) ){	gltCntxEdit.push_back( gstContextItem[iSel] );	}
	else
	{
		itMnItm = gltCntxEdit.begin();
		std::advance( itMnItm, iIns+1 );
		//	�C�e���[�^�̒��O�ɓ���
		gltCntxEdit.insert( itMnItm, gstContextItem[iSel] );
	}



	CntxDlgBuildListUp( hDlg );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e�����폜
	@param[in]	hDlg	�_�C�����O�n���h��
*/
VOID CntxDlgItemDel( HWND hDlg )
{
	HWND	hListWnd, hBuildWnd;
	INT		iSel;
	CTXI_LITR	itMnItm;

	hListWnd  = GetDlgItem( hDlg, IDLV_MENU_ALLITEM );
	hBuildWnd = GetDlgItem( hDlg, IDLV_MENU_BUILDX );

	iSel = ListView_GetNextItem( hBuildWnd, -1, LVNI_ALL | LVNI_SELECTED );
	if( 0 > iSel )	return;	//	�I�����ĂȂ�������I���

	itMnItm = gltCntxEdit.begin();
	std::advance( itMnItm, iSel );

	gltCntxEdit.erase( itMnItm );

	CntxDlgBuildListUp( hDlg );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e��������
	@param[in]	hDlg	�_�C�����O�n���h��
*/
VOID CntxDlgItemSpinUp( HWND hDlg )
{
	HWND	hBuildWnd;
	INT		iSel;
	CTXI_LITR	itTgtItm, itSwpItm;
	CONTEXTITEM	stItem;

	hBuildWnd = GetDlgItem( hDlg, IDLV_MENU_BUILDX );

	iSel = ListView_GetNextItem( hBuildWnd, -1, LVNI_ALL | LVNI_SELECTED );
	if( 0 >= iSel ){	 return;	}	//	�I�����ĂȂ�����ԏ�Ȃ�I���

	itTgtItm = gltCntxEdit.begin();
	std::advance( itTgtItm, iSel );
	itSwpItm = itTgtItm;
	itSwpItm--;	//	������͈�O

	StringCchCopy( stItem.atString, MIN_STRING, itTgtItm->atString );
	stItem.dCommandoID = itTgtItm->dCommandoID;

	gltCntxEdit.erase( itTgtItm );
	gltCntxEdit.insert( itSwpItm, stItem );

	CntxDlgBuildListUp( hDlg );

	ListView_SetItemState( hBuildWnd, --iSel, LVIS_SELECTED, LVIS_SELECTED );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e��������
	@param[in]	hDlg	�_�C�����O�n���h��
*/
VOID CntxDlgItemSpinDown( HWND hDlg )
{
	HWND	hBuildWnd;
	INT		iSel, iCount;
	CTXI_LITR	itTgtItm, itSwpItm;
	CONTEXTITEM	stItem;

	hBuildWnd = GetDlgItem( hDlg, IDLV_MENU_BUILDX );

	iSel = ListView_GetNextItem( hBuildWnd, -1, LVNI_ALL | LVNI_SELECTED );
	if( 0 > iSel )	return;	//	�I�����ĂȂ�������I���

	iCount = ListView_GetItemCount( hBuildWnd );
	if( iSel >= (iCount-1) )	 return;	//	���[�Ȃ�I���

	//	�����̈ʒu�֌W�ɒ���
	itSwpItm = gltCntxEdit.begin();
	std::advance( itSwpItm, iSel );
	itTgtItm = itSwpItm;
	itTgtItm++;	//	������͈��

	StringCchCopy( stItem.atString, MIN_STRING, itTgtItm->atString );
	stItem.dCommandoID = itTgtItm->dCommandoID;

	gltCntxEdit.erase( itTgtItm );
	gltCntxEdit.insert( itSwpItm, stItem );

	CntxDlgBuildListUp( hDlg );

	ListView_SetItemState( hBuildWnd, ++iSel, LVIS_SELECTED, LVIS_SELECTED );

	return;
}
//-------------------------------------------------------------------------------------------------



#ifdef ACCELERATOR_EDIT
//-------------------------------------------------------------------------------------------------

//	�z�b�g�L�[�R���g���[���́A�t�H�[�J�X���ĂăL�[���͂���ƔF������
//	�o�^�ς݂̃O���[�o���z�b�g�L�[������ƁA���������D�悵�ē���

/*!
	�A�N�Z�����[�g�L�[�ҏWDIALOGUE���J��
	@param[in]	hWnd		�E�C���h�E�n���h��
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT AccelKeyDlgOpen( HWND hWnd )
{
	INT_PTR	iRslt;

	LPACCEL	pstAccel;
	INT	iEntry;

	iRslt = DialogBoxParam( ghInst, MAKEINTRESOURCE(IDD_ACCEL_KEY_DLG), hWnd, AccelKeyDlgProc, 0 );
	if( IDOK == iRslt )
	{
		//	�A�N�Z���n���h���������K�v
		//	�A�N�Z���e�[�u���m�ۂ��ă������\���̂Ŋm��
		pstAccel = AccelKeyTableLoadAlloc( &iEntry );
		AccelKeyTableCreate( pstAccel, iEntry );

		ToolBarInfoChange( pstAccel, iEntry );

		FREE( pstAccel );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�N�Z���L�[�ҏW�_�C�����O�{�b�N�X�̃��Z�[�W�n���h�������Ă΂�
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK AccelKeyDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	static list<ACCEL>	cltAccel;

	INT		iAccEntry;
	LPACCEL	pstAccel;

	static  HWND	hHokyWnd;
	HWND	hLvWnd;
	LRESULT	lRslt;
	ACCEL	stAcce;
//	TCHAR	atBuffer[SUB_STRING];

	INT		i;
	INT		iItem;

	INT		id;
	HWND	hWndCtl;
	UINT	codeNotify;

//�z�b�g�L�[CONTROL�́A�X�y�[�X���g���Ȃ�

	switch( message )
	{
		case WM_INITDIALOG:
			hLvWnd = GetDlgItem( hDlg, IDLV_FUNCKEY_LIST );
			ListView_SetExtendedListViewStyle( hLvWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP );

			cltAccel.clear();
			pstAccel = AccelKeyTableGetAlloc( &iAccEntry );
			//	�����Ŋm�ۂ����A�N�Z���̓��X�g�ɂ��ĕێ����Ă���
			for( i = 0; iAccEntry > i; i++ ){	cltAccel.push_back( pstAccel[i] );	}
			FREE( pstAccel );

			AccelKeyListInit( hDlg, &cltAccel );

			hHokyWnd = GetDlgItem( hDlg, IDHKC_FUNCKEY_INPUT );
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			id         = LOWORD(wParam);	//	���b�Z�[�W�𔭐��������q�E�C���h�E�̎��ʎq
			hWndCtl    = (HWND)lParam;		//	���b�Z�[�W�𔭐��������q�E�C���h�E�̃n���h��
			codeNotify = HIWORD(wParam);	//	�ʒm���b�Z�[�W
			switch( id )
			{
				case IDOK:
					AccelKeyTableSave( &cltAccel );	//	�Z�[�u
				case IDCANCEL:
					cltAccel.clear();
					EndDialog( hDlg, id );
					return (INT_PTR)TRUE;


				case IDB_FUNCKEY_CLEAR:	//	����
					SendMessage( hHokyWnd, HKM_SETHOTKEY, 0, 0 );
					iItem = WndTagGet( hHokyWnd );
					AccelKeyBindListMod( hDlg, iItem, NULL, &cltAccel );
					return (INT_PTR)TRUE;


				case IDB_FUNCKEY_SET:	//	�Z�b�g
					lRslt = SendMessage( hHokyWnd, HKM_GETHOTKEY, 0, 0 );
					stAcce.key   = LOBYTE( lRslt );
					if( BST_CHECKED == IsDlgButtonChecked( hDlg, IDCB_FUNCKEY_SPACE ) )
					{	stAcce.key = VK_SPACE;	}
					stAcce.fVirt  = AccelHotModExchange( HIBYTE( lRslt ), 0 );
					stAcce.cmd    = 0;
					if( SUCCEEDED( AccelKeyBindExistCheck( hDlg, &stAcce, &cltAccel ) ) )
					{
						//	���Ԃ��ĂȂ�������o�^����
						iItem = WndTagGet( hHokyWnd );
						AccelKeyBindListMod( hDlg, iItem, &stAcce, &cltAccel );
					}
					return (INT_PTR)TRUE;


				case IDB_FUNCKEY_INIT:	//	�ݒ������������
					if( IDOK == MessageBox( hDlg, TEXT("�L�[�ݒ��������Ԃɖ߂���"), TEXT("�ݒ胊�Z�b�g����"), MB_OKCANCEL | MB_ICONQUESTION ) )
					{
						AccelKeySettingReset( hDlg, &cltAccel );
						AccelKeyListInit( hDlg, &cltAccel );
					}
					return (INT_PTR)TRUE;


				case IDB_FUNCKEY_FILEOUT:	//	�ݒ���t�@�C���ɏo��
					AccelKeyListOutput( hDlg );
					return (INT_PTR)TRUE;


				default:	break;
			}
			break;

		case WM_NOTIFY:
			return AccelKeyNotify( hDlg, (INT)(wParam), (LPNMHDR)(lParam), &cltAccel );

		default:	break;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�N�Z���L�[�ҏW�̃m�[�e�B�t�@�C���b�Z�[�W����
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	idFrom		���������R������ID�ԍ�
	@param[in]	pstNmhdr	�C�x���g���e
	@param[in]	*pltAccel	�A�N�Z���L�[�e�[�u��
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR AccelKeyNotify( HWND hDlg, INT idFrom, LPNMHDR pstNmhdr, list<ACCEL> *pltAccel )
{
	LPNMLISTVIEW	pstLv;
	LVITEM			stLvi;
	BYTE	bMod;
	HWND	hHokyWnd;

	list<ACCEL>::iterator	itAccel;

	if( IDLV_FUNCKEY_LIST == idFrom )	//	�ꗗ���X�g�r���[��
	{
		if( NM_CLICK == pstNmhdr->code )	//	�N���b�P���ꂽ��
		{
			pstLv = (LPNMLISTVIEW)pstNmhdr;
			pstLv->iItem;	//	���̃A�C�e�����m�F����

			ZeroMemory( &stLvi, sizeof(stLvi) );
			stLvi.mask     = LVIF_PARAM;	//	�R�}���h�R�[�h���m�ۂ���
			stLvi.iItem    = pstLv->iItem;
			ListView_GetItem( pstNmhdr->hwndFrom, &stLvi );

			hHokyWnd = GetDlgItem( hDlg, IDHKC_FUNCKEY_INPUT );
			WndTagSet( hHokyWnd , stLvi.iItem );	//	�I�������s���m��

			for( itAccel = (*pltAccel).begin(); itAccel != (*pltAccel).end(); itAccel++ )
			{
				if( stLvi.lParam == itAccel->cmd )
				{
					bMod = AccelHotModExchange( itAccel->fVirt, 1 );
					if( 0x20 == itAccel->key )
					{	SendMessage( hHokyWnd , HKM_SETHOTKEY, MAKEWORD(itAccel->key, bMod), 0 );	}
					else{	SendMessage( hHokyWnd, HKM_SETHOTKEY, MAKEWORD(itAccel->key, (bMod|HOTKEYF_EXT)), 0 );	}

					break;			//		���HOTKEYF_EXT�����ƁASpace���{���ɋ󔒂ɂȂ�
				}
			}
			if( itAccel == (*pltAccel).end() )	SendMessage( hHokyWnd, HKM_SETHOTKEY, 0, 0 );

			SetFocus( GetDlgItem( hDlg, IDHKC_FUNCKEY_INPUT ) );
		}
		return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�N�Z���e�[�u���������t�@�C������m�ۂ���E�������m�ۂ���̂ŕԂ�l�̈����ɒ���
	@param[out]	piEntry	�m�ۂ����G���g������߂�
	@return		�m�ۂ����e�[�u���\���̂̃|�C���^�[�Efree�ɒ���
*/
LPACCEL AccelKeyTableLoadAlloc( LPINT piEntry )
{
	UINT	dCount, dValue;
	UINT	i, aim = 0;
	TCHAR	atKeyName[MIN_STRING];
	LPACCEL	pstAccel = NULL;

	//	�������
	dCount = GetPrivateProfileInt( TEXT("Accelerator"), TEXT("Count"), 0, gatCntxIni );
	if( 1 <= dCount )	//	�f�[�^�L��
	{
		pstAccel = (LPACCEL)malloc( dCount * sizeof(ACCEL) );

		for( i = 0; dCount > i; i++ )
		{
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("AcCMD%d"), i );
			dValue = GetPrivateProfileInt( TEXT("Accelerator"), atKeyName, 0, gatCntxIni );
			if( 0 == dValue )	continue;	//	�ݒ肪�O�Ȃ牽�����Ȃ�
			pstAccel[aim].cmd = dValue;

			StringCchPrintf( atKeyName, MIN_STRING, TEXT("AcVirt%d"), i );
			dValue = GetPrivateProfileInt( TEXT("Accelerator"), atKeyName, 0, gatCntxIni );
			pstAccel[aim].fVirt = dValue;

			StringCchPrintf( atKeyName, MIN_STRING, TEXT("AcKey%d"), i );
			dValue = GetPrivateProfileInt( TEXT("Accelerator"), atKeyName, 0, gatCntxIni );
			pstAccel[aim].key = dValue;

			aim++;
		}
	}

	if( piEntry )	*piEntry = aim;

	return pstAccel;
}
//-------------------------------------------------------------------------------------------------

/*!
	�L�[�o�C���h���Q�Ƃ��āA����������
	@param[in]	pstAccel	�A�N�Z���L�[�e�[�u��
	@param[in]	ptBuffer	����������o�b�t�@�̃|�C���^�[
	@param[in]	cchSize		�o�b�t�@�̕�����
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT AccelKeyBindString( LPACCEL pstAccel, LPTSTR ptBuffer, UINT_PTR cchSize )
{
	TCHAR	atKey[MIN_STRING];

	ZeroMemory( ptBuffer, cchSize * sizeof(TCHAR) );

//	FVIRTKEY  FNOINVERT  �͏�Ɋ܂߂�

	if( FCONTROL & pstAccel->fVirt )	StringCchCat( ptBuffer, cchSize, TEXT("Ctrl + ") );
	if( FSHIFT   & pstAccel->fVirt )	StringCchCat( ptBuffer, cchSize, TEXT("Shift + ") );
	if( FALT     & pstAccel->fVirt )	StringCchCat( ptBuffer, cchSize, TEXT("Alt + ") );

	if( '0' <= pstAccel->key && pstAccel->key <= '9' )	//	VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
	{
		StringCchPrintf( atKey, MIN_STRING, TEXT("%c"), pstAccel->key );
	}
	else if( 'A' <= pstAccel->key && pstAccel->key <= 'Z' )	//	VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
	{
		StringCchPrintf( atKey, MIN_STRING, TEXT("%c"), pstAccel->key );
	}
	else if( 0x60 <= pstAccel->key && pstAccel->key <= 0x69 )	//	NUMPAD0 - NUMPAD9
	{
		StringCchPrintf( atKey, MIN_STRING, TEXT("NUMPAD%u"), pstAccel->key - 0x60 );
	}
	else if( 0x70 <= pstAccel->key && pstAccel->key <= 0x87 )	//	F1 - F24
	{
		StringCchPrintf( atKey, MIN_STRING, TEXT("F%u"), pstAccel->key - 0x70 + 1 );
	}
	else
	{
		switch( pstAccel->key )
		{
			case VK_CANCEL:		StringCchCopy( atKey, MIN_STRING, TEXT("Break") );	break;
			case VK_BACK:		StringCchCopy( atKey, MIN_STRING, TEXT("BackSpace") );	break;
			case VK_TAB:		StringCchCopy( atKey, MIN_STRING, TEXT("TAB") );	break;
			case VK_CLEAR:		StringCchCopy( atKey, MIN_STRING, TEXT("CLEAR") );	break;
			case VK_RETURN:		StringCchCopy( atKey, MIN_STRING, TEXT("Enter") );	break;
			case VK_PAUSE:		StringCchCopy( atKey, MIN_STRING, TEXT("Pause") );	break;
			case VK_CAPITAL:	StringCchCopy( atKey, MIN_STRING, TEXT("CAPITAL") );	break;
			case VK_KANA:		StringCchCopy( atKey, MIN_STRING, TEXT("KANA") );	break;
			case VK_ESCAPE:		StringCchCopy( atKey, MIN_STRING, TEXT("Esc") );	break;
			case VK_CONVERT:	StringCchCopy( atKey, MIN_STRING, TEXT("�ϊ�") );	break;
			case VK_NONCONVERT:	StringCchCopy( atKey, MIN_STRING, TEXT("�ٕϊ�") );	break;
			case VK_SPACE:		StringCchCopy( atKey, MIN_STRING, TEXT("Space") );	break;
			case VK_PRIOR:		StringCchCopy( atKey, MIN_STRING, TEXT("PageUp") );	break;
			case VK_NEXT:		StringCchCopy( atKey, MIN_STRING, TEXT("PageDown") );	break;
			case VK_END:		StringCchCopy( atKey, MIN_STRING, TEXT("End") );	break;
			case VK_HOME:		StringCchCopy( atKey, MIN_STRING, TEXT("Home") );	break;
			case VK_LEFT:		StringCchCopy( atKey, MIN_STRING, TEXT("��") );	break;
			case VK_UP:			StringCchCopy( atKey, MIN_STRING, TEXT("��") );	break;
			case VK_RIGHT:		StringCchCopy( atKey, MIN_STRING, TEXT("��") );	break;
			case VK_DOWN:		StringCchCopy( atKey, MIN_STRING, TEXT("��") );	break;
			case VK_SELECT:		StringCchCopy( atKey, MIN_STRING, TEXT("SELECT") );	break;
			case VK_PRINT:		StringCchCopy( atKey, MIN_STRING, TEXT("PRINT") );	break;
			case VK_EXECUTE:	StringCchCopy( atKey, MIN_STRING, TEXT("EXECUTE") );	break;
			case VK_SNAPSHOT:	StringCchCopy( atKey, MIN_STRING, TEXT("PrintScr") );	break;
			case VK_INSERT:		StringCchCopy( atKey, MIN_STRING, TEXT("Insert") );	break;
			case VK_DELETE:		StringCchCopy( atKey, MIN_STRING, TEXT("Delete") );	break;
			case VK_HELP:		StringCchCopy( atKey, MIN_STRING, TEXT("Help") );	break;
			case VK_LWIN:		StringCchCopy( atKey, MIN_STRING, TEXT("��Win") );	break;
			case VK_RWIN:		StringCchCopy( atKey, MIN_STRING, TEXT("�EWin") );	break;
			case VK_APPS:		StringCchCopy( atKey, MIN_STRING, TEXT("APPZ") );	break;
			case VK_SLEEP:		StringCchCopy( atKey, MIN_STRING, TEXT("SLEEP") );	break;
			case VK_MULTIPLY:	StringCchCopy( atKey, MIN_STRING, TEXT("NUM *") );	break;
			case VK_ADD:		StringCchCopy( atKey, MIN_STRING, TEXT("NUM +") );	break;
			case VK_SEPARATOR:	StringCchCopy( atKey, MIN_STRING, TEXT("NUM ,") );	break;
			case VK_SUBTRACT:	StringCchCopy( atKey, MIN_STRING, TEXT("NUM -") );	break;
			case VK_DECIMAL:	StringCchCopy( atKey, MIN_STRING, TEXT("NUM .") );	break;
			case VK_DIVIDE:		StringCchCopy( atKey, MIN_STRING, TEXT("NUM /") );	break;
			case VK_NUMLOCK:	StringCchCopy( atKey, MIN_STRING, TEXT("NumLock") );	break;
			case VK_SCROLL:		StringCchCopy( atKey, MIN_STRING, TEXT("ScrollLock") );	break;
			case VK_OEM_NEC_EQUAL:	StringCchCopy( atKey, MIN_STRING, TEXT("NUM =") );	break;
			case VK_BROWSER_BACK:		StringCchCopy( atKey, MIN_STRING, TEXT("�߂�") );	break;
			case VK_BROWSER_FORWARD:	StringCchCopy( atKey, MIN_STRING, TEXT("�i��") );	break;
			case VK_BROWSER_REFRESH:	StringCchCopy( atKey, MIN_STRING, TEXT("�X�V") );	break;
			case VK_BROWSER_STOP:		StringCchCopy( atKey, MIN_STRING, TEXT("��~") );	break;
			case VK_BROWSER_SEARCH:		StringCchCopy( atKey, MIN_STRING, TEXT("����") );	break;
			case VK_BROWSER_FAVORITES:	StringCchCopy( atKey, MIN_STRING, TEXT("�C��") );	break;
			case VK_BROWSER_HOME:		StringCchCopy( atKey, MIN_STRING, TEXT("�z��") );	break;
			case VK_VOLUME_MUTE:		StringCchCopy( atKey, MIN_STRING, TEXT("����") );	break;
			case VK_VOLUME_DOWN:		StringCchCopy( atKey, MIN_STRING, TEXT("����") );	break;
			case VK_VOLUME_UP:			StringCchCopy( atKey, MIN_STRING, TEXT("����") );	break;
			case VK_MEDIA_NEXT_TRACK:	StringCchCopy( atKey, MIN_STRING, TEXT("����") );	break;
			case VK_MEDIA_PREV_TRACK:	StringCchCopy( atKey, MIN_STRING, TEXT("�O��") );	break;
			case VK_MEDIA_STOP:			StringCchCopy( atKey, MIN_STRING, TEXT("��~") );	break;
			case VK_MEDIA_PLAY_PAUSE:	StringCchCopy( atKey, MIN_STRING, TEXT("�Đ�") );	break;
			case VK_LAUNCH_MAIL:		StringCchCopy( atKey, MIN_STRING, TEXT("����") );	break;
			case VK_LAUNCH_MEDIA_SELECT:StringCchCopy( atKey, MIN_STRING, TEXT("�I��") );	break;
			case VK_LAUNCH_APP1:		StringCchCopy( atKey, MIN_STRING, TEXT("APP1") );	break;
			case VK_LAUNCH_APP2:		StringCchCopy( atKey, MIN_STRING, TEXT("APP2") );	break;
			case VK_OEM_1:		StringCchCopy( atKey, MIN_STRING, TEXT(":") );	break;
			case VK_OEM_PLUS:	StringCchCopy( atKey, MIN_STRING, TEXT(";") );	break;
			case VK_OEM_COMMA:	StringCchCopy( atKey, MIN_STRING, TEXT(",") );	break;
			case VK_OEM_MINUS:	StringCchCopy( atKey, MIN_STRING, TEXT("-") );	break;
			case VK_OEM_PERIOD:	StringCchCopy( atKey, MIN_STRING, TEXT(".") );	break;
			case VK_OEM_2:		StringCchCopy( atKey, MIN_STRING, TEXT("/") );	break;
			case VK_OEM_3:		StringCchCopy( atKey, MIN_STRING, TEXT("@") );	break;
			case VK_OEM_4:		StringCchCopy( atKey, MIN_STRING, TEXT("[") );	break;
			case VK_OEM_5:		StringCchCopy( atKey, MIN_STRING, TEXT("\\") );	break;	//	��
			case VK_OEM_6:		StringCchCopy( atKey, MIN_STRING, TEXT("]") );	break;
			case VK_OEM_7:		StringCchCopy( atKey, MIN_STRING, TEXT("^") );	break;
			case VK_OEM_8:		StringCchCopy( atKey, MIN_STRING, TEXT("_") );	break;
			case VK_OEM_102:	StringCchCopy( atKey, MIN_STRING, TEXT("��") );	break;	//	��
			case VK_OEM_ATTN:	StringCchCopy( atKey, MIN_STRING, TEXT("CapsLock") );	break;
			case VK_OEM_COPY:	StringCchCopy( atKey, MIN_STRING, TEXT("�J�[�Ђ�") );	break;
			case VK_OEM_AUTO:	StringCchCopy( atKey, MIN_STRING, TEXT("��/�S ��1") );	break;
			case VK_OEM_ENLW:	StringCchCopy( atKey, MIN_STRING, TEXT("��/�S ��2") );	break;

			//	����Ȃ����͂r�c�j����ǉ�����
			default:	StringCchPrintf( atKey, MIN_STRING, TEXT("0x%02X"), pstAccel->key );	break;
		}
	}
//0x40 : unassigned
//0x07 : unassigned
//0x0A - 0x0B : reserved
//0x5E : reserved
//0x88 - 0x8F : unassigned
//0x97 - 0x9F : unassigned
//0xB8 - 0xB9 : reserved
//0xC1 - 0xD7 : reserved
//0xD8 - 0xDA : unassigned
//0xE0 : reserved
//0xE8 : unassigned
//0xFF : reserved

	StringCchCat( ptBuffer, cchSize, atKey );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�R�}���h�ԍ����Q�Ƃ��āA�q�b�g������A�N�Z�������������Ă�������
	@param[in]	ptText		�������ʂ��������镶����|�C���^�[
	@param[in]	cchSize		�o�b�t�@�̕�����
	@param[in]	dCommand	�R�}���h�ԍ�
	@param[in]	pstAccel	�A�N�Z���L�[�e�[�u��
	@param[in]	iEntry		�e�[�u���̃f�[�^��
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT AccelKeyTextBuild( LPTSTR ptText, UINT_PTR cchSize, DWORD dCommand, CONST LPACCEL pstAccel, INT iEntry )
{
	TCHAR	atKeystr[SUB_STRING];
	INT		i;

	for( i = 0; iEntry > i; i++ )
	{
		if( pstAccel[i].cmd == dCommand )
		{
			AccelKeyBindString( &(pstAccel[i]), atKeystr, SUB_STRING );

			StringCchCat( ptText, cchSize, TEXT("\r\n") );
			StringCchCat( ptText, cchSize, atKeystr );

			return S_OK;
		}
	}

	return E_OUTOFMEMORY;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�N�Z���L�[�ҏW�̃��X�g�r���[������
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	*pltAccel	�A�N�Z���L�[�e�[�u��
*/
VOID AccelKeyListInit( HWND hDlg, list<ACCEL> *pltAccel )
{
	HWND		hLvWnd;
	LVCOLUMN	stLvColm;
	LVITEM		stItem;
	RECT		rect;
	LONG		width, i, j;
	TCHAR		atBuffer[SUB_STRING];

	list<ACCEL>::iterator	itAccel;

	hLvWnd = GetDlgItem( hDlg, IDLV_FUNCKEY_LIST );
	GetClientRect( hLvWnd, &rect );
	width = rect.right - 23;
	width /= 2;

	ZeroMemory( &stLvColm, sizeof(LVCOLUMN) );
	stLvColm.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	stLvColm.fmt      = LVCFMT_LEFT;

	stLvColm.iSubItem = 0;
	stLvColm.pszText  = TEXT("�@�\");
	stLvColm.cx       = width;
	ListView_InsertColumn( hLvWnd, 0, &stLvColm );

	stLvColm.iSubItem = 1;
	stLvColm.pszText  = TEXT("�L�[�o�C���h");
	stLvColm.cx       = width;
	ListView_InsertColumn( hLvWnd, 1, &stLvColm );


	ZeroMemory( &stItem, sizeof(LVITEM) );

	for( i = 0, j = 0; ALL_ITEMS > i; i++ )
	{
		stItem.iItem = j;

		//	�֌W������͔�΂�
		if( 0 == gstContextItem[i].dCommandoID || 
		IDM_MN_UNISPACE     == gstContextItem[i].dCommandoID || 
		IDM_MN_COLOUR_SEL   == gstContextItem[i].dCommandoID || 
		IDM_MN_INSFRAME_SEL == gstContextItem[i].dCommandoID || 
		IDM_MN_USER_REFS    == gstContextItem[i].dCommandoID )
		{	continue;	}

		StringCchCopy( atBuffer, SUB_STRING, gstContextItem[i].atString );
		stItem.mask     = LVIF_TEXT | LVIF_PARAM;
		stItem.pszText  = atBuffer;
		stItem.lParam   = gstContextItem[i].dCommandoID;
		stItem.iSubItem = 0;
		ListView_InsertItem( hLvWnd, &stItem );

		ZeroMemory( atBuffer, sizeof(atBuffer) );

		for( itAccel = (*pltAccel).begin(); itAccel != (*pltAccel).end(); itAccel++ )
		{
			if( gstContextItem[i].dCommandoID == itAccel->cmd )
			{
				AccelKeyBindString( &(*itAccel), atBuffer, SUB_STRING );
				break;
			}
		}

		stItem.mask     = LVIF_TEXT;
		stItem.pszText  = atBuffer;
		stItem.iSubItem = 1;
		ListView_SetItem( hLvWnd, &stItem );

		j++;
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�N�Z���L�[�ƃz�b�g�L�[�̏C���q�����
	@param[in]	bSrc	���̏C���q�R�[�h
	@param[in]	bDrct	��O�A�N�Z�����z�b�g�@�O�z�b�g���A�N�Z��
	@return	�ϊ������R�[�h
*/
BYTE AccelHotModExchange( BYTE bSrc, BOOLEAN bDrct )
{
	BYTE	bDest = 0;

	if( bDrct )	//	�A�N�Z�����z�b�g
	{
		if( bSrc & FSHIFT )		bDest |= HOTKEYF_SHIFT;		//	�V�t�g
		if( bSrc & FCONTROL )	bDest |= HOTKEYF_CONTROL;	//	�R���g���[��
		if( bSrc & FALT )		bDest |= HOTKEYF_ALT;		//	�A���^�l�[�g
	}
	else	//	�z�b�g���A�N�Z��
	{
		if( bSrc & HOTKEYF_SHIFT )		bDest |= FSHIFT;	//	�V�t�g
		if( bSrc & HOTKEYF_CONTROL )	bDest |= FCONTROL;	//	�R���g���[��
		if( bSrc & HOTKEYF_ALT )		bDest |= FALT;		//	�A���^�l�[�g

		bDest |= (FVIRTKEY|FNOINVERT);	//	��ɂ���
	}

	return bDest;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�N�Z���e�[�u����ۑ�
	@param[in]	*pltAccel	�A�N�Z���L�[�e�[�u��
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT AccelKeyTableSave( list<ACCEL> *pltAccel )
{
	INT_PTR	i;
	TCHAR	atKeyName[MIN_STRING], atBuff[MIN_STRING];
	list<ACCEL>::iterator	itAccel;

	//	��U�Z�N�V��������ɂ���
	ZeroMemory( atBuff, sizeof(atBuff) );
	WritePrivateProfileSection( TEXT("Accelerator"), atBuff, gatCntxIni );

	i = 0;
	for( itAccel = (*pltAccel).begin(); itAccel != (*pltAccel).end(); itAccel++ )
	{
		StringCchPrintf( atKeyName, MIN_STRING, TEXT("AcCMD%d"), i );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), itAccel->cmd );
		WritePrivateProfileString( TEXT("Accelerator"), atKeyName, atBuff, gatCntxIni );

		StringCchPrintf( atKeyName, MIN_STRING, TEXT("AcVirt%d"), i );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), itAccel->fVirt );
		WritePrivateProfileString( TEXT("Accelerator"), atKeyName, atBuff, gatCntxIni );

		StringCchPrintf( atKeyName, MIN_STRING, TEXT("AcKey%d"), i );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), itAccel->key );
		WritePrivateProfileString( TEXT("Accelerator"), atKeyName, atBuff, gatCntxIni );

		i++;
	}

	StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), i );
	WritePrivateProfileString( TEXT("Accelerator"), TEXT("Count"), atBuff, gatCntxIni );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�N�Z���ݒ��������ԂɃA�b�[
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	*pltAccel	�A�N�Z���L�[�e�[�u��
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT AccelKeySettingReset( HWND hDlg, list<ACCEL> *pltAccel )
{
	HWND	hLvWnd;
	HACCEL	hAccel;	//	
	LPACCEL	pstAccel = NULL;
	INT		iItems, i;

	//	���X�̃e�[�u�����m��
	hAccel = LoadAccelerators( ghInst, MAKEINTRESOURCE(IDC_ORINRINEDITOR) );

	//	�܂����m��
	iItems = CopyAcceleratorTable( hAccel, NULL, 0 );
	if( 0 >= iItems )	return E_POINTER;

	pstAccel = (LPACCEL)malloc( iItems * sizeof(ACCEL) );
	if( !(pstAccel) )	return NULL;

	//	�{�̊m��
	iItems = CopyAcceleratorTable( hAccel, pstAccel, iItems );

	DestroyAcceleratorTable( hAccel );	//	�S�����`������Ԃ��󂵂Ă���

	(*pltAccel).clear();	//	�N�����[

	for( i = 0; iItems > i; i++ ){	(*pltAccel).push_back( pstAccel[i] );	}

	FREE( pstAccel );

	hLvWnd = GetDlgItem( hDlg, IDLV_FUNCKEY_LIST );
	ListView_DeleteAllItems( hLvWnd );	//	���X�g�r���[�͉󂵂Ă�����������

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ݒ���t�@�C���ɃG�N�X�|�[�g����
	@param[in]	hDlg	�_�C�����O�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT AccelKeyListOutput( HWND hDlg )
{
//	CONST  WCHAR	rtHead = 0xFEFF;	//	���j�R�[�h�e�L�X�g�w�b�_
	//	�t�@�C���`���́H�@SJIS��UTF8�ł���

	HANDLE	hFile;
	DWORD	wrote;
	BOOLEAN	bOpened;

	OPENFILENAME	stSaveFile;

	INT		i, iAccEntry;
	INT		j;
	LPACCEL	pstAccel;

	UINT_PTR	cchSz;
	INT		cbSize;


	TCHAR	atFilePath[MAX_PATH], atFileName[MAX_STRING];
	TCHAR	atCmdName[MIN_STRING], atKeyBind[SUB_STRING];
	TCHAR	atBuffer[MAX_PATH];

	CHAR	acString[BIG_STRING];


	ZeroMemory( &stSaveFile, sizeof(OPENFILENAME) );

	ZeroMemory( atFilePath,  sizeof(atFilePath) );
	ZeroMemory( atFileName,  sizeof(atFileName) );
	ZeroMemory( atBuffer,  sizeof(atBuffer) );

	StringCchCopy( atFilePath, MAX_PATH, TEXT("Accelerator.txt") );
#if 1
	//������ FileSaveDialogue ���o��
	stSaveFile.lStructSize     = sizeof(OPENFILENAME);
	stSaveFile.hwndOwner       = hDlg;
	stSaveFile.lpstrFilter     = TEXT("�e�L�X�g�t�@�C�� ( *.txt )\0*.txt\0�S�Ẵt�@�C�� ( *.* )\0*.*\0\0");
	stSaveFile.nFilterIndex    = 1;	//	�f�t�H�̃t�B���^�I����
	stSaveFile.lpstrFile       = atFilePath;
	stSaveFile.nMaxFile        = MAX_PATH;
	stSaveFile.lpstrFileTitle  = atFileName;
	stSaveFile.nMaxFileTitle   = MAX_STRING;
	stSaveFile.lpstrDefExt     = TEXT("txt");
//	stSaveFile.lpstrInitialDir = 
	stSaveFile.lpstrTitle      = TEXT("�ۑ�����t�@�C�������w�肵�Ă�");
	stSaveFile.Flags           = OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;

	bOpened = GetSaveFileName( &stSaveFile );
	if( !(bOpened) ){	return  E_ABORT;	}
	//	�L�����Z�����Ă��牽�����Ȃ�
#endif
	hFile = CreateFile( atFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE == hFile )
	{
		MessageBox( hDlg, TEXT("�t�@�C�����J���Ȃ�������E�E�E"), NULL, MB_OK | MB_ICONERROR );
		return E_HANDLE;
	}
	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );

	//WriteFile( hFile, &rtHead, 2, &wrote, NULL );

	//	�ݒ肳��Ă�e�[�u�����m��
	pstAccel = AccelKeyTableGetAlloc( &iAccEntry );
	for( i = 0; iAccEntry > i; i++ )
	{
		AccelKeyBindString( &(pstAccel[i]), atKeyBind, SUB_STRING );

		StringCchCopy( atCmdName, MIN_STRING, TEXT("�i���̕s���j") );
		for( j = 0; FULL_ITEMS > j; j++ )
		{
			if( gstContextItem[j].dCommandoID == pstAccel[i].cmd )
			{
				StringCchCopy( atCmdName, MIN_STRING, gstContextItem[j].atString );
				break;
			}
		}

		StringCchPrintf( atBuffer, MAX_PATH, TEXT("%s\t%s\r\n"), atCmdName, atKeyBind );
		StringCchLength( atBuffer, MAX_PATH, &cchSz );

		cbSize = WideCharToMultiByte( CP_ACP, WC_NO_BEST_FIT_CHARS, atBuffer, -1, acString, BIG_STRING, NULL, NULL );
									//	CP_UTF8, 0	
		//	�ϊ��������ɂ̓k���^�[�~�l�[�^���܂܂�Ă���̂Œ���
		WriteFile( hFile, acString, cbSize-1, &wrote, NULL );
	}
	FREE( pstAccel );

	SetEndOfFile( hFile );
	CloseHandle( hFile );

	MessageBox( hDlg, TEXT("�t�@�C���o�͂�����B"), TEXT("�i�E�́E�j��"), MB_OK | MB_ICONINFORMATION );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��̃R�}���h���g���Ă邩�ǂ����m�F
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	pstAccel	�A�N�Z���L�[�e�[�u��
	@param[in]	*pltAccel	�����̂�̈ꗗ
	@return		HRESULT		�I����ԃR�[�h�@S_OK���Ԃ薳���@E_ACCESSDENIED���Ԃ�L����
*/
HRESULT AccelKeyBindExistCheck( HWND hDlg, LPACCEL pstAccel, list<ACCEL> *pltAccel )
{
	INT	i;
	//BYTE	fVirt;
	WORD	dCommand;
	list<ACCEL>::iterator	itAccel;
	TCHAR	atFuncName[MIN_STRING], atMsg[MAX_STRING];

	for( itAccel = (*pltAccel).begin(); itAccel != (*pltAccel).end(); itAccel++ )
	{
		//	�����̂����邩�ǂ����T��
		//fVirt= itAccel->fVirt;
		//fVirt &= ~(FVIRTKEY|FNOINVERT);�ϊ����ɕt�������Ă�̂ŗv��Ȃ�
		if( pstAccel->key == itAccel->key && pstAccel->fVirt == itAccel->fVirt )
		{
			//	�����̂�������
			dCommand = itAccel->cmd;
			StringCchCopy( atFuncName, MIN_STRING, TEXT("�i���̕s���j") );

			for( i = 0; FULL_ITEMS > i; i++ )
			{
				if( dCommand == gstContextItem[i].dCommandoID )
				{
					StringCchCopy( atFuncName, MIN_STRING, gstContextItem[i].atString );
					break;
				}
			}

			StringCchPrintf( atMsg, MAX_STRING, TEXT("���̃L�[�o�C���h�́u%s�v�Ŏg���Ă��B"), atFuncName );
			MessageBox( hDlg, atMsg, TEXT("���Ԃ����������"), MB_OK | MB_ICONWARNING );

			return E_ACCESSDENIED;
		}
	}
	
	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�N�Z���L�[�ҏW�̃��X�g�r���[�Ƀf�[�^����
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	iItem		���삷��s�ԍ�
	@param[in]	pstAccel	�A�N�Z���L�[�e�[�u��
	@param[in]	*pltAccel	�����̂�̈ꗗ
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT AccelKeyBindListMod( HWND hDlg, INT iItem, LPACCEL pstAccel, list<ACCEL> *pltAccel )
{
	HWND	hLvWnd = GetDlgItem( hDlg, IDLV_FUNCKEY_LIST );
	HWND	hHkcWnd = GetDlgItem( hDlg, IDHKC_FUNCKEY_INPUT );
	LVITEM	stLvi;
	TCHAR	atBuffer[SUB_STRING];
	WORD	dCommand;
	list<ACCEL>::iterator	itAccel;

	//	�Y���s����A�R�}���h�ԍ��������Ă���
	ZeroMemory( &stLvi, sizeof(stLvi) );
	stLvi.mask     = LVIF_PARAM;	//	�R�}���h�R�[�h���m�ۂ���
	stLvi.iItem    = iItem;
	ListView_GetItem( hLvWnd, &stLvi );
	dCommand = stLvi.lParam;

	for( itAccel = (*pltAccel).begin(); itAccel != (*pltAccel).end(); itAccel++ )
	{
		if( dCommand ==  itAccel->cmd ){	break;	}
		//	�q�b�g������\���ł���
	}

	if( pstAccel )	//	�ύX�E�ǉ�
	{
		if( itAccel == (*pltAccel).end() )	//	�ǉ�
		{
			pstAccel->cmd = dCommand;
			(*pltAccel).push_back( *pstAccel );
		}
		else	//	�ύX
		{
			itAccel->key   = pstAccel->key;
			itAccel->fVirt = pstAccel->fVirt;
		}
		
		//	�\���p����������
		AccelKeyBindString( pstAccel, atBuffer, SUB_STRING );
		ZeroMemory( &stLvi, sizeof(stLvi) );
		stLvi.mask     = LVIF_TEXT;	//	�R�}���h�R�[�h���m�ۂ���
		stLvi.iItem    = iItem;
		stLvi.iSubItem = 1;
		stLvi.pszText  = atBuffer;
		ListView_SetItem( hLvWnd, &stLvi );
	}
	else	//	����
	{
		//	�q�b�g���ĂȂ��Ȃ�Ă��Ƃ͖����͂�����
		if( itAccel == (*pltAccel).end( ) ){	return E_HANDLE;	}

		//	�Y���v�f���폜
		(*pltAccel).erase( itAccel );
		//	�z�b�g�L�[�R���g���[�������
		SendMessage( hHkcWnd, HKM_SETHOTKEY, 0, 0 );
		//	���X�g�r���[�̕\������ɂ���
		ZeroMemory( atBuffer, sizeof(atBuffer) );
		ZeroMemory( &stLvi, sizeof(stLvi) );
		stLvi.mask     = LVIF_TEXT;	//	�R�}���h�R�[�h���m�ۂ���
		stLvi.iItem    = iItem;
		stLvi.iSubItem = 1;
		stLvi.pszText  = atBuffer;
		ListView_SetItem( hLvWnd, &stLvi );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���j���[������ɁA�A�N�Z���[�L�[���̂����蓖�ĂĂ���
	@param[in]	hWnd		���j���[�̂���E�C���h�E�n���h��
	@param[in]	pstAccel	���e�e�[�u���E��������NULL
	@param[in]	iEntry		�e�[�u���̃G���g����
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT AccelKeyMenuRewrite( HWND hWnd, LPACCEL pstAccel, CONST INT iEntry )
{
	HMENU		hMenu;
	WORD		dCmd;
	INT			i, j, iRslt;
	UINT		mRslt;
	UINT_PTR	d, cchSz;
	BOOLEAN		bModify;
	TCHAR		atBuffer[MAX_STRING], atBind[SUB_STRING];

	hMenu = GetMenu( hWnd );	//	�T�u���j���[�܂őS���C�P��

	for( i = 0; FULL_ITEMS > i; i++ )
	{
		//	�֌W������͔�΂�
		if( 0 == gstContextItem[i].dCommandoID )	continue;

		dCmd = gstContextItem[i].dCommandoID;	//	��{�I�ɂ���Ńq�b�g����͂�
		ZeroMemory( atBuffer, sizeof(atBuffer) );
		iRslt = GetMenuString( hMenu, dCmd, atBuffer, MAX_STRING, MF_BYCOMMAND );
		if( !(iRslt) )	continue;

		bModify = FALSE;

		//	��̓��e��j�󂷂�
		StringCchLength( atBuffer, MAX_STRING, &cchSz );
		for( d = 0; cchSz > d; d++ )
		{
			if( TEXT('\t') == atBuffer[d] )
			{
				atBuffer[d] = 0;
				bModify = TRUE;
				break;
			}
		}

		//	���̃R�}���h�̃G���g���[�͂��邩
		for( j = 0; iEntry > j; j++ )
		{
			if( dCmd == pstAccel[j].cmd )	//	��������쐬
			{
				ZeroMemory( atBind, sizeof(atBind) );
				AccelKeyBindString( &(pstAccel[j]), atBind, SUB_STRING );

				StringCchCat( atBuffer, MAX_STRING, TEXT("\t") );
				StringCchCat( atBuffer, MAX_STRING, atBind );

				bModify = TRUE;
				break;
			}
		}

		if( bModify )
		{
			//	�K�v�ɉ����ă`�F�b�N��Ԃ��m�ۂ���
			mRslt = GetMenuState( hMenu, dCmd, MF_BYCOMMAND );

			ModifyMenu( hMenu, dCmd, (MF_CHECKED & mRslt), dCmd, atBuffer );
			//	MF_BYCOMMAND | MF_STRING �͗����O�Ȃ̂ŁA�K�v�Ȃ̂̓`�F�b�N�̐��񂾂�
		}
	}

	DrawMenuBar( hWnd );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

#endif
