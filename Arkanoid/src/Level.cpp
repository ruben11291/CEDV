#include "Level.h"

Level::Level(int difficult, Ogre::SceneManager * sceneManager){
  _sceneMgr = sceneManager;
  worldCreation();
  scenarioCreation();
  Ogre::Vector3 velocity(0,0,-1);
  ballCreation(velocity*(difficult+1));
  orionCreation();
  

  Ogre::AxisAlignedBox box_floor= _scenarioNode->getAttachedObject("left")->getBoundingBox();
  Ogre::Vector3 frb = box_floor.getCorner( AxisAlignedBox::FAR_RIGHT_BOTTOM);
  Ogre::Vector3 nrb = box_floor.getCorner( AxisAlignedBox::NEAR_RIGHT_BOTTOM);
  box_floor= _scenarioNode->getAttachedObject("right")->getBoundingBox();
  Ogre::Vector3 flb = box_floor.getCorner( AxisAlignedBox::FAR_LEFT_BOTTOM);
  Ogre::Vector3 nlb =  box_floor.getCorner( AxisAlignedBox::NEAR_LEFT_BOTTOM);
  
  _section = new Section(_world,difficult, frb , nrb , flb , nlb);
}

Level::~Level(){
  delete _section;
  Ogre::SceneManager* mgr=Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager");

  std::vector<OgreBulletCollisions::CollisionShape *>::iterator il=_shapes.begin();
  while(il!=_shapes.end()){
    delete *il;
    il++;
  }
  std::vector<OgreBulletDynamics::RigidBody*>::iterator ib=_bodies.begin();
  while(ib!=_bodies.end()){
    delete *ib;
    ib++;
  }
  std::vector<OgreBulletCollisions::TriangleMeshCollisionShape*>::iterator it=_trimesh.begin();
  while(it!=_trimesh.end()){
    delete *it;
    it++;
  }
  delete _debugDrawer;
  delete _world;  

  mgr->destroyEntity("floor");
  mgr->destroyEntity("back");
  mgr->destroyEntity("left");
  mgr->destroyEntity("right");

  mgr->destroySceneNode("debugNode");
  mgr->destroySceneNode(_ballNode);
  mgr->destroySceneNode(_orionNode);
  mgr->destroySceneNode(_scenarioNode);
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
  Ogre::Entity* ent1 = _sceneMgr->createEntity("floor", "Suelo.mesh");
  ent1->setMaterialName("suelo");
  ent1->setCastShadows(true);
  _scenarioNode->attachObject(ent1);
  
  ent1 = _sceneMgr->createEntity("under_orion", "cube.mesh");
  AxisAlignedBox boundingB = ent1->getBoundingBox();
  Vector3 size = boundingB.getSize()* Vector3(100,1,1);
  size /=2.0;
  
  OgreBulletCollisions::BoxCollisionShape* cubeShape =   new OgreBulletCollisions::BoxCollisionShape(size);
  
  OgreBulletDynamics::RigidBody *rigidbody =   new OgreBulletDynamics::RigidBody("under_orion", _world);
  rigidbody->setShape(_scenarioNode, cubeShape,
		      0 /*Restitucion*/, 0,// Friccion,
  		      0, Ogre::Vector3(0,0,0),
		      Ogre::Quaternion(0,0,0,0));
  //_scenarioNode->attachObject(ent1);
  _shapes.push_back(cubeShape);
  _bodies.push_back(rigidbody);

  rigidbody->enableActiveState();
  //  rigidbody->setKinematicObject(true);
  rigidbody->disableDeactivation();


  //Lateral left
  Ogre::Entity *ent = _sceneMgr->createEntity("left","lat_left.mesh");
  ent->setCastShadows(true);
  _scenarioNode->attachObject(ent);
  createCollision(_scenarioNode,ent,_world,std::string("left"));

 //Lateral meshes
  ent = _sceneMgr->createEntity("right","lat_right.mesh");
  ent->setCastShadows(true);
  _scenarioNode->attachObject(ent);
  createCollision(_scenarioNode,ent, _world,std::string("right"));


 //Lateral meshes
  ent = _sceneMgr->createEntity("back","lat_back.mesh");
  ent->setCastShadows(true);
  _scenarioNode->attachObject(ent);
  createCollision(_scenarioNode,ent,_world,std::string("back"));
  _sceneMgr->getRootSceneNode()->addChild(_scenarioNode);

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
  _ballbody = rigidbody;

  // rigidbody->enableActiveState();
  //rigidbody->setKinematicObject(true);
  //rigidbody->disableDeactivation();
  _shapes.push_back(ballShape);
  _bodies.push_back(rigidbody);
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
  
   rigidbody->enableActiveState();
   //  rigidbody->setKinematicObject(true);
   rigidbody->disableDeactivation();

  _shapes.push_back(cubeShape);
  _bodies.push_back(rigidbody);
}

