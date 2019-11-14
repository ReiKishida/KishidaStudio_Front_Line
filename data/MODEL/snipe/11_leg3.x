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
 24;
 1.12715;1.02728;0.90439;,
 -0.23647;-1.18771;2.00256;,
 -2.46058;-1.16271;-0.70874;,
 -1.09696;1.05227;-1.80691;,
 -7.55006;4.11569;3.51489;,
 -8.23015;3.96237;4.07137;,
 -8.12176;6.77834;4.00841;,
 -6.81032;7.22646;2.93675;,
 -5.40017;2.45844;1.73603;,
 -6.86996;4.26902;2.95841;,
 -5.50635;6.48399;1.86024;,
 -4.03244;5.32952;0.64053;,
 -1.80833;5.30452;3.35183;,
 -3.28224;6.45899;4.57154;,
 -4.64585;4.24402;5.66971;,
 -3.17606;2.43345;4.44733;,
 -5.89765;6.75334;6.71969;,
 -6.00604;3.93737;6.78265;,
 -4.58621;7.20146;5.64805;,
 -5.32595;4.09070;6.22618;,
 -2.56675;2.86285;-0.58453;,
 -0.34264;2.83785;2.12677;,
 -1.70626;0.62287;3.22494;,
 -3.93037;0.64787;0.51364;;
 
 22;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;15,14,9,8;,
 4;11,10,13,12;,
 4;16,6,5,17;,
 4;18,16,17,19;,
 4;19,17,5,4;,
 4;7,6,16,18;,
 4;10,7,18,13;,
 4;14,19,4,9;,
 4;13,18,19,14;,
 4;9,4,7,10;,
 4;3,20,21,0;,
 4;20,11,12,21;,
 4;1,22,23,2;,
 4;22,15,8,23;,
 4;0,21,22,1;,
 4;21,12,15,22;,
 4;2,23,20,3;,
 4;23,8,11,20;;
 
 MeshMaterialList {
  2;
  22;
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
  26;
  0.603756;-0.620076;-0.500984;,
  -0.634208;0.007127;-0.773130;,
  0.634206;-0.007125;0.773131;,
  -0.634211;0.007127;-0.773127;,
  -0.772204;0.043895;0.633857;,
  -0.634209;0.007127;-0.773129;,
  0.634203;-0.007125;0.773133;,
  0.576206;0.671105;-0.466481;,
  -0.634208;0.007127;-0.773130;,
  -0.534950;-0.726010;0.432132;,
  0.634208;-0.007127;0.773130;,
  -0.534949;-0.726011;0.432132;,
  -0.227457;-0.957426;0.177759;,
  0.517480;0.746872;-0.417608;,
  0.359913;0.887729;-0.287055;,
  0.634203;-0.007127;0.773133;,
  0.128443;-0.985091;-0.114445;,
  0.128443;-0.985091;-0.114446;,
  0.064399;0.996970;-0.043634;,
  -0.193400;0.966705;0.167562;,
  0.534950;0.726009;-0.432133;,
  -0.534952;-0.726009;0.432132;,
  0.634207;-0.007129;0.773130;,
  0.634208;-0.007128;0.773130;,
  -0.634207;0.007129;-0.773130;,
  -0.634208;0.007128;-0.773130;;
  22;
  4;0,0,0,0;,
  4;5,3,3,5;,
  4;8,1,1,8;,
  4;10,2,2,10;,
  4;11,12,12,11;,
  4;13,14,14,13;,
  4;4,4,4,4;,
  4;6,15,15,6;,
  4;16,17,17,16;,
  4;18,19,19,18;,
  4;14,18,18,14;,
  4;12,16,16,12;,
  4;2,6,6,2;,
  4;1,5,5,1;,
  4;20,7,7,20;,
  4;7,13,13,7;,
  4;21,9,9,21;,
  4;9,11,11,9;,
  4;22,23,23,22;,
  4;23,10,10,23;,
  4;24,25,25,24;,
  4;25,8,8,25;;
 }
 MeshTextureCoords {
  24;
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