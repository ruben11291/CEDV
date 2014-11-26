
#ifndef Minesweeper_H
#define Minesweeper_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include "Table.h"

class Minesweeper{

public:
	Minesweeper(int squares);
	~Minesweeper();
	void setMov(int face,int row, int column);
	Table * getTable();
	int getRemainingMines();
	bool isValid(int face,int row,int column);
	
private:
	Table * table;

	
};
#endif
