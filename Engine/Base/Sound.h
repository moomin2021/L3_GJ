#pragma once
#include <xaudio2.h>
#include <fstream>
#include <memory>
#include <wrl.h>
#include <map>

// チャンクヘッダ
struct ChunkHeader {
	char id[4];		// チャンク毎のID
	uint32_t size;	// チャンクサイズ
};

// RIFFヘッダチャンク
struct RiffHeader {
	ChunkHeader chunk;	// RIFF
	char type[4];		// WAVE
};

// FMTチャンク
struct FormatChunk {
	ChunkHeader chunk;	// fmt
	WAVEFORMATEX fmt;	// 波形フォーマット
};

// 音声データ
struct SoundData {
	WAVEFORMATEX wfex;	// 波形フォーマット
	BYTE* pBuffer;		// バッファの先頭アドレス
	uint32_t bufferSize;// バッファサイズ
};

class Sound
{
private:
	// エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region メンバ変数
private:
	// XAudio2エンジンのインスタンス
	static ComPtr<IXAudio2> pXAudio2_;

	// マスタリング音声
	IXAudio2MasteringVoice* pMasterVoice_ = nullptr;

	// ソース音声連想配列で保存
	static std::map<uint16_t, IXAudio2SourceVoice*> sourceVoices_;

	// ソース音声生成カウント
	static uint16_t sourceVoiceCount_;
#pragma endregion

#pragma region メンバ関数
public:
	// インスタンス取得
	static Sound* GetInstance();

	// 初期化処理
	void Initialize();

	// サウンド読み込み
	static uint16_t LoadWave(const char* filename);

	// サウンドの再生
	static void Play(uint16_t sourceVoiceKey);

	// サウンドの停止
	static void Stop(uint16_t sourceVoiceKey);

	// サウンドの音量調節
	static void SetVolume(uint16_t sourceVoiceKey, float volumeValue);

private:
	// コンストラクタ
	Sound();
#pragma endregion
};