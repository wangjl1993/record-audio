#include <fstream>
#include <vector>
#include "record.h"

// 定义常量
const int SAMPLE_RATE = 44100;
const int RECORD_CHANNELS = 2;
const int BITS_PER_SAMPLE = 16;
const int BUFFER_SIZE = 1024;

// OpenSL ES相关对象
SLObjectItf engineObject = nullptr;
SLEngineItf engineEngine = nullptr;
SLObjectItf recorderObject = nullptr;
SLRecordItf recorderRecord = nullptr;
SLAndroidSimpleBufferQueueItf bufferQueue = nullptr;

// 音频数据存储
std::vector<int16_t> audioData;
bool isRecording = false;

// 回调函数，用于填充缓冲区
void bufferCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    if (isRecording) {
        // 添加更多数据到音频缓冲区
        audioData.insert(audioData.end(), (int16_t *)context, (int16_t *)context + BUFFER_SIZE/2);
        
        // 继续录音
        (*bq)->Enqueue(bq, context, BUFFER_SIZE);
    }
}

// 初始化OpenSL ES引擎和录音器
void startRecording() {
    SLresult result;

    // 创建引擎
    result = slCreateEngine(&engineObject, 0, nullptr, 0, nullptr, nullptr);
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);

    // 配置麦克风作为音源
    SLDataLocator_IODevice loc_dev = {SL_DATALOCATOR_IODEVICE, SL_IODEVICE_AUDIOINPUT,
                                      SL_DEFAULTDEVICEID_AUDIOINPUT, nullptr};
    SLDataSource audioSrc = {&loc_dev, nullptr};

    // 配置PCM输出格式
    SLDataLocator_AndroidSimpleBufferQueue loc_bq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, (SLuint32)RECORD_CHANNELS, SL_SAMPLINGRATE_44_1,
                                   SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
                                   SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT, SL_BYTEORDER_LITTLEENDIAN};
    SLDataSink audioSnk = {&loc_bq, &format_pcm};

    // 创建录音器
    const SLInterfaceID id[1] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};
    result = (*engineEngine)->CreateAudioRecorder(engineEngine, &recorderObject, &audioSrc, &audioSnk, 1, id, req);
    result = (*recorderObject)->Realize(recorderObject, SL_BOOLEAN_FALSE);
    result = (*recorderObject)->GetInterface(recorderObject, SL_IID_RECORD, &recorderRecord);
    result = (*recorderObject)->GetInterface(recorderObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &bufferQueue);

    // 注册回调
    static int16_t buffer[BUFFER_SIZE];
    result = (*bufferQueue)->RegisterCallback(bufferQueue, bufferCallback, buffer);

    // 启动录音
    audioData.clear();
    isRecording = true;
    result = (*recorderRecord)->SetRecordState(recorderRecord, SL_RECORDSTATE_RECORDING);
    result = (*bufferQueue)->Enqueue(bufferQueue, buffer, BUFFER_SIZE);
}

void stopRecording(std::string &filename) {
    SLresult result;

    // 停止录音
    isRecording = false;
    result = (*recorderRecord)->SetRecordState(recorderRecord, SL_RECORDSTATE_STOPPED);

    // 销毁录音器
    if (recorderObject != nullptr) {
        (*recorderObject)->Destroy(recorderObject);
        recorderObject = nullptr;
        recorderRecord = nullptr;
        bufferQueue = nullptr;
    }
    // writeToWav(filename, audioData);
    writeToMp3(filename, audioData);
}

void writeToWav(std::string &filename, std::vector<int16_t> &audioData) {
    // 写入WAV文件
    std::ofstream outFile(filename, std::ios::binary);
    
    // 写入WAV文件头
    uint32_t chunkSize = 36 + audioData.size() * sizeof(int16_t);
    uint32_t subChunk1Size = 16;  // PCM
    uint32_t subChunk2Size = audioData.size() * sizeof(int16_t);
    
    // RIFF Header
    outFile.write("RIFF", 4);
    outFile.write(reinterpret_cast<const char *>(&chunkSize), 4);
    outFile.write("WAVE", 4);

    // FMT Subchunk
    outFile.write("fmt ", 4);
    outFile.write(reinterpret_cast<const char *>(&subChunk1Size), 4);
    outFile.write("\x01\x00", 2);  // AudioFormat (1 = PCM)
    outFile.write(reinterpret_cast<const char *>(&RECORD_CHANNELS), 2);
    outFile.write(reinterpret_cast<const char *>(&SAMPLE_RATE), 4);
    uint32_t byteRate = SAMPLE_RATE * RECORD_CHANNELS * BITS_PER_SAMPLE / 8;
    outFile.write(reinterpret_cast<const char *>(&byteRate), 4);
    uint16_t blockAlign = RECORD_CHANNELS * BITS_PER_SAMPLE / 8;
    outFile.write(reinterpret_cast<const char *>(&blockAlign), 2);
    outFile.write(reinterpret_cast<const char *>(&BITS_PER_SAMPLE), 2);

    // Data Subchunk
    outFile.write("data", 4);
    outFile.write(reinterpret_cast<const char *>(&subChunk2Size), 4);
    outFile.write(reinterpret_cast<const char *>(audioData.data()), subChunk2Size);

    outFile.close();
}

void writeToMp3(std::string &filename, std::vector<int16_t> &audioData){
    // 初始化LAME
    lame_t lame = lame_init();
    lame_set_in_samplerate(lame, SAMPLE_RATE);
    lame_set_num_channels(lame, RECORD_CHANNELS);
    lame_set_quality(lame, 5); // 5是中等质量，0是最高质量
    lame_init_params(lame);

    // 打开MP3文件
    std::string mp3Filename = filename + ".mp3";
    std::ofstream outFile(mp3Filename, std::ios::binary);

    // 分配缓冲区
    int mp3BufferSize = 1.25 * audioData.size() + 7200; // 根据LAME文档建议
    std::vector<unsigned char> mp3Buffer(mp3BufferSize);

    // 进行编码
    int numSamples = audioData.size();
    int encodedBytes = lame_encode_buffer_interleaved(
        lame,
        audioData.data(),
        numSamples / RECORD_CHANNELS,
        mp3Buffer.data(),
        mp3BufferSize
    );

    // 写入MP3文件
    outFile.write(reinterpret_cast<const char*>(mp3Buffer.data()), encodedBytes);

    // 完成编码
    encodedBytes = lame_encode_flush(lame, mp3Buffer.data(), mp3BufferSize);
    outFile.write(reinterpret_cast<const char*>(mp3Buffer.data()), encodedBytes);

    // 关闭文件
    outFile.close();

    // 清理LAME
    lame_close(lame);
}

