

#ifndef RecordState_H
#define RecordState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <string>
#include <sstream>
#include <fstream>
#include "GameState.h"
#include <CEGUI.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>

class RecordState : public Ogre::Singleton<RecordState>, public GameState
{
 public:
  RecordState () {}

  void enter ();
  void exit ();
  void pause ();
  void resume ();

  void keyPressed (const OIS::KeyEvent &e);
  void keyReleased (const OIS::KeyEvent &e);

  void mouseMoved (const OIS::MouseEvent &e);
  void mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  void mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);

  bool frameStarted (const Ogre::FrameEvent& evt);
  bool frameEnded (const Ogre::FrameEvent& evt);

  // Heredados de Ogre::Singleton.
  static RecordState& getSingleton ();
  static RecordState* getSingletonPtr ();
  
  std::string convert(std::string name, double time);
  

  bool quit(const CEGUI::EventArgs &e);

  //GUI
  void createBackground();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  Ogre::OverlayManager* _overlayManager;
  Ogre::Overlay *_overlay;
  Ogre::Rectangle2D* _rect;
  CEGUI::Window * _return, * _v;
  Ogre::SceneNode * _node;
  bool _exitGame;


};

#endif
