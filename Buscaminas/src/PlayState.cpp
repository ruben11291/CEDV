#include "PlayState.h"
#include "PauseState.h"
#include <sstream>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void
PlayState::enter ()
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
  
  _pSoundFXManager = new SoundFXManager;
  _simpleEffect = _pSoundFXManager->load("bomb.wav");
  
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  // Nuevo background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
  
  double width = _viewport->getActualWidth();
  double height = _viewport->getActualHeight();
  _camera->setAspectRatio(width / height);
  
  _cubes = new std::vector<Ogre::SceneNode*>();//for locating the cubes when a click was performed
  Ogre::Entity* ent1 = _sceneMgr->createEntity("Cube1.mesh");
  ent1->setQueryFlags(STAGE);

  Ogre::Vector3 bbSize = ent1->getMesh()->getBounds().getSize();
  
  _fnode = _sceneMgr->createSceneNode("aux");
  // _fnode->setPosition(0,4,0.0);
  //_fnode->setVisible(False);
  _fnode->attachObject(ent1);
  _sceneMgr->getRootSceneNode()->addChild(_fnode);
  std::cout << _fnode->getPosition() << " " << _fnode->_getDerivedPosition() << " " << _fnode->getInitialPosition() << _fnode->getLocalAxes()<< std::endl;
  // _sceneMgr->destroyEntity(ent1);

  int n = 5;//to be done , only for testing
  //  creation of the cube 
  for (int i=0;i<n;i++){
    for (int j=0;j<n;j++){
      //base
      Ogre::Entity* ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      //    std::stringstream ss;//create a stringstream
      //    ss << j;//add number to the stream
      Ogre::SceneNode* _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _fnode->addChild(_node);

      _node->_setDerivedPosition(Ogre::Vector3(float(i*bbSize.x),0.1,float(j*bbSize.x)));
      _node->scale(1,0.1,1);
      std::cout << _node->getPosition() << " " << _node->_getDerivedPosition() << " " << _node->getInitialPosition() << _node->getLocalAxes()<< std::endl;
//       _sceneMgr->getRootSceneNode()->addChild(_node);
      _cubes->push_back(_node);

      //tapa
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _fnode->addChild(_node);

      _node->_setDerivedPosition(Ogre::Vector3(float(i*bbSize.x),float(n*bbSize.x+0.05),float(j*bbSize.x)));
      _node->scale(1,0,1);
std::cout << _node->getPosition() << " " << _node->_getDerivedPosition() << " " << _node->getInitialPosition() << _node->getLocalAxes()<< std::endl;
//       _sceneMgr->getRootSceneNode()->addChild(_node);
      _cubes->push_back(_node);

      //lateral izq
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _fnode->addChild(_node);
      _node->scale(1,0,1);
      _node->roll(Ogre::Degree(90),Ogre::Node::TS_PARENT);
      _node->_setDerivedPosition(Ogre::Vector3(-bbSize.x + 0.25,float(i*bbSize.x + 0.3),float(j*bbSize.x)));
//       _sceneMgr->getRootSceneNode()->addChild(_node);
      _cubes->push_back(_node);
std::cout << _node->getPosition() << " " << _node->_getDerivedPosition() << " " << _node->getInitialPosition() << _node->getLocalAxes()<< std::endl;
      //lateral derecho
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _fnode->addChild(_node);
      _node->_setDerivedPosition(Ogre::Vector3(n*bbSize.x+bbSize.x-0.75, float(i*bbSize.x +0.3),float(j*bbSize.x)));
      _node->scale(1,0,1);
      _node->roll(Ogre::Degree(90),Ogre::Node::TS_PARENT);
//       _sceneMgr->getRootSceneNode()->addChild(_node);
      _cubes->push_back(_node);
std::cout << _node->getPosition() << " " << _node->_getDerivedPosition() << " " << _node->getInitialPosition() << _node->getLocalAxes()<< std::endl;
      //tapa trasera
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _fnode->addChild(_node);
      _node->_setDerivedPosition(Ogre::Vector3(float(i*bbSize.x),float(j*bbSize.x+0.3),-bbSize.x +0.25));
      _node->scale(1,0,1);
      _node->pitch(Ogre::Degree(-90),Ogre::Node::TS_PARENT);
//       _sceneMgr->getRootSceneNode()->addChild(_node);
      _cubes->push_back(_node);
std::cout << _node->getPosition() << " " << _node->_getDerivedPosition() << " " << _node->getInitialPosition() << _node->getLocalAxes()<< std::endl;
      //frontal
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _fnode->addChild(_node);
      _node->_setDerivedPosition(Ogre::Vector3(float(i*bbSize.x),float(j*bbSize.x+0.3),n*bbSize.x+bbSize.x-0.75));
      _node->scale(1,0,1);
      _node->pitch(Ogre::Degree(-90),Ogre::Node::TS_PARENT);
//       _sceneMgr->getRootSceneNode()->addChild(_node);
      _cubes->push_back(_node);std::cout << _node->getPosition() << " " << _node->_getDerivedPosition() << " " << _node->getInitialPosition() << _node->getLocalAxes()<< std::endl;  
    }
    //  Ogre::Vector3 mediumPointXZ = (cubes[0]->getPosition()+cubes[n-1]->getPosition()) / 2;
    //Ogre::Vector3 mediumPointYZ = (cubes[]
    //Ogre::Vector2 mediumPointXY(_cubes[0]->getPosition()
    _raySceneQuery = _sceneMgr->createRayQuery(Ogre::Ray());
    
  }
  
  
