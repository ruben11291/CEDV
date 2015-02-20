#ifndef _SECTIONH_
#define _SECTIONH_ 

#include <Ogre.h>
#include <deque>
#include <list>
#include <Cube.h>
#include <stdio.h>      
#include <stdlib.h>   
#include <time.h>

class Section{
	public:
	Section();
	~Section();
	Section(OgreBulletDynamics::DynamicsWorld *, int difficult, Ogre::Vector3&, Ogre::Vector3&, Ogre::Vector3&, Ogre::Vector3&);
	std::deque <Cube*>& getCubes();
	bool isWin();


	private:
	std::deque <Cube*> _cubes;
	void createTable(OgreBulletDynamics::DynamicsWorld *, int ncubesR, int ncubesC, int fixed, int two_impact, Ogre::Vector3&, Ogre::Vector3&);
	int _two_impact, _fixed, _ncubes, _ncubesRow, _ncubesCol;

};

#endif
