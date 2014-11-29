#include "Table.h"

Table::Table(int squares, float distance, Ogre::SceneManager * sceneMgr, Ogre::SceneNode * node):_rootNode(node){

  Ogre::SceneManager * _sceneMgr = sceneMgr;
  _squares = squares;
  _distance = distance;
  _flags = 0;
  _mines = squares-1;
  _remaining_free = 0; //calcular despues de poblar 

  _table = std::vector<std::vector <int> >(6, std::vector<int>(squares*squares));
  _nodes = std::vector<std::vector <Ogre::SceneNode*> >(6, std::vector<Ogre::SceneNode*>(squares*squares));
  //for (std::vector<std::vector<int>>::iterator it=_table->begin();it!=_table->end();it++)
    //    (it) = new std::vector<int>();

  Ogre::SceneNode * _node;
  Ogre::Real distance_to_center = _distance*_squares/2;
  //  creation of the cube 
  for (int i=0;i < squares;i++){
    for (int j=0;j < squares;j++){
      //base
      Ogre::Entity* ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->translate(Ogre::Vector3(i*_distance,0.1,float(j*_distance))-distance_to_center);
      _node->scale(1,0.1,1);
      _rootNode->addChild(_node);
      std::cout << "antes insercion " << std::endl;
      (_nodes[BASE]).push_back(_node);
      std::cout << "despues insercion " << std::endl;

      //tapa
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->translate(Ogre::Vector3(float(i*_distance),float(_squares*_distance+0.05),float(j*_distance))-distance_to_center);
      _node->scale(1,0,1);
      _rootNode->addChild(_node);
      (_nodes[TAPA]).push_back(_node);

      //lateral izq
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->scale(1,0,1);
      _node->translate(Ogre::Vector3(-_distance+0.25,float(i*_distance +0.3),float(j*_distance))-distance_to_center);
      _node->roll(Ogre::Degree(90),Ogre::Node::TS_PARENT);
      _rootNode->addChild(_node);
      (_nodes[IZQ]).push_back(_node);

      //lateral derecho
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->translate(Ogre::Vector3(_squares*_distance+_distance-0.75, float(i*_distance +0.3),float(j*_distance))-distance_to_center);
      _node->scale(1,0,1);
      _node->roll(Ogre::Degree(90),Ogre::Node::TS_PARENT);
      _rootNode->addChild(_node);
      (_nodes[DER]).push_back(_node);

      //tapa trasera
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->translate(Ogre::Vector3(float(i*_distance),float(j*_distance+0.3),-_distance+0.25 )-distance_to_center);
      _node->scale(1,0,1);
      _node->pitch(Ogre::Degree(-90),Ogre::Node::TS_PARENT);
      _rootNode->addChild(_node);
      (_nodes[TRAS]).push_back(_node);

      //frontal
      ent1 = _sceneMgr->createEntity("Cube1.mesh");
      ent1->setQueryFlags(CUBE);
      _node = _sceneMgr->createSceneNode();
      _node->attachObject(ent1);
      _node->translate(Ogre::Vector3(float(i*_distance),float(j*_distance+0.3),_squares*_distance+_distance-0.75)-distance_to_center);
      _node->scale(1,0,1);
      _node->pitch(Ogre::Degree(-90),Ogre::Node::TS_PARENT);
      _rootNode->addChild(_node);
      (_nodes[FRONT]).push_back(_node);
    }

  }
  
}

Table::~Table(){
}

void 
Table::setMov(int face, int row, int column){}

int
Table::getRemainingFree(){
  return _remaining_free;
}

bool 
Table::isValid(int face, int row, int column){
  return true;
}

int
Table::getDiscovered(){
  return _squares - _remaining_free;
}
int
Table::getFlags(){
  return _flags;
}

bool 
Table::isWin(){
  //TODO
 return false;
}
bool Table::isLose(){
  //TODO
  return false;
}
