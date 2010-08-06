/*
 *   Copyright (c) 2010 CNRS-AIST 
 *
 *   Research carried out within the scope of the Associated
 *   International Laboratory: Joint Japanese-French Robotics
 *   Laboratory (JRL)
 *
 *   Author: Olivier Stasse
 *
 *   Please refers to file License.txt for details on the license.
 *
 */
#include <dynamicsJRLJapan/GeometricData.h>

namespace dynamicsJRLJapan
{
  namespace Geometry
  {

    /*! Material object. */
    Material::Material()
    {
      ambientIntensity = 0.2;
      diffuseColor[0] = 0.8;
      diffuseColor[1] = 0.8;
      diffuseColor[2] = 0.8;
      emissiveColor[0] = 0.0;
      emissiveColor[1] = 0.0;
      emissiveColor[2] = 0.0;
      shininess = 0.2;
      specularColor[0] =
	specularColor[1] = 
	specularColor[2] = 0.0;
      transparency=0.0;
    }
    /*! Texture Transform */
    TextureTransform::TextureTransform()
    {
      center[0] = center[1] = center[2] = 0.0;
      rotation = 0.0;
      scale[0] = scale[1] = 1.0;
      translation[0] = 0.0;
      translation[1] = 0.0;
    }
    
    /*! Appearance object */
    Appearance::Appearance()
    {}
    Appearance::~Appearance()
    {}

    void Appearance::setMaterial(Material &aMaterial)
    {
      m_Material = aMaterial;
    }
    
    Material & Appearance::getMaterial() 
    {
      return m_Material;
    }

    void Appearance::setTexture(Texture &aTexture)
    {
      m_Texture = aTexture;
    }
    
    Texture & Appearance::getTexture() 
    {
      return m_Texture;
    }
    
    void Appearance::setTextureTransform(TextureTransform &aTextureTransform)
    {
      m_TextureTransform = aTextureTransform;
    }
    
    TextureTransform & Appearance::getTextureTransform()
    {
      return m_TextureTransform;
    }

    /*! Shape object.*/
    Shape::Shape()
    {}

    Shape::~Shape()
    {}
    
    void Shape::setAppearance(Appearance &anAppearance)
    {
      m_Appearance = anAppearance;
    }

    Appearance & Shape::getAppearance()
    {
      return m_Appearance;
    }

    void Shape::setIndexedFaceSet(IndexedFaceSet &anIndexedFaceSet)
    {
      m_IndexedFaceSet = anIndexedFaceSet;
    }

    IndexedFaceSet & Shape::getIndexedFaceSet()
    {
      return m_IndexedFaceSet;
    }
    

  };

  const matrix3d & BodyGeometricalData::getRotationForDisplay()
  {
    return m_RotationForDisplay;
  }

  BodyGeometricalData::BodyGeometricalData():
    m_RotationForDisplay(1,0,0, 0,1,0, 0,0,1),
    m_URLs(0),
    m_Shapes(0)
  {}

  void BodyGeometricalData::setRotationForDisplay(const matrix3d & RotationForDisplay)
  {
    m_RotationForDisplay = RotationForDisplay;
  }

  const std::vector< std::string > & BodyGeometricalData::getURLs()
  {
    return m_URLs;
  }

  void BodyGeometricalData::resetURL( )
  {
    m_URLs.clear();
  }

  void BodyGeometricalData::addURL(const std::string & URLtoVRML)
  {
    m_URLs.push_back(URLtoVRML);
  }

  void BodyGeometricalData::addShape(Geometry::Shape &aShape)
  {
    m_Shapes.push_back(aShape);
  }
};