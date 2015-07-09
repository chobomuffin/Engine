//
//  Author: Shervin Aflatooni
//

#include "Engine.h"

#include "components/MeshRenderer.h"
#include "components/Camera.h"
#include "components/FreeMove.h"
#include "components/FreeLook.h"
#include "components/DirectionalLight.h"
#include "components/SpotLight.h"
#include "components/PointLight.h"
#include "components/Sphere.h"

#include "Plane.h"
#include "Mesh.h"
#include "Texture.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "MediaLoader.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

  #include "stb_image_write.h"

extern "C" {
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>
  #include <libswscale/swscale.h>
}

#include <stdio.h>

class CoolGame : public Game
{
public:
  virtual void init(GLManager *glManager);
  virtual void update(int delta);

private:
  Camera *primary_camera;
  TextureData *textureData;
  AVFormatContext *pFormatCtx = NULL;
  int i, videoStream;
  AVCodecContext *pCodecCtx;
  AVCodec *pCodec;
  AVFrame *pFrame;
  AVFrame *pFrameRGB;
  int numBytes;
  uint8_t *buffer;
  struct SwsContext * pSwsCtx;
  AVPacket packet;
  int frameFinished;
  int accumulator = 0;
  int frameRate;
};

void CoolGame::update(int delta)
{
  accumulator += delta;

  if (accumulator >= (1000.0 / frameRate)) {
    while (av_read_frame(pFormatCtx, &packet) >= 0) {


      if (packet.stream_index == videoStream) {
        avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

        if (frameFinished) {
          sws_scale(pSwsCtx,
                    (const uint8_t * const *) pFrame->data,
                    pFrame->linesize, 0, pCodecCtx->height,
                    pFrameRGB->data,
                    pFrameRGB->linesize);

          log_info("READING FRAME");

          frameFinished = 0;
          break;
        }

      }
    }

    log_info("UPDATE TEXTURE %p", pFrameRGB->data[0]);
    textureData->updateTexture(pFrameRGB->data[0]);

    av_free_packet(&packet);

    accumulator = 0;
  }


  Game::update(delta);
}

// CoolGame::~CoolGame() {
//  // Free the RGB image
//  av_free(buffer);
//  av_free(pFrameRGB);
//
//  // Free the YUV frame
//  av_free(pFrame);
//
//  // Close the codec
//  avcodec_close(pCodecCtx);
//
//  // Close the video file
//  avformat_close_input(&pFormatCtx);
// }

void CoolGame::init(GLManager *glManager)
{
  /////////////////  /////////////////  /////////////////  /////////////////  /////////////////  /////////////////
  /////////////////  /////////////////  /////////////////  /////////////////  /////////////////  /////////////////
  /////////////////  /////////////////  /////////////////  /////////////////  /////////////////  /////////////////
  av_register_all();

  if (avformat_open_input(&pFormatCtx, "/Users/shervinaflatooni/Downloads/Mograph_test_project/\(Footage\)/user\ replaceable\ assets/videos/movie\ 01.mov", NULL, NULL) != 0)
    exit(1);

  if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
    exit(1);

  videoStream = -1;
  for (i = 0; i < pFormatCtx->nb_streams; i++) {
    if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
      videoStream = i;
      break;
    }
  }

  if (videoStream == -1)
    exit(1);

  pCodecCtx = pFormatCtx->streams[videoStream]->codec;
  frameRate = pFormatCtx->streams[videoStream]->r_frame_rate.num / 1000;

  pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
  if (pCodec == NULL) {
    fprintf(stderr, "Unsupported codec!\n");
    exit(1);
  }

  if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    exit(1);

  pFrame = av_frame_alloc();
  pFrameRGB = av_frame_alloc();
  if (pFrameRGB == NULL)
    exit(1);



  numBytes = avpicture_get_size(PIX_FMT_RGBA, pCodecCtx->width,
                                pCodecCtx->height);
  buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));

  avpicture_fill((AVPicture *) pFrameRGB, buffer, PIX_FMT_RGBA,
                 pCodecCtx->width, pCodecCtx->height);

  pSwsCtx = sws_getContext(pCodecCtx->width,
                           pCodecCtx->height, pCodecCtx->pix_fmt,
                           pCodecCtx->width, pCodecCtx->height,
                           PIX_FMT_RGBA, SWS_FAST_BILINEAR, NULL, NULL, NULL);

  if (pSwsCtx == NULL) {
    fprintf(stderr, "Cannot initialize the sws context\n");
    exit(1);
  }


  while (av_read_frame(pFormatCtx, &packet) >= 0) {
          log_info("INITIAL FRAME");

    if (packet.stream_index == videoStream) {
      avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

      if (frameFinished) {
        sws_scale(pSwsCtx,
                  (const uint8_t * const *) pFrame->data,
                  pFrame->linesize, 0, pCodecCtx->height,
                  pFrameRGB->data,
                  pFrameRGB->linesize);

        frameFinished = 0;
        break;
      }


    }

    av_free_packet(&packet);
  }
  ///////////////////  ///////////////////  ///////////////////  ///////////////////
  textureData = new TextureData(pCodecCtx->width, pCodecCtx->height, pFrameRGB->data[0], GL_RGBA, GL_TEXTURE_2D, GL_LINEAR);
  Texture* texture = new Texture(textureData);

  MediaLoader il = MediaLoader(texture);
  il.getEntity()->getTransform().scale(glm::vec3(10, 10, 10));
  il.getEntity()->getTransform().rotate(glm::vec3(1, 0, 0), 90 * 0.0174532925);
  addToScene(il.getEntity());


  Entity *ent = new Entity();
  Camera *cam2 = new Camera(45.0f, getEngine()->getWindow()->getWidth() / (float)getEngine()->getWindow()->getHeight(), 0.1f, 100.0f);
  ent->addComponent(cam2);
  ent->getTransform().setPosition(glm::vec3(0, 0, 10));
  ent->addComponent(new FreeMove());
  ent->addComponent(new FreeLook());
  addToScene(ent);

  primary_camera = cam2;

  getEngine()->getGLManager()->setActiveCamera(primary_camera);
}

int main(int argc, char **argv){
  CoolGame game;
  Engine gameEngine(&game);

  gameEngine.start();

  return 0;
}
