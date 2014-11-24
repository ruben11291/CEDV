#include "PlayState.h"
#include "PauseState.h"
#include <sstream>
#include <iostream>

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void
PlayState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  _camera->setPosition(Ogre::Vector3(5,10.5,12));
  _camera->lookAt(Ogre::Vector3(1.4,4.3,3));
  _camera->setNearClipDistance(5);
  _camera->setFarClipDistance(10000);
  _camera->setFOVy(Ogre::Degree(48));
  
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  // Nuevo background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
  
  double width = _viewport->getActualWidth();
  double height = _viewport->getActualHeight();
  _camera->setAspectRatio(width / height);
  
  _cubes = new std::vector<Ogre::SceneNode*>();//for locating the cubes when a click was performed
  Ogre::Entity* ent1 = _sceneMgr->createEntity("Cube1.mesh");
  Ogre::Vector3 bbSize = ent1->getMesh()->getBounds().getSize();
  _sceneMgr->destroyEntity(ent1);

  int n = 10;//to be done , only for testing
  //  creation of the cube 
  for (int i=0;i<n;i++){
    for (int j=0;j<n;j++){
      //base
      Ogre::Entity* ent1 = _sceneMgr->createEntity("Cube1.mesh");
      //    std::stringstream ss;//create a stringstream
      //    ss << j;//add number to the stream
      Ogre::SceneNode* _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->setPosition(float(i*bbSize.x),0,float(j*bbSize.x));
      _node->scale(1,0.5,1);
      _sceneMgr->getRootSceneNode()->addChild(_node);
      _cubes->push_back(_node);

      //tapa
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->setPosition(float(i*bbSize.x),float(n*bbSize.x),float(j*bbSize.x));
      _node->scale(1,0.5,1);
      _sceneMgr->getRootSceneNode()->addChild(_node);
      _cubes->push_back(_node);

      //lateral izq
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->scale(1,0.5,1);
      _node->roll(Ogre::Degree(90),Ogre::Node::TS_PARENT);
      _node->setPosition(-bbSize.x,float(i*bbSize.x),float(j*bbSize.x));
      _sceneMgr->getRootSceneNode()->addChild(_node);
      _cubes->push_back(_node);

      //lateral derecho
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->setPosition(n*bbSize.x+bbSize.x,float(i*bbSize.x),float(j*bbSize.x));
      _node->scale(1,0.5,1);
      _node->roll(Ogre::Degree(90),Ogre::Node::TS_PARENT);
      _sceneMgr->getRootSceneNode()->addChild(_node);
      _cubes->push_back(_node);

      //tapa trasera
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->setPosition(float(i*bbSize.x),float(j*bbSize.x),-bbSize.x);
      _node->scale(1,0.5,1);
      _node->pitch(Ogre::Degree(-90),Ogre::Node::TS_PARENT);
      _sceneMgr->getRootSceneNode()->addChild(_node);
      _cubes->push_back(_node);

      //frontal
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->setPosition(float(i*bbSize.x),float(j*bbSize.x),n*bbSize.x+bbSize.x);
      _node->scale(1,0.5,1);
      _node->pitch(Ogre::Degree(-90),Ogre::Node::TS_PARENT);
      _sceneMgr->getRootSceneNode()->addChild(_node);
      _cubes->push_back(_node);
  }
  }
  
  
  _sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
  _sceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
  
  
  _light = _sceneMgr->createLight("Light1");
  _light->setType(Ogre::Light::LT_DIRECTIONAL);
  _light->setDirection(Ogre::Vector3(1,-1,0));

  _light2 = _sceneMgr->createLight("Light2");
  _light2->setType(Ogre::Light::LT_POINT);
  _light2->setPosition(8, 8, -2);
  _light2->setSpecularColour(0.9, 0.9, 0.9); 
  _light2->setDiffuseColour(0.9, 0.9, 0.9);

  Ogre::Plane plane1(Ogre::Vector3::UNIT_Y, 0);
  Ogre::MeshManager::getSingleton().createPlane("plane1",
	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
	200,200,1,1,true,1,10,10,Ogre::Vector3::UNIT_Z);

  _ground = _sceneMgr->createSceneNode("ground");
  Ogre::Entity* groundEnt = _sceneMgr->createEntity("planeEnt", "plane1");
  groundEnt->setMaterialName("Ground");
  _ground->attachObject(groundEnt);//comprobar si se libera sola la entidad
  _sceneMgr->getRootSceneNode()->addChild(_ground);
  

  _overlayManager = Ogre::OverlayManager::getSingletonPtr();
  _overlay = _overlayManager->getByName("Info");
  _overlay->show();

  _exitGame = true;
}

void
PlayState::exit ()
{ 
  // _root->getAutoCreatedWindow()->removeAllViewports();
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();

  //delete _overlay;
  //delete _ground;
  delete _cubes;
  // delete _sceneMgr;
  // delete _overlayManager;
  // delete _root;
  
}

void
PlayState::pause()
{
}

void
PlayState::resume()
{
  // Se restaura el background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
}

bool
PlayState::frameStarted
(const Ogre::FrameEvent& evt)
{
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
    pushState(PauseState::getSingletonPtr());
  }
  
  Ogre::Real r =0;
  if(e.key == OIS::KC_W){
    r+=180;
    //for (std::vector<Ogre::SceneNode*>::iterator it=_cubes->begin();it!=_cubes->end();it++)
    //(*it)->yaw(Ogre::Degree(r*0.1));
    _camera->yaw(Ogre::Degree(r*0.1));
  }
  
  if(e.key == OIS::KC_Q){
    r-=180;
    //for (std::vector<Ogre::SceneNode*>::iterator it=_cubes->begin();it!=_cubes->end();it++)
    //(*it)->yaw(Ogre::Degree(r*0.1));
    _camera->yaw(Ogre::Degree(r*0.1));

  }
  
  if(e.key == OIS::KC_S){
      Ogre::Vector3 vt(0,0,0);
    Ogre::Real tSpeed = 10.0; 
    vt+= Ogre::Vector3(0,0,-10)*0.05 * 7;   
    _camera->moveRelative(vt * 0.05 * tSpeed);
  }
   if(e.key == OIS::KC_A){
      Ogre::Vector3 vt(0,0,0);
    Ogre::Real tSpeed = 10.0; 
    vt-= Ogre::Vector3(0,0,-10)*0.05 * 7;   
    _camera->moveRelative(vt * 0.05 * tSpeed);
  }
  
}

void
PlayState::keyReleased
(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = false;
  }
}

void
PlayState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
PlayState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  if(e.state.buttonDown(OIS::MB_Left)){
//     Ogre::Vector3 vt(0,0,0);
//     Ogre::Real tSpeed = 10.0; 
//     vt+= Ogre::Vector3(0,0,-10)*0.05 * e.state.Z.rel;   
//     _camera->moveRelative(vt * 0.05 * tSpeed);
  }
  
}

void
PlayState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
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