//   _sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
//   _sceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
//   
//   
//   _light = _sceneMgr->createLight("Light1");
//   _light->setType(Ogre::Light::LT_DIRECTIONAL);
//   _light->setDirection(Ogre::Vector3(1,-1,0));
// 
//   _light2 = _sceneMgr->createLight("Light2");
//   _light2->setType(Ogre::Light::LT_POINT);
//   _light2->setPosition(8, 15, -2);
//   _light2->setSpecularColour(0.9, 0.9, 0.9); 
//   _light2->setDiffuseColour(0.9, 0.9, 0.9);
// 
//   Ogre::Plane plane1(Ogre::Vector3::UNIT_Y, 0);
//   Ogre::MeshManager::getSingleton().createPlane("plane1",
// 	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
// 	200,200,1,1,true,1,10,10,Ogre::Vector3::UNIT_Z);
// 
//   _ground = _sceneMgr->createSceneNode("ground");
//   Ogre::Entity* groundEnt = _sceneMgr->createEntity("planeEnt", "plane1");
//   groundEnt->setQueryFlags(STAGE);
// 
//   groundEnt->setMaterialName("Ground");
//   _ground->attachObject(groundEnt);//comprobar si se libera sola la entidad
//   _sceneMgr->getRootSceneNode()->addChild(_ground);
  
  
  // Create background material
    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("Backgr", "General");
    material->getTechnique(0)->getPass(0)->createTextureUnitState("back.jpg");
    material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    
    // Create background rectangle covering the whole screen
    _rect = new Ogre::Rectangle2D(true);
    _rect->setCorners(-1.0, 1.0, 1.0, -1.0);
    _rect->setMaterial("Backgr");
    
    // Render the background before everything else
    _rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);
    
    // Use infinite AAB to always stay visible
    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    _rect->setBoundingBox(aabInf);
    
    // Attach background to the scene
    Ogre::SceneNode* node = _sceneMgr->getRootSceneNode()->createChildSceneNode("Back");
    node->attachObject(_rect);
  

  _overlayManager = Ogre::OverlayManager::getSingletonPtr();
  _overlay = _overlayManager->getByName("Info");
  _overlay->show();

  _exitGame = true;
}

void
PlayState::exit ()
{ 
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();

  delete _cubes;
 
}

void
PlayState::pause()
{
  _overlay->hide();
}

void
PlayState::resume()
{
  // Se restaura el background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
  _overlay->show();
}

bool
PlayState::frameStarted
(const Ogre::FrameEvent& evt)
{
  deltaT = evt.timeSinceLastFrame;
  int fps = 1.0 / deltaT;

  Ogre::OverlayElement *oe;
  oe = _overlayManager->getOverlayElement("fpsInfo");
//   oe->setCaption(Ogre::StringConverter::toString(fps));
  oe->setCaption("600/600");
  
  oe = _overlayManager->getOverlayElement("minesinf");
  oe->setCaption("30/30");
  
  oe = _overlayManager->getOverlayElement("timeinf");
  oe->setCaption("0.00");
  return true;
}

bool
PlayState::frameEnded
(const Ogre::FrameEvent& evt)
{
  return _exitGame;
}

