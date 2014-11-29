#include "IntroState.h"
#include "PlayState.h"
#include "CreditState.h"
#include "RecordState.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

IntroState::IntroState(){
  initSDL();
  _pTrackManager = NULL;
}

void
IntroState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  _pTrackManager = TrackManager::getSingletonPtr();
  _mainTrack = _pTrackManager->load("music.wav");   
  _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
  _camera = _sceneMgr->createCamera("IntroCamera");
  _camera->setPosition(Ogre::Vector3(5,20,20));
  _camera->lookAt(Ogre::Vector3(0,0,0));
  _camera->setNearClipDistance(5);
  _camera->setFarClipDistance(10000);
  
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  _viewport->setBackgroundColour(Ogre::ColourValue(1.0, 1.0, 1.0));
  double width = _viewport->getActualWidth();
  double height = _viewport->getActualHeight();
  _camera->setAspectRatio(width / height);
  
  _overlayManager = Ogre::OverlayManager::getSingletonPtr();
  _overlay = _overlayManager->getByName("Logo");
  _overlay->show();
  
  _exitGame = false;
  
  createBackground();
  
  createGUI();
  
  // Reproducción del track principal...
  this->_mainTrack->play();
  std::cout << "FIN DE ENTER INTRO STATE" << std::endl;
  
}

bool IntroState::initSDL () {
    // Inicializando SDL...
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return false;
    // Llamar a  SDL_Quit al terminar.
    atexit(SDL_Quit);
 
    // Inicializando SDL mixer...
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS, 4096) < 0)
      return false;
 
    // Llamar a Mix_CloseAudio al terminar.
    atexit(Mix_CloseAudio);
   
    return true;    
}

void IntroState::createBackground(){
  
    // Create background material
    _material = Ogre::MaterialManager::getSingleton().create("Background", "General");
    _material->getTechnique(0)->getPass(0)->createTextureUnitState("forest.jpg");
    _material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    _material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    _material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    
    // Create background rectangle covering the whole screen
    _rect = new Ogre::Rectangle2D(true);
    _rect->setCorners(-1.0, 1.0, 1.0, -1.0);
    _rect->setMaterial("Background");
    
    // Render the background before everything else
    _rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

    // Attach background to the scene
    _node = _sceneMgr->getRootSceneNode()->createChildSceneNode("B");
    //_sceneMgr->getRootSceneNode()->addChild(_node);
    _node->attachObject(_rect);
    
    // Example of background scrolling
    _material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.05, 0.0);
  
}

void IntroState::createGUI()
{
  //CEGUI
  renderer = &CEGUI::OgreRenderer::bootstrapSystem();
  CEGUI::Scheme::setDefaultResourceGroup("Schemes");
  CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
  CEGUI::Font::setDefaultResourceGroup("Fonts");
  CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
  CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

//   CEGUI::SchemeManager::getSingleton().create("WindowsLook.scheme");
  CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
  CEGUI::SchemeManager::getSingleton().create("OgreTray.scheme");
  CEGUI::System::getSingleton().setDefaultFont("DejaVuSans-10");
  CEGUI::System::getSingleton().setDefaultMouseCursor("OgreTrayImages","MouseArrow");

  //Sheet
  _sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Ex1/Sheet");
  
  _newButton = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button","Ex1/NewGameButton");
  _newButton->setText("New Game");
  _newButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.4,0),CEGUI::UDim(0.1,0)));
  _newButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0),CEGUI::UDim(0.42,0)));
  _newButton->subscribeEvent(CEGUI::PushButton::EventClicked,
 			     CEGUI::Event::Subscriber(&IntroState::start, 
						      this));
  _sheet->addChildWindow(_newButton);
  
  _loadButton = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button","Ex1/CreditsButton");
  _loadButton->setText("Credits");
  _loadButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.4,0),CEGUI::UDim(0.1,0)));
  _loadButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0),CEGUI::UDim(0.52,0)));
  _loadButton->subscribeEvent(CEGUI::PushButton::EventClicked,
 			     CEGUI::Event::Subscriber(&IntroState::credit, 
						      this));
  
  _sheet->addChildWindow(_loadButton);
  
  

  _recordsButton = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button","Ex1/RecordsButton");
  _recordsButton->setText("Records");
  _recordsButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.4,0),CEGUI::UDim(0.1,0)));
  _recordsButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0),CEGUI::UDim(0.62,0)));
  _sheet->addChildWindow(_recordsButton);
  _recordsButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			     CEGUI::Event::Subscriber(&IntroState::record,this));
  _sheet->addChildWindow(_recordsButton);

  
  CEGUI::System::getSingleton().setGUISheet(_sheet);
  
  _quitButton = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button","Ex1/QuitButton");
  _quitButton->setText("Quit");
  _quitButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.4,0),CEGUI::UDim(0.1,0)));
  _quitButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0),CEGUI::UDim(0.72,0)));
  _quitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			     CEGUI::Event::Subscriber(&IntroState::quit, 
						      this));
  _sheet->addChildWindow(_quitButton);
  
}

