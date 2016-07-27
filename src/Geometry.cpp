/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "Geometry.h"

#include <ode/objects.h>

#include <string>
#include <vector>

#include "Body.h"
#include "Space.h"

using namespace std;
using namespace cotave;
using namespace daisie;

// Geometry

Geometry::Geometry(Space *space, dGeomID id, const string &name)
    : DaisieObject(name), _id(id) {
  //  dGeomTransformSetCleanup( _id, 1 );    // geomIDがdestroy時に,
  //  transferedGeomIDもdestroy
  //  dGeomTransformSetGeom( _id, _transID );
  dGeomSetData(_id, this);
}

Geometry::~Geometry() { dGeomDestroy(_id); }

dGeomID Geometry::id() const { return _id; }

void Geometry::setPosition(const ColumnVector3 &p) {
  dGeomSetPosition(_id, p(0), p(1), p(2));
}

void Geometry::setQuaternion(const Quaternion &q) {
  dGeomSetQuaternion(_id, q.data());
}

ColumnVector3 Geometry::getPosition() const {
  return ColumnVector3(dGeomGetPosition(_id));
}

Quaternion Geometry::getQuaternion() const {
  dQuaternion q;
  dGeomGetQuaternion(_id, q);
  return Quaternion(q);
}

void Geometry::connect(Body *body) {
  dGeomSetBody(_id, ((body != NULL) ? body->id() : NULL));
}

Body *Geometry::getConnectedBody() const {
  return static_cast<Body *>(dBodyGetData(dGeomGetBody(_id)));
}

void Geometry::setFeedbackEnable(bool onoff) { keepColData = onoff; }

bool Geometry::isFeedbackEnabled() const { return keepColData; }

const vector<CollisionData> &Geometry::getFeedback() const {
  return colDataVec;
}

void Geometry::clearFeedback() { colDataVec.clear(); }

// SphereGeometry

SphereGeometry::SphereGeometry(Space *space, const string &name)
    : Geometry(space, dCreateSphere(space->id(), 0.0), name) {}

void SphereGeometry::setRadius(double radius) {
  dGeomSphereSetRadius(id(), radius);
}

// BoxGeometry

BoxGeometry::BoxGeometry(Space *space, const string &name)
    : Geometry(space, dCreateBox(space->id(), 0.0, 0.0, 0.0), name) {}

void BoxGeometry::setLengths(double lx, double ly, double lz) {
  dGeomBoxSetLengths(id(), lx, ly, lz);
}

// PlaneGeometry

PlaneGeometry::PlaneGeometry(Space *space, const string &name)
    : Geometry(space, dCreatePlane(space->id(), 0.0, 0.0, 1.0, 0.0), name) {}

void PlaneGeometry::setParams(double a, double b, double c, double d) {
  dGeomPlaneSetParams(id(), a, b, c, d);
}

// CapsuleGeometry

CapsuleGeometry::CapsuleGeometry(Space *space, const string &name)
    : Geometry(space, dCreateCapsule(space->id(), 0.0, 0.0), name) {}

void CapsuleGeometry::setParams(double radius, double length) {
  dGeomCapsuleSetParams(id(), radius, length);
}

// CylinderGeometry

CylinderGeometry::CylinderGeometry(Space *space, const string &name)
    : Geometry(space, dCreateCylinder(space->id(), 0.0, 0.0), name) {}

void CylinderGeometry::setParams(double radius, double length) {
  dGeomCylinderSetParams(id(), radius, length);
}

// RayGeometry

RayGeometry::RayGeometry(Space *space, const string &name)
    : Geometry(space, dCreateRay(space->id(), 0.0), name) {
  dGeomRaySet(id(), 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
}

void RayGeometry::setLength(double length) { dGeomRaySetLength(id(), length); }

// TriangleMeshGeometry

TriangleMeshGeometry::TriangleMeshGeometry(Space *space, const string &name)
    : Geometry(space, dCreateTriMesh(space->id(), dGeomTriMeshDataCreate(),
                                     NULL, NULL, NULL),
               name),
      Data(dGeomTriMeshGetData(id())),
      vertexNum(0),
      vertexBuf(NULL),
      indexNum(0),
      indexBuf(NULL) {}

TriangleMeshGeometry::~TriangleMeshGeometry() {
  destroy();
  dGeomTriMeshDataDestroy(Data);
}

void TriangleMeshGeometry::build(const vector<ColumnVector3> &vertices,
                                 const vector<IntVector3> &indices) {
  destroy();

  vertexNum = vertices.size();
  vertexBuf = new double *[vertexNum];
  //  vertexBuf = new float *[vertexNum];
  for (int i = 0; i < vertexNum; ++i) {
    const ColumnVector3 &tmp = vertices[i];
    vertexBuf[i] = new double[3];
    //    vertexBuf[i] = new float[3];
    vertexBuf[i][0] = tmp(0);
    vertexBuf[i][1] = tmp(1);
    vertexBuf[i][2] = tmp(2);
  }

  indexNum = indices.size();
  indexBuf = new dTriIndex[3 * indexNum];
  for (int i = 0; i < indexNum; ++i) {
    const IntVector3 &tmp = indices[i];
    indexBuf[3 * i + 0] = tmp(0);
    indexBuf[3 * i + 1] = tmp(1);
    indexBuf[3 * i + 2] = tmp(2);
  }

  dGeomTriMeshDataBuildDouble(Data, vertexBuf, 3 * sizeof(double), vertexNum,
                              indexBuf, 3 * indexNum, 3 * sizeof(dTriIndex));
  // dGeomTriMeshDataBuildSingle(Data, vertexBuf, 3 * sizeof(float), vertexNum,
  // indexBuf, 3 * indexNum, 3 * sizeof(dTriIndex));
}

void TriangleMeshGeometry::destroy() {
  if (vertexBuf != NULL) {
    for (int i = 0; i < vertexNum; ++i) {
      delete[] vertexBuf[i];
    }
    delete[] vertexBuf;
    delete[] indexBuf;

    vertexNum = 0;
    vertexBuf = NULL;
    indexNum = 0;
    indexBuf = NULL;
  }
}

// int main() {
//   Space space;
//   BoxGeometry *b = space.create< BoxGeometry >( "b" );
//   SphereGeometry *s = space.create< SphereGeometry >( "s" );
//   PlaneGeometry *p = space.create< PlaneGeometry >( "p" );
//   CylinderGeometry *cy = space.create< CylinderGeometry >( "cy" );
//   CapsuleGeometry *ca = space.create< CapsuleGeometry >( "ca" );
//   TriangleMeshGeometry *t = space.create< TriangleMeshGeometry >( "t" );
//   return 0;
// }
