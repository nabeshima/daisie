/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _DAISIE_GEOMETRY_H_
#define _DAISIE_GEOMETRY_H_

#include <cutil/Kinematics.h>

#include <ode/collision.h>

#include <string>
#include <vector>

#include "DaisieObject.h"

namespace daisie {

class Body;
class Geometry;
class Space;
class NearCallback;

using namespace std;
using namespace cotave;

struct CollisionData {
  Geometry *opposite;
  double depth;
  ColumnVector3 absPos;
  ColumnVector3 relPos;
  ColumnVector3 absNormal;
  ColumnVector3 relNormal;
  ColumnVector3 absForce;
  ColumnVector3 relForce;
  ColumnVector3 absTorque;
  ColumnVector3 relTorque;
};

class Geometry : public DaisieObject {
  friend class NearCallback;

 private:
  dGeomID _id;

  bool keepColData;
  vector<CollisionData> colDataVec;

  // non-copyable
  Geometry(const Geometry &);
  void operator=(const Geometry &);

 protected:
  Geometry(Space *space, dGeomID id, const string &name);
  virtual ~Geometry();

 public:
  dGeomID id() const;

  void setPosition(const ColumnVector3 &p);
  void setQuaternion(const Quaternion &q);

  ColumnVector3 getPosition() const;
  Quaternion getQuaternion() const;

  void connect(Body *body);
  Body *getConnectedBody() const;

  void setFeedbackEnable(bool onoff);
  bool isFeedbackEnabled() const;

  const vector<CollisionData> &getFeedback() const;
  void clearFeedback();
};

class SphereGeometry : public Geometry {
  friend class DaisieObjectFactory<Space>;

 protected:
  SphereGeometry(Space *space, const string &name);

 public:
  void setRadius(double radius);
};

class BoxGeometry : public Geometry {
  friend class DaisieObjectFactory<Space>;

 protected:
  BoxGeometry(Space *space, const string &name);

 public:
  void setLengths(double lx, double ly, double lz);
};

class PlaneGeometry : public Geometry {
  friend class DaisieObjectFactory<Space>;

 protected:
  PlaneGeometry(Space *space, const string &name);

 public:
  // These parameters are for ax + by + cz = d
  void setParams(double a, double b, double c, double d);
};

class CapsuleGeometry : public Geometry {
  friend class DaisieObjectFactory<Space>;

 protected:
  CapsuleGeometry(Space *space, const string &name);

 public:
  void setParams(double radius, double length);
};

class CylinderGeometry : public Geometry {
  friend class DaisieObjectFactory<Space>;

 protected:
  CylinderGeometry(Space *space, const string &name);

 public:
  void setParams(double radius, double length);
};

class RayGeometry : public Geometry {
  friend class DaisieObjectFactory<Space>;

 protected:
  RayGeometry(Space *space, const string &name);

 public:
  void setLength(double length);
};

class TriangleMeshGeometry : public Geometry {
  friend class DaisieObjectFactory<Space>;

 private:
  dTriMeshDataID Data;

  int vertexNum;
  double **vertexBuf;
  // float **vertexBuf;

  int indexNum;
  dTriIndex *indexBuf;

  void destroy();

 protected:
  TriangleMeshGeometry(Space *space, const string &name);
  ~TriangleMeshGeometry();

 public:
  void build(const vector<ColumnVector3> &vertices,
             const vector<IntVector3> &indices);
};
}  // namespace daisie

#endif
