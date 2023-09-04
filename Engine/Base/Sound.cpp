#include "Sound.h"
#include <cassert>

Microsoft::WRL::ComPtr<IXAudio2> Sound::pXAudio2_ = nullptr;
std::map<uint16_t, IXAudio2SourceVoice*> Sound::sourceVoices_;
uint16_t Sound::sourceVoiceCount_ = 0;

Sound* Sound::GetInstance()
{
	// インスタンス生成
	static Sound inst;

	// インスタンスを返す
	return &inst;
}

void Sound::Initialize()
{
	HRESULT result;

	// XAudioエンジンのインスタンスを作成
	result = XAudio2Create(&pXAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	// マスタリング音声を作成
	result = pXAudio2_->CreateMasteringVoice(&pMasterVoice_);
	assert(SUCCEEDED(result));
}

uint16_t Sound::LoadWave(const char* filename) {
	// ファイル入力ストリームのインスタンス
	std::ifstream file;

	// .wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);

	// ファイルオープン失敗を検出する
	assert(file.is_open());

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	// Formatチェックの読み込み
	FormatChunk format = {};

	// チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクの読み込み
	ChunkHeader data{};
	file.read((char*)&data, sizeof(data));

	// JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	// Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// ファイルを閉じる
	file.close();

	// returnする為の音声データ
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	HRESULT result;

	// ソース音声を作成
	sourceVoices_[sourceVoiceCount_] = nullptr;
	result = pXAudio2_->CreateSourceVoice(&sourceVoices_[sourceVoiceCount_], &soundData.wfex);
	assert(SUCCEEDED(result));

	// ソース音声にバッファを送信
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	result = sourceVoices_[sourceVoiceCount_]->SubmitSourceBuffer(&buf);

	sourceVoiceCount_++;

	return sourceVoiceCount_ - 1;
}

void Sound::Play(uint16_t sourceVoiceKey)
{
	HRESULT result;

	// 再生
	result = sourceVoices_[sourceVoiceKey]->Start(0);
	assert(SUCCEEDED(result));
}

void Sound::Stop(uint16_t sourceVoiceKey)
{
	HRESULT result;

	// 停止
	result = sourceVoices_[sourceVoiceKey]->Stop(0);
	assert(SUCCEEDED(result));
}

void Sound::SetVolume(uint16_t sourceVoiceKey, float volumeValue) {
	HRESULT result;

	// 音量調節
	result = sourceVoices_[sourceVoiceKey]->SetVolume(volumeValue);
	assert(SUCCEEDED(result));
}

Sound::Sound() {}