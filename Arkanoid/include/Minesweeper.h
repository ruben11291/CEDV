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
	void show();
	void hide();
	void pitch(Ogre::Degree degree);
	void yaw(Ogre::Degree degree);
	void sendMove(Ogre::SceneNode *);
	void setFlag(Ogre::SceneNode *);
	int getFlags();
	int getTotalMines();
	int getSquares();
	int getDiscovered();
	void showMines();

private:
	Table * _table;
	Ogre::SceneManager * _sceneMgr;
	Ogre::SceneNode * _node;
};
#endif
