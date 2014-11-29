
#ifndef Table_H
#define Table_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <vector>
#include "Types.h"

class Table{

public:
  Table(int squares, float distance , Ogre::SceneManager * _mgr , Ogre::SceneNode * node);
  ~Table();
  void setMov(int face,int row, int column);
  int getRemainingFree();
  int getDiscovered();
  int getFlags();
  bool isValid(int face,int row,int column);
  bool isWin();
  bool isLose();

 private:
  
  Ogre::SceneNode * _rootNode;
  std::vector<std::vector<int> >  _table;
  std::vector<std::vector<Ogre::SceneNode*> >  _nodes;
  int _flags; //contains the number of flags performed
  int _mines; //total mines
  int _remaining_free;
  float _distance;
  int _squares;
};
#endif
