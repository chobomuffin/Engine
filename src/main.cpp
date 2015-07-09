//
//  Author: Shervin Aflatooni
//

#include "Engine.h"

#include "components/Camera.h"
#include "components/FreeMove.h"
#include "components/FreeLook.h"

#include "Texture.h"
#include "Logger.h"
#include "MediaLoader.h"
#include "Video.h"

#include <stdio.h>

class CoolGame : public Game
{
public:
  virtual ~CoolGame(void);
  virtual void init(GLManager *glManager);
  virtual void update(int delta);

private:
  Camera *primary_camera;
  Video *video;
  TextureData *textureData;
  int accumulator = 0;
};

CoolGame::~CoolGame(void)
{
  delete video;
}

void CoolGame::update(int delta)
{
  accumulator += delta;

  uint8_t *frame = nullptr;

  if (accumulator >= (1000.0 / video->getFrameRate()) && (frame = video->nextFrame()) != nullptr) {
    textureData->updateTexture(frame);
    accumulator = 0;
  }

  Game::update(delta);
}

void CoolGame::init(GLManager *glManager)
{
  video = new Video("/Users/shervinaflatooni/Downloads/Mograph_test_project/\(Footage\)/user\ replaceable\ assets/videos/movie\ 01.mov");
  textureData = new TextureData(video->getWidth(), video->getHeight(), video->nextFrame(), GL_RGB, GL_TEXTURE_2D, GL_LINEAR);
  Texture* texture = new Texture(textureData);

  MediaLoader il = MediaLoader(texture);
  il.getEntity()->getTransform().scale(glm::vec3(10, 10, 10));
  il.getEntity()->getTransform().rotate(glm::vec3(1, 0, 0), 90 * 0.0174532925);
  addToScene(il.getEntity());

  Entity *ent = new Entity();
  primary_camera = new Camera(45.0f, getEngine()->getWindow()->getWidth() / (float)getEngine()->getWindow()->getHeight(), 0.1f, 100.0f);
  ent->addComponent(primary_camera);
  ent->getTransform().setPosition(glm::vec3(0, 0, 10));
  ent->addComponent(new FreeMove());
  ent->addComponent(new FreeLook());
  addToScene(ent);

  getEngine()->getGLManager()->setActiveCamera(primary_camera);
}

int main(int argc, char **argv){
  CoolGame game;
  Engine gameEngine(&game);

  gameEngine.start();

  return 0;
}
