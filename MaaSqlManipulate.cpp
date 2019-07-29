/*! @file
	@brief SQLite�̑�������܂��B
	���̃t�@�C���� MaaSqlManipulate.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/06/22
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

#include "MaaTemplate.h"
//-------------------------------------------------------------------------------------------------



//	�e�[�{�[�̃o�[�W����
#define TREE_TBL_VER	1500	//	�v���E�t�@�C��������
#define SUBTAB_NAME_VER	2000	//	���^�u�̕\�����̕ύX�ɑΉ�	20120613
//-------------------------------------------------------------------------------------------------

//	�e�[�u������
CONST CHAR	cacDetectTable[] = { ("SELECT name FROM sqlite_master WHERE type='table'") };

//	�o�[�W�����e�[�u��
CONST CHAR	cacVersionTable[]  = { ("CREATE TABLE BuildVer ( id INTEGER PRIMARY KEY, number INTEGER NOT NULL, vertext TEXT NOT NULL)") };
CONST CHAR	cacVerStrInsFmt[]  = { ("INSERT INTO BuildVer ( number, vertext ) VALUES ( %d, '%s' )") };
CONST CHAR	cacVersionNumGet[] = { ("SELECT number FROM BuildVer WHERE id == 1") };
CONST CHAR	cacVersionUpdate[] = { ("UPDATE BuildVer SET number = %d, vertext = '%s' WHERE id == 1") };


static sqlite3	*gpDataBase;	//	�g�����`�`�X�g�A�{�c���[�̃L���b�V��

static sqlite3	*gpTreeCache;	//	�c���[�ҏW�̃I���������L���b�V��

//-------------------------------------------------------------------------------------------------

HRESULT	SqlFavUpdate( UINT );	//!<	
HRESULT	SqlFavInsert( LPTSTR, DWORD, LPSTR, UINT );	//!<	
//-------------------------------------------------------------------------------------------------

//	WORK_LOG_OUT
//#ifdef _DEBUG
/*!
	SQLite�̃G���[�����o�n��e���{�e�ɕ\��
	@param[in]	*psqTarget	�G���[�`�F�L�������f�^�x�X
	@param[in]	dline		�Ăяo�����n�_�̍s��
*/
VOID SqlErrMsgView( sqlite3 *psqTarget, DWORD dline )
{
	//TCHAR	atStr[MAX_PATH];	//	����ȃo�b�t�@�e�ʂő��v��
	//StringCchPrintf( atStr, MAX_PATH, TEXT("%s[%u]\r\n"), sqlite3_errmsg16( psqTarget ), dline );
	//OutputDebugString( atStr );
	TRACE( TEXT("%s[%u]"), sqlite3_errmsg16( psqTarget ), dline );
	return;
}
//-------------------------------------------------------------------------------------------------
//#endif

