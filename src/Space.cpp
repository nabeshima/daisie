/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "Space.h"

#include <ode/collision.h>

#include <list>

#include "Geometry.h"

using namespace std;
using namespace daisie;

Space::Space() : _id(dSimpleSpaceCreate(0)) { dSpaceSetCleanup(_id, 0); }

Space::~Space() { dSpaceDestroy(_id); }

dSpaceID Space::id() const { return _id; }

void Space::clearFeedback() {
  list<DaisieObject *>::iterator it = objList.begin(), endIt = objList.end();

  while (it != endIt) {
    (static_cast<Geometry *>(*it))->clearFeedback();
    ++it;
  }
}
