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
   _light->setPosition(9,7,2);
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
  
  _level = new Level(2,_sceneMgr);
  


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
  delete _level;
   SoundFXManager::getSingletonPtr()->unload("bomb.wav");
   SoundFXManager::getSingletonPtr()->unload("impact.wav");
  _sceneMgr->destroyEntity("planeEnt");
  _overlayManager->getByName("Info")->hide();
  _sceneMgr->destroyLight(_light);
  _sceneMgr->destroySceneNode(_ground);
 

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

  Ogre::OverlayElement *oe;
  oe = _overlayManager->getOverlayElement("fpsInfo");
  oe->setCaption(remaining.str());
  
  oe = _overlayManager->getOverlayElement("minesinf");
  oe->setCaption(mines.str());
   

  _level->getWorld()->stepSimulation(deltaT);
    if( _level->detectCollision()){
      SoundFXPtr  impact = SoundFXManager::getSingletonPtr()->load("impact.wav");
      impact->play();
      if(checkWin())
	_end_game = true;
    }
  
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
  
  _level->getWorld()->stepSimulation(evt.timeSinceLastFrame);

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

  if(e.key == OIS::KC_A)  _level->translate(Ogre::Vector3(-0.6,0,0));
  if(e.key == OIS::KC_D) _level->translate(Ogre::Vector3(0.6,0,0));
  if(e.key == OIS::KC_W)  _level->translate(Ogre::Vector3(0,0,-0.1));
  if(e.key == OIS::KC_S) _level->translate(Ogre::Vector3(0,0,0.1));
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
