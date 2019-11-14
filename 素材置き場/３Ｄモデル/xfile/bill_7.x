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
 68;
 54.67602;10.17790;-54.67602;,
 -54.67602;10.17790;-54.67602;,
 -54.67602;204.71805;-54.67602;,
 54.67602;204.71805;-54.67602;,
 54.67602;204.71805;-54.67602;,
 -54.67602;204.71805;-54.67602;,
 -54.67602;204.71805;54.67602;,
 54.67602;204.71805;54.67602;,
 54.67602;204.71805;54.67602;,
 -54.67602;204.71805;54.67602;,
 -54.67602;10.17790;54.67602;,
 54.67602;10.17790;54.67602;,
 54.67602;0.12651;54.67602;,
 -54.67602;0.12651;54.67602;,
 54.67602;0.12651;-54.67602;,
 -54.67602;0.12651;11.71217;,
 -54.67602;0.12651;-15.62585;,
 -54.67602;0.12651;-54.67602;,
 -54.67602;10.17790;54.67602;,
 -54.67602;204.71805;54.67602;,
 -54.67602;10.17790;11.71217;,
 -54.67602;204.71805;-54.67602;,
 -54.67602;10.17790;-15.62585;,
 -54.67602;10.17790;-54.67602;,
 54.67602;10.17790;54.67602;,
 54.67602;10.17790;-54.67602;,
 54.67602;204.71805;-54.67602;,
 54.67602;204.71805;54.67602;,
 54.67602;0.12651;-54.67602;,
 -54.67602;0.12651;-54.67602;,
 -54.67602;10.17790;-54.67602;,
 54.67602;10.17790;-54.67602;,
 -54.67602;0.12651;54.67602;,
 -54.67602;10.17790;54.67602;,
 -54.67602;0.12651;11.71217;,
 -54.67602;10.17790;11.71217;,
 -54.67602;10.17790;-15.62585;,
 -54.67602;0.12651;-15.62585;,
 54.67602;10.17790;54.67602;,
 -54.67602;10.17790;54.67602;,
 -54.67602;0.12651;54.67602;,
 54.67602;0.12651;54.67602;,
 54.67602;0.12651;54.67602;,
 54.67602;10.17790;54.67602;,
 43.51552;181.23025;-3.15405;,
 -2.16625;181.23025;-3.15405;,
 -2.16625;226.91494;-3.15405;,
 43.51552;226.91494;-3.15405;,
 43.51552;226.91494;-3.15405;,
 -2.16625;226.91494;-3.15405;,
 -2.16625;226.91494;42.52773;,
 43.51552;226.91494;42.52773;,
 43.51552;226.91494;42.52773;,
 -2.16625;226.91494;42.52773;,
 -2.16625;181.23025;42.52773;,
 43.51552;181.23025;42.52773;,
 43.51552;181.23025;42.52773;,
 -2.16625;181.23025;42.52773;,
 -2.16625;181.23025;-3.15405;,
 43.51552;181.23025;-3.15405;,
 -2.16625;181.23025;-3.15405;,
 -2.16625;181.23025;42.52773;,
 -2.16625;226.91494;42.52773;,
 -2.16625;226.91494;-3.15405;,
 43.51552;181.23025;42.52773;,
 43.51552;181.23025;-3.15405;,
 43.51552;226.91494;-3.15405;,
 43.51552;226.91494;42.52773;;
 
 27;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 3;12,13,14;,
 3;14,13,15;,
 3;14,15,16;,
 3;16,17,14;,
 3;18,19,20;,
 3;19,21,20;,
 3;20,21,22;,
 3;21,23,22;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 3;32,33,34;,
 3;33,35,34;,
 3;35,36,34;,
 3;34,36,37;,
 3;36,30,37;,
 3;30,29,37;,
 4;38,39,40,41;,
 4;42,28,31,43;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;52,53,54,55;,
 4;56,57,58,59;,
 4;60,61,62,63;,
 4;64,65,66,67;;
 
 MeshMaterialList {
  1;
  27;
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
   0.656000;0.656000;0.656000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MODEL\\bill\\bill_7.jpg";
   }
  }
 }
 MeshNormals {
  6;
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;;
  27;
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  4;5,5,5,5;,
  4;1,1,1,1;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  4;3,3,3,3;,
  4;5,5,5,5;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;5,5,5,5;;
 }
 MeshTextureCoords {
  68;
  -1.352060;2.032810;,
  2.523270;2.032810;,
  2.523270;-1.211840;,
  -1.352060;-1.211840;,
  0.647680;0.855230;,
  0.659900;0.855230;,
  0.659900;0.843010;,
  0.647680;0.843010;,
  -1.384050;2.045710;,
  2.491280;2.045710;,
  2.491280;-1.198930;,
  -1.384050;-1.198930;,
  3.229570;-4.654890;,
  3.851820;-4.654890;,
  3.229570;-5.277140;,
  3.851820;-4.966020;,
  3.851820;-5.121580;,
  3.851820;-5.277140;,
  2.496300;2.032810;,
  2.496300;-1.211840;,
  0.558640;2.032810;,
  -1.379020;-1.211840;,
  -0.410190;2.032810;,
  -1.379020;2.032810;,
  -1.325090;2.032810;,
  2.550240;2.032810;,
  2.550240;-1.211840;,
  -1.325090;-1.211840;,
  0.345490;0.758620;,
  0.366730;0.758620;,
  0.366730;0.755160;,
  0.345490;0.755160;,
  0.387970;0.758620;,
  0.387970;0.755160;,
  0.377350;0.758620;,
  0.377350;0.755160;,
  0.372040;0.755160;,
  0.372040;0.758620;,
  0.324100;0.759750;,
  0.345340;0.759750;,
  0.345340;0.756290;,
  0.324100;0.756290;,
  0.324250;0.758620;,
  0.324250;0.755160;,
  0.321610;1.053390;,
  0.678390;1.053390;,
  0.678390;0.696610;,
  0.321610;0.696610;,
  0.244410;0.454830;,
  0.410740;0.454830;,
  0.410740;0.288500;,
  0.244410;0.288500;,
  0.244410;0.457050;,
  0.410740;0.457050;,
  0.410740;0.290710;,
  0.244410;0.290710;,
  0.244410;0.453250;,
  0.410740;0.453250;,
  0.410740;0.286920;,
  0.244410;0.286920;,
  0.242190;0.452610;,
  0.408520;0.452610;,
  0.408520;0.286280;,
  0.242190;0.286280;,
  0.241190;0.452610;,
  0.407530;0.452610;,
  0.407530;0.286280;,
  0.241190;0.286280;;
 }
}
