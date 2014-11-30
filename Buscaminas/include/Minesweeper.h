
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
	bool isGameOver();
	bool isWin();
//	void assignCube(std::vector<Ogre::SceneNode *> *nodes);
	void deleteCube();
	void show();
	void hide();
	void pitch(Ogre::Degree degree);
	void yaw(Ogre::Degree degree);
	void sendMove(Ogre::SceneNode *);
	void setFlag(Ogre::SceneNode *);
	int getFlags();
	int getTotalMines();

private:
	Table * _table;
	Ogre::SceneManager * _sceneMgr;
	Ogre::SceneNode * _node;
};
#endif
