#include "Level.h"

Level::Level(int id,int difficult, Ogre::SceneManager * sceneManager){
  _sceneMgr = sceneManager;
  _id=id;
  worldCreation();
  scenarioCreation();
  Ogre::Vector3 velocity(0,0,-1);
  _ballVelocity =3;
  ballCreation(velocity*_ballVelocity);
  orionCreation();
  _puntuation = 0;

  Ogre::AxisAlignedBox box_floor= _latLeftNode->getAttachedObject("left")->getBoundingBox();
  Ogre::Vector3 frb = box_floor.getCorner( AxisAlignedBox::FAR_RIGHT_BOTTOM);
  Ogre::Vector3 nrb = box_floor.getCorner( AxisAlignedBox::NEAR_RIGHT_BOTTOM);
  box_floor= _latRightNode->getAttachedObject("right")->getBoundingBox();
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
  mgr->destroySceneNode(_latLeftNode);
  mgr->destroySceneNode(_latRightNode);
  mgr->destroySceneNode(_latBackNode);
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
  _scenarioNode->translate(0,-0.3,0);
  _scenarioNode->attachObject(ent1);
    _sceneMgr->getRootSceneNode()->addChild(_scenarioNode);
   
  
  //Lateral left
  _latLeftNode = _sceneMgr->createSceneNode("latLeft");
  Ogre::Entity *ent = _sceneMgr->createEntity("left","lat_left.mesh");
  ent->setCastShadows(true);
  _latLeftNode->attachObject(ent);
  _sceneMgr->getRootSceneNode()->addChild(_latLeftNode);
  createCollision(_latLeftNode,ent,_world,std::string("left"));
  
 //Lateral meshes
  _latRightNode = _sceneMgr->createSceneNode("latRight");
  ent = _sceneMgr->createEntity("right","lat_right.mesh");
  ent->setCastShadows(true);
  _latRightNode->attachObject(ent);
  _sceneMgr->getRootSceneNode()->addChild(_latRightNode);
  createCollision(_latRightNode,ent, _world,std::string("right"));


 //Lateral meshes
  _latBackNode = _sceneMgr->createSceneNode("latBack");
  ent = _sceneMgr->createEntity("back","lat_back.mesh");
  ent->setCastShadows(true);
  _latBackNode->attachObject(ent);
  _sceneMgr->getRootSceneNode()->addChild(_latBackNode);
  createCollision(_latBackNode,ent,_world,std::string("back"));

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
  rigidbody->setAngularVelocity(Vector3::ZERO); 

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
		      0.99 /*Restitucion*/, 0.5,// Friccion,
  		      0/*Mass*/, _orionNode->getPosition(),
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
  lateral->setShape(node, latTrimesh, 0.99, 0.5, 0, Ogre::Vector3(0,-0.3,0), 
		       Quaternion::IDENTITY);

  delete trimeshConverter;



  _trimesh.push_back(latTrimesh);
  _bodies.push_back(lateral);
}

