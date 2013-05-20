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
using namespace daisie;

int main() {
  Simulator sim;
  
  World &world = sim.getWorld();
  world.setGravity( ColumnVector3( 0.0, 0.0, -9.8 ) );

  // // system default
  // world.setCFM( 1.0e-9 );
  // world.setERP( 0.2 );
  
  Body *b = world.create< Body >( "b" );
  
  Mass m;
  dMassSetSphere( &m, 1000.0, 0.2 );
  b->setMass( m );

  Space &space = sim.getSpace();
  PlaneGeometry *p = space.create< PlaneGeometry >( "p" );
  p->setParams( 0.0, 0.0, 1.0, -1.0 );
  
  SphereGeometry *s = space.create< SphereGeometry >( "s" );
  s->setRadius( 0.2 );
  s->connect( b );
  
  //  sim.addIgnoredGeometryPair( p, s );

  while ( sim.getTime() < 1.0 ) {
    sim.update( 0.01 );
    cout << sim.getTime() << " " << b->getPosition().transpose() << endl;
  }

  return 0;
}
