#include "Level.h"

Level::Level(int difficult, Ogre::SceneManager * sceneManager){
  _sceneMgr = sceneManager;
  worldCreation();
  scenarioCreation();
  Ogre::Vector3 velocity(0,0,-1);
  ballCreation(velocity*difficult);
  orionCreation();
  _section = new Section(_world,difficult, 0,100,0,100);//TODO
}

Level::~Level(){
  delete _section;
  Ogre::SceneManager* mgr=Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager");
  mgr->destroySceneNode("debugNode");
  mgr->destroySceneNode(_ballNode);
  mgr->destroySceneNode(_orionNode);
  mgr->destroySceneNode(_scenarioNode);
  mgr->destroySceneNode(_ground);
}

void Level::worldCreation(){
  
  // Creacion del modulo de debug visual de Bullet ------------------
  _debugDrawer = new OgreBulletCollisions::DebugDrawer();
  _debugDrawer->setDrawWireframe(true);	 
  SceneNode *_debug = _sceneMgr->getRootSceneNode()->
    createChildSceneNode("debugNode", Vector3::ZERO);
  _debug->attachObject(static_cast <SimpleRenderable *>(_debugDrawer));
  
  
  // Creacion del mundo (definicion de los limites y la gravedad) ---
  AxisAlignedBox worldBounds = AxisAlignedBox (
					       Vector3 (-10000, -10000, -10000), 
					       Vector3 (10000,  10000,  10000));
  Vector3 gravity = Vector3(0, 0, 0);
  
  _world = new OgreBulletDynamics::DynamicsWorld(Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager"), worldBounds,gravity);
  _world->setDebugDrawer (_debugDrawer);
  _world->setShowDebugShapes (true); 

}

void Level::scenarioCreation(){
   //Floor
  _scenarioNode = _sceneMgr->createSceneNode("plano");
  Ogre::Entity* ent1 = _sceneMgr->createEntity("Suelo.mesh");
  ent1->setMaterialName("suelo");
  ent1->setCastShadows(true);
  _scenarioNode->attachObject(ent1);


  //Lateral meshes
  Ogre::Entity *ent3 = _sceneMgr->createEntity("laterales.mesh");
  ent3->setCastShadows(true);
  //_scenario->scale(1.30,1.30,1.30);
  _scenarioNode->attachObject(ent3);
  _sceneMgr->getRootSceneNode()->addChild(_scenarioNode);
  
  //Creation of static mesh
  OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = new 
    OgreBulletCollisions::StaticMeshToShapeConverter(ent3);

  OgreBulletCollisions::TriangleMeshCollisionShape *latTrimesh = 
    trimeshConverter->createTrimesh();

  OgreBulletDynamics::RigidBody *laterals = new 
    OgreBulletDynamics::RigidBody("laterals", _world);
  laterals->setShape(_scenarioNode, latTrimesh, 0.8, 0.95, 0, Ogre::Vector3(0,-0.3,0), 
		       Quaternion::IDENTITY);

  delete trimeshConverter;

}

void Level::ballCreation(Ogre::Vector3 velocity){
  
   /////////Ball creation
  _ballNode = _sceneMgr->createSceneNode("bola");
  Ogre::Entity* ball = _sceneMgr->createEntity("ball.mesh");
  ball->setMaterialName("ball");
  ball->setCastShadows(true);
  _ballNode->scale(0.3,0.3,0.3);
  _ballNode->attachObject(ball);
  Ogre::Vector3 vball(0,0,-0.65);
  _ballNode->translate(vball);
  _sceneMgr->getRootSceneNode()->addChild(_ballNode);
  ///Ball colission shape
  OgreBulletCollisions::SphereCollisionShape *ballShape = 
    new OgreBulletCollisions::SphereCollisionShape(ball->getBoundingRadius()*0.3);

  OgreBulletDynamics::RigidBody *rigidbody = new 
    OgreBulletDynamics::RigidBody("ball", _world);
  rigidbody->setShape(_ballNode, ballShape, 0.05, 0.05, 0.3, vball, 
		      Quaternion::IDENTITY);

  rigidbody->setLinearVelocity(velocity); 
}

void Level::orionCreation(){
 _orionNode= _sceneMgr->createSceneNode("nave");
  Ogre::Entity* ent2 = _sceneMgr->createEntity("Cube.mesh");
  ent2->setCastShadows(true);
  _orionNode->scale(0.2,0.2,0.2);
  // _nave->yaw(Ogre::Degree(-180));
  _orionNode->attachObject(ent2);
  _orionNode->translate(0,0.5,0);
  _sceneMgr->getRootSceneNode()->addChild(_orionNode);
  
  AxisAlignedBox boundingB = ent2->getBoundingBox();
  Vector3 size = boundingB.getSize()* Vector3(0.2,0.2,0.2);
  size /=2.0;
  OgreBulletCollisions::BoxCollisionShape* cubeShape =   new OgreBulletCollisions::BoxCollisionShape(size);
  
  OgreBulletDynamics::RigidBody *rigidbody =   new OgreBulletDynamics::RigidBody("aircraft", _world);
  rigidbody->setShape(_orionNode, cubeShape,
		      20 /*Restitucion*/, 0,// Friccion,
  		      0, _orionNode->getPosition(),
		      Ogre::Quaternion(0,0,0.5,0));
}
