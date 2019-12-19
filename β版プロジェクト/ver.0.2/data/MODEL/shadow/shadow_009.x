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
 215.17368;-0.00005;1081.54484;,
 215.17368;-0.00003;740.11608;,
 215.17368;218.35550;740.11608;,
 503.26119;218.35547;1081.54484;,
 503.26119;-0.00005;1081.54484;,
 215.17368;-0.00005;1081.54484;,
 124.11843;-0.00004;808.83634;,
 412.20591;-0.00005;1150.26515;,
 503.26119;218.35547;1081.54484;,
 215.17368;218.35547;1081.54484;,
 215.17368;218.35550;740.11608;,
 124.11845;-0.00005;1150.26515;,
 215.17368;218.35547;1081.54484;;
 
 14;
 3;0,1,2;,
 3;3,4,5;,
 3;1,6,2;,
 3;7,8,9;,
 3;9,10,6;,
 3;8,7,4;,
 3;5,11,6;,
 3;0,2,12;,
 3;3,5,12;,
 3;7,9,11;,
 3;9,6,11;,
 3;5,6,1;,
 3;11,5,7;,
 3;7,5,4;;
 
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
  17;
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.300201;0.953876;,
  0.000000;0.000000;-1.000000;,
  -0.910924;0.286397;-0.296974;,
  -0.000000;-1.000000;-0.000000;,
  0.212379;0.211673;0.953986;,
  -0.602403;0.000000;-0.798192;,
  -0.864680;0.218176;-0.452468;,
  0.320957;0.159945;0.933490;,
  -0.922966;0.384881;0.000000;,
  0.602403;0.000000;0.798192;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.922966;0.384881;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;0.000000;;
  14;
  3;0,0,0;,
  3;3,1,3;,
  3;7,4,8;,
  3;6,9,2;,
  3;10,8,4;,
  3;9,6,11;,
  3;12,5,13;,
  3;0,0,0;,
  3;3,3,1;,
  3;6,2,2;,
  3;10,4,14;,
  3;12,13,15;,
  3;5,12,16;,
  3;16,12,16;;
 }
 MeshTextureCoords {
  13;
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.170000;0.000000;,
  0.010000;1.000000;,
  0.010000;1.000000;,
  0.170000;1.000000;,
  0.170000;0.000000;,
  0.170000;1.000000;,
  1.000000;0.000000;;
 }
}
