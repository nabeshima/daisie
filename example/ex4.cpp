/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <cmath>

#include <iostream>

#include <cutil/STLFileIO.h>

#include <daisie.h>

using namespace std;
using namespace cutil;
using namespace glutil;
using namespace daisie;

int main() {
  Simulator sim;
  
  World &world = sim.getWorld();
  world.setGravity( ColumnVector3( 0.0, 0.0, -9.8 ) );

  DaisieUtility util( &sim );

  SphereObjectPack sop = util.createSphere( 0.2, 1000.0, "sphere" );
  SphereObjectPack sop2 = util.createSphere( 0.2, 1000.0, "sphere2" );
  SphereObjectPack sop3 = util.createSphere( 0.4, 1000.0, "sphere2" );
  PlaneObjectPack pop = util.createPlane( 0.0, 0.0, 1.0, -1.0,
					  10.0, 4, "plane" );
  
  sop2.body->setPosition( ColumnVector3( 0.0, 0.0, 0.4 ) );
  sop3.body->setPosition( ColumnVector3( 0.0, 0.0, 1.0 ) );
  
  HingeJoint *joint = world.create< HingeJoint >( "hinge" );
  joint->connect( sop.body, sop2.body );
  joint->setAnchor( ColumnVector3( 0.0, 0.0, 0.2 ) );
  joint->setAxis( ColumnVector3( 1.0, 0.0, 0.0 ) );
  joint->setFMax( 10.0 );

  HingeJoint *joint2 = world.create< HingeJoint >( "hinge2" );
  joint2->connect( sop2.body, sop3.body );
  joint2->setAnchor( ColumnVector3( 0.0, 0.0, 0.8 ) );
  joint2->setAxis( ColumnVector3( 1.0, 0.0, 0.0 ) );
  joint2->setFMax( 10.0 );

  sim.addIgnoredGeometryPair( sop.geom, sop2.geom );
  sim.addIgnoredGeometryPair( sop2.geom, sop3.geom );


  vector< ColumnVector3 > vertices;
  vector< IntVector3 > indices;
  // vertices.push_back( ColumnVector3( 0.0, 0.0, 0.0 ) );
  // vertices.push_back( ColumnVector3( 1.0, 0.0, 0.0 ) );
  // vertices.push_back( ColumnVector3( 0.0, 1.2, 0.0 ) );
  // vertices.push_back( ColumnVector3( 0.0, 0.0, 1.1 ) );
  // indices.push_back( IntVector3( 0, 2, 1 ) );
  // indices.push_back( IntVector3( 0, 1, 3 ) );
  // indices.push_back( IntVector3( 0, 3, 2 ) );
  // indices.push_back( IntVector3( 1, 2, 3 ) );

  STLFileIO::read( "monkey_binary.stl", vertices, indices );
  //  STLFileIO::read( "box.stl", vertices, indices );

  //  STLFileIO::read( "cube-ascii.stl", vertices, indices );
  
  TriangleMeshObjectPack top = util.createTriangleMesh( vertices, indices, 1000.0, "tri" );  
  top.body->setPosition( ColumnVector3( 0.0, 0.0, 11 ) );
  
  
  GLWorld glworld;

  // 光源の作成
  GLLight *light0 = new GLLight( 0 );
  light0->setDiffuse( GLColor( 1.0, 1.0, 1.0, 1.0 ) );
  light0->setSpecular( GLColor( 1.0, 1.0, 1.0, 1.0 ) );
  light0->setAmbient( GLColor( 0.1, 0.1, 0.1, 1.0 ) );
  light0->setPosition( ColumnVector3( 1.0, 1.0, 1.0 ) );
  // 光源をセット
  glworld.addLight( light0 );

  // ObjectPackのGLShapeをセット
  glworld.addShape( sop.shape );
  glworld.addShape( sop2.shape );
  glworld.addShape( sop3.shape );
  glworld.addShape( pop.shape );
  glworld.addShape( top.shape );


  //! 三軸の作成
  GLArrowShape axisX, axisY, axisZ;
  axisX.setParams( 0.01, 0.5, 0.03, 0.1 );
  axisX.setDiffuse( GLColor( 1.0, 0, 0, 1.0 ) );
  axisX.setAmbient( GLColor( 1.0, 0, 0, 1.0 ) );
  axisX.setQuaternion( Quaternion( ColumnVector3( 0, 1, 0 ), M_PI / 2.0 ) );
  axisY.setParams( 0.01, 0.5, 0.03, 0.1 );
  axisY.setDiffuse( GLColor( 0, 1.0, 0, 1.0 ) );
  axisY.setAmbient( GLColor( 0, 1.0, 0, 1.0 ) );
  axisY.setQuaternion( Quaternion( ColumnVector3( -1, 0, 0 ), M_PI / 2.0 ) );
  axisZ.setParams( 0.01, 0.5, 0.03, 0.1 );
  axisZ.setDiffuse( GLColor( 0, 0, 1.0, 1.0 ) );
  axisZ.setAmbient( GLColor( 0, 0, 1.0, 1.0 ) );

  //! 三軸をセット
  glworld.addShape( &axisX );
  glworld.addShape( &axisY );
  glworld.addShape( &axisZ );



  GLCameraViewer camera;
  camera.open( &glworld, 150, 150, "ex4" );
  
  while ( sim.getTime() < 30.0 ) {

    sop.body->setLinearVelocity( ColumnVector3( 0.1, 0.0, 0.0 ) );
    sop.body->setAngularVelocity( ColumnVector3( 10.0, 0.0, 0.0 ) );
    //    joint->setVelocity( 10.0 );
    //    joint2->setVelocity( 20.0 );

    util.update( 0.01 );
    camera.repaint();
    cout << sim.getTime() << " " 
	 << sop.body->getPosition().transpose() << " "
	 << joint->getAngle() << " "
	 << endl;
    //    usleep( 100000 );
  }

  return 0;
}
