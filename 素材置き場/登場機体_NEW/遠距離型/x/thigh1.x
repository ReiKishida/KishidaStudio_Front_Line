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
 -1.04620;1.43082;-0.94791;,
 -1.04620;-1.37007;-0.94791;,
 1.04362;-1.37007;1.57413;,
 1.04362;1.43082;1.57413;,
 1.64656;-1.37007;-4.05036;,
 1.82885;-1.54152;-4.20142;,
 4.77468;-1.54152;-0.64633;,
 4.59238;-1.37007;-0.49527;,
 4.59238;1.43083;-0.49527;,
 1.64655;1.43082;-4.05036;,
 6.30088;-4.77832;-1.91097;,
 8.04024;-3.18880;-3.35225;,
 6.09670;0.64310;-1.74179;,
 3.15088;0.64310;-5.29688;,
 5.09442;-3.18881;-6.90734;,
 3.35505;-4.77832;-5.46606;;
 
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
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  18;
  -0.770002;0.000000;0.638041;,
  0.572819;-0.000001;0.819682;,
  -0.698981;-0.000001;-0.715140;,
  -0.237377;-0.951295;0.196696;,
  0.638043;-0.000000;0.770001;,
  0.487635;0.773914;-0.404066;,
  -0.565110;-0.679249;0.468263;,
  0.638042;0.000001;0.770001;,
  0.443115;-0.817822;-0.367174;,
  0.503742;0.000000;0.863854;,
  -0.755211;-0.000001;-0.655482;,
  0.000000;-1.000000;-0.000000;,
  -0.000002;1.000000;0.000000;,
  0.146655;0.981695;-0.121523;,
  -0.638047;-0.000000;-0.769997;,
  -0.638041;0.000000;-0.770003;,
  -0.656665;-0.522222;0.544128;,
  0.643036;0.550085;-0.532833;;
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
