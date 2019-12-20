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
 755.22682;195.18535;1078.47629;,
 755.22682;195.18537;897.40597;,
 678.87345;-0.00004;936.61994;,
 755.22682;195.18537;780.12740;,
 678.87345;-0.00004;819.34142;,
 1389.33117;195.18535;1078.47629;,
 1312.97785;-0.00005;1117.69025;,
 1478.55090;-0.00005;1117.69025;,
 678.87345;-0.00005;1117.69025;,
 755.22682;-0.00003;780.12740;,
 1554.90422;-0.00005;1078.47629;,
 1554.90422;195.18535;1078.47629;,
 729.68673;-0.00005;1058.92013;;
 
 22;
 3;0,1,2;,
 3;1,3,4;,
 3;5,6,7;,
 3;0,8,6;,
 3;3,9,4;,
 3;10,11,7;,
 3;8,10,6;,
 3;9,12,2;,
 3;1,12,9;,
 3;10,5,11;,
 3;0,2,8;,
 3;1,4,2;,
 3;5,7,11;,
 3;0,6,5;,
 3;10,8,12;,
 3;6,10,7;,
 3;4,9,2;,
 3;2,12,8;,
 3;12,1,0;,
 3;1,9,3;,
 3;0,5,12;,
 3;12,5,10;;
 
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
   0.020800;0.640000;0.000000;1.000000;;
   0.000000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  29;
  -0.931281;0.364302;0.000000;,
  -0.931281;0.364302;0.000000;,
  0.236101;0.101793;0.966382;,
  0.456855;0.000000;0.889541;,
  0.000000;0.196970;0.980410;,
  -0.931281;0.364302;0.000000;,
  -0.931281;0.364302;0.000000;,
  0.000000;0.196970;0.980409;,
  0.156807;0.135212;0.978330;,
  0.000000;0.196970;0.980410;,
  -0.931281;0.364302;0.000000;,
  -0.456856;0.000000;-0.889541;,
  -0.000000;-1.000000;-0.000000;,
  -0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.997635;-0.061637;0.030435;,
  0.994676;-0.092400;0.045625;,
  0.998583;-0.027405;0.045610;,
  0.011845;0.010048;-0.999879;,
  0.007903;0.039969;-0.999170;,
  0.000000;0.000000;-1.000000;,
  -0.931281;0.364302;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.991547;-0.129744;-0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.099694;-0.995018;,
  0.011854;0.059945;-0.998131;;
  22;
  3;1,0,6;,
  3;0,5,10;,
  3;4,9,8;,
  3;7,7,9;,
  3;11,11,11;,
  3;3,2,8;,
  3;13,14,14;,
  3;15,12,16;,
  3;17,18,19;,
  3;20,21,22;,
  3;1,6,23;,
  3;0,10,6;,
  3;4,8,2;,
  3;7,9,4;,
  3;14,13,12;,
  3;14,14,14;,
  3;24,15,16;,
  3;16,12,13;,
  3;18,17,25;,
  3;17,19,26;,
  3;27,21,28;,
  3;28,21,20;;
 }
 MeshTextureCoords {
  13;
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
