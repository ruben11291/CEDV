#include "Minesweeper.h"

Minesweeper::Minesweeper(int squares,Ogre::SceneManager * sceneMgr):_sceneMgr(sceneMgr){
  _node =  _sceneMgr->createSceneNode("aux");
  _node->setPosition(0.0,4,0.0);
  _sceneMgr->getRootSceneNode()->addChild(_node);
  Ogre::Entity* ent1 = _sceneMgr->createEntity("Cube1.mesh");
  Ogre::Vector3 bbSize = ent1->getMesh()->getBounds().getSize();
  _sceneMgr->destroyEntity(ent1);
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
Minesweeper::isGameOver(){
  return _table->isEnd();
}
bool 
Minesweeper::isWin(){
  std::cout << "NUMBERS : "<<_table->getDiscovered() << " " << _table->getMines() << " " << _table->getSquares() << std::endl;
  return ((_table->getDiscovered() + _table->getMines()) == (_table->getSquares() * _table->getSquares()*6));
}

void Minesweeper::setFlag(Ogre::SceneNode * node){
  int face = _table->findFace(node);
  int pos = _table->findPos(face,node);
  if (_table->isValid(face,pos)){
    _table->setFlag(face,pos);
  }
  else if(_table->isFlag(face,pos)){
    _table->quitFlag(face,pos);
  }
}
    
void 
Minesweeper::sendMove(Ogre::SceneNode * node){
  int face = _table->findFace(node);
  int pos = _table->findPos(face,node);
  if (_table->isValid(face,pos)){
    std::cout<< "MINESWEEPER movimiento valido"<<std::endl;
    _table->setMov(face,pos,node);
  }
}

int Minesweeper::getFlags(){
  return _table->getFlags();
}

int Minesweeper::getTotalMines(){
  return _table->getMines();
}

int Minesweeper::getDiscovered(){
  return _table->getDiscovered();
}

int Minesweeper::getSquares(){
  return _table->getSquares();
}
