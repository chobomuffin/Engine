#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "../EntityComponent.h"
#include <freetype-gl.h>
#include "../Mesh.h"

class TextRenderer : public EntityComponent
{
public:
  TextRenderer();
  virtual ~TextRenderer(void);

  virtual void update(int delta);
  virtual void render(Shader *shader);

private:
  Mesh *mesh;
  texture_atlas_t *atlas;
};

#endif
