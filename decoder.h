
#define __STDC_CONSTANT_MACROS
#include <string>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <memory>

#ifdef __cplusplus
extern "C"{
#endif
#include "libavutil/avassert.h"
#include "libavutil/channel_layout.h"
#include "libavutil/opt.h"
#include "libavutil/mathematics.h"
#include "libavutil/timestamp.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/imgutils.h"
#ifdef __cplusplus
}
#endif

#include "callback.h"

class decoder
{
private:
    /* data */
public:
    decoder(/* args */);
    ~decoder();
public:
    int init();
    void setUrl(std::string url);
    std::string &getUrl();
    void decode(AVCodecContext *dec_ctx, AVPacket *pkt, AVFrame *frame);
    void setCallback(CPcmCallback _CPcmCallback);
    void run();
    bool clear();
 
private:
    std::string _url;
    CPcmCallback _CPcmCallback;
    AVFormatContext *_fmt_ctx = nullptr;
    AVCodec * _codec;
    AVCodecContext *_c;
    int _video_index;
    int _audio_index;
    AVFrame *frame;
};



