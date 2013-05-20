/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _DAISIE_DAISIEOBJECT_H_
#define _DAISIE_DAISIEOBJECT_H_

#include "ObjectFactory.h"

namespace daisie {

using namespace std;

class DaisieObject {

private:
  
  string _name;
  
public:
  
  DaisieObject( const string &name );
  virtual ~DaisieObject();
  
  const string& name() const;
};


template< class MANAGER_CLASS > 
class DaisieObjectFactory
  : public ObjectFactory< DaisieObject > {
  
public:
  
  // override
  template< class OBJECT_CLASS > 
  OBJECT_CLASS* create( const string &name );
};

#include "DaisieObject.ipp"

}

#endif