void
PlayState::keyPressed
(const OIS::KeyEvent &e)
{
  // Tecla p --> PauseState.
  if (e.key == OIS::KC_P) {
    pushState(PauseState::getSingletonPtr());
  }
  
  Ogre::Vector3 vt(0,0,0);     Ogre::Real tSpeed = 20.0;
  Ogre::Real deltaT = 0.03;
  
 
  if(e.key == OIS::KC_UP){ vt+=Ogre::Vector3(0,0,-1); _camera->moveRelative(vt * deltaT * tSpeed);}
  if(e.key == OIS::KC_DOWN){  vt+=Ogre::Vector3(0,0,1); _camera->moveRelative(vt * deltaT * tSpeed);}
  if(e.key == OIS::KC_LEFT){  vt+=Ogre::Vector3(-1,0,0); _camera->moveRelative(vt * deltaT * tSpeed);}
  if(e.key == OIS::KC_RIGHT){ vt+=Ogre::Vector3(1,0,0); _camera->moveRelative(vt * deltaT * tSpeed);}
  
//   Ogre::Real r =0;
//   if(e.key == OIS::KC_W){
//     r+=180;
//     //for (std::vector<Ogre::SceneNode*>::iterator it=_cubes->begin();it!=_cubes->end();it++)
//     //(*it)->yaw(Ogre::Degree(r*0.1));
//     _camera->yaw(Ogre::Degree(r*0.03));
//   }
//   
//   if(e.key == OIS::KC_Q){
//     r-=180;
//     //for (std::vector<Ogre::SceneNode*>::iterator it=_cubes->begin();it!=_cubes->end();it++)
//     //(*it)->yaw(Ogre::Degree(r*0.1));
//     _camera->yaw(Ogre::Degree(r*0.03));
// 
//   }
//   
//   if(e.key == OIS::KC_Z){
//     r+=180;
//     //for (std::vector<Ogre::SceneNode*>::iterator it=_cubes->begin();it!=_cubes->end();it++)
//     //(*it)->yaw(Ogre::Degree(r*0.1));
//     _camera->pitch(Ogre::Degree(r*0.03));
//   }
//   
//   if(e.key == OIS::KC_X){
//     r-=180;
//     //for (std::vector<Ogre::SceneNode*>::iterator it=_cubes->begin();it!=_cubes->end();it++)
//     //(*it)->yaw(Ogre::Degree(r*0.1));
//     _camera->pitch(Ogre::Degree(r*0.03));
// 
//   }
//   Ogre::Vector3 vt(0,0,0);
//    Ogre::Real tSpeed = 10.0; 
//   if(e.key == OIS::KC_S){
// 
//     vt+= Ogre::Vector3(0,0,-10)*0.05 * 7;   
//     _camera->moveRelative(vt * 0.05 * tSpeed);
//   }
//    if(e.key == OIS::KC_A){
//       Ogre::Vector3 vt(0,0,0);
//     Ogre::Real tSpeed = 10.0; 
//     vt-= Ogre::Vector3(0,0,-10)*0.05 * 7;   
//     _camera->moveRelative(vt * 0.05 * tSpeed);
//   }
//   
//   
//   if(e.key == OIS::KC_E){
//     r+=180;
//     //for (std::vector<Ogre::SceneNode*>::iterator it=_cubes->begin();it!=_cubes->end();it++)
//     //(*it)->yaw(Ogre::Degree(r*0.1));
//     _fnode->yaw(Ogre::Degree(r*0.1),Ogre::Node::TS_WORLD);
//   }
//   
//   if(e.key == OIS::KC_R){
//     r-=180;
//     //for (std::vector<Ogre::SceneNode*>::iterator it=_cubes->begin();it!=_cubes->end();it++)
//     //(*it)->yaw(Ogre::Degree(r*0.1));
//      _fnode->yaw(Ogre::Degree(r*0.1),Ogre::Node::TS_WORLD);
// 
//   }
//   
//   if(e.key == OIS::KC_D){
//     r+=180;
//     //for (std::vector<Ogre::SceneNode*>::iterator it=_cubes->begin();it!=_cubes->end();it++)
//     //(*it)->yaw(Ogre::Degree(r*0.1));
//     _fnode->pitch(Ogre::Degree(r*0.1),Ogre::Node::TS_WORLD);
//   }
//   
//   if(e.key == OIS::KC_F){
//     r-=180;
//     //for (std::vector<Ogre::SceneNode*>::iterator it=_cubes->begin();it!=_cubes->end();it++)
//     //(*it)->yaw(Ogre::Degree(r*0.1));
//      _fnode->pitch(Ogre::Degree(r*0.1),Ogre::Node::TS_WORLD);
//       _simpleEffect->play();
//   }
//   
  if(e.key == OIS::KC_SPACE){
    _simpleEffect->play();
  }
  
  
}

