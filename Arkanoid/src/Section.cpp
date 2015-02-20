
#include "Section.h"

Section::Section(){
}


Section::Section(OgreBulletDynamics::DynamicsWorld* world,int difficult,  Ogre::Vector3& frb, Ogre::Vector3& nrb, Ogre::Vector3& flb, Ogre::Vector3& nlb){

  Ogre::Real dist_x = frb.distance(flb);
  int cubes_line = 7;
  float len_x_cube = 0.55;//dist_x/(float)cubes_line;
  Ogre::Vector3 scale(len_x_cube, 0.2, 0.2), initialPoint(frb.x + len_x_cube + 0.5,0, frb.z+ 0.2*2 );
  _ncubes = cubes_line* cubes_line;
  switch(difficult){
  case 0://easy level
    _fixed = 0;
    _two_impact = _ncubes/5;
    break;
  case 1:
    _fixed = _two_impact = _ncubes / 5;
    break;
  case 2:
    _fixed = _ncubes/5;
    _two_impact = 2*_ncubes/5;
    break;
  }
  createTable(world, _ncubes, cubes_line, _fixed, _two_impact, scale, initialPoint);
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
			  0.2 /* Restitucion */, 0 /* Friccion */,
			  0 /* Masa */,  initial_point+Ogre::Vector3(1.75*j,0, i), 		     Quaternion::IDENTITY /* Orien*/);
      _cubes.push_back(new Cube(cube,cubo_n,rigidbody,cubeShape));
      n++;
     }    
  }
   srand(time(NULL));
   
   std::deque<Cube*> tmp(_cubes);
   while(fixed && tmp.size() > 0 ){
     //   int kind = rand()%5+1;
       int pos = rand()%tmp.size();
       if(tmp[pos]!=NULL){
	 // tmp[pos]->getEntity().setMaterialName("Cube"+StringConverter::toString(kind));
	 tmp[pos]->getEntity().setMaterialName("Cube1");
	 tmp[pos]->setType(1);
	 tmp[pos]->setRemaining(-1);
	 fixed--;
	 tmp.erase(tmp.begin()+pos);

       }
   }
   while(two_impact && tmp.size() > 0 ){
     int kind = (rand()%4+1)+1;
     int pos = rand()%tmp.size();
     if(tmp[pos]!=NULL){
       tmp[pos]->getEntity().setMaterialName("Cube"+StringConverter::toString(kind));
       tmp[pos]->setType(2);
       two_impact--;
       tmp[pos]->setRemaining(2);
       tmp.erase(tmp.begin()+pos);
     }
   }
      
  for(std::deque<Cube*>::iterator it = tmp.begin();it!=tmp.end();it++){
    int kind = (rand()%4+1)+1;
    (*it)->getEntity().setMaterialName("Cube"+StringConverter::toString(kind));
    (*it)->setType(0);
    (*it)->setRemaining(1);
  }


  // for(std::deque<Cube*>::iterator it = _cubes.begin();it!=_cubes.end();it++){
  //   int pos = rand()%5+1;
  //   std::cout << pos<< std::endl;
  //   (*it)->getEntity().setMaterialName("Cube"+StringConverter::toString(pos));
  //   //cube->getEntity().setMaterialName("Cube2");
  //   int value=(pos%5 == 0)?0:(fixed--)?1:(two_impact--)?2:0;
  //   (*it)->setType(value);
  //   (*it)->setRemaining((pos==);

  // }
 
}


Section::~Section(){

   std::deque<Cube*>::iterator it=_cubes.begin();
   while(it!=_cubes.end()){
     delete *it;
     it++;
   }
}

bool Section::isWin(){
  return (int)_cubes.size() == (int) _fixed;
}
