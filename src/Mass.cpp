/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "Mass.h"

#include <vector>

using namespace cotave;
using namespace daisie;

ColumnVector3 Mass::centerOfMass() const { return ColumnVector3(c); }

Matrix33 Mass::inertiaMatrix() const {
  return Matrix33(I[0], I[1], I[2], I[4], I[5], I[6], I[8], I[9], I[10]);
}

void Mass::setTriangleMesh(double density,
                           const vector<ColumnVector3> &vertices,
                           const vector<IntVector3> &indices) {
  ColumnVector3 com(0.0);
  double vol = 0.0;
  double ixx = 0.0, ixy = 0.0, ixz = 0.0, iyy = 0.0, iyz = 0.0, izz = 0.0;

  int triNum = indices.size();

  for (int i = 0; i < triNum; ++i) {
    const IntVector3 &index = indices[i];
    const ColumnVector3 &a = vertices[index(0)], &b = vertices[index(1)],
                        &c = vertices[index(2)];

    double v = a.outerProduct(b).innerProduct(c);

    com += v * (a + b + c);
    vol += v;

    double kx = a(0) + b(0) + c(0), ky = a(1) + b(1) + c(1),
           kz = a(2) + b(2) + c(2),
           kxx = a(0) * a(0) + b(0) * b(0) + c(0) * c(0),
           kyy = a(1) * a(1) + b(1) * b(1) + c(1) * c(1),
           kzz = a(2) * a(2) + b(2) * b(2) + c(2) * c(2),
           kxy = a(0) * a(1) + b(0) * b(1) + c(0) * c(1),
           kyz = a(1) * a(2) + b(1) * b(2) + c(1) * c(2),
           kzx = a(2) * a(0) + b(2) * b(0) + c(2) * c(0);

    ixx += v * (ky * ky - kyy + kz * kz - kzz);
    iyy += v * (kx * kx - kxx + kz * kz - kzz);
    izz += v * (kx * kx - kxx + ky * ky - kyy);
    ixy += -0.5 * v * (kx * ky + kxy);
    ixz += -0.5 * v * (kx * kz + kzx);
    iyz += -0.5 * v * (ky * kz + kyz);
  }

  com /= (vol * 4.0);
  this->c[0] = com(0);
  this->c[1] = com(1);
  this->c[2] = com(2);

  this->mass = density * vol / 6.0;

  double dd = density / 60.0;
  this->I[0] = ixx * dd;
  this->I[1] = this->I[4] = ixy * dd;
  this->I[2] = this->I[8] = ixz * dd;
  this->I[5] = iyy * dd;
  this->I[6] = this->I[9] = iyz * dd;
  this->I[10] = izz * dd;
}
