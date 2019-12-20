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
 18;
 65.65973;22.36698;-59.59739;,
 54.30268;-5.73563;-54.58638;,
 54.30268;-5.73564;-8.81486;,
 65.65973;22.36697;-13.82587;,
 108.26177;-5.73564;-8.81486;,
 109.98679;22.36697;-13.82587;,
 65.65973;22.36698;-77.03279;,
 54.30268;-5.73563;-72.02177;,
 109.98679;22.36698;-59.59739;,
 110.12720;-5.69249;-49.93927;,
 141.40075;-5.69249;-49.93927;,
 141.40075;22.36698;-59.59739;,
 69.52292;-5.02724;-22.49324;,
 69.52292;-3.70401;-61.81113;,
 107.60002;-3.70401;-22.49324;,
 69.52292;-3.70401;-76.78819;,
 107.60002;-1.65982;-61.81113;,
 134.58471;-1.27479;-61.81113;;
 
 20;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;6,7,1,0;,
 4;8,9,10,11;,
 4;8,5,4,9;,
 4;3,12,13,0;,
 4;5,14,12,3;,
 4;0,13,15,6;,
 4;8,16,14,5;,
 4;11,17,16,8;,
 3;6,15,7;,
 3;11,10,17;,
 3;12,1,13;,
 4;2,12,14,4;,
 4;4,14,16,9;,
 3;10,9,16;,
 3;15,13,7;,
 3;7,13,1;,
 3;1,12,2;,
 3;10,16,17;;
 
 MeshMaterialList {
  1;
  20;
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
  27;
  -0.927151;0.374688;0.000000;,
  -0.927151;0.374688;0.000000;,
  0.000000;0.325462;0.945555;,
  -0.927151;0.374688;0.000000;,
  0.000000;0.175542;0.984472;,
  0.989199;0.146579;0.000000;,
  0.989469;0.144743;0.001230;,
  0.470239;-0.053586;-0.880911;,
  -0.967963;-0.250503;-0.017199;,
  0.989733;0.142906;0.002460;,
  -0.005497;0.309023;-0.951039;,
  0.999454;-0.024102;0.022613;,
  -0.995495;0.094777;0.002558;,
  -0.000655;0.092553;-0.995708;,
  -0.292087;-0.052241;-0.954964;,
  0.820622;-0.185992;-0.540357;,
  0.060214;-0.997188;-0.044620;,
  0.098561;-0.995068;-0.011145;,
  0.127104;-0.991827;-0.011137;,
  0.031888;-0.998231;-0.050179;,
  0.017196;-0.994817;-0.100222;,
  -0.457889;-0.885696;-0.076685;,
  -0.468440;-0.859929;-0.202698;,
  0.006669;-0.940908;-0.338598;,
  0.132308;-0.991209;-0.000000;,
  0.132308;-0.991209;-0.000000;,
  0.013336;-0.934605;-0.355437;;
  20;
  4;1,1,3,3;,
  4;4,4,4,4;,
  4;0,0,1,1;,
  4;2,2,2,2;,
  4;11,11,11,11;,
  4;9,9,6,6;,
  4;10,10,10,10;,
  4;6,6,5,5;,
  4;12,8,8,12;,
  4;7,7,13,13;,
  3;14,14,14;,
  3;7,15,7;,
  3;16,17,18;,
  4;19,16,20,21;,
  4;21,8,8,22;,
  3;23,22,23;,
  3;24,18,25;,
  3;25,18,17;,
  3;17,16,19;,
  3;23,23,26;;
 }
 MeshTextureCoords {
  18;
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
