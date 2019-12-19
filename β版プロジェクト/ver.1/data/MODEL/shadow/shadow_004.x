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
 15;
 1269.70559;-0.00000;83.65421;,
 1215.03742;-0.00001;175.38295;,
 1269.70559;171.26232;83.65422;,
 1734.64788;-0.00002;414.55919;,
 1734.64788;171.26231;414.55919;,
 1734.64788;171.26228;789.55455;,
 1734.64788;-0.00003;789.55494;,
 1678.57249;-0.00004;897.08302;,
 1976.42894;-0.00004;897.08302;,
 2032.50452;171.26228;789.55455;,
 1269.70559;171.26231;414.55919;,
 1203.61135;-0.00002;493.24444;,
 1668.55374;-0.00002;493.24444;,
 2032.50452;-0.00003;789.55455;,
 1269.70559;-0.00002;414.55919;;
 
 14;
 3;0,1,2;,
 4;3,4,5,6;,
 4;5,7,8,9;,
 4;10,11,12,4;,
 4;4,12,7,5;,
 4;2,1,11,10;,
 4;9,13,6,5;,
 4;7,6,13,8;,
 4;10,4,3,14;,
 3;9,8,13;,
 4;2,10,14,0;,
 4;3,6,7,12;,
 4;14,3,12,11;,
 4;1,0,14,11;;
 
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
  15;
  -0.945592;0.170485;-0.277112;,
  0.000000;0.417488;0.908683;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.531739;0.846909;,
  -0.000001;-0.000001;-1.000000;,
  -0.859014;0.000000;-0.511952;,
  -0.939523;0.342284;0.011733;,
  -0.945448;0.325335;-0.016857;,
  0.000000;-1.000000;-0.000000;,
  -0.000000;-1.000000;-0.000000;,
  0.886674;0.000000;0.462396;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;;
  14;
  3;6,0,0;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;1,1,1,1;,
  4;7,7,7,7;,
  4;0,0,8,8;,
  4;5,5,5,5;,
  4;9,9,10,10;,
  4;3,3,3,3;,
  3;11,11,11;,
  4;2,2,2,2;,
  4;12,9,9,12;,
  4;13,12,12,13;,
  4;14,14,13,13;;
 }
 MeshTextureCoords {
  15;
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
