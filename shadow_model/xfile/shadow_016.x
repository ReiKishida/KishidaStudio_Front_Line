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
 33;
 -525.92384;-0.00002;490.31632;,
 -525.92384;218.18406;490.31632;,
 -503.11495;218.18406;528.64600;,
 -322.30306;218.18406;555.04547;,
 -322.30306;-0.00002;555.04547;,
 -374.53016;-0.00002;568.46153;,
 -425.15850;-0.00002;568.46153;,
 -425.15850;218.18406;568.46153;,
 -374.53016;218.18406;568.46153;,
 -296.57371;218.18406;528.64600;,
 -296.57371;-0.00002;528.64600;,
 -503.11495;-0.00002;528.64600;,
 -477.38560;218.18406;555.04547;,
 -538.33741;-0.00002;444.14034;,
 -538.33741;218.18406;444.14034;,
 -538.33741;0.00000;78.38602;,
 -538.33741;218.18409;78.38603;,
 -538.33741;218.18406;444.14034;,
 -296.57371;218.18406;528.64600;,
 -263.94295;-0.00003;601.60352;,
 -645.82745;-0.00001;148.09296;,
 -322.30306;218.18406;555.04547;,
 -549.54923;-0.00003;643.30353;,
 -467.05255;-0.00003;664.49518;,
 -590.19078;-0.00003;601.60352;,
 -304.58445;-0.00003;643.30353;,
 -374.53016;218.18406;568.46153;,
 -626.21920;-0.00002;541.05880;,
 -387.08116;-0.00003;664.49518;,
 -645.82745;-0.00002;468.12032;,
 -477.38560;-0.00002;555.04547;,
 -387.08116;-0.00003;664.49518;,
 -645.82745;-0.00002;468.12032;;
 
 50;
 3;0,1,2;,
 3;3,4,5;,
 3;6,7,8;,
 3;3,9,10;,
 3;11,2,12;,
 3;13,14,1;,
 3;15,16,17;,
 3;18,19,10;,
 3;15,20,16;,
 3;12,7,6;,
 3;19,18,21;,
 3;12,22,23;,
 3;12,2,24;,
 3;25,21,26;,
 3;2,1,27;,
 3;7,23,28;,
 3;17,16,20;,
 3;1,14,29;,
 3;27,11,0;,
 3;0,2,11;,
 3;3,5,8;,
 3;6,8,5;,
 3;3,10,4;,
 3;11,12,30;,
 3;13,1,0;,
 3;15,17,13;,
 3;12,6,30;,
 3;19,21,25;,
 3;12,23,7;,
 3;12,24,22;,
 3;25,26,31;,
 3;2,27,24;,
 3;7,28,8;,
 3;17,20,32;,
 3;1,29,27;,
 3;20,13,15;,
 3;20,32,13;,
 3;13,32,0;,
 3;27,0,32;,
 3;22,30,24;,
 3;24,11,27;,
 3;28,6,23;,
 3;23,30,22;,
 3;6,30,23;,
 3;19,4,25;,
 3;25,5,28;,
 3;4,19,10;,
 3;5,25,4;,
 3;5,6,28;,
 3;24,30,11;;
 
 MeshMaterialList {
  1;
  50;
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
  51;
  1.000000;0.000000;0.000000;,
  0.984673;0.000000;-0.174410;,
  1.000000;0.000000;0.000000;,
  0.996190;0.000000;-0.087205;,
  0.902328;0.000000;-0.431049;,
  0.938079;0.000000;-0.346423;,
  -0.716137;0.000000;-0.697959;,
  0.768616;0.000000;-0.639710;,
  0.816652;0.000000;-0.577130;,
  -0.501081;0.000000;-0.865400;,
  0.501081;0.000000;-0.865400;,
  -0.167039;0.000000;-0.985950;,
  0.125390;0.000000;-0.992108;,
  -0.083520;0.000000;-0.996506;,
  -0.544101;0.000000;-0.839019;,
  -0.879254;0.450261;0.155491;,
  -0.802278;0.457871;0.383019;,
  0.912856;0.000000;-0.408281;,
  -0.681809;0.461813;0.567332;,
  0.390753;0.372274;0.841857;,
  -0.443431;0.458260;0.770303;,
  0.077143;0.397932;0.914166;,
  -0.150765;0.422356;0.893804;,
  0.677918;0.322318;0.660710;,
  0.544540;0.352148;0.761228;,
  -0.443431;0.458260;0.770303;,
  -0.724931;0.460584;0.512189;,
  0.154286;0.390581;0.907547;,
  -0.835148;0.455577;0.308185;,
  -0.075383;0.413797;0.907243;,
  -0.890917;0.447462;0.077745;,
  -0.897046;0.441937;0.000000;,
  -0.897046;0.441937;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.000000;1.000000;0.000000;,
  -0.000000;1.000000;0.000000;,
  -0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.000000;1.000000;0.000000;,
  -0.000000;1.000000;0.000000;,
  -0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.000000;1.000000;0.000000;,
  -0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;;
  50;
  3;4,5,8;,
  3;9,9,11;,
  3;12,12,13;,
  3;9,6,6;,
  3;7,8,10;,
  3;1,3,5;,
  3;0,2,3;,
  3;17,17,17;,
  3;14,14,14;,
  3;10,12,12;,
  3;23,23,24;,
  3;25,20,22;,
  3;25,26,18;,
  3;19,24,27;,
  3;26,28,16;,
  3;29,22,21;,
  3;30,31,32;,
  3;28,30,15;,
  3;33,34,35;,
  3;4,8,7;,
  3;9,11,13;,
  3;12,13,11;,
  3;9,6,9;,
  3;7,10,10;,
  3;1,5,4;,
  3;0,3,1;,
  3;10,12,10;,
  3;23,24,19;,
  3;25,22,29;,
  3;25,18,20;,
  3;19,27,21;,
  3;26,16,18;,
  3;29,21,27;,
  3;30,32,15;,
  3;28,15,16;,
  3;36,37,38;,
  3;36,39,37;,
  3;37,39,35;,
  3;33,35,39;,
  3;40,41,42;,
  3;42,34,33;,
  3;43,44,45;,
  3;45,41,40;,
  3;44,41,45;,
  3;46,47,48;,
  3;48,49,43;,
  3;47,46,50;,
  3;49,48,47;,
  3;49,44,43;,
  3;42,41,34;;
 }
 MeshTextureCoords {
  33;
  0.018370;0.654590;,
  0.016130;0.135300;,
  0.016130;0.246260;,
  0.016130;0.691410;,
  0.018370;0.432850;,
  0.018370;0.481330;,
  0.018370;0.526400;,
  0.016130;0.456790;,
  0.016130;0.569820;,
  0.016130;0.780350;,
  0.018370;0.397390;,
  0.018370;0.610350;,
  0.016130;0.335190;,
  0.018370;0.703210;,
  0.016130;0.013350;,
  0.018370;1.003830;,
  0.979110;0.000080;,
  0.000080;0.000080;,
  0.446600;0.812620;,
  0.446600;0.812620;,
  0.979110;0.000080;,
  0.459900;0.828350;,
  0.016130;0.335190;,
  0.016130;0.456790;,
  0.016130;0.246260;,
  0.459900;0.828350;,
  0.486910;0.836340;,
  0.016130;0.135300;,
  0.016130;0.569820;,
  0.016130;0.013350;,
  0.018370;0.574880;,
  0.486910;0.836340;,
  0.000080;0.000080;;
 }
}