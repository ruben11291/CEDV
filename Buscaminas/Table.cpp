#include "Table.h"

Minesweeper::Minesweeper(int squares):table(new Table(squares)){}

Minesweeper::~Minesweeper(){
  vector<int>::iterator i = _table.begin();
  for (;i!=_table.end();i++)
    delete i;
  delete _table;
}

void 
Minesweeper::setMov(int face,int row,int column,int mov){
  if (isValid(face,row,column,mov))
    _table.setMov(face,row,column);
}

bool 
Minesweeper::isValid(int face,int row,int column,int mov){
  // return _table.getTable()[face*]
  return true;
}

Table * 
Minesweeper::getTable(){
  return _table;
}

int 
Minesweeper::getRemainingMines(){
  return _table.getRemainingMines
