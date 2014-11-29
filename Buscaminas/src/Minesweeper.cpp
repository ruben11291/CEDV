#include "Minesweeper.h"

Minesweeper::Minesweeper(int squares,Ogre::SceneManager * sceneMgr):_sceneMgr(sceneMgr){
  _node =  _sceneMgr->createSceneNode("aux");
  _node->setPosition(0.0,4,0.0);
  _sceneMgr->getRootSceneNode()->addChild(_node);
  Ogre::Entity* ent1 = _sceneMgr->createEntity("Cube1.mesh");
  Ogre::Vector3 bbSize = ent1->getMesh()->getBounds().getSize();
  _sceneMgr->destroyEntity(ent1);
  std::cout << "antes creacion tablero"<< std::endl;
  _table = new Table(squares,float(bbSize.x) ,_sceneMgr, _node);

  
}
Minesweeper::~Minesweeper(){
  delete _table;
  _sceneMgr->destroySceneNode(_node);

}

void
Minesweeper::hide(){
  _node->setVisible(false,true);
}
void
Minesweeper::show(){
  _node->setVisible(true,true);
}

void
Minesweeper::pitch(Ogre::Degree degree){
  _node->pitch(degree,Ogre::Node::TS_WORLD);
}
void
Minesweeper::yaw(Ogre::Degree degree){
  _node->yaw(degree,Ogre::Node::TS_WORLD);
}

bool 
Minesweeper::isValid(int face, int row, int column){
  return _table->isValid(face,row,column);
}

bool 
Minesweeper::isGameOver(){
  return true;
}
bool 
Minesweeper::isWin(){
  return true;
}

void 
Minesweeper::assignCube(std::vector<Ogre::SceneNode *> *nodes){
  for (std::vector<Ogre::SceneNode*>::iterator it = nodes->begin();it!=nodes->end();it++)
    _node->addChild(*it);
}



void 
Minesweeper::setMov(int face, int row, int column){
  _table->setMov(face,row,column);
}
