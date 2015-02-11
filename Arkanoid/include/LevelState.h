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

#ifndef LevelState_H
#define LevelState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <string>
#include <sstream>
#include <fstream>
#include "GameState.h"
#include <CEGUI.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>

class LevelState : public Ogre::Singleton<LevelState>, public GameState
{
 public:
  LevelState () {}

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
  static LevelState& getSingleton ();
  static LevelState* getSingletonPtr ();
  
  std::string convert(std::string name, double time);
  

  bool quit(const CEGUI::EventArgs &e);
  bool start(const CEGUI::EventArgs &e);
  //GUI
  void createBackground();
  void createGUI();
  
 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  Ogre::OverlayManager* _overlayManager;
  Ogre::Overlay *_overlay;
  Ogre::Rectangle2D* _rect;
  CEGUI::OgreRenderer* renderer; 
  CEGUI::Window* _sheet,* _level1,* _level2, *_level3;
  Ogre::SceneNode * _node;
  bool _exitGame;


};

#endif
