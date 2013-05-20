/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _DAISIE_DAISIEUTILITY_H_
#define _DAISIE_DAISIEUTILITY_H_

#include <string>

#include <cutil/Kinematics.h>

#include <glutil/glutil.h>

#include "ObjectFactory.h"
#include "Body.h"
#include "Geometry.h"
#include "Simulator.h"

namespace daisie {

using namespace std;
using namespace cotave;
using namespace glutil;

struct SphereObjectPack {
  Body *body;
  SphereGeometry *geom;
  GLSphereShape *shape;
};

struct BoxObjectPack {
  Body *body;
  BoxGeometry *geom;
  GLBoxShape *shape;
};

struct CapsuleObjectPack {
  Body *body;
  CapsuleGeometry *geom;
  GLCapsuleShape *shape;
};

struct CylinderObjectPack {
  Body *body;
  CylinderGeometry *geom;
  GLCylinderShape *shape;
};

struct TriangleMeshObjectPack {
  Body *body;
  TriangleMeshGeometry *geom;
  GLTriangleMeshShape *shape;
};

struct PlaneObjectPack {
  PlaneGeometry *geom;
  GLPlaneShape *shape;
};

struct RayObjectPack {
  RayGeometry *geom;
  GLLineShape *shape;
};


class DaisieUtility 
  : public ObjectFactory< GLShape > {
  
private:

  Simulator *simulator;

  // for visualizaiton
  list< pair< Body*, GLShape* > > bodyShapeList;
  list< pair< Geometry*, GLShape* > > geomShapeList;
  void syncShape();
  
public:

  DaisieUtility( Simulator *simulator );
  
  SphereObjectPack createSphere( double radius, double density, const string &name_prefix = "" );
  BoxObjectPack createBox( double lx, double ly, double lz, double density, const string &name_prefix = "" );
  CapsuleObjectPack createCapsule( double radius, double length, double density, const string &name_prefix = "" );
  CylinderObjectPack createCylinder( double radius, double length, double density, const string &name_prefix = "" );
  TriangleMeshObjectPack createTriangleMesh( const vector< ColumnVector3 > &vertices,
					     const vector< IntVector3 > &indices,
					     double density, 
					     const string &name_prefix = "" );
  
  PlaneObjectPack createPlane( double a, double b, double c, double d, double size = 10.0, int num = 4, const string &name_prefix = "" );
  RayObjectPack createRay( double length, const string &name_prefix = "" );
  
  // for visualizaiton
  bool connect( Body *body, GLShape *shape );
  bool connect( Geometry *geom, GLShape *shape );

  bool disconnect( Body *body, GLShape *shape );
  bool disconnect( Geometry *geom, GLShape *shape );

  // calculation
  void update( double timeStep );

};

}

#endif
