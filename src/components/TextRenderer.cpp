#include "TextRenderer.h"
#include <math.h>
#include "../Logger.h"

float norm(float n, int x) {
  return n / x;
}

TextRenderer::TextRenderer()
{
  float penX = 0;
  float penY = 0;
  atlas = texture_atlas_new( 1024, 1024, 1 );
  const char * filename = "/Library/Fonts/Arial.ttf";
  wchar_t *text = L"The quick brown fox jumps over the lazy dogTHE QUICK BROWN FOX JUMPS OVER THE LAZY DOG0123456789";
  texture_font_t * m_font = texture_font_new_from_file( atlas, 100, filename );
  texture_font_load_glyphs( m_font, text );
  Vertex* vertices = new Vertex[wcslen(text) * 4];
  unsigned int indices[wcslen(text) * 6];

  for(size_t i=0; i < wcslen(text); i++ )
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
        float x1  = (float)( x0 + norm(glyph->width, m_font->height) );
        float y1  = (float)( y0 - norm(glyph->height, m_font->height) );

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

  mesh = new Mesh("asdokasoaspdkasspo", vertices, wcslen(text) * 4, indices, wcslen(text) * 6);
}

TextRenderer::~TextRenderer(void)
{
  delete mesh;
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