OgreBulletDynamics::DynamicsWorld* Level::getWorld(){
  return _world;
}

void Level::translate(Ogre::Vector3 v){
  _orionNode->translate(v);
}

void Level::createCollision(Ogre::SceneNode * node,Ogre::Entity * ent,OgreBulletDynamics::DynamicsWorld* world, std::string name){
  
 //Creation of static mesh
  OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = new 
    OgreBulletCollisions::StaticMeshToShapeConverter(ent);

  OgreBulletCollisions::TriangleMeshCollisionShape *latTrimesh = 
    trimeshConverter->createTrimesh();

  OgreBulletDynamics::RigidBody *lateral = new 
    OgreBulletDynamics::RigidBody(name,world);
  lateral->setShape(node, latTrimesh, 0.8, 0.95, 0, Ogre::Vector3(0,-0.3,0), 
		       Quaternion::IDENTITY);

  delete trimeshConverter;

  lateral->enableActiveState();
  lateral->disableDeactivation();

  _trimesh.push_back(latTrimesh);
  _bodies.push_back(lateral);
}

int Level::detectCollision() {

 btCollisionWorld *bulletWorld = _world->getBulletCollisionWorld();
  int numManifolds = bulletWorld->getDispatcher()->getNumManifolds();

  for (int i=0;i<numManifolds;i++) {
    btPersistentManifold* contactManifold = 
      bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);
    btCollisionObject* obA = 
      (btCollisionObject*)(contactManifold->getBody0());
    btCollisionObject* obB = 
      (btCollisionObject*)(contactManifold->getBody1());
    

    
    Ogre::SceneNode* orion = _sceneMgr->getSceneNode("nave");
    Ogre::SceneNode* ball = _sceneMgr->getSceneNode("bola");
    Ogre::SceneNode* plane = _sceneMgr->getSceneNode("plano");

    OgreBulletCollisions::Object *orion_obj = _world->findObject(orion);
    OgreBulletCollisions::Object *ball_obj = _world->findObject(ball);

    OgreBulletCollisions::Object * limits = _world->findObject(plane);
 
    OgreBulletCollisions::Object * obOB_A = _world->findObject(obA);
    OgreBulletCollisions::Object * obOB_B = _world->findObject(obB);


  
    if(_ballbody!=NULL){
      if (((obOB_A == orion_obj) || (obOB_B == orion_obj))) {
	std::cout << _ballbody->getLinearVelocity()<<std::endl;
	//	_ballbody->setLinearVelocity(Vector3(3,0,3));
	std::cout << "CON ORION" << std::endl;
	return 1;
      }
      else if ((obOB_A== limits) && (obOB_B== limits)) {
	//   _ballbody->setLinearVelocity(_ballbody->getLinearVelocity()*-1); 
	//   std::cout << _ballbody->getLinearVelocity()<<std::endl;
	
	std::cout << "CON Limits" << std::endl;
	
	 return 1;
      }
      else{
	for(std::vector<Cube*>::iterator it=_section->getCubes().begin(); it!=_section->getCubes().end();it++){
	  OgreBulletCollisions::Object* cube =  _world->findObject(&((*it)->getSceneNode()));
	  if ((obOB_A == cube) || (obOB_B == cube)) {
	    Ogre::SceneNode* node = NULL;
	    if ((obOB_A == cube) && (obOB_A)) {
	      node = obOB_A->getRootNode(); delete obOB_A;//segmentation, hay que eliminar cubo tambien
	    }
	    else if ((obOB_B == cube) && (obOB_B)) {
	      node = obOB_B->getRootNode(); delete obOB_B;
	    }
	    if(node){
	      _sceneMgr->destroySceneNode(node);
	    }
	    //	  _ballbody->setLinearVelocity(_ballbody->getLinearVelocity()*-1);Ogre::SceneNode* node = NULL;

	    std::cout << "CON CUBE" << std::endl;
	    return 1;
	  }
	}
      } 
    }
  }

}
