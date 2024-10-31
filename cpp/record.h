/*
 * @Author: jielong.wang jielong.wang@akuvox.com
 * @Date: 2024-10-25 17:16:48
 * @LastEditors: jielong.wang jielong.wang@akuvox.com
 * @LastEditTime: 2024-10-31 15:45:29
 * @FilePath: /record_voice/recorde.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#ifndef RECORD_VOICE_H_
#define RECORD_VOICE_H_

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <fstream>
#include <vector>
#include "lame.h"


// 回调函数，用于填充缓冲区
void bufferCallback(SLAndroidSimpleBufferQueueItf bq, void *context);

// 初始化OpenSL ES引擎和录音器
void startRecording();

void stopRecording(std::string &filename);

void writeToWav(std::string &filename, std::vector<int16_t> &audioData);

void writeToMp3(std::string &filename, std::vector<int16_t> &audioData);

#endif // RECORD_VOICE_H_