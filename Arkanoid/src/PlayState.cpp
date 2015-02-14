#include "PlayState.h"
#include "PauseState.h"

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void
PlayState::enter ()
{  std::cout << "Play state enter" << std::endl;

  _root = Ogre::Root::getSingletonPtr();
  
  _sceneMgr = _root->getSceneManager("SceneManager");
    _sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
   _sceneMgr->setShadowColour(Ogre::ColourValue(0.5,0.5,0.5));
   _sceneMgr->setAmbientLight(Ogre::ColourValue(0.9,0.9,0.9));
   _sceneMgr->setShadowTextureCount(3);
    _sceneMgr->setShadowTextureSize(512);
     
  _light = _sceneMgr->createLight("Light1");
   _light->setPosition(-5,12,2);
  _light->setType(Ogre::Light::LT_SPOTLIGHT);
  _light->setDirection(Ogre::Vector3(1,-1,0));
   _light->setSpotlightInnerAngle(Ogre::Degree(25.0f));
  _light->setSpotlightOuterAngle(Ogre::Degree(60.0f));
  _light->setSpotlightFalloff(0.0f);

  _camera = _sceneMgr->getCamera("IntroCamera");
  _camera->setPosition(Ogre::Vector3(0.07694,7.3,6.5));
  _camera->lookAt(Ogre::Vector3(0.67329,-0.12087,1.1));
  _camera->setDirection(Ogre::Vector3(-0.0213742,-0.590729,-0.806589));
  _camera->setNearClipDistance(5);
  _camera->setFarClipDistance(10000);
  _camera->setFOVy(Ogre::Degree(53.13));
      std::cout << "Play state end" << std::endl;

  SoundFXManager::getSingletonPtr()->load("bomb.wav");
  SoundFXManager::getSingletonPtr()->load("impact.wav");
  
  //World's creation
 // Creacion del modulo de debug visual de Bullet ------------------
   OgreBulletCollisions::DebugDrawer * _debugDrawer = new OgreBulletCollisions::DebugDrawer();
  _debugDrawer->setDrawWireframe(true);	 
  SceneNode *_debug = _sceneMgr->getRootSceneNode()->
    createChildSceneNode("debugNode", Vector3::ZERO);
  _debug->attachObject(static_cast <SimpleRenderable *>(_debugDrawer));


 // Creacion del mundo (definicion de los limites y la gravedad) ---
  AxisAlignedBox worldBounds = AxisAlignedBox (
    Vector3 (-10000, -10000, -10000), 
    Vector3 (10000,  10000,  10000));
  Vector3 gravity = Vector3(0, 0, 0);

  _world = new OgreBulletDynamics::DynamicsWorld(_sceneMgr,
						 worldBounds,gravity);
  _world->setDebugDrawer (_debugDrawer);

  //Floor
  _scenario = _sceneMgr->createSceneNode("plano");
  Ogre::Entity* ent1 = _sceneMgr->createEntity("Suelo.mesh");
  ent1->setMaterialName("suelo");
  ent1->setCastShadows(true);
  _scenario->attachObject(ent1);


  //Lateral meshes
  Ogre::Entity *ent3 = _sceneMgr->createEntity("laterales.mesh");
  ent3->setCastShadows(true);
  //_scenario->scale(1.30,1.30,1.30);
  _scenario->attachObject(ent3);
  _sceneMgr->getRootSceneNode()->addChild(_scenario);
  
  //Creation of static mesh
  OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = new 
    OgreBulletCollisions::StaticMeshToShapeConverter(ent3);

  OgreBulletCollisions::TriangleMeshCollisionShape *latTrimesh = 
    trimeshConverter->createTrimesh();

  OgreBulletDynamics::RigidBody *laterals = new 
    OgreBulletDynamics::RigidBody("laterals", _world);
  laterals->setShape(_scenario, latTrimesh, 0.8, 0.95, 0, Ogre::Vector3(0,-0.3,0), 
		       Quaternion::IDENTITY);

  delete trimeshConverter;


  _world->setShowDebugShapes (true); 

  /////////Ball creation
  _ball = _sceneMgr->createSceneNode("bola");
  Ogre::Entity* ball = _sceneMgr->createEntity("ball.mesh");
  ball->setMaterialName("ball");
  ball->setCastShadows(true);
  _ball->scale(0.3,0.3,0.3);
  _ball->attachObject(ball);
  Ogre::Vector3 vball(0,0,-0.65);
  _ball->translate(vball);
  _sceneMgr->getRootSceneNode()->addChild(_ball);
  ///Ball colission shape
  OgreBulletCollisions::SphereCollisionShape *ballShape = 
    new OgreBulletCollisions::SphereCollisionShape(ball->getBoundingRadius()*0.3);

  OgreBulletDynamics::RigidBody *rigidbody = new 
    OgreBulletDynamics::RigidBody("ball", _world);
  rigidbody->setShape(_ball, ballShape, 0.05, 0.05, 0.3, vball, 
		      Quaternion::IDENTITY);

  rigidbody->setLinearVelocity(Ogre::Vector3(0,0,-1)*2); 

  Ogre::Vector3 cube_scale(2,1,1);
  Ogre::SceneNode* cubo_n = _sceneMgr->createSceneNode("normal");
  Ogre::Entity* cube = _sceneMgr->createEntity("Cube2.mesh");
  cube->setMaterialName("Cube1");
  cube->setCastShadows(true);
  cubo_n->scale(cube_scale);
  cubo_n->attachObject(cube);
  _sceneMgr->getRootSceneNode()->addChild(cubo_n);

  AxisAlignedBox boundingB = cube->getBoundingBox();
  Ogre::Vector3 size = boundingB.getSize();
  size/=2.0f;
  OgreBulletCollisions::BoxCollisionShape *cubeShape = 
    new OgreBulletCollisions::BoxCollisionShape(size*cube_scale);

  rigidbody =   new OgreBulletDynamics::RigidBody("caja1", _world);
  rigidbody->setShape(cubo_n, cubeShape,
  		     20 /* Restitucion */, 0 /* Friccion */,
  		      0 /* Masa */, Ogre::Vector3(0,-0.2,-8.5) /* Posicion inicial */,
  		     Quaternion::IDENTITY /* Orientacion */);


  Ogre::SceneNode* cubo_g = _sceneMgr->createSceneNode("verde");
  cube = _sceneMgr->createEntity("Cube2.mesh");
  cube->setMaterialName("Cube2");
  cube->setCastShadows(true);
  cubo_g->scale(cube_scale);
  cubo_g->attachObject(cube);
  cubo_g->translate(-1.4,0,-8.5);
  _sceneMgr->getRootSceneNode()->addChild(cubo_g);
 
  _nave= _sceneMgr->createSceneNode("nave");
  Ogre::Entity* ent2 = _sceneMgr->createEntity("Cube.mesh");
  ent2->setCastShadows(true);
  _nave->scale(0.2,0.2,0.2);
  // _nave->yaw(Ogre::Degree(-180));
  _nave->attachObject(ent2);
  _nave->translate(0,0.5,0);
  _sceneMgr->getRootSceneNode()->addChild(_nave);
  
  boundingB = ent2->getBoundingBox();
  size = boundingB.getSize()* Vector3(0.2,0.2,0.2);
  size /=2.0;
  cubeShape =   new OgreBulletCollisions::BoxCollisionShape(size);

  rigidbody =   new OgreBulletDynamics::RigidBody("aircraft", _world);
  rigidbody->setShape(_nave, cubeShape,
		      10 /*Restitucion*/, 0,// Friccion,
  		      0, _nave->getPosition(),
		      Ogre::Quaternion(0,0,0.5,0));



  Ogre::Plane plane1(Ogre::Vector3::UNIT_Y, 0);
  Ogre::MeshManager::getSingleton().createPlane("plane1",
	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
	200,200,1,1,true,1,20,20,Ogre::Vector3::UNIT_Z);

   _ground = _sceneMgr->createSceneNode("ground");
  Ogre::Entity* groundEnt = _sceneMgr->createEntity("planeEnt", "plane1");
  groundEnt->setMaterialName("Ground");
  groundEnt->setCastShadows(false);
  _ground->attachObject(groundEnt);
  _ground->translate(0,-0.4,0);
  _sceneMgr->getRootSceneNode()->addChild(_ground);
  
  
  _raySceneQuery = _sceneMgr->createRayQuery(Ogre::Ray());
  
  _overlayManager = Ogre::OverlayManager::getSingletonPtr();
  _overlayManager->getByName("Info")->show();
  
  _overlayManager->getOverlayElement("logoGO")->hide();
  _overlayManager->getOverlayElement("logoClear")->hide();
  
  _last_time = _time_count = 0;
  _key_pressed=_pick = _end_game = false;
  _exitGame = true;
}

