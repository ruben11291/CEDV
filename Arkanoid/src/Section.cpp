
#include "Section.h"

Section::Section(){
}


Section::Section(OgreBulletDynamics::DynamicsWorld* world,int difficult,  Ogre::Vector3& frb, Ogre::Vector3& nrb, Ogre::Vector3& flb, Ogre::Vector3& nlb){

  Ogre::Real dist_x = frb.distance(flb);
  int cubes_line = 7;
  float len_x_cube = 0.55;//dist_x/(float)cubes_line;
  Ogre::Vector3 scale(len_x_cube, 0.2, 0.2), initialPoint(frb.x + len_x_cube + 0.5,0, frb.z+ 0.2*2 );
  int ncubes, fixed, two_impact;
  ncubes = cubes_line* cubes_line;
  switch(difficult){
  case 0://easy level
    fixed = 0;
    two_impact = ncubes/5;
    break;
  case 1:
    fixed = two_impact = ncubes / 5;
    break;
  case 2:
    fixed = ncubes/5;
    two_impact = 2*ncubes/5;
    break;
  }
  createTable(world,ncubes, cubes_line, fixed,two_impact, scale, initialPoint);
}

std::deque <Cube*>& Section::getCubes(){
  return _cubes;
}


void Section::createTable(OgreBulletDynamics::DynamicsWorld* world,int ncubes, int cubes_line, int fixed, int two_impact, Ogre::Vector3& scale, Ogre::Vector3& initial_point){

  Ogre::Entity* cube;
   Ogre::SceneNode* cubo_n;
   Ogre::SceneManager * _sceneMgr= Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager");
   AxisAlignedBox boundingB;
   Ogre::Vector3 size;
   OgreBulletCollisions::BoxCollisionShape *cubeShape ;
   OgreBulletDynamics::RigidBody * rigidbody;
   srand(time(NULL));
   
   for(int i=0,n=0;i<sqrt(ncubes);i++){
     for(int j=0;j<sqrt(ncubes);j++){
       cube = _sceneMgr->createEntity("cube.mesh");
       cubo_n = _sceneMgr->createSceneNode("cube"+StringConverter::toString(n));
       //cube->setMaterialName("Cube1");
      
      cube->setCastShadows(true);
      cubo_n->scale(scale);
      cubo_n->attachObject(cube);
      _sceneMgr->getRootSceneNode()->addChild(cubo_n);
      
      boundingB = cube->getBoundingBox();
      size = boundingB.getSize();
      size/=2.0f;
      
      //initialization of dynamics
      cubeShape = 
	new OgreBulletCollisions::BoxCollisionShape(size*scale);
      
      rigidbody =   new OgreBulletDynamics::RigidBody("box"+StringConverter::toString(n), world);
      rigidbody->setShape(cubo_n, cubeShape,
			  0 /* Restitucion */, 0 /* Friccion */,
			  0 /* Masa */,  initial_point+Ogre::Vector3(1.75*j,0, i), 		     Quaternion::IDENTITY /* Orien*/);
      _cubes.push_back(new Cube(cube,cubo_n,rigidbody,cubeShape));
      n++;
     }    
  }
   srand(time(NULL));
 
  for(std::deque<Cube*>::iterator it = _cubes.begin();it!=_cubes.end();it++){
    int pos = rand()%5+1;
    std::cout << pos<< std::endl;
    (*it)->getEntity().setMaterialName("Cube"+StringConverter::toString(pos));
    //cube->getEntity().setMaterialName("Cube2");
    int value=(pos%5 == 0)?0:(fixed--)?1:(two_impact--)?2:3;
    (*it)->setType(value);
  }
 
}


Section::~Section(){

   std::deque<Cube*>::iterator it=_cubes.begin();
   while(it!=_cubes.end()){
     delete *it;
     it++;
   }
}
