/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _DAISIE_SIMULATOR_H_
#define _DAISIE_SIMULATOR_H_

#include <map>

#include <ode/common.h>
#include <ode/contact.h>

#include "World.h"
#include "Space.h"

namespace daisie {

class Body;
class Geometry;

class Simulator {
  
private:
  
  // ODE body/joint world
  World world;

  // ODE geometry space
  Space space;
  
  // Contacts between Geometries to be ignored
  multimap< dGeomID, dGeomID > ignoredGeomPair;
  
  const int maxContacts;
  dSurfaceParameters surfaceParams;
  
  double simTime;                    //! simulation time [sec]
  
  void addIgnoredGeometryPair( dGeomID geom1, dGeomID geom2 );
  void removeIgnoredGeometryPair( dGeomID geom1, dGeomID geom2 );
  bool isIgnored( dGeomID geom1, dGeomID geom2 ) const;

  // non-copyable
  Simulator( const Simulator& );
  void operator=( const Simulator& );
  
public:
  
  Simulator( int maxContacts = 64 ); // 4 is sufficient unless TriangleMeshGeometry is used.
  
  World& getWorld();
  Space& getSpace();
  
  bool addIgnoredGeometryPair( Geometry* geom1, Geometry* geom2 );
  bool removeIgnoredGeometryPair( Geometry* geom1, Geometry* geom2 );
  bool isIgnored( Geometry* geom1, Geometry* geom2 ) const;
    
  // calculation
  void update( double timeStep );
  
  double getTime() const;
};

}


#endif
