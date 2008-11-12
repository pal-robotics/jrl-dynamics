#include <string>
#include "dynamicsJRLJapan/Joint.h"
#include "dynamicsJRLJapan/HumanoidDynamicMultiBody.h"
#include "robotDynamics/jrlRobotDynamicsObjectConstructor.h"
#include "dynamicsJRLJapan/robotDynamicsImpl.h"

#include "jrlMathTools/constants.h"

static CjrlRobotDynamicsObjectConstructor <
  CimplDynamicRobot, 
  CimplHumanoidDynamicRobot, 
  CimplJointFreeFlyer, 
  CimplJointRotation,
  CimplJointTranslation,
  CimplBody >  robotDynamicsObjectConstructor;


using namespace std;
using namespace dynamicsJRLJapan;

/*
  This function computes an homogeneous matrix moving 
    (0,0,0) to inCenter and
    x-axis to vector inAxis
*/

static matrix4d getPoseFromAxisAndCenter(const vector3d inAxis, const vector3d inCenter)
{
  matrix4d outPose;
  vector3d x(inAxis);
  vector3d y;
  vector3d z;
  int smallestComponent(0);

  x.normalize();

  if((fabs(x[0]) <= fabs(x[1])) && (fabs(x[0]) <= fabs(x[2]))) {
    smallestComponent = 0;
  }
  else if((fabs(x[1]) <= fabs(x[0])) && (fabs(x[1]) <= fabs(x[2]))) {
    smallestComponent = 1;
  }
  else if((fabs(x[2]) <= fabs(x[0])) && (fabs(x[2]) <= fabs(x[1]))) {
    smallestComponent = 2;
  }

  y[smallestComponent] = 1.;

  // we just built a y vector that is neither colinear nor nearly colinear with inAxis
  
  z = x * y;
  z.normalize();
  
  y = z * x;
  
  // (inAxis, y, z) is now a directly-oriented orthonormal frame
  
  MAL_S4x4_MATRIX_ACCESS_I_J(outPose,0,0) = x[0];
  MAL_S4x4_MATRIX_ACCESS_I_J(outPose,1,0) = x[1];
  MAL_S4x4_MATRIX_ACCESS_I_J(outPose,2,0) = x[2];
  MAL_S4x4_MATRIX_ACCESS_I_J(outPose,3,0) = inCenter[0];

  MAL_S4x4_MATRIX_ACCESS_I_J(outPose,0,1) = y[0];
  MAL_S4x4_MATRIX_ACCESS_I_J(outPose,1,1) = y[1];
  MAL_S4x4_MATRIX_ACCESS_I_J(outPose,2,1) = y[2];
  MAL_S4x4_MATRIX_ACCESS_I_J(outPose,3,1) = inCenter[1];

  MAL_S4x4_MATRIX_ACCESS_I_J(outPose,0,2) = z[0];
  MAL_S4x4_MATRIX_ACCESS_I_J(outPose,1,2) = z[1];
  MAL_S4x4_MATRIX_ACCESS_I_J(outPose,2,2) = z[2];
  MAL_S4x4_MATRIX_ACCESS_I_J(outPose,3,2) = inCenter[2];

  MAL_S4x4_MATRIX_ACCESS_I_J(outPose,0,3) = 0;
  MAL_S4x4_MATRIX_ACCESS_I_J(outPose,1,3) = 0;
  MAL_S4x4_MATRIX_ACCESS_I_J(outPose,2,3) = 0;
  MAL_S4x4_MATRIX_ACCESS_I_J(outPose,3,3) = 1;

  return outPose;
}


void DisplayBody(CjrlBody *aBody)
{
  matrix3d InertiaMatrix = aBody->inertiaMatrix();
  
  cout << "Attached body:" << endl;
  cout << "Mass of the attached body: " << aBody->mass() << endl;
  cout << "Local center of mass: " << aBody->localCenterOfMass() << endl;
  cout << "Inertia matrix:" << endl;
  cout << InertiaMatrix << endl;
  
}
void DisplayJoint(Joint * aJoint)
{
  cout << "Joint : " << aJoint->getName() << endl;
  unsigned int NbCJ = aJoint->countChildJoints();
  if (aJoint->parentJoint()!=0)
    cout << "Father: " << ((Joint *)(aJoint->parentJoint()))->getName() << endl;
  cout << "Number of children : " << NbCJ << endl;
  for(unsigned int li=0;li<NbCJ;li++)
    cout << "Child("<<li<<")=" 
	 << ((Joint*)(aJoint->childJoint(li)))->getName() << endl;
  
  cout << "Current transformation " << aJoint->currentTransformation() << endl;

}

void RecursiveDisplayOfJoints(CjrlJoint *aJoint)
{
  if (aJoint==0)
    return;

  Joint *a2Joint=0;

  int NbChildren = aJoint->countChildJoints();
 
  a2Joint = dynamic_cast<Joint *>( aJoint);
  if (a2Joint==0)
    return;

  DisplayJoint(a2Joint);
  //DisplayBody(aJoint->linkedBody());
  for(int i=0;i<NbChildren;i++)
    {
      // Returns a const so we have to force the casting/
      RecursiveDisplayOfJoints((CjrlJoint *)a2Joint->childJoint(i)); 
    }
  //cout << " End for Joint: " << a2Joint->getName() << endl;
}


