//========================================
// 
// サウンド処理
// Author : AKIRA TANAKA
// |
// Arrange : RIKU NISHIMURA
//			 石原颯馬
// 
//========================================
// *** sound.cpp ***
//========================================
#include "sound.h"
#include "setting.h"
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// サウンドのデータファイルの相対パス
#define SOUND_DATA_FILE_PATH	"data\\GAMEDATA\\SOUND\\SOUND_DATA.txt"

// サウンドの最大数
#define SOUND_MAX	(64)

//****************************************
// プロトタイプ宣言
//****************************************
// チャンクのチェック
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);

// チャンクデータの読み込み
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//****************************************
// グローバル変数
//****************************************
IXAudio2				*g_pXAudio2			// XAudio2オブジェクトへのインターフェイス
						= NULL;
IXAudio2MasteringVoice	*g_pMasteringVoice	// マスターボイス
						= NULL;
IXAudio2SourceVoice		*g_apSourceVoice	// ソースボイス
						[SOUND_MAX]
						= {};
BYTE					*g_apDataAudio		// オーディオデータ
						[SOUND_MAX]
						= {};
DWORD					g_aSizeAudio		// オーディオデータサイズ
						[SOUND_MAX]
						= {};
float					g_aSoundVolume		// 音量
						[SOUND_TYPE_MAX]
						= { 1.0f,1.0f };
float					g_aSettingVolume	// 設定の音量
						[SOUND_TYPE_MAX];
SOUNDINFO				g_aSoundInfo		// サウンドの情報
						[SOUND_MAX];
int						g_nSoundNum;		// サウンドの数