bool IntroState::start (const CEGUI::EventArgs &e){
  

  pushState(PlayState::getSingletonPtr());
  std::cout << "intro state pushing playstate"<<std::endl;
  return true;
}

bool IntroState::quit (const CEGUI::EventArgs &e){
  
  _exitGame = true;
  return true;
}

bool IntroState::credit (const CEGUI::EventArgs &e){
  
  pushState(CreditState::getSingletonPtr());
  
  return true;
}

bool IntroState::record (const CEGUI::EventArgs &e){
  
  pushState(RecordState::getSingletonPtr());
  
  return true;
}


void
IntroState::exit()
{  
  std::cout << "EXIT INTROSTATE"<<std::endl;
  CEGUI::WindowManager::getSingleton().destroyWindow("Ex1/QuitButton");
  CEGUI::WindowManager::getSingleton().destroyWindow("Ex1/RecordsButton");
  CEGUI::WindowManager::getSingleton().destroyWindow("Ex1/CreditsButton");
  CEGUI::WindowManager::getSingleton().destroyWindow("Ex1/Sheet");
 
  CEGUI::WindowManager::getSingleton().destroyWindow("Ex1/Sheet");
std::cout << "exit"<<std::endl;
  Ogre::MaterialManager::getSingleton().remove("Background");
std::cout << "exit"<<std::endl;
  _node->detachAllObjects();
  _sceneMgr->destroySceneNode(_node);
  std::cout << "exit"<<std::endl;
  delete _rect;
   _root->getAutoCreatedWindow()->removeAllViewports();
  _sceneMgr->clearScene();
  std::cout << "exit"<<std::endl;
  
}

void
IntroState::pause ()
{
  _overlay->hide();
  CEGUI::WindowManager::getSingleton().getWindow("Ex1/QuitButton")->hide();
 CEGUI::WindowManager::getSingleton().getWindow("Ex1/RecordsButton")->hide();
  CEGUI::WindowManager::getSingleton().getWindow("Ex1/CreditsButton")->hide();
  CEGUI::WindowManager::getSingleton().getWindow("Ex1/Sheet")->hide();
  _node->setVisible(false,true);

}


void
IntroState::resume ()
{
  std::cout << "resume"<<std::endl;
  _overlay->show();
  CEGUI::WindowManager::getSingleton().getWindow("Ex1/QuitButton")->show(); 

  CEGUI::WindowManager::getSingleton().getWindow("Ex1/RecordsButton")->show();
  CEGUI::WindowManager::getSingleton().getWindow("Ex1/CreditsButton")->show();
  CEGUI::WindowManager::getSingleton().getWindow("Ex1/Sheet")->show();
  std::cout << "NODE "<< _node<<std::endl;
  _node->setVisible(true,false);
  std::cout <<"RESUME END"<<std::endl;
}

bool
IntroState::frameStarted
(const Ogre::FrameEvent& evt) 
{
  return true;
}

bool
IntroState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
IntroState::keyPressed
(const OIS::KeyEvent &e)
{
  // Transición al siguiente estado.
  // Espacio --> PlayState
  if (e.key == OIS::KC_SPACE) {
    pushState(PlayState::getSingletonPtr());
  }
}

void
IntroState::keyReleased
(const OIS::KeyEvent &e )
{
  if (e.key == OIS::KC_ESCAPE) {
      _exitGame = true;
  }
}

void
IntroState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
IntroState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
IntroState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

IntroState*
IntroState::getSingletonPtr ()
{
return msSingleton;
}

IntroState&
IntroState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}
