
#ifndef Minesweeper_H
#define Minesweeper_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include "Table.h"
#include <vector>

class Minesweeper{

public:
	Minesweeper(int squares, Ogre::SceneManager * _sceneMgr, Ogre::Viewport * _viewport);
	~Minesweeper();
	void setMov(int face,int row, int column);
	Table * getTable();
	int getRemainingMines();
	bool isValid(int face,int row,int column);
	bool isGameOver();
	bool isWin();
	void assignCube(vector<Ogre::SceneNode *> nodes, Ogre::SceneNode *central);
	void deleteCube();
	
	
private:
	Table * table;
	Ogre::RaySceneQuery *_raySceneQuery;
	Ogre::SceneManager * _sceneMgr;
	Ogre::Viewport *_viewport; 
};
#endif
