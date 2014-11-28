
#ifndef Table_H
#define Table_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <vector>
#include "Types.h"

class Table{

public:
	Table(int squares);
	~Table();
	void setMov(int face,int row, int column);
	int getRemainingMines();
	int getRemainingFree();
	int getDiscovered();
	int getFlags();
	bool isValid(int face,int row,int column);
	bool isWin();
	bool isLose();
	
private:
	vector<Types> * table;
	SceneNode * _central;
	vector<SceneNode*> _nodes;
	bool isValid(int face,int row,int column);
	int flags; //contains the number of flags performed
	int mines; //total mines
	int remaining_mines;
	int remaining_free;
	int squares;
};
#endif
