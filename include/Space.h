/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _DAISIE_SPACE_H_
#define _DAISIE_SPACE_H_

#include <list>
#include <map>
#include <string>

#include <ode/collision_space.h>

#include "DaisieObject.h"

namespace daisie {

class Geometry;

using namespace std;

class Space 
  : public DaisieObjectFactory< Space > {
  
private:
  
  dSpaceID _id;
  
  // non-copyable
  Space( const Space& );
  void operator=( const Space& );
  
public:
  
  Space();
  virtual ~Space();
  
  dSpaceID id() const;
  
  void clearFeedback();
};

}

#endif
