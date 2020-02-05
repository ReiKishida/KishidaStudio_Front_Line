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
 -0.57667;-0.60704;2.19334;,
 -3.77906;-0.60704;2.19334;,
 -3.77906;-0.60704;5.29557;,
 -0.57667;-4.14546;2.19334;,
 -3.77906;-4.14546;2.19334;,
 -0.57667;-1.95589;6.41823;,
 -0.57667;-4.14546;6.41823;,
 -0.61575;-3.11933;5.35822;,
 -3.77906;-1.95589;6.41823;,
 -0.57667;-0.60704;5.29557;,
 -3.77906;-4.14546;6.41823;,
 3.89198;-0.60704;2.19334;,
 0.68960;-0.60704;2.19334;,
 0.68960;-0.60704;5.29557;,
 3.89198;-4.14546;2.19334;,
 0.68960;-4.14546;2.19334;,
 3.89199;-0.60704;5.29557;,
 3.89199;-4.14546;6.41823;,
 0.72868;-1.54006;3.47257;,
 0.68960;-1.95588;6.41823;,
 0.68960;-4.14546;6.41823;,
 -0.61575;-1.54007;4.85715;,
 -0.61575;-3.11933;3.47257;,
 -0.61575;-1.54007;3.47257;,
 0.72868;-2.14208;5.35822;,
 0.72868;-1.54006;4.85715;,
 0.72868;-3.11933;3.47257;,
 -0.61575;-2.14208;5.35822;,
 0.72868;-3.11933;5.35822;,
 2.09504;-0.87952;4.48441;,
 2.09504;-3.70189;4.48441;,
 2.09504;-3.70189;2.61304;,
 2.09504;-0.87952;2.61304;,
 -1.12704;-3.70189;2.61304;,
 -1.12704;-0.87952;2.61304;,
 -1.12704;-3.70189;4.48441;,
 -1.12704;-0.87952;4.48441;,
 2.23215;-0.21359;1.84500;,
 2.23215;-4.15061;1.84500;,
 3.79630;-4.15061;1.84500;,
 3.79630;-4.15061;-3.51901;,
 3.79630;-4.15061;-6.53513;,
 3.79630;-1.44660;-6.53513;,
 2.23215;-4.15061;-6.53513;,
 0.11379;-4.15061;1.84500;,
 -3.56871;-4.15061;-3.51901;,
 3.79630;-0.21359;1.84500;,
 3.79630;-0.21359;-3.51901;,
 0.11379;-0.21359;1.84500;,
 2.23215;-1.44660;-6.53513;,
 -3.56871;-0.21359;-3.51901;,
 1.23691;-3.79421;-8.09196;,
 1.49054;-3.84183;-7.91418;,
 1.49054;-3.84183;-6.71291;,
 1.49054;-4.15061;-6.53513;,
 0.11379;-4.15061;-6.53513;,
 0.11379;-4.15061;-6.53513;,
 -1.00932;-3.79421;-8.09196;,
 -1.00932;-3.79421;-9.98469;,
 1.49054;-1.75537;-6.71291;,
 4.01144;-1.75537;-6.71291;,
 4.01144;-1.75537;-7.91418;,
 1.49054;-1.75537;-7.91418;,
 1.23691;-1.80300;-8.09196;,
 1.49054;-1.44660;-6.53513;,
 4.01144;-3.84183;-6.71291;,
 4.01144;-3.84183;-7.91418;,
 1.49054;-4.29068;-10.64502;,
 4.01144;-4.29068;-10.64502;,
 4.01144;-4.29068;-13.99274;,
 4.01144;-2.47581;-12.22933;,
 4.01144;-2.20421;-10.64502;,
 1.49054;-2.20421;-10.64502;,
 1.49054;-2.47581;-12.22933;,
 1.49054;-4.29068;-13.99274;,
 -3.56871;-4.15061;1.84500;,
 -2.00457;-4.15061;1.84500;,
 -2.00457;-0.21359;1.84500;,
 -3.56871;-1.44660;-6.53513;,
 -1.26295;-4.15061;-6.53513;,
 -3.56871;-0.21359;1.84500;,
 -2.00457;-1.44660;-6.53513;,
 -3.56871;-4.15061;-6.53513;,
 -1.26295;-3.84183;-6.71291;,
 -1.26295;-3.84183;-7.91418;,
 -2.00457;-4.15061;-6.53513;,
 1.23691;-3.79421;-9.98469;,
 -1.00932;-2.50135;-9.98469;,
 -1.00932;-1.80300;-8.09196;,
 -1.26295;-1.44660;-6.53513;,
 -1.26295;-1.75537;-6.71291;,
 -1.26295;-1.75537;-7.91418;,
 -3.78385;-1.75537;-7.91418;,
 -3.78385;-1.75537;-6.71290;,
 -3.78385;-3.84183;-6.71291;,
 -3.78385;-3.84183;-7.91418;,
 -3.78385;-4.29068;-10.64502;,
 -1.26295;-4.29068;-10.64502;,
 -3.78385;-2.20421;-10.64502;,
 -3.78385;-2.47581;-12.22933;,
 -3.78385;-4.29068;-13.99274;,
 -1.26295;-2.20421;-10.64502;,
 -1.26295;-2.47581;-12.22933;,
 -1.26295;-4.29068;-13.99274;,
 1.23691;-2.50135;-9.98469;,
 1.23691;-1.80300;-9.43394;,
 -1.00932;-1.80300;-9.43394;,
 3.89199;-1.95588;6.41823;;
 
 166;
 3;0,1,2;,
 3;0,3,4;,
 3;5,6,7;,
 3;4,3,6;,
 3;8,2,4;,
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
 3;12,15,26;,
 3;5,27,21;,
 3;19,24,28;,
 3;9,21,23;,
 3;20,28,26;,
 3;6,3,22;,
 3;0,23,22;,
 3;19,13,25;,
 3;29,30,31;,
 3;32,31,33;,
 3;34,33,35;,
 3;36,35,30;,
 3;33,31,30;,
 3;32,34,36;,
 3;37,38,39;,
 3;40,41,42;,
 3;40,43,41;,
 3;40,44,45;,
 3;46,39,40;,
 3;47,48,37;,
 3;47,49,50;,
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
 3;45,50,78;,
 3;57,56,79;,
 3;45,75,80;,
 3;81,82,78;,
 3;43,42,41;,
 3;76,44,48;,
 4;57,79,83,84;,
 3;55,85,79;,
 3;86,58,87;,
 3;81,88,89;,
 4;90,91,92,93;,
 4;88,57,84,91;,
 4;79,89,90,83;,
 4;89,88,91,90;,
 4;94,93,92,95;,
 4;83,90,93,94;,
 4;84,83,94,95;,
 4;84,95,96,97;,
 4;96,98,99,100;,
 4;95,92,98,96;,
 4;91,84,97,101;,
 4;92,91,101,98;,
 4;102,103,100,99;,
 4;98,101,102,99;,
 4;101,97,103,102;,
 4;97,96,100,103;,
 3;57,87,58;,
 3;63,104,105;,
 3;104,87,106;,
 3;0,2,9;,
 3;0,4,1;,
 3;5,7,27;,
 3;4,6,10;,
 3;10,8,4;,
 3;4,2,1;,
 3;9,8,5;,
 3;8,6,5;,
 3;11,13,16;,
 3;11,15,12;,
 3;14,107,17;,
 3;107,14,16;,
 3;15,17,20;,
 3;13,18,25;,
 3;16,19,107;,
 3;19,17,107;,
 3;22,27,7;,
 3;27,22,21;,
 3;28,24,26;,
 3;26,25,18;,
 3;12,26,18;,
 3;5,21,9;,
 3;19,28,20;,
 3;9,23,0;,
 3;20,26,15;,
 3;6,22,7;,
 3;0,22,3;,
 3;19,25,24;,
 3;29,31,32;,
 3;32,33,34;,
 3;34,35,36;,
 3;36,30,29;,
 3;33,30,35;,
 3;32,36,29;,
 3;37,39,46;,
 3;40,42,47;,
 3;43,40,55;,
 3;55,45,85;,
 3;85,45,82;,
 3;45,55,40;,
 3;76,45,44;,
 3;44,40,38;,
 3;76,75,45;,
 3;38,40,39;,
 3;46,40,47;,
 3;80,77,50;,
 3;50,48,47;,
 3;47,37,46;,
 3;48,50,77;,
 3;78,50,81;,
 3;81,50,49;,
 3;49,47,42;,
 3;48,38,37;,
 3;55,54,43;,
 3;51,58,86;,
 3;75,77,80;,
 3;45,78,82;,
 3;54,56,51;,
 3;51,56,57;,
 3;45,80,50;,
 3;82,81,85;,
 3;85,81,79;,
 3;79,81,89;,
 3;42,43,49;,
 3;49,43,64;,
 3;64,43,54;,
 3;76,48,77;,
 3;55,79,56;,
 3;86,87,104;,
 3;64,81,49;,
 3;63,88,64;,
 3;64,88,81;,
 3;106,63,105;,
 3;88,63,106;,
 3;106,87,88;,
 3;88,87,57;,
 3;86,104,51;,
 3;51,104,63;,
 3;104,106,105;;
 
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
  85;
  0.999602;0.025379;-0.012285;,
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.999315;-0.013375;-0.034493;,
  0.000000;0.829085;0.559122;,
  0.000000;0.960128;0.279561;,
  -1.000000;0.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  1.000000;-0.000001;-0.000002;,
  1.000000;-0.000001;-0.000002;,
  0.000000;0.960128;0.279560;,
  0.000000;0.340143;0.940374;,
  0.999903;0.010876;0.008727;,
  0.999785;0.009517;-0.018427;,
  0.999752;-0.020931;0.007636;,
  0.999735;-0.019074;-0.012858;,
  0.999829;-0.006689;-0.017249;,
  -0.999752;-0.020931;0.007636;,
  -0.999903;0.010876;0.008727;,
  -0.999785;0.009517;-0.018427;,
  -0.999829;-0.006688;-0.017249;,
  -0.999735;-0.019074;-0.012858;,
  1.000000;0.000000;-0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.988049;-0.154139;,
  0.000000;0.925639;-0.378407;,
  0.920978;-0.380200;-0.085130;,
  0.920974;0.380210;-0.085129;,
  0.146912;-0.980933;-0.127232;,
  0.938703;0.201017;-0.280050;,
  0.377632;0.922043;-0.085031;,
  0.249232;0.961691;-0.114164;,
  0.726675;-0.400587;-0.558098;,
  0.000000;0.996685;-0.081362;,
  0.000000;-0.996685;0.081363;,
  0.000000;0.884778;-0.466013;,
  0.000000;0.696866;-0.717202;,
  0.000000;0.981234;-0.192822;,
  0.000000;0.000000;0.000000;,
  -0.375732;-0.903770;-0.205001;,
  0.000000;-0.974783;0.223154;,
  -0.184999;-0.969590;-0.160217;,
  -0.377632;0.922043;-0.085031;,
  -0.920978;-0.380200;-0.085130;,
  -0.249232;0.961691;-0.114164;,
  -0.726675;-0.400587;-0.558098;,
  0.000000;0.996685;-0.081362;,
  0.000000;-0.996685;0.081363;,
  0.000000;0.217034;-0.976164;,
  0.000000;0.899790;-0.436324;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.340141;0.940374;,
  1.000000;-0.000000;-0.000002;,
  1.000000;0.000000;-0.000003;,
  -0.999264;-0.036820;-0.010714;,
  -0.999695;-0.013954;0.020363;,
  0.000000;0.829087;0.559120;,
  -0.999702;0.019034;0.015272;,
  -0.999316;-0.013375;-0.034493;,
  0.999264;-0.036820;-0.010714;,
  -0.999602;0.025379;-0.012285;,
  0.999702;0.019034;0.015272;,
  0.999695;-0.013954;0.020363;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.573980;0.000000;-0.818869;,
  0.952939;0.000000;-0.303162;,
  0.000000;0.986197;-0.165576;,
  0.000000;-0.974783;-0.223154;,
  -0.308494;0.886336;-0.345312;,
  -0.952939;0.000000;-0.303162;,
  -0.573980;0.000000;-0.818869;,
  -0.920974;0.380210;-0.085129;,
  0.000002;-0.000002;1.000000;,
  0.000000;0.986197;-0.165576;,
  0.000000;0.434068;-0.900880;,
  1.000000;0.000000;-0.000002;,
  0.000000;0.944809;-0.327623;,
  0.000000;0.925639;-0.378407;,
  0.000000;-0.974783;-0.223154;,
  0.000000;0.997198;-0.074805;,
  0.000000;0.974783;-0.223154;;
  166;
  3;2,5,8;,
  3;4,1,4;,
  3;6,0,16;,
  3;10,53,10;,
  3;9,9,9;,
  3;7,8,54;,
  3;54,3,3;,
  3;2,5,13;,
  3;4,4,4;,
  3;12,55,56;,
  3;10,10,10;,
  3;57,58,20;,
  3;59,13,14;,
  3;14,3,3;,
  3;18,15,17;,
  3;23,24,21;,
  3;58,60,21;,
  3;6,19,18;,
  3;61,23,22;,
  3;62,18,17;,
  3;63,22,21;,
  3;0,64,15;,
  3;65,17,15;,
  3;61,57,24;,
  3;26,25,26;,
  3;4,1,4;,
  3;9,66,9;,
  3;3,67,3;,
  3;10,10,10;,
  3;2,2,2;,
  3;3,3,3;,
  3;26,26,26;,
  3;10,10,10;,
  3;10,10,10;,
  3;26,26,26;,
  3;27,2,2;,
  3;27,28,40;,
  3;3,3,3;,
  4;31,35,29,29;,
  3;41,41,41;,
  3;31,44,10;,
  4;33,2,36,34;,
  4;32,68,35,69;,
  4;29,29,26,30;,
  4;30,33,34,32;,
  4;26,26,26,26;,
  4;3,3,3,3;,
  4;37,37,10,10;,
  4;37,37,37,37;,
  4;26,26,26,26;,
  4;26,26,26,26;,
  4;9,9,9,9;,
  4;36,70,70,34;,
  4;38,38,39,39;,
  4;70,38,38,70;,
  4;9,9,9,9;,
  4;37,10,10,37;,
  3;3,3,3;,
  3;9,9,9;,
  3;44,71,42;,
  3;9,9,9;,
  3;4,4,4;,
  3;4,4,4;,
  3;3,3,3;,
  4;44,42,46,48;,
  3;41,41,41;,
  3;4,4,51;,
  3;43,43,43;,
  4;45,47,49,2;,
  4;72,73,48,74;,
  4;9,75,9,46;,
  4;75,72,47,45;,
  4;9,9,9,9;,
  4;76,76,76,76;,
  4;50,10,10,50;,
  4;50,50,50,50;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;26,26,26,26;,
  4;49,47,77,77;,
  4;38,39,39,38;,
  4;77,77,38,38;,
  4;26,26,26,26;,
  4;50,50,10,10;,
  3;73,9,66;,
  3;32,26,26;,
  3;78,51,52;,
  3;2,8,7;,
  3;4,4,1;,
  3;6,16,19;,
  3;10,10,53;,
  3;9,9,9;,
  3;9,9,66;,
  3;7,54,54;,
  3;54,3,54;,
  3;2,13,59;,
  3;4,4,4;,
  3;55,11,79;,
  3;11,55,12;,
  3;10,10,53;,
  3;57,20,24;,
  3;59,14,14;,
  3;14,3,14;,
  3;15,19,16;,
  3;19,15,18;,
  3;22,23,21;,
  3;21,24,20;,
  3;58,21,20;,
  3;6,18,62;,
  3;61,22,63;,
  3;62,17,65;,
  3;63,21,60;,
  3;0,15,16;,
  3;65,15,64;,
  3;61,24,23;,
  3;26,26,25;,
  3;4,4,1;,
  3;9,9,66;,
  3;3,3,67;,
  3;10,10,10;,
  3;2,2,2;,
  3;3,3,3;,
  3;26,26,26;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,53,10;,
  3;10,10,10;,
  3;26,26,26;,
  3;5,2,40;,
  3;40,2,27;,
  3;27,2,2;,
  3;2,40,2;,
  3;28,40,80;,
  3;80,40,28;,
  3;28,27,81;,
  3;3,3,3;,
  3;41,41,41;,
  3;31,10,10;,
  3;3,3,3;,
  3;9,9,9;,
  3;82,71,31;,
  3;31,71,44;,
  3;9,9,9;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;3,3,3;,
  3;41,41,41;,
  3;4,51,78;,
  3;41,41,41;,
  3;83,72,84;,
  3;84,72,80;,
  3;52,83,52;,
  3;72,83,52;,
  3;9,9,9;,
  3;9,9,73;,
  3;25,26,69;,
  3;69,26,32;,
  3;78,52,52;;
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
