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
 141.48966;-42.22802;-152.07642;,
 340.87552;-41.37896;-49.53243;,
 58.49816;-41.37896;-49.53242;,
 141.48965;-42.22800;-569.14791;,
 141.48965;228.56675;-569.14791;,
 141.48966;228.56673;-152.07641;,
 141.48966;-42.22802;-152.07642;,
 423.86700;-42.22802;-152.07643;,
 423.86700;228.56673;-152.07642;,
 423.86700;228.56673;-152.07642;,
 58.49816;-41.37894;-466.60391;,
 141.48966;228.56673;-152.07641;,
 141.48965;228.56675;-569.14791;;
 
 10;
 3;0,1,2;,
 4;3,4,5,6;,
 4;7,0,5,8;,
 3;9,1,7;,
 3;3,10,4;,
 4;9,11,2,1;,
 4;11,12,10,2;,
 3;10,3,0;,
 3;0,7,1;,
 3;2,10,0;;
 
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
  12;
  1.000000;0.000000;-0.000000;,
  -0.005115;-0.999978;0.004140;,
  -0.000000;0.000000;-1.000000;,
  -0.928276;0.157392;-0.336945;,
  0.000000;-0.999966;0.008280;,
  0.436160;0.199255;0.877532;,
  0.777320;-0.000000;0.629105;,
  -0.777320;0.000000;-0.629105;,
  0.000000;0.355111;0.934824;,
  -0.955847;0.293864;0.000000;,
  -0.010230;-0.999948;-0.000000;,
  -0.010230;-0.999948;-0.000000;;
  10;
  3;1,4,1;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  3;5,5,6;,
  3;7,3,3;,
  4;5,8,8,5;,
  4;9,3,3,9;,
  3;10,11,1;,
  3;1,4,4;,
  3;1,10,1;;
 }
 MeshTextureCoords {
  13;
  1.000000;1.000000;,
  0.010000;1.000000;,
  0.170000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.010000;1.000000;,
  0.170000;0.000000;,
  0.170000;1.000000;,
  0.170000;0.000000;;
 }
}
