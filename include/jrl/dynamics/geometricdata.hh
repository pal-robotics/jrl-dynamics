/*! \file GeometricData.h Geometric data for robots when available.
 *
 * Copyright 2010,
 *
 * Olivier Stasse
 *
 * JRL/LAAS, CNRS/AIST
 *
 * This file is part of dynamicsJRLJapan.
 * dynamicsJRLJapan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dynamicsJRLJapan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Lesser Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with dynamicsJRLJapan.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _DYNAMICS_JRL_JAPAN_GEOMETRIC_DATA_H_
#define _DYNAMICS_JRL_GEOMETRIC_DATA_H_

#include "jrl/dynamics/dll.hh"

#include <vector>
#include <string>
#include <ostream>

#include <jrl/mal/matrixabstractlayer.hh>
#include "abstract-robot-dynamics/humanoid-dynamic-robot.hh"
#include "abstract-robot-dynamics/robot-dynamics-object-constructor.hh"

namespace dynamicsJRLJapan
{
  namespace Geometry
  {
    typedef std::vector<int> polygonIndex;

    struct DYN_JRL_JAPAN_EXPORT IndexedFaceSet
    {
      bool ccw;
      std::vector<int> colorIndex;
      bool colorPerVertex;
      bool convex;
      std::vector<polygonIndex> coordIndex;
      double creaseAngle;
      std::vector<int> normalIndex;
      bool normalPerVertex;
      bool solid;
      std::vector<int> texCoordIndex;
      std::vector<vector3d> coord;

      void reset();

    };

    struct DYN_JRL_JAPAN_EXPORT Material
    {
      double ambientIntensity;
      double diffuseColor[3];
      double emissiveColor[3];
      double shininess;
      double specularColor[3];
      double transparency;

      Material();

      friend std::ostream & operator<<(std::ostream &, const Material &);
    };

    std::ostream &operator<<(std::ostream &,const Material &);

    struct DYN_JRL_JAPAN_EXPORT Texture
    {
      std::string FileName;
    };

    struct DYN_JRL_JAPAN_EXPORT TextureTransform
    {
      double center[2];
      double rotation;
      double scale[2];
      double translation[2];

      TextureTransform();
    };


    class DYN_JRL_JAPAN_EXPORT Appearance
    {
    private:

      Material         m_Material;
      Texture          m_Texture;
      TextureTransform m_TextureTransform;

    public:
      Appearance();
      ~Appearance();
      /*! \name Setter and getters
	@{
       */
      void setMaterial(Material &aMaterial);
      Material &getMaterial();
      const Material &getMaterial() const ;

      void setTexture(Texture &aTexture);
      Texture & getTexture();
      const Texture &getTexture() const;

      void setTextureTransform(TextureTransform &aTextureTransform);
      TextureTransform & getTextureTransform();
      const TextureTransform &getTextureTransform() const;
      /*! @} */
    };

    class DYN_JRL_JAPAN_EXPORT Shape
    {
    private:
      Appearance m_Appearance;
      IndexedFaceSet m_IndexedFaceSet;

    public:
      Shape();
      ~Shape();
       void setAppearance(Appearance &anAppearance);
       const Appearance & getAppearance() const;
       Appearance & getAppearance();

       void setIndexedFaceSet(IndexedFaceSet &anIndexedFaceSet);
       const IndexedFaceSet & getIndexedFaceSet() const ;
       IndexedFaceSet & getIndexedFaceSet();

       void reset();
     };

  } // end of namespace Geometry.

   class DYN_JRL_JAPAN_EXPORT BodyGeometricalData
   {
   private:
     matrix3d m_RotationForDisplay;
     std::vector < std::string > m_URLs;
     std::vector < Geometry::Shape > m_Shapes;
     std::string m_BodyName;
     std::string m_RelatedJointName;

  public:

    BodyGeometricalData();
    ~BodyGeometricalData();

    const matrix3d & getRotationForDisplay() const;
    void setRotationForDisplay(const matrix3d &RotationForDisplay);

    const std::vector< std::string > & getURLs() const;
    void resetURL( );
    void addURL(const std::string & URLtoVRML);

    const std::vector<Geometry::Shape > & getShapes() const;
    void addShape(Geometry::Shape aShape);

    BodyGeometricalData & operator=(const BodyGeometricalData & );

    void setBodyName(const std::string &);
    const std::string & getBodyName() const;
    std::string & getBodyName();

    void setRelatedJointName(const std::string &);
    const std::string & getRelatedJointName() const;
    std::string & getRelatedJointName();
   };

} // end of namespace dynamicsJRLJapan.

#endif /*! _DYNAMICS_JRL_GEOMETRIC_DATA_H_ */
