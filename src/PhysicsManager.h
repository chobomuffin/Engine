//
//  Author: Shervin Aflatooni
//

#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "components/Sphere.h"
#include "Entity.h"
#include "Ray.h"

#include <vector>

class PhysicsManager
{
public:
  PhysicsManager(void);
  ~PhysicsManager(void);

  void registerCollider(Sphere *sphere);

  Entity *pick(Ray *ray);

private:
  std::vector<Sphere *> m_colliders;
};

#endif
