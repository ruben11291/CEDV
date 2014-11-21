#include "PlayState.h"
#include "PauseState.h"

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
  
  Ogre::Entity* ent1 = _sceneMgr->createEntity("Cube1.mesh");
  _node = _sceneMgr->createSceneNode("SinbadNode");
  _node->attachObject(ent1);
  _sceneMgr->getRootSceneNode()->addChild(_node);
  
  
  _sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
  _sceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
  
  
  Ogre::Light* light = _sceneMgr->createLight("Light1");
  light->setType(Ogre::Light::LT_DIRECTIONAL);
  light->setDirection(Ogre::Vector3(1,-1,0));

  Ogre::Light* light2 = _sceneMgr->createLight("Light2");
  light2->setType(Ogre::Light::LT_POINT);
  light2->setPosition(8, 8, -2);
  light2->setSpecularColour(0.9, 0.9, 0.9); 
  light2->setDiffuseColour(0.9, 0.9, 0.9);

  Ogre::Plane plane1(Ogre::Vector3::UNIT_Y, 0);
  Ogre::MeshManager::getSingleton().createPlane("plane1",
	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
	200,200,1,1,true,1,10,10,Ogre::Vector3::UNIT_Z);

  Ogre::SceneNode* node3 = _sceneMgr->createSceneNode("ground");
  Ogre::Entity* groundEnt = _sceneMgr->createEntity("planeEnt", "plane1");
  groundEnt->setMaterialName("Ground");
  node3->attachObject(groundEnt);
  _sceneMgr->getRootSceneNode()->addChild(node3);
  

  _overlayManager = Ogre::OverlayManager::getSingletonPtr();
  Ogre::Overlay *overlay = _overlayManager->getByName("Info");
  overlay->show();

  _exitGame = false;
}

void
PlayState::exit ()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
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
  if (_exitGame)
    return false;
  
  return true;
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
    _node->yaw(Ogre::Degree(r*0.1));
  }
  
  if(e.key == OIS::KC_Q){
    r-=180;
    _node->yaw(Ogre::Degree(r*0.1));
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
    _exitGame = true;
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
