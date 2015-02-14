#ifndef _SECTIONH_
#define _SECTIONH_ 

#include <Ogre.h>
#include <vector>
#include <Cube.h>
#include <stdio.h>      
#include <stdlib.h>   
#include <time.h>

class Section{
	public:
	Section();
	~Section();
	Section(OgreBulletDynamics::DynamicsWorld *, int difficult, float x0, float xf, float y0, float yf);
	std::vector <Cube*>& getCubes();
	


	private:
	std::vector <Cube*> _cubes;
	void createTable(OgreBulletDynamics::DynamicsWorld *, int ncubes, int cubes_line, int fixed, int two_impact, Ogre::Vector3 scale, float x0, float y0);

};

#endif
