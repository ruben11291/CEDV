
#include "Section.h"
Section::Section(){
}
Section::Section(OgreBulletDynamics::DynamicsWorld* world,int difficult, float x0,  float xf, float y0, float yf){
  double dist_x = abs(xf-x0);
  int cubes_line = 12;
  float len_x_cube = dist_x/(float)cubes_line;
  //  float len_y_cube = dist_y/(float)cubes_line;
  Ogre::Vector3 scale(len_x_cube, 0.2, 0.2);
  int ncubes, fixed, two_impact;
  
  switch(difficult){
  case 0://easy level
    ncubes = cubes_line*4;
    fixed = 0;
    two_impact = ncubes/5;
    break;
  case 1:
    ncubes = cubes_line*6;
    fixed = two_impact = ncubes / 5;
    break;
  case 2:
    ncubes = cubes_line * cubes_line;
    fixed = ncubes/5;
    two_impact = 2*ncubes/5;
    break;
  }
  createTable(world,ncubes, cubes_line, fixed,two_impact, scale, x0, y0);
}

std::vector <Cube*>& Section::getCubes(){
  return _cubes;
}


void Section::createTable(OgreBulletDynamics::DynamicsWorld* world,int ncubes, int cubes_line, int fixed, int two_impact, Ogre::Vector3 scale, float x0, float y0){

  Ogre::Entity* cube;
   Ogre::SceneNode* cubo_n;
   Ogre::SceneManager * _sceneMgr= Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager");
   AxisAlignedBox boundingB;
   Ogre::Vector3 size;
   OgreBulletCollisions::BoxCollisionShape *cubeShape ;
   OgreBulletDynamics::RigidBody * rigidbody;

  for(;ncubes > 0;ncubes--){
      cube = _sceneMgr->createEntity("cube.mesh");
      cubo_n = _sceneMgr->createSceneNode("cube"+StringConverter::toString(ncubes));
      //     cube->setMaterialName("Cube"+StringConverter::toString(i));
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
      
      rigidbody =   new OgreBulletDynamics::RigidBody("box"+StringConverter::toString(ncubes), world);
      rigidbody->setShape(cubo_n, cubeShape,
			  50 /* Restitucion */, 0 /* Friccion */,
			  0 /* Masa */, /*Ogre::Vector3(((int)x0%cubes_line)+(float)size.x+ncubes,0,y0*(ncubes%cubes_line)) /* Posicion inicial */ Ogre::Vector3(x0+size.x,0,y0+size.x), 		     Quaternion::IDENTITY /* Orientacion */);///TO DO WELL
      _cubes.push_back(new Cube(cube,cubo_n,rigidbody,cubeShape));
    
  }
  
  std::vector <Cube*> tmp(_cubes);
  srand(time(NULL));
  for(uint i = 0;i<tmp.size();i++){
    int pos = rand()%tmp.size();
    Cube * cube = tmp[pos];
    (cube->getEntity()).setMaterialName("Cube"+StringConverter::toString(pos%5+1));
    int value=(pos%5 == 0)?0:(fixed--)?1:(two_impact--)?2:3;
    cube->setType(value);
    std::remove(tmp.begin(),tmp.end(),tmp[pos]);
  }
 
}


Section::~Section(){

   std::vector<Cube*>::iterator it=_cubes.begin();
   while(it!=_cubes.end()){
     delete *it;
     it++;
   }
}
