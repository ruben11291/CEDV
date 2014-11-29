
#ifndef PlayState_H
#define PlayState_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"
#include <vector>
#include "Minesweeper.h"
#include "Types.h"

class PlayState : public Ogre::Singleton<PlayState>, public GameState
{
 public:
  PlayState () {}
  
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
  static PlayState& getSingleton ();
  static PlayState* getSingletonPtr ();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  Ogre::OverlayManager* _overlayManager;
  std::vector<Ogre::SceneNode*> *_cubes;
  Ogre::SceneNode* _ground;
  Ogre::SceneNode* _fnode,*_node;
  Ogre::Light* _light,*_light2;
  Ogre::Overlay *_overlay;
  Ogre::RaySceneQuery *_raySceneQuery;
  Ogre::Real deltaT;
  Ogre::Rectangle2D* _rect;
  Ogre::MaterialPtr _material;
  Minesweeper * _minesweeper;
  //bool _key_pressed;

  SoundFXManager* _pSoundFXManager;
  SoundFXPtr _simpleEffect;
  bool _exitGame;

 private:
  Ogre::Ray setRayQuery(int posx, int posy, int mask);
  OIS::MouseState _mouse_position;
  bool _key_pressed;
};

#endif
