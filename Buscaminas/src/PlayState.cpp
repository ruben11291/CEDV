#include "PlayState.h"
#include "PauseState.h"
#include <sstream>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

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
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
  
  double width = _viewport->getActualWidth();
  double height = _viewport->getActualHeight();
  _camera->setAspectRatio(width / height);
  
  _cubes = new std::vector<Ogre::SceneNode*>();//for locating the cubes when a click was performed
  Ogre::Entity* ent1 = _sceneMgr->createEntity("Cube1.mesh");
  //ent1->setQueryFlags(STAGE);
  
  Ogre::Vector3 bbSize = ent1->getMesh()->getBounds().getSize();
  
  _fnode = _sceneMgr->createSceneNode("aux");
  _fnode->setPosition(0.0,4,0.0);
  _sceneMgr->getRootSceneNode()->addChild(_fnode);
  _sceneMgr->destroyEntity(ent1);

  int n = 5;//to be done , only for testing
  Ogre::Vector3 distance_to_center = bbSize*n/2;//distance to center of cube
  
  //  creation of the cube 
  for (int i=0;i<n;i++){
    for (int j=0;j<n;j++){
      //base
      Ogre::Entity* ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->translate(Ogre::Vector3(float(i*bbSize.x),0.1,float(j*bbSize.x))-distance_to_center);
      _node->scale(1,0.1,1);
      _fnode->addChild(_node);
      _cubes->push_back(_node);

      //tapa
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);

      _node->translate(Ogre::Vector3(float(i*bbSize.x),float(n*bbSize.x+0.05),float(j*bbSize.x))-distance_to_center);
      _node->scale(1,0,1);
      _fnode->addChild(_node);
      _cubes->push_back(_node);

      //lateral izq
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->scale(1,0,1);
      _node->translate(Ogre::Vector3(-bbSize.x+0.25,float(i*bbSize.x +0.3),float(j*bbSize.x))-distance_to_center);
      _node->roll(Ogre::Degree(90),Ogre::Node::TS_PARENT);
      _fnode->addChild(_node);
      _cubes->push_back(_node);

      //lateral derecho
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->translate(Ogre::Vector3(n*bbSize.x+bbSize.x-0.75, float(i*bbSize.x +0.3),float(j*bbSize.x))-distance_to_center);
      _node->scale(1,0,1);
      _node->roll(Ogre::Degree(90),Ogre::Node::TS_PARENT);
      _fnode->addChild(_node);
      _cubes->push_back(_node);



      //tapa trasera
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->translate(Ogre::Vector3(float(i*bbSize.x),float(j*bbSize.x+0.3),-bbSize.x+0.25 )-distance_to_center);
      _node->scale(1,0,1);
      _node->pitch(Ogre::Degree(-90),Ogre::Node::TS_PARENT);
      _fnode->addChild(_node);

//       _sceneMgr->getRootSceneNode()->addChild(_node);
      _cubes->push_back(_node);

      //frontal
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->translate(Ogre::Vector3(float(i*bbSize.x),float(j*bbSize.x+0.3),n*bbSize.x+bbSize.x-0.75)-distance_to_center);
      _node->scale(1,0,1);
      _node->pitch(Ogre::Degree(-90),Ogre::Node::TS_PARENT);
      _fnode->addChild(_node);
      _cubes->push_back(_node);
}
 
    _raySceneQuery = _sceneMgr->createRayQuery(Ogre::Ray());
    
  }
  
  
  
  
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
  

  _overlayManager = Ogre::OverlayManager::getSingletonPtr();
  _overlay = _overlayManager->getByName("Info");
  _overlay->show();
  _key_pressed=false;
  _exitGame = true;
}

void
PlayState::exit ()
{   std::cout << "Play state exit" << std::endl;

  _overlay->hide();
  _overlayManager->destroy("Info");
  for (std::vector<Ogre::SceneNode*>::iterator it = _cubes->begin();it!=_cubes->end();it++)
    _sceneMgr->destroySceneNode(*it);
  _sceneMgr->destroySceneNode(_ground);
  _sceneMgr->destroySceneNode(_fnode);
  _sceneMgr->destroyQuery(static_cast<Ogre::RaySceneQuery*>(_raySceneQuery));
  delete _cubes;
  
  //_sceneMgr->clearScene();
  //_root->getAutoCreatedWindow()->removeAllViewports();

  // delete _cubes;
 
}

void
PlayState::pause()
{  std::cout << "Play state pause" << std::endl;

  //parar tiempo
  _ground->setVisible(false);
  _fnode->setVisible(false,true);
  _overlay->hide();
}

void
PlayState::resume()
{
  std::cout << "Play state resume" << std::endl;
  // Se restaura el background colour.
  _ground->setVisible(true);
  _fnode->setVisible(true,true);
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
  _overlay->show();
}

bool
PlayState::frameStarted
(const Ogre::FrameEvent& evt)
{
  deltaT = evt.timeSinceLastFrame;
  // int fps = 1.0 / deltaT;

  Ogre::OverlayElement *oe;
  oe = _overlayManager->getOverlayElement("fpsInfo");
//   oe->setCaption(Ogre::StringConverter::toString(fps));
  oe->setCaption("600/600");
  
  oe = _overlayManager->getOverlayElement("minesinf");
  oe->setCaption("30/30");
  
  oe = _overlayManager->getOverlayElement("timeinf");
  oe->setCaption("0.00");
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
      
      _fnode->yaw(Ogre::Degree(r*0.1),Ogre::Node::TS_WORLD);
    }
  
  if(e.key == OIS::KC_D){
    r+=180;
   
    _fnode->pitch(Ogre::Degree(r*0.1),Ogre::Node::TS_WORLD);
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
  std::cout<< e.state.X.abs << " "<<e.state.Y.abs <<std::endl;
  Ogre::Vector3 vt(0,0,0);     
  Ogre::Real deltaT = 0.02;
   
  if (e.state.Z.rel > 0){
    if(_camera->getPosition().length() > 10.0){
       vt+=Ogre::Vector3(0,0,-e.state.Z.rel);
       std::cout<< "negativo"<<std::endl;
    }
  }
  else{
//     if(_camera->getPosition().length() < 40.0){
       vt+=Ogre::Vector3(0,0,-e.state.Z.rel);
      std::cout<< "positivo"<<std::endl;
//     }
  }
 _camera->moveRelative(vt*deltaT);

   if (_key_pressed){
     _fnode->yaw(Ogre::Radian(_mouse_position.X.rel - e.state.X.rel*deltaT));
     _fnode->pitch(Ogre::Radian(_mouse_position.Y.rel - e.state.Y.rel*deltaT));
   }
  
}

void
PlayState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  std::cout <<  id << std::endl;
  // Ogre::Real deltaT = e.timeSinceLastFrame;deberia ir en frameupdate y actualizar lo que aqui se modifique
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

  switch(id){
  case OIS::MB_Left:
    if (_selectedNode){
      _selectedNode->showBoundingBox(true);
      static_cast<Ogre::Entity *>(_selectedNode->getAttachedObject(0))->setMaterialName("Cube2");
    }
    break;
  case OIS::MB_Middle:
    _key_pressed = true;
    _mouse_position = e.state;
    break;
  case OIS::MB_Right:
    if (_selectedNode){
      _selectedNode->showBoundingBox(true);
    static_cast<Ogre::Entity *>(_selectedNode->getAttachedObject(0))->setMaterialName("Cube2");
    }
  default:
    std::cout << "Default"<<std::endl;break;
  }
   
  
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
