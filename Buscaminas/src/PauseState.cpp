#include "PauseState.h"

template<> PauseState* Ogre::Singleton<PauseState>::msSingleton = 0;


void
PauseState::enter ()
{    std::cout << "Pause state enter"<<std::endl;

  _root = Ogre::Root::getSingletonPtr();

  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->getViewport(0);  
  _overlay = Ogre::OverlayManager::getSingletonPtr()->getByName("Pause");
  _overlay->show();
}



void
PauseState::exit ()
{
  _overlay->hide();
}

void
PauseState::pause ()
{
}

void
PauseState::resume ()
{
}

bool
PauseState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
PauseState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
PauseState::keyPressed
(const OIS::KeyEvent &e) {
  // Tecla p --> Estado anterior.
  if (e.key == OIS::KC_P) {
    popState();
  }
}

void
PauseState::keyReleased
(const OIS::KeyEvent &e)
{
}

void
PauseState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
PauseState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
PauseState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

PauseState*
PauseState::getSingletonPtr ()
{
return msSingleton;
}

PauseState&
PauseState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}
