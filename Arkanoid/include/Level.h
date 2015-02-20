#ifndef _LEVELH_
#define _LEVELH_

#include <Ogre.h>
#include <OgreBulletDynamicsRigidBody.h>
#include "OgreBulletCollisionsObject.h"
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include <Shapes/OgreBulletCollisionsStaticPlaneShape.h>

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
  int detectCollision();
  int getPuntuation();
  bool isWin();
  bool isLose();

 private:
  SceneNode * _ballNode, * _orionNode, *_scenarioNode, *_latLeftNode, *_latRightNode, *_latBackNode;
  OgreBulletDynamics::RigidBody* _ballbody;
  btScalar _ballVelocity;
  Section *_section;
  OgreBulletDynamics::DynamicsWorld *_world;
  OgreBulletCollisions::DebugDrawer * _debugDrawer;
  Ogre::SceneManager *_sceneMgr;
  std::deque<OgreBulletCollisions::CollisionShape*> _shapes;
  std::deque<OgreBulletDynamics::RigidBody*> _bodies;
  std::deque<OgreBulletCollisions::TriangleMeshCollisionShape*> _trimesh;
  int _puntuation;
  
  void worldCreation();
  void scenarioCreation();
  void orionCreation();
  void ballCreation(Ogre::Vector3);
  void createCollision(Ogre::SceneNode * node, Ogre::Entity * ent, OgreBulletDynamics::DynamicsWorld* world, std::string str);
  bool collisionInCubes( OgreBulletCollisions::Object* element);
  
};

#endif
