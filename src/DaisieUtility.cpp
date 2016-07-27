/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "DaisieUtility.h"

#include <cmath>
#include <cstdlib>
#include <list>
#include <string>
#include <utility>
#include <vector>

using namespace std;
using namespace cotave;
using namespace daisie;

GLColor DaisieUtility::randomColor(float Satuation, float Value) {
  float Hue = 360.0f * static_cast<float>(rand_r(&rand_seed)) /
              (static_cast<float>(RAND_MAX) + 1.0f);
  int Hi = static_cast<int>(Hue / 60.0f) % 6;
  float f = Hue / 60.0f - static_cast<float>(Hi),
        p = Value * (1.0f - Satuation), q = Value * (1.0f - f * Satuation),
        t = Value * (1.0f - (1.0f - f) * Satuation);

  if (Satuation == 0.0f) {
    Hi = -1;
  }

  switch (Hi) {
    case 0:
      return GLColor(Value, t, p);
    case 1:
      return GLColor(q, Value, p);
    case 2:
      return GLColor(p, Value, t);
    case 3:
      return GLColor(p, q, Value);
    case 4:
      return GLColor(t, p, Value);
    case 5:
      return GLColor(Value, p, q);
    default:
      return GLColor(Value, Value, Value);
  }
}

DaisieUtility::DaisieUtility(Simulator *simulator)
    : simulator(simulator), rand_seed((unsigned int)time(NULL)) {}

SphereObjectPack DaisieUtility::createSphere(double radius, double density,
                                             const string &name_prefix) {
  SphereObjectPack ret;

  ret.body = simulator->getWorld().create<Body>(name_prefix + "::Body");

  Mass m;
  m.setSphere(density, radius);
  ret.body->setMass(m);

  ret.geom = simulator->getSpace().create<SphereGeometry>(name_prefix +
                                                          "::SphereGeometry");
  ret.geom->setRadius(radius);
  ret.geom->connect(ret.body);

  ret.shape = this->create<GLSphereShape>(name_prefix + "::GLSphereShape");
  ret.shape->setRadius(radius);
  GLColor color = randomColor(0.5, 1.0);
  ret.shape->setAmbient(color);
  ret.shape->setDiffuse(color);
  this->connect(ret.body, ret.shape);

  return ret;
}

BoxObjectPack DaisieUtility::createBox(double lx, double ly, double lz,
                                       double density,
                                       const string &name_prefix) {
  BoxObjectPack ret;

  ret.body = simulator->getWorld().create<Body>(name_prefix + "::Body");

  Mass m;
  m.setBox(density, lx, ly, lz);
  ret.body->setMass(m);

  ret.geom =
      simulator->getSpace().create<BoxGeometry>(name_prefix + "::BoxGeometry");
  ret.geom->setLengths(lx, ly, lz);
  ret.geom->connect(ret.body);

  ret.shape = this->create<GLBoxShape>(name_prefix + "::GLBoxShape");
  ret.shape->setLengths(lx, ly, lz);
  GLColor color = randomColor(0.5, 1.0);
  ret.shape->setAmbient(color);
  ret.shape->setDiffuse(color);
  this->connect(ret.body, ret.shape);

  return ret;
}

CapsuleObjectPack DaisieUtility::createCapsule(double radius, double length,
                                               double density,
                                               const string &name_prefix) {
  CapsuleObjectPack ret;

  ret.body = simulator->getWorld().create<Body>(name_prefix + "::Body");

  Mass m;
  m.setCapsule(density, 3, radius, length);
  ret.body->setMass(m);

  ret.geom = simulator->getSpace().create<CapsuleGeometry>(name_prefix +
                                                           "::CapsuleGeometry");
  ret.geom->setParams(radius, length);
  ret.geom->connect(ret.body);

  ret.shape = this->create<GLCapsuleShape>(name_prefix + "::GLCapsuleShape");
  ret.shape->setParams(radius, length);
  GLColor color = randomColor(0.5, 1.0);
  ret.shape->setAmbient(color);
  ret.shape->setDiffuse(color);
  this->connect(ret.body, ret.shape);

  return ret;
}

CylinderObjectPack DaisieUtility::createCylinder(double radius, double length,
                                                 double density,
                                                 const string &name_prefix) {
  CylinderObjectPack ret;

  ret.body = simulator->getWorld().create<Body>(name_prefix + "::Body");

  Mass m;
  m.setCylinder(density, 3, radius, length);
  ret.body->setMass(m);

  ret.geom = simulator->getSpace().create<CylinderGeometry>(
      name_prefix + "::CylinderGeometry");
  ret.geom->setParams(radius, length);
  ret.geom->connect(ret.body);

  ret.shape = this->create<GLCylinderShape>(name_prefix + "::GLCylinderShape");
  ret.shape->setParams(radius, length);
  GLColor color = randomColor(0.5, 1.0);
  ret.shape->setAmbient(color);
  ret.shape->setDiffuse(color);
  this->connect(ret.body, ret.shape);

  return ret;
}

