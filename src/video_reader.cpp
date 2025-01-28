#include "video_reader.hpp"
#include <iostream>
#include <stdexcept>

VideoReader::VideoReader(const char *filename) : m_video_stream_index{-1}
{
    m_format_ctx = avformat_alloc_context();
    if (!m_format_ctx)
        throw std::runtime_error("Failed to allocate format context");

    if (avformat_open_input(&m_format_ctx, filename, nullptr, nullptr) != 0)
    {
        std::cout << "Filename: " << filename << std::endl;
        throw std::runtime_error("Failed to open file");
    }

    bool found_video_stream = false;
    AVStream *video_stream = nullptr;

    for (uint32_t i = 0; i < m_format_ctx->nb_streams; ++i)
    {
        AVCodecParameters *codec_params = m_format_ctx->streams[i]->codecpar;

        if (codec_params->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_stream = m_format_ctx->streams[i];
            m_video_stream_index = i;
            found_video_stream = true;
            break;
        }
    }
    if (!found_video_stream)
        throw std::runtime_error("File doesn't contain video");

    AVCodecParameters *codec_params = video_stream->codecpar;
    const AVCodec *codec = avcodec_find_decoder(codec_params->codec_id);
    if (!codec)
        throw std::runtime_error("Failed to find decoder");

    m_codec_ctx = avcodec_alloc_context3(codec);

    if (!m_codec_ctx)
        throw std::runtime_error("Failed to allocate codec context");
    if (avcodec_parameters_to_context(m_codec_ctx, codec_params) < 0)
        throw std::runtime_error("Failed to initialize codec context");
    if (avcodec_open2(m_codec_ctx, codec, nullptr) < 0)
        throw std::runtime_error("Couldn't open codec");

    m_frame = av_frame_alloc();
    if (!m_frame)
        throw std::runtime_error("Failed to alloc frame");
    m_packet = av_packet_alloc();
    if (!m_packet)
        throw std::runtime_error("Failed to alloc packet");
}
VideoReader::~VideoReader()
{
    sws_freeContext(m_sws_ctx);
    avformat_close_input(&m_format_ctx);
    avcodec_free_context(&m_codec_ctx);
    av_frame_free(&m_frame);
    av_packet_free(&m_packet);
    avformat_free_context(m_format_ctx);
}
Frame VideoReader::read_frame()
{
    Frame result;

    while (av_read_frame(m_format_ctx, m_packet) >= 0)
    {
        if (m_packet->stream_index != m_video_stream_index)
        {
            av_packet_unref(m_packet);
            continue;
        }
        int response = avcodec_send_packet(m_codec_ctx, m_packet);

        if (response < 0)
        {
            std::cout << av_err2str(response) << std::endl;
            throw std::runtime_error("Failed to decode packet");
        }

        response = avcodec_receive_frame(m_codec_ctx, m_frame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
        {
            continue;
        }
        else if (response < 0)
        {
            std::cout << av_err2str(response) << std::endl;
            throw std::runtime_error("Failed to recieve frame");
        }

        av_packet_unref(m_packet);
        break;
    }

    result.width = m_frame->width;
    result.height = m_frame->height;
    result.data.resize(m_frame->width * m_frame->height * 4);
    result.time = m_frame->pts * (double)m_format_ctx->streams[m_video_stream_index]->time_base.num /
                  (double)m_format_ctx->streams[m_video_stream_index]->time_base.den;

    if (!m_sws_ctx)
    {
        m_sws_ctx = sws_getContext(m_frame->width, m_frame->height, m_codec_ctx->pix_fmt, m_frame->width,
                                   m_frame->height, AV_PIX_FMT_RGB0, SWS_BILINEAR, NULL, NULL, NULL);
    }
    if (!m_sws_ctx)
    {
        throw std::runtime_error("Couldn't initialize sw scaler\n");
    }

    uint8_t *dest[4] = {result.data.data(), NULL, NULL, NULL};
    int dest_linesize[4] = {m_frame->width * 4, 0, 0, 0};
    sws_scale(m_sws_ctx, m_frame->data, m_frame->linesize, 0, m_frame->height, dest, dest_linesize);

    return result;
}
