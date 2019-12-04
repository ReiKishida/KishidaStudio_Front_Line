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
 -1071.48127;-0.00003;576.27257;,
 -1137.22756;-0.00003;647.79894;,
 -770.41427;-0.00003;647.79894;,
 -1071.48127;0.00000;77.29185;,
 -1071.48127;188.12849;77.29186;,
 -1071.48127;188.12847;576.27252;,
 -1071.48127;-0.00003;576.27257;,
 -704.66793;-0.00003;576.27257;,
 -704.66793;188.12847;576.27252;,
 -704.66793;188.12847;576.27252;,
 -1137.22756;-0.00001;148.81824;,
 -1071.48127;188.12847;576.27252;,
 -1071.48127;188.12849;77.29186;;
 
 10;
 3;0,1,2;,
 4;3,4,5,6;,
 4;7,0,5,8;,
 3;9,2,7;,
 3;3,10,4;,
 4;9,11,1,2;,
 4;11,12,10,1;,
 3;10,0,3;,
 3;0,2,7;,
 3;1,0,10;;
 
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
  -0.000000;1.000000;0.000000;,
  0.000000;-0.000000;-1.000000;,
  -0.912579;0.179182;-0.367550;,
  0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.407439;0.196673;0.891803;,
  0.736228;0.000000;0.676734;,
  -0.736228;0.000000;-0.676733;,
  0.000000;0.355381;0.934722;,
  -0.944013;0.329909;0.000000;,
  -0.000000;1.000000;0.000000;,
  -0.000000;1.000000;0.000000;;
  10;
  3;1,4,5;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  3;6,6,7;,
  3;8,3,3;,
  4;6,9,9,6;,
  4;10,3,3,10;,
  3;11,1,12;,
  3;1,5,5;,
  3;4,1,11;;
 }
 MeshTextureCoords {
  13;
  1.000000;1.000000;,
  0.107460;0.495800;,
  0.039280;0.495800;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.039280;0.495800;,
  0.107460;0.463220;,
  0.107460;0.495800;,
  0.107460;0.463220;;
 }
}
