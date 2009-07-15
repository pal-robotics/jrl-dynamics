/* @doc Object used to handle hand

   Copyright (c) 2005-2009, 

   @author : 
   Oussama Kanoun
   
   JRL-Japan, CNRS/AIST

   All rights reserved.
   
   Please refers to file License.txt for details on the license.

*/
#include "dynamicsJRLJapan/Hand.h"

using namespace dynamicsJRLJapan;

Hand::Hand(const CjrlJoint* inWristJoint) :
  attAssociatedWrist(inWristJoint)
{
}

Hand::~Hand()
{
}

const CjrlJoint* Hand::associatedWrist()
{
    return attAssociatedWrist;
}

vector3d& Hand::centerInWristFrame()
{
    return attCenter;
}

vector3d& Hand::okayAxisInWristFrame()
{
    return attOkayAxis;
}

vector3d& Hand::showingAxisInWristFrame()
{
    return attShowingAxis;
}

vector3d& Hand::palmAxisInWristFrame()
{
    return attPalmAxis;
}

void Hand::getCenter(vector3d& outCenter) const
{
  outCenter = attCenter;
}

void Hand::setCenter(const vector3d& inCenter)
{
  attCenter = inCenter;
}

void Hand::getThumbAxis(vector3d& outThumbAxis) const
{
  outThumbAxis = attOkayAxis;
}

void Hand::setThumbAxis(const vector3d& inThumbAxis)
{
  attOkayAxis = inThumbAxis;
}

void Hand::getForeFingerAxis(vector3d& outForeFingerAxis) const
{
  outForeFingerAxis = attShowingAxis;
}

void Hand::setForeFingerAxis(const vector3d& inForeFingerAxis)
{
  attShowingAxis = inForeFingerAxis;
}

void Hand::getPalmNormal(vector3d& outPalmNormal) const
{
  outPalmNormal = attPalmAxis;
}

void Hand::setPalmNormal(const vector3d& inPalmNormal)
{
  attPalmAxis = inPalmNormal;
}