void
PlayState::exit ()
{ 
  _sceneMgr->destroyQuery(static_cast<Ogre::RaySceneQuery*>(_raySceneQuery));
   SoundFXManager::getSingletonPtr()->unload("bomb.wav");
   SoundFXManager::getSingletonPtr()->unload("impact.wav");

  _overlayManager->destroy(_overlayManager->getByName("Info"));
  // _overlay->hide();
  _sceneMgr->destroyLight(_light);
  _sceneMgr->destroySceneNode(_ground);
  _sceneMgr->destroySceneNode(_scenario);
  _sceneMgr->destroySceneNode(_nave);

}

void
PlayState::pause()
{  
  _pick=false;
  _overlayManager->getByName("Info")->hide();
}

void
PlayState::resume()
{
  _pick=true;
  _ground->setVisible(true);
  _overlayManager->getByName("Info")->show();
}

bool
PlayState::frameStarted
(const Ogre::FrameEvent& evt)
{
  std::stringstream mines,remaining;
  deltaT = evt.timeSinceLastFrame;
  // int fps = 1.0 / deltaT;

  Ogre::OverlayElement *oe;
  oe = _overlayManager->getOverlayElement("fpsInfo");
//   remaining << _minesweeper->getDiscovered() << "/" << (_minesweeper->getSquares()*_minesweeper->getSquares()*6)-_minesweeper->getTotalMines();
  oe->setCaption(remaining.str());
  
  oe = _overlayManager->getOverlayElement("minesinf");
//   mines << _minesweeper->getFlags() << "/" << _minesweeper->getTotalMines();
  oe->setCaption(mines.str());
  
  _world->stepSimulation(deltaT);
  oe = _overlayManager->getOverlayElement("timeinf");
  if (_pick){
    _last_time += deltaT;
    oe->setCaption("");
  }
  else oe->setCaption("");
  
  
  return true;
}

