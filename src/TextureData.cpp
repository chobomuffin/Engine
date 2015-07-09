//
//  Author: Shervin Aflatooni
//

#include "TextureData.h"

TextureData::TextureData(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter)
{
  createTexture(width, height, data, textureTarget, filter);
}

TextureData::~TextureData()
{
  glDeleteTextures(1, &m_textureId);
}

void TextureData::createTexture(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter)
{
  m_width = width;
  m_height = height;
  m_textureTarget = textureTarget;

  glGenTextures(1, &m_textureId);
  glBindTexture(textureTarget, m_textureId);
  glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, filter);
  glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, filter);
  glTexImage2D(textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void TextureData::updateTexture(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter)
{
  glDeleteTextures(1, &m_textureId);

  m_width = width;
  m_height = height;
  m_textureTarget = textureTarget;
  
  glGenTextures(1, &m_textureId);
  glBindTexture(textureTarget, m_textureId);
  glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, filter);
  glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, filter);
  glTexImage2D(textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void TextureData::bind(unsigned int unit)
{
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(m_textureTarget, m_textureId);
}


int TextureData::getWidth() {
  return m_width;
}

int TextureData::getHeight() {
  return m_height;
}