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
 -1.23610;0.78787;2.03858;,
 -2.58087;-1.42711;0.91740;,
 -0.31171;-1.45140;-1.75631;,
 1.03307;0.76358;-0.63513;,
 -5.33047;3.82706;-6.06367;,
 -6.00114;3.67373;-6.63148;,
 -5.89416;6.48972;-6.56626;,
 -4.60090;6.93782;-5.47275;,
 -3.21044;2.16978;-4.24935;,
 -4.65980;3.98038;-5.49587;,
 -3.31503;6.19535;-4.37469;,
 -1.86159;5.04085;-3.13067;,
 -4.13076;5.06514;-0.45696;,
 -5.58419;6.21964;-1.70098;,
 -6.92897;4.00467;-2.82216;,
 -5.47960;2.19407;-1.57564;,
 -8.16333;6.51401;-3.89255;,
 -8.27031;3.69803;-3.95776;,
 -6.87007;6.96212;-2.79904;,
 -7.59964;3.85135;-3.38996;,
 -0.41630;2.57417;-1.88165;,
 -2.68546;2.59846;0.79206;,
 -4.03024;0.38348;-0.32912;,
 -1.76107;0.35919;-3.00283;;
 
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
  1;
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
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  26;
  0.595367;-0.620081;0.510919;,
  0.647056;-0.006927;-0.762411;,
  -0.647055;0.006927;0.762412;,
  0.647057;-0.006925;-0.762410;,
  -0.761498;0.043906;-0.646679;,
  0.647056;-0.006926;-0.762411;,
  -0.647054;0.006927;0.762413;,
  0.568235;0.671102;0.476162;,
  0.647056;-0.006926;-0.762411;,
  -0.527553;-0.726007;-0.441136;,
  -0.647056;0.006927;0.762411;,
  -0.527554;-0.726007;-0.441137;,
  -0.224331;-0.957426;-0.181690;,
  0.510328;0.746867;0.426328;,
  0.354949;0.887726;0.293178;,
  -0.647054;0.006926;0.762413;,
  0.126635;-0.985093;0.116425;,
  0.126637;-0.985093;0.116427;,
  0.063534;0.996971;0.044862;,
  -0.190689;0.966709;-0.170621;,
  0.527553;0.726007;0.441136;,
  -0.527553;-0.726007;-0.441136;,
  -0.647055;0.006927;0.762412;,
  -0.647055;0.006927;0.762412;,
  0.647055;-0.006927;-0.762412;,
  0.647055;-0.006926;-0.762412;;
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
