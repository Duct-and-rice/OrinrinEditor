/*! @file
	@brief MLTツリーとかの部分
	このファイルは MaaTemplate.cpp です。
	@author	SikigamiHNQ
	@date	2011/06/21
*/

/*
Orinrin Editor : AsciiArt Story Editor for Japanese Only
Copyright (C) 2011 Orinrin/SikigamiHNQ

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.
*/
//-------------------------------------------------------------------------------------------------

/*

HukuTabs の中身
2ch\やる夫派生\やる夫\やる夫11（ミニサイズ）.mlt
2ch\やる夫派生\やらない夫\やらない夫01（基本）.mlt

PathStripPath	末端をファイル名ディレクトリ関係なしに切り出す

*/


#include "stdafx.h"
#include "OrinrinEditor.h"
#include "MaaTemplate.h"
//-------------------------------------------------------------------------------------------------

#ifdef _ORRVW
#define  MAATMPLT_CLASS_NAME	TEXT("ORINRINVIEWER")
#else
#define  MAATMPLT_CLASS_NAME	TEXT("MULTILINEAA_CLASS")
#endif
#define MA_WIDTH	320
#define MA_HEIGHT	320
//-------------------------------------------------------------------------------------------------

static HINSTANCE	ghInst;		//!<	アプリの実存

static  HWND	ghMainWnd;		//!<	本体ウインドウ


static  HWND	ghMaaWnd;		//!<	このウインドウ・staticを解除してはイケナイ

static  HWND	ghStsBarWnd;	//!<	ステータスバーハンドル

EXTERNED HWND	ghSplitaWnd;	//!<	スプリットバーハンドル

EXTERNED UINT	gbAAtipView;	//!<	非０で、ＡＡツールチップ表示

EXTERNED HWND	ghMaaFindDlg;	//!<	MAA検索ダイヤログハンドル

#ifdef MAA_PROFILE
static TCHAR	gatProfilePath[MAX_PATH];	//!<	プロファイルディレクトリ
#endif
static TCHAR	gatTemplatePath[MAX_PATH];	//!<	MLTルートディレクトリ

static CONST INT	giStbRoom[] = { 250 , 350 , -1 };
//-------------------------------------------------------------------------------------------------

LRESULT	CALLBACK MaaTmpltWndProc( HWND, UINT, WPARAM, LPARAM );
BOOLEAN	Maa_OnCreate( HWND, LPCREATESTRUCT );			//!<	WM_CREATE の処理・固定Editとかつくる
VOID	Maa_OnPaint( HWND );							//!<	WM_PAINT の処理・枠線描画とか
VOID	Maa_OnDestroy( HWND );							//!<	WM_DESTROY の処理・BRUSHとかのオブジェクトの破壊を忘れないように
LRESULT	Maa_OnNotify( HWND , INT, LPNMHDR );			//!<	
VOID	Maa_OnDrawItem( HWND, CONST DRAWITEMSTRUCT * );	//!<	
VOID	Maa_OnMeasureItem( HWND, MEASUREITEMSTRUCT * );	//!<	

#ifdef MAA_PROFILE
#define TREEPROF_AUTOCHECK

INT_PTR	CALLBACK TreeProfileDlgProc( HWND, UINT, WPARAM, LPARAM );	//!<	
HRESULT	TreeProfListUp( HWND, HWND, LPTSTR, HTREEITEM, UINT, INT );	//!<	
UINT	TreeLoadNodeProc( HWND, HWND, HTREEITEM, UINT );					//!<	
VOID	TreeProfCheckState( HWND, HTREEITEM, UINT );				//!<	
#ifdef TREEPROF_AUTOCHECK
UINT	TreeProfCheckExistent( HWND, LPTSTR, HWND, HTREEITEM, UINT );		//!<	
#endif
#endif
//-------------------------------------------------------------------------------------------------

/*!
	複数行ＡＡテンプレウインドウの作成
	@param[in]	hInstance	アプリのインスタンス
	@param[in]	hParentWnd	編集ビューのくっついてるウインドウのハンドル・これが本体
	@param[in]	pstFrame	
	@return		作ったビューのウインドウハンドル
*/
HWND MaaTmpltInitialise( HINSTANCE hInstance, HWND hParentWnd, LPRECT pstFrame )
{
	WNDCLASSEX	wcex;
	RECT	wdRect, rect;
#ifndef _ORRVW
	RECT	sbRect;
#endif
	INT		bMode = 0;

#ifdef _ORRVW
	INT	bTopMost;
#endif

#ifdef MAA_PROFILE
	WIN32_FIND_DATA	stFindData;
	HANDLE	hFind;
#endif

	ghMainWnd  = hParentWnd;
	ghInst = hInstance;

	//	専用のウインドウクラス作成
	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= MaaTmpltWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszClassName	= MAATMPLT_CLASS_NAME;
#ifdef _ORRVW
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ORINRINEDITOR));
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ORINRINVIEWER);
#else
	wcex.hIcon			= NULL;
	wcex.hIconSm		= NULL;
	wcex.lpszMenuName	= NULL;
#endif

	RegisterClassEx( &wcex );

	ghMaaFindDlg = NULL;	//	初期化

#ifdef _ORRVW
	SplitBarClass( hInstance );	//	スプリットバーの準備
#endif

	InitWindowPos( INIT_LOAD, WDP_MAATPL, &rect );
	if( 0 == rect.right || 0 == rect.bottom )	//	幅高さが０はデータ無し
	{
		GetWindowRect( hParentWnd, &wdRect );
#ifdef _ORRVW
		rect.left = ( wdRect.right  - MA_WIDTH ) / 2;
		rect.top  = ( wdRect.bottom - MA_HEIGHT ) / 2;
#else
		rect.left   = wdRect.right + 64;
		rect.top    = wdRect.top + 64;
#endif
		rect.right  = MA_WIDTH;
		rect.bottom = MA_HEIGHT;
		InitWindowPos( INIT_SAVE, WDP_MAATPL, &rect );//起動時保存
	}

	ghMaaWnd = CreateWindowEx(
#ifdef _ORRVW
		0, MAATMPLT_CLASS_NAME, TEXT("Orinrin Viewer"), WS_OVERLAPPEDWINDOW,
#else
		WS_EX_TOOLWINDOW,
		MAATMPLT_CLASS_NAME, TEXT("Multi Line AA Template"),
		WS_POPUP | WS_THICKFRAME | WS_BORDER | WS_CAPTION | WS_VISIBLE,
#endif
		rect.left, rect.top, rect.right, rect.bottom,
		NULL, NULL, hInstance, NULL);

	if( !(ghMaaWnd) )	return NULL;

	//	常に最全面に表示を？
