#include "RecordState.h"


template<> RecordState* Ogre::Singleton<RecordState>::msSingleton = 0;

void
RecordState::enter ()
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
  

  _exitGame = true;
}



void RecordState::createBackground(){
  
    // Create background material
  Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("Backr", "General");
  material->getTechnique(0)->getPass(0)->createTextureUnitState("space.jpg");
  material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
  material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
  material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
  
  // Create background rectangle covering the whole screen
  _rect = new Ogre::Rectangle2D(true);
  _rect->setCorners(-1.0, 1.0, 1.0, -1.0);
  _rect->setMaterial("Backr");
  
  //   Render the background before everything else
  _rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);
  
  // Attach background to the scene
  _node = _sceneMgr->getRootSceneNode()->createChildSceneNode("Backr");
  _node->attachObject(_rect);
  
  // Example of background scrolling
  material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.02, 0.0);
  
  _overlayManager = Ogre::OverlayManager::getSingletonPtr();
  _overlay = _overlayManager->getByName("Record");
  _overlay->show();
}


void
RecordState::exit ()
{ 
  std::cout << "Record state exit" << std::endl;
  delete _rect;
  _sceneMgr->destroySceneNode(_node);
  _overlay->hide();
}

void
RecordState::pause()
{
}

void
RecordState::resume()
{
}

bool
RecordState::frameStarted
(const Ogre::FrameEvent& evt)
{
  std::string matriz[5], names[100];
  std::string line, tempname, name;
  std::ifstream file;
  int c = 0;
  float time, temp;
  float times[100];
  
  file.open("records.txt");
  //Se leen los records del fichero
  while(getline(file,line)){
    std::stringstream convertor(line);
    convertor >> name  >> time;
    names[c] = name;
    times[c] = time;
    c++;
  }
  file.close();
  //Se ordenan los records 
  for(int i=0;i<c;i++){
    for(int j=0;j<c-1;j++){
      if(times[j]>times[j+1]){// cambia "<" a ">" para cambiar la manera de ordenar
	temp=times[j];
	tempname=names[j];
	times[j]=times[j+1];
	names[j]=names[j+1];
	times[j+1]=temp;
	names[j+1]=tempname;
      }
    }
  }
  
  if(c > 5){c = 5;}
  //Se almacenan los 5 primeros y se muestran en el overlay
  for(int i=0; i<c; i++){
    std::stringstream top;
    top << names[i] << " " << times[i] <<std::endl;
    matriz[i] = top.str();
  }
  
  Ogre::OverlayElement *oe;
  oe = _overlayManager->getOverlayElement("record1");
  oe->setCaption(matriz[0]);
  oe = _overlayManager->getOverlayElement("record2");
  oe->setCaption(matriz[1]);
  oe = _overlayManager->getOverlayElement("record3");
  oe->setCaption(matriz[2]);
  oe = _overlayManager->getOverlayElement("record4");
  oe->setCaption(matriz[3]);
  oe = _overlayManager->getOverlayElement("record5");
  oe->setCaption(matriz[4]);
  
  return true;
}

std::string RecordState::convert(std::string name, double time){
  std::stringstream c;
  c << name << "  " << time <<"s";
  return c.str();
}

bool
RecordState::frameEnded
(const Ogre::FrameEvent& evt)
{
  return _exitGame;
}

void
RecordState::keyPressed
(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_P) {
    popState();
  }
  
}

void
RecordState::keyReleased
(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = false;
  }
}

void
RecordState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
RecordState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{

  
}

void
RecordState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

RecordState*
RecordState::getSingletonPtr ()
{
return msSingleton;
}

RecordState&
RecordState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}


