#include <iostream>

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "glfw_context.hpp"
#include "image_renderer.hpp"
#include "libswscale/swscale.h"
#include "window.hpp"

#include "image.hpp"

#include "load_frame.hpp"

constexpr uint32_t window_width = 800;
constexpr uint32_t window_height = 800;

extern "C"
{
#include <inttypes.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

Frame load_frame(const char *filename)
{
    Frame result;

    AVFormatContext *format_ctx = avformat_alloc_context();

    if (!format_ctx)
        throw std::runtime_error("Couldn't allocate avformat context");

    if (avformat_open_input(&format_ctx, filename, nullptr, nullptr) != 0)
    {
        std::cout << "Filename: " << filename << std::endl;
        throw std::runtime_error("Failed to open file");
    }

    bool found_video_stream = false;
    AVStream *video_stream = nullptr;
    int video_stream_index = -1;

    for (uint32_t i = 0; i < format_ctx->nb_streams; ++i)
    {
        AVCodecParameters *codec_params = format_ctx->streams[i]->codecpar;

        if (codec_params->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_stream = format_ctx->streams[i];
            video_stream_index = i;
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

    AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);

    if (!codec_ctx)
        throw std::runtime_error("Failed to allocate codec context");
    if (avcodec_parameters_to_context(codec_ctx, codec_params) < 0)
        throw std::runtime_error("Failed to initialize codec context");
    if (avcodec_open2(codec_ctx, codec, nullptr) < 0)
        throw std::runtime_error("Couldn't open codec");

    AVFrame *frame = av_frame_alloc();
    if (!frame)
        throw std::runtime_error("Failed to alloc frame");
    AVPacket *packet = av_packet_alloc();
    if (!packet)
        throw std::runtime_error("Failed to alloc packet");

    uint32_t count, frame_num = 20;

    while (av_read_frame(format_ctx, packet) >= 0)
    {
        if (packet->stream_index != video_stream_index)
        {
            continue;
        }
        int response = avcodec_send_packet(codec_ctx, packet);

        if (response < 0)
        {
            std::cout << av_err2str(response) << std::endl;
            throw std::runtime_error("Failed to decode packet");
        }

        response = avcodec_receive_frame(codec_ctx, frame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
        {
            continue;
        }
        else if (response < 0)
        {
            std::cout << av_err2str(response) << std::endl;
            throw std::runtime_error("Failed to recieve frame");
        }

        av_packet_unref(packet);
        if (count == frame_num)
        {
            break;
        }
        count++;
    }

    result.width = frame->width;
    result.height = frame->height;
    result.data.resize(frame->width * frame->height * 4);

    SwsContext *sws_scaler_ctx = sws_getContext(frame->width, frame->height, codec_ctx->pix_fmt, frame->width,
                                                frame->height, AV_PIX_FMT_RGB0, SWS_BILINEAR, NULL, NULL, NULL);

    if (!sws_scaler_ctx)
    {
        throw std::runtime_error("Couldn't initialize sw scaler\n");
    }

    uint8_t *dest[4] = {result.data.data(), NULL, NULL, NULL};
    int dest_linesize[4] = {frame->width * 4, 0, 0, 0};
    sws_scale(sws_scaler_ctx, frame->data, frame->linesize, 0, frame->height, dest, dest_linesize);
    sws_freeContext(sws_scaler_ctx);

    /*for (auto &&a : result.data)*/
    /*{*/
    /*    a = 0xff;*/
    /*}*/
    /**/
    /*for (uint32_t y = 0; y < frame->height; ++y)*/
    /*{*/
    /*    for (uint32_t x = 0; x < frame->width; ++x)*/
    /*    {*/
    /*        result.data[y * frame->width * 3 + x * 3 + 0] = frame->data[0][y * frame->linesize[0] + x];*/
    /*        result.data[y * frame->width * 3 + x * 3 + 1] = frame->data[0][y * frame->linesize[0] + x];*/
    /*        result.data[y * frame->width * 3 + x * 3 + 2] = frame->data[0][y * frame->linesize[0] + x];*/
    /*    }*/
    /*}*/

    /*pgm_save(frame->data[0], frame->linesize[0], frame->width, frame->height, "./assets/prikol.pgm");*/

    avformat_close_input(&format_ctx);
    avcodec_free_context(&codec_ctx);
    av_frame_free(&frame);
    av_packet_free(&packet);
    avformat_free_context(format_ctx);

    return result;
}

void resize_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    try
    {
        GlfwContext glfw{};
        Window window{window_width, window_height, "Helo"};

        window.make_current_context();

        glfwSetFramebufferSizeCallback(window.window, resize_callback);

        if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");
            return -1;
        }

        ImageRenderer image_renderer{};

        Frame frame = load_frame("./assets/test-video.mp4");

        while (window.is_open())
        {
            image_renderer.render(frame);

            window.swap_buffers();
            glfwPollEvents();
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Exception thrown:\n" << e.what() << std::endl;
        return -1;
    }
    catch (...)
    {
        std::cout << "Unknown exception thrown terminating" << std::endl;
        return -1;
    }

    return 0;
}
