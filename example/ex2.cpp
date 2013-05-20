/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <cmath>

#include <iostream>

#include <daisie.h>

using namespace std;
using namespace glutil;
using namespace daisie;

int main() {
  Simulator sim;
  
  World &world = sim.getWorld();
  world.setGravity( ColumnVector3( 0.0, 0.0, -9.8 ) );

  DaisieUtility util( &sim );

  SphereObjectPack sop = util.createSphere( 0.2, 1000.0, "sphere" );
  SphereObjectPack sop2 = util.createSphere( 0.2, 1000.0, "sphere2" );
  PlaneObjectPack pop = util.createPlane( 0.0, 0.0, 1.0, -1.0,
					  10.0, 4, "plane" );
  
  sop2.body->setPosition( ColumnVector3( 0.0, 0.0, 2.0 ) );

  GLWorld glworld;

  // 光源の作成
  GLLight *light0 = new GLLight( 0 );
  light0->setDiffuse( GLColor( 1.0, 1.0, 1.0, 1.0 ) );
  light0->setSpecular( GLColor( 1.0, 1.0, 1.0, 1.0 ) );
  light0->setAmbient( GLColor( 0.1, 0.1, 0.1, 1.0 ) );
  // 光源をセット
  glworld.addLight( light0 );

  // ObjectPackのGLShapeをセット
  sop.shape->setDiffuse( GLColor( 1.0, 0, 0, 1.0 ) );
  sop.shape->setAmbient( GLColor( 1.0, 0, 0, 1.0 ) );

  sop2.shape->setDiffuse( GLColor( 1.0, 0, 0, 1.0 ) );
  sop2.shape->setAmbient( GLColor( 1.0, 0, 0, 1.0 ) );

  pop.shape->setDiffuse( GLColor( 1.0, 1.0, 0, 1.0 ) );
  pop.shape->setAmbient( GLColor( 1.0, 1.0, 0, 1.0 ) );

  glworld.addShape( sop.shape );
  glworld.addShape( sop2.shape );
  glworld.addShape( pop.shape );

  GLCameraViewer camera;
  camera.open( &glworld, 150, 150, "ex2" );
  
  while ( sim.getTime() < 1.0 ) {
    util.update( 0.01 );
    camera.repaint();
    cout << sim.getTime() << " " << sop.body->getPosition().transpose() << endl;
    usleep( 100000 );
  }

  return 0;
}
