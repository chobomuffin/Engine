#include "TextRenderer.h"
#include <math.h>
#include "../Logger.h"

float norm(float n, float x) {
  return n / x;
}

TextRenderer::TextRenderer(std::wstring text, std::string font)
{
  atlas = texture_atlas_new( 1024, 1024, 4 );
  createTextMesh(text, font);
}

TextRenderer::~TextRenderer(void)
{
  texture_atlas_delete(atlas);
  delete mesh;
}

void TextRenderer::updateText(std::wstring text, std::string font)
{
  // TODO: FIND MORE EFFICENT WAY TO DO THIS..
  delete mesh;

  createTextMesh(text, font);
}

void TextRenderer::createTextMesh(std::wstring text, std::string font)
{
  float penX = 0;
  float penY = 0;

  texture_font_t * m_font = texture_font_new_from_file( atlas, 300, font.c_str() );
  texture_font_load_glyphs( m_font, (wchar_t *) text.c_str() );
  Vertex* vertices = new Vertex[text.length() * 4];
  unsigned int indices[text.length() * 6];

  for(size_t i=0; i < text.length(); i++ )
  {
    texture_glyph_t *glyph = texture_font_get_glyph( m_font, text[i] );
    if( glyph != NULL )
    {
      float kerning =  0.0f;
      if( i > 0)
      {
        kerning = texture_glyph_get_kerning( glyph, text[i-1] );
      }
      penX += norm(kerning, m_font->height);

      float x0  = (float)( penX + norm(glyph->offset_x, m_font->height) );
      float y0  = (float)( penY + norm(glyph->offset_y, m_font->height) );
      // https://github.com/rougier/freetype-gl/blob/87204894a46d7198040f27db19834c64d23f65f5/texture-font.c#L601
      // because depth is 4 to normalize multiply by 4 + 1 ish
      // this must be wrong..
      float x1  = (float)( x0 + norm(glyph->width * 4.9, m_font->height) );
      float y1  = (float)( y0 - norm(glyph->height, m_font->height) );

      log_err("[%f]", (float)glyph->width);

      float s0 = glyph->s0;
      float t0 = glyph->t0;
      float s1 = glyph->s1;
      float t1 = glyph->t1;

      vertices[(i * 4)] = Vertex(glm::vec3(x0,y0,0), glm::vec2(s0,t0), glm::vec3(0, 0, -1), glm::vec3(1, 0, 0));
      vertices[(i * 4) + 1] = Vertex(glm::vec3(x0,y1,0), glm::vec2(s0,t1), glm::vec3(0, 0, -1), glm::vec3(1, 0, 0));
      vertices[(i * 4) + 2] = Vertex(glm::vec3(x1,y1,0), glm::vec2(s1,t1), glm::vec3(0, 0, -1), glm::vec3(1, 0, 0));
      vertices[(i * 4) + 3] = Vertex(glm::vec3(x1,y0,0), glm::vec2(s1,t0), glm::vec3(0, 0, -1), glm::vec3(1, 0, 0));

      indices[(i * 6)] = (i * 4);
      indices[(i * 6) + 1] = (i * 4) + 1;
      indices[(i * 6) + 2] = (i * 4) + 2;
      indices[(i * 6) + 3] = (i * 4);
      indices[(i * 6) + 4] = (i * 4) + 2;
      indices[(i * 6) + 5] = (i * 4) + 3;

      penX += norm(glyph->advance_x, m_font->height);
    }
  }

  std::string str(text.begin(), text.end());
  mesh = new Mesh(str, vertices, text.length() * 4, indices, text.length() * 6);
  delete[] vertices;
}

void TextRenderer::update(int delta)
{
}

void TextRenderer::render(Shader *shader)
{
  shader->setUniformMatrix4f("World", parentEntity->getWorldMatrix());
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, atlas->id);
  mesh->render();
}