void
PlayState::keyReleased
(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = false;
  }
}

void
PlayState::mouseMoved
(const OIS::MouseEvent &e)
{
  //  Ogre::Vector3 destinity(_lastposition-(Ogre::Vector3(e.state.X.abs,e.state.Y.abs,e.state.Z.abs)));
  //_camera->moveRelative(*_lastposition-Ogre::Vector3(e.state.X.abs,e.state.Y.abs,e.state.Z.abs));
}

void
PlayState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  std::cout <<  id << std::endl;
  // Ogre::Real deltaT = e.timeSinceLastFrame;deberia ir en frameupdate y actualizar lo que aqui se modifique
  //int fps = 1.0 / deltaT;
  int posx = e.state.X.abs;
  int posy = e.state.Y.abs;
  int posz_rel = e.state.Z.rel;
  float rotx,roty;
  Ogre::Vector3 vt(0,0,0);     
  Ogre::Real tSpeed = 10.0;  
  Ogre::Ray r;
  Ogre::RaySceneQueryResult result;
  Ogre::RaySceneQueryResult::iterator it;
  Ogre::SceneNode * _selectedNode;

 // Si usamos la rueda, desplazamos en Z la camara ------------------
  //  vt+= Ogre::Vector3(0,0,-10)*deltaT * posz_rel;   
  //_camera->moveRelative(vt * deltaT * tSpeed);

   r = setRayQuery(posx, posy, CUBE);
   result = _raySceneQuery->execute();
   
    it = result.begin();
    if (it!=result.end()){
      _selectedNode = it->movable->getParentSceneNode();
    }
    else _selectedNode=0;

  switch(id){
  case OIS::MB_Left:
    std::cout << "Boton Izquierdo" << std::endl;
    if (_selectedNode){
      _selectedNode->showBoundingBox(true);
      static_cast<Ogre::Entity *>(_selectedNode->getAttachedObject(0))->setMaterialName("Cube2");
    }
    break;
  case OIS::MB_Middle:
    std::cout << "Boton centro" << std::endl;
    // float rotx = posx * deltaT * -1;
    //float roty = posy * deltaT * -1;
    //if (_lastposition)
    // delete _lastposition;
    // this->_lastposition= new Ogre::Vector3(e.state.X.abs,e.state.Y.abs,e.state.Z.abs); 
      //rotx = posx * deltaT*-1;
      //roty = posy *  deltaT*-1;
      //  _camera->yaw(Ogre::Radian(rotx));
      //_camera->pitch(Ogre::Radian(roty));
    
    break;
  case OIS::MB_Right:
    std::cout << "Boton derecho "<<std::endl;break;
    if (_selectedNode){
      _selectedNode->showBoundingBox(true);
    static_cast<Ogre::Entity *>(_selectedNode->getAttachedObject(0))->setMaterialName("Cube2");
    }
  default:
    std::cout << "Default"<<std::endl;break;
  }
  
// if (mbmiddle) { // Con boton medio pulsado, rotamos camara ---------
//     float rotx = _mouse->getMouseState().X.rel * deltaT * -1;
//     float roty = _mouse->getMouseState().Y.rel * deltaT * -1;
//     _camera->yaw(Radian(rotx));
//     _camera->pitch(Radian(roty));
//     cout << "Boton Medio" << endl;
//   }
 
  
  //if(e.state.buttonDown(OIS::MB_Left)){
//     Ogre::Vector3 vt(0,0,0);
//     Ogre::Real tSpeed = 10.0; 
//     vt+= Ogre::Vector3(0,0,-10)*0.05 * e.state.Z.rel;   
//     _camera->moveRelative(vt * 0.05 * tSpeed); 

}

void
PlayState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{

 

  // float rotx = e.X.rel * deltaT * -1;
  // float roty = e.Y.rel * deltaT * -1;
  // _camera->yaw(Ogre::Radian(rotx));
  // _camera->pitch(Ogre::Radian(roty));

}




PlayState*
PlayState::getSingletonPtr ()
{
return msSingleton;
}

PlayState&
PlayState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}


Ogre::Ray PlayState::setRayQuery(int posx, int posy, int mask) {
   Ogre::Ray rayMouse = _camera->getCameraToViewportRay
    (posx/float(_root->getAutoCreatedWindow()->getWidth()), posy/float(_root->getAutoCreatedWindow()->getHeight()));
   _raySceneQuery->setRay(rayMouse);
   _raySceneQuery->setSortByDistance(true);
   _raySceneQuery->setQueryMask(mask);
   return (rayMouse);
}