int Level::detectCollision() {

  // for (std::
 btCollisionWorld *bulletWorld = _world->getBulletCollisionWorld();
  int numManifolds = bulletWorld->getDispatcher()->getNumManifolds();
  int ret=0;
  for (int i=0;i<numManifolds;i++) {
    btPersistentManifold* contactManifold = 
      bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);
    btCollisionObject* obA = 
      (btCollisionObject*)(contactManifold->getBody0());
    btCollisionObject* obB = 
      (btCollisionObject*)(contactManifold->getBody1());

    OgreBulletCollisions::Object *orion_obj = _world->findObject(_sceneMgr->getSceneNode("nave"));
    OgreBulletCollisions::Object *ball_obj = _world->findObject( _sceneMgr->getSceneNode("bola"));

    OgreBulletCollisions::Object * latLeft = _world->findObject(_sceneMgr->getSceneNode("latLeft"));
    OgreBulletCollisions::Object * latRight = _world->findObject(_sceneMgr->getSceneNode("latRight"));
    OgreBulletCollisions::Object * latBack = _world->findObject(_sceneMgr->getSceneNode("latBack"));
      
    OgreBulletCollisions::Object * obOB_A = _world->findObject(obA);
    OgreBulletCollisions::Object * obOB_B = _world->findObject(obB);
    OgreBulletCollisions::Object * aux;

    if(_ballbody!=NULL && contactManifold->getNumContacts()){
      if((obOB_A == ball_obj || obOB_B == ball_obj) && (obOB_A && obOB_B)){

	if(!((RigidBody*)obOB_A)->isStaticObject())
	  ((RigidBody*)obOB_A)->enableActiveState();
	if(!((RigidBody*)obOB_B)->isStaticObject())
	  ((RigidBody*)obOB_B)->enableActiveState();
	
	aux = (obOB_A== ball_obj)?obOB_B:obOB_A;//it contains the other element nor ball


	//ESTO ES LO QUE HE HECHO---------
	// btManifoldPoint& pt = contactManifold->getContactPoint(0);
	// btVector3 ptA = pt.getPositionWorldOnA();
        // btVector3 ptB = pt.getPositionWorldOnB();
	// // double ptdist = pt.getDistance();
	// btVector3 average =ptA;// (ptA+ptB)/2;
	// btVector3 current = _ballbody->getBulletRigidBody()->getLinearVelocity();
	// btVector3 cm = ((RigidBody*)(aux))->getBulletRigidBody()->getCenterOfMassPosition();
	// Ogre::Vector3 baverage(average.getX(),average.getY(),average.getZ());
	// Ogre::Vector3 bcm(cm.getX(),cm.getY(),cm.getZ());
	// bcm.normalise();
	// baverage.normalise();
	// Ogre::Vector3 cm_coll = baverage-bcm;
	// //btVector3 CM_Collision = average.normalize() -cm.normalize();
	// btVector3 CM_Collision(cm_coll.x, 0, cm_coll.z);
	// btScalar angle = CM_Collision.angle(current);//obtains the angle between the velocity and the vector point_collision-centre_mass
	// btVector3 updated_velocity = current.rotate(btVector3(1,1,1),angle);//rotate in the 
	//-----------------------------

	if(aux == orion_obj){
	  std::cout << "CON ORION" << std::endl;
	  ret = 1;
	}
	else if(aux == latLeft){
	  std::cout << "CON Izquierdo" << std::endl;
	  ret= 1;

	}
	else if(aux ==  latRight){
	  std::cout << "CON Derecho" << std::endl;
	  ret= 1;

	}
	else if(aux ==  latBack){
	  std::cout << "CON Atras" << std::endl;
	  ret= 1;
	}
	
	else{
	  std::cout << "CON Cuboo" << std::endl;
	  if(collisionInCubes(aux)) 
	    ret= 1;	  
	}
	//	_ballbody->getBulletRigidBody()->setLinearVelocity(updated_velocity*_ballVelocity);
	
	_ballbody->setAngularVelocity(Vector3::ZERO); 
	btVector3 currentVelocityDirection = _ballbody->getBulletRigidBody()->getLinearVelocity();
	btScalar currentVelocity = currentVelocityDirection.length();
	if (currentVelocity < _ballVelocity)
	  {
	    currentVelocityDirection *=(float) _ballVelocity/(float)currentVelocity;
	    _ballbody->setLinearVelocity(Ogre::Vector3(currentVelocityDirection.x(), 0,currentVelocityDirection.z()));
	    
	}  
      }
    } 
  }
  return ret;
}

bool Level::collisionInCubes( OgreBulletCollisions::Object* element){
  for(std::deque<Cube*>::iterator it=_section->getCubes().begin(); it!=_section->getCubes().end();it++){
    OgreBulletCollisions::Object* cube =  _world->findObject(&((*it)->getSceneNode()));
    if (element == cube){
      switch((*it)->getType()){
      case 0://normal
	delete *it;
	std::cout << "one touch"<<std::endl;
	_section->getCubes().erase(it);
	_puntuation += 10; //adding 10 points
	break;
      case 1://fixed
	//do nothing
	break;
      case 2://two touches
	if((*it)->getRemaining() == 1){
	  std::cout << "two touches"<< std::endl;
	  delete *it;
	  _section->getCubes().erase(it);
	  _puntuation += 30; //adding 10 points
	}
	else (*it)->setRemaining((*it)->getRemaining()-1);
	break;
      default:
	exit(-1);
      }
      return true;
    }
  }
  return false;
}

int Level::getPuntuation(){
  return _puntuation;
}

bool Level::isWin(){
  return _section->isWin();
}

bool Level::isLose(){
  float tmp =_ballbody->getSceneNode()->getPosition().z;
  return tmp >= 1;
}

void Level::resetBall(){

  Ogre::Vector3 position(0,0,-1); //Have to be initialized
  btTransform transform; //Declaration of the btTransform
  transform.setIdentity(); //This function put the variable of the object to default. The ctor of btTransform doesnt do it.
  transform.setOrigin(OgreBulletCollisions::OgreBtConverter::to(position)); //Set the new position/origin
  _ballbody->getBulletRigidBody()->setWorldTransform(transform);
}
  

void Level::setInitialVelocityBall(){
   _ballbody->setLinearVelocity(Ogre::Vector3(0,0,-1)*_ballVelocity);
}

int Level::getId(){
  return _id;
}
