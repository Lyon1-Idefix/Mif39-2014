#include "Assets/Material.hpp"

namespace Assets {

/*
typedef enum {
Opaque,
CutOut,
Fade,
Transparent
} MaterialRenderingMode;

typedef enum {
UV0,
UV1
} MaterialUVSet;

typedef enum {
None,
Realtime,
Baked
} MaterialGlobalIllumination;

typedef enum {
Color,                  // 255, 255, 255
SpecColor,              //  51,  51,  51
EmissionColor,          //   0,   0,   0
EmissionColorUI         // 255, 255, 255
} MaterialColor;

typedef enum {
MainTex,
BumpMap,
DetailNormalMap,
ParallaxMap,
OcclusionMap,
EmissionMap,
DetailMask,
DetailAlbedoMap,
MetallicGlossMap,
SpecGlossMap
} MaterialTexture;

typedef enum {
CutOff,                 // 0.500000 : for Unlit/Transparent Cutout Shader
Shininess,              // 0.078125 : for Mobile/Bumped Specular Shader
SourceBlend,            // 1.000000 :
DestinationBlend,       // 0.000000 :
Parallax,               // 0.020000 : [0.005;0.08]
ZWrite,                 // 1.000000 :
Glossiness,             // 0.500000 : [0;1]
BumpScale,              // 1.000000 : [-inf;inf]
OcclusionStrength,      // 1.000000 : [0;1]
DetailNormalMapScale,   // 1.000000 : [-inf;inf]
UVSec,                  //        0 : set {0;1} : MaterialUVSet
EmissionScaleUI,        // 0.000000 : [0;inf]
Mode,                   //        0 : set {0;1;2;3} : MaterialRenderingMode
Metallic                // 0.000000 : [0;1]
} MaterialFloat;
*/

/*
* Shader: Standard
*      Main Maps:
*          Albedo      : Tex:MainTex Color:Color
*          Metallic    : Float:Metallic Float:Glossiness | Tex:MetallicGlossMap
*          Normal Map  : Tex:BumpMap Float:BumpScale
*          Height Map  : Tex:ParallaxMap Float:Parallax
*          Occlusion   : Tex:OcclusionMap Float:OcclusionStrength
*          Emission    : Tex:EmissionMap Float: Color:EmissionColor=EmissionColorUI Float:EmissionScaleUI
*          Detail Mask : Tex:DetailMask
*          Tiling : Vec2
*          Offset : Vec2
*      Secondary Maps:
*          Detailled Albedo : Tex:DetailAlbedoMap
*          Normal Map       : Tex:DetailNormalMap Float:DetailNormalMapScale
*          Tiling : Vec2
*          Offset : Vec2
*          UV Set : { UV0 UV1 }:UVSec
*      Lightmap Flags : set {0;1;2} : MaterialGlobalIllumination
* Shader: Standard (Specular setup)
*      Main Maps:
*          Albedo      : Tex:MainTex Color:Color
*          Specular    : Color:SpecColor Float:Glossiness | Tex:SpecGlossMap
*          Normal Map  : Tex:BumpMap Float:BumpScale
*          Height Map  : Tex:ParallaxMap Float:Parallax
*          Occlusion   : Tex:OcclusionMap Float:OcclusionStrength
*          Emission    : Tex:EmissionMap Float: Color:EmissionColor=EmissionColorUI Float:EmissionScaleUI
*          Detail Mask : Tex:DetailMask
*          Tiling : Vec2
*          Offset : Vec2
*      Secondary Maps:
*          Detailled Albedo : Tex:DetailAlbedoMap
*          Normal Map       : Tex:DetailNormalMap Float:DetailNormalMapScale
*          Tiling : Vec2
*          Offset : Vec2
*          UV Set : { UV0 UV1 }:UVSec
*      Lightmap Flags : set {0;1;2} : MaterialGlobalIllumination
*/

Material::Material (QString name)
{
    HasName::Initialize ( name );
}

Material::~Material ()
{
}

MaterialLoader::MaterialLoader () {
}

MaterialLoader::~MaterialLoader () {
}

MaterialPtr MaterialManager::getByUUID ( QUuid uuid )
{
    return SharedResourceManager < Material >::getByUUID(uuid).dynamicCast < Material > ();
}

MaterialManager::MaterialManager() :
    ISharedResourceManager ( "material/" ), SharedResourceManager < Material > ( "material/" )
{
    HasUUID::InitializeUUID ( "material/" );
}

}