TriangleMeshObjectPack DaisieUtility::createTriangleMesh(
    const vector<ColumnVector3> &vertices, const vector<IntVector3> &indices,
    double density, const string &name_prefix) {
  TriangleMeshObjectPack ret;
  ret.body = simulator->getWorld().create<Body>(name_prefix + "::Body");

  ret.geom = simulator->getSpace().create<TriangleMeshGeometry>(
      name_prefix + "::TriangleMeshGeometry");
  ret.geom->build(vertices, indices);
  ret.geom->connect(ret.body);

  ret.shape =
      this->create<GLTriangleMeshShape>(name_prefix + "::GLTriangleMeshShape");
  ret.shape->setTriangles(vertices, indices);
  GLColor color = randomColor(0.5, 1.0);
  ret.shape->setAmbient(color);
  ret.shape->setDiffuse(color);
  this->connect(ret.body, ret.shape);

  Mass m;
  /// m.setTrimesh(density, ret.geom->id());  // BUGGY
  m.setTriangleMesh(density, vertices, indices);

  ColumnVector3 offset = -m.centerOfMass();
  ret.geom->setPosition(offset);
  m.translate(offset(0), offset(1), offset(2));

  ret.body->setMass(m);
  // ret.geom->connect(ret.body);
  return ret;
}

PlaneObjectPack DaisieUtility::createPlane(double a, double b, double c,
                                           double d, double size, int num,
                                           const string &name_prefix) {
  PlaneObjectPack ret;

  ret.geom = simulator->getSpace().create<PlaneGeometry>(name_prefix +
                                                         "::PlaneGeometry");
  ret.geom->setParams(a, b, c, d);

  ret.shape = this->create<GLPlaneShape>(name_prefix + "::GLPlaneShape");
  ret.shape->setParams(a, b, c, d, size, num,
                       ColumnVector3(a, b, c).normalize());
  GLColor color = randomColor(0.5, 1.0);
  ret.shape->setAmbient(color);
  ret.shape->setDiffuse(color);
  //  this->connect( ret.geom, ret.shape );

  return ret;
}

RayObjectPack DaisieUtility::createRay(double length,
                                       const string &name_prefix) {
  RayObjectPack ret;

  ret.geom =
      simulator->getSpace().create<RayGeometry>(name_prefix + "::RayGeometry");
  ret.geom->setLength(length);

  ret.shape = this->create<GLLineShape>(name_prefix + "::GLLineShape");
  ret.shape->setSegment(ColumnVector3(0.0), ColumnVector3(0.0, 0.0, length));
  GLColor color = randomColor(0.5, 1.0);
  ret.shape->setAmbient(color);
  ret.shape->setDiffuse(color);
  this->connect(ret.geom, ret.shape);

  return ret;
}

bool DaisieUtility::connect(Body *body, GLShape *shape) {
  if (body != NULL && shape != NULL) {
    bodyShapeList.push_back(pair<Body *, GLShape *>(body, shape));
    return true;
  }
  return false;
}

bool DaisieUtility::connect(Geometry *geom, GLShape *shape) {
  if (geom != NULL && shape != NULL) {
    geomShapeList.push_back(pair<Geometry *, GLShape *>(geom, shape));
    return true;
  }
  return false;
}

bool DaisieUtility::disconnect(Body *body, GLShape *shape) {
  if (body != NULL && shape != NULL) {
    bodyShapeList.remove(pair<Body *, GLShape *>(body, shape));
    return true;
  }
  return false;
}

bool DaisieUtility::disconnect(Geometry *geom, GLShape *shape) {
  if (geom != NULL && shape != NULL) {
    geomShapeList.remove(pair<Geometry *, GLShape *>(geom, shape));
    return true;
  }
  return false;
}

void DaisieUtility::syncShape() {
  {
    list<pair<Body *, GLShape *> >::iterator it = bodyShapeList.begin(),
                                             endIt = bodyShapeList.end();

    while (it != endIt) {
      it->second->setPosition(it->first->getPosition());
      it->second->setQuaternion(it->first->getQuaternion());
      ++it;
    }
  }

  {
    list<pair<Geometry *, GLShape *> >::iterator it = geomShapeList.begin(),
                                                 endIt = geomShapeList.end();

    while (it != endIt) {
      it->second->setPosition(it->first->getPosition());
      it->second->setQuaternion(it->first->getQuaternion());
      ++it;
    }
  }
}

void DaisieUtility::update(double timeStep) {
  simulator->update(timeStep);

  // for visualization
  syncShape();
}