void recursiveMultibodyCopy(Joint *initJoint, CjrlJoint *newJoint)
{
  int lNbOfChildren= initJoint->countChildJoints();

  // stop test
  if (lNbOfChildren == 0) return ;

  for(int li=0;li<lNbOfChildren;li++) {
    Joint *Child = dynamic_cast<Joint*> (initJoint->childJoint(li)) ;

    if (Child != 0) {
      int type =  Child->type();
      vector3d axisInLocalFrame = Child->axe();
      vector3d staticTrans; 
      Child->getStaticTranslation(staticTrans);
      matrix3d staticRotation;
      Child->getStaticRotation(staticRotation);
      vector3d axisInGlobalFrame = staticRotation*axisInLocalFrame;
      matrix4d pose=getPoseFromAxisAndCenter(axisInGlobalFrame, staticTrans);
      
      CjrlJoint* a2newJoint=0;

      switch (type) {
      case Joint::REVOLUTE_JOINT:
	a2newJoint = robotDynamicsObjectConstructor.createJointRotation(pose);
	break;
      case Joint::FREE_JOINT:
	a2newJoint = robotDynamicsObjectConstructor.createJointFreeflyer(pose);
	break;
      }
           
      newJoint->addChildJoint(*a2newJoint);
#if 0
      Joint *a2newJointdc = dynamic_cast<Joint*>(a2newJoint);
      // 
      // This function is not in the abstract interface. MAybe a source of bug.
      a2newJointdc->SetFatherJoint(dynamic_cast<Joint*>(newJoint));
#endif
      recursiveMultibodyCopy(Child, a2newJoint) ;
    }
  }
}


void PerformCopyFromJointsTree(HumanoidDynamicMultiBody* aHDR,
			       CjrlHumanoidDynamicRobot* a2HDR)
{
  Joint* InitJoint = dynamic_cast<Joint *>( aHDR->rootJoint()) ;
  
  int type =  InitJoint->type();
  vector3d axisInLocalFrame = InitJoint->axe();
  string name = InitJoint->getName() ;
  vector3d staticTrans; 
  InitJoint->getStaticTranslation(staticTrans);
  matrix3d staticRotation;
  InitJoint->getStaticRotation(staticRotation);
  vector3d axisInGlobalFrame = staticRotation*axisInLocalFrame;
  matrix4d pose=getPoseFromAxisAndCenter(axisInGlobalFrame, staticTrans);
  CjrlJoint* newJoint=0;

  switch (type) {
  case Joint::REVOLUTE_JOINT:
    newJoint = robotDynamicsObjectConstructor.createJointRotation(pose);
    break;
  case Joint::FREE_JOINT:
    newJoint = robotDynamicsObjectConstructor.createJointFreeflyer(pose);
    break;
  }
  
  a2HDR->rootJoint(*newJoint);
  
  cout << " ================== BEGIN COPY BY CONSTRUCTOR  =================== " << endl ;

  recursiveMultibodyCopy(InitJoint, newJoint) ;
 
  cout << " ================== END COPY BY CONSTRUCTOR  =================== " << endl ;
  RecursiveDisplayOfJoints(a2HDR->rootJoint());

  // Initialize the second humanoid from the joint tree.
  std::vector<NameAndRank_t> LinkJointNameAndRank;
  aHDR->getLinksBetweenJointNamesAndRank(LinkJointNameAndRank);
  //  a2HDR->setLinksBetweenJointNamesAndRank(LinkJointNameAndRank);

  a2HDR->initialize();
 
  // Copy the bodies.
  std::vector<CjrlJoint *> VecOfInitJoints = aHDR->jointVector();
  std::vector<CjrlJoint *> VecOfCopyJoints = a2HDR->jointVector();
  
  if (VecOfInitJoints.size()!=VecOfCopyJoints.size())
    {
      std::cout << "Problem while copying the joints. size : " <<VecOfInitJoints.size() << " size copy : " << VecOfCopyJoints.size()  <<endl;
      cout << endl << endl << "There is a probleme the new joints vector is not updated" << endl << endl ;
      exit(-1);
    }

  

  for(unsigned int i=0;i<VecOfInitJoints.size();i++)
    {
      if ((VecOfCopyJoints[i]!=0) &&
	  (VecOfInitJoints[i]!=0))
      *((DynamicBody *)VecOfCopyJoints[i]->linkedBody()) = 
	*((DynamicBody *)VecOfInitJoints[i]->linkedBody());
    }

}