#ifdef _ORRVW
	bTopMost = InitParamValue( INIT_LOAD, VL_MAA_TOPMOST, 1 );
	if( bTopMost )
	{
		SetWindowPos( ghMaaWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
		CheckMenuItem( GetMenu(ghMaaWnd), IDM_TOPMOST_TOGGLE, MF_CHECKED );
	}
#else
	if( InitWindowTopMost( INIT_LOAD, WDP_MAATPL , 0 ) )
	{	SetWindowPos( ghMaaWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );	}
#endif

	ShowWindow( ghMaaWnd, SW_SHOW );
	UpdateWindow( ghMaaWnd );

	ZeroMemory( gatTemplatePath, sizeof(gatTemplatePath) );

#ifdef MAA_PROFILE
	//	プロファイルロード
	ZeroMemory( gatProfilePath, sizeof(gatProfilePath) );
	InitParamString( INIT_LOAD, VS_PROFILE_NAME, gatProfilePath );

	//	そのファイルは存在するか？
	hFind = FindFirstFile( gatProfilePath, &stFindData );	//	TEXT("*")
	if( INVALID_HANDLE_VALUE != hFind ){	FindClose( hFind  );	}
	else{		ZeroMemory( gatProfilePath, sizeof(gatProfilePath) );	};

	if( NULL != gatProfilePath[0] )	//	起動時無ければ何もしない
	{
		bMode = TreeProfileMake( ghMaaWnd, gatProfilePath );
		if( 0 > bMode ){	bMode = 0;	}
	}
#else
	//	MLTディレクトリの位置
	InitMaaFldrOpen( INIT_LOAD, gatTemplatePath );
	bMode = 0;
#endif
	TreeConstruct( ghMaaWnd, gatTemplatePath, TRUE );

	gbAAtipView = InitParamValue( INIT_LOAD, VL_MAATIP_VIEW, 1 );

#ifndef MAA_PROFILE
	SqlDatabaseOpenClose( M_CREATE, MAA_FAVDB_FILE );
	SqlFavTableCreate( NULL );
#endif

#ifndef _ORRVW
	//	無効高さ値を持って帰る
	MaaTabBarSizeGet( pstFrame );
	pstFrame->top = pstFrame->bottom;
	GetClientRect( ghStsBarWnd, &sbRect );
	pstFrame->bottom = sbRect.bottom;	//	ステータスバーの分の面倒見る
#endif

	return ghMaaWnd;
}
//-------------------------------------------------------------------------------------------------

