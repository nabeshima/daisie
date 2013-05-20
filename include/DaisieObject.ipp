/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

// DaisieObject

inline
DaisieObject::DaisieObject( const string &name )
  : _name( name ) {}

inline
DaisieObject::~DaisieObject() {}

inline
const string& DaisieObject::name() const {
  return _name;
}

template< class MANAGER_CLASS > 
template< class OBJECT_CLASS > 
inline
OBJECT_CLASS* DaisieObjectFactory< MANAGER_CLASS >::create( const string &name ) {
  OBJECT_CLASS *obj = new OBJECT_CLASS( *( static_cast< MANAGER_CLASS* >( this ) ), name );
  add( obj, name );
  return obj;
}
