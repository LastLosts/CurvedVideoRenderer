#pragma once

extern "C"
{
#include <inttypes.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#include "frame.hpp"

class VideoReader
{
  public:
    VideoReader(const char *filename);
    ~VideoReader();

    VideoReader(const VideoReader &) = delete;
    VideoReader(VideoReader &&) = delete;
    VideoReader &operator=(const VideoReader &) = delete;
    VideoReader &operator=(VideoReader &&) = delete;

    Frame read_frame();

  private:
    AVFormatContext *m_format_ctx{nullptr};
    AVCodecContext *m_codec_ctx{nullptr};
    AVFrame *m_frame{nullptr};
    AVPacket *m_packet{nullptr};
    SwsContext *m_sws_ctx{nullptr};

    int m_video_stream_index;
};
