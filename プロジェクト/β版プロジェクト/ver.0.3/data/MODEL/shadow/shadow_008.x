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
 591.20645;-74.28009;-1030.45031;,
 490.62559;-99.71448;-938.72716;,
 1153.59123;-99.71448;-938.72716;,
 591.20645;-74.28009;-1030.45031;,
 591.20641;-74.28008;-1308.53654;,
 591.20641;183.02230;-1308.53654;,
 1254.17199;183.02229;-1030.45021;,
 1254.17199;-74.28009;-1030.45031;,
 490.62559;-94.99022;-1216.81349;,
 1254.17199;183.02229;-1030.45021;,
 591.20645;183.02229;-1030.45021;,
 591.20641;183.02230;-1308.53654;,
 591.20645;183.02229;-1030.45021;;
 
 14;
 3;0,1,2;,
 3;3,4,5;,
 3;6,7,0;,
 3;4,8,5;,
 3;7,6,2;,
 3;2,9,10;,
 3;10,11,8;,
 3;8,0,4;,
 3;0,2,7;,
 3;1,0,8;,
 3;3,5,12;,
 3;6,0,12;,
 3;2,10,1;,
 3;10,8,1;;
 
 MeshMaterialList {
  1;
  14;
  0,
  0,
  0,
  0,
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
  19;
  1.000000;0.000000;-0.000000;,
  -0.109735;0.984087;0.139754;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;-0.000000;,
  0.000000;0.308580;0.951199;,
  -0.931506;0.246755;-0.267224;,
  -0.117182;0.982618;0.143977;,
  0.000000;0.722214;0.691669;,
  -0.673822;0.000000;-0.738894;,
  -0.893014;0.188213;-0.408780;,
  0.673823;-0.000000;0.738893;,
  0.365127;0.167211;0.915818;,
  -0.940959;0.338507;0.002861;,
  -0.216335;0.976284;0.008265;,
  -0.201675;0.979453;0.000000;,
  0.000000;0.963638;0.267212;,
  1.000000;0.000000;-0.000000;,
  0.000000;0.308580;0.951199;,
  -0.941557;0.336806;0.005722;;
  14;
  3;1,6,7;,
  3;3,0,3;,
  3;2,2,2;,
  3;8,5,9;,
  3;10,11,10;,
  3;7,11,4;,
  3;12,9,5;,
  3;13,1,14;,
  3;1,7,15;,
  3;6,1,13;,
  3;3,3,16;,
  3;2,2,2;,
  3;7,4,17;,
  3;12,5,18;;
 }
 MeshTextureCoords {
  13;
  1.000000;1.000000;,
  0.170000;1.000000;,
  0.010000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.170000;0.000000;,
  0.010000;1.000000;,
  0.170000;1.000000;,
  0.170000;0.000000;,
  1.000000;0.000000;;
 }
}
