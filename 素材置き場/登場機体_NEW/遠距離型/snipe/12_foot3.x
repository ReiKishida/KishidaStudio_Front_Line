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
 44;
 -0.80409;0.27260;0.88839;,
 0.92016;-1.28358;-0.77180;,
 -0.77752;-1.09060;-2.71589;,
 -2.50178;0.46558;-1.05571;,
 -4.54519;-8.54295;-0.16550;,
 -4.77001;-9.60026;-0.07413;,
 -5.97086;-9.74219;1.49498;,
 -5.89903;-7.82957;1.08756;,
 -1.42400;-2.36932;-2.27828;,
 -5.02759;-0.97307;1.00716;,
 -1.63221;-1.35901;4.89535;,
 -2.50366;-8.21552;4.97575;,
 -1.14982;-8.92890;3.72270;,
 1.97137;-2.75527;1.60991;,
 0.27368;-2.56230;-0.33418;,
 -3.52253;-8.74124;2.36754;,
 -4.02605;-1.17153;3.55863;,
 -4.89750;-8.02803;3.63903;,
 -3.10221;-10.06826;4.77999;,
 -4.36253;-13.19239;4.60138;,
 -2.18144;-13.10330;2.70558;,
 -1.37463;-9.98621;3.81406;,
 -3.74735;-9.79855;2.45891;,
 -5.23269;-9.91071;3.74486;,
 -3.99619;-15.58464;2.10188;,
 -4.61973;-15.61855;2.64303;,
 -5.00555;-15.57469;2.20121;,
 -3.13831;-12.99453;1.60983;,
 -4.09518;-12.88577;0.51408;,
 -5.31940;-13.08363;3.50563;,
 -6.27626;-12.97486;2.40988;,
 0.89359;0.07963;2.83248;,
 1.15496;1.76128;2.03775;,
 2.61785;-1.47655;1.17230;,
 0.00392;4.99443;0.65148;,
 -2.01232;2.11568;2.12642;,
 -0.31464;1.92271;4.07052;,
 1.21500;3.49813;2.37076;,
 -3.71001;2.30866;0.18233;,
 -2.24042;2.14723;-1.85044;,
 -1.66761;3.82580;-0.93023;,
 -4.23392;-15.66240;3.08484;,
 -4.38201;-15.54078;1.66006;,
 -3.61037;-15.62849;2.54370;;
 
 48;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,4,7,9;,
 4;10,11,12,13;,
 4;14,15,4,8;,
 4;16,17,11,10;,
 4;18,19,20,21;,
 4;11,18,21,12;,
 4;15,22,5,4;,
 4;17,23,18,11;,
 3;24,25,26;,
 4;22,27,28,5;,
 4;23,29,19,18;,
 4;5,28,30,6;,
 3;31,0,32;,
 4;1,14,8,2;,
 4;31,10,13,33;,
 4;2,8,9,3;,
 4;34,35,36,37;,
 4;31,32,36,10;,
 4;16,35,38,9;,
 4;9,38,39,3;,
 4;10,36,35,16;,
 4;40,38,35,34;,
 4;33,13,14,1;,
 3;0,39,40;,
 4;6,30,29,23;,
 4;21,20,27,22;,
 4;29,25,41,19;,
 4;7,6,23,17;,
 4;12,21,22,15;,
 4;9,7,17,16;,
 4;13,12,15,14;,
 4;31,33,1,0;,
 4;27,24,42,28;,
 3;32,37,36;,
 3;38,40,39;,
 4;20,43,24,27;,
 4;28,42,26,30;,
 4;19,41,43,20;,
 4;30,26,25,29;,
 3;25,43,41;,
 3;24,26,42;,
 3;25,24,43;,
 3;34,37,0;,
 3;32,0,37;,
 3;39,0,3;,
 3;0,40,34;;
 
 MeshMaterialList {
  1;
  48;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  76;
  0.670445;-0.399636;-0.625135;,
  0.655928;-0.074558;0.751132;,
  0.452780;0.835066;-0.312498;,
  -0.692906;0.030801;-0.720369;,
  0.616177;-0.118003;0.778718;,
  -0.718900;-0.111118;-0.686175;,
  0.564975;-0.257056;0.784044;,
  -0.667037;-0.228592;-0.709089;,
  0.583300;-0.370717;0.722724;,
  0.802410;-0.069292;-0.592737;,
  -0.686340;0.037936;-0.726291;,
  -0.655928;0.074559;-0.751132;,
  0.655928;-0.074558;0.751132;,
  0.686339;-0.037936;0.726291;,
  -0.694134;0.713294;-0.096901;,
  -0.355205;0.724909;0.590201;,
  -0.445297;0.616887;0.648968;,
  -0.731376;0.671778;0.117488;,
  0.726746;0.180720;-0.662707;,
  0.452779;0.835066;-0.312500;,
  -0.742355;0.116400;0.659818;,
  0.672324;-0.394653;-0.626282;,
  -0.745005;0.096032;0.660110;,
  0.753897;0.015454;-0.656810;,
  -0.750345;-0.172651;0.638102;,
  0.748616;-0.063096;-0.659995;,
  -0.698085;0.318596;0.641229;,
  0.608718;-0.536178;-0.584786;,
  -0.463408;0.745723;0.478696;,
  -0.549447;0.635152;0.542853;,
  0.572504;-0.128156;-0.809824;,
  -0.738321;0.252280;-0.625489;,
  -0.018000;-0.996372;-0.083177;,
  -0.711227;-0.394335;0.581942;,
  0.688487;-0.348832;-0.635848;,
  -0.018004;-0.996372;-0.083180;,
  0.688486;-0.348830;-0.635849;,
  -0.018008;-0.996372;-0.083185;,
  0.806249;-0.401181;-0.434760;,
  0.651512;-0.539982;-0.532871;,
  -0.468485;0.083279;0.879538;,
  -0.517524;0.290883;0.804709;,
  0.893444;-0.003594;-0.449161;,
  -0.531995;0.071367;0.843735;,
  -0.679742;-0.179339;0.711188;,
  0.619321;-0.515713;-0.592014;,
  0.619321;-0.515713;-0.592014;,
  0.700850;-0.019525;0.713042;,
  -0.655928;0.074559;-0.751132;,
  -0.700850;0.019524;-0.713042;,
  0.773111;-0.052538;0.632091;,
  -0.836693;0.327162;0.439214;,
  -0.741218;-0.035637;-0.670318;,
  0.619321;-0.515713;-0.592014;,
  0.563160;-0.529939;-0.634046;,
  0.792500;-0.222435;-0.567861;,
  0.811869;-0.097337;-0.575669;,
  -0.898639;0.113043;0.423875;,
  -0.813166;-0.164173;0.558398;,
  0.566890;0.033529;-0.823111;,
  0.689240;-0.056429;-0.722332;,
  -0.711229;-0.394334;0.581940;,
  -0.923892;0.135044;0.358032;,
  0.501649;-0.366554;-0.783573;,
  0.452778;0.835066;-0.312501;,
  0.688487;-0.348831;-0.635848;,
  0.797078;-0.140174;0.587383;,
  -0.628399;-0.248499;-0.737132;,
  0.614374;-0.389765;0.686023;,
  -0.711231;-0.394334;0.581937;,
  -0.017997;-0.996373;-0.083172;,
  -0.017997;-0.996373;-0.083173;,
  -0.018008;-0.996371;-0.083185;,
  0.735782;-0.158938;-0.658303;,
  0.592164;-0.000345;-0.805817;,
  0.670445;-0.399633;-0.625137;;
  48;
  4;18,19,2,2;,
  4;3,5,5,3;,
  4;11,3,3,10;,
  4;13,4,4,12;,
  4;27,21,38,39;,
  4;26,20,40,41;,
  4;6,8,8,6;,
  4;4,6,6,4;,
  4;21,23,42,38;,
  4;20,22,43,40;,
  3;35,32,37;,
  4;23,25,9,42;,
  4;22,24,44,43;,
  4;5,7,7,5;,
  3;0,0,30;,
  4;45,27,39,46;,
  4;47,13,12,1;,
  4;48,11,10,49;,
  4;28,29,16,15;,
  4;47,50,50,13;,
  4;26,29,17,51;,
  4;10,52,31,49;,
  4;41,16,29,26;,
  4;14,17,29,28;,
  4;53,54,27,45;,
  3;18,55,56;,
  4;57,58,24,22;,
  4;59,60,25,23;,
  4;24,61,33,44;,
  4;62,57,22,20;,
  4;63,59,23,21;,
  4;51,62,20,26;,
  4;54,63,21,27;,
  4;64,64,19,18;,
  4;25,65,36,9;,
  3;50,66,50;,
  3;17,14,31;,
  4;60,34,65,25;,
  4;7,67,67,7;,
  4;8,68,68,8;,
  4;58,69,61,24;,
  3;32,70,71;,
  3;35,37,72;,
  3;32,35,70;,
  3;73,74,18;,
  3;30,18,74;,
  3;55,18,75;,
  3;18,56,73;;
 }
 MeshTextureCoords {
  44;
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
