/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _DAISIE_MASS_H_
#define _DAISIE_MASS_H_

#include <cutil/Kinematics.h>

#include <ode/mass.h>

#include <vector>

namespace daisie {

using namespace std;
using namespace cotave;

struct Mass : public dMass {
 public:
  ColumnVector3 centerOfMass() const;
  Matrix33 inertiaMatrix() const;

  void setTriangleMesh(double density, const vector<ColumnVector3> &vertices,
                       const vector<IntVector3> &indices);
};
}  // namespace daisie

#endif
