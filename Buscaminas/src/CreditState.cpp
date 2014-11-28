#include "CreditState.h"
#include "IntroState.h"
#include <sstream>
#include <iostream>

template<> CreditState* Ogre::Singleton<CreditState>::msSingleton = 0;

void
CreditState::enter ()
{
  std::cout << "ENTER CREDITSTATE" << std::endl;
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
  _overlay = _overlayManager->getByName("Credits");
  _overlay->show();
    std::cout << " CREDITSTATE" << std::endl;

  createBackground();

  _exitGame = true;
}



void CreditState::createBackground(){
  
    // Create background material   
  // _material = Ogre::MaterialManager::getSingleton().create("Back", "General");

 //  _material->getTechnique(0)->getPass(0)->createTextureUnitState("waterwall.jpg");

 //  _material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
 //  _material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
 //  _material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

    // Create background rectangle covering the whole screen
    _rect = new Ogre::Rectangle2D(true);
    _rect->setCorners(-1.0, 1.0, 1.0, -1.0);
    _rect->setMaterial("Background");
        std::cout << "BACKGROUND" << std::endl;

    // Render the background before everything else
    _rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);
    
   
    
    // Attach background to the scene
    _node = _sceneMgr->getRootSceneNode()->createChildSceneNode("Background");
    _node->attachObject(_rect);
    
    // Example of background scrolling
    // _material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.02, 0.0);
  
}


void
CreditState::exit ()
{ 
  _sceneMgr->destroySceneNode(_node);
  _overlay->hide();
  
}

void
CreditState::pause()
{
}

void
CreditState::resume()
{
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
  if (e.key == OIS::KC_P) {
    popState();
  } 
  
}

void
CreditState::keyReleased
(const OIS::KeyEvent &e)
{
  
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


