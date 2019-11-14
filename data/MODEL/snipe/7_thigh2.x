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
 16;
 -0.27390;1.43082;1.73077;,
 -0.27390;-1.37007;1.73077;,
 1.76638;-1.37007;-0.83151;,
 1.76638;1.43082;-0.83151;,
 -3.86231;-1.37007;-0.26911;,
 -4.04751;-1.54152;-0.41658;,
 -1.17151;-1.54152;-4.02838;,
 -0.98631;-1.37007;-3.88091;,
 -0.98631;1.43082;-3.88091;,
 -3.86231;1.43082;-0.26910;,
 -2.72206;-4.77832;-5.26305;,
 -4.48917;-3.18881;-6.67017;,
 -2.51463;0.64310;-5.09788;,
 -5.39063;0.64310;-1.48608;,
 -7.36517;-3.18881;-3.05836;,
 -5.59806;-4.77832;-1.65125;;
 
 14;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;2,7,8,3;,
 4;0,9,4,1;,
 4;1,4,7,2;,
 4;3,8,9,0;,
 4;6,10,11,12;,
 4;8,12,13,9;,
 4;7,6,12,8;,
 4;9,13,5,4;,
 4;14,11,10,15;,
 4;13,14,15,5;,
 4;5,15,10,6;,
 4;12,11,14,13;;
 
 MeshMaterialList {
  2;
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
   0.640000;0.640000;0.640000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  18;
  0.782288;0.000000;0.622917;,
  0.684909;-0.000000;-0.728628;,
  -0.556739;-0.000001;0.830687;,
  0.241171;-0.951293;0.192039;,
  0.622918;-0.000000;-0.782287;,
  -0.495415;0.773914;-0.394488;,
  0.574131;-0.679243;0.457168;,
  0.622920;-0.000000;-0.782286;,
  -0.450184;-0.817822;-0.358472;,
  0.742297;0.000000;-0.670071;,
  -0.486817;-0.000002;0.873504;,
  0.000000;-1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.148995;0.981695;-0.118642;,
  -0.622919;-0.000000;0.782287;,
  -0.622918;0.000000;0.782287;,
  0.667141;-0.522223;0.531231;,
  -0.653296;0.550083;-0.520205;;
  14;
  4;0,0,0,0;,
  4;3,6,6,3;,
  4;9,1,1,9;,
  4;10,2,2,10;,
  4;11,3,3,11;,
  4;12,13,13,12;,
  4;4,7,7,4;,
  4;13,5,5,13;,
  4;1,4,4,1;,
  4;2,14,14,2;,
  4;8,8,8,8;,
  4;14,15,15,14;,
  4;6,16,16,6;,
  4;5,17,17,5;;
 }
 MeshTextureCoords {
  16;
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
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}