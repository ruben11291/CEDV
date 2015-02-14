#include "Cube.h"

Cube::Cube(){
}

Cube::Cube( Entity * cube,  SceneNode* node,  OgreBulletDynamics::RigidBody* rigidbody,  OgreBulletCollisions::CollisionShape* sh):sceneNode(node),entity(cube), rigidBody(rigidbody),shape(sh),type(0) {
 
}

Cube::~Cube(){
  delete rigidBody;
  delete shape;
  Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager")->destroySceneNode(sceneNode);
}

SceneNode& Cube::getSceneNode() const{
  return *sceneNode;
}

Entity& Cube::getEntity() const{
  return *entity;
}

OgreBulletDynamics::RigidBody& Cube::getRigidBody() const{
  return *rigidBody;
}
OgreBulletCollisions::CollisionShape& Cube::getShape() const{
  return *shape;
}

int Cube::getType(){
  return type;
}


Cube& Cube::operator=(const Cube& cub){
  this->sceneNode = new Ogre::SceneNode(cub.getSceneNode());
  this->entity = new Ogre::Entity(cub.getEntity());
  this->rigidBody = new OgreBulletDynamics::RigidBody(cub.getRigidBody());
  this->shape = new OgreBulletCollisions::CollisionShape(cub.getShape());
  this->type= cub.type;
  return *this;
}

Cube::Cube(const Cube& cub){
  sceneNode = new Ogre::SceneNode(cub.getSceneNode());
  entity = new Ogre::Entity(cub.getEntity());
  rigidBody = new OgreBulletDynamics::RigidBody(cub.getRigidBody());
  shape = new OgreBulletCollisions::CollisionShape(cub.getShape());
  this->type = cub.type;
}

void Cube::setType(int& type){
  this->type = type;
}
