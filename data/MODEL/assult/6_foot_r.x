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
 108;
 -0.64245;-0.26702;-0.32725;,
 -3.60296;-0.26702;-0.32725;,
 -3.60296;-0.22264;2.23396;,
 -0.64245;-3.53488;-0.27063;,
 -3.60296;-3.53488;-0.27063;,
 -0.64244;-1.45229;3.18243;,
 -0.64244;-3.47444;3.21747;,
 -0.67857;-2.54194;2.32589;,
 -3.60296;-1.45229;3.18243;,
 -0.64244;-0.22264;2.23396;,
 -3.60296;-3.47444;3.21747;,
 3.48869;-0.26702;-0.32725;,
 0.52818;-0.26702;-0.32725;,
 0.52818;-0.22264;2.23396;,
 3.48869;-3.53488;-0.27063;,
 0.52818;-3.53488;-0.27063;,
 3.48869;-0.22264;2.23396;,
 3.48869;-3.47444;3.21747;,
 0.56431;-1.11040;0.74382;,
 0.52818;-1.45229;3.18243;,
 0.52818;-3.47444;3.21747;,
 -0.67857;-1.09060;1.88694;,
 -0.67857;-2.56891;0.76909;,
 -0.67857;-1.11040;0.74382;,
 0.56431;-1.09060;1.88694;,
 0.56431;-2.56891;0.76909;,
 0.56431;-1.63941;2.31025;,
 -0.67857;-1.63941;2.31025;,
 0.56431;-2.54194;2.32589;,
 1.82747;-0.48589;1.56863;,
 1.82747;-3.09245;1.61380;,
 1.82747;-3.11922;0.06878;,
 1.82747;-0.51266;0.02361;,
 -1.15125;-3.11922;0.06878;,
 -1.15125;-0.51266;0.02361;,
 -1.15125;-3.09245;1.61380;,
 -1.15125;-0.48589;1.56863;,
 1.95422;0.09136;-0.62114;,
 1.95422;-3.54461;-0.55814;,
 3.40023;-3.54461;-0.55814;,
 3.40023;-3.62135;-4.98669;,
 3.40023;-3.66450;-7.47682;,
 3.40023;-1.16725;-7.52009;,
 1.95422;-3.66450;-7.47682;,
 -0.00413;-3.54461;-0.55814;,
 3.40023;0.09136;-0.62114;,
 3.40023;0.01463;-5.04969;,
 -3.40850;0.01463;-5.04969;,
 -0.00413;0.09136;-0.62114;,
 1.95422;-1.16725;-7.52009;,
 -1.96249;-1.16725;-7.52009;,
 1.03415;-3.35762;-8.76784;,
 1.26863;-3.39906;-8.62031;,
 1.26863;-3.38188;-7.62853;,
 1.26863;-3.66450;-7.47682;,
 -0.00413;-3.66450;-7.47682;,
 -0.00413;-3.66450;-7.47682;,
 -1.04242;-3.35762;-8.76784;,
 -1.04242;-3.38470;-10.33049;,
 1.26863;-1.45496;-7.66192;,
 3.59912;-1.45496;-7.66192;,
 3.59912;-1.47214;-8.65370;,
 1.26863;-1.47214;-8.65370;,
 1.03415;-1.51867;-8.79971;,
 1.26863;-1.16725;-7.52009;,
 3.59912;-3.38188;-7.62853;,
 3.59912;-3.39906;-8.62031;,
 1.26863;-3.85265;-10.86772;,
 3.59912;-3.85265;-10.86772;,
 3.59912;-3.90055;-13.63161;,
 3.59912;-2.19923;-12.20477;,
 3.59912;-1.92573;-10.90111;,
 1.26863;-1.92573;-10.90111;,
 1.26863;-2.19923;-12.20477;,
 1.26863;-3.90055;-13.63161;,
 -3.40850;-3.54461;-0.55814;,
 -1.96249;-3.54461;-0.55814;,
 -1.96249;0.09136;-0.62114;,
 -3.40850;-3.62135;-4.98669;,
 -3.40850;-1.16725;-7.52009;,
 -3.40850;0.09136;-0.62114;,
 -1.27689;-3.66450;-7.47681;,
 -1.96249;-3.66450;-7.47681;,
 -1.27689;-3.38188;-7.62853;,
 -1.27689;-3.39906;-8.62031;,
 1.03415;-3.38470;-10.33049;,
 -1.04242;-2.19070;-10.35118;,
 -1.27689;-1.45496;-7.66192;,
 -1.27689;-1.47214;-8.65370;,
 -3.60739;-1.47214;-8.65370;,
 -3.60739;-1.45496;-7.66192;,
 -1.04242;-1.51867;-8.79971;,
 -1.27689;-1.16725;-7.52009;,
 -3.60739;-3.38188;-7.62853;,
 -3.60739;-3.39906;-8.62031;,
 -3.60739;-3.85265;-10.86772;,
 -1.27689;-3.85265;-10.86772;,
 -3.60739;-1.92573;-10.90111;,
 -3.60739;-2.19923;-12.20477;,
 -3.60739;-3.90055;-13.63161;,
 -1.27689;-1.92573;-10.90111;,
 -1.27689;-2.19923;-12.20477;,
 -1.27689;-3.90055;-13.63161;,
 1.03415;-2.19070;-10.35118;,
 -1.04242;-1.53787;-9.90766;,
 3.48869;-1.45229;3.18243;,
 -3.40850;-3.66450;-7.47681;,
 1.03415;-1.53787;-9.90766;;
 
 166;
 3;0,1,2;,
 3;0,3,4;,
 3;5,6,7;,
 3;4,3,6;,
 3;2,4,8;,
 3;9,2,8;,
 3;8,10,6;,
 3;11,12,13;,
 3;11,14,15;,
 3;16,14,11;,
 3;15,14,17;,
 3;13,12,18;,
 3;16,13,19;,
 3;19,20,17;,
 3;21,22,23;,
 3;24,25,26;,
 3;12,15,25;,
 3;5,27,21;,
 3;19,26,28;,
 3;9,21,23;,
 3;20,28,25;,
 3;6,3,22;,
 3;0,23,22;,
 3;19,13,24;,
 3;29,30,31;,
 3;32,31,33;,
 3;34,33,35;,
 3;36,35,30;,
 3;33,31,30;,
 3;32,34,36;,
 3;37,38,39;,
 3;40,41,42;,
 3;40,43,41;,
 3;38,44,40;,
 3;45,39,40;,
 3;46,47,48;,
 3;49,50,47;,
 3;48,44,38;,
 4;51,52,53,54;,
 3;55,56,54;,
 3;51,57,58;,
 4;59,60,61,62;,
 4;63,62,52,51;,
 4;54,53,59,64;,
 4;64,59,62,63;,
 4;65,66,61,60;,
 4;53,65,60,59;,
 4;52,66,65,53;,
 4;52,67,68,66;,
 4;68,69,70,71;,
 4;66,68,71,61;,
 4;62,72,67,52;,
 4;61,71,72,62;,
 4;73,70,69,74;,
 4;71,70,73,72;,
 4;72,73,74,67;,
 4;67,74,69,68;,
 3;75,76,77;,
 3;78,47,79;,
 3;57,51,56;,
 3;78,75,80;,
 3;81,82,50;,
 3;64,49,43;,
 3;76,44,48;,
 4;57,81,83,84;,
 3;55,82,81;,
 3;85,58,86;,
 3;49,64,50;,
 4;87,88,89,90;,
 4;91,57,84,88;,
 4;81,92,87,83;,
 4;92,91,88,87;,
 4;93,90,89,94;,
 4;83,87,90,93;,
 4;84,83,93,94;,
 4;84,94,95,96;,
 4;95,97,98,99;,
 4;94,89,97,95;,
 4;88,84,96,100;,
 4;89,88,100,97;,
 4;101,102,99,98;,
 4;97,100,101,98;,
 4;100,96,102,101;,
 4;96,95,99,102;,
 3;57,91,86;,
 3;63,51,103;,
 3;103,86,104;,
 3;0,2,9;,
 3;0,4,1;,
 3;5,7,27;,
 3;4,6,10;,
 3;10,8,4;,
 3;1,4,2;,
 3;9,8,5;,
 3;8,6,5;,
 3;11,13,16;,
 3;11,15,12;,
 3;14,105,17;,
 3;105,14,16;,
 3;15,17,20;,
 3;13,18,24;,
 3;16,19,105;,
 3;19,17,105;,
 3;22,27,7;,
 3;27,22,21;,
 3;28,26,25;,
 3;18,25,24;,
 3;12,25,18;,
 3;5,21,9;,
 3;19,28,20;,
 3;9,23,0;,
 3;20,25,15;,
 3;6,22,7;,
 3;0,22,3;,
 3;19,24,26;,
 3;29,31,32;,
 3;32,33,34;,
 3;34,35,36;,
 3;36,30,29;,
 3;33,30,35;,
 3;32,36,29;,
 3;37,39,45;,
 3;40,42,46;,
 3;43,40,55;,
 3;82,78,106;,
 3;78,55,40;,
 3;55,78,82;,
 3;76,78,44;,
 3;38,40,39;,
 3;40,44,78;,
 3;75,78,76;,
 3;45,40,46;,
 3;80,77,47;,
 3;46,37,45;,
 3;48,47,77;,
 3;46,48,37;,
 3;79,47,50;,
 3;49,46,42;,
 3;46,49,47;,
 3;48,38,37;,
 3;55,54,43;,
 3;51,58,85;,
 3;75,77,80;,
 3;78,79,106;,
 3;54,56,51;,
 3;57,56,81;,
 3;78,80,47;,
 3;106,50,82;,
 3;81,50,92;,
 3;50,106,79;,
 3;42,43,49;,
 3;64,43,54;,
 3;43,42,41;,
 3;76,48,77;,
 3;55,81,56;,
 3;85,86,103;,
 3;91,107,104;,
 3;91,63,107;,
 3;92,63,91;,
 3;92,64,63;,
 3;50,64,92;,
 3;104,86,91;,
 3;57,86,58;,
 3;85,103,51;,
 3;63,103,107;,
 3;103,104,107;;
 
 MeshMaterialList {
  1;
  166;
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
   0.640000;0.640000;0.640000;1.000000;;
   0.500000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  117;
  0.999583;0.025159;-0.014191;,
  0.000000;-0.017324;-0.999850;,
  0.000000;0.999850;-0.017325;,
  0.000000;0.017326;0.999850;,
  0.999165;-0.014053;-0.038376;,
  0.000000;0.817264;0.576264;,
  0.000000;0.960416;0.278571;,
  -1.000000;0.000000;0.000000;,
  0.000000;-0.999850;0.017325;,
  0.000000;-0.017324;-0.999850;,
  0.000000;0.999850;-0.017325;,
  1.000000;0.000000;0.000000;,
  0.000000;0.330819;0.943694;,
  0.999893;0.011053;0.009576;,
  0.999742;0.009167;-0.020790;,
  0.999744;-0.020797;0.008907;,
  0.999714;-0.019337;-0.014062;,
  0.999791;-0.007027;-0.019191;,
  -0.999744;-0.020799;0.008911;,
  -0.999893;0.011054;0.009580;,
  -0.999742;0.009167;-0.020790;,
  -0.999791;-0.007027;-0.019191;,
  -0.999714;-0.019337;-0.014062;,
  0.000000;-0.017327;-0.999850;,
  0.000000;0.017327;0.999850;,
  0.000000;0.017324;0.999850;,
  0.000000;0.982334;-0.187135;,
  0.000000;0.017324;0.999850;,
  0.000000;-0.999850;0.017326;,
  0.000000;0.902080;-0.431570;,
  0.920995;-0.379483;-0.088090;,
  0.920994;0.376207;-0.101183;,
  0.147140;-0.981227;-0.124674;,
  0.933800;0.196270;-0.299159;,
  0.378143;0.919117;-0.110598;,
  0.249509;0.957628;-0.143851;,
  0.704402;-0.409688;-0.579632;,
  0.000000;0.999850;-0.017320;,
  0.000000;0.994148;-0.108025;,
  0.000000;-0.994148;0.108029;,
  0.000000;0.857782;-0.514013;,
  0.000000;0.642593;-0.766208;,
  0.000000;-0.999850;0.017326;,
  0.000000;0.017324;0.999850;,
  0.000000;0.973259;-0.229709;,
  0.000000;-0.017328;-0.999850;,
  0.000000;0.902080;-0.431570;,
  -0.920994;0.376208;-0.101183;,
  -0.185231;-0.969324;-0.161557;,
  -0.933801;0.196269;-0.299156;,
  0.000000;-0.999850;0.017326;,
  0.000000;0.999850;-0.017324;,
  0.000000;0.000000;0.000000;,
  -0.378143;0.919117;-0.110598;,
  -0.920995;-0.379484;-0.088089;,
  -0.249509;0.957628;-0.143851;,
  -0.704409;-0.409688;-0.579624;,
  0.000000;0.999850;-0.017320;,
  0.000000;0.994148;-0.108025;,
  0.000000;-0.994148;0.108029;,
  0.000000;-0.017326;-0.999850;,
  0.000000;-0.999850;0.017327;,
  0.000000;0.947799;-0.318868;,
  0.000000;0.183372;-0.983044;,
  0.000000;0.785783;-0.618503;,
  0.000000;-0.999850;0.017325;,
  -0.000000;-0.999850;0.017325;,
  0.000000;0.330819;0.943694;,
  1.000000;0.000000;0.000000;,
  -0.999249;-0.037056;-0.011354;,
  -0.999643;-0.013570;0.023036;,
  -0.999672;0.019345;0.016765;,
  -0.999165;-0.014053;-0.038376;,
  0.999249;-0.037053;-0.011355;,
  -0.999583;0.025161;-0.014190;,
  0.999672;0.019342;0.016759;,
  0.999643;-0.013568;0.023028;,
  0.000000;0.017327;0.999850;,
  0.000000;-0.999850;0.017324;,
  0.000000;0.999850;-0.017324;,
  0.000000;-0.999850;0.017326;,
  -0.000000;-0.999850;0.017326;,
  0.000000;-0.999850;0.017326;,
  0.530627;-0.014686;-0.847478;,
  0.948226;-0.005503;-0.317550;,
  0.000000;0.017326;0.999850;,
  0.000000;-0.994148;0.108025;,
  0.000000;-0.999850;0.017320;,
  0.000000;0.979472;-0.201583;,
  0.000000;-0.999850;0.017328;,
  -0.000001;-0.972892;-0.231260;,
  0.000000;-0.017324;-0.999850;,
  0.000000;1.000000;0.000000;,
  0.000000;-0.017326;-0.999850;,
  -0.948227;-0.005503;-0.317545;,
  -0.530643;-0.014686;-0.847468;,
  0.000000;0.017326;0.999850;,
  0.000000;-0.999850;0.017320;,
  0.000000;0.979472;-0.201583;,
  0.000000;-0.999850;0.017328;,
  0.000000;0.384819;-0.922992;,
  -1.000000;0.000000;0.000000;,
  0.000000;-0.999850;0.017326;,
  0.000000;-0.999850;0.017326;,
  0.000000;-0.999850;0.017326;,
  0.000000;-0.999850;0.017326;,
  0.000000;0.999850;-0.017324;,
  0.000000;0.999850;-0.017324;,
  0.000000;0.902080;-0.431570;,
  -0.000000;-0.999850;0.017327;,
  0.000000;0.017324;0.999850;,
  0.000000;-0.972892;-0.231260;,
  -0.000002;-0.972892;-0.231259;,
  0.000000;0.994934;-0.100526;,
  0.000000;0.983000;-0.183604;,
  0.000000;0.964297;-0.264823;,
  0.000000;0.964297;-0.264823;;
  166;
  3;2,2,6;,
  3;1,1,1;,
  3;4,0,14;,
  3;65,66,65;,
  3;7,7,7;,
  3;5,6,67;,
  3;67,3,3;,
  3;10,10,6;,
  3;1,9,1;,
  3;11,11,68;,
  3;8,66,8;,
  3;69,70,18;,
  3;5,6,12;,
  3;12,3,3;,
  3;16,13,15;,
  3;22,19,21;,
  3;70,71,19;,
  3;4,17,16;,
  3;72,21,20;,
  3;73,16,15;,
  3;74,20,19;,
  3;0,75,13;,
  3;76,15,13;,
  3;72,69,22;,
  3;11,11,11;,
  3;23,23,23;,
  3;7,7,7;,
  3;24,77,24;,
  3;78,78,78;,
  3;79,79,79;,
  3;27,27,25;,
  3;11,11,11;,
  3;80,28,81;,
  3;82,50,80;,
  3;11,11,11;,
  3;26,44,51;,
  3;29,46,44;,
  3;27,27,27;,
  4;32,36,30,30;,
  3;52,52,52;,
  3;32,48,61;,
  4;34,37,38,35;,
  4;33,83,36,84;,
  4;30,30,11,31;,
  4;31,34,35,33;,
  4;11,11,11,11;,
  4;85,85,85,85;,
  4;86,86,87,87;,
  4;86,39,39,86;,
  4;11,11,11,11;,
  4;11,11,11,11;,
  4;7,7,7,7;,
  4;38,88,88,35;,
  4;40,40,41,41;,
  4;88,40,40,88;,
  4;7,7,7,7;,
  4;39,89,89,39;,
  3;43,27,43;,
  3;7,7,7;,
  3;48,32,90;,
  3;7,7,7;,
  3;45,45,45;,
  3;91,91,91;,
  3;27,27,27;,
  4;48,54,54,56;,
  3;92,92,92;,
  3;60,93,63;,
  3;52,52,52;,
  4;53,55,58,57;,
  4;49,94,56,95;,
  4;54,47,7,54;,
  4;47,49,55,53;,
  4;7,7,7,7;,
  4;96,96,96,96;,
  4;86,97,97,86;,
  4;86,86,59,59;,
  4;7,7,7,7;,
  4;7,7,7,7;,
  4;11,11,11,11;,
  4;58,55,98,98;,
  4;40,41,41,40;,
  4;98,98,40,40;,
  4;11,11,11,11;,
  4;59,59,99,99;,
  3;94,49,7;,
  3;33,84,11;,
  3;100,63,64;,
  3;2,6,5;,
  3;1,1,1;,
  3;4,14,17;,
  3;65,65,65;,
  3;7,7,7;,
  3;101,7,7;,
  3;5,67,67;,
  3;67,3,67;,
  3;10,6,5;,
  3;1,1,9;,
  3;11,11,11;,
  3;11,11,11;,
  3;8,8,66;,
  3;69,18,22;,
  3;5,12,12;,
  3;12,3,12;,
  3;13,17,14;,
  3;17,13,16;,
  3;20,21,19;,
  3;18,19,22;,
  3;70,19,18;,
  3;4,16,73;,
  3;72,20,74;,
  3;73,15,76;,
  3;74,19,71;,
  3;0,13,14;,
  3;76,13,75;,
  3;72,22,21;,
  3;11,11,11;,
  3;23,23,23;,
  3;7,7,7;,
  3;24,24,77;,
  3;78,78,78;,
  3;79,79,79;,
  3;27,25,25;,
  3;11,11,11;,
  3;28,80,102;,
  3;103,104,42;,
  3;104,102,80;,
  3;102,104,103;,
  3;82,104,50;,
  3;82,80,50;,
  3;80,50,104;,
  3;105,104,82;,
  3;11,11,11;,
  3;51,106,44;,
  3;26,51,107;,
  3;51,44,106;,
  3;26,51,51;,
  3;108,44,46;,
  3;29,26,46;,
  3;26,29,44;,
  3;27,27,27;,
  3;52,52,52;,
  3;32,61,109;,
  3;43,43,110;,
  3;7,7,7;,
  3;111,90,32;,
  3;48,90,112;,
  3;7,7,7;,
  3;45,45,45;,
  3;45,45,45;,
  3;45,45,45;,
  3;91,91,91;,
  3;91,91,91;,
  3;91,91,91;,
  3;27,27,43;,
  3;52,52,52;,
  3;60,63,100;,
  3;113,62,64;,
  3;113,114,62;,
  3;115,114,113;,
  3;115,116,114;,
  3;52,52,52;,
  3;7,7,49;,
  3;94,7,7;,
  3;11,11,84;,
  3;33,11,11;,
  3;100,64,62;;
 }
 MeshTextureCoords {
  108;
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
