#include "LevelState.h"
#include "PlayState.h"

template<> LevelState* Ogre::Singleton<LevelState>::msSingleton = 0;

void
LevelState::enter ()
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
  
  double width = _viewport->getActualWidth();
  double height = _viewport->getActualHeight();
  _camera->setAspectRatio(width / height);
  
  createBackground();
  createGUI();

  _exitGame = true;
}



void LevelState::createBackground(){
  
    // Create background material
  Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("Backgro", "General");
  material->getTechnique(0)->getPass(0)->createTextureUnitState("space2.jpg");
  material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
  material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
  material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
  
  // Create background rectangle covering the whole screen
  _rect = new Ogre::Rectangle2D(true);
  _rect->setCorners(-1.0, 1.0, 1.0, -1.0);
  _rect->setMaterial("Backgro");
  
  //   Render the background before everything else
  _rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);
  
  // Attach background to the scene
  _node = _sceneMgr->getRootSceneNode()->createChildSceneNode("Backgro");
  _node->attachObject(_rect);
}

void LevelState::createGUI()
{
  //Sheet
  _sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Ex1/Sheete");
  
  _level1 = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button","Ex1/Level1Button");
  _level1->setText("Facil");
  _level1->setSize(CEGUI::UVector2(CEGUI::UDim(0.4,0),CEGUI::UDim(0.1,0)));
  _level1->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0),CEGUI::UDim(0.22,0)));
  _level1->subscribeEvent(CEGUI::PushButton::EventClicked,
 			     CEGUI::Event::Subscriber(&LevelState::start, 
						      this));
  _sheet->addChildWindow(_level1);
  
  _level2 = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button","Ex1/Level2Button");
  _level2->setText("Normal");
  _level2->setSize(CEGUI::UVector2(CEGUI::UDim(0.4,0),CEGUI::UDim(0.1,0)));
  _level2->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0),CEGUI::UDim(0.42,0)));
//   _level2->subscribeEvent(CEGUI::PushButton::EventClicked,
//  			     CEGUI::Event::Subscriber(&IntroState::credit, 
// 						      this));
  
  _sheet->addChildWindow(_level2);
  
  

  _level3 = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button","Ex1/Level3Button");
  _level3->setText("Dificil");
  _level3->setSize(CEGUI::UVector2(CEGUI::UDim(0.4,0),CEGUI::UDim(0.1,0)));
  _level3->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0),CEGUI::UDim(0.62,0)));
  _sheet->addChildWindow(_level3);
//   _level3->subscribeEvent(CEGUI::PushButton::EventClicked,
// 			     CEGUI::Event::Subscriber(&IntroState::record,this));
  _sheet->addChildWindow(_level3);

  
  CEGUI::System::getSingleton().setGUISheet(_sheet);
  
}
bool LevelState::start (const CEGUI::EventArgs &e){
  

  pushState(PlayState::getSingletonPtr());
  return true;
}


void
LevelState::exit ()
{ 
  std::cout << "EXIT LEVELSTATE"<<std::endl;
//   CEGUI::WindowManager::getSingleton().destroyWindow("Ex1/Level4Button");
  CEGUI::WindowManager::getSingleton().destroyWindow("Ex1/Level3Button");
  CEGUI::WindowManager::getSingleton().destroyWindow("Ex1/Level2Button");
  CEGUI::WindowManager::getSingleton().destroyWindow("Ex1/Sheete");
 
  CEGUI::WindowManager::getSingleton().destroyWindow("Ex1/Sheete");
std::cout << "exit"<<std::endl;
  Ogre::MaterialManager::getSingleton().remove("Backgro");
std::cout << "exit"<<std::endl;
//   _overlayManager->destroyAll();
  _node->detachAllObjects();
  _sceneMgr->destroySceneNode(_node);
  std::cout << "exit"<<std::endl;
  delete _rect;
  //  _root->getAutoCreatedWindow()->removeAllViewports();
   // _sceneMgr->clearScene();
  std::cout << "exit"<<std::endl;
}

void
LevelState::pause()
{
//   _overlay->hide();
//   CEGUI::WindowManager::getSingleton().getWindow("Ex1/Level4Button")->hide();
 CEGUI::WindowManager::getSingleton().getWindow("Ex1/Level3Button")->hide();
  CEGUI::WindowManager::getSingleton().getWindow("Ex1/Level2Button")->hide();
  CEGUI::WindowManager::getSingleton().getWindow("Ex1/Sheete")->hide();
  _node->setVisible(false,true);
}

void
LevelState::resume()
{
 std::cout << "resume"<<std::endl;
//   _overlay->show();
//   CEGUI::WindowManager::getSingleton().getWindow("Ex1/Level4Button")->show(); 

  CEGUI::WindowManager::getSingleton().getWindow("Ex1/Level3Button")->show();
  CEGUI::WindowManager::getSingleton().getWindow("Ex1/Level2Button")->show();
  CEGUI::WindowManager::getSingleton().getWindow("Ex1/Sheete")->show();
  _node->setVisible(true,false);
  std::cout <<"RESUME END"<<std::endl;
}

bool
LevelState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
LevelState::frameEnded
(const Ogre::FrameEvent& evt)
{
  return _exitGame;
}

void
LevelState::keyPressed
(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_P) {
    popState();
  }
  
}

void
LevelState::keyReleased
(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = false;
  }
}

void
LevelState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
LevelState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{

  
}

void
LevelState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

LevelState*
LevelState::getSingletonPtr ()
{
return msSingleton;
}

LevelState&
LevelState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}


