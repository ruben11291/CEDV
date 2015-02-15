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
  OgreBulletDynamics::DynamicsWorld * getWorld();
  void translate(Ogre::Vector3);
 private:
  SceneNode * _ballNode, * _orionNode, *_scenarioNode;
  Section *_section;
  OgreBulletDynamics::DynamicsWorld *_world;
  OgreBulletCollisions::DebugDrawer * _debugDrawer;
  Ogre::SceneManager *_sceneMgr;
  std::vector<OgreBulletCollisions::CollisionShape*> _shapes;
  std::vector<OgreBulletDynamics::RigidBody*> _bodies;
  std::vector<OgreBulletCollisions::TriangleMeshCollisionShape*> _trimesh;

  void worldCreation();
  void scenarioCreation();
  void orionCreation();
  void ballCreation(Ogre::Vector3);
  void createCollision(Ogre::SceneNode * node, Ogre::Entity * ent, OgreBulletDynamics::DynamicsWorld* world, std::string str);


};

#endif
