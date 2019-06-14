/*! @file
	@brief �S�ʓI�ȏ����Ƃ��ł�
	���̃t�@�C���� stdafx.cpp �ł��B
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
#ifdef _ORCOLL
#include "OrinrinCollector.h"
#else
#include "OrinrinEditor.h"
#endif
#include "Entity.h"
//------------------------------------------------------------------------------------------------------------------------

#ifndef _ORRVW
extern  UINT	gbUniRadixHex;	//	���j�R�[�h���l�Q�Ƃ��P�U�i���ł��邩
#endif
//------------------------------------------------------------------------------------------------------------------------

//	�����񌟍��͊Ȉ�Boyer-Moore�@
typedef struct tagFINDPATTERN
{
	TCHAR	cchMozi;
	INT		iDistance;

} FINDPATTERN, *LPFINDPATTERN;
//--------------------------------

//	�m�F�`�F�b�N���b�Z�[�W�_�C�����O�p
typedef struct tagMSGBOXMSG
{
	TCHAR	atMsg1[MAX_PATH];	//!<	������P
	TCHAR	atMsg2[MAX_PATH];	//!<	������Q

	UINT	bChecked;	//!<	��O�`�F�L��ꂽ�@�O�m�[�`�F�b�N

} MSGBOXMSG, *LPMSGBOXMSG;

//------------------------------------------------------------------------------------------------------------------------

#ifdef SPMOZI_ENCODE

EXTERNED UINT	gbSpMoziEnc;	//!<	�@��ˑ������𐔒l�Q�ƃR�s�[����

//	�@��ˑ�����	2015/01/23	�Ȃ񂩑�����
static CONST TCHAR	gatSpMoziList[] = {
	TEXT("�@�A�B�C�D�E�F�G�H�I�J�K�L�M�N�O�P�Q�R�S�T�U�V�W�X�Y�Z�[�\�]�@�A�B�C�D�E�F�G�H�I�_�`�a�b�c�d�e�f�g�h")
	TEXT("�i�j�k�l�m�n�o�p�q�r�s�t�u�~�����������������������������������߁燓����ہڇ����恿�����U�V�W�h�\�]")	//	100
	TEXT("�^�_�`�a�b�c�d�e�f�g�i�j�k�l�m�n�o�p�q�r�s�t�u�v�w�x�y�z�{�|�}�~������������������������������������")
	TEXT("����������������������������������������������������������������������������������������������������")	//	200
	TEXT("����������������������������������������������������������������������������������������������������")
	TEXT("���������������@�A�B�C�D�E�F�G�H�I�J�K�L�M�N�O�P�Q�R�S�T�U�V�W�X�Y�Z�[�\�]�^�_�`�a�b�c�d�e�f�g�h�i�j")	//	300
	TEXT("�k�l�m�n�o�p�q�r�s�t�u�v�w�x�y�z�{�|�}�~������������������������������������������������������������")
	TEXT("����������������������������������������������������������������������������������������������������")	//	400
	TEXT("�������������������������������������������������������������������������������������������@�A�B�C�D")
	TEXT("�E�F�G�H�I�J�K") };	//	457


#define SPMOZI_CNT	457	//	������
#endif
//------------------------------------------------------------------------------------------------------------------------

#ifdef SPMOZI_ENCODE
/*!
	�Ώۂ̃��j�R�[�h������SJIS�̋@��ˑ������ł��邩
	@param[in]	tMozi	�`�F�b�N���郆�j�R�[�h����
	@return	��O�Y������@�O���Ȃ�
*/
UINT IsSpMozi( TCHAR tMozi )
{
	UINT	i;

	if( !(gbSpMoziEnc) )	return 0;	//	���g�p�Ȃ炢�ł��Y�����Ȃ�

	for( i = 0; SPMOZI_CNT > i; i++ )
	{
		if( gatSpMoziList[i] == tMozi )	return 1;
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------------------
#endif

/*!
	�E�C���h�E�Ƀ��[�U�f�[�^����������
	@param[in]	hWnd	�������݂����E�C���h�E�̃n���h��
	@param[in]	tag		�������ސ��l�BLONG�l
	@return	����
*/
VOID WndTagSet( HWND hWnd, LONG_PTR tag )
{
	SetWindowLongPtr( hWnd, GWLP_USERDATA, tag );
}
//------------------------------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�̃��[�U�f�[�^��ǂݍ���
	@param[in]	hWnd	�ǂݍ��ރE�C���h�E�̃n���h��
	@return	���[�U�f�[�^�l�B�����l��0
*/
LONG_PTR WndTagGet( HWND hWnd )
{
	return GetWindowLongPtr( hWnd, GWLP_USERDATA );
}
//-------------------------------------------------------------------------------------------------

/*!
�t�@�C���̊g���q���r
	@param[in]	ptFile	�`�F�L�肽���t�@�C����
	@param[in]	ptExte	�`�F�L�肽���g���q�E�s�����h���܂߂�
	@return		BOOLEAN	��O�Y���g���q�ł���@�O�Ⴄ���A�g���q������
*/
BOOLEAN FileExtensionCheck( LPTSTR ptFile, LPTSTR ptExte )
{
	TCHAR	atExBuf[10];
	LPTSTR	ptExten;

	//	�g���q���m�F�E�h�b�g���݂���`��
	ptExten = PathFindExtension( ptFile );	//	�g���q�������Ȃ�NULL�A�Ƃ��������[�ɂȂ�
	if( 0 == *ptExten ){	return 0;	}
	//	�g���q�w�肪�Ȃ��Ȃ�Ȃɂ��ł��Ȃ�

	StringCchCopy( atExBuf, 10, ptExten );
	CharLower( atExBuf );	//	��r�̂��߂ɏ������ɂ����Ⴄ

	//	���e�Ⴄ�Ȃ�^�l�ƂȂ�
	if( StrCmp( atExBuf, ptExte ) ){	return 0;	}

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�v���r���p�ɁAHTML�I�Ƀ��o�C�������G���e�B�e�B����ESJIS�p
	@param[in]	adMozi	�`�F�L�肽�����j�R�[�h����
	@param[out]	pcStr	�G���e�B�e�B�����ꍇ�́A���̕����������
	@param[in]	cbSize	������o�b�t�@�̕������i�o�C�g���j
	@return		BOOLEAN	��O�G���e�B�e�B�����@�O��薳��
*/
BOOLEAN HtmlEntityCheckA( TCHAR adMozi, LPSTR pcStr, UINT_PTR cbSize )
{
	INT	i;

	ZeroMemory( pcStr, cbSize );

	for( i = 0; 4 > i; i++ )
	{
		if( 0 == gstEttySP[i].dUniCode )	break;

		if( gstEttySP[i].dUniCode == adMozi )
		{
			StringCchCopyA( pcStr, cbSize, gstEttySP[i].acCodeA );
			return TRUE;
		}
	}

	return FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�v���r���p�ɁAHTML�I�Ƀ��o�C�������G���e�B�e�B����EUnicode�p
	@param[in]	adMozi	�`�F�L�肽�����j�R�[�h����
	@param[out]	ptStr	�G���e�B�e�B�����ꍇ�́A���̕����������
	@param[in]	cchSize	������o�b�t�@�̕�����
	@return		BOOLEAN	��O�G���e�B�e�B�����@�O��薳��
*/
BOOLEAN HtmlEntityCheckW( TCHAR adMozi, LPTSTR ptStr, UINT_PTR cchSize )
{
	INT	i;

	ZeroMemory( ptStr, cchSize * sizeof(TCHAR) );

	for( i = 0; 4 > i; i++ )
	{
		if( 0 == gstEttySP[i].dUniCode )	break;

		if( gstEttySP[i].dUniCode == adMozi )
		{
			StringCchCopy( ptStr, cchSize, gstEttySP[i].atCodeW );
			return TRUE;
		}
	}

	return FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	SJIS����"&#0000;"���l�����ă��j�R�[�h�ɕϊ�
	"&#"����n�܂镔�������̎Q�Ƃ��ǂ����`�F�b�N�E�P�O�����݂�A�k���^�[�~�l�[�^�܂�
	@param[in]	pcStr	�m�F���镶����̐擪
	@return		TCHAR	���j�R�[�h�����i���l�̕������̂܂܁j
*/
TCHAR UniRefCheck( LPSTR pcStr )
{
	CHAR	acValue[10];
	PCHAR	pcEnd;
	UINT	i, code;
	INT		radix = 10;
	BOOLEAN	bXcode = FALSE;

	ZeroMemory( acValue, sizeof(acValue) );
	if( NULL == pcStr[2] )	return 0x0000;	//	���[�������ꍇ

	pcStr += 2;	//	&#������i�߂�

	//	&#xhhhh;�ȂP�U�i�����m�F
	if( 'x' == pcStr[0] || 'X' == pcStr[0] )
	{
		bXcode = TRUE;
		pcStr++;	//	�����炪�{��
		radix = 16;
	}

	for( i = 0; 10 > i; i++ )
	{
		if( ';' == pcStr[i] )	break;	//	���[�m�F

		if( NULL == pcStr[i] )	return 0x0000;	//	���[�������ꍇ

		if( 0 > (INT)(pcStr[i]) )	return 0x0000;	//	�Ƃɂ����֌W�Ȃ������ꍇ

		if( bXcode )
		{
			if( isxdigit( pcStr[i] ) ){	acValue[i] = pcStr[i];	}
			else	return 0x0000;	//	�オ���l����Ȃ������ꍇ
		}
		else
		{
			if( isdigit( pcStr[i] ) ){	acValue[i] = pcStr[i];	}
			else	return 0x0000;	//	�オ���l����Ȃ������ꍇ
		}
	}
	if( 10 <= i ){	return 0x0000;	}	//	�Ȃ񂩐U��؂��Ă�

	code = strtoul( acValue, &pcEnd, radix );	//	�i���ɍ��킹�ĕϊ������Ⴄ

//	�@��ˑ������ϊ��@���j�R�[�h�Ƃ��Ĉ����Ă���̂ŁA�����ňꗥ�����Ŗ�薳��
//	SPMOZI_ENCODE


	if( 0xFFFF < code ){	code = 0x0000;	}	//	���炩�ɂ��������ꍇ

	return (TCHAR)code;
}
//-------------------------------------------------------------------------------------------------

/*!
	SJIS��������󂯎���āA���̎Q�Ƃ����j�R�[�h���l�Q�Ƃɕϊ�����
	@param[in]	pcMoto	���̕�����
	@return	LPSTR	�ϊ�����������E�󂯎�����ق���FREE����E��������������NULL
*/
LPSTR SjisEntityExchange( LPCSTR pcMoto )
{
	LPSTR		pcOutput, pcTemp;
	CHAR		acSrp[12], acChk[3], acUni[10];
	UINT		check, el;
	UINT_PTR	szMoto, szStr, sc, dt, se, rp, cbSz;
	BOOLEAN		bStr = FALSE, bHit = FALSE;

	CONST ENTITYLIST	*pstEty;

	szMoto = strlen( pcMoto );
	szStr = szMoto + 2;
	pcOutput = (LPSTR)malloc( szStr );	//	�k���^�[�~�l�[�^�p
	if( !(pcOutput) )	return NULL;
	ZeroMemory( pcOutput, szStr );	


	for( sc = 0, dt = 0; szMoto > sc; sc++ )
	{
		if( '&' == pcMoto[sc] )
		{
			se = sc;	//	�g�p����
			bStr = FALSE;

			for( rp = 0; 10 > rp; rp++ )
			{
				acSrp[rp] = pcMoto[se++];	acSrp[rp+1] = 0x00;
				if( ';'  == acSrp[rp] ){	bStr = TRUE;	break;	}

				if( 1 == rp )
				{
					if( 0 > acSrp[1] )	break;	//	isalpha�Ƀ}�C�i�X�l�ł�����H
					if( !( isalpha( acSrp[1] ) ) )	break;
				}
			}
			//	���̎��_�ŁAbStr��FALSE�Ȃ�A&xxxx; �ȓ��e����Ȃ�����
			if( !(bStr) ){	pcOutput[dt++] = pcMoto[sc];	continue;	}

			//	���̎��̎����m�F���āA�����͈͂��i�荞��
			acChk[0] = acSrp[1];	acChk[1] = 0x00;
			CharLowerA( acChk );
			check = acChk[0] - 'a';	//	�ʒu���m�F
			//	20120301	�O�`�Q�T�͈̔͊O�ł���̂Ȃ�A&xxxx;�ł͂Ȃ�
			if( 26 <= check ){	pcOutput[dt++] = pcMoto[sc];	continue;	}

			pstEty = gpstEntitys[check];

			bHit = FALSE;
			//	�����肪���邩�ǂ������Ă���
			for( el = 0; 0 != pstEty[el].dUniCode; el++ )
			{
				if( 0 == StrCmpA( acSrp, pstEty[el].acCodeA ) )	//	�q�b�g
				{
					szStr += 4;	//	�͂ݏo�����獢��̂ŏ������₷
					pcTemp = (LPSTR)realloc( pcOutput, szStr );
					if( pcTemp )	pcOutput = pcTemp;

					bHit = TRUE;
					StringCchPrintfA( acUni, 10, ("&#%d;"), pstEty[el].dUniCode );
					StringCchLengthA( acUni, 10, &cbSz );
					for( rp = 0; cbSz > rp; rp++ )
					{
						pcOutput[dt++] = acUni[rp];
					}
					sc = se - 1;	//	�ʒu���������߂��E���[�v�Ł{�{����邩������Ă���
					break;
				}
			}
			//	���̎��_�ŁAbHit��FALSE�Ȃ�A���̂̂�����e����Ȃ�����
			if( !(bHit) ){	pcOutput[dt++] = pcMoto[sc];	}
		}
		else	//	�G���e�B�e�B����Ȃ��Ȃ炻�̂܂܈ڂ��΂���
		{
			pcOutput[dt++] = pcMoto[sc];
		}
	}

	return pcOutput;
}
//-------------------------------------------------------------------------------------------------

/*!
	�V�t�gJIS��������󂯎���āA���j�R�[�h������ɕϊ����ĕԂ�l�E�Ă񂾂ق���free����̖Y����
	@param[in]	pcBuff	�V�t�gJIS������o�b�t�@�̃|�C���^�[
	@return		�m�ۂ������j�R�[�h������o�b�t�@�̃|�C���^�[�Efree�Z��
*/
LPTSTR SjisDecodeAlloc( LPSTR pcBuff )
{
	DWORD	cbWrtSize;
	LPSTR	pcPos, pcChk, pcPosEx;

	DWORD	cchSize, cchWrtSize;
	LPTSTR	ptBuffer, ptWrtpo;
	TCHAR	chMozi;

	DWORD	dStart, dEnd;

	if( !(pcBuff) ){	return NULL;	}	//	�f�[�^��������������I���

	//	�o�͗p���j�R�[�h�o�b�t�@���m�ۂ���B�K�v�ȃT�C�Y�����߂�
	cchSize = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pcBuff, -1, NULL, 0 );

	cchSize += 2;	//	�o�b�t�@���̗̂ʂȂ̂ł����ŕύX���Ă���
	ptBuffer = (LPTSTR)malloc( cchSize * sizeof(TCHAR) );
	ZeroMemory( ptBuffer, cchSize * sizeof(TCHAR) );

	dStart = 0;
	dEnd   = 0;

//	pcPos = pcBuff;
	ptWrtpo = ptBuffer;

	//�܂�SJIS�̂܂܂ŁA���̎Q�Ƃ����j�R�[�h�Q�Ƃɕϊ�����
	pcPosEx = SjisEntityExchange( pcBuff );

	pcPos = pcPosEx;

	pcChk = StrStrA( pcPosEx, "&#" );	//	�L�[���[�h�Ƀq�b�g���邩�ǂ���
	while( pcChk )
	{
		//	���̉ӏ��̎�O�܂ŕϊ�
		cbWrtSize  = pcChk - pcPos;	//	�����܂ł̃T�C�Y���߂�
		cchWrtSize = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pcPos, cbWrtSize, ptWrtpo, cchSize );

		ptWrtpo += cchWrtSize;	//	�����ꏊ�ړ�
		cchSize -= cchWrtSize;	//	�c�蕶����

		chMozi = UniRefCheck( pcChk );	//	�L���ȓ��e���ǂ����m�F
		if( 0 != chMozi )	//	�L���ȃi�j�J��������
		{
			*ptWrtpo = chMozi;	//	�������Ԃ������
			ptWrtpo++;			//	���ɐi�߂�
			cchSize--;			//	�c�蕶�����͌��炷
			pcChk = StrStrA( pcChk, ";" );	//	�I�[�܂Ői�߂Ă���
			pcChk++;	//	';'�܂Ŋ܂ނ̂ŁA����ɂ��̎����炪�{��
		}
		else	//	�֌W�Ȃ��Ȃ�A�q�b�g�����Q�������J�L�R���āA���ɐi��
		{
			cchWrtSize = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pcChk, 2, ptWrtpo, cchSize );

			ptWrtpo += cchWrtSize;	//	�����ꏊ�ړ�
			cchSize -= cchWrtSize;	//	�c�蕶����
			pcChk += 2;	//	�g�������i�߂�
		}
		pcPos = pcChk;	//	���͂������璲�ׂĂ���

		pcChk = StrStrA( pcPos , "&#" );	//	�L�[���[�h�Ƀq�b�g���邩�ǂ���
	}
	//	�c��̕����A�������͍ŏ�����Ō�܂ł�ϊ�
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pcPos, -1, ptWrtpo, cchSize );

	FREE( pcPosEx );

	return ptBuffer;
}
//-------------------------------------------------------------------------------------------------

/*!
	������������Ƃ��āA�s���ƍő�h�b�g�����v�Z
	@param[in]	ptText	�`�F�L�肽�����j�R�[�h������󂯎��
	@param[out]	piLine	�s���Ԃ�
	@return		�ő�h�b�g��
*/
INT TextViewSizeGet( LPCTSTR ptText, PINT piLine )
{
	UINT_PTR	cchSize, i;
	INT		xDot, yLine, dMaxDot;

	wstring	wString;


	StringCchLength( ptText, STRSAFE_MAX_CCH, &cchSize );

	yLine = 1;	dMaxDot = 0;
	for( i = 0; cchSize > i; i++ )
	{
		if( CC_CR == ptText[i] && CC_LF == ptText[i+1] )	//	���s�ł�������
		{
			//	�h�b�g���m�F
			xDot = ViewStringWidthGet( wString.c_str() );
			if( dMaxDot < xDot )	dMaxDot = xDot;

			wString.clear( );
			i++;		//	0x0D,0x0A������A�땶����΂��̂��|�C���g
			yLine++;	//	���s��������s��������
		}
		else if( CC_TAB == ptText[i] )
		{
			//	�^�u�͖����������Ƃɂ���
		}
		else
		{
			wString += ptText[i];
		}
	}

	if( 1 <= wString.size() )	//	�ŏI�s�m�F
	{
		//	�h�b�g���m�F
		xDot = ViewStringWidthGet( wString.c_str() );
		if( dMaxDot < xDot )	dMaxDot = xDot;
	}

	if( piLine )	*piLine = yLine;	//	��s�������Ƃ��Ă��P�s�͂���
	return dMaxDot;

//ViewStringWidthGet�́AViewCentral�AOrinrinViewer�ɁA���ꂼ�ꂠ��
}
//-------------------------------------------------------------------------------------------------

/*!
	���ݍs����A���̍s�̐擪�ֈړ�
	@param[in]	pt	���s�������J�n����Ƃ���
	@return		���s�̎��̈ʒu
*/
LPCTSTR NextLineW( LPCTSTR pt )
{
	while( *pt && *pt != 0x000D ){	pt++;	}

	if( 0x000D == *pt )
	{
		pt++;
		if( 0x000A == *pt ){	pt++;	}
	}

	return pt;
}
//-------------------------------------------------------------------------------------------------
LPTSTR NextLineW( LPTSTR pt )
{
	while( *pt && *pt != 0x000D ){	pt++;	}

	if( 0x000D == *pt )
	{
		pt++;
		if( 0x000A == *pt ){	pt++;	}
	}

	return pt;
}
//-------------------------------------------------------------------------------------------------


/*!
	���ݍs����A���̍s�̐擪�ֈړ�
	@param[in]	pt	���s�������J�n����Ƃ���
	@return		���s�̎��̈ʒu
*/
LPSTR NextLineA( LPSTR pt )
{
	while( *pt && *pt != 0x0D ){	pt++;	}

	if( 0x0D == *pt )
	{
		pt++;
		if( 0x0A == *pt ){	pt++;	}
	}

	return pt;
}
//-------------------------------------------------------------------------------------------------


#ifndef _ORRVW

/*!
	���j�R�[�h��������󂯎���āASJIS������ɕϊ��E�Ԃ�l�͌Ă񂾕���free���ɂ�
	@param[in]	ptTexts	���j�R�[�h������o�b�t�@�̃|�C���^�[
	@return		�m�ۂ���SJIS������o�b�t�@�̃|�C���^�[�Efree�Z��
*/
LPSTR SjisEncodeAlloc( LPCTSTR ptTexts )
{
	TCHAR	atMozi[2];
	CHAR	acSjis[10];
	BOOL	bCant = FALSE;
	INT		iRslt;
	UINT_PTR	cchSize, d, cbSize;
	LPSTR	pcString;

	string	sString;
	stringstream ss;

	if( !(ptTexts) ){	return NULL;	}

	//	�����m�F
	StringCchLength( ptTexts, STRSAFE_MAX_CCH, &cchSize );


	atMozi[1] = 0;
	for( d = 0; cchSize > d; d++ )	//	�땶�����ϊ����Ă���
	{
		atMozi[0] = ptTexts[d];
		ZeroMemory( acSjis, sizeof(acSjis) );

		iRslt = WideCharToMultiByte( CP_ACP, WC_NO_BEST_FIT_CHARS, atMozi, 1, acSjis, 10, "?", &bCant );
		//	�ϊ��o����΂���ł����A�_���Ȃ琔�l�Q�ƌ`���ɂ����Ⴄ
		if( bCant )
		{
			if( gbUniRadixHex ){	StringCchPrintfA( acSjis, 10 , ("&#x%X;"), ptTexts[d] );	}
			else{					StringCchPrintfA( acSjis, 10 , ("&#%d;"),  ptTexts[d] );	}
		}

#ifdef SPMOZI_ENCODE
		if( IsSpMozi( ptTexts[d] ) )	//	�@��ˑ������ϊ�
		{
			if( gbUniRadixHex ){	StringCchPrintfA( acSjis, 10 , ("&#x%X;"), ptTexts[d] );	}
			else{					StringCchPrintfA( acSjis, 10 , ("&#%d;"),  ptTexts[d] );	}
		}
#endif

		ss << string( acSjis );
	}

	ss >> sString;
	cbSize = sString.size( ) + 2;
	pcString = (LPSTR)malloc( cbSize );
	ZeroMemory( pcString, cbSize );
	StringCchCopyA( pcString, cbSize, sString.c_str() );

	return pcString;
}
//-------------------------------------------------------------------------------------------------
#endif


/*!
	BM�����e�[�u������
	@param[in]	ptPattern	�����p�^�[��
	@return	LPFINDPATTERN	�쐬���������e�[�u��
*/
LPFINDPATTERN FindTableMake( LPCTSTR ptPattern )
{
	UINT		i;
	UINT_PTR	dLength;
	LPFINDPATTERN	pstPtrn;

	//	�e�[�u�����p�^�[���̒����ŏ���������
	StringCchLength( ptPattern, STRSAFE_MAX_CCH, &dLength );
	pstPtrn = (LPFINDPATTERN)malloc( (dLength+1) * sizeof(FINDPATTERN) );
	ZeroMemory( pstPtrn, (dLength+1) * sizeof(FINDPATTERN) );

	for( i = 0; dLength >= i; i++ ){	pstPtrn[i].iDistance =  dLength;	}

	//	�p�^�[���̐擪����A�����ɑΉ�����ʒu�ɖ�������̒�����o�^����
	while( dLength > 0 )
	{
		i = 0;
		while( pstPtrn[i].cchMozi )
		{
			if( pstPtrn[i].cchMozi ==  *ptPattern ){	break;	}
			i++;
		}
		pstPtrn[i].cchMozi   = *ptPattern;
		pstPtrn[i].iDistance = --dLength;

		ptPattern++;
	}

	return pstPtrn;
}
//-------------------------------------------------------------------------------------------------

/*!
	BM��������
	@param[in]	ptText		�����Ώە�����
	@param[in]	ptPattern	�����p�^�[��
	@param[out]	pdCch		�q�b�g���������ʒu���E�Ȃ�������0
	@return		LPTSTR		�q�b�g���������̊J�n�E�Ȃ�������NULL
*/
LPTSTR FindStringProc( LPTSTR ptText, LPTSTR ptPattern, LPINT pdCch )
{
	UINT_PTR	dPtrnLen, dLength;
	LPTSTR	ptTextEnd;
	INT		i, j, k, jump, cch;

	LPFINDPATTERN	pstPattern;

	StringCchLength( ptText, STRSAFE_MAX_CCH, &dLength );

	StringCchLength( ptPattern, STRSAFE_MAX_CCH, &dPtrnLen );
	dPtrnLen--;

	ptTextEnd = ptText + dLength - dPtrnLen;

	pstPattern = FindTableMake( ptPattern );

	cch = 0;
	while( ptText < ptTextEnd )
	{
		for( i = dPtrnLen ; i >= 0 ; i-- )
		{
			if( ptText[i] != ptPattern[i] ){	break;	}
		}

		//	�S�Ĉ�v����
		if( i < 0 ){	FREE( pstPattern  );	*pdCch = cch;	return( ptText );	}

		//	�e�[�u������ړ��ʂ����߂�(�����Ȃ�ړ��ʂ͂Q)
		k = 0;
		while( pstPattern[k].cchMozi )
		{
			if( pstPattern[k].cchMozi == ptText[i] ){	break;	}
			k++;
		}
		j = pstPattern[k].iDistance - ( dPtrnLen - i );
		jump = ( 0 < j ) ? j : 2;
		ptText += jump;
		cch += jump;
	}

	FREE( pstPattern );

	*pdCch = 0;

	return( NULL );
}
//-------------------------------------------------------------------------------------------------

/*!
	������\�����Ȃ��`�F�L�{�b�N�X�t���_�C�����O�{�b�N�X�v���V�[�W��
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK MsgCheckBoxProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	static LPMSGBOXMSG	pcstMsg = NULL;
	INT	id;

	switch( message )
	{
		default:	break;

		case WM_INITDIALOG:
			pcstMsg = (LPMSGBOXMSG)lParam;
			SetDlgItemText( hDlg, IDS_MC_MSG1, pcstMsg->atMsg1 );
			SetDlgItemText( hDlg, IDS_MC_MSG2, pcstMsg->atMsg2 );
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			id = LOWORD(wParam);
			if( pcstMsg ){	pcstMsg->bChecked = IsDlgButtonChecked( hDlg, IDCB_MC_CHECKBOX );	}
			if( IDYES == id || IDOK == id ){	EndDialog( hDlg, IDYES );	}
			if( IDNO == id || IDCANCEL == id ){	EndDialog( hDlg, IDNO );	}
			return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�`�F�b�N�{�b�N�X�t�����b�Z�[�W�{�b�N�X���ǂ�
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	hInst	�A�v���̎���
	@param[in]	dStyle	�\��Message�@�O�œ����@�P�ŕ����@�Q�ō폜
*/
INT_PTR MessageBoxCheckBox( HWND hWnd, HINSTANCE hInst, UINT dStyle )
{
	INT_PTR	iRslt;
	UINT	number;
	MSGBOXMSG	stMsg;


	switch( dStyle )
	{
		case  0:	//	�����m�F
			StringCchCopy( stMsg.atMsg1, MAX_PATH, TEXT("��������������畜�A�ł��Ȃ���") );
			StringCchCopy( stMsg.atMsg2, MAX_PATH, TEXT("�{���ɂ������Ă����H") );
			number = VL_PCOMBINE_NM;
			break;

		case  1:	//	�����m�F
			StringCchCopy( stMsg.atMsg1, MAX_PATH, TEXT("��������������畜�A�ł��Ȃ���") );
			StringCchCopy( stMsg.atMsg2, MAX_PATH, TEXT("�{���Ƀo�����Ă����H") );
			number = VL_PDIVIDE_NM;
			break;

		case  2:	//	�폜�m�F
			StringCchCopy( stMsg.atMsg1, MAX_PATH, TEXT("�폜����������畜�A�ł��Ȃ���") );
			StringCchCopy( stMsg.atMsg2, MAX_PATH, TEXT("�{���ɂ��ځ[�񂵂Ă����H") );
			number = VL_PDELETE_NM;
			break;

		default:	return IDCANCEL;
	}

	stMsg.bChecked = InitParamValue( INIT_LOAD, number, 0 );
	if( 1 == stMsg.bChecked ){	iRslt = IDYES;	}
	else
	{
		iRslt = DialogBoxParam( hInst, MAKEINTRESOURCE(IDD_MSGCHECKBOX_DLG), hWnd, MsgCheckBoxProc, (LPARAM)(&stMsg) );
		InitParamValue( INIT_SAVE, number, (BST_CHECKED==stMsg.bChecked) ? 1 : 0 );
	}
	return iRslt;
}
//-------------------------------------------------------------------------------------------------

#ifdef DO_TRY_CATCH
/*!
	��O�������
*/
LRESULT ExceptionMessage( LPCSTR pcExpMsg, LPCSTR pcFuncName, UINT dLine, LPARAM lReturn )
{
	CHAR	acMessage[BIG_STRING];

	StringCchPrintfA( acMessage, BIG_STRING, ("�ُ픭����%s��[%s:%u]\r\n�v���O�����𑱍s�ł��܂���B"), pcExpMsg, pcFuncName, dLine );

	MessageBoxA( GetDesktopWindow(), acMessage, ("�v���I�G���[����"), MB_OK );

	return lReturn;
}
//-------------------------------------------------------------------------------------------------

#endif

