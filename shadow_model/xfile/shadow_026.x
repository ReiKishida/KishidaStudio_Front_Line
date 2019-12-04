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
 18;
 -1707.12014;-0.00005;1146.24347;,
 -1498.13059;-0.00005;1146.23996;,
 -1463.46614;90.52379;1081.82697;,
 -1672.45569;90.52379;1081.82697;,
 -1672.45549;-0.00005;1081.82697;,
 -1541.67278;-0.00005;1081.82697;,
 -1498.13059;-0.00005;1073.14718;,
 -1336.14279;-0.00005;1073.14718;,
 -1301.47834;90.52379;1008.73058;,
 -1463.46614;90.52379;1008.73058;,
 -1301.47814;-0.00004;1008.73058;,
 -1541.67278;-0.00004;1008.73058;,
 -1463.46614;90.52379;1008.73058;,
 -1541.67278;-0.00005;1081.82697;,
 -1463.46614;90.52379;1081.82697;,
 -1498.13059;-0.00005;1073.14718;,
 -1498.13059;-0.00005;1146.23996;,
 -1541.67278;-0.00004;1008.73058;;
 
 11;
 4;0,1,2,3;,
 4;1,0,4,5;,
 4;6,7,8,9;,
 4;10,7,6,11;,
 4;12,11,13,14;,
 4;15,12,14,16;,
 4;2,5,4,3;,
 4;8,10,17,9;,
 4;5,11,15,1;,
 3;8,7,10;,
 3;4,0,3;;
 
 MeshMaterialList {
  1;
  11;
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
  11;
  0.000007;0.579774;0.814777;,
  -0.000000;-1.000000;-0.000000;,
  -0.000000;-1.000000;-0.000000;,
  0.000000;0.579787;0.814768;,
  0.000000;-1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.756711;0.653749;0.000000;,
  0.933871;-0.357609;-0.000000;,
  0.000000;0.000000;-1.000000;,
  0.880592;0.000002;0.473875;,
  -0.880592;-0.000002;-0.473876;;
  11;
  4;0,0,0,0;,
  4;2,4,4,2;,
  4;3,3,3,3;,
  4;5,5,1,1;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;8,8,8,8;,
  4;2,1,1,2;,
  3;9,9,9;,
  3;10,10,10;;
 }
 MeshTextureCoords {
  18;
  0.990290;1.008560;,
  0.000710;1.008560;,
  0.000710;0.012210;,
  0.990290;0.012210;,
  0.990290;0.012210;,
  0.000710;0.012210;,
  0.000080;0.000080;,
  0.737780;0.000080;,
  0.737780;0.965300;,
  0.000080;0.965300;,
  0.737780;0.965300;,
  0.000050;0.000050;,
  0.000050;0.000050;,
  0.259490;0.000050;,
  0.259490;0.000050;,
  0.000050;0.999950;,
  0.259490;0.999950;,
  0.000080;0.965300;;
 }
}
