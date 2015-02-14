#ifndef _LEVELH_
#define _LEVELH_

#include <Ogre.h>
#include <OgreBulletDynamicsRigidBody.h>
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include "Section.h"
using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

class Level{

 public:
  Level(int, Ogre::SceneManager *);
  ~Level();
 private:
  SceneNode * _ballNode, * _orionNode, *_scenarioNode, *_ground;
  Section *_section;
  OgreBulletDynamics::DynamicsWorld *_world;
  OgreBulletCollisions::DebugDrawer * _debugDrawer;
  Ogre::SceneManager *_sceneMgr;
  void worldCreation();
  void scenarioCreation();
  void orionCreation();
  void ballCreation(Ogre::Vector3);
};

#endif