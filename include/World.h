/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _DAISIE_WORLD_H_
#define _DAISIE_WORLD_H_

#include <cutil/Kinematics.h>

#include <ode/common.h>

#include <list>
#include <map>
#include <string>

#include "DaisieObject.h"

namespace daisie {

using namespace std;
using namespace cotave;

class World : public DaisieObjectFactory<World> {
 private:
  dWorldID _id;

  // non-copyable
  World(const World&);
  void operator=(const World&);

 public:
  World();
  virtual ~World();

  dWorldID id() const;

  void setGravity(const ColumnVector3& g);
  void setCFM(double cfm);
  void setERP(double erp);

  ColumnVector3 getGravity() const;
  double getCFM() const;
  double getERP() const;

  void step(double timeStep);
};
}  // namespace daisie

#endif