/*!
	ウインドウプロシージャ
	@param[in]	hWnd		親ウインドウのハンドル
	@param[in]	message		ウインドウメッセージの識別番号
	@param[in]	wParam		追加の情報１
	@param[in]	lParam		追加の情報２
	@retval 0	メッセージ処理済み
	@retval no0	ここでは処理せず次に回す
*/
LRESULT CALLBACK MaaTmpltWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	UINT	uRslt;

	switch( message )
	{
		HANDLE_MSG( hWnd, WM_CREATE,      Maa_OnCreate );		//	画面の構成パーツを作る。ボタンとか
		HANDLE_MSG( hWnd, WM_PAINT,       Maa_OnPaint );		//	画面の更新とか
		HANDLE_MSG( hWnd, WM_COMMAND,     Maa_OnCommand );		//	ボタン押されたとかのコマンド処理
		HANDLE_MSG( hWnd, WM_DESTROY,     Maa_OnDestroy );		//	ソフト終了時の処理
		HANDLE_MSG( hWnd, WM_NOTIFY,      Maa_OnNotify );		//	コモンコントロールの個別イベント
		HANDLE_MSG( hWnd, WM_SIZE,        Maa_OnSize  );		//	
		HANDLE_MSG( hWnd, WM_CHAR,        Maa_OnChar  );		//	
		HANDLE_MSG( hWnd, WM_DRAWITEM,    Maa_OnDrawItem  );	//	
		HANDLE_MSG( hWnd, WM_MEASUREITEM, Maa_OnMeasureItem );	//	
		HANDLE_MSG( hWnd, WM_CONTEXTMENU, Maa_OnContextMenu );	//	
		
		HANDLE_MSG( hWnd, WM_VSCROLL,     Aai_OnVScroll );	

		case WM_MOUSEWHEEL:	//	返り値が必要な場合を考慮
			uRslt = Maa_OnMouseWheel( hWnd, (INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam), (INT)(SHORT)HIWORD(wParam), (UINT)(SHORT)LOWORD(wParam) );
			break;

		default:	break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	クリエイト。
	@param[in]	hWnd			親ウインドウのハンドル
	@param[in]	lpCreateStruct	アプリケーションの初期化内容
	@return	TRUE	特になし
*/
BOOLEAN Maa_OnCreate( HWND hWnd, LPCREATESTRUCT lpCreateStruct )
{
	HINSTANCE lcInst = lpCreateStruct->hInstance;	//	受け取った初期化情報から、インスタンスハンドルをひっぱる
	RECT	rect, sbRect, tbRect;

	INT	spPos;


	GetClientRect( hWnd, &rect );

	//	ステータスバー
	ghStsBarWnd = CreateStatusWindow( WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP, TEXT(""), hWnd, IDSB_STATUSBAR );
	GetClientRect( ghStsBarWnd, &sbRect );
	rect.bottom -= sbRect.bottom;

	SendMessage( ghStsBarWnd, SB_SETPARTS, 3, (LPARAM)giStbRoom );

	TreeInitialise( hWnd, lcInst, &rect );	//	ツリービューとお気にビュー作る

	MaaTabBarSizeGet( &tbRect );

	//	設定からスプリットバーの位置を引っ張る
	spPos = InitParamValue( INIT_LOAD, VL_MAA_SPLIT, TMPL_DOCK );
	ghSplitaWnd = SplitBarCreate( lcInst, hWnd, spPos, tbRect.bottom, rect.bottom - tbRect.bottom );

	AaItemsInitialise( hWnd, lcInst, &rect );	//	MLTの中身表示ビューつくる

	return TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	COMMANDメッセージの受け取り。ボタン押されたとかで発生
	@param[in]	hWnd		親ウインドウのハンドル
	@param[in]	id			メッセージを発生させた子ウインドウの識別子	LOWORD(wParam)
	@param[in]	hwndCtl		メッセージを発生させた子ウインドウのハンドル	lParam
	@param[in]	codeNotify	通知メッセージ	HIWORD(wParam)
	@return		なし
*/
VOID Maa_OnCommand( HWND hWnd, INT id, HWND hwndCtl, UINT codeNotify )
{
	LONG_PTR	rdExStyle;

	switch( id )
	{
		//	リストスタティックでのクリックはここにくる
		case IDSO_AAITEMS:	TRACE( TEXT("static") );	break;

#if defined( DRAUGHT_STYLE )
		//	ドラフトボードオーポン
		case IDM_DRAUGHT_OPEN:	DraughtWindowCreate( GetModuleHandle(NULL), ghMaaWnd, 0 );	break;
#endif
#ifdef THUMBNAIL_STYLE
		//	サムネイルオーポン
		case IDM_MAA_THUMBNAIL_OPEN:	DraughtWindowCreate( GetModuleHandle(NULL), ghMaaWnd, 1 );	break;
#endif

		case IDLB_FAVLIST:	FavListSelected( hWnd, codeNotify );	break;

		case IDM_TOPMOST_TOGGLE:	//	常時最全面と通常ウインドウのトグル
			rdExStyle = GetWindowLongPtr( hWnd, GWL_EXSTYLE );
			if( WS_EX_TOPMOST & rdExStyle )
			{
				SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
#ifdef _ORRVW
				CheckMenuItem( GetMenu(hWnd), IDM_TOPMOST_TOGGLE, MF_UNCHECKED );
				InitParamValue( INIT_SAVE, VL_MAA_TOPMOST, 0 );
#else
				InitWindowTopMost( INIT_SAVE, WDP_MAATPL, 0 );
#endif
			}
			else
			{
				SetWindowPos( hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
#ifdef _ORRVW
				CheckMenuItem( GetMenu(hWnd), IDM_TOPMOST_TOGGLE, MF_CHECKED );
				InitParamValue( INIT_SAVE, VL_MAA_TOPMOST, 1 );
#else
				InitWindowTopMost( INIT_SAVE, WDP_MAATPL, 1 );
#endif
			}
#ifdef _ORRVW
			DrawMenuBar( hWnd );
#endif
			break;

		case IDCB_AAITEMTITLE:	AaTitleSelect( hWnd, codeNotify );	break;

		//	ここに来るのは、メインメニューからの選択のみ
#ifndef MAA_PROFILE
		case IDM_TREE_RECONSTRUCT:	//	プロフモードでは、ディレクトリ選択ダイヤログ出す
			InitMaaFldrOpen( INIT_LOAD, gatTemplatePath );
			TreeConstruct( hWnd , gatTemplatePath, FALSE );
			break;
#endif

#ifdef _ORRVW
		case IDM_ABOUT:	DialogBox( ghInst , MAKEINTRESOURCE(IDD_ORRVWR_ABOUTBOX), hWnd, About );	break;
		case IDM_EXIT:	DestroyWindow( hWnd );	break;

		case IDM_GENERAL_OPTION:	//	設定
			DialogBoxParam( ghInst, MAKEINTRESOURCE(IDD_ORRVWR_OPTION_DLG), hWnd, OptionDlgProc, NULL );
			break;

	#ifdef MAA_PROFILE
		case IDM_MAA_PROFILE_MAKE:	TreeProfileOpen( hWnd );	break;
		case IDM_TREE_RECONSTRUCT:	TreeProfileRebuild( hWnd  );	break;
	#endif
#else
		case  IDM_WINDOW_CHANGE:	WindowFocusChange( WND_MAAT,  1 );	break;
		case  IDM_WINDOW_CHG_RVRS:	WindowFocusChange( WND_MAAT, -1 );	break;
#endif
		//	Ｅコンテキスト・アクセロリータ　Ｖメニュー・アクセロリータ
		case IDM_FINDMAA_DLG_OPEN:	TreeMaaFileFind( hWnd );	break;
		//全文検索できるか？
		default:	break;
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	PAINT。無効領域が出来たときに発生。背景の扱いに注意。背景を塗りつぶしてから、オブジェクトを描画
	@param[in]	hWnd	親ウインドウのハンドル
	@return		無し
*/
VOID Maa_OnPaint( HWND hWnd )
{
	PAINTSTRUCT	ps;
	HDC			hdc;

	hdc = BeginPaint( hWnd, &ps );

	EndPaint( hWnd, &ps );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	ウインドウを閉じるときに発生。デバイスコンテキストとか確保した画面構造のメモリとかも終了。
	@param[in]	hWnd	親ウインドウのハンドル
	@return		無し
*/
VOID Maa_OnDestroy( HWND hWnd )
{
	RECT	rect;
	DWORD	dwStyle;

	dwStyle = GetWindowStyle( hWnd );
	if( !(dwStyle & WS_MINIMIZE) )
	{
		//	最大化してたら記録しない？

		//	スプリットバーの位置を記録
		SplitBarPosGet( ghSplitaWnd, &rect );
		InitParamValue( INIT_SAVE, VL_MAA_SPLIT, rect.left );

		//	ウインドウ位置も記録
		GetWindowRect( hWnd, &rect );
		rect.right  -= rect.left;
		rect.bottom -= rect.top;
		InitWindowPos( INIT_SAVE, WDP_MAATPL, &rect );//終了時保存
	}

	AaItemsInitialise( NULL, NULL, NULL );
	AacMatrixClear(   );	//	既存の内容全破壊

	TreeInitialise( NULL, NULL, NULL );

	SqlDatabaseOpenClose( M_DESTROY, NULL );

#ifdef _ORRVW
	PostQuitMessage( 0 );
#endif

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	ウインドウのサイズ変更
	@param[in]	hWnd	ウインドウハンドル
*/
VOID Maa_OnSize( HWND hWnd, UINT state, INT cx, INT cy )
{
	RECT	rect, sbRect, tbRect;

	GetClientRect( hWnd, &rect );

	MoveWindow( ghStsBarWnd, 0, 0, 0, 0, TRUE );	//	ステータスバー
	GetClientRect( ghStsBarWnd, &sbRect );
	rect.bottom -= sbRect.bottom;	//	ステータスバーの分の面倒見る

	TabBarResize( hWnd, &rect );
	MaaTabBarSizeGet( &tbRect );
	rect.top = tbRect.bottom;
	rect.bottom -= tbRect.bottom;	//	タブバーの分の面倒も見る

	SplitBarResize( ghSplitaWnd, &rect );	//	スプリットバー

	TreeResize( hWnd, &rect );		//	ツリービュー
	AaItemsResize( hWnd, &rect );	//	一覧ビュー

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	ノーティファイメッセージの処理
	@param[in]	hWnd		ウインドウハンドル
	@param[in]	idFrom		NOTIFYを発生させたコントロールのＩＤ
	@param[in]	pstNmhdr	NOTIFYの詳細
	@return		処理した内容とか
*/
LRESULT Maa_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	//	ツリービューのやつ
	if( IDTV_ITEMTREE == idFrom ){	TreeNotify( hWnd , (LPNMTREEVIEW)pstNmhdr );	}
	if( IDTB_TREESEL == idFrom ){	TabBarNotify( hWnd , pstNmhdr );	}

	return 0;	//	何もないなら０を戻す
}
//-------------------------------------------------------------------------------------------------

/*!
	オーナードローの発生した処理
	@param[in]	hWnd			親ウインドウのハンドル
	@param[in]	*pstDrawItem	ドローの為の情報
*/
VOID Maa_OnDrawItem( HWND hWnd, CONST DRAWITEMSTRUCT *pstDrawItem )
{
	if( IDSO_AAITEMS == pstDrawItem->CtlID )
	{
		AaItemsDrawItem( hWnd, pstDrawItem );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	オーナードローの描画エリアの処理
	@param[in]	hWnd			親ウインドウのハンドル
	@param[in]	*pstMeasureItem	描画の為の情報を出したり入れたり
*/
VOID Maa_OnMeasureItem( HWND hWnd, MEASUREITEMSTRUCT *pstMeasureItem )
{
#if 0
	if( IDSO_AAITEMS == pstMeasureItem->CtlID )
	{
		AaItemsMeasureItem( hWnd, pstMeasureItem );
	}
#endif
	return;
}
//-------------------------------------------------------------------------------------------------


/*!
	まうすほい〜る廻ったら
	@param[in]	hWnd	親ウインドウのハンドル
	@param[in]	xPos	発生した座標Ｘ
	@param[in]	yPos	発生した座標Ｙ
	@param[in]	zDelta	回転量・WHEEL_DELTAの倍数・正の値は前(奥)、負の値は後ろ(手前)へ回された
	@param[in]	fwKeys	押されてるキー
	@return		非０フォーカスと真下が違う　０同じ
*/
UINT Maa_OnMouseWheel( HWND hWnd, INT xPos, INT yPos, INT zDelta, UINT fwKeys )
{
	HWND	hChdWnd;
	POINT	stPoint;

	stPoint.x = xPos;
	stPoint.y = yPos;
	ScreenToClient( ghMaaWnd, &stPoint );
	hChdWnd = ChildWindowFromPointEx( ghMaaWnd, stPoint, CWP_SKIPINVISIBLE | CWP_SKIPDISABLED | CWP_SKIPTRANSPARENT );

	if( hChdWnd == hWnd )	return 0;

	if( AaItemsIsUnderCursor( ghMaaWnd, hChdWnd , zDelta ) )	return 1;

	TreeFavIsUnderCursor( ghMaaWnd, hChdWnd, xPos, yPos, zDelta, fwKeys );

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	ステータスバーになんか表示
	@param[in]	room	描画する枠０インデックス
	@param[in]	ptMsg	描画する文字列
*/
VOID StatusBarMsgSet( UINT room, LPTSTR ptMsg )
{

	SendMessage( ghStsBarWnd, SB_SETTEXT, room, (LPARAM)ptMsg );

	UpdateWindow( ghStsBarWnd );

	return;
}
//-------------------------------------------------------------------------------------------------

#ifdef MAA_PROFILE

/*!
	プロッファイルを作ってMLTディレクトリも指定したり直ぐ開いたり
	@param[in]	hWnd	ウインドウハンドル・どこのだろう？
	@param[in]	ptProf	開くプロッファイル名
	@return		UINT	正：ＳＱＬから構築　０ファイルから構築セヨ　負：エラー
*/
INT TreeProfileMake( HWND hWnd, LPTSTR ptProf )
{
	OPENFILENAME	stOpenFile;
	BOOLEAN	bOpened;
	UINT	iCount;
	TCHAR	atFilePath[MAX_PATH], atFileName[MAX_STRING], atBuffer[MAX_PATH];
	TCHAR	atFolder[MAX_PATH];
	INT_PTR	iRslt;



	ZeroMemory( atFilePath,  sizeof(atFilePath) );
	ZeroMemory( atFileName,  sizeof(atFileName) );

	if( ptProf )
	{
		StringCchCopy( atFilePath, MAX_PATH, ptProf );
	}
	else	//	開く指定がなかったら、こっちから指定する
	{
		ZeroMemory( &stOpenFile, sizeof(OPENFILENAME) );
		stOpenFile.lStructSize     = sizeof(OPENFILENAME);
		stOpenFile.hwndOwner       = ghMaaWnd;
		stOpenFile.lpstrFilter     = TEXT("プロファイル(*.qor)\0*.qor\0全てのファイル(*.*)\0*.*\0\0");
//		stOpenFile.nFilterIndex    = 1;
		stOpenFile.lpstrFile       = atFilePath;
		stOpenFile.nMaxFile        = MAX_PATH;
		stOpenFile.lpstrFileTitle  = atFileName;
		stOpenFile.nMaxFileTitle   = MAX_STRING;
	//	stOpenFile.lpstrInitialDir = 
		stOpenFile.lpstrTitle      = TEXT("ファイル名を指定するか、作成したい名前を入力するかしらー");
		stOpenFile.Flags           = OFN_EXPLORER | OFN_HIDEREADONLY;
		stOpenFile.lpstrDefExt     = TEXT("qor");

		//ここで FileSaveDialogue を出す
		bOpened = GetOpenFileName( &stOpenFile );

		if( !(bOpened) ){	return -1;	}	//	キャンセルしてたら何もしない

		StringCchCopy( gatProfilePath, MAX_PATH, atFilePath );
		InitParamString( INIT_SAVE, VS_PROFILE_NAME, atFilePath );
	}


	TabMultipleStore( hWnd );	//	既存のデータ閉じる前に開いてる副タブを保存
	TabMultipleDeleteAll( hWnd );	//	副タブ全部閉じる

	SqlDatabaseOpenClose( M_DESTROY, NULL );	//	既存のデータは閉じる

	//	プロッファイル名をフルパスで渡す

	SqlDatabaseOpenClose( M_CREATE, atFilePath );//MAA_TREE_CACHE	TEXT(":memory:")
	//	中で開くか勝手に作られる

	//	プロファイル名をステータスバーに表示しておく
	StringCchCopy( atBuffer, MAX_PATH, atFilePath );
	PathStripPath( atBuffer );
	StatusBarMsgSet( 0, atBuffer );

	ZeroMemory( atFolder,  sizeof(atFolder) );

	//	MLTディレクトリをセット
	ZeroMemory( gatTemplatePath,  sizeof(gatTemplatePath) );
	SqlTreeProfSelect( NULL, 0, gatTemplatePath, MAX_PATH );
	//	ディレクトリがなかったらフォルダオーポンダイヤログで指定して再構築
	//	含むディレクトリと含まないディレクトリを指定出来るようにする
	//	ディレクトリが有ったら、Cacheを見て再構築すればいい
	if( NULL == gatTemplatePath[0] )
	{
		//ダイヤログ呼び出す
		//ダイヤログでは、ディレクトリ直下のディレクトリをリストアップ
		//ON/OFFをCheckして、再構築釦＞ツリー再構築
		//OFFのは再構築しない
		//	ツリー再構築なら、このダイヤログを呼び出す
		iRslt = DialogBoxParam( ghInst, MAKEINTRESOURCE(IDD_PROFILE_TREESEL_DLG), hWnd, TreeProfileDlgProc, (LPARAM)atFolder );
		if( IDCANCEL == iRslt ){	return -1;	}

		StringCchCopy( gatTemplatePath, MAX_PATH, atFolder );
	}

	//	データあるか？
	iCount = SqlTreeCount( 1, NULL );
	if( 0 >= iCount )	return 0;

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	別Profile開いたり作ったり
*/
INT TreeProfileOpen( HWND hWnd )
{
	INT	iRslt;

	iRslt = TreeProfileMake( hWnd, NULL );
	if( 0 > iRslt ){	return 0;	}	//	内容変更なので、負ならナニもしない。
	//	ここで、構築するを呼べばよろしい
	TreeConstruct( ghMaaWnd, gatTemplatePath, TRUE );
	//既存の開くなら１、新規作成なら０が戻るはず
	//いずれにしても、副タブ再構築が必要

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	現在のプロファイルの内容を編集する
	@param[in]	hWnd	ハンドル
*/
INT TreeProfileRebuild( HWND hWnd )
{
	TCHAR	atFolder[MAX_PATH];
	INT_PTR	iRslt;

	//	プロファイルが開けてない場合は、警告メッセージだしてなにもしない

	//	ルートディレクトリを、ダイヤログに渡してオーポン
	StringCchCopy( atFolder, MAX_PATH, gatTemplatePath );	//	ルートディレクトリをコピー

	iRslt = DialogBoxParam( ghInst, MAKEINTRESOURCE(IDD_PROFILE_TREESEL_DLG), hWnd, TreeProfileDlgProc, (LPARAM)atFolder );
	if( IDCANCEL == iRslt ){	return (-1);	}	//	内容変更なので、キャンセルならナニもしない。
	//	ルートディレクトリ変更してから再構築開始
	if( NULL != atFolder[0] )	//	この時点で空はないか？
	{
		StringCchCopy( gatTemplatePath, MAX_PATH, atFolder );
		//	こっちから開いた場合は、副タブ再構築不要
		TreeConstruct( ghMaaWnd, gatTemplatePath, FALSE );
	}

	return iRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	ツリーに取り込むディレクトリーを設定して構築する
	@param[in]	hDlg	ダイヤログハンドル
	@param[in]	message	ウインドウメッセージの識別番号
	@param[in]	wParam	追加の情報１
	@param[in]	lParam	追加の情報２
	@retval 0	メッセージは処理していない
	@retval no0	なんか処理された
*/
INT_PTR CALLBACK TreeProfileDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	static BOOLEAN	cbAct;
	static LPTSTR	ptFolder;
	static  HWND	chTvWnd;

	static HTREEITEM	chTreeRoot;
	static TVINSERTSTRUCT	cstRootIns;

	HWND	hWorkWnd;
	TCHAR	atTgtDir[MAX_PATH];
	INT		id;

	DWORD	mPos;
	UINT	bCheck, count;
	INT		idFrom;
	LPNMHDR	pstNmhdr;

	LPNMTREEVIEW	pstNmTrVw;
	TVHITTESTINFO	stTreeHit;
	HTREEITEM		hItem;

	SHFILEINFO		stShFileInfo;

//そのままリストアップすると、内容編集のときに中身が愚茶倶邪になる

	switch( message )
	{
		default:	break;

		case WM_INITDIALOG:
			ptFolder = (LPTSTR)lParam;

			cbAct = FALSE;

			hWorkWnd = GetDlgItem( hDlg, IDPB_PRTREE_PROGRESS );
			SendMessage( hWorkWnd, PBM_SETRANGE32, 0, 0xFF );
			SendMessage( hWorkWnd, PBM_SETPOS, 0, 0 );
			SendMessage( hWorkWnd, PBM_SETSTEP, 1, 0 );
			ShowWindow( hWorkWnd, SW_HIDE );

			Edit_SetText( GetDlgItem(hDlg,IDE_PRTREE_DIR), ptFolder );

			chTvWnd = GetDlgItem( hDlg, IDTV_PRTREE_DIR_TVIEW );

			//	ツリービュー構築
			TreeView_DeleteAllItems( chTvWnd );
			//	ルートアイテム作る
			ZeroMemory( &cstRootIns, sizeof(TVINSERTSTRUCT) );
			cstRootIns.hParent      = TVI_ROOT;
			cstRootIns.hInsertAfter = TVI_SORT;
			cstRootIns.item.mask    = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			cstRootIns.item.pszText = TEXT("ROOT");
			cstRootIns.item.lParam  = 0;

			SHGetFileInfo( TEXT(""), 0, &stShFileInfo, sizeof(SHFILEINFO), (SHGFI_SYSICONINDEX|SHGFI_SMALLICON) );
			cstRootIns.item.iImage = stShFileInfo.iIcon;
			SHGetFileInfo( TEXT(""), 0, &stShFileInfo, sizeof(SHFILEINFO), (SHGFI_SYSICONINDEX|SHGFI_SMALLICON|SHGFI_OPENICON) );
			cstRootIns.item.iSelectedImage = stShFileInfo.iIcon;

			chTreeRoot = TreeView_InsertItem( chTvWnd, &cstRootIns );

			SqlTreeCacheOpenClose( M_CREATE );

			//	開いたとき、ルートフォルダ指定が有効なら、自動リストアップして、ディレクトリを確認してチェック付ける
			//if( NULL != ptFolder[0] )
			//{
			//	TreeProfListUp( hDlg, chTvWnd, ptFolder, chTreeRoot, 0, 1 );
			//	TreeView_Expand( chTvWnd, chTreeRoot, TVE_EXPAND );
			//}
			//開けたときにやると重い
			return (INT_PTR)TRUE;


		case WM_COMMAND:
			id = LOWORD(wParam);
			switch( id )
			{
				case  IDB_PRTREE_DIR_REF:	//	ディレクトリ参照
					if( SelectDirectoryDlg( hDlg, atTgtDir, MAX_PATH ) )
					{
						Edit_SetText( GetDlgItem(hDlg,IDE_PRTREE_DIR), atTgtDir );
					}
					return (INT_PTR)TRUE;

				case  IDB_PRTREE_LISTUP:	//	リストアッポ開始
					Edit_GetText( GetDlgItem(hDlg,IDE_PRTREE_DIR), atTgtDir, MAX_PATH );
					if( NULL != atTgtDir[0] )
					{
						TreeView_DeleteAllItems( chTvWnd  );	//	一旦全破壊してルート作り直し
						SqlTreeNodeDelete(  0 );	//	キャッシュも破壊
						chTreeRoot = TreeView_InsertItem( chTvWnd, &cstRootIns );	//	ルート作成
#ifndef TREEPROF_AUTOCHECK
						TreeView_SetCheckState( chTvWnd , chTreeRoot, TRUE );	//	チェキマーク？
#endif
						UpdateWindow( chTvWnd );

						StringCchCopy( ptFolder, MAX_PATH, atTgtDir );	//	文字数キメうち注意

						hWorkWnd = GetDlgItem( hDlg, IDPB_PRTREE_PROGRESS );
						SendMessage( hWorkWnd, PBM_SETPOS, 0, 0 );
						ShowWindow( hWorkWnd, SW_SHOW );
						TreeProfListUp( hDlg, chTvWnd, atTgtDir, chTreeRoot, 0, 1 );
						TreeView_Expand( chTvWnd, chTreeRoot, TVE_EXPAND );
#ifdef TREEPROF_AUTOCHECK
						//	今のルートと、PROFILEのルートを確認して、同じなら、チェックを付けていく
						TreeProfCheckExistent( hDlg, atTgtDir, chTvWnd, chTreeRoot, 0 );
#endif
						ShowWindow( hWorkWnd, SW_HIDE );
					}
					return (INT_PTR)TRUE;

				case IDOK:
					count = TreeView_GetCount( chTvWnd );
					if( 1 >= count )
					{
						MessageBox( hDlg, TEXT("リストアップ出来ていないようなのです。\r\nこのままではツリーが作れないのです。"), TEXT("あぅあぅ"), MB_OK | MB_ICONERROR );
						return (INT_PTR)TRUE;
					}

					//	ディレクトリ選択に合わせてSQLを固定
					if( SUCCEEDED( TreeLoadDirCheck( hDlg, chTvWnd ) ) )
					{
						SqlTreeCacheOpenClose( M_DESTROY );
						EndDialog(hDlg, IDOK );
					}
					return (INT_PTR)TRUE;

				case IDCANCEL:
					SqlTreeCacheOpenClose( M_DESTROY );
					EndDialog(hDlg, IDCANCEL );
					return (INT_PTR)TRUE;
			}
			break;


		case WM_NOTIFY:
			idFrom   = (INT)wParam;
			pstNmhdr = (LPNMHDR)lParam;
			if( IDTV_PRTREE_DIR_TVIEW == idFrom )
			{
				pstNmTrVw = (LPNMTREEVIEW)pstNmhdr;
				if( NM_CLICK == pstNmhdr->code )
				{
					ZeroMemory( &stTreeHit, sizeof(TVHITTESTINFO) );
					mPos = GetMessagePos( );
					stTreeHit.pt.x = GET_X_LPARAM( mPos );
					stTreeHit.pt.y = GET_Y_LPARAM( mPos );
					MapWindowPoints( HWND_DESKTOP, chTvWnd, &stTreeHit.pt, 1 );
					TreeView_HitTest( chTvWnd, &stTreeHit );

					//	チェックが入る前に来る
					if ( TVHT_ONITEMSTATEICON & stTreeHit.flags )
					{
						//	チェックが入る時→チェックが入っていない状態
						bCheck = TreeView_GetCheckState( chTvWnd, stTreeHit.hItem );

						//	先に下方向をスキャァ〜ンしないとおかしくなるようだ
						hItem = TreeView_GetChild( chTvWnd, stTreeHit.hItem );
						TreeProfCheckState( chTvWnd, hItem, bCheck );

						//	上方向には、チェック入ったら連鎖ＯＮにする
						if( !(bCheck)  )	//	チェキするとき
						{
							hItem = TreeView_GetParent( chTvWnd, stTreeHit.hItem );
							while( hItem  )
							{
								TreeView_SetCheckState( chTvWnd, hItem, TRUE );
								hItem = TreeView_GetParent( chTvWnd, hItem );
							}
						}
					}

					SetWindowLong( hDlg, DWL_MSGRESULT, 0 );
					return (INT_PTR)TRUE;
				}

			}
			break;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	呼ばれる度に、プログレスバーを増やす
	@param[in]	hDlg	ダイヤログハンドル
*/
VOID TreeProfProgressUp( HWND hDlg )
{
	HWND	hProgWnd;
	UINT	pos;

	hProgWnd = GetDlgItem( hDlg, IDPB_PRTREE_PROGRESS );

	pos = SendMessage( hProgWnd, PBM_GETPOS, 0, 0 );
	pos++;
	pos &= 0xFF;
	SendMessage( hProgWnd, PBM_SETPOS, pos, 0 );
	UpdateWindow( hProgWnd );

	return;
}
//-------------------------------------------------------------------------------------------------

#ifdef TREEPROF_AUTOCHECK
/*!
	既存のプロフの内容チェキを再現する・再帰
	@param[in]	ptTgDir	選択してるディレクトリ・プロフのと違うなら何もしない・NULLなら確認しない
	@param[in]	hTvWnd	ツリービューハンドル
	@param[in]	hNode	確認するノード
	@param[in]	sqlID	親ノードのsqlID・ルートは０
	@return	チェックした回数
*/
UINT TreeProfCheckExistent( HWND hDlg, LPTSTR ptTgDir, HWND hTvWnd, HTREEITEM hNode, UINT sqlID )
{
	UINT		checked = 0, tgtID;
	TCHAR		atProfRoot[MAX_PATH];
	TCHAR		atName[MAX_PATH];
	HTREEITEM	hItem, hRoot;
	TVITEM		stItem;

	if( ptTgDir )	//	ここが有効なのはルートの時のみ
	{
		ZeroMemory( atProfRoot, sizeof(atProfRoot) );
		SqlTreeProfSelect( NULL, 0, atProfRoot, MAX_PATH );
		//	異なるなら何もしない
		if( StrCmp( atProfRoot, ptTgDir ) )	return 0;

		//ルートなので、Childから始める
		hRoot = hNode;
		hItem = TreeView_GetChild( hTvWnd , hNode );	//	子ノードを確認
		hNode = hItem;
		if( !(hNode) )	return 0;
		sqlID = 0;	//	問題無いはず
	}

	do{
		//	自ノードの名称確認して、親IDと一緒にデータひっぱる
		ZeroMemory( &stItem, sizeof(TVITEM) );
		stItem.mask       = TVIF_HANDLE | TVIF_TEXT;
		stItem.hItem      = hNode;
		stItem.pszText    = atName;
		stItem.cchTextMax = MAX_PATH;
		TreeView_GetItem( hTvWnd, &stItem );
		tgtID = SqlTreeFileGetOnParent( atName, sqlID );
		//	ここでＩＤ有効であれば、プロフに含まれている
		if( tgtID )
		{
			checked++;
			TreeView_SetCheckState( hTvWnd, hNode, TRUE );
			//	ディレクトリのチェックが有効でなければ、その下は何も無い
			hItem = TreeView_GetChild( hTvWnd , hNode );	//	子ノードを確認
			//	存在してたら下の階層をチェック
			if( hItem ){	checked += TreeProfCheckExistent( hDlg, NULL , hTvWnd, hItem, tgtID );	}
		}

		//	終わったら次にいく
		hItem = TreeView_GetNextSibling( hTvWnd, hNode );
		hNode = hItem;

		TreeProfProgressUp( hDlg );
	}
	while( hNode );

	if( ptTgDir )	//	ここが有効なのはルートの時のみ
	{
		if( checked ){	TreeView_SetCheckState( hTvWnd, hRoot, TRUE );	}
	}

	return checked;
}
//-------------------------------------------------------------------------------------------------
#endif
/*!
	再帰で、以下のツリーのチャックのON/OFFする
	@param[in]	hTvWnd	ツリービューハンドル
	@param[in]	hNode	確認するノード
	@param[in]	bCheck	ON/OFFのセット
*/
VOID TreeProfCheckState( HWND hTvWnd, HTREEITEM hNode, UINT bCheck )
{
	HTREEITEM	hItem;

	if( !(hNode) )	return;

	do
	{
		TreeView_SetCheckState( hTvWnd, hNode, bCheck ? FALSE : TRUE );

		hItem = TreeView_GetChild( hTvWnd , hNode );	//	子ノードを確認

		//	存在してたら下の階層をチェック
		if( hItem ){	TreeProfCheckState( hTvWnd, hItem, bCheck );	}

		//	終わったら次にいく
		hItem = TreeView_GetNextSibling( hTvWnd, hNode );
		hNode = hItem;
	}
	while( hNode );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	リストアップ処理
	@param[in]	hTvWnd	ツリービューハンドル
	@param[in]	ptRoot	MLTルートディレクトリ
	@param[in]	hTreePr	対象ディレクトリのツリーアイテム・こいつにぶら下げていく
	@param[in]	dPrntID	SQLのID・ディレクトリ番号
	@param[in]	fCheck	１全チャックする　０ＳＱＬに既存ならチェキ　−１チョックしない
	@return		HRESULT	終了状態コード
*/
HRESULT TreeProfListUp( HWND hDlg, HWND hTvWnd, LPTSTR ptRoot, HTREEITEM hTreePr, UINT dPrntID, INT fCheck )
{
	HANDLE	hFind;
	TCHAR	atPath[MAX_PATH], atNewTop[MAX_PATH], atTarget[MAX_PATH];
	BOOL	bRslt;
	UINT	dPnID = 0;

	WIN32_FIND_DATA	stFindData;

	HTREEITEM	hNewParent, hLastDir = TVI_FIRST;
	TVINSERTSTRUCT	stTreeIns;
	SHFILEINFO	stShFileInfo;

	ZeroMemory( &stTreeIns, sizeof(TVINSERTSTRUCT) );
	stTreeIns.hParent      = hTreePr;
	stTreeIns.hInsertAfter = TVI_LAST;
	stTreeIns.item.mask    = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	ZeroMemory( atTarget, sizeof(atTarget) );
	StringCchCopy( atTarget, MAX_PATH, ptRoot );
	PathAppend( atTarget, TEXT("*") );

	hFind = FindFirstFile( atTarget, &stFindData );	//	TEXT("*")
	do{
		if( lstrcmp( stFindData.cFileName, TEXT("..") ) && lstrcmp( stFindData.cFileName, TEXT(".") ) )
		{
			TreeProfProgressUp( hDlg );

			StringCchCopy( atPath, MAX_PATH, ptRoot );
			PathAppend( atPath, stFindData.cFileName );

			SHGetFileInfo( atPath, 0, &stShFileInfo, sizeof(SHFILEINFO), (SHGFI_SYSICONINDEX|SHGFI_SMALLICON) );
			stTreeIns.item.iImage = stShFileInfo.iIcon;
			SHGetFileInfo( atPath, 0, &stShFileInfo, sizeof(SHFILEINFO), (SHGFI_SYSICONINDEX|SHGFI_SMALLICON|SHGFI_OPENICON) );
			stTreeIns.item.iSelectedImage = stShFileInfo.iIcon;

			stTreeIns.item.pszText = stFindData.cFileName;

			if( stFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{	//	ディレクトリの場合
				dPnID = SqlTreeCacheInsert( FILE_ATTRIBUTE_DIRECTORY, dPrntID, stFindData.cFileName );

				stTreeIns.item.lParam  = dPnID;	//	通し番号であることに注意
				stTreeIns.hInsertAfter = hLastDir;
				hNewParent = TreeView_InsertItem( hTvWnd, &stTreeIns );
				hLastDir = hNewParent;

#ifndef TREEPROF_AUTOCHECK
				TreeView_SetCheckState( hTvWnd , hNewParent, TRUE );	//	チェキマーク？
#endif
				StringCchCopy( atNewTop, MAX_PATH, ptRoot );
				PathAppend( atNewTop, stFindData.cFileName );

				TreeProfListUp( hDlg, hTvWnd, atNewTop, hNewParent, dPnID, fCheck );	//	該当ディレクトリ内を再帰検索

			}
			else
			{	//	ファイルの場合
				bRslt  = PathMatchSpec( stFindData.cFileName, TEXT("*.mlt") );	//	ヒットしたらTRUE
				bRslt |= PathMatchSpec( stFindData.cFileName, TEXT("*.ast") );	//	ヒットしたらTRUE
				if( bRslt )	//	20110720	ASTもイケるようにする
				{
					dPnID = SqlTreeCacheInsert( FILE_ATTRIBUTE_NORMAL, dPrntID, stFindData.cFileName );

					stTreeIns.item.lParam  = dPnID;	//	通し番号であることに注意
					stTreeIns.hInsertAfter = TVI_LAST;
					hNewParent = TreeView_InsertItem( hTvWnd, &stTreeIns );

#ifndef TREEPROF_AUTOCHECK
					TreeView_SetCheckState( hTvWnd , hNewParent, TRUE );	//	チェキマーク？
#endif
				}
			}
		}

	}while( FindNextFile( hFind, &stFindData ) );

	FindClose( hFind );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	ツリープロファイル作成で、チェックされてるでゅれくとりと麾下のファイルを記録
	@param[in]	hDlg	ダイヤログハンドル
	@param[in]	hTvWnd	ツリービューハンドル
	@return		HRESULT	終了状態コード
*/
HRESULT TreeLoadDirCheck( HWND hDlg, HWND hTvWnd )
{
	TCHAR	atTgtDir[MAX_PATH];
	HTREEITEM	hTreeRoot, hItem;
	UINT	dCacheMax, dCacheCnt, m, count;
	UINT	dType, dPrnt, index, logoa;
	TCHAR	atName[MAX_PATH];
	HWND	hWorkWnd;

	hTreeRoot = TreeView_GetRoot( hTvWnd  );	//	とりやえずルート確保
	//	ルートは関係ないので、直下を調べる
	hItem = TreeView_GetChild( hTvWnd, hTreeRoot );	//	子ノードを確認


	hWorkWnd = GetDlgItem( hDlg, IDPB_PRTREE_PROGRESS );
	SendMessage( hWorkWnd, PBM_SETPOS, 0, 0 );
	ShowWindow( hWorkWnd, SW_SHOW );

	//	チェック状況を確認・ファイルのチェックが無いならヤバイ
	count = TreeLoadNodeProc( hDlg, hTvWnd, hItem, 0 );
	TRACE( TEXT("%u"), count );
	if( 0 == count )
	{
		ShowWindow( hWorkWnd, SW_HIDE );
		MessageBox( hDlg, TEXT("ファイルが一つも選択されていないのです。\r\nこのままだと使えないのです。"), TEXT("あぅあぅ"), MB_OK | MB_ICONERROR );
		return E_ABORT;
	}

	TreeLoadNodeProc( hDlg, hTvWnd, hItem, 1 );

	Edit_GetText( GetDlgItem(hDlg,IDE_PRTREE_DIR), atTgtDir, MAX_PATH );

	//	ツリーデータの入れ替え
	SqlTransactionOnOff( TRUE );	//	トランザクション開始

	SqlTreeProfUpdate( NULL, atTgtDir );	//	ルートパスを変更
	SqlTreeNodeDelete(  1 );	//	ファイルから構築する場合、本体SQLの中身を空にしてから

	dCacheCnt = SqlTreeCount( 3, &dCacheMax );
	index = 0;
	for( m = 0; dCacheMax > m; m++ )
	{
		ZeroMemory( atName, sizeof(atName) );
		index = SqlTreeNodePickUpID( index, &dType, &dPrnt, atName, 0x00 );
		TRACE( TEXT("[%4u]%4u\t%4u\t%4u\t%s"), m, index, dType, dPrnt, atName );
		if( 0 >= index )	break;	//	データ無くなったら終わり

		logoa = SqlTreeNodeInsert( index, dType, dPrnt, atName );

		TreeProfProgressUp( hDlg );
	}

	ShowWindow( hWorkWnd, SW_HIDE );

	SqlTransactionOnOff( FALSE );	//	トランザクション終了

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	再帰検索で、ツリーのチェック状況を調べてキャッシュを操作
	@param[in]	hTvWnd	ツリーハンドル
	@param[in]	hNode	チェキる基点ノード
	@param[in]	bFixe	非０実際に操作　０Check状況の確認
*/
UINT TreeLoadNodeProc( HWND hDlg, HWND hTvWnd, HTREEITEM hNode, UINT bFixe )
{
	TCHAR		atName[MAX_PATH];
	INT			param;
	UINT		dRslt, dType, dPrID, count = 0;
	HTREEITEM	hItem;
	TVITEM		stItem;

	do
	{
		//	該当するアイテムＩＤを引っ張って
		ZeroMemory( &stItem, sizeof(TVITEM) );
		stItem.mask  = TVIF_HANDLE | TVIF_PARAM | TVIF_TEXT;
		stItem.hItem      = hNode;
		stItem.pszText    = atName;
		stItem.cchTextMax = MAX_PATH;
		TreeView_GetItem( hTvWnd, &stItem );
		param = stItem.lParam;

		//	チェック状況を確認
		dRslt = TreeView_GetCheckState( hTvWnd, hNode );
		TRACE( TEXT("Node:[%d][%d]%s"), dRslt, param, atName );

		if( bFixe )
		{
			//	チェックが無かったら
			if( !(dRslt)  ){	SqlTreeCacheDelID( param  );	}
		}
		else
		{	//	入力値を超えた値なので、目標をゲットするには−１する
			SqlTreeNodePickUpID( param-1, &dType, &dPrID, atName, 0x00 );
			//	チェックがあり、ファイルである場合
			if( dRslt && (FILE_ATTRIBUTE_NORMAL==dType) ){	count++;	}
		}

		hItem = TreeView_GetChild( hTvWnd , hNode );	//	子ノードを確認

		//	存在してたら下の階層をチェック
		if( hItem ){	count += TreeLoadNodeProc( hDlg, hTvWnd, hItem, bFixe );	}

		//	終わったら次にいく
		hItem = TreeView_GetNextSibling( hTvWnd, hNode );
		hNode = hItem;

		TreeProfProgressUp( hDlg );
	}
	while( hNode );

	return count;
}
//-------------------------------------------------------------------------------------------------

/*!
	パスを受け取って、先頭のでゅれくとりをコピーする
	@param[in]	ptSource	元パス・MAX_PATHであること
	@param[in]	ptSplits	先頭パスをコピーするバッファ・MAX_PATHであること
	@return		元パスの、次のディレクトリ位置
*/
LPTSTR PathSplitFirstPath( LPTSTR ptSource, LPTSTR ptSplit )
{
	UINT	d;

	if( !(ptSource) )	return NULL;
	if( NULL ==  ptSource[0] )	return NULL;

	for( d = 0; MAX_PATH > d; d++ )
	{
		if( TEXT('\\') == ptSource[d] ){	ptSplit[d] = NULL;	d++;	break;	}
		else if( NULL ==  ptSource[d] ){	ptSplit[d] = NULL;	break;	}
		else{	ptSplit[d] = ptSource[d];	}
	}

	return &(ptSource[d]);
}
//-------------------------------------------------------------------------------------------------

/*!
	文字列の次の改行までを取る
	@param[in]	ptSource	元文字列
	@param[in]	*ptNextLn	次の行の先頭
	@return		確保した文字列・freeセヨ
*/
LPTSTR StringLineGet( LPCTSTR ptSource, LPCTSTR *ptNextLn )
{
	UINT		t = 0;
	UINT_PTR	len;
	wstring		wStr;
	LPTSTR		ptDest;

	while( ptSource[t] )
	{
		if( TEXT('\r') == ptSource[t] || TEXT('\n') == ptSource[t] )	break;

		wStr += ptSource[t];
		t++;
	}

	if( TEXT('\r') == ptSource[t] || TEXT('\n') == ptSource[t] )	t++;
	if( TEXT('\r') == ptSource[t] || TEXT('\n') == ptSource[t] )	t++;
	*ptNextLn = &(ptSource[t]);

	len = wStr.size() + 1;
	ptDest = (LPTSTR)malloc( len * 2 );
	ZeroMemory( ptDest, len * 2 );
	StringCchCopy( ptDest, len, wStr.c_str() );

	return ptDest;
}
//-------------------------------------------------------------------------------------------------

/*
ツリー内からサーチ
同じ親番号を持つ者内でサーチ・同じディレクトリ内には、単一名しか入らないから、
それが同じモノだと判断できるはず

*/

#endif	//	MAA_PROFILE


/*!
	検索してリストビューに入れる
	@param[in]	hDlg	ダイヤログハンドル
*/
HRESULT MaaFindExecute( HWND hDlg )
{
	UINT	dCnt, dMax, d;
	UINT	dItem, dType, dPrntID, dOwnID;
	UINT	dDmyType, dDmyID;
	TCHAR	atPattern[MAX_PATH];
	TCHAR	atFileName[MAX_PATH], atPrntName[MAX_PATH];
	HWND	hLvWnd, hEdWnd;
	LVITEM	stLvi;

	hEdWnd = GetDlgItem( hDlg, IDE_MAA_FIND_NAME );
	hLvWnd = GetDlgItem( hDlg, IDLV_MAA_FINDED_FILE );

	ListView_DeleteAllItems( hLvWnd );

	ZeroMemory( atPattern, sizeof(atPattern) );
	GetDlgItemText( hDlg, IDE_MAA_FIND_NAME, atPattern, MAX_PATH );

	dCnt = SqlTreeCount( 1, &dMax );

	dOwnID = 0;
	for( d = 0; dMax > d; d++ )
	{
		dOwnID = SqlTreeFileSearch( atPattern, dOwnID );	//	ヒットを確認
		if( 0 == dOwnID )	break;	//	それ以上無いようなら終わり

		ZeroMemory( atFileName, sizeof(atFileName) );
		dType   = 0;
		dPrntID = 0;

		//	該当ＩＤの内容を確認
		SqlTreeNodePickUpID( dOwnID, &dType, &dPrntID, atFileName, 0x11 );
		if( FILE_ATTRIBUTE_NORMAL == dType )
		{
			//	引っ張った内容ファイル名をリストビューに表示
			dItem = ListView_GetItemCount( hLvWnd );

			ZeroMemory( &stLvi, sizeof(stLvi) );
			stLvi.iItem = dItem;

			stLvi.mask     = LVIF_TEXT | LVIF_PARAM;
			stLvi.pszText  = atFileName;
			stLvi.lParam   = dOwnID;
			stLvi.iSubItem = 0;
			ListView_InsertItem( hLvWnd, &stLvi );

			SqlTreeNodePickUpID( dPrntID, &dDmyType, &dDmyID, atPrntName, 0x11 );

			stLvi.mask     = LVIF_TEXT;
			stLvi.pszText  = atPrntName;
			stLvi.iSubItem = 1;
			ListView_SetItem( hLvWnd, &stLvi );
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


/*!
	MAA検索ダイヤログのノーティファイメッセージの処理
	@param[in]	hDlg		ダイヤログハンドル
	@param[in]	idFrom		NOTIFYを発生させたコントロールのＩＤ
	@param[in]	pstNmhdr	NOTIFYの詳細
	@return		処理した内容とか
*/
INT_PTR MaaFindOnNotify( HWND hDlg, INT idFrom, LPNMHDR pstNmhdr )
{
	HWND	hLvWnd;
	INT		iItem, nmCode;
	LPNMLISTVIEW	pstNmLv;
	LVITEM			stLvi;
	HTREEITEM		hTgtItem;

	if( IDLV_MAA_FINDED_FILE == idFrom )
	{
		pstNmLv = (LPNMLISTVIEW)pstNmhdr;

		hLvWnd = pstNmLv->hdr.hwndFrom;
		nmCode = pstNmLv->hdr.code;

		//	選択されてる項目を確保
		iItem = ListView_GetNextItem( hLvWnd, -1, LVNI_ALL | LVNI_SELECTED );

		if( 0 >  iItem )	return FALSE;	//	未選択状態なら何もしない

		//	ダブルクルックであった場合
		if( NM_DBLCLK == nmCode )
		{
			ZeroMemory( &stLvi, sizeof(stLvi) );
			stLvi.mask     = LVIF_PARAM;
			stLvi.iItem    = iItem;
			stLvi.iSubItem = 0;
			ListView_GetItem( hLvWnd, &stLvi );

			hTgtItem = MaaSelectIDfile( hDlg, stLvi.lParam );	//	SqlID渡して開くようにする
			//	ツリーのを選択状態にしている

			if( hTgtItem )
			{
				SetForegroundWindow( ghMaaWnd );

				//	ここで、タブ選択からチェインさせればいい
				TabMultipleCtrlFromFind( ghMaaWnd );
				//AaTitleClear(  );	//	ここでクルヤーせないかん
				//TreeSelItemProc( ghMaaWnd, hTgtItem, 0 );	//	渡すハンドル、MAA窓のハンドルにしておかないとまずい？
			}
		}
	}

	return TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	検索ダイヤログのプロシージャ
	@param[in]	hDlg	ダイヤログハンドル
	@param[in]	message	ウインドウメッセージの識別番号
	@param[in]	wParam	追加の情報１
	@param[in]	lParam	追加の情報２
	@retval 0	メッセージは処理していない
	@retval no0	なんか処理された
*/
INT_PTR CALLBACK TreeMaaFindDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	HWND	hWorkWnd;
	UINT	id;
//	HWND	hWndChild;
	LVCOLUMN	stLvColm;


	switch( message )
	{
		default:	break;

		case WM_INITDIALOG:
			hWorkWnd = GetDlgItem( hDlg, IDLV_MAA_FINDED_FILE );
			ListView_SetExtendedListViewStyle( hWorkWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
			ZeroMemory( &stLvColm, sizeof(LVCOLUMN) );
			stLvColm.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			stLvColm.fmt = LVCFMT_LEFT;
			stLvColm.pszText = TEXT("ファイル名");	stLvColm.cx = 250;	stLvColm.iSubItem = 0;	ListView_InsertColumn( hWorkWnd, 0, &stLvColm );
			stLvColm.pszText = TEXT("所属");		stLvColm.cx = 250;	stLvColm.iSubItem = 1;	ListView_InsertColumn( hWorkWnd, 1, &stLvColm );
			SetFocus( GetDlgItem(hDlg,IDE_MAA_FIND_NAME) );
			return (INT_PTR)FALSE;

		case WM_COMMAND:
			id = LOWORD(wParam);
			hWorkWnd = GetDlgItem( hDlg, IDE_FIND_TEXT );
			switch( id )
			{
				case IDCANCEL:	DestroyWindow( hDlg );	return (INT_PTR)TRUE;
				case IDOK:		MaaFindExecute( hDlg );	return (INT_PTR)TRUE;	//	検索する

				case IDM_PASTE:	SendMessage( hWorkWnd, WM_PASTE, 0, 0 );	return (INT_PTR)TRUE;
				case IDM_COPY:	SendMessage( hWorkWnd, WM_COPY,  0, 0 );	return (INT_PTR)TRUE;
				case IDM_CUT:	SendMessage( hWorkWnd, WM_CUT,   0, 0 );	return (INT_PTR)TRUE;
				case IDM_UNDO:	SendMessage( hWorkWnd, WM_UNDO,  0, 0 );	return (INT_PTR)TRUE;

				default:	break;
			}
			break;

		case WM_CLOSE:	DestroyWindow( hDlg );	ghMaaFindDlg = NULL;	return (INT_PTR)TRUE;

		case WM_DESTROY:	return (INT_PTR)TRUE;

		case WM_NOTIFY:		MaaFindOnNotify( hDlg, (INT)(wParam), (LPNMHDR)(lParam) );	return (INT_PTR)TRUE;

	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------　λ...

/*!
	MAAファイル検索窓の処理
	@param[in]	hWnd	ウインドウハンドル
*/
HRESULT TreeMaaFileFind( HWND hWnd )
{
	HINSTANCE	hInst;

	hInst = GetModuleHandle( NULL );


	if( ghMaaFindDlg )
	{
		SetForegroundWindow( ghMaaFindDlg );
		return S_OK;
	}

	ghMaaFindDlg = CreateDialogParam( hInst, MAKEINTRESOURCE(IDD_FIND_MAA_DLG), hWnd, TreeMaaFindDlgProc, 0 );

	ShowWindow( ghMaaFindDlg, SW_SHOW );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


