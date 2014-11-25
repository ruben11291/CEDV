#include "CreditState.h"
#include "IntroState.h"
#include <sstream>
#include <iostream>

template<> CreditState* Ogre::Singleton<CreditState>::msSingleton = 0;

void
CreditState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  
  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  _camera->setPosition(Ogre::Vector3(5,10.5,20));
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
 
  _overlayManager = Ogre::OverlayManager::getSingletonPtr();
  _overlay = _overlayManager->getByName("Credits");
  _overlay->show();
  
  createBackground();

  _exitGame = true;
}



void CreditState::createBackground(){
  
    // Create background material
    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("Back", "General");
    material->getTechnique(0)->getPass(0)->createTextureUnitState("waterwall.jpg");
    material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    
    // Create background rectangle covering the whole screen
    _rect = new Ogre::Rectangle2D(true);
    _rect->setCorners(-1.0, 1.0, 1.0, -1.0);
    _rect->setMaterial("Back");
    
    // Render the background before everything else
    _rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);
    
    // Use infinite AAB to always stay visible
    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    _rect->setBoundingBox(aabInf);
    
    // Attach background to the scene
    Ogre::SceneNode* node = _sceneMgr->getRootSceneNode()->createChildSceneNode("Back");
    node->attachObject(_rect);
    
    // Example of background scrolling
    material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.02, 0.0);
  
}

void
CreditState::exit ()
{ 
  // _root->getAutoCreatedWindow()->removeAllViewports();
//   _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
  _root->destroySceneManager(_sceneMgr);
  delete _rect;
//   CEGUI::OgreRenderer::destroySystem();
  //delete _overlay;
  //delete _ground;
//   delete _cubes;
  // delete _sceneMgr;
  // delete _overlayManager;
  // delete _root;
  
}

void
CreditState::pause()
{
}

void
CreditState::resume()
{
  // Se restaura el background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
}

bool
CreditState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
CreditState::frameEnded
(const Ogre::FrameEvent& evt)
{
  return _exitGame;
}

void
CreditState::keyPressed
(const OIS::KeyEvent &e)
{
  // Tecla p --> PauseState.
  if (e.key == OIS::KC_P) {
    
    changeState(IntroState::getSingletonPtr());

  } 
  
}

void
CreditState::keyReleased
(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = false;
  }
}

void
CreditState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
CreditState::mousePressed
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
CreditState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

CreditState*
CreditState::getSingletonPtr ()
{
return msSingleton;
}

CreditState&
CreditState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}


