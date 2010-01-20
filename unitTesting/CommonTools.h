/* @doc \file Common Tools for unitary testing

   Copyright (c) 2010
   @author Olivier Stasse
   JRL-Japan, CNRS/AIST
 
   All rights reserved.

   Please refers to file License.txt for details on the license.

*/

#ifndef _DYNAMIC_JRL_JAPAN_COMMON_TOOLS_H_
#define _DYNAMIC_JRL_JAPAN_COMMON_TOOLS_H_

#include <string>
#include <iostream>
#include <dynamicsJRLJapan/dynamicsJRLJapanFactory.h>

namespace dynamicsJRLJapan
{
  /*! \name Display related functions. 
     @{
   */
  
  /*! \name Display mathematical objects
    @{ */
  /*! \brief Display horizontally in os a 3d vector. */
  void dvd3d(vector3d &av3d, std::ostream &os);
  
  /*! \brief Display a 3x3 matrix in os with a shift. */
  void dm3d(const matrix3d &todisplay, 
	    std::ostream &os,
	    std::string shifttab);

  /*! \brief Display a 4x4 matrix in os with a shift. */
  void dm4d(const matrix4d &todisplay, 
	    std::ostream &os,
	    std::string shifttab);

  /*! \brief Display a 4x4 matrix in os as a vector. */
  void dm4dv(const matrix4d &todisplay, 
	    std::ostream &os);

  /*! @} */

  /*! \name Display robotic objects
    @{ */

  /*! Display a body of the robot */
  void DisplayBody(CjrlBody *aBody,
		   std::string &shifttab,
		   std::ostream &tcout);

  /*! Display a hand of the robot */
  void DisplayHand(CjrlHand *ajrlHand,
		   std::string &shifttab,
		   std::ostream &tcout);

  /*! Display a foot of the robot */
  void DisplayFoot(CjrlFoot *aFoot,
		   std::string &shifttab,
		   std::ostream &tcout);

  /*! Display actuated */
  void DisplayActuated(CjrlHumanoidDynamicRobot *aHDR,
		       std::string &shifttab,
		       std::ostream &tcout);

  /*! Display full humanoid information */
  void DisplayHumanoid(CjrlHumanoidDynamicRobot *aHDR,
		       std::ostream &tcout);

  /*! Display all the joints of a kinematic subtree */
  void RecursiveDisplayOfJoints(CjrlJoint *aJoint, 
				std::ostream &tcout,
				unsigned int verbosedisplay=0,
				unsigned int ldepth=0);
  
  /*! @} */

  /*! Compare two files for testing */
  bool CompareTwoFiles(char *RefFileName,
		       char *OurFileName);
  /*! @} */
};
#endif