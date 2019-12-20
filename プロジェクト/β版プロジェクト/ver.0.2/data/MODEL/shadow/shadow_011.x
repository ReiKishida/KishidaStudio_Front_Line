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
 141.48965;0.00001;-152.07637;,
 58.49817;0.00000;-49.53238;,
 340.87552;0.00000;-49.53238;,
 141.48965;0.00001;-152.07637;,
 141.48965;0.00002;-569.14791;,
 141.48965;228.56675;-569.14786;,
 423.86700;228.56673;-152.07636;,
 423.86700;0.00001;-152.07637;,
 423.86700;228.56673;-152.07636;,
 58.49817;0.00002;-466.60386;,
 141.48965;228.56673;-152.07636;,
 141.48965;228.56675;-569.14786;,
 141.48965;228.56673;-152.07636;;
 
 14;
 3;0,1,2;,
 3;3,4,5;,
 3;6,7,0;,
 3;8,2,7;,
 3;4,9,5;,
 3;2,8,10;,
 3;10,11,9;,
 3;9,0,4;,
 3;0,2,7;,
 3;1,0,9;,
 3;3,5,12;,
 3;6,0,12;,
 3;2,10,1;,
 3;10,9,1;;
 
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
  16;
  1.000000;0.000000;0.000000;,
  -0.000000;1.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.409332;0.912386;,
  -0.944053;0.242507;-0.223506;,
  -0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.438111;0.230707;0.868811;,
  0.287779;0.303085;0.908473;,
  0.777320;-0.000000;0.629105;,
  -0.777320;0.000000;-0.629105;,
  -0.923041;0.183446;-0.338146;,
  -0.939957;0.341294;0.000000;,
  -0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.939957;0.341294;0.000000;;
  14;
  3;1,5,6;,
  3;0,0,0;,
  3;2,2,2;,
  3;7,8,9;,
  3;10,4,11;,
  3;8,7,3;,
  3;12,11,4;,
  3;13,1,14;,
  3;1,6,6;,
  3;5,1,13;,
  3;0,0,0;,
  3;2,2,2;,
  3;8,3,3;,
  3;12,4,15;;
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
  0.010000;1.000000;,
  0.170000;0.000000;,
  0.170000;1.000000;,
  0.170000;0.000000;,
  1.000000;0.000000;;
 }
}
