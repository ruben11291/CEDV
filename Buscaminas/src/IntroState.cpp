#include "IntroState.h"
#include "PlayState.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void
IntroState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

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
  
  _exitGame = false;
  
  createBackground();
  
  createGUI();
  
  _root->startRendering();
  
}

void IntroState::createBackground(){
  
    // Create background material
    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("Background", "General");
    material->getTechnique(0)->getPass(0)->createTextureUnitState("wood.jpg");
    material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    
    // Create background rectangle covering the whole screen
    _rect = new Ogre::Rectangle2D(true);
    _rect->setCorners(-1.0, 1.0, 1.0, -1.0);
    _rect->setMaterial("Background");
    
    // Render the background before everything else
    _rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);
    
    // Use infinite AAB to always stay visible
    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    _rect->setBoundingBox(aabInf);
    
    // Attach background to the scene
    Ogre::SceneNode* node = _sceneMgr->getRootSceneNode()->createChildSceneNode("Background");
    node->attachObject(_rect);
    
    // Example of background scrolling
    material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.05, 0.0);
  
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
  //CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
  CEGUI::SchemeManager::getSingleton().create("OgreTray.scheme");
  CEGUI::System::getSingleton().setDefaultFont("DejaVuSans-10");
  CEGUI::System::getSingleton().setDefaultMouseCursor("OgreTrayImages","MouseArrow");

  //Sheet
  _sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Ex1/Sheet");
  
  CEGUI::Window* newButton = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button","Ex1/NewGameButton");
  newButton->setText("New Game");
  std::cout << "ID NEW: " << newButton->getID() << std::endl;
  newButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.4,0),CEGUI::UDim(0.2,0)));
  newButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0),CEGUI::UDim(0.1,0)));
  newButton->subscribeEvent(CEGUI::PushButton::EventClicked,
 			     CEGUI::Event::Subscriber(&IntroState::start, 
						      this));
  _sheet->addChildWindow(newButton);
  
  CEGUI::Window* loadButton = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button","Ex1/CreditsButton");
  loadButton->setText("Credits");
  std::cout << "ID LOAD: " << newButton->getID() << std::endl;
  loadButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.4,0),CEGUI::UDim(0.2,0)));
  loadButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0),CEGUI::UDim(0.35,0)));
  _sheet->addChildWindow(loadButton);
  
  CEGUI::Window* quitButton = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button","Ex1/QuitButton");
  quitButton->setText("Quit");
  std::cout << "ID QUIT: " << newButton->getID() << std::endl;
  quitButton->setSize(CEGUI::UVector2(CEGUI::UDim(0.4,0),CEGUI::UDim(0.2,0)));
  quitButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0),CEGUI::UDim(0.6,0)));
  quitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			     CEGUI::Event::Subscriber(&IntroState::quit, 
						      this));
  _sheet->addChildWindow(quitButton);
  
  CEGUI::System::getSingleton().setGUISheet(_sheet);
}

bool IntroState::start (const CEGUI::EventArgs &e){
  
  changeState(PlayState::getSingletonPtr());
  
  return true;
}

bool IntroState::quit (const CEGUI::EventArgs &e){
  
  _exitGame = true;
  return true;
}

void
IntroState::exit()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
  unsigned int newi = 0;
  _sheet->removeChildWindow(newi);
  _sheet->removeChildWindow(newi);
  _sheet->removeChildWindow(newi);
  delete _rect;
}

void
IntroState::pause ()
{
}

void
IntroState::resume ()
{
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
    changeState(PlayState::getSingletonPtr());
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
