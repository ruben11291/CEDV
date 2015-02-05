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

#ifndef IntroState_H
#define IntroState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>

#include "GameState.h"

class IntroState : public Ogre::Singleton<IntroState>, public GameState
{
 public:

  IntroState();

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
  static IntroState& getSingleton ();
  static IntroState* getSingletonPtr ();
  
  bool start(const CEGUI::EventArgs &e);
  bool quit(const CEGUI::EventArgs &e);
  bool credit(const CEGUI::EventArgs &e);
  bool record(const CEGUI::EventArgs &e);
  
  //GUI
  void createGUI();
  void createBackground();
  
  bool initSDL();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  CEGUI::OgreRenderer* renderer; 
  CEGUI::Window* _sheet,* _loadButton,* _recordsButton, *_quitButton,* _newButton;
  Ogre::Rectangle2D* _rect;
  Ogre::OverlayManager* _overlayManager;
  Ogre::Overlay * _overlay;
  Ogre::SceneNode * _node;
  Ogre::MaterialPtr _material;
  
      // Manejadores del sonido
  
  TrackManager* _pTrackManager;
  SoundFXManager* _pSoundFXManager;
  TrackPtr _mainTrack;
  SoundFXPtr _simpleEffect;
  
  bool _exitGame;
};

#endif