bool
PlayState::frameEnded
(const Ogre::FrameEvent& evt)
{
  return _exitGame;
}

void
PlayState::keyPressed
(const OIS::KeyEvent &e)
{
  // Tecla p --> PauseState.
  if (e.key == OIS::KC_P && !_end_game) {
    pushState(PauseState::getSingletonPtr());
  }
  else{
    Ogre::Vector3 vt(0,0,0);     Ogre::Real tSpeed = 20.0;
    Ogre::Real deltaT = 0.03;
    
    
    if(e.key == OIS::KC_UP){ vt+=Ogre::Vector3(0,0,-1); _camera->moveRelative(vt * deltaT * tSpeed);}
    if(e.key == OIS::KC_DOWN){  vt+=Ogre::Vector3(0,0,1); _camera->moveRelative(vt * deltaT * tSpeed);}
    if(e.key == OIS::KC_LEFT){  vt+=Ogre::Vector3(-1,0,0); _camera->moveRelative(vt * deltaT * tSpeed);}
    if(e.key == OIS::KC_RIGHT){ vt+=Ogre::Vector3(1,0,0); _camera->moveRelative(vt * deltaT * tSpeed);
    }

  if(e.key == OIS::KC_A)  _nave->translate(Ogre::Vector3(-0.6,0,0));
  if(e.key == OIS::KC_D) _nave->translate(Ogre::Vector3(0.6,0,0));
  if(e.key == OIS::KC_W)  _nave->translate(Ogre::Vector3(0,0,-0.1));
  if(e.key == OIS::KC_S) _nave->translate(Ogre::Vector3(0,0,0.1));
    Ogre::Real r =0;
    
    if(e.key == OIS::KC_R){
//       r-=180;
      SoundFXManager::getSingletonPtr()->getMusic("impact.wav")->play();
//       _minesweeper->yaw(Ogre::Degree(r*0.1));
    }
    
    if(e.key == OIS::KC_I){ vt+=Ogre::Vector3(0,-1,0); _camera->moveRelative(vt * deltaT * tSpeed);}
    if(e.key == OIS::KC_K){  vt+=Ogre::Vector3(0,1,0); _camera->moveRelative(vt * deltaT * tSpeed);}
    if(e.key == OIS::KC_J){  vt+=Ogre::Vector3(0,-1,0); _camera->moveRelative(vt * deltaT * tSpeed);}
    if(e.key == OIS::KC_L){ vt+=Ogre::Vector3(0,1,0); _camera->moveRelative(vt * deltaT * tSpeed);}
 
    
    if(_end_game){
      if(e.key == OIS::KC_SPACE){
	popState();
      }
    }
  }
}

