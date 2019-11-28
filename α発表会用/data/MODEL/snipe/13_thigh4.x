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
 1.62717;1.43082;-0.68535;,
 1.62717;-1.37007;-0.68535;,
 -0.59846;-1.37007;1.71770;,
 -0.59846;1.43082;1.71770;,
 5.05649;-1.37007;1.57649;,
 5.23019;-1.54152;1.73736;,
 2.09292;-1.54152;5.12470;,
 1.91923;-1.37007;4.96383;,
 1.91923;1.43082;4.96383;,
 5.05649;1.43082;1.57649;,
 3.54711;-4.77832;6.47153;,
 5.20440;-3.18881;8.00647;,
 3.35257;0.64309;6.29135;,
 6.48984;0.64310;2.90401;,
 8.34167;-3.18881;4.61913;,
 6.68438;-4.77832;3.08419;;
 
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
  -0.733672;0.000000;-0.679504;,
  -0.737325;0.000001;0.675538;,
  0.617117;0.000002;-0.786872;,
  -0.226179;-0.951294;-0.209481;,
  -0.679507;0.000002;0.733669;,
  0.464626;0.773914;0.430326;,
  -0.538447;-0.679248;-0.498696;,
  -0.679506;0.000000;0.733670;,
  0.422206;-0.817822;0.391037;,
  -0.790186;0.000000;0.612867;,
  0.550586;0.000000;-0.834778;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  0.139736;0.981695;0.129421;,
  0.679503;0.000002;-0.733673;,
  0.679506;-0.000000;-0.733670;,
  -0.625680;-0.522223;-0.579489;,
  0.612694;0.550084;0.567462;;
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
