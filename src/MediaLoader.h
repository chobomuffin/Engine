#ifndef MEDIA_LOADER_H
#define MEDIA_LOADER_H

#include "Entity.h"
#include "Texture.h"
#include <string>

class MediaLoader
{
public:
  MediaLoader(const std::string file);
  MediaLoader(Texture* texture);
  ~MediaLoader(void);

  Entity *getEntity(void);

private:
  void initEntity(Texture *texture);
  Entity *m_entity;
};

#endif
