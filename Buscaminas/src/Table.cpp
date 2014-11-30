#include "Table.h"

Table::Table(int squares, float distance, Ogre::SceneManager * sceneMgr, Ogre::SceneNode * node):_rootNode(node){

  Ogre::SceneManager * _sceneMgr = sceneMgr;
  _squares = squares;
  _distance = distance;
  _flags = 0;
  _mines = (squares-1)*6;
  _discovered=0;

  _table = std::vector<std::vector <int> >(6, std::vector<int>(squares*squares,VOID));
  _nodes = std::vector<std::vector <Ogre::SceneNode*> >(6, std::vector<Ogre::SceneNode*>());
  _mask = std::vector<std::vector <char> > (6, std::vector<char> (squares*squares,1));
  _flags_pos = std::vector<std::vector <char> > (6, std::vector<char> (squares*squares,0));

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
      (_nodes[BASE]).push_back(_node);

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
  addMines();
  addNumbers();
}

Table::~Table(){
}

void 
Table::setMov(int face, int pos, Ogre::SceneNode * node){
  if(_mask[face][pos] == 1 && _flags_pos[face][pos] == 0){
    _mask[face][pos] = 0;
    int n = _table[face][pos];
    std::cout << "N :"<<n<<std::endl;
    std::stringstream s;
    s << "Cubem_";
    switch(n){
    case MINE:
      std::cout << "MINA"<<std::endl;
      static_cast<Ogre::Entity*>(_nodes[face][pos]->getAttachedObject(0))->setMaterialName("Cubem_bomb");
      _end = true;
      break;
    case C1:    case C2:   case C3:   case C4:    
    case C5:    case C6:   case C7:   case C8:
      s << n;
      static_cast<Ogre::Entity*>(_nodes[face][pos]->getAttachedObject(0))->setMaterialName(s.str());
      _discovered++;
      break;
    case VOID:
      expand(face,pos);
      break;
    }
  }
}
int
Table::getRemainingMines(){
  return _remaining_mines;
}

bool 
Table::isValid(int face, int pos){
  return _mask[face][pos];
}

int
Table::getDiscovered(){
  return _discovered;
}
int
Table::getFlags(){
  return _flags;
}


int Table::findPos(int face, Ogre::SceneNode * node){
  int pos =0;
  if(node){
    for (std::vector<Ogre::SceneNode *>::iterator it = (_nodes[face]).begin();it!=(_nodes[face]).end();it++){
      if((*it) == node) break;
      else pos++;
    }
    
  }
  return pos;
}

int Table::findFace(Ogre::SceneNode * node){
  int count=0;
  if (!node) return -1;
  for (std::vector <std::vector <Ogre::SceneNode *> >::iterator it = _nodes.begin();it!=_nodes.end();it++){
    if(std::find((*it).begin(),(*it).end(),node) == (*it).end())
      count++;
    else break;
  }
  return count;
}

void Table::addMines(){
  //Mines
  for (int i=0;i<6;i++){
    int mines = _mines/6;
    while(mines){
      int n1=rand()%(_squares*_squares);
      if( _table[i][n1] !=MINE){
	_table[i][n1] = MINE;
	mines--;
      }
    }
  }

}

void Table::addNumbers(){
  for(int face=0;face < 6;face++){
    for(int pos = 0;pos<_squares*_squares;pos++){
      int bombs =0;
      if(_table[face][pos] == VOID){
	bombs = countNear(face,pos);
	if(bombs != 0)
	  _table[face][pos] = MINE+bombs;//regarding defines     
      }
    }
  }
}

int Table::countNear(int face,int pos){
  int bombs=0;
  std::vector < std::pair<int,int> > neighbours = searchNeighbours(face,pos);
  for (std::vector <std::pair<int,int> >::iterator it = neighbours.begin();it!=neighbours.end();it++)
    bombs += _table[(*it).first][(*it).second]-MINE; 
  return bombs;
}

void Table::expand(int face,int pos){
  if(_table[face][pos] == VOID){
    std::vector < std::pair<int,int> > neigbours = searchNeighbours(face,pos);
    _discovered++;
    _mask[face][pos] = 1;
    static_cast<Ogre::Entity *>(_nodes[face][pos]->getAttachedObject(0))->setMaterialName("Cube2");   
    for (std::vector <std::pair<int,int> >::iterator it = neigbours.begin();it!=neigbours.end();it++)
      expand((*it).first,(*it).second);
  }
}

std::vector < std::pair <int, int> > Table::searchNeighbours(int face, int pos){
  std::vector < std::pair < int, int > > ret;
 switch(face){
  case BASE:
    
    break;
  case TAPA:
    break;
  case IZQ:
    break;
  case DER:
    break;
  case TRAS:
   break;
  case FRONT:
    break;
  }
 return ret;
}

bool Table::isFlag(int face, int pos){
  return (_table[face][pos]==FLAG && _mask[face][pos]==0 && _flags_pos[face][pos] != 0);
}

void Table::setFlag(int face,int pos){
  std::cout << "CASILLA "<<_table[face][pos]<< std::endl;
  _flags_pos [face][pos] = _table[face][pos];
  _table[face][pos]=FLAG;
  _mask[face][pos]=0;
  static_cast<Ogre::Entity *>(_nodes[face][pos]->getAttachedObject(0))->setMaterialName("Flag");
  _flags++;
}

void Table::quitFlag(int face,int pos){
  _table[face][pos]=_flags_pos[face][pos];
  _flags_pos[face][pos]=0;
  _mask[face][pos]=1;
  _flags--;
  static_cast<Ogre::Entity *>(_nodes[face][pos]->getAttachedObject(0))->setMaterialName("Cube1");
}

int Table::getMines(){
  return _mines;
}