/*!
	��������NULL�w�肾������u�b��Ԃ̂ő΍�
	@param[in]	ptDest	�R�s�[�敶����|�C���^
	@param[in]	cchSize	�R�s�[�敶����o�b�t�@�̕�����
	@param[in]	ptSrc	��������
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT String_Cch_Copy( LPTSTR ptDest, size_t cchSize, LPCTSTR ptSrc )
{
	ZeroMemory( ptDest, cchSize * sizeof(TCHAR) );

	if( !(ptSrc) )	return S_FALSE;

	return StringCchCopy( ptDest, cchSize , ptSrc );
}
//-------------------------------------------------------------------------------------------------



/*!
	SQLite�̎g�����`�`�̃f�[�^�x�[�X�������J������
	@param[in]	mode	�J���̂�����̂�
	@param[in]	ptDbase	�J���f�[�^�x�[�X�t�@�C���̖��O
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT SqlDatabaseOpenClose( BYTE mode, LPCTSTR ptDbase )
{
	INT		rslt;
	TCHAR	atDbPath[MAX_PATH];

	if( M_CREATE == mode )
	{
		StringCchPrintf( atDbPath, MAX_PATH, ptDbase );

		rslt = sqlite3_open16( atDbPath, &gpDataBase );
		if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_FAIL;	}

		if( 0 == StrCmp( atDbPath, TEXT(":memory:") ) )
		{
			StringCchCopy( atDbPath, MAX_PATH, TEXT("memory.qor") );
		}

		PathStripPath( atDbPath );	//	�t�@�C���������ɂ���
		PathRemoveExtension( atDbPath );	//	�g���q���O��

		SqlTreeTableCreate( atDbPath );	//	�v���t�@�C�����Ƃ��ēn��
	}
	else if( M_DESTROY == mode )
	{
		if( gpDataBase )
		{
			rslt =  sqlite3_close( gpDataBase );
			gpDataBase = NULL;
		}
	}
	else	//	�I�[�|�����Ă邩�ǂ����m�F
	{
		if( !(gpDataBase) ){	return E_OUTOFMEMORY;	}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�o�^����Ă������ID�ő�l�m�ہE�f�B���N�g�������S�̂n�j
	@param[in]	ptFdrName	�Ώۃf�B���N�g�����E�m�t�k�k�Ȃ�S��
	@param[in]	pdMax		��m�t�k�k�Ȃ�A�ő�l���ꂿ�Ⴄ
	@return		�o�^����Ă����
*/
UINT SqlFavCount( LPCTSTR ptFdrName, PUINT pdMax )
{
	TCHAR	atQuery[MAX_STRING];
	UINT	rslt, iCount, iMax;
	sqlite3_stmt*	statement;

	if( ptFdrName )
	{
		StringCchPrintf( atQuery, MAX_STRING, TEXT("SELECT COUNT(id) FROM ArtList WHERE folder == '%s'"), ptFdrName );
	}
	else
	{
		StringCchCopy( atQuery, MAX_STRING, TEXT("SELECT COUNT(id) FROM ArtList") );
	}


	rslt = sqlite3_prepare16( gpDataBase, atQuery, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return 0;	}

	sqlite3_step( statement );
	iCount = sqlite3_column_int( statement, 0 );
	sqlite3_finalize(statement);

	if( !pdMax )	return iCount;


	if( !ptFdrName )
	{
		StringCchPrintf( atQuery, MAX_STRING, TEXT("SELECT MAX(id) FROM ArtList WHERE folder == '%s'"), ptFdrName );
	}
	else
	{
		StringCchCopy( atQuery, MAX_STRING, TEXT("SELECT MAX(id) FROM ArtList") );
	}

	rslt = sqlite3_prepare16( gpDataBase, atQuery, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return 0;	}

	sqlite3_step( statement );
	iMax = sqlite3_column_int( statement, 0 );
	sqlite3_finalize(statement);

	*pdMax = iMax;

	return iCount;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g����AA���A���X�g�m�F���Ēǉ�������X�V������
	@param[in]	ptBaseName	��_�f�B���N�g����
	@param[in]	dHash		�`�`�F���p�̃n�b�V���l
	@param[in]	pcConts		�`�`�{��
	@param[in]	rdLength	�o�C�g��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT SqlFavUpload( LPTSTR ptBaseName, DWORD dHash, LPSTR pcConts, UINT rdLength )
{
	CONST CHAR	acArtSelect[] = { ("SELECT id, folder FROM ArtList WHERE hash == ?") };
	INT		rslt;
	UINT	index, cntID, d;
	//LPCTSTR	ptFolder;
	TCHAR	atFolder[MAX_PATH];
	BOOLEAN	bIsExist;
	sqlite3_stmt	*statement;

	rslt = sqlite3_prepare( gpDataBase, acArtSelect, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_OUTOFMEMORY;	}

	bIsExist = FALSE;
	cntID = SqlFavCount( NULL, NULL );

//��ɓo�^������A���̂`�`�̃x�[�X�l�[�����ς�����ꍇ�A�V�������O�ŏo�Ă��Ȃ����
//�n�b�V���Ō��Ă�̂ŁA�`�`���͈̂�����o�^����Ȃ�
//���x�[�X���܂Ŋm�F����΂����B�q�b�g���Ȃ��Ȃ�܂ŉ񂵂āA�x�[�X�����Ȃ�������V�K�ǉ�
	sqlite3_reset( statement );
	sqlite3_bind_int( statement, 1, dHash );	//	hash

	for( d = 0; cntID >  d; d++ )	//	�S���񂹂΂���
	{
		rslt = sqlite3_step( statement );
		if( SQLITE_ROW == rslt )	//	���݂���
		{
			ZeroMemory( atFolder, sizeof(atFolder) );

			index = sqlite3_column_int( statement , 0 );	//	id
			String_Cch_Copy( atFolder, MAX_PATH, (LPCTSTR)sqlite3_column_text16( statement, 1 ) );	//	folder
			//ptFolder = (LPCTSTR)sqlite3_column_text16( statement, 1 );
			//if( ptFolder ){	StringCchCopy( atFolder, MAX_PATH, ptFolder );	}

			SqlFavUpdate( index );	//	���e�X�V

			//	�x�[�X���`�F�L�E�����̂�����΂���
			if( !( StrCmp( ptBaseName, atFolder ) ) ){	bIsExist = TRUE;	}
		}
		else{	break;	}	//	����ȏ�Ȃ��Ȃ�I���
	}

	sqlite3_finalize( statement );


	if( !(bIsExist) )	//	���L�^�̂Ȃ獀�ڒǉ�
	{
		SqlFavInsert( ptBaseName, dHash, pcConts, rdLength );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g����AA�̋L�^���X�V
	@param[in]	index	�g����AA��ID�ԍ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT SqlFavUpdate( UINT index )
{
	CONST CHAR	acArtSelCount[] = { ("SELECT count FROM ArtList WHERE id == %u") };
	CONST CHAR	acArtUpdate[] = { ("UPDATE ArtList SET count = %u, lastuse = %f WHERE id == %u") };

	CHAR	acQuery[MAX_STRING];
	UINT	iCount, rslt;
	DOUBLE	ddJulius;
	sqlite3_stmt	*statement;

	StringCchPrintfA( acQuery, MAX_STRING, acArtSelCount, index );
	//	�܂��J�E���g�l�擾����
	rslt = sqlite3_prepare( gpDataBase, acQuery, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_OUTOFMEMORY;	}
	rslt = sqlite3_step( statement );
	iCount = sqlite3_column_int( statement , 0 );	//	count
	sqlite3_finalize( statement );

	iCount++;	//	�g�p�񐔂ł���E���₷

	//	���̃����E�X���Ԃ��Q�b�g
	rslt = sqlite3_prepare( gpDataBase, ("SELECT julianday('now')"), -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_OUTOFMEMORY;	}
	sqlite3_step( statement );
	ddJulius = sqlite3_column_double( statement, 0 );
	sqlite3_finalize( statement );

	StringCchPrintfA( acQuery, MAX_STRING, acArtUpdate, iCount, ddJulius, index );
	rslt = sqlite3_prepare( gpDataBase, acQuery, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_OUTOFMEMORY;	}
	rslt = sqlite3_step( statement );
	if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpDataBase );	}

	sqlite3_finalize(statement);

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g����AA���L�^
	@param[in]	ptBaseName	��_�f�B���N�g����
	@param[in]	dHash		�`�`�F���p�̃n�b�V���l
	@param[in]	pcConts		�`�`�{��
	@param[in]	rdLength	�o�C�g��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT SqlFavInsert( LPTSTR ptBaseName, DWORD dHash, LPSTR pcConts, UINT rdLength )
{
	CONST CHAR	acArtInsert[] = { ("INSERT INTO ArtList ( count, folder, lastuse, hash, conts ) VALUES ( ?, ?, ?, ?, ? )") };

	INT		rslt;
	CHAR	acText[MAX_PATH];
	DOUBLE	ddJulius;
	sqlite3_stmt	*statement;

	ZeroMemory( acText, sizeof(acText) );

	//	���̃����E�X���Ԃ��Q�b�g
	rslt = sqlite3_prepare( gpDataBase, ("SELECT julianday('now')"), -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_OUTOFMEMORY;	}
	sqlite3_step( statement );
	ddJulius = sqlite3_column_double( statement, 0 );
	sqlite3_finalize( statement );


	rslt = sqlite3_prepare( gpDataBase, acArtInsert, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_OUTOFMEMORY;	}
	sqlite3_reset( statement );
	rslt = sqlite3_bind_int(    statement, 1, 1 );			//	count
	rslt = sqlite3_bind_text16( statement, 2, ptBaseName, -1, SQLITE_STATIC );	//	folder
	rslt = sqlite3_bind_double( statement, 3, ddJulius );	//	lastuse
	rslt = sqlite3_bind_int(    statement, 4, dHash );		//	hash
	rslt = sqlite3_bind_blob(   statement, 5, pcConts, rdLength, SQLITE_STATIC );	//	conts

	rslt = sqlite3_step( statement );
	if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpDataBase );	}
	sqlite3_finalize(statement);

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�o�^����Ă��郋�[�g�������X�g�A�b�v���ăR�[���o�b�N����
	@param[in]	pfFolderNameSet	���e���R�[���o�b�N���锟���|�C���^�[
	@return		HRESULT			�I����ԃR�[�h
*/
HRESULT SqlFavFolderEnum( BUFFERBACK pfFolderNameSet )
{
	CONST CHAR	acQuery[] = { ("SELECT DISTINCT folder FROM ArtList") };

	INT		rslt, count, i;
	sqlite3_stmt	*statement;


	if( !(pfFolderNameSet) ){	return E_INVALIDARG;	}
	if( !(gpDataBase) ){	return E_OUTOFMEMORY;	}

	count = SqlFavCount( NULL, NULL );

	rslt = sqlite3_prepare( gpDataBase, acQuery, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_FAIL;	}
	rslt = sqlite3_reset( statement );

	for( i = 0; count > i; i++ )
	{
		rslt = sqlite3_step( statement );
		if( SQLITE_ROW == rslt )
		{
			pfFolderNameSet( i, 0, 0, sqlite3_column_text16( statement, 0 ) );
		}
		else{	break;	}
	}

	sqlite3_finalize(statement);

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�L�^����Ă�AA���A�f�B���N�g�����ɏ]���ă��X�g�A�b�v���ăR�[���o�b�N����
	@param[in]	ptFdrName	�f�B���N�g�����ENULL�Ȃ�S��
	@param[in]	pfInflate	AA���R�[���o�b�N���锟���|�C���^�[
	@return		HRESULT			�I����ԃR�[�h
*/
HRESULT SqlFavArtEnum( LPCTSTR ptFdrName, BUFFERBACK pfInflate )
{
	TCHAR	atQuery[MAX_STRING];
	UINT	rslt, iCount, i, szCont;
	sqlite3_stmt*	statement;

	if( ptFdrName )
	{
		StringCchPrintf( atQuery, MAX_STRING, TEXT("SELECT conts FROM ArtList WHERE folder == '%s' ORDER BY lastuse DESC"), ptFdrName );
	}
	else
	{
		StringCchCopy( atQuery, MAX_STRING, TEXT("SELECT conts FROM ArtList ORDER BY lastuse") );
	}

	rslt = sqlite3_prepare16( gpDataBase, atQuery, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_FAIL;	}
	rslt = sqlite3_reset( statement );

	iCount = SqlFavCount( ptFdrName, NULL );

	for( i = 0; iCount > i; i++ )
	{
		rslt = sqlite3_step( statement );
		if( SQLITE_ROW == rslt )
		{
			szCont = sqlite3_column_bytes( statement, 0 );
			pfInflate( szCont, 0, 0, sqlite3_column_blob( statement, 0 ) );
		}
		else{	break;	}
	}

	sqlite3_finalize(statement);

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C�ɓ��胊�X�g����폜����
	@param[in]	ptBaseName	��_�f�B���N�g�����E�g���ĂȂ�
	@param[in]	dHash		�`�`�F���p�̃n�b�V���l
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT SqlFavDelete( LPTSTR ptBaseName, DWORD dHash )
{
	CONST CHAR	acArtDelete[] = { ("DELETE FROM ArtList WHERE hash == ?") };
	INT		rslt;
	sqlite3_stmt	*statement;

	rslt = sqlite3_prepare( gpDataBase, acArtDelete, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_OUTOFMEMORY;	}
	sqlite3_reset( statement );
	sqlite3_bind_int( statement, 1, dHash );	//	hash
	rslt = sqlite3_step( statement );
	sqlite3_finalize( statement );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C�ɓ��胊�X�g����_���ƍ폜
	@param[in]	ptBaseName	��_�f�B���N�g����
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT SqlFavFolderDelete( LPTSTR ptBaseName )
{
	CONST CHAR	acFolderDelete[] = { ("DELETE FROM ArtList WHERE folder == ?") };
	INT		rslt;
	sqlite3_stmt	*statement;

	rslt = sqlite3_prepare( gpDataBase, acFolderDelete, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_OUTOFMEMORY;	}
	sqlite3_reset( statement );
	sqlite3_bind_text16( statement, 1, ptBaseName, -1, SQLITE_STATIC );	//	folder
	rslt = sqlite3_step( statement );
	sqlite3_finalize( statement );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------




#if 0
/*!
	SQLite�̎g�����`�`�̃f�[�^�x�[�X�������J������
	@param[in]	mode		�J���̂�����̂�
	@param[in]	ptDbName	�J���f�[�^�x�[�X�t�@�C�����O
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT SqlTreeOpenClose( BYTE mode, LPCTSTR ptDbName )
{
	INT		rslt;
	TCHAR	atDbPath[MAX_PATH];

	if( M_CREATE == mode )
	{
		StringCchPrintf( atDbPath, MAX_PATH, ptDbName );

		rslt = sqlite3_open16( atDbPath, &gpTreeCache );
		if( SQLITE_OK != rslt ){	SQL_DEBUG( gpTreeCache );	return E_FAIL;	}

		if( 0 == StrCmp( atDbPath, TEXT(":memory:") ) )
		{
			StringCchCopy( atDbPath, MAX_PATH, TEXT("memory.qor") );
		}

		PathStripPath( atDbPath );	//	�t�@�C���������ɂ���
		PathRemoveExtension( atDbPath );	//	�g���q���O��

		SqlTreeTableCreate( atDbPath );	//	�v���t�@�C�����Ƃ��ēn��
	}
	else if( M_DESTROY == mode )
	{
		if( gpTreeCache )
		{
			rslt =  sqlite3_close( gpTreeCache );
			gpTreeCache = NULL;
		}
	}
	else	//	�I�[�|�����Ă邩�ǂ����m�F
	{
		if( !(gpTreeCache) ){	return E_OUTOFMEMORY;	}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------
#endif

/*!
	SQLite��Transaction���������߂���
	@param[in]	mode	��O�J�n�@�O�I��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT SqlTransactionOnOff( BYTE mode )
{
	if( mode )
	{
		sqlite3_exec( gpDataBase, "BEGIN TRANSACTION", NULL, NULL, NULL );	//	�g�����U�N�V�����J�n
	}
	else
	{
		sqlite3_exec( gpDataBase, "COMMIT TRANSACTION", NULL, NULL, NULL );	//	�g�����U�N�V�����I��
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�c���[�̃e�[�u���̑��݂��m�F���āA�Ȃ�������쐬
	@param[in]	ptProfName	�Ȃɂ�
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT SqlTreeTableCreate( LPTSTR ptProfName )
{
	//	�v���t���
	CONST CHAR	cacProfilesTable[] = { ("CREATE TABLE Profiles ( id INTEGER PRIMARY KEY, profname TEXT NOT NULL, rootpath TEXT NULL)") };
	//	�����v���t���쐬
	CONST TCHAR	catProfInsFmt[] = { TEXT("INSERT INTO Profiles ( profname ) VALUES( '%s' )") };
	//	�c���[���
	CONST CHAR	cacTreeNodeTable[] = { ("CREATE TABLE TreeNode ( id INTEGER PRIMARY KEY, type INTEGER NOT NULL, parentid INTEGER NOT NULL, nodename TEXT NOT NULL )") };
	//	���^�u���
	CONST CHAR	cacMultiTabTable[] = { ("CREATE TABLE MultiTab ( id INTEGER PRIMARY KEY, filepath TEXT NOT NULL, basename TEXT NOT NULL, dispname TEXT )") };	//	20120613	�\�����̒ǉ�
	//	�g�p�`�`���
	CONST CHAR	cacArtListTable[]  = { ("CREATE TABLE ArtList ( id INTEGER PRIMARY KEY, count INTEGER NOT NULL, folder TEXT NOT NULL, lastuse  REAL NOT NULL, hash INTEGER NOT NULL, conts BLOB NOT NULL )") };

	CHAR	acBuildVer[SUB_STRING], acText[MAX_PATH];
	TCHAR	atStr[MAX_STRING];
	SYSTEMTIME	stSysTime;

	BYTE	yMode = FALSE;	//	Table�������Ƃ��Ȃ������Ƃ�
	INT		rslt;

	UINT	dVersion;
	CHAR	acQuery[MAX_PATH];

#ifdef _DEBUG
	INT		i;
#endif
	sqlite3_stmt	*statement;

	ZeroMemory( acText, sizeof(acText) );

	//	�e�[�u���̑��݂��m�F����
	rslt = sqlite3_prepare( gpDataBase, cacDetectTable, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}

	rslt = sqlite3_step( statement );
	if( SQLITE_ROW == rslt )
	{
#ifdef _DEBUG
		for( i = 0; 10 > i; i++ )
		{
			TRACE( TEXT("[TREE] %s"), (LPCTSTR)sqlite3_column_text16( statement, 0 ) );
			rslt = sqlite3_step( statement );
			if( SQLITE_DONE == rslt ){	break;	}
		}
#endif
		rslt = sqlite3_finalize( statement );
		yMode = TRUE;
	}
	//	�܂��A�ā[�Ԃ�傪�L�邩�������ɂ���

	if( !(yMode) )
	{

		TRACE( TEXT("�c���[�p�e�[�u����������Ȃ�����") );

		rslt = sqlite3_finalize( statement );	//	�ǂ����ɂ��Ă�����


	//VERSION�ԍ��e�[�u���쐬���\�ɂ͏o�Ȃ�
		rslt = sqlite3_prepare( gpDataBase, cacVersionTable, -1, &statement, NULL );
		if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}
		rslt = sqlite3_step( statement );	//	���s
		if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}
		rslt = sqlite3_finalize(statement);

		//VERSION���e���쐬	20120613�ύX
		GetLocalTime( &stSysTime );
		StringCchPrintfA( acBuildVer, SUB_STRING, ("%d.%02d%02d.%02d%02d.%d"), stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay, stSysTime.wHour, stSysTime.wMinute, SUBTAB_NAME_VER );
		StringCchPrintfA( acText, MAX_PATH, cacVerStrInsFmt, SUBTAB_NAME_VER, acBuildVer );
		//	�����f�[�^�Ԃ�����
		rslt = sqlite3_prepare( gpDataBase, acText, -1, &statement, NULL );
		if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}
		sqlite3_reset( statement );
		rslt = sqlite3_step( statement );
		sqlite3_finalize(statement);


	//�v���t�e�[�u�����쐬
		rslt = sqlite3_prepare( gpDataBase, cacProfilesTable, -1, &statement, NULL );
		if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}
		rslt = sqlite3_step( statement );	//	���s
		if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}
		rslt = sqlite3_finalize(statement);

		//�����l���쐬
		ZeroMemory( atStr, sizeof(atStr) );
		StringCchPrintf( atStr, MAX_STRING, catProfInsFmt, ptProfName );
		rslt = sqlite3_prepare16( gpDataBase, atStr, -1, &statement, NULL );
		if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}
		sqlite3_reset( statement );
		rslt = sqlite3_step( statement );
		sqlite3_finalize(statement);

	//�c���[�e�[�u���𐶐�
		rslt = sqlite3_prepare( gpDataBase, cacTreeNodeTable, -1, &statement, NULL );
		if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}
		rslt = sqlite3_step( statement );	//	���s
		if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}
		rslt = sqlite3_finalize(statement);

	//���^�u�e�[�u�����쐬
		rslt = sqlite3_prepare( gpDataBase, cacMultiTabTable, -1, &statement, NULL );
		if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}
		rslt = sqlite3_step( statement );	//	���s
		if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}
		rslt = sqlite3_finalize(statement);

	//�g�p�`�`�e�[�u�����쐬	
		rslt = sqlite3_prepare( gpDataBase, cacArtListTable, -1, &statement, NULL );
		if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}
		rslt = sqlite3_step( statement );	//	���s
		if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}
		rslt = sqlite3_finalize(statement);

	}
	else	//	���݂��Ă�Ȃ�A���@�[�W�������m���߂ā[�e�[�{�[��ύX
	{
		dVersion = 0;
		rslt = sqlite3_prepare( gpDataBase, cacVersionNumGet, -1, &statement, NULL );
		if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}
		rslt = sqlite3_step( statement );
		if( SQLITE_ROW == rslt ){	dVersion = sqlite3_column_int( statement, 0 );	}
		sqlite3_finalize( statement );
		TRACE( TEXT("PROFILE VERSION[%d]"), dVersion );



		//	���^�u�̕\�����̂�ǉ�
		if( TREE_TBL_VER == dVersion )
		{
			//	�e�[�u���ɒǉ�
			rslt = sqlite3_prepare( gpDataBase, ("ALTER TABLE MultiTab ADD COLUMN dispname TEXT DEFAULT \"\" "), -1, &statement, NULL );
			if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}
			rslt = sqlite3_step( statement );	//	���s
			if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}
			rslt = sqlite3_finalize( statement );

			//�o�[�W����������������
			GetLocalTime( &stSysTime  );	//	�X�V�����ƃo�[�W����
			StringCchPrintfA( acBuildVer, SUB_STRING, ("%d.%02d%02d.%02d%02d.%d"),
				stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay,
				stSysTime.wHour, stSysTime.wMinute, SUBTAB_NAME_VER );

			//	Query����������
			StringCchPrintfA( acQuery, MAX_PATH, cacVersionUpdate, SUBTAB_NAME_VER, acBuildVer );

			//	��񏑂�������
			rslt = sqlite3_prepare( gpDataBase, acQuery, -1, &statement, NULL);
			if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_ACCESSDENIED;	}

			sqlite3_reset( statement );
			rslt = sqlite3_step( statement );
			sqlite3_finalize( statement );

			dVersion = SUBTAB_NAME_VER;
		}
	}


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�o�^����Ă������ID�ő�l�m��
	@param[in]	bType	�O�v���t�E�P�c���[�E�Q���^�u�E�R�c���[�L���b�V��
	@param[out]	pdMax	��m�t�k�k�Ȃ�A�ő�l���ꂿ�Ⴄ
	@return		�o�^����Ă����
*/
UINT SqlTreeCount( UINT bType, PUINT pdMax )
{
	CHAR	acQuery[MAX_STRING];
	UINT	rslt, iCount, iMax;
	sqlite3_stmt*	statement;

	sqlite3	*pDB;

	switch( bType )
	{
		case  0:	pDB = gpDataBase;	StringCchCopyA( acQuery, MAX_STRING, ("SELECT COUNT(id) FROM Profiles") );	break;
		case  1:	pDB = gpDataBase;	StringCchCopyA( acQuery, MAX_STRING, ("SELECT COUNT(id) FROM TreeNode") );	break;
		case  2:	pDB = gpDataBase;	StringCchCopyA( acQuery, MAX_STRING, ("SELECT COUNT(id) FROM MultiTab") );	break;
		case  3:	pDB = gpTreeCache;	StringCchCopyA( acQuery, MAX_STRING, ("SELECT COUNT(id) FROM TreeNode") );	break;
		default:	return 0;
	}


	rslt = sqlite3_prepare( pDB, acQuery, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( pDB );	return 0;	}

	sqlite3_step( statement );
	iCount = sqlite3_column_int( statement, 0 );
	sqlite3_finalize(statement);

	if( !pdMax )	return iCount;

	switch( bType )
	{
		case  0:	StringCchCopyA( acQuery, MAX_STRING, ("SELECT MAX(id) FROM Profiles") );	break;
		case  1:	StringCchCopyA( acQuery, MAX_STRING, ("SELECT MAX(id) FROM TreeNode") );	break;
		case  2:	StringCchCopyA( acQuery, MAX_STRING, ("SELECT MAX(id) FROM MultiTab") );	break;
		case  3:	StringCchCopyA( acQuery, MAX_STRING, ("SELECT MAX(id) FROM TreeNode") );	break;
		default:	return 0;
	}

	rslt = sqlite3_prepare( pDB, acQuery, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( pDB );	return 0;	}

	sqlite3_step( statement );
	iMax = sqlite3_column_int( statement, 0 );
	sqlite3_finalize( statement );

	*pdMax = iMax;

	return iCount;
}
//-------------------------------------------------------------------------------------------------

/*!
	�v���t���A���[�g�p�X��ύX
	@param[in]	ptProfName	�v���t��
	@param[in]	ptRootPath	AA�̃��[�g�_�f�B���N�g����
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT SqlTreeProfUpdate( LPCTSTR ptProfName, LPCTSTR ptRootPath )
{
	//	�P�t�@�C���ɕt���P���R�[�h�������݂��Ȃ��̂ŁAID�͂P�Œ�
	CONST  TCHAR	catUpdateName[] = { TEXT("UPDATE Profiles SET profname = '%s' WHERE id == 1") };
	CONST  TCHAR	catUpdateRoot[] = { TEXT("UPDATE Profiles SET rootpath = '%s' WHERE id == 1") };

	TCHAR	atText[MAX_PATH];
	INT		rslt;
	sqlite3_stmt	*statement;

	//	�g�����U�N�V������ON/OFF�͕K�v�ɉ����ĊO�Ń���

	if( ptProfName )
	{
		StringCchPrintf( atText, MAX_PATH, catUpdateName, ptProfName );

		rslt = sqlite3_prepare16( gpDataBase, atText, -1, &statement, NULL );
		if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_OUTOFMEMORY;	}
		rslt = sqlite3_step( statement );	if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpDataBase );	}
		sqlite3_finalize( statement );
	}

	if( ptRootPath )
	{
		StringCchPrintf( atText, MAX_PATH, catUpdateRoot, ptRootPath );

		rslt = sqlite3_prepare16( gpDataBase, atText, -1, &statement, NULL );
		if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_OUTOFMEMORY;	}
		rslt = sqlite3_step( statement );	if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpDataBase );	}
		sqlite3_finalize( statement );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�v���t���A���[�g�p�X���m��
	@param[out]	ptProfName	�v���t�������
	@param[in]	szName		�v���t���̕�����
	@param[out]	ptRootPath	AA�̃��[�g�_�f�B���N�g����
	@param[in]	szRoot		���[�g�_�f�B�̕�����
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT SqlTreeProfSelect( LPTSTR ptProfName, UINT szName, LPTSTR ptRootPath, UINT szRoot )
{
	CONST CHAR	cacSelectQuery[] = { ("SELECT * FROM Profiles WHERE id == 1") };

	//LPTSTR	ptBuf;
	TCHAR	atName[MAX_STRING], atRoot[MAX_PATH];
	INT		index;
	INT		rslt;
	sqlite3_stmt	*statement;

	ZeroMemory( atName, sizeof(atName) );
	ZeroMemory( atRoot, sizeof(atRoot) );

	//	�N�F���Z�b�g
	rslt = sqlite3_prepare( gpDataBase, cacSelectQuery, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_OUTOFMEMORY;	}

	rslt = sqlite3_reset( statement );

	//	���e��ǂݏo��
	rslt = sqlite3_step( statement );
	if( SQLITE_ROW == rslt )
	{
		index = sqlite3_column_int( statement , 0 );	//	id
		String_Cch_Copy( atName , MAX_STRING, (LPTSTR)sqlite3_column_text16( statement, 1 ) );	//	profname
		String_Cch_Copy( atRoot , MAX_STRING, (LPTSTR)sqlite3_column_text16( statement, 2 ) );	//	rootpath
		//ptBuf = (LPTSTR)sqlite3_column_text16( statement, 2 );	//	rootpath
		//if( ptBuf ){	StringCchCopy( atRoot , MAX_PATH, ptBuf );	}
	}
	sqlite3_finalize( statement );

	if( ptProfName )	StringCchCopy( ptProfName, szName, atName );
	if( ptRootPath )	StringCchCopy( ptRootPath, szRoot, atRoot );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


/*!
	�f�B���N�g�����t�@�C���̃f�[�^���h�s���b
	@param[in]	uqID	�����ς̂h�c�ԍ�
	@param[in]	dType	�f�B���N�g�����t�@�C����
	@param[in]	dPrnt	�e�c���[�m�[�h��SQL�I�h�c�ԍ�
	@param[in]	ptName	�m�[�h�̖���
	@return	UINT	���ܓo�^����ID�ԍ�
*/
UINT SqlTreeNodeInsert( UINT uqID, UINT dType, UINT dPrnt, LPTSTR ptName )
{
	CONST CHAR	acTreeNodeInsert[] = { ("INSERT INTO TreeNode ( id, type, parentid, nodename ) VALUES ( ?, ?, ?, ? )") };
	CONST CHAR	acAddNumCheck[] = { ("SELECT LAST_INSERT_ROWID( ) FROM TreeNode") };

	INT		rslt;
	UINT	iRast = 0;
	sqlite3_stmt	*statement;
	

	if( !(gpDataBase) ){	TRACE( TEXT("NoDatabase") );	return 0;	}

	rslt = sqlite3_prepare( gpDataBase, acTreeNodeInsert, -1, &statement, NULL);
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return 0;	}

	sqlite3_reset( statement );
	rslt = sqlite3_bind_int(    statement, 1, uqID );	//	id
	rslt = sqlite3_bind_int(    statement, 2, dType );	//	type
	rslt = sqlite3_bind_int(    statement, 3, dPrnt );	//	parentid
	rslt = sqlite3_bind_text16( statement, 4, ptName, -1, SQLITE_STATIC );	//	nodename

	rslt = sqlite3_step( statement );
	if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpDataBase );	}

	sqlite3_finalize( statement );

	//	���ǉ�������̃A�����擾
	rslt = sqlite3_prepare( gpDataBase, acAddNumCheck, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return 0;	}

	rslt = sqlite3_step( statement );

	iRast = sqlite3_column_int( statement, 0 );

	sqlite3_finalize( statement );

	return iRast;
}
//-------------------------------------------------------------------------------------------------

#ifdef EXTRA_NODE_STYLE

/*!
	�G�L�X�g���t�@�C���̃f�[�^���h�s���b
	@param[in]	dType	�P�f�B���N�g�����O�t�@�C�����E�Ƃ肠��������
	@param[in]	ptName	�m�[�h�̖���
	@return	UINT	���ܓo�^����ID�ԍ�
*/
UINT SqlTreeNodeExtraInsert( UINT dType, LPCTSTR ptName )
{
	CONST CHAR	acTreeNodeExIns[] = { ("INSERT INTO TreeNode ( type, parentid, nodename ) VALUES ( ?, ?, ? )") };
	CONST CHAR	acAddNumCheck[] = { ("SELECT LAST_INSERT_ROWID( ) FROM TreeNode") };

	INT		rslt;
	UINT	iRast = 0;
	sqlite3_stmt	*statement;

	if( !(gpDataBase) ){	TRACE( TEXT("NoDatabase") );	return 0;	}

	rslt = sqlite3_prepare( gpDataBase, acTreeNodeExIns, -1, &statement, NULL);
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return 0;	}

	//	�t�@�C���Œ�A�e�h�c�́|�P�Œ�
	sqlite3_reset( statement );
	rslt = sqlite3_bind_int(    statement, 1, 0 );	//	type
	rslt = sqlite3_bind_int(    statement, 2, -1 );	//	parentid
	rslt = sqlite3_bind_text16( statement, 3, ptName, -1, SQLITE_STATIC );	//	nodename

	rslt = sqlite3_step( statement );
	if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpDataBase );	}

	sqlite3_finalize( statement );

	//	���ǉ�������̃A�����擾�E�h�c�͂��ł����j�[�N�ł���
	rslt = sqlite3_prepare( gpDataBase, acAddNumCheck, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return 0;	}

	rslt = sqlite3_step( statement );

	iRast = sqlite3_column_int( statement, 0 );

	sqlite3_finalize( statement );

	return iRast;
}
//-------------------------------------------------------------------------------------------------

/*!
	�h�c�w�肵�č폜
	@param[in]	delID	���̂h�c�̃f�[�^���폜����
	@return	UINT	���ɂȂ�
*/
UINT SqlTreeNodeExtraDelete( UINT delID )
{
	CONST CHAR	cacDelete[] = { ("DELETE FROM TreeNode WHERE id == ?") };

	INT		rslt;
	sqlite3_stmt	*statement;

	if( !(gpDataBase) ){	TRACE( TEXT("NoDatabase") );	return 0;	}

	rslt = sqlite3_prepare( gpDataBase, cacDelete, -1, &statement, NULL);
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return 0;	}

	sqlite3_reset( statement );
	rslt = sqlite3_bind_int( statement, 1 , delID );	//	type

	rslt = sqlite3_step( statement );
	if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpDataBase );	}

	sqlite3_finalize( statement );

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�h�c�w�肵�ă��X�g�A�b�v
	@param[in]	seekID	�P�ȏ�̏ꍇ�A����ID�̂�T���B�������D��
	@param[in]	tgtID	���̔ԍ��𒴂��čŏ��Ƀq�b�g�������Ԃ��EseekID���O�̏ꍇ
	@param[out]	ptName	�m�[�h�̃p�X�����o�b�t�@�EMAX_PATH�ł��邱��
	@return	UINT	������������̂h�c�E����������O
*/
UINT SqlTreeNodeExtraSelect( UINT seekID, UINT tgtID, LPTSTR ptName )
{
	//LPCTSTR	ptBuffer;
	CHAR	acQuery[MAX_STRING];
	INT		rslt;
	UINT	id = 0;
	sqlite3_stmt*	statement;

	if( !(gpDataBase) ){	TRACE( TEXT("NoDatabase") );	return 0;	}

	if( 1 <= seekID ){	StringCchPrintfA( acQuery, MAX_STRING, ("SELECT id, nodename FROM TreeNode WHERE id == %u"), seekID );	}
	else{	StringCchPrintfA( acQuery, MAX_STRING, ("SELECT id, nodename FROM TreeNode WHERE id > %u AND parentid == -1 ORDER BY id ASC"), tgtID );	}

	rslt = sqlite3_prepare( gpDataBase, acQuery, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return 0;	}
	rslt = sqlite3_step( statement );
	if( SQLITE_ROW == rslt )
	{
		id = sqlite3_column_int( statement, 0 );	//	id
		String_Cch_Copy( ptName, MAX_PATH, (LPCTSTR)sqlite3_column_text16( statement, 1 ) );	//	nodename
		//ptBuffer = (LPCTSTR)sqlite3_column_text16( statement, 1 );	//	nodename
		//if( ptBuffer  ){	StringCchCopy( ptName, MAX_PATH, ptBuffer );	}
	}

	sqlite3_finalize( statement );

	return id;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C�����w�肵�āA�����̂����邩�ǂ���
	@param[out]	ptName	�m�[�h�̃p�X�����o�b�t�@�EMAX_PATH�ł��邱��
	@return	UINT	�q�b�g������̂h�c�E����������O
*/
UINT SqlTreeNodeExtraIsFileExist( LPCTSTR ptName ) 
{
	CONST CHAR	cacNameSearch[] = { ("SELECT id FROM TreeNode WHERE nodename == ?") };

	INT		rslt;
	UINT	id = 0;
	sqlite3_stmt*	statement;

	rslt = sqlite3_prepare( gpDataBase, cacNameSearch, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return 0;	}

	sqlite3_reset( statement );
	rslt = sqlite3_bind_text16( statement, 1, ptName, -1, SQLITE_STATIC );	//	nodename

	rslt = sqlite3_step( statement );
	if( SQLITE_ROW == rslt )
	{
		id = sqlite3_column_int( statement, 0 );	//	id
	}

	sqlite3_finalize( statement );

	return id;
}
//-------------------------------------------------------------------------------------------------


#endif


#if 0
/*!
	���ԂɑS�����X�g�A�b�v
	@param[in]	dProfID		���X�g�A�b�v����v���t�h�c
	@param[in]	pfDataing	�f�[�^�����锟���|�C���^
*/
HRESULT SqlTreeNodeEnum( UINT dProfID, BUFFERBACK pfDataing )
{
	CONST CHAR	acQuery[] = { ("SELECT * FROM TreeNode ORDER BY id ASC") };
	INT		rslt;
	UINT	i, cnt;
	UINT	id, type, prnt;
	TCHAR	atName[MAX_PATH];
	sqlite3_stmt*	statement;


	if( !(gpDataBase) ){	return E_OUTOFMEMORY;	}

	rslt = sqlite3_prepare( gpDataBase, acQuery, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_FAIL;	}

	cnt = SqlTreeCount( 1, NULL );

	for( i = 0; cnt > i; i++ )
	{
		rslt = sqlite3_step( statement );
		if( SQLITE_ROW == rslt )
		{
			id   = sqlite3_column_int( statement, 0 );	//	id
			type = sqlite3_column_int( statement, 1 );	//	type
			prnt = sqlite3_column_int( statement, 2 );	//	parentid
			String_Cch_Copy( atName, MAX_PATH, (LPCTSTR)sqlite3_column_text16( statement, 3 ) );	//	nodename

			pfDataing( id, type, prnt, atName );
		}
		else{	break;	}
	}

	sqlite3_finalize( statement );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------
#endif

#if 0
/*!
	�w��̖��O�̃f�B���N�g���̓��[�g�ɂ���̂ŁH
	@param[out]	ptDirName	�m�[�h�̖���
	@return	UINT			�O���������@�P�`�q�b�g�����h�c
*/
UINT SqlTreeNodeRootSearch( LPTSTR ptDirName )
{
	TCHAR	atQuery[BIG_STRING];
	INT		rslt;
	UINT	id = 0;
	sqlite3_stmt*	statement;


	if( !(gpDataBase) ){	return 0;	}

	StringCchPrintf( atQuery, BIG_STRING, TEXT("SELECT id FROM TreeNode WHERE parentid == 0 AND nodename == '%s'"), ptDirName );

	rslt = sqlite3_prepare16( gpDataBase, atQuery, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return 0;	}

	rslt = sqlite3_step( statement );
	if( SQLITE_ROW == rslt )
	{
		id = sqlite3_column_int( statement, 0 );	//	id
	}

	sqlite3_finalize( statement );

	return id;
}
//-------------------------------------------------------------------------------------------------
#endif

/*!
	�h�c�w�肵�ă��X�g�A�b�v
	@param[in]	tgtID	���̔ԍ��𒴂��čŏ��Ƀq�b�g�������Ԃ�
	@param[out]	pType	�f�B���N�g��(FILE_ATTRIBUTE_DIRECTORY)���t�@�C����(FILE_ATTRIBUTE_NORMAL)
	@param[out]	pPrntID	�e�c���[�m�[�h��SQL�I�h�c�ԍ�
	@param[out]	ptName	�m�[�h�̖���
	@param[in]	bStyle	0x01�ʏ�@0x00�c���[�L���b�V���@�^�@0x10�h�c��v�@0x00�h�c������
	@return	UINT	������������̂h�c�E����������O
*/
UINT SqlTreeNodePickUpID( UINT tgtID, PUINT pType, PUINT pPrntID, LPTSTR ptName, UINT bStyle ) 
{
	CHAR	acQuery[MAX_STRING];
	INT		rslt;
	UINT	id = 0;
	sqlite3_stmt*	statement;

	sqlite3	*pDB;

	if( bStyle & 0x01 ){	pDB = gpDataBase;	}
	else{					pDB = gpTreeCache;	}

	if( !(pDB) ){	return 0;	}

	if( bStyle & 0x10 ){	StringCchPrintfA( acQuery, MAX_STRING, ("SELECT * FROM TreeNode WHERE id == %u"), tgtID );	}
	else{	StringCchPrintfA( acQuery, MAX_STRING, ("SELECT * FROM TreeNode WHERE id > %u ORDER BY id ASC"), tgtID );	}

	rslt = sqlite3_prepare( pDB, acQuery, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( pDB );	return 0;	}
	sqlite3_reset( statement );
	rslt = sqlite3_step( statement );
	if( SQLITE_ROW == rslt )
	{
		id       = sqlite3_column_int( statement, 0 );	//	id
		*pType   = sqlite3_column_int( statement, 1 );	//	type
		*pPrntID = sqlite3_column_int( statement, 2 );	//	parentid
		String_Cch_Copy( ptName, MAX_PATH, (LPCTSTR)sqlite3_column_text16( statement, 3 ) );	//	nodename
	}

	sqlite3_finalize( statement );

	return id;
}
//-------------------------------------------------------------------------------------------------

/*!
	�e�h�c�w�肵�ă��X�g�A�b�v
	@param[in]	dPrntID	�e�h�c
	@param[in]	tgtID	���̔ԍ��𒴂��čŏ��Ƀq�b�g�������Ԃ�
	@param[out]	pType	�f�B���N�g��(FILE_ATTRIBUTE_DIRECTORY)���t�@�C����(FILE_ATTRIBUTE_NORMAL)
	@param[out]	ptName	�m�[�h�̖���
	@return	UINT	������������̂h�c�E����������O
*/
UINT SqlChildNodePickUpID( UINT dPrntID, UINT tgtID, PUINT pType, LPTSTR ptName )
{
	CHAR	acQuery[MAX_PATH];
	INT		rslt;
	UINT	id = 0, dummy;
	sqlite3_stmt*	statement;

	StringCchPrintfA( acQuery, MAX_PATH, ("SELECT * FROM TreeNode WHERE parentid == %u AND id > %u ORDER BY id ASC"), dPrntID, tgtID );

	rslt = sqlite3_prepare( gpDataBase, acQuery, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return 0;	}
	sqlite3_reset( statement );
	rslt = sqlite3_step( statement );
	if( SQLITE_ROW == rslt )
	{
		id     = sqlite3_column_int( statement, 0 );	//	id
		*pType = sqlite3_column_int( statement, 1 );	//	type
		dummy  = sqlite3_column_int( statement, 2 );	//	parentid
		String_Cch_Copy( ptName, MAX_PATH, (LPCTSTR)sqlite3_column_text16( statement, 3 ) );	//	nodename
	}

	sqlite3_finalize( statement );

	return id;
}
//-------------------------------------------------------------------------------------------------

/*!
	�c���[�f�[�^��S�č폜
	@param[in]	bStyle	��O�f�[�^�x�[�X�{�́@�O�v���t�\�z�p�L���b�V��
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT SqlTreeNodeAllDelete( UINT bStyle )
{
	CONST CHAR	acTreeDelete[] = { ("DELETE FROM TreeNode") };
	INT		rslt;
	sqlite3_stmt	*statement;

	sqlite3	*pDB;

	if( bStyle )	pDB = gpDataBase;
	else			pDB = gpTreeCache;


	rslt = sqlite3_prepare( pDB, acTreeDelete, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( pDB );	return E_OUTOFMEMORY;	}

	rslt = sqlite3_step( statement );
	sqlite3_finalize( statement );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�p���[�����󂯎���āA�Y������h�c��Ԃ��B
	@param[in]	ptName	�p���[��
	@param[in]	dStart	�����J�n�h�c�E���̂h�c�̎��̒l���猟���J�n
	@return	�q�b�g�����h�c�E����������O
*/
UINT SqlTreeFileSearch( LPTSTR ptName, UINT dStart )
{
	TCHAR	atReqest[SUB_STRING];
	INT		rslt;
	UINT	dxID;
	sqlite3_stmt*	statement;

	if( !(gpDataBase) ){	return 0;	}

	rslt = sqlite3_prepare( gpDataBase, ("SELECT id FROM TreeNode WHERE nodename LIKE ? AND id > ? ORDER BY id ASC"), -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return 0;	}

	sqlite3_reset( statement );

	StringCchPrintf( atReqest, SUB_STRING, TEXT("%%%s%%"), ptName );
	rslt = sqlite3_bind_text16( statement, 1, atReqest, -1, SQLITE_STATIC );	//	
	sqlite3_bind_int( statement, 2, dStart );

	rslt = sqlite3_step( statement );
	if( SQLITE_ROW == rslt ){	dxID = sqlite3_column_int( statement, 0 );	}
	else{	dxID = 0;	}

	sqlite3_finalize( statement );

	return dxID;
}
//-------------------------------------------------------------------------------------------------

/*!
	����̐e�h�c�����m�[�h���̂�T���āA�Y������h�c��Ԃ��B
	@param[in]	ptName	�p���[��
	@param[in]	dPrntID	����̐e�h�c
	@return	�q�b�g�����h�c�E����������O
*/
UINT SqlTreeFileGetOnParent( LPTSTR ptName, UINT dPrntID )
{
	INT		rslt;
	UINT	dxID;
	sqlite3_stmt*	statement;

	if( !(gpDataBase) ){	return 0;	}

	rslt = sqlite3_prepare( gpDataBase, ("SELECT id FROM TreeNode WHERE nodename == ? AND parentid == ?"), -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return 0;	}

	sqlite3_reset( statement );

	rslt = sqlite3_bind_text16( statement, 1, ptName, -1, SQLITE_STATIC );	//	
	sqlite3_bind_int( statement, 2, dPrntID );

	rslt = sqlite3_step( statement );
	if( SQLITE_ROW == rslt ){	dxID = sqlite3_column_int( statement, 0 );	}
	else{	dxID = 0;	}

	sqlite3_finalize( statement );

	return dxID;
}
//-------------------------------------------------------------------------------------------------




/*!
	�c���[�L���b�V���p�I���������c�a
*/
HRESULT SqlTreeCacheOpenClose( UINT bMode )
{
	//	�c���[���
	CONST CHAR	cacTreeNodeTable[] = { ("CREATE TABLE TreeNode ( id INTEGER PRIMARY KEY, type INTEGER NOT NULL, parentid INTEGER NOT NULL, nodename TEXT NOT NULL )") };
	INT		rslt;
	sqlite3_stmt	*statement;

	if( bMode )
	{
		rslt = sqlite3_open( (":memory:"), &gpTreeCache );
		if( SQLITE_OK != rslt ){	SQL_DEBUG( gpTreeCache );	return E_FAIL;	}

		//�c���[�e�[�u���𐶐�
		rslt = sqlite3_prepare( gpTreeCache, cacTreeNodeTable, -1, &statement, NULL );
		if( SQLITE_OK != rslt ){	SQL_DEBUG( gpTreeCache );	return E_ACCESSDENIED;	}
		rslt = sqlite3_step( statement );	//	���s
		if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpTreeCache );	return E_ACCESSDENIED;	}
		rslt = sqlite3_finalize(statement);

		sqlite3_exec( gpTreeCache, "BEGIN TRANSACTION", NULL, NULL, NULL );	//	�g�����U�N�V�����J�n
	}
	else
	{
		sqlite3_exec( gpTreeCache, "COMMIT TRANSACTION", NULL, NULL, NULL );	//	�g�����U�N�V�����I��

		if( gpTreeCache ){	rslt = sqlite3_close( gpTreeCache );	}
		gpTreeCache = NULL;
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�c���[Cache����AID�����ɍ폜
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT SqlTreeCacheDelID( INT tgtID )
{
	CONST CHAR	acTreeDel[] = { ("DELETE FROM TreeNode WHERE id == ? OR parentid == ?") };
	INT		rslt;
	sqlite3_stmt	*statement;

	//	�p�����g�h�c���Y�����郄�c���폜��


	rslt = sqlite3_prepare( gpTreeCache, acTreeDel, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpTreeCache );	return E_OUTOFMEMORY;	}

	sqlite3_reset( statement );
	sqlite3_bind_int( statement, 1, tgtID );
	sqlite3_bind_int( statement, 2, tgtID );
	sqlite3_step( statement );
	sqlite3_finalize( statement );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�f�B���N�g�����t�@�C���̃f�[�^�ꎞ�o�b�t�@�Ƀh�s���b
	@param[in]	dType	�f�B���N�g�����t�@�C����
	@param[in]	dPrnt	�e�c���[�m�[�h��SQL�I�h�c�ԍ�
	@param[in]	ptName	�m�[�h�̖���
	@return	UINT	���ܓo�^����ID�ԍ�
*/
UINT SqlTreeCacheInsert( UINT dType, UINT dPrnt, LPTSTR ptName )
{
	CONST CHAR	acTreeNodeInsert[] = { ("INSERT INTO TreeNode ( type, parentid, nodename ) VALUES ( ?, ?, ? )") };
	CONST CHAR	acAddNumCheck[] = { ("SELECT LAST_INSERT_ROWID( ) FROM TreeNode") };

	INT		rslt;
	UINT	iRast = 0;
	sqlite3_stmt	*statement;


	if( !(gpTreeCache) ){	TRACE( TEXT("NoDatabase") );	return 0;	}

	rslt = sqlite3_prepare( gpTreeCache, acTreeNodeInsert, -1, &statement, NULL);
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpTreeCache );	return 0;	}

	sqlite3_reset( statement );
	rslt = sqlite3_bind_int(    statement, 1, dType );	//	type
	rslt = sqlite3_bind_int(    statement, 2, dPrnt );	//	parentid
	rslt = sqlite3_bind_text16( statement, 3, ptName, -1, SQLITE_STATIC );	//	nodename

	rslt = sqlite3_step( statement );
	if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpTreeCache );	}

	sqlite3_finalize( statement );

	//	���ǉ�������̃A�����擾
	rslt = sqlite3_prepare( gpTreeCache, acAddNumCheck, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpTreeCache );	return 0;	}

	rslt = sqlite3_step( statement );

	iRast = sqlite3_column_int( statement, 0 );

	sqlite3_finalize( statement );

	return iRast;
}
//-------------------------------------------------------------------------------------------------




/*!
	���^�u����ǉ�
	@param[in]	ptFilePath	�t�@�C���p�X�E��Ȃ�g�p����J����
	@param[in]	ptBaseName	���[�g�����̃f�B���N�g����
	@param[in]	ptDispName	�^�u�̕\����
	@return	UINT	���ܓo�^����ID�ԍ��E�]��Ӗ��͂Ȃ�
*/
UINT SqlMultiTabInsert( LPCTSTR ptFilePath, LPCTSTR ptBaseName, LPCTSTR ptDispName )
{
	CONST CHAR	acMultitabInsert[] = { ("INSERT INTO MultiTab ( filepath, basename, dispname ) VALUES ( ?, ?, ? )") };
	CONST CHAR	acAddNumCheck[] = { ("SELECT LAST_INSERT_ROWID( ) FROM MultiTab") };

	INT		rslt;
	UINT	iRast = 0;
	sqlite3_stmt	*statement;

	if( !(gpDataBase) ){	return 0;	}

	rslt = sqlite3_prepare( gpDataBase, acMultitabInsert, -1, &statement, NULL);
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return 0;	}

	sqlite3_reset( statement );
	rslt = sqlite3_bind_text16( statement, 1, ptFilePath, -1, SQLITE_STATIC );	//	filepath
	rslt = sqlite3_bind_text16( statement, 2, ptBaseName, -1, SQLITE_STATIC );	//	basename
	rslt = sqlite3_bind_text16( statement, 3, ptDispName, -1, SQLITE_STATIC );	//	dispname

	rslt = sqlite3_step( statement );
	if( SQLITE_DONE != rslt ){	SQL_DEBUG( gpDataBase );	}

	sqlite3_finalize( statement );

	//	���ǉ�������̃A�����擾
	rslt = sqlite3_prepare( gpDataBase, acAddNumCheck, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return 0;	}

	rslt = sqlite3_step( statement );

	iRast = sqlite3_column_int( statement, 0 );

	sqlite3_finalize( statement );

	return iRast;
}
//-------------------------------------------------------------------------------------------------

/*!
	�h�c���w�肵�ăf�[�^�Q�b�g
	@param[in]	id			��������h�c
	@param[out]	ptFilePath	�t�@�C���p�X�o�b�t�@�EMAX_PATH�ł��邱��
	@param[out]	ptBaseName	���[�g�����̃f�B���N�g�����o�b�t�@�EMAX_PATH�ł��邱��
	@param[in]	ptDispName	�^�u�̕\�����EMAX_PATH�ł��邱��
	@return	UINT	�������������c��ID�ԍ��E�q�b�g���Ȃ�������O
*/
UINT SqlMultiTabSelect( INT id, LPTSTR ptFilePath, LPTSTR ptBaseName, LPTSTR ptDispName )
{
	CHAR	acQuery[MAX_STRING];
	INT		rslt;
	UINT	index = 0;
	sqlite3_stmt*	statement;


	if( !(gpDataBase) ){	return 0;	}

	StringCchPrintfA( acQuery, MAX_STRING, ("SELECT * FROM MultiTab WHERE id == %d"), id );

	rslt = sqlite3_prepare( gpDataBase, acQuery, -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return 0;	}

	rslt = sqlite3_step( statement );
	if( SQLITE_ROW == rslt )
	{
		index = sqlite3_column_int( statement , 0 );	//	id
		String_Cch_Copy( ptFilePath, MAX_PATH, (LPCTSTR)sqlite3_column_text16( statement, 1 ) );	//	filepath
		String_Cch_Copy( ptBaseName, MAX_PATH, (LPCTSTR)sqlite3_column_text16( statement, 2 ) );	//	basename
		String_Cch_Copy( ptDispName, MAX_PATH, (LPCTSTR)sqlite3_column_text16( statement, 3 ) );	//	dispname
	}

	sqlite3_finalize( statement );

	return index;
}
//-------------------------------------------------------------------------------------------------


/*!
	���^�u�f�[�^���폜
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT SqlMultiTabDelete( VOID )
{
	INT		rslt;
	sqlite3_stmt	*statement;

	if( !(gpDataBase) ){	return 0;	}

	rslt = sqlite3_prepare( gpDataBase, ("DELETE FROM MultiTab"), -1, &statement, NULL );
	if( SQLITE_OK != rslt ){	SQL_DEBUG( gpDataBase );	return E_OUTOFMEMORY;	}
	rslt = sqlite3_step( statement );
	sqlite3_finalize( statement );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