int main(int argc, char *argv[])
{
  int VerboseMode = 3;
  string aPath="/home/stasse/src/OpenHRP/etc/HRP2JRL/";
  string aName="HRP2JRLmain.wrl";
  string pathToWalkGenJRL = "/home/stasse/src/OpenHRP/JRL/src/PatternGeneratorJRL/src/data/";

  int argindex=1;
  if (argc>1)
    {
      aPath=argv[argindex++];
      argc--;
      if (aPath == "--help") {
	std::cout << "TestCopyJointTreeThroughAbstractInterface .../OpenHRP/etc/HRP2JRL/ .../share/walkgenjrl/" << std::endl;
	exit (0);
      }
    }
  if (argc>1)
    {
      pathToWalkGenJRL=argv[argindex++];
      argc--;
    }

  string JointToRank = pathToWalkGenJRL+"HRP2LinkJointRank.xml";
  string aSpecificitiesFileName = pathToWalkGenJRL+"HRP2Specificities.xml";
  // Read the first humanoid.
  HumanoidDynamicMultiBody * aHDR = 
    dynamic_cast<HumanoidDynamicMultiBody*>(robotDynamicsObjectConstructor.createhumanoidDynamicRobot());

  aHDR->parserVRML(aPath,
		   aName,
		   (char *)JointToRank.c_str());
  
  aHDR->SetHumanoidSpecificitiesFile(aSpecificitiesFileName);


  // 
  // The second humanoid is constructed through the abstract interface
  //
  CjrlHumanoidDynamicRobot* a2HDR = robotDynamicsObjectConstructor.createhumanoidDynamicRobot();
  
  PerformCopyFromJointsTree(aHDR, a2HDR);


  // Test the new humanoid structure.
  double dInitPos[40] = { 
    0.0, 0.0, -26.0, 50.0, -24.0, 0.0, 0.0, 0.0, -26.0, 50.0, -24.0, 0.0,  // legs

    0.0, 0.0, 0.0, 0.0, // chest and head

    15.0, -10.0, 0.0, -30.0, 0.0, 0.0, 10.0, // right arm
    15.0,  10.0, 0.0, -30.0, 0.0, 0.0, 10.0, // left arm 

    -20.0, 20.0, -20.0, 20.0, -20.0, // right hand
    -10.0, 10.0, -10.0, 10.0, -10.0  // left hand
  };

  // This is mandatory for this implementation of computeForwardKinematics
  // to compute the derivative of the momentum.
  aHDR->SetTimeStep(0.005);
  aHDR->setComputeAcceleration(false);
  aHDR->setComputeBackwardDynamics(false);
  aHDR->setComputeZMP(true);

#if 0
  // 
  // These functions are not in the abstract interface. Might be a source of bug.
  //
  a2HDR->SetTimeStep(0.005);
  a2HDR->setComputeZMP(true);
  a2HDR->setComputeAcceleration(false);
  a2HDR->setComputeBackwardDynamics(false);
#endif

  int NbOfDofs = a2HDR->numberDof();
  if (VerboseMode>2)
    std::cout << "NbOfDofs :" << NbOfDofs << std::endl;

  MAL_VECTOR_DIM(aCurrentConf,double,NbOfDofs);
  int lindex=0;
  for(int i=0;i<6;i++)
    aCurrentConf[lindex++] = 0.0;
  
  for(int i=0;i<(NbOfDofs-6 < 40 ? NbOfDofs-6 : 40) ;i++)
    aCurrentConf[lindex++] = dInitPos[i]*M_PI/180.0;
  
  aHDR->currentConfiguration(aCurrentConf);
  a2HDR->currentConfiguration(aCurrentConf);

  MAL_VECTOR_DIM(aCurrentVel,double,NbOfDofs); 
  lindex=0;
  for(int i=0;i<NbOfDofs;i++)
    aCurrentVel[lindex++] = 0.0;

  MAL_VECTOR_DIM(aCurrentAcc,double,NbOfDofs);
  MAL_VECTOR_FILL(aCurrentAcc,0.0);
  
  MAL_S3_VECTOR(ZMPval,double);

  for(int i=0;i<4;i++)
    {
      aHDR->currentVelocity(aCurrentVel);
      aHDR->currentAcceleration(aCurrentAcc);
      aHDR->computeForwardKinematics();
      ZMPval = aHDR->zeroMomentumPoint();
      if (VerboseMode>4)
	{
	  cout << i << "-th value of ZMP : " << ZMPval <<endl;
	  cout << "Should be equal to the CoM: " << aHDR->positionCenterOfMass() << endl;
	}

      a2HDR->currentVelocity(aCurrentVel);
      a2HDR->currentAcceleration(aCurrentAcc);
      a2HDR->computeForwardKinematics();
      ZMPval = a2HDR->zeroMomentumPoint();
      if(VerboseMode>4)
	{
	  cout << i << "-th value of ZMP : " << ZMPval <<endl;
	  cout << "Should be equal to the CoM: " << aHDR->positionCenterOfMass() << endl;
	}

    }

  if (VerboseMode>2)
    cout << " ================== AFTER PATGEN  =================== " << endl ;
     RecursiveDisplayOfJoints(a2HDR->rootJoint());
  delete aHDR;
  delete a2HDR;

}
