/*********************************************************************
 * Módulo 1. Curso de Experto en Desarrollo de Videojuegos
 * Autor: Ruben Perez Pascual ruben.perez@alu.uclm.es
 * Autor: Angel Peralta Lopez angel.peralta@alu.uclm.es
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  
 *********************************************************************/
#ifndef Table_H
#define Table_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <vector>
#include "Types.h"
#include <sstream>
#include <utility>

class Table{

public:
  Table(int squares, float distance , Ogre::SceneManager * _mgr , Ogre::SceneNode * node);
  ~Table();
  void setMov(int face,int pos,Ogre::SceneNode *);
  int getDiscovered();
  int getRemainingMines();
  int getFlags();
  bool isValid(int face,int pos);
  int findFace(Ogre::SceneNode*);
  int findPos(int, Ogre::SceneNode *);
  void setFlag(int,int);
  bool isFlag(int, int);
  void quitFlag(int,int);
  int getMines();
  bool isEnd();
  int getSquares();
  void showMines();

 private:
  
  void addMines();
  void addNumbers();
  int countNear(int,int);
  void expand(int,int);
  std::vector < std::pair <int, int> > searchNeighbours(int,int);
  bool isVert(int);
  bool isFrame(int);
  
  Ogre::SceneNode * _rootNode;
  std::vector<std::vector<int> >  _table;
  std::vector<std::vector<int> >  _flags_pos;
  std::vector<std::vector<int> >  _mask;
  std::vector<std::vector<Ogre::SceneNode*> >  _nodes;
  int _flags; //contains the number of flags performed
  int _mines; //total mines
  int _remaining_mines;
  int _discovered;
  float _distance;
  int _squares;
  bool _end;
};
#endif
