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
 656.59732;223.66978;-770.32794;,
 656.59732;223.66975;-138.25882;,
 695.22926;0.00001;-224.93250;,
 695.22926;0.00003;-767.88199;,
 543.02681;0.00000;-88.14863;,
 1082.61779;0.00000;-88.14863;,
 1099.86799;223.66975;-138.25882;,
 1099.86799;223.66978;-595.97398;,
 1101.27209;0.00002;-499.39273;,
 1414.00754;0.00002;-499.39273;,
 1414.00754;223.66978;-595.97398;,
 1076.00021;0.00001;-224.93250;,
 1076.00021;0.00003;-618.11143;,
 1345.84729;0.00003;-618.11143;,
 543.02681;0.00003;-720.21774;;
 
 14;
 4;0,1,2,3;,
 4;1,4,5,6;,
 4;7,8,9,10;,
 4;7,6,5,8;,
 4;6,11,2,1;,
 4;7,12,11,6;,
 4;10,13,12,7;,
 3;0,3,14;,
 3;10,9,13;,
 4;5,11,12,8;,
 4;8,12,13,9;,
 4;4,2,11,5;,
 4;14,3,2,4;,
 4;0,14,4,1;;
 
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
  0.985410;0.170199;0.000000;,
  0.000000;0.396424;0.918067;,
  0.000000;0.218617;0.975811;,
  -0.298278;-0.061934;-0.952468;,
  0.000000;0.098493;-0.995138;,
  -0.994355;0.106107;0.000000;,
  0.000000;0.361327;-0.932439;,
  0.999286;-0.030261;0.022627;,
  0.847857;-0.210194;-0.486782;,
  -0.000000;-1.000000;-0.000000;,
  -0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.891642;0.452740;0.000000;;
  14;
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;7,7,7,7;,
  4;6,6,6,6;,
  4;5,5,5,5;,
  4;4,4,4,4;,
  3;3,3,3;,
  3;8,8,8;,
  4;9,9,10,10;,
  4;10,10,11,11;,
  4;12,12,9,9;,
  4;13,13,12,12;,
  4;14,14,14,14;;
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
