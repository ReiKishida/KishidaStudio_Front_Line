xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 13;
 135.49489;-0.00001;83.54262;,
 135.49489;324.71988;83.54263;,
 135.49489;324.71985;572.63482;,
 135.49489;-0.00003;572.63482;,
 502.30821;-0.00003;572.63482;,
 135.49489;-0.00003;572.63482;,
 502.30821;324.71985;572.63482;,
 502.30821;324.71985;572.63482;,
 135.49489;324.71985;572.63482;,
 34.76362;-0.00003;719.37560;,
 401.57693;-0.00003;719.37560;,
 135.49489;324.71988;83.54263;,
 34.76362;-0.00001;230.28335;;
 
 10;
 4;0,1,2,3;,
 4;4,5,2,6;,
 4;7,8,9,10;,
 4;8,11,12,9;,
 3;0,12,11;,
 3;7,10,4;,
 3;5,9,12;,
 3;5,12,0;,
 3;9,5,10;,
 3;10,5,4;;
 
 MeshMaterialList {
  1;
  10;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.020800;0.640000;0.000000;1.000000;;
   0.000000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  13;
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  -0.941195;0.156702;-0.299327;,
  0.000000;0.411804;0.911273;,
  0.473516;0.236518;0.848435;,
  -0.955101;0.296282;0.000000;,
  -0.824443;0.000000;-0.565945;,
  0.824443;0.000000;0.565945;,
  -0.000000;-1.000000;-0.000000;,
  -0.000000;-1.000000;-0.000000;,
  -0.000000;-1.000000;-0.000000;,
  -0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;0.000000;;
  10;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;4,3,3,4;,
  4;5,2,2,5;,
  3;6,2,2;,
  3;4,4,7;,
  3;8,9,10;,
  3;8,10,11;,
  3;9,8,12;,
  3;12,8,12;;
 }
 MeshTextureCoords {
  13;
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.039280;0.495800;,
  0.107460;0.495800;,
  0.107460;0.495800;,
  0.039280;0.495800;,
  0.107460;0.463220;,
  0.107460;0.463220;;
 }
}
