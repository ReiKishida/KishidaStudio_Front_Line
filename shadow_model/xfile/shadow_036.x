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
 32;
 -2082.37191;0.00001;-158.85869;,
 -2121.92718;0.00001;-167.55111;,
 -2121.92718;159.70421;-167.55110;,
 -2082.37191;159.70424;-158.85868;,
 -2154.76136;0.00001;-183.52267;,
 -2177.37601;0.00001;-201.53922;,
 -2177.37601;159.70421;-201.53922;,
 -2154.76136;159.70421;-183.52266;,
 -2188.86839;0.00001;-273.56206;,
 -2188.86839;159.70424;-273.56206;,
 -2188.86839;159.70424;-238.11038;,
 -2188.86839;0.00001;-238.11038;,
 -1840.10100;159.70424;-158.85867;,
 -1920.26956;0.00000;-87.85864;,
 -1840.10100;0.00001;-158.85868;,
 -2082.37191;159.70424;-158.85868;,
 -2177.37601;0.00001;-310.13321;,
 -2177.37601;159.70421;-310.13321;,
 -2269.03695;0.00001;-202.56201;,
 -2269.03695;0.00001;-167.11033;,
 -2121.92718;159.70421;-167.55110;,
 -2154.76136;159.70421;-183.52266;,
 -2234.92992;0.00000;-112.52261;,
 -2202.09574;0.00000;-96.55109;,
 -2082.37191;159.70424;-158.85868;,
 -2162.54046;0.00000;-87.85859;,
 -2177.37601;159.70421;-201.53922;,
 -2257.54456;0.00001;-130.53919;,
 -2257.54456;0.00001;-239.13316;,
 -2188.86839;159.70424;-238.11038;,
 -2269.03695;0.00001;-167.11033;,
 -2162.54046;0.00000;-87.85859;;
 
 30;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;5,11,10,6;,
 3;12,13,14;,
 4;1,4,7,2;,
 4;0,15,12,14;,
 4;16,17,9,8;,
 4;9,18,19,10;,
 4;20,21,22,23;,
 4;24,20,23,25;,
 4;21,26,27,22;,
 4;17,28,18,9;,
 3;17,16,28;,
 4;26,29,30,27;,
 4;15,31,13,12;,
 3;11,5,18;,
 3;13,0,14;,
 3;13,31,0;,
 3;0,31,1;,
 3;23,4,31;,
 3;27,5,22;,
 3;22,4,23;,
 3;5,4,22;,
 3;18,5,30;,
 3;30,5,27;,
 3;16,8,28;,
 3;28,11,18;,
 3;8,11,28;,
 3;31,4,1;;
 
 MeshMaterialList {
  1;
  30;
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
  30;
  0.663002;-0.000000;0.748618;,
  0.107947;0.000000;-0.994157;,
  0.361261;0.221352;0.905811;,
  0.328320;0.000000;-0.944567;,
  0.533491;0.000000;-0.845806;,
  0.824612;0.000000;-0.565699;,
  0.954004;0.000000;0.299793;,
  0.988434;0.000000;-0.151651;,
  0.988434;0.000000;0.151651;,
  -0.094870;0.443781;0.891100;,
  -0.281876;0.504354;0.816194;,
  -0.447869;0.541218;0.711686;,
  -0.688170;0.553828;0.468718;,
  -0.822390;0.171689;-0.542400;,
  -0.862042;0.490121;0.129095;,
  -0.908575;0.392340;-0.143391;,
  0.000000;0.000000;-1.000000;,
  -0.663002;0.000000;-0.748618;,
  0.000000;-1.000000;-0.000000;,
  -0.000000;-1.000000;-0.000000;,
  -0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.000000;-1.000000;-0.000000;,
  -0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;;
  30;
  4;1,3,3,1;,
  4;4,5,5,4;,
  4;8,8,7,7;,
  4;5,7,7,5;,
  3;2,2,0;,
  4;3,4,4,3;,
  4;1,1,16,16;,
  4;6,6,8,8;,
  4;15,15,14,14;,
  4;10,11,11,10;,
  4;9,10,10,9;,
  4;11,12,12,11;,
  4;13,13,15,15;,
  3;13,17,13;,
  4;12,14,14,12;,
  4;9,9,2,2;,
  3;18,19,18;,
  3;20,21,22;,
  3;20,23,21;,
  3;21,23,24;,
  3;25,26,23;,
  3;27,19,28;,
  3;28,26,25;,
  3;19,26,28;,
  3;18,19,18;,
  3;18,19,27;,
  3;29,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;23,26,24;;
 }
 MeshTextureCoords {
  32;
  0.018370;0.304520;,
  0.018370;0.353140;,
  0.016130;0.891300;,
  0.016130;1.013250;,
  0.018370;0.397390;,
  0.018370;0.432850;,
  0.016130;0.691410;,
  0.016130;0.780350;,
  0.018370;0.526400;,
  0.016130;0.456790;,
  0.016130;0.569820;,
  0.018370;0.481330;,
  0.000080;0.000080;,
  0.000080;0.000080;,
  0.018370;0.003900;,
  0.987040;0.000080;,
  0.018370;0.574880;,
  0.016130;0.335190;,
  0.016130;0.456790;,
  0.016130;0.569820;,
  0.439560;0.789790;,
  0.450500;0.812620;,
  0.450500;0.812620;,
  0.439560;0.789790;,
  0.433610;0.762280;,
  0.433610;0.762280;,
  0.462830;0.828350;,
  0.462830;0.828350;,
  0.016130;0.335190;,
  0.487870;0.836340;,
  0.487870;0.836340;,
  0.987040;0.000080;;
 }
}
