#include "Table.h"

Table::Table(int squares, float distance, Ogre::SceneManager * sceneMgr, Ogre::SceneNode * node):_rootNode(node){

  Ogre::SceneManager * _sceneMgr = sceneMgr;
  _squares = squares;
  _distance = distance;
  _flags = 0;
  _mines = (squares-1)*6;
  _discovered=0;
  _end=false;

  _table = std::vector<std::vector <int> >(6, std::vector<int>(squares*squares,VOID));
  _nodes = std::vector<std::vector <Ogre::SceneNode*> >(6, std::vector<Ogre::SceneNode*>());
  _mask = std::vector<std::vector <int> > (6, std::vector<int> (squares*squares,1));
  _flags_pos = std::vector<std::vector <int> > (6, std::vector<int> (squares*squares,0));

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
    int n = _table[face][pos];
    std::stringstream s;
    s << "Cubem_";
    switch(n){
    case MINE:
      static_cast<Ogre::Entity*>(_nodes[face][pos]->getAttachedObject(0))->setMaterialName("Cubem_bomb");
      _end = true;
      break;
    case C1:    case C2:   case C3:   case C4:    
    case C5:    case C6:   case C7:   case C8:
      s << n-MINE;
      static_cast<Ogre::Entity*>(_nodes[face][pos]->getAttachedObject(0))->setMaterialName(s.str());
      _discovered++;
      break;
    case VOID:
      expand(face,pos);
      break;
    }
    _mask[face][pos] = 0;
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
  for (std::vector <std::pair<int,int> >::iterator it = neighbours.begin();it!=neighbours.end();it++){
    if(_table[(*it).first][(*it).second] == MINE)
      bombs++;
  }
  return bombs;
}

void Table::expand(int face,int pos){
  if(_mask[face][pos]==1){
    std::vector < std::pair<int,int> > neigbours = searchNeighbours(face,pos);
    _discovered++;
    _mask[face][pos] = 0;
    static_cast<Ogre::Entity *>(_nodes[face][pos]->getAttachedObject(0))->setMaterialName("Cube2");   
    if(_table[face][pos] ==VOID){
      static_cast<Ogre::Entity *>(_nodes[face][pos]->getAttachedObject(0))->setMaterialName("Cube2");   
      for (std::vector <std::pair<int,int> >::iterator it = neigbours.begin();it!=neigbours.end();it++){
	if(_table[(*it).first][(*it).second] != MINE && _flags_pos[(*it).first][(*it).second] == 0 && _mask[(*it).first][(*it).second]==1 )
	  expand((*it).first,(*it).second);
      }
    }
    else{
      std::stringstream s;
      s << "Cubem_" <<  _table[face][pos]-MINE;
      static_cast<Ogre::Entity*>(_nodes[face][pos]->getAttachedObject(0))->setMaterialName(s.str());
    }
  }
}

