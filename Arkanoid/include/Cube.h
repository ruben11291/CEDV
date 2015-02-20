#ifndef _CUBEH_
#define _CUBEH_

#include <Ogre.h>
#include <OgreBulletDynamicsRigidBody.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>
#include "Shapes/OgreBulletCollisionsSphereShape.h"	
using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

class Cube{

	public:
	Cube();
	Cube( Entity * ,  SceneNode* ,  OgreBulletDynamics::RigidBody* ,  OgreBulletCollisions::CollisionShape* 
);	
	
	~Cube();
	SceneNode& getSceneNode() const;
	Entity& getEntity() const;
	OgreBulletDynamics::RigidBody& getRigidBody() const;
	OgreBulletCollisions::CollisionShape& getShape() const;
	Cube& operator=(const Cube&);
	Cube(const Cube&);
	int getType();
	void setType(int);
	int getRemaining();
	void setRemaining(int);

	private:
	int type;
	int _remaining;
	Ogre::SceneNode * sceneNode;
	Ogre::Entity * entity;
	OgreBulletDynamics::RigidBody * rigidBody;
	OgreBulletCollisions::CollisionShape * shape;

};

#endif
