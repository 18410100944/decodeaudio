#include "decoder.h"

decoder::decoder(/* args */):
_fmt_ctx(nullptr),
_codec(nullptr),
_c(nullptr),
_CPcmCallback(nullptr),
_video_index(-1),
_audio_index(-1){
    av_register_all();
    avformat_network_init();
}

decoder::~decoder(){
    clear();
}

int decoder::init(){
    if (avformat_open_input(&_fmt_ctx, _url.c_str(), NULL, NULL) < 0) {
        return false;
    }

    if (avformat_find_stream_info(_fmt_ctx, nullptr) < 0) {
        return false;
    }

    for (int i = 0; i < _fmt_ctx->nb_streams; i++) {
        if (_fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            _video_index = i;
            continue;
        }

        if (_fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            _audio_index = i;
            continue;
        }
    }

    _c = _fmt_ctx->streams[_audio_index]->codec; 
    this->_codec = avcodec_find_decoder(_c->codec_id); 
    if (!_codec) {
        return false; //Codec not found
    }

    if (avcodec_open2(_c, _codec, NULL) < 0) {
        avcodec_free_context(&_c); //free it
        this->_c = nullptr;
        return false; //Could not open codec
    }

    return 0;
}

void decoder::setUrl(std::string url){
    this->_url=url;
}

std::string & decoder::getUrl(){
    return this->_url;
}

void decoder::setCallback(CPcmCallback _CPcmCallback){
    this->_CPcmCallback=_CPcmCallback;
}

void decoder::run(){
    AVFrame *decoded_frame = NULL;
    if (!(decoded_frame = av_frame_alloc())) {
        return ;
    }

    AVPacket pkt={0};
    while (true) {
        if (av_read_frame(_fmt_ctx, &pkt) < 0) {
            av_packet_unref(&pkt);
            break;
        }

        if(pkt.stream_index!=_audio_index){
            av_packet_unref(&pkt);
            continue;
        }

        decode(_c, &pkt, decoded_frame);
        av_packet_unref(&pkt);
    }
    
    av_frame_free(&decoded_frame);
    av_packet_unref(&pkt);
}

bool decoder::clear(){
    if (_c!=NULL){
        avcodec_close(_c);
    }

    if (_fmt_ctx != nullptr) {
        if (!(_fmt_ctx->flags & AVFMT_NOFILE)){
            avio_close(_fmt_ctx->pb);
            _fmt_ctx->pb= nullptr;
        }

        avformat_close_input(&_fmt_ctx);
        _fmt_ctx = nullptr;
    }
    
    return true;
}



void decoder::decode(AVCodecContext *dec_ctx, AVPacket *pkt, AVFrame *frame)
{
    int ret;
    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0) {
        return ;
    }

    while (ret >= 0) {
        av_frame_unref(frame);
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
            return;
        } 

        else if (ret < 0) {
            return;
        }

        int buff_size = av_samples_get_buffer_size(frame->linesize,frame->channels,frame->nb_samples,dec_ctx->sample_fmt,0);
        if (_CPcmCallback==nullptr){
            return ;
        }

        _CPcmCallback((char *)(frame->data[0]),buff_size,frame->pts);
    }
}

