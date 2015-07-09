//
//  ImageLoader.h
//  Engine
//
//  Created by Daniel J Whelan on 9/07/2015.
//
//

#ifndef __Engine__ImageLoader__
#define __Engine__ImageLoader__

#include "Entity.h"
#include <string>

class ImageLoader
{
public:
    ImageLoader(const std::string file);
    ~ImageLoader(void);
    
    Entity *getEntity(void);
    
private:
    std::string m_fileName;
    
    Entity *m_entity;
};

#endif /* defined(__Engine__ImageLoader__) */
