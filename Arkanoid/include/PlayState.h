/*********************************************************************
 * Módulo 1. Curso de Experto en Desarrollo de Videojuegos
 * Autor: Ruben Perez Pascual ruben.perez@alu.uclm.es
 * Autor: Angel Peralta Lopez angel.peralta@alu.uclm.es
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  
 *********************************************************************/
#ifndef PlayState_H
#define PlayState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <OISKeyboard.h>
#include "GameState.h"
#include "Level.h"
#include <vector>
#include <sstream>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <fstream>
#include <OgreBulletDynamicsRigidBody.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"	
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"	


using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;
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
  Ogre::Camera* _camera;
  Ogre::OverlayManager* _overlayManager;
  Ogre::SceneNode* _ground;
  Ogre::Light* _light;
  Ogre::Real deltaT;
  Ogre::Rectangle2D* _rect;
  Ogre::MaterialPtr _material;
  bool _exitGame;
  int _lifes; //cambiar luego-> crear clase Arkanoid <lista de levels> <caracteristicas horizontales a todos>
  std::vector<Level*> _levels;
  Level * _level;
  int _difficult;
  void setDifficult(int);

 private:
  OIS::MouseState _mouse_position;
  bool _key_pressed;
  OIS::KeyCode _event;
  bool _end_game;
  bool _pick;
  Ogre::Real _time_count,_last_time;
  void gameOver();
  void gameWin();
  bool checkWin(Level&);
  void resetBall();
};

#endif
