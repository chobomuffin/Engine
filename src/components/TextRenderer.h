#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "../EntityComponent.h"
#include <freetype-gl/freetype-gl.h>
#include "../Mesh.h"

#include <string>

class TextRenderer : public EntityComponent
{
public:
  TextRenderer(std::wstring text, std::string font = "/Library/Fonts/Arial.ttf");
  virtual ~TextRenderer(void);

  virtual void update(int delta);
  virtual void render(Shader *shader);

  void updateText(std::wstring text, std::string font = "/Library/Fonts/Arial.ttf");

private:
  void createTextMesh(std::wstring text, std::string font);

  Mesh *mesh;
  texture_atlas_t *atlas;
};

#endif
