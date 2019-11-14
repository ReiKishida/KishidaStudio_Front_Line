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
 1.49525;1.43082;2.04380;,
 1.49525;-1.37007;2.04380;,
 -0.49952;-1.37007;-0.55407;,
 -0.49952;1.43082;-0.55407;,
 -1.31080;-1.37008;5.04417;,
 -1.49858;-1.54153;5.18835;,
 -4.31042;-1.54153;1.52637;,
 -4.12265;-1.37008;1.38219;,
 -4.12265;1.43082;1.38219;,
 -1.31080;1.43082;5.04417;,
 -5.88251;-4.77833;2.73350;,
 -7.67417;-3.18881;4.10922;,
 -5.67220;0.64309;2.57201;,
 -2.86036;0.64309;6.23399;,
 -4.86233;-3.18881;7.77121;,
 -3.07067;-4.77833;6.39548;;
 
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
  0.793154;0.000000;-0.609022;,
  -0.541999;-0.000001;-0.840379;,
  0.671946;-0.000000;0.740600;,
  0.244521;-0.951293;-0.187755;,
  -0.609022;-0.000001;-0.793153;,
  -0.502298;0.773913;0.385688;,
  0.582105;-0.679245;-0.446968;,
  -0.609022;-0.000001;-0.793154;,
  -0.456439;-0.817820;0.350475;,
  -0.471332;0.000000;-0.881956;,
  0.730362;0.000000;0.683060;,
  0.000002;-1.000000;-0.000002;,
  0.000000;1.000000;0.000000;,
  -0.151067;0.981694;0.115996;,
  0.609017;-0.000000;0.793157;,
  0.609023;0.000000;0.793153;,
  0.676409;-0.522223;-0.519378;,
  -0.662370;0.550085;0.508598;;
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
