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
//   _light->setCastShadows(true);
  
	
   _light2 = _sceneMgr->createLight("Light2");
  _light2->setType(Ogre::Light::LT_SPOTLIGHT);
  _light2->setDiffuseColour(0.2,0.2,0.2);
  _light2->setPosition(3,12,3);
  _light2->setDirection(Ogre::Vector3(-0.3, -1, 0));
  _light2->setSpotlightInnerAngle(Ogre::Degree(25.0f));
  _light2->setSpotlightOuterAngle(Ogre::Degree(65.0f));
  _light2->setSpotlightFalloff(5.0f);
//   _light2->setCastShadows(true);
  
  _camera = _sceneMgr->getCamera("IntroCamera");
  _camera->setPosition(Ogre::Vector3(0.07694,7.3,6.5));
  _camera->lookAt(Ogre::Vector3(0.67329,-0.12087,1.1));
  _camera->setDirection(Ogre::Vector3(-0.0213742,-0.590729,-0.806589));
  _camera->setNearClipDistance(5);
  _camera->setFarClipDistance(10000);
  _camera->setFOVy(Ogre::Degree(53.13));
  
  _pSoundFXManager = SoundFXManager::getSingletonPtr();
  _simpleEffect = _pSoundFXManager->load("bomb.wav");
  _simpleEffect = _pSoundFXManager->load("impact.wav");
  
  _viewport = _root->getAutoCreatedWindow()->getViewport(0);
  
  double width = _viewport->getActualWidth();
  double height = _viewport->getActualHeight();
  _camera->setAspectRatio(width / height);
  
 
  
  
  /*Creation of the minesweeper*/
  _node = _sceneMgr->createSceneNode("plano");
   Ogre::Entity* ent1 = _sceneMgr->createEntity("Suelo.mesh");
//    ent1->setMaterialName("suelo");
   ent1->setCastShadows(true);
   Ogre::Entity *ent3 = _sceneMgr->createEntity("laterales.mesh");
   ent3->setCastShadows(true);
   _node->scale(1.30,1.30,1.30);
  _node->attachObject(ent1);
  _node->attachObject(ent3);
  _sceneMgr->getRootSceneNode()->addChild(_node);
  
  Ogre::SceneNode* bola = _sceneMgr->createSceneNode("bola");
  Ogre::Entity* ball = _sceneMgr->createEntity("Planet.mesh");
  ball->setCastShadows(true);
  bola->scale(0.14,0.14,0.14);
  bola->attachObject(ball);
  bola->translate(0,0.4,-0.65);
  _sceneMgr->getRootSceneNode()->addChild(bola);
  
  
  _nave= _sceneMgr->createSceneNode("nave");
  Ogre::Entity* ent2 = _sceneMgr->createEntity("Cube.mesh");
  ent2->setCastShadows(true);
  _nave->scale(0.2,0.2,0.2);
  _nave->yaw(Ogre::Degree(-180));
  _nave->attachObject(ent2);
  _nave->translate(0,0.5,0);
  _sceneMgr->getRootSceneNode()->addChild(_nave);
  
  Ogre::Plane plane1(Ogre::Vector3::UNIT_Y, 0);
  Ogre::MeshManager::getSingleton().createPlane("plane1",
	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
	200,200,1,1,true,1,20,20,Ogre::Vector3::UNIT_Z);

   _ground = _sceneMgr->createSceneNode("ground");
  Ogre::Entity* groundEnt = _sceneMgr->createEntity("planeEnt", "plane1");
  groundEnt->setMaterialName("Ground");
  groundEnt->setCastShadows(false);
  _ground->attachObject(groundEnt);
  _sceneMgr->getRootSceneNode()->addChild(_ground);

  
  _raySceneQuery = _sceneMgr->createRayQuery(Ogre::Ray());
  
  _overlayManager = Ogre::OverlayManager::getSingletonPtr();
  _overlay = _overlayManager->getByName("Info");
  _overlay->show();

  Ogre::OverlayElement * oe = _overlayManager->getOverlayElement("logoGO");
  oe->hide();
  //   oe->hide();
  oe = _overlayManager->getOverlayElement("logoClear");
  oe->hide();

  _last_time = _time_count = 0;
  _key_pressed=_pick = _end_game = false;
  _exitGame = true;
  _s << "0.0";
}

void
PlayState::exit ()
{ 
  delete _rect;
  _sceneMgr->destroyQuery(static_cast<Ogre::RaySceneQuery*>(_raySceneQuery));
  _s.str(" ");
  _overlay->hide();
  _sceneMgr->destroySceneNode(_ground);
  _sceneMgr->destroySceneNode(_node);
  _sceneMgr->destroySceneNode(_nave);
//   delete _node;
//   delete _nave;
//   delete _minesweeper;
}

void
PlayState::pause()
{  
  _pick=false;
//   _minesweeper->hide();
  _ground->setVisible(false);
  _overlay->hide();
}

void
PlayState::resume()
{
  _pick=true;
  _ground->setVisible(true);
//   _minesweeper->show();
  _overlay->show();
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
  
  oe = _overlayManager->getOverlayElement("timeinf");
  if (_pick){
    _last_time += deltaT;
    _s.str("");
    _s << std::setprecision(1)<<std::fixed << _last_time;
    oe->setCaption(_s.str());
  }
  else oe->setCaption(_s.str());
  
  
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
        _simpleEffect->play();

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

  r = setRayQuery(posx, posy, CUBE);
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
  Ogre::OverlayElement * oe = _overlayManager->getOverlayElement("logoGO");
  oe->show();
  _simpleEffect->play();
//   _minesweeper->showMines();
  _end_game = true;
}


void PlayState::gameWin(){
  _pick = false;
  Ogre::OverlayElement* oe = _overlayManager->getOverlayElement("logoClear");
  oe->show();
  std::ofstream file("records.txt", std::ofstream::app | std::ofstream::out);
  file << "Player " << _s.str() << std::endl;
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
