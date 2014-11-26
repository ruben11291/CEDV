
#ifndef Table_H
#define Table_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <vector>


class Table{

public:
	Table(int squares);
	~Table();
	void setMov(int face,int row, int column);
	vector<int> * getSquares();
	int getRemainingMines();
	bool isValid(int face,int row,int column);
	
private:
	vector<int> * table;
	bool isValid(int face,int row,int column);
	
};
#endif
