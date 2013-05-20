/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _DAISIE_OBJECTFACTORY_H_
#define _DAISIE_OBJECTFACTORY_H_

#include <list>
#include <map>
#include <string>
#include <algorithm>

namespace daisie {

using namespace std;

template< class BASE_CLASS >
class ObjectFactory {
  
protected:  
  
  list< BASE_CLASS* > objList;
  map< const string, BASE_CLASS* > objMap;
  
  virtual ~ObjectFactory();
  
  void add( BASE_CLASS *obj, const string &name );
  void destroyAll();
  
public:
  
  template< class OBJECT_CLASS > 
  OBJECT_CLASS* create( const string &name );
  bool destroy( BASE_CLASS* obj );
  bool destroy( const string &name );
  BASE_CLASS* get( const string &name ) const;
};

#include "ObjectFactory.ipp"

}

#endif
