#include "Level.h"

Level::Level(int difficult, Ogre::SceneManager * sceneManager){
  _sceneMgr = sceneManager;
  worldCreation();
  scenarioCreation();
  Ogre::Vector3 velocity(0,0,-1);
  _ballVelocity =3;
  ballCreation(velocity*_ballVelocity);
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

  std::deque<OgreBulletCollisions::CollisionShape *>::iterator il=_shapes.begin();
  while(il!=_shapes.end()){
    delete *il;
    il++;
  }
  std::deque<OgreBulletDynamics::RigidBody*>::iterator ib=_bodies.begin();
  while(ib!=_bodies.end()){
    delete *ib;
    ib++;
  }
  std::deque<OgreBulletCollisions::TriangleMeshCollisionShape*>::iterator it=_trimesh.begin();
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
  
  _scenarioNode = _sceneMgr->createSceneNode("plano");
  Ogre::Entity* ent1 = _sceneMgr->createEntity("floor", "Suelo.mesh");
  ent1->setMaterialName("suelo");
  ent1->setCastShadows(true);
  _scenarioNode->attachObject(ent1);
  // OgreBulletCollisions::CollisionShape *Shape;
  // Shape = new OgreBulletCollisions::StaticPlaneCollisionShape(Ogre::Vector3(0,0,-1), 0);  

  // OgreBulletDynamics::RigidBody *rigidbody =   new OgreBulletDynamics::RigidBody("under_orion", _world);
  // rigidbody->setStaticShape(Shape, 0.0, 0.8); 
  // _shapes.push_back(Shape);
  // _bodies.push_back(rigidbody);
  
  
  
  
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
  rigidbody->setShape(_ballNode, ballShape, 0, 0, 0.3, vball, 
		      Quaternion::IDENTITY);

  rigidbody->setLinearVelocity(velocity); 
  _ballbody = rigidbody;

  // rigidbody->enableActiveState();
  //rigidbody->setKinematicObject(true);
  rigidbody->disableDeactivation();
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
		      0 /*Restitucion*/, 0,// Friccion,
  		      0, _orionNode->getPosition(),
		      Ogre::Quaternion(0,0,0.5,0));
  rigidbody->setKinematicObject(true);
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
  lateral->setShape(node, latTrimesh, 0, 0, 0, Ogre::Vector3(0,-0.3,0), 
		       Quaternion::IDENTITY);

  delete trimeshConverter;



  _trimesh.push_back(latTrimesh);
  _bodies.push_back(lateral);
}

int Level::detectCollision() {

  // for (std::
 btCollisionWorld *bulletWorld = _world->getBulletCollisionWorld();
  int numManifolds = bulletWorld->getDispatcher()->getNumManifolds();

  for (int i=0;i<numManifolds;i++) {
    btPersistentManifold* contactManifold = 
      bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);
    btCollisionObject* obA = 
      (btCollisionObject*)(contactManifold->getBody0());
    btCollisionObject* obB = 
      (btCollisionObject*)(contactManifold->getBody1());

    OgreBulletCollisions::Object *orion_obj = _world->findObject(_sceneMgr->getSceneNode("nave"));
    OgreBulletCollisions::Object *ball_obj = _world->findObject( _sceneMgr->getSceneNode("bola"));

    OgreBulletCollisions::Object * limits = _world->findObject(_sceneMgr->getSceneNode("plano"));
 
    OgreBulletCollisions::Object * obOB_A = _world->findObject(obA);
    OgreBulletCollisions::Object * obOB_B = _world->findObject(obB);
    OgreBulletCollisions::Object * finish = NULL;//to get structure

    OgreBulletCollisions::Object * aux;

    if(!((RigidBody*)obOB_A)->isStaticObject())
      ((RigidBody*)obOB_A)->enableActiveState();
    if(!((RigidBody*)obOB_B)->isStaticObject())
      ((RigidBody*)obOB_B)->enableActiveState();

  
    if(_ballbody!=NULL && contactManifold->getNumContacts()){
      if(obOB_A == ball_obj || obOB_B == ball_obj){

	aux = (obOB_A== ball_obj)?obOB_B:obOB_A;//it contains the other element nor ball
	//	btVector3 massCentre = ((btRigidBody*)((RigidBody*)aux)->getBulletRigidBody())->getCenterOfMassPosition();
	//btManifoldPoint& pt = contactManifold->getContactPoint(0);
	
	//	std::cout <<"Position world BA " << pt.getPositionWorldOnB().x() <<pt.getPositionWorldOnB().y()<< pt.getPositionWorldOnB().z()<< " "<< pt.getPositionWorldOnA().x()<<pt.getPositionWorldOnA().y()<<pt.getPositionWorldOnA().z()<<std::endl;
	
	if(aux == orion_obj){
	  std::cout << _ballbody->getLinearVelocity()<<std::endl;
	  //  _ballbody->setLinearVelocity(/*getVelocity(getCenterOfMassPosition()*/);
	  std::cout << "CON ORION" << std::endl;
	  return 1;
	}
	else if(aux == limits){
	  std::cout << "CON Limits" << std::endl;
	  return 1;
	}
	// else if(aux == finish){
	//   std::cout << "CON finish" << std::endl;
	// }
	else{
	  std::cout << "CON Cuboo" << std::endl;
	  if(collisionInCubes(aux)) 
	    return 1;//checkWin();
	}
      }
    }
  }
  Ogre::Vector3 currentVelocityDirection = _ballbody->getLinearVelocity();
  btScalar currentVelocty = currentVelocityDirection.length();
  if (currentVelocty < _ballVelocity)
    {
      currentVelocityDirection *= _ballVelocity/currentVelocty;
      _ballbody->setLinearVelocity(Ogre::Vector3(currentVelocityDirection.x, 0,currentVelocityDirection.z));
    }
}

bool Level::collisionInCubes( OgreBulletCollisions::Object* element){
  for(std::deque<Cube*>::iterator it=_section->getCubes().begin(); it!=_section->getCubes().end();it++){
    OgreBulletCollisions::Object* cube =  _world->findObject(&((*it)->getSceneNode()));
    if (element == cube){
      delete *it;
      _section->getCubes().erase(it);
      return true;
    }
  }
  return false;
}

// OgreBulletDynamics::RigidBody * getBall(){
//   return _ballbody;
// }
