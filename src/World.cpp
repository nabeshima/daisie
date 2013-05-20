/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "World.h"

#include <ode/objects.h>

using namespace cotave;
using namespace daisie;

World::World()
  : _id( dWorldCreate() ) {}


World::~World() {
  destroyAll(); // to avoid segmentation fault in ODE.
  dWorldDestroy( _id );
}

dWorldID World::id() const {
  return _id;
}

void World::setGravity( const ColumnVector3 &g ) {
  dWorldSetGravity( _id, g( 0 ), g( 1 ), g( 2 ) );
}

void World::setCFM( double cfm ) {
  dWorldSetCFM( _id, cfm );
}    

void World::setERP( double erp ) {
  dWorldSetERP( _id, erp );
}

ColumnVector3 World::getGravity() const {
  dVector3 g;
  dWorldGetGravity( _id, g );
  return ColumnVector3( g[ 0 ], g[ 1 ], g[ 2 ] );
}
 
double World::getCFM() const {
  return dWorldGetCFM( _id );
}

double World::getERP() const {
  return dWorldGetERP( _id );
}

void World::step( double timeStep ) {
  dWorldStep( _id, timeStep );
}