void
PlayState::keyReleased
(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_ESCAPE) {
    popState();
  }

}

void
PlayState::mouseMoved
(const OIS::MouseEvent &e)
{
  Ogre::Vector3 vt(0,0,0);     
  Ogre::Real deltaT = 0.02;
   
  if (e.state.Z.rel > 0){
    if(_camera->getPosition().length() >10.0){
       vt+=Ogre::Vector3(0,0,-e.state.Z.rel);
    }
  }
  else{
       vt+=Ogre::Vector3(0,0,-e.state.Z.rel);
  }
 _camera->moveRelative(vt*deltaT);
/*
   if (_key_pressed){
     _minesweeper->yaw(Ogre::Radian(_mouse_position.X.rel - e.state.X.rel*deltaT));
     _minesweeper->pitch(Ogre::Radian(_mouse_position.Y.rel - e.state.Y.rel*deltaT));
   }*/
  
}

void
PlayState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  int posx = e.state.X.abs;
  int posy = e.state.Y.abs;
  Ogre::Vector3 vt(0,0,0);     
  Ogre::Ray r;
  Ogre::RaySceneQueryResult result;
  Ogre::RaySceneQueryResult::iterator it;
  Ogre::SceneNode * _selectedNode;

  r = setRayQuery(posx, posy, 0);//MODIFICAR.-------------------------------------------
  result = _raySceneQuery->execute();
  it = result.begin();
  if (it!=result.end()){
    _selectedNode = it->movable->getParentSceneNode();
  }
  else _selectedNode=0;
 
//   switch(id){
//   case OIS::MB_Left:
//     if (_selectedNode && !_end_game){
//       _pick = true;
//       _minesweeper->sendMove(_selectedNode);
//       
//       if(_minesweeper->isGameOver())
// 	gameOver();
//       else if(_minesweeper->isWin())
// 	gameWin();
//     }
//     break;
//   case OIS::MB_Middle:
//     _key_pressed = true;
//     _mouse_position = e.state;
//     break;
//   case OIS::MB_Right:
//     if (_selectedNode && !_end_game){
//       _minesweeper->setFlag(_selectedNode);
//     }
//     break;
//   default:
//     ;
//   }
}

void PlayState::gameOver(){
  _pick = false;
  _overlayManager->getOverlayElement("logoGO")->show();
  SoundFXManager::getSingletonPtr()->getMusic("bomb.wav")->play();

  _end_game = true;
}


void PlayState::gameWin(){
  _pick = false;
  _overlayManager->getOverlayElement("logoClear")->show();
  std::ofstream file("records.txt", std::ofstream::app | std::ofstream::out);
  file.close();
  _end_game = true;
}

void
PlayState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  
  if(id == OIS::MB_Middle)
     _key_pressed=false;

}




PlayState*
PlayState::getSingletonPtr ()
{
return msSingleton;
}

PlayState&
PlayState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}


Ogre::Ray PlayState::setRayQuery(int posx, int posy, int mask) {
   Ogre::Ray rayMouse = _camera->getCameraToViewportRay
    (posx/float(_root->getAutoCreatedWindow()->getWidth()), posy/float(_root->getAutoCreatedWindow()->getHeight()));
   _raySceneQuery->setRay(rayMouse);
   _raySceneQuery->setSortByDistance(true);
   _raySceneQuery->setQueryMask(mask);
   return (rayMouse);
}
