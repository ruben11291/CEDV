#include "PlayState.h"
#include "PauseState.h"

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void
PlayState::enter ()
{  std::cout << "Play state enter" << std::endl;

  _root = Ogre::Root::getSingletonPtr();
  
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  _camera->setPosition(Ogre::Vector3(5,10.5,20));
  _camera->lookAt(Ogre::Vector3(1.4,4.3,3));
  _camera->setNearClipDistance(5);
  _camera->setFarClipDistance(10000);
  _camera->setFOVy(Ogre::Degree(48));
  
  _pSoundFXManager = SoundFXManager::getSingletonPtr();
  _simpleEffect = _pSoundFXManager->load("bomb.wav");
  // std::cout << "DESPUES MANAGER " << std::endl;
  _viewport = _root->getAutoCreatedWindow()->getViewport(0);
  // Nuevo background colour.
  //_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
  
  double width = _viewport->getActualWidth();
  double height = _viewport->getActualHeight();
  _camera->setAspectRatio(width / height);
  
 
  _minesweeper = new Minesweeper(5,_sceneMgr);

  
  // Create background material
  _material = Ogre::MaterialManager::getSingleton().create("Backgr", "General");
  _material->getTechnique(0)->getPass(0)->createTextureUnitState("back.jpg");
  _material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
  _material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
  _material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
  
  // Create background rectangle covering the whole screen
  _rect = new Ogre::Rectangle2D(true);
  _rect->setCorners(-1.0, 1.0, 1.0, -1.0);
  _rect->setMaterial("Backgr"); 
  
    // Render the background before everything else
  _rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);
  
    
    
    // Attach background to the scene
  _ground = _sceneMgr->getRootSceneNode()->createChildSceneNode("Back");
  _ground->attachObject(_rect);
  
  _raySceneQuery = _sceneMgr->createRayQuery(Ogre::Ray());

  _overlayManager = Ogre::OverlayManager::getSingletonPtr();
  _overlay = _overlayManager->getByName("Info");
  _overlay->show();

  Ogre::OverlayElement * oe = _overlayManager->getOverlayElement("logoGO");
  oe->hide();
  oe = _overlayManager->getOverlayElement("timeClear");
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
{   std::cout << "Play state exit" << std::endl;
  _sceneMgr->destroyQuery(static_cast<Ogre::RaySceneQuery*>(_raySceneQuery));
  _s.str(" ");
  _overlay->hide();
  _sceneMgr->destroySceneNode(_ground);
  delete _minesweeper;
}

void
PlayState::pause()
{  std::cout << "Play state pause" << std::endl;
  _pick=false;
  _minesweeper->hide();
  _ground->setVisible(false);
  _overlay->hide();
}

void
PlayState::resume()
{
  std::cout << "Play state resume" << std::endl;
  _pick=true;
  _ground->setVisible(true);
  _minesweeper->show();
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
//   oe->setCaption(Ogre::StringConverter::toString(fps));
  remaining << _minesweeper->getDiscovered() << "/" << (_minesweeper->getSquares()*_minesweeper->getSquares()*6)-_minesweeper->getTotalMines();
  oe->setCaption(remaining.str());
  
  oe = _overlayManager->getOverlayElement("minesinf");
  mines << _minesweeper->getFlags() << "/" << _minesweeper->getTotalMines();
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
  if (e.key == OIS::KC_P) {
    std::cout << "Play state key pressed"<<std::endl;
    pushState(PauseState::getSingletonPtr());
  }
  
  Ogre::Vector3 vt(0,0,0);     Ogre::Real tSpeed = 20.0;
  Ogre::Real deltaT = 0.03;
  
 
  if(e.key == OIS::KC_UP){ vt+=Ogre::Vector3(0,0,-1); _camera->moveRelative(vt * deltaT * tSpeed);}
  if(e.key == OIS::KC_DOWN){  vt+=Ogre::Vector3(0,0,1); _camera->moveRelative(vt * deltaT * tSpeed);}
  if(e.key == OIS::KC_LEFT){  vt+=Ogre::Vector3(-1,0,0); _camera->moveRelative(vt * deltaT * tSpeed);}
  if(e.key == OIS::KC_RIGHT){ vt+=Ogre::Vector3(1,0,0); _camera->moveRelative(vt * deltaT * tSpeed);
  }
  
  Ogre::Real r =0;
   
    if(e.key == OIS::KC_R){
       r-=180;
      _minesweeper->yaw(Ogre::Degree(r*0.1));
    }
  
  if(e.key == OIS::KC_D){
    r+=180;
    _minesweeper->pitch(Ogre::Degree(r*0.1));
  }
  
  if(_end_game){
    if(e.key == OIS::KC_SPACE){
      popState();
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
    if(_camera->getPosition().length() > 10.0){
       vt+=Ogre::Vector3(0,0,-e.state.Z.rel);
    }
  }
  else{
       vt+=Ogre::Vector3(0,0,-e.state.Z.rel);
  }
 _camera->moveRelative(vt*deltaT);

   if (_key_pressed){
     _minesweeper->yaw(Ogre::Radian(_mouse_position.X.rel - e.state.X.rel*deltaT));
     _minesweeper->pitch(Ogre::Radian(_mouse_position.Y.rel - e.state.Y.rel*deltaT));
   }
  
}

void
PlayState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  std::cout <<  id << std::endl;
  //int fps = 1.0 / deltaT;
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
 
  if(!_end_game){
    switch(id){
    case OIS::MB_Left:
      if (_selectedNode){
	_pick = true;
	_minesweeper->sendMove(_selectedNode);
	
	if(_minesweeper->isGameOver())
	  gameOver();
	else if(_minesweeper->isWin())
	  gameWin();
      }
      break;
    case OIS::MB_Middle:
      _key_pressed = true;
      _mouse_position = e.state;
      break;
    case OIS::MB_Right:
      if (_selectedNode){
	_minesweeper->setFlag(_selectedNode);
      }
      break;
    default:
      std::cout << "mierda default " <<std::endl;
    }
  }
}

void PlayState::gameOver(){
  _pick = false;
  Ogre::OverlayElement * oe = _overlayManager->getOverlayElement("logoGO");
  oe->show();
  _simpleEffect->play();
  _end_game = true;
}

void PlayState::gameWin(){
  _pick = false;
  Ogre::OverlayElement* oe = _overlayManager->getOverlayElement("timeClear");
  oe->setCaption(_s.str());
  oe = _overlayManager->getOverlayElement("logoClear");
  oe->show();
  std::ofstream file("records.txt", std::ofstream::app);
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