//========== *** サウンドの情報を取得 ***
SOUNDINFO *GetSoundInfo(void)
{
	return g_aSoundInfo;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// CheckChunk関数 - チャンクのチェック -
//========================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//========================================
// ReadChunkData関数 - チャンクデータの読み込み -
//========================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadSound関数 - サウンドの読み込み処理 -
//========================================
void LoadSound(void)
{
	FILE				*pFile;		// ファイルポインタ
	char				aDataSearch	// データ検索用
						[TXT_MAX];
	SOUNDINFO			*pSoundInfo	// サウンドの情報のポインタ
						= g_aSoundInfo;

	// サウンド数を初期化
	g_nSoundNum = 0;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(SOUND_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if (!strcmp(aDataSearch, "END"))	{ fclose(pFile); break; }	// 読み込みを終了
		if (!strcmp(aDataSearch, "SOUND"))	{
			fscanf(pFile, "%s", &pSoundInfo->aFileName);	// ファイル名
			fscanf(pFile, "%d", &pSoundInfo->nCntLoop);		// ループカウント
			fscanf(pFile, "%f", &pSoundInfo->fVolume);		// 音量
			fscanf(pFile, "%d", &pSoundInfo->type);			// 種類
			pSoundInfo++;	// ポインタを進める
			g_nSoundNum++;	// サウンド数を加算
		}
	}
}

//========================================
// InitSound関数 - サウンドの初期化処理 -
//========================================
HRESULT InitSound(HWND hWnd)
{
	// 設定音量を読み込む
	g_aSettingVolume[SOUND_TYPE_BGM] = (float)GetSetting()->nBGMVolume / (float)VOLUME_STAGE;
	g_aSettingVolume[SOUND_TYPE_SE] = (float)GetSetting()->nSEVolume / (float)VOLUME_STAGE;

	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if(g_pXAudio2 != NULL)
		{
			// XAudio2オブジェクトの開放
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for(int nCntSound = 0; nCntSound < g_nSoundNum; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(g_aSoundInfo[nCntSound].aFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			char aString[TXT_MAX];
			sprintf(aString, "サウンドデータファイルの生成に失敗！(1) (カウント%d)", nCntSound);
			MessageBox(hWnd, aString, "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// ソースボイスの生成
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = g_aSoundInfo[nCntSound].nCntLoop;

		// オーディオバッファの登録
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//========================================
// UninitSound関数 - サウンドの終了処理 -
//========================================
void UninitSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < g_nSoundNum; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);
	
			// ソースボイスの破棄
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;
	
			// オーディオデータの開放
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// マスターボイスの破棄
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	
	if(g_pXAudio2 != NULL)
	{
		// XAudio2オブジェクトの開放
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//========================================
// PlaySound関数 - セグメント再生(再生中なら停止) -
//========================================
HRESULT PlaySound(int nSoundIdx)
{
	if (nSoundIdx == -1) 
	{// サウンド番号が-1の時、
		return S_OK;	// 処理を終了する
	}

	if (g_aSoundInfo[nSoundIdx].type == SOUND_TYPE_BGM) 
	{// サウンドの種類がBGMだった時、
		StopSoundType(SOUND_TYPE_BGM);	// BGMを一括停止する
	}

	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[nSoundIdx];
	buffer.pAudioData = g_apDataAudio[nSoundIdx];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aSoundInfo[nSoundIdx].nCntLoop;

	// 状態取得
	g_apSourceVoice[nSoundIdx]->GetState(&xa2state);

	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		g_apSourceVoice[nSoundIdx]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[nSoundIdx]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	g_apSourceVoice[nSoundIdx]->SubmitSourceBuffer(&buffer);

	// 音量を設定
	g_apSourceVoice[nSoundIdx]->SetVolume(g_aSoundInfo[nSoundIdx].fVolume * g_aSoundVolume[g_aSoundInfo[nSoundIdx].type] * g_aSettingVolume[g_aSoundInfo[nSoundIdx].type]);

	// 再生
	g_apSourceVoice[nSoundIdx]->Start(0);

	return S_OK;
}

//========================================
// StopSound - セグメント停止(ラベル指定) -
//========================================
void StopSound(int nSoundIdx)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	g_apSourceVoice[nSoundIdx]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		g_apSourceVoice[nSoundIdx]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[nSoundIdx]->FlushSourceBuffers();
	}
}

//========================================
// StopSound - セグメント停止(全て) -
//========================================
void StopSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < g_nSoundNum; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//========================================
// SoundTypeVolumeChange関数 - 種類毎に音量変更 -
//========================================
void SoundTypeVolumeChange(SOUND_TYPE type, float fVolume)
{
	// 音量を制御
	if (fVolume <= 0.0f) { fVolume = 0.0f; }
	if (fVolume >= 1.0f) { fVolume = 1.0f; }

	// 音量を設定
	g_aSoundVolume[type] = fVolume;

	for (int nCntSound = 0; nCntSound < g_nSoundNum; nCntSound++)
	{
		if (g_aSoundInfo[nCntSound].type == type)
		{// サウンドの種類が一致した時、
			// 音量を設定
			g_apSourceVoice[nCntSound]->SetVolume(g_aSoundInfo[nCntSound].fVolume * g_aSoundVolume[type] * g_aSettingVolume[type]);
		}
	}
}

//========================================
// SetSettingVolume関数 - 設定音量を設定 -
//========================================
void SetSettingVolume(SOUND_TYPE type) 
{
	// 種類に応じた設定音量を取得
	if (type == SOUND_TYPE_BGM) {
		g_aSettingVolume[type] = (float)GetSetting()->nBGMVolume / (float)VOLUME_STAGE;
	}
	else if (type == SOUND_TYPE_SE) {
		g_aSettingVolume[type] = (float)GetSetting()->nSEVolume / (float)VOLUME_STAGE;
	}
	
	// 種類毎に音量を適用
	SoundTypeVolumeChange(type, g_aSoundVolume[type]);
}

//========================================
// StopSoundType関数 - 種類毎に再生停止 -
//========================================
void StopSoundType(SOUND_TYPE type)
{
	XAUDIO2_VOICE_STATE xa2state;
	
	for (int nCntSound = 0; nCntSound < g_nSoundNum; nCntSound++)
	{
		if (g_aSoundInfo[nCntSound].type == type)
		{// サウンドの種類が一致した時、
			// 状態取得
			g_apSourceVoice[nCntSound]->GetState(&xa2state);
			if (xa2state.BuffersQueued != 0)
			{// 再生中
				// 一時停止
				g_apSourceVoice[nCntSound]->Stop(0);

				// オーディオバッファの削除
				g_apSourceVoice[nCntSound]->FlushSourceBuffers();
			}
		}
	}
}

//========================================
// PauseSound関数 - 種類毎に一時停止 -
// Author : 石原颯馬
//========================================
void PauseSound(int nSoundIdx)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	g_apSourceVoice[nSoundIdx]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		g_apSourceVoice[nSoundIdx]->Stop(0);
	}
}

//========================================
// PauseSound関数 - 種類毎に再生再開 -
// Author : 石原颯馬
//========================================
void RestartSound(int nSoundIdx)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	g_apSourceVoice[nSoundIdx]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 一時停止中
	 // 再生
		g_apSourceVoice[nSoundIdx]->Start(0);
	}
}