std::vector < std::pair <int, int> > Table::searchNeighbours(int face, int pos){
  std::vector < std::pair < int, int > > ret;

  if(isVert(pos)){
    switch(face){
    case BASE:
      if(pos == 0){
 	ret.push_back(std::pair<int,int>(face,1));
 	ret.push_back(std::pair<int,int>(face,_squares));
 	ret.push_back(std::pair<int,int>(face,_squares+1));
 	ret.push_back(std::pair<int,int>(IZQ,0));
 	ret.push_back(std::pair<int,int>(IZQ,1));////////NO TOCAR
 	ret.push_back(std::pair<int,int>(TRAS,0));
 	ret.push_back(std::pair<int,int>(TRAS,_squares));
      }
      else if (pos == _squares-1){
 	ret.push_back(std::pair<int,int>(face,pos+_squares));
 	ret.push_back(std::pair<int,int>(face,pos-1));
 	ret.push_back(std::pair<int,int>(face,pos+_squares-1));
 	ret.push_back(std::pair<int,int>(IZQ,_squares-1));//NO TOCAR
 	ret.push_back(std::pair<int,int>(IZQ,_squares-2));
 	ret.push_back(std::pair<int,int>(FRONT,0));
 	ret.push_back(std::pair<int,int>(FRONT,_squares));
      }
      else if(pos == (_squares*_squares) -1){
 	ret.push_back(std::pair<int,int>(face,pos-1));
 	ret.push_back(std::pair<int,int>(face,pos-_squares-1));
 	ret.push_back(std::pair<int,int>(face,pos-_squares));
 	ret.push_back(std::pair<int,int>(DER,_squares-1));
 	ret.push_back(std::pair<int,int>(DER,_squares-2));//NO TOCAR
 	ret.push_back(std::pair<int,int>(FRONT,(_squares*_squares)-_squares));
 	ret.push_back(std::pair<int,int>(FRONT,(_squares*_squares)-(2*_squares)));
      }
      else{
 	ret.push_back(std::pair<int,int>(face,pos+1));
 	ret.push_back(std::pair<int,int>(face,pos-_squares));
 	ret.push_back(std::pair<int,int>(face,pos-_squares+1));
 	ret.push_back(std::pair<int,int>(DER,0));//NO TOCAR
 	ret.push_back(std::pair<int,int>(DER,1));
 	ret.push_back(std::pair<int,int>(TRAS,pos-_squares));
	ret.push_back(std::pair<int,int>(TRAS,pos));
      }///bien
      break;
    case TAPA:
      
      if(pos == 0){
	ret.push_back(std::pair<int,int>(face,1));
	ret.push_back(std::pair<int,int>(face,_squares));
	ret.push_back(std::pair<int,int>(face,_squares+1));
	ret.push_back(std::pair<int,int>(IZQ,(_squares*_squares) - _squares));
	ret.push_back(std::pair<int,int>(IZQ,((_squares*_squares) - _squares)+1));
	ret.push_back(std::pair<int,int>(TRAS,_squares-1));//NO TOCAR
	ret.push_back(std::pair<int,int>(TRAS,(2*_squares)-1));
      }
      else if (pos == _squares-1){
	ret.push_back(std::pair<int,int>(face,pos+_squares));
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos+_squares-1));
	ret.push_back(std::pair<int,int>(IZQ,(_squares*_squares)-1));
	ret.push_back(std::pair<int,int>(IZQ,(_squares*_squares)-2));
	ret.push_back(std::pair<int,int>(FRONT,pos));//NO TOCAR
	ret.push_back(std::pair<int,int>(FRONT,pos+_squares));
      }
      else if(pos == (_squares*_squares) -1){
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos-_squares));
	ret.push_back(std::pair<int,int>(face,pos-_squares-1));
	ret.push_back(std::pair<int,int>(DER,pos));//NO TOCAR
	ret.push_back(std::pair<int,int>(DER,pos-1));
	ret.push_back(std::pair<int,int>(FRONT,pos));
	ret.push_back(std::pair<int,int>(FRONT,pos-_squares));
      }
      else{
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,pos-_squares));
	ret.push_back(std::pair<int,int>(face,pos-_squares+1));
	ret.push_back(std::pair<int,int>(DER,pos));//NO TOCAR
	ret.push_back(std::pair<int,int>(DER,pos+1));
	ret.push_back(std::pair<int,int>(TRAS,(_squares*_squares)-1));
	ret.push_back(std::pair<int,int>(TRAS,pos-1));
      }
      break;
    case IZQ:
      if(pos == 0){
 	ret.push_back(std::pair<int,int>(face,1));
 	ret.push_back(std::pair<int,int>(face,_squares));
 	ret.push_back(std::pair<int,int>(face,_squares+1));
 	ret.push_back(std::pair<int,int>(BASE,0));
 	ret.push_back(std::pair<int,int>(BASE,1));//NO TOCAR
 	ret.push_back(std::pair<int,int>(TRAS,0));
 	ret.push_back(std::pair<int,int>(TRAS,1));
      }
      else if (pos == _squares-1){
 	ret.push_back(std::pair<int,int>(face,pos+_squares));
 	ret.push_back(std::pair<int,int>(face,pos-1));
 	ret.push_back(std::pair<int,int>(face,pos+_squares-1));
 	ret.push_back(std::pair<int,int>(BASE,pos));
 	ret.push_back(std::pair<int,int>(BASE,pos-1));
 	ret.push_back(std::pair<int,int>(FRONT,0));//NO TOCAR
 	ret.push_back(std::pair<int,int>(FRONT,1));
      }
      else if(pos == (_squares*_squares) -1){
 	ret.push_back(std::pair<int,int>(face,pos-1));
 	ret.push_back(std::pair<int,int>(face,pos-_squares));
 	ret.push_back(std::pair<int,int>(face,pos-_squares-1));
 	ret.push_back(std::pair<int,int>(FRONT,_squares-1));
 	ret.push_back(std::pair<int,int>(FRONT,_squares-2));
 	ret.push_back(std::pair<int,int>(TAPA,_squares-1));//NO TOCAR
 	ret.push_back(std::pair<int,int>(TAPA,_squares-2));
      }
      else{
 	ret.push_back(std::pair<int,int>(face,pos+1));
 	ret.push_back(std::pair<int,int>(face,pos-_squares));
 	ret.push_back(std::pair<int,int>(face,pos-_squares+1));
 	ret.push_back(std::pair<int,int>(TAPA,0));
 	ret.push_back(std::pair<int,int>(TAPA,1));//NO TOCAR
 	ret.push_back(std::pair<int,int>(TRAS,_squares-1));
 	ret.push_back(std::pair<int,int>(TRAS,_squares-2));
      }
      break;
    case DER:
      if(pos == 0){
 	ret.push_back(std::pair<int,int>(face,1));
 	ret.push_back(std::pair<int,int>(face,_squares));
 	ret.push_back(std::pair<int,int>(face,_squares+1));
 	ret.push_back(std::pair<int,int>(BASE,(_squares*_squares)-_squares));
 	ret.push_back(std::pair<int,int>(BASE,((_squares*_squares)-_squares)+1));
 	ret.push_back(std::pair<int,int>(TRAS,(_squares*_squares)-_squares));//NO TOCAR
	ret.push_back(std::pair<int,int>(TRAS,((_squares*_squares)-_squares)+1));
      }
      else if (pos == _squares-1){
	ret.push_back(std::pair<int,int>(face,pos+_squares));
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos+_squares-1));
	ret.push_back(std::pair<int,int>(BASE,_squares*_squares-1));//NO TOCAR
 	ret.push_back(std::pair<int,int>(BASE,_squares*_squares-2));
 	ret.push_back(std::pair<int,int>(FRONT,_squares*_squares-_squares));
 	ret.push_back(std::pair<int,int>(FRONT,((_squares*_squares)-_squares)+1));
      }
      else if(pos == (_squares*_squares) -1){
 	ret.push_back(std::pair<int,int>(face,pos-1));
 	ret.push_back(std::pair<int,int>(face,pos-_squares));
 	ret.push_back(std::pair<int,int>(face,pos-_squares-1));
 	ret.push_back(std::pair<int,int>(TAPA,pos));
 	ret.push_back(std::pair<int,int>(TAPA,pos-1));
 	ret.push_back(std::pair<int,int>(FRONT,pos));//NO TOCAR
 	ret.push_back(std::pair<int,int>(FRONT,pos-1));
      }
      else{
 	ret.push_back(std::pair<int,int>(face,pos+1));
 	ret.push_back(std::pair<int,int>(face,pos-_squares));
 	ret.push_back(std::pair<int,int>(face,pos-_squares+1));
 	ret.push_back(std::pair<int,int>(TAPA,pos));
 	ret.push_back(std::pair<int,int>(TAPA,pos+1));
 	ret.push_back(std::pair<int,int>(TRAS,pos+_squares-1));//NO TOCAR
 	ret.push_back(std::pair<int,int>(TRAS,pos+_squares-2));
      }
      break;
    case TRAS:
      if(pos == 0){
 	ret.push_back(std::pair<int,int>(face,1));
 	ret.push_back(std::pair<int,int>(face,_squares));
 	ret.push_back(std::pair<int,int>(face,_squares+1));
 	ret.push_back(std::pair<int,int>(IZQ,0));
 	ret.push_back(std::pair<int,int>(IZQ,_squares));
 	ret.push_back(std::pair<int,int>(BASE,0));//NO TOCAR
 	ret.push_back(std::pair<int,int>(BASE,_squares));
      }
      else if (pos == _squares-1){
 	ret.push_back(std::pair<int,int>(face,pos+_squares));
 	ret.push_back(std::pair<int,int>(face,pos-1));
 	ret.push_back(std::pair<int,int>(face,pos+_squares-1));
 	ret.push_back(std::pair<int,int>(IZQ,_squares*_squares-_squares));
 	ret.push_back(std::pair<int,int>(IZQ,(_squares*_squares)-_squares*2));
 	ret.push_back(std::pair<int,int>(TAPA,0));//NO TOCAR
 	ret.push_back(std::pair<int,int>(TAPA,_squares));
      }
      
      else if(pos == (_squares*_squares) -1){
 	ret.push_back(std::pair<int,int>(face,pos-1));
 	ret.push_back(std::pair<int,int>(face,pos-_squares-1));
 	ret.push_back(std::pair<int,int>(face,pos-_squares));
 	ret.push_back(std::pair<int,int>(TAPA,_squares*_squares-_squares));
 	ret.push_back(std::pair<int,int>(TAPA,(_squares*_squares)-(_squares*2)));
 	ret.push_back(std::pair<int,int>(DER,_squares*_squares-_squares));//NO TOCAR
 	ret.push_back(std::pair<int,int>(DER,(_squares*_squares)-(_squares*2)));
      }
      else{
 	ret.push_back(std::pair<int,int>(face,pos+1));
 	ret.push_back(std::pair<int,int>(face,pos-_squares));
 	ret.push_back(std::pair<int,int>(face,pos-_squares+1));
 	ret.push_back(std::pair<int,int>(DER,0));
 	ret.push_back(std::pair<int,int>(DER,_squares));
 	ret.push_back(std::pair<int,int>(BASE,pos));//NO TOCAR
 	ret.push_back(std::pair<int,int>(BASE,pos-_squares));
      }
      break;
    case FRONT:
      if(pos == 0){
 	ret.push_back(std::pair<int,int>(face,1));
 	ret.push_back(std::pair<int,int>(face,_squares));
 	ret.push_back(std::pair<int,int>(face,_squares+1));
 	ret.push_back(std::pair<int,int>(BASE,_squares-1));
 	ret.push_back(std::pair<int,int>(BASE,(_squares*2)-1));
 	ret.push_back(std::pair<int,int>(IZQ,_squares-1));//NO TOCAR
 	ret.push_back(std::pair<int,int>(IZQ,(_squares*2)-1));
      }
      else if (pos == _squares-1){
 	ret.push_back(std::pair<int,int>(face,pos+_squares));
 	ret.push_back(std::pair<int,int>(face,pos-1));
 	ret.push_back(std::pair<int,int>(face,(pos+_squares)-1));
 	ret.push_back(std::pair<int,int>(TAPA,_squares*2-1));
 	ret.push_back(std::pair<int,int>(TAPA,pos));
 	ret.push_back(std::pair<int,int>(IZQ,(_squares*_squares)-1));//NO TOCAR
 	ret.push_back(std::pair<int,int>(IZQ,((_squares*_squares)-_squares)-1));
      }
      else if(pos == (_squares*_squares) -1){
 	ret.push_back(std::pair<int,int>(face,pos-1));
 	ret.push_back(std::pair<int,int>(face,pos-_squares));
 	ret.push_back(std::pair<int,int>(face,pos-_squares-1));
 	ret.push_back(std::pair<int,int>(TAPA,pos));
 	ret.push_back(std::pair<int,int>(TAPA,pos-_squares));
 	ret.push_back(std::pair<int,int>(DER,pos));//NO TOCAR
 	ret.push_back(std::pair<int,int>(DER,pos-_squares));
      }
      else{
 	ret.push_back(std::pair<int,int>(face,pos+1));
 	ret.push_back(std::pair<int,int>(face,pos-_squares));
 	ret.push_back(std::pair<int,int>(face,pos-_squares+1));
 	ret.push_back(std::pair<int,int>(BASE,pos-1));
 	ret.push_back(std::pair<int,int>(BASE,pos+_squares-1));
 	ret.push_back(std::pair<int,int>(DER,_squares*2-1));//NO TOCAR
 	ret.push_back(std::pair<int,int>(DER,_squares-1));
      }
 break;
    }
  }
  else if(isFrame(pos)){
    std::cout << "ES FRAME" << std::endl;
    
    switch(face){
    case BASE:
      if(pos < _squares && pos >= 0){
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,pos+_squares+1));
	ret.push_back(std::pair<int,int>(face,pos+_squares-1));
	ret.push_back(std::pair<int,int>(face,pos+_squares));
	
 	ret.push_back(std::pair<int,int>(IZQ,pos));//NO TOCAR
 	ret.push_back(std::pair<int,int>(IZQ,pos+1));
 	ret.push_back(std::pair<int,int>(IZQ,pos-1));
      }
      else if(pos >= (_squares*_squares)-_squares && pos < (_squares*_squares)){
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,pos-_squares+1));
	ret.push_back(std::pair<int,int>(face,pos-_squares-1));
	ret.push_back(std::pair<int,int>(face,pos-_squares));

	ret.push_back(std::pair<int,int>(DER, pos-(_squares*_squares-_squares)));//NO TOCAR
	ret.push_back(std::pair<int,int>(DER,pos-(_squares*_squares-_squares)-1));
 	ret.push_back(std::pair<int,int>(DER,pos-(_squares*_squares-_squares)+1));
      }
      else if (pos % _squares == 0 ){
	ret.push_back(std::pair<int,int>(face,pos-_squares));
	ret.push_back(std::pair<int,int>(face,pos+_squares));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,pos-_squares+1));
	ret.push_back(std::pair<int,int>(face,pos+_squares+1));

	ret.push_back(std::pair<int,int>(TRAS,pos));//NO TOCAR
 	ret.push_back(std::pair<int,int>(TRAS,pos+_squares));
 	ret.push_back(std::pair<int,int>(TRAS,pos-_squares));
      }
      else{
	ret.push_back(std::pair<int,int>(face,pos-_squares));
	ret.push_back(std::pair<int,int>(face,pos+_squares));
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos-_squares-1));
	ret.push_back(std::pair<int,int>(face,pos+_squares-1));

	ret.push_back(std::pair<int,int>(FRONT,(pos-_squares)+1));//NO TOCAR
 	ret.push_back(std::pair<int,int>(FRONT,(pos-_squares)+1+_squares));
 	ret.push_back(std::pair<int,int>(FRONT,(pos-_squares)+1-_squares));/////////////////////NI MIRAR VAMOS
      }
      break;
    case TAPA:
      if(pos < _squares && pos >= 0){
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,pos+_squares+1));
	ret.push_back(std::pair<int,int>(face,pos+_squares-1));
	ret.push_back(std::pair<int,int>(face,pos+_squares));
	
	//+20
	ret.push_back(std::pair<int,int>(IZQ, pos+(_squares*_squares-_squares)));//NO TOCAR
 	ret.push_back(std::pair<int,int>(IZQ,pos+(_squares*_squares-_squares)-1));
 	ret.push_back(std::pair<int,int>(IZQ,pos+(_squares*_squares-_squares)+1));
 }
      else if(pos >= (_squares*_squares)-_squares && pos < (_squares*_squares)){
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,pos-_squares+1));
	ret.push_back(std::pair<int,int>(face,pos-_squares-1));
	ret.push_back(std::pair<int,int>(face,pos-_squares));

	ret.push_back(std::pair<int,int>(DER,pos));//NO TOCAR
 	ret.push_back(std::pair<int,int>(DER,pos+1));
 	ret.push_back(std::pair<int,int>(DER,pos-1));

      }
      else if (pos % _squares == 0 ){
	ret.push_back(std::pair<int,int>(face,pos-_squares));
	ret.push_back(std::pair<int,int>(face,pos+_squares));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,(pos-_squares)+1));
	ret.push_back(std::pair<int,int>(face,(pos+_squares)+1));

	ret.push_back(std::pair<int,int>(TRAS,(pos+_squares)-1));//NO TOCAR
 	ret.push_back(std::pair<int,int>(TRAS,((pos+_squares)-1)+_squares));
 	ret.push_back(std::pair<int,int>(TRAS,((pos+_squares)-1)-_squares));

      }
      else{
	ret.push_back(std::pair<int,int>(face,pos-_squares));
	ret.push_back(std::pair<int,int>(face,pos+_squares));
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos-_squares-1));
	ret.push_back(std::pair<int,int>(face,pos+_squares-1));

	ret.push_back(std::pair<int,int>(FRONT,pos));//NO TOCAR
 	ret.push_back(std::pair<int,int>(FRONT,pos+_squares));
 	ret.push_back(std::pair<int,int>(FRONT,pos-_squares));

      }///////////////////////BREAKPOINT
      break;
    case IZQ:
      if(pos < _squares && pos >= 0){
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,pos+_squares+1));
	ret.push_back(std::pair<int,int>(face,pos+_squares-1));
	ret.push_back(std::pair<int,int>(face,pos+_squares));
	

	ret.push_back(std::pair<int,int>(BASE,pos));//NO TOCAR
 	ret.push_back(std::pair<int,int>(BASE,pos+1));
 	ret.push_back(std::pair<int,int>(BASE,pos-1));


      }
      else if(pos >= (_squares*_squares)-_squares && pos < (_squares*_squares)){
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,pos-_squares+1));
	ret.push_back(std::pair<int,int>(face,pos-_squares-1));
	ret.push_back(std::pair<int,int>(face,pos-_squares));

	ret.push_back(std::pair<int,int>(TAPA, pos-(_squares*_squares-_squares)));//NO TOCAR
 	ret.push_back(std::pair<int,int>(TAPA,pos-(_squares*_squares-_squares)-1));
 	ret.push_back(std::pair<int,int>(TAPA,pos-(_squares*_squares-_squares)+1));

      }
      else if (pos % _squares == 0 ){
	ret.push_back(std::pair<int,int>(face,pos-_squares));
	ret.push_back(std::pair<int,int>(face,pos+_squares));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,pos-_squares+1));
	ret.push_back(std::pair<int,int>(face,pos+_squares+1));


	ret.push_back(std::pair<int,int>(TRAS, int(pos/_squares)));//NO TOCAR
 	ret.push_back(std::pair<int,int>(TRAS,int(pos/_squares)+1));
 	ret.push_back(std::pair<int,int>(TRAS,int(pos/_squares)-1));

      }
      else{
	ret.push_back(std::pair<int,int>(face,pos-_squares));
	ret.push_back(std::pair<int,int>(face,pos+_squares));
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos-_squares-1));
	ret.push_back(std::pair<int,int>(face,pos+_squares-1));

	ret.push_back(std::pair<int,int>(FRONT, int(pos/_squares)));//NO TOCAR
 	ret.push_back(std::pair<int,int>(FRONT,int(pos/_squares)+1));
 	ret.push_back(std::pair<int,int>(FRONT,int(pos/_squares)-1));
	/////////////////////////////////
      }
      break;
    case DER:
      if(pos < _squares && pos >= 0){
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,pos+_squares+1));
	ret.push_back(std::pair<int,int>(face,pos+_squares-1));
	ret.push_back(std::pair<int,int>(face,pos+_squares));


	ret.push_back(std::pair<int,int>(BASE, pos+(_squares*_squares-_squares)));//NO TOCAR
	ret.push_back(std::pair<int,int>(BASE,pos+(_squares*_squares-_squares)-1));
	ret.push_back(std::pair<int,int>(BASE,pos+(_squares*_squares-_squares)+1));


      }
      else if(pos >= (_squares*_squares)-_squares && pos < (_squares*_squares)){
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,pos-_squares+1));
	ret.push_back(std::pair<int,int>(face,pos-_squares-1));
	ret.push_back(std::pair<int,int>(face,pos-_squares));


	ret.push_back(std::pair<int,int>(TAPA, pos));//NO TOCAR
	ret.push_back(std::pair<int,int>(TAPA,pos+1));
	ret.push_back(std::pair<int,int>(TAPA,pos-1));


      }
      else if (pos % _squares == 0 ){
	ret.push_back(std::pair<int,int>(face,pos-_squares));
	ret.push_back(std::pair<int,int>(face,pos+_squares));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,pos-_squares+1));
	ret.push_back(std::pair<int,int>(face,pos+_squares+1));


	ret.push_back(std::pair<int,int>(TRAS, int(pos/_squares)+(_squares*_squares-_squares)));//NO TOCAR
	ret.push_back(std::pair<int,int>(TRAS,int(pos/_squares)+(_squares*_squares-_squares)+1));
	ret.push_back(std::pair<int,int>(TRAS,int(pos/_squares)+(_squares*_squares-_squares)-1));



      }
      else{
	ret.push_back(std::pair<int,int>(face,pos-_squares));
	ret.push_back(std::pair<int,int>(face,pos+_squares));
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos-_squares-1));
	ret.push_back(std::pair<int,int>(face,pos+_squares-1));
	//////////////////////////////////////////////////
	ret.push_back(std::pair<int,int>(FRONT, int(pos/_squares)+(_squares*_squares-_squares)));//NO TOCAR
	ret.push_back(std::pair<int,int>(FRONT,int(pos/_squares)+(_squares*_squares-_squares)+1));
	ret.push_back(std::pair<int,int>(FRONT,int(pos/_squares)+(_squares*_squares-_squares)-1));

      }
      break;
    case TRAS:
      if(pos < _squares && pos >= 0){
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos+_squares+1));
	ret.push_back(std::pair<int,int>(face,pos+_squares-1));
	ret.push_back(std::pair<int,int>(face,pos+_squares));

	ret.push_back(std::pair<int,int>(IZQ, pos*_squares));//NO TOCAR
 	ret.push_back(std::pair<int,int>(IZQ,(pos*_squares)+_squares));
 	ret.push_back(std::pair<int,int>(IZQ,(pos*_squares)-_squares));



      }
      else if(pos >= (_squares*_squares)-_squares && pos < (_squares*_squares)){
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,pos-_squares+1));
	ret.push_back(std::pair<int,int>(face,pos-_squares-1));
	ret.push_back(std::pair<int,int>(face,pos-_squares));

	ret.push_back(std::pair<int,int>(DER, (pos%_squares)*_squares));//NO TOCAR
 	ret.push_back(std::pair<int,int>(DER,((pos%_squares)*_squares)+_squares));
 	ret.push_back(std::pair<int,int>(DER,((pos%_squares)*_squares)-_squares));

      }
      else if (pos % _squares == 0 ){
	ret.push_back(std::pair<int,int>(face,pos-_squares));
	ret.push_back(std::pair<int,int>(face,pos+_squares));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,pos-_squares+1));
	ret.push_back(std::pair<int,int>(face,pos+_squares+1));

	ret.push_back(std::pair<int,int>(BASE, pos));//NO TOCAR
 	ret.push_back(std::pair<int,int>(BASE,pos+_squares));
 	ret.push_back(std::pair<int,int>(BASE,pos-_squares));

      }
      else{
	ret.push_back(std::pair<int,int>(face,pos-_squares));
	ret.push_back(std::pair<int,int>(face,pos+_squares));
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos-_squares-1));
	ret.push_back(std::pair<int,int>(face,pos+_squares-1));

	ret.push_back(std::pair<int,int>(TAPA,(pos-_squares)+1));//NO TOCAR
 	ret.push_back(std::pair<int,int>(TAPA,(pos-_squares)+1+_squares));
 	ret.push_back(std::pair<int,int>(TAPA,(pos-_squares)+1-_squares));

      }
      break;
    case FRONT:
      if(pos < _squares && pos >= 0){
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,(pos+_squares)+1));
	ret.push_back(std::pair<int,int>(face,(pos+_squares)-1));
	ret.push_back(std::pair<int,int>(face,(pos+_squares)));
	
	ret.push_back(std::pair<int,int>(IZQ,(pos*_squares)+(_squares-1)));//NO TOCAR
 	ret.push_back(std::pair<int,int>(IZQ,((pos*_squares)+(_squares-1))+_squares));
 	ret.push_back(std::pair<int,int>(IZQ,((pos*_squares)+(_squares-1))-_squares));

	

      }
      else if(pos >= (_squares*_squares)-_squares && pos < (_squares*_squares)){
	ret.push_back(std::pair<int,int>(face,pos-1));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,(pos-_squares)+1));
	ret.push_back(std::pair<int,int>(face,(pos-_squares)-1));
	ret.push_back(std::pair<int,int>(face,pos-_squares));
	
	int sq  =( _squares*_squares)-_squares;
	ret.push_back(std::pair<int,int>(DER,((pos-sq)* _squares) + (_squares-1)) ) ;//NO TOCAR
	ret.push_back(std::pair<int,int>(DER,(((pos-sq)* _squares) + (_squares-1)) + _squares)) ;//NO TOCAR
	ret.push_back(std::pair<int,int>(DER,(((pos-sq)* _squares) + (_squares-1)) - _squares)) ;//NO TOCAR

	
      }
      else if (pos % _squares == 0 ){
	ret.push_back(std::pair<int,int>(face,pos-_squares));
	ret.push_back(std::pair<int,int>(face,pos+_squares));
	ret.push_back(std::pair<int,int>(face,pos+1));
	ret.push_back(std::pair<int,int>(face,(pos-_squares)+1));
	ret.push_back(std::pair<int,int>(face,(pos+_squares)+1));

	ret.push_back(std::pair<int,int>(BASE,pos+(_squares-1)));//NO TOCAR
 	ret.push_back(std::pair<int,int>(BASE,(pos+(_squares-1))+_squares));
	ret.push_back(std::pair<int,int>(BASE,(pos+(_squares-1))-_squares));
    
  }
  else{
    ret.push_back(std::pair<int,int>(face,pos-_squares));
    ret.push_back(std::pair<int,int>(face,pos+_squares));
    ret.push_back(std::pair<int,int>(face,pos-1));
    ret.push_back(std::pair<int,int>(face,(pos-_squares)-1));
    ret.push_back(std::pair<int,int>(face,(pos+_squares)-1));
    
    ret.push_back(std::pair<int,int>(TAPA,pos));//NO TOCAR
    ret.push_back(std::pair<int,int>(TAPA,pos+_squares));
    ret.push_back(std::pair<int,int>(TAPA,pos-_squares));

      }
      break;
    }
  }
  else{
    std::cout << "ES NORMAL" << std::endl;
    
    ret.push_back(std::pair<int,int>(face,pos-1));
    ret.push_back(std::pair<int,int>(face,pos+1));
    ret.push_back(std::pair<int,int>(face,(pos+_squares)-1));
    ret.push_back(std::pair<int,int>(face,(pos+_squares)+1));
    ret.push_back(std::pair<int,int>(face,pos+_squares));
    ret.push_back(std::pair<int,int>(face,(pos-_squares)-1));
    ret.push_back(std::pair<int,int>(face,(pos-_squares)));
    ret.push_back(std::pair<int,int>(face,(pos-_squares)+1));
  }
  return ret;
}

bool Table::isVert(int pos){
  return (pos == 0 || pos== _squares-1 || pos ==( _squares*_squares)-1 || pos == (_squares * _squares) - _squares);
}

bool Table::isFrame(int pos){
  return ( (pos < _squares && pos >= 0) || 
	   (pos >= (_squares*_squares)-_squares && pos < (_squares*_squares) ) || 
	   (pos % _squares == 0 ) || (pos % _squares == (_squares -1)) );
}	    

bool Table::isFlag(int face, int pos){
  return (_table[face][pos]==FLAG && _mask[face][pos]==0 && _flags_pos[face][pos] != 0);
}

void Table::setFlag(int face,int pos){
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

int Table::getSquares(){
  return _squares;
}

bool Table::isEnd(){
  return _end;
}

void Table::showMines(){
  for (int i = 0;i<6;i++)
    for(int j= 0;j<_squares*_squares;j++)
      if(_table[i][j] == MINE || _flags_pos[i][j] == MINE)
	static_cast<Ogre::Entity*>(_nodes[i][j]->getAttachedObject(0))->setMaterialName("Cubem_bomb");
}
