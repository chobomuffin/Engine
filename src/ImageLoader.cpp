//
//  ImageLoader.cpp
//  Engine
//
//  Created by Daniel J Whelan on 9/07/2015.
//
//

#include "ImageLoader.h"
#include "Material.h"
#include "Texture.h"
#include "Logger.h"
#include "components/MeshRenderer.h"
#include "Plane.h"

ImageLoader::ImageLoader(const std::string file)
{
    m_fileName = file;
    m_entity = new Entity();
    Texture* texture = new Texture(Asset(m_fileName));
    
    float aR = texture->getWidth() / (texture->getHeight() * 1.0f);
    
    m_entity->addComponent(new MeshRenderer((new Plane())->getMesh(), new Material(texture)));
    
    m_entity->getTransform().setScale(glm::vec3(aR, 1, 1));
}

ImageLoader::ImageLoader(Texture* texture)
{
  m_entity = new Entity();
  float aR = texture->getWidth() / (texture->getHeight() * 1.0f);
  
  m_entity->addComponent(new MeshRenderer((new Plane())->getMesh(), new Material(texture)));
  
  m_entity->getTransform().setScale(glm::vec3(aR, 1, 1));
}


ImageLoader::~ImageLoader(void)
{
}

Entity *ImageLoader::getEntity(void)
{
    return m_entity;
}