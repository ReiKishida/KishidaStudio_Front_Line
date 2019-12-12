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
 766.14650;265.73130;415.32213;,
 766.14650;-39.68337;415.32213;,
 788.76197;-39.68337;452.44346;,
 788.76197;265.73128;452.44346;,
 968.04083;265.73128;478.01070;,
 916.25656;265.73130;491.00377;,
 916.25656;-39.68339;491.00377;,
 968.04083;-39.68337;478.01070;,
 866.05743;265.73130;491.00377;,
 866.05743;-39.68339;491.00377;,
 993.55197;265.73128;452.44346;,
 993.55197;-39.68337;452.44346;,
 814.27321;-39.68337;478.01070;,
 814.27321;265.73128;478.01070;,
 753.83815;265.73133;370.60189;,
 753.83815;-39.68338;370.60189;,
 753.83815;265.73133;86.69930;,
 753.83815;-39.68337;86.69929;,
 753.83815;265.73133;370.60189;,
 993.55197;265.73128;452.44346;,
 1025.90607;-79.75532;523.10094;,
 647.25958;-79.75527;154.20869;,
 968.04083;265.73128;478.01070;,
 985.60919;-79.75532;563.48634;,
 742.72140;-79.75532;563.48634;,
 824.51866;-79.75529;584.00992;,
 702.42447;-79.75532;523.10094;,
 916.25656;265.73130;491.00377;,
 903.81193;-79.75529;584.00992;,
 666.70147;-79.75531;464.46495;,
 903.81193;-79.75529;584.00992;,
 647.25958;-79.75529;393.82586;,
 647.25958;-79.75529;393.82586;;
 
 34;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,6,5;,
 4;10,4,7,11;,
 4;3,2,12,13;,
 4;14,15,1,0;,
 4;16,17,15,18;,
 3;19,20,11;,
 3;17,21,16;,
 4;13,12,9,8;,
 4;19,22,23,20;,
 4;13,24,25,8;,
 4;3,26,24,13;,
 4;22,27,28,23;,
 4;0,29,26,3;,
 4;8,25,30,5;,
 4;16,21,31,18;,
 4;14,32,29,0;,
 3;26,12,2;,
 3;21,15,17;,
 3;21,31,15;,
 3;15,31,1;,
 3;29,1,31;,
 3;29,2,1;,
 3;24,12,26;,
 3;26,2,29;,
 3;30,9,25;,
 3;25,12,24;,
 3;9,12,25;,
 3;20,6,23;,
 3;23,6,30;,
 3;7,20,11;,
 3;6,20,7;,
 3;6,9,30;;
 
 MeshMaterialList {
  1;
  34;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  37;
  -0.876101;0.000000;-0.482127;,
  -0.990997;0.000000;0.133887;,
  -0.941116;0.111385;-0.319207;,
  -0.979503;0.151919;0.132270;,
  -0.917964;0.000000;0.396664;,
  -0.906946;0.154593;0.391855;,
  0.707883;0.000000;0.706330;,
  -0.786460;0.000000;0.617642;,
  0.704069;0.103667;0.702524;,
  -0.776893;0.155563;0.610113;,
  0.417005;0.314470;0.852769;,
  -0.493547;0.000000;0.869719;,
  0.490658;0.117685;0.863369;,
  -0.487211;0.153274;0.859728;,
  0.111725;0.508992;0.853490;,
  -0.122607;0.000000;0.992455;,
  0.121719;0.128142;0.984258;,
  -0.121288;0.136981;0.983120;,
  -0.857408;0.169553;-0.485905;,
  -0.644018;0.758818;0.097138;,
  -0.589726;0.762497;0.266124;,
  0.909214;0.000000;-0.416330;,
  -0.502503;0.763354;0.405933;,
  0.370293;0.655318;0.658363;,
  -0.351480;0.696594;0.625474;,
  0.074927;0.721294;0.688564;,
  -0.092099;0.736886;0.669714;,
  0.390721;0.771175;0.502619;,
  -0.166069;0.938991;0.301193;,
  -0.274409;0.940938;0.198329;,
  -0.351931;0.936026;0.000000;,
  -0.345924;0.937764;0.030571;,
  -0.351931;0.936026;0.000000;,
  -0.319307;0.939981;0.120331;,
  -0.030124;0.922787;0.384131;,
  0.183219;0.924035;0.335545;,
  0.341314;0.876083;0.340565;;
  34;
  4;5,4,7,9;,
  4;12,16,14,10;,
  4;17,15,14,16;,
  4;8,12,10,6;,
  4;9,7,11,13;,
  4;3,1,4,5;,
  4;2,0,1,3;,
  3;21,21,21;,
  3;0,18,2;,
  4;13,11,15,17;,
  4;8,12,23,27;,
  4;13,24,26,17;,
  4;9,22,24,13;,
  4;12,16,25,23;,
  4;5,20,22,9;,
  4;17,26,25,16;,
  4;2,18,19,3;,
  4;3,19,20,5;,
  3;22,28,29;,
  3;30,31,32;,
  3;30,19,31;,
  3;31,19,33;,
  3;20,33,19;,
  3;20,29,33;,
  3;24,28,22;,
  3;22,29,20;,
  3;25,34,26;,
  3;26,28,24;,
  3;34,28,26;,
  3;27,35,23;,
  3;23,35,25;,
  3;36,27,36;,
  3;14,27,10;,
  3;14,34,25;;
 }
 MeshTextureCoords {
  33;
  0.016130;0.135300;,
  0.018370;0.654590;,
  0.018370;0.610350;,
  0.016130;0.246260;,
  0.016130;0.691410;,
  0.016130;0.569820;,
  0.018370;0.481330;,
  0.018370;0.432850;,
  0.016130;0.456790;,
  0.018370;0.526400;,
  0.016130;0.780350;,
  0.018370;0.397390;,
  0.018370;0.574880;,
  0.016130;0.335190;,
  0.016130;0.013350;,
  0.018370;0.703210;,
  0.979110;0.000080;,
  0.018370;1.003830;,
  0.000080;0.000080;,
  0.446600;0.812620;,
  0.446600;0.812620;,
  0.979110;0.000080;,
  0.459900;0.828350;,
  0.459900;0.828350;,
  0.016130;0.335190;,
  0.016130;0.456790;,
  0.016130;0.246260;,
  0.486910;0.836340;,
  0.486910;0.836340;,
  0.016130;0.135300;,
  0.016130;0.569820;,
  0.000080;0.000080;,
  0.016130;0.013350;;
 }
}
