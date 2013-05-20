/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _DAISIE_JOINT_H_
#define _DAISIE_JOINT_H_

#include <string>

#include <cutil/Kinematics.h>

#include <ode/common.h>

#include "DaisieObject.h"

namespace daisie {

class Body;
class World;

using namespace std;
using namespace cotave;

class Joint 
  : public DaisieObject {

private:
  
  dJointID _id;
  
  // non-copyable
  Joint( const Joint& );
  void operator=( const Joint& );

  dJointFeedback jFeedback;
  
protected:
  
  Joint( dJointID id, const string &name );
  virtual ~Joint();
  
public:
  
  dJointID id() const;
  
  void connect( Body* body1, Body* body2 );
  Body* getConnectedBody( bool isFirst = true ) const;

  void setFeedbackEnable( bool onoff );
  bool isFeedbackEnabled() const;
  ColumnVector3 getFeedbackForce( bool isFirst = true ) const;
  ColumnVector3 getFeedbackTorque( bool isFirst = true ) const;
};


class BallJoint
  : public Joint {

  friend class DaisieObjectFactory< World >;
  
protected:

  BallJoint( World &world, const string &name );
  
public:
  
  void setAnchor( const ColumnVector3 &anchor );
  ColumnVector3 getAnchor() const;
};


class HingeJoint 
  : public Joint {

  friend class DaisieObjectFactory< World >;
  
protected:

  HingeJoint( World &world, const string &name );

public:
  
  void setAnchor( const ColumnVector3 &anchor );
  void setAxis( const ColumnVector3 &axis );
  void setLoStop( double loStop );
  void setHiStop( double hiStop);
  void setVelocity( double vel );
  void setFMax( double fMax );
  void setFudgeFactor( double fudgeFactor );
  void setBounce( double bounce );
  void setCFM( double cfm );
  void setStopERP( double erp );
  void setStopCFM( double stopCFM );

  double getAngle() const;
  double getAngleRate() const;
  double getLoStop() const;
  double getHiStop() const;
  ColumnVector3 getAnchor( bool isFirst ) const;

  void addTorque( double torque );
};


class Hinge2Joint 
  : public Joint {

  friend class DaisieObjectFactory< World >;
  
protected:

  // constructer used in World
  Hinge2Joint( World &world, const string &name );
  
public:
  
  void setAnchor( const ColumnVector3 &anchor );
  void setAxis( const ColumnVector3 &axis, bool isFirst );
  void setLoStop( double loStop, bool isFirst );
  void setHiStop( double hiStop, bool isFirst );
  void setVelocity( double vel, bool isFirst );
  void setFMax( double fMax, bool isFirst );
  void setFudgeFactor( double fudgeFactor, bool isFirst );
  void setBounce( double bounce, bool isFirst );
  void setCFM( double cfm, bool isFirst );
  void setStopERP( double stopERP, bool isFirst );
  void setStopCFM( double stopCFM, bool isFirst );

  double getAngle( bool isFirst = true ) const;
  double getAngleRate( bool isFirst ) const;
  double getLoStop( bool isFirst ) const;
  double getHiStop( bool isFirst ) const;
  ColumnVector3 getAnchor( bool isFirst ) const;

  void addTorque( double torque, bool isFirst );
};


class SliderJoint 
  : public Joint {

  friend class DaisieObjectFactory< World >;
  
protected:

  // constructer used in World
  SliderJoint( World &world, const string &name );
  
public:
  void setAxis( const ColumnVector3 &axis );
  void setLoStop( double loStop );
  void setHiStop( double hiStop );
  void setVelocity( double vel );
  void setFMax( double fMax );
  void setFudgeFactor( double fudgeFactor );
  void setBounce( double bounce );
  void setCFM( double cfm );
  void setStopERP( double erp );
  void setStopCFM( double stopCFM );

  double getPosition() const;
  double getPositionRate() const;  

  void addForce( double force );
};


class UniversalJoint 
  : public Joint {

  friend class DaisieObjectFactory< World >;
  
protected:

  UniversalJoint( World &world, const string &name );

public:
  
  void setAnchor( const ColumnVector3 &anchor );
  void setAxis( const ColumnVector3 &axis, bool isFirst );
  void setLoStop( double loStop, bool isFirst );
  void setHiStop( double hiStop, bool isFirst );
  void setVelocity( double vel, bool isFirst );
  void setFMax( double fMax, bool isFirst );
  void setFudgeFactor( double fudgeFactor, bool isFirst );
  void setBounce( double bounce, bool isFirst );
  void setCFM( double cfm, bool isFirst );
  void setStopERP( double stopERP, bool isFirst );
  void setStopCFM( double stopCFM, bool isFirst );

  double getAngle( bool isFirst ) const;
  double getAngleRate( bool isFirst ) const;
  double getLoStop( bool isFirst ) const;
  double getHiStop( bool isFirst ) const;
  ColumnVector3 getAnchor( bool isFirst ) const;

  void addTorque( double torque, bool isFirst );  
};


// not yet implemented.
class PistonJoint 
  : public Joint {

  friend class DaisieObjectFactory< World >;

protected:

  PistonJoint( World &world, const string &name );
  
public:
  
};


class FixedJoint 
  : public Joint {

  friend class DaisieObjectFactory< World >;

protected:

  FixedJoint( World &world, const string &name );
};


// not yet implemented.
class AMotorJoint 
  : public Joint {

  friend class DaisieObjectFactory< World >;

protected:

  AMotorJoint( World &world, const string &name );
  
public:
  
};


// not yet implemented.
class LMotorJoint 
  : public Joint {
  
  friend class DaisieObjectFactory< World >;

protected:

  LMotorJoint( World &world, const string &name );
  
public:
  
};


class Plane2DJoint
  : public Joint {

  friend class DaisieObjectFactory< World >;

protected:

  Plane2DJoint( World &world, const string &name );
}; 

}

#endif
