
#ifndef Minesweeper_H
#define Minesweeper_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include "Table.h"
#include <vector>

class Minesweeper {

public:
  Minesweeper(int squares, Ogre::SceneManager * _sceneMgr);
	~Minesweeper();
	void setMov(int face,int row, int column);
	//	Table * getTable();
	bool isValid(int face,int row,int column);
	bool isGameOver();
	bool isWin();
	void assignCube(std::vector<Ogre::SceneNode *> *nodes);
	void deleteCube();
	void show();
	void hide();
	void pitch(Ogre::Degree degree);
	void yaw(Ogre::Degree degree);
	
private:
	Table * _table;
	Ogre::SceneManager * _sceneMgr;
	Ogre::SceneNode * _node;
};
#endif
