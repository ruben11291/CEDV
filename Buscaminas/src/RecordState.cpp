#include "RecordState.h"
#include "IntroState.h"
#include <sstream>
#include <iostream>

template<> RecordState* Ogre::Singleton<RecordState>::msSingleton = 0;

void
RecordState::enter ()
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
  
  _viewport = _root->getAutoCreatedWindow()->getViewport(0);
  // Nuevo background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
  
  double width = _viewport->getActualWidth();
  double height = _viewport->getActualHeight();
  _camera->setAspectRatio(width / height);
 
  _overlayManager = Ogre::OverlayManager::getSingletonPtr();
  _overlay = _overlayManager->getByName("Logo");
  _overlay->show();
  
  createBackground();
  

  _exitGame = true;
}



void RecordState::createBackground(){
  
    // Create background material
    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("Backr", "General");
    material->getTechnique(0)->getPass(0)->createTextureUnitState("waterwall.jpg");
    material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    
    // Create background rectangle covering the whole screen
    _rect = new Ogre::Rectangle2D(true);
    _rect->setCorners(-1.0, 1.0, 1.0, -1.0);
    // _rect->setMaterial("Backr");
    
    //   Render the background before everything else
    // _rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);
    
    // Attach background to the scene
    _node = _sceneMgr->getRootSceneNode()->createChildSceneNode("Backr");
    _node->attachObject(_rect);
    
    // Example of background scrolling
    material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.02, 0.0);

    
    _overlay = Ogre::OverlayManager::getSingletonPtr()->getByName("Pause");
    _overlay->show();

  }


void
RecordState::exit ()
{ 
  std::cout << "Record state exit" << std::endl;

  _sceneMgr->destroySceneNode(_node);
  _overlay->hide();
}

void
RecordState::pause()
{
}

void
RecordState::resume()
{
  std::cout << "Record state resume" << std::endl;
  // Se restaura el background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
}

bool
RecordState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
RecordState::frameEnded
(const Ogre::FrameEvent& evt)
{
  return _exitGame;
}

void
RecordState::keyPressed
(const OIS::KeyEvent &e)
{
  // Tecla p --> PauseState.
  if (e.key == OIS::KC_P) {
    popState();
  }
  
}

void
RecordState::keyReleased
(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = false;
  }
}

void
RecordState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
RecordState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{

  
}

void
RecordState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

RecordState*
RecordState::getSingletonPtr ()
{
return msSingleton;
}

RecordState&
RecordState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}


