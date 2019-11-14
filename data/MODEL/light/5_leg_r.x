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
 192;
 0.03615;-1.26813;0.30351;,
 0.03616;0.03270;-1.40783;,
 -2.33373;-0.20449;-1.09580;,
 -3.03213;-1.24407;0.27186;,
 -2.51152;-7.36367;-3.05808;,
 -2.45269;-7.81700;-3.72907;,
 -2.33373;-8.23935;-3.17343;,
 -2.33373;-7.82043;-2.45718;,
 -2.33372;-5.54383;-5.45223;,
 -3.07021;-4.94193;-6.24408;,
 0.04247;-4.59894;-6.69531;,
 0.03616;-5.24574;-5.84439;,
 0.04247;-0.24865;-2.90368;,
 3.15515;-0.40368;-2.67966;,
 3.15515;-1.49628;-3.44457;,
 0.04247;-1.30273;-3.69919;,
 2.24465;-6.74217;-0.90851;,
 0.03616;-7.98935;-2.23494;,
 2.40604;-7.82043;-2.45717;,
 -2.33373;-6.81519;-1.08376;,
 -3.07021;-6.72763;-0.92765;,
 -3.07021;-7.82043;-2.45718;,
 0.03616;-6.82793;-7.33326;,
 0.03616;-7.93744;-8.17662;,
 -2.33372;-8.32415;-7.66788;,
 -2.33372;-7.22819;-6.80669;,
 2.90126;-6.54822;-4.13086;,
 2.73738;-7.06297;-4.72104;,
 2.40604;-6.10334;-5.98351;,
 2.40604;-5.54383;-5.45222;,
 0.03616;-5.76159;-6.43312;,
 -2.33372;-6.10334;-5.98351;,
 0.03616;-8.38288;-2.98459;,
 2.40604;-8.23935;-3.17342;,
 -2.42310;-9.84901;-5.66180;,
 -2.53038;-12.34865;-7.17367;,
 -2.33372;-12.77100;-6.61803;,
 -2.33372;-10.23045;-5.15999;,
 0.03616;-9.25775;-4.13665;,
 0.03616;-10.34379;-5.01088;,
 2.40604;-10.23045;-5.15998;,
 2.40604;-9.14166;-4.28937;,
 -2.43852;-8.75650;-4.79608;,
 -2.33372;-9.14166;-4.28937;,
 2.69793;-8.06888;-5.70070;,
 2.65499;-9.16803;-6.55769;,
 2.40604;-8.32415;-7.66787;,
 2.40604;-7.22819;-6.80669;,
 0.03616;-11.58231;-8.18185;,
 -2.88146;-11.59462;-8.16565;,
 -2.33372;-10.57882;-9.50203;,
 0.03616;-10.25008;-9.93451;,
 2.95378;-11.59462;-8.16565;,
 2.40604;-10.57882;-9.50202;,
 -2.49775;-2.69499;2.18065;,
 -3.07021;-2.69499;2.18065;,
 -3.07021;-4.66082;0.71864;,
 -2.49775;-4.66082;0.71864;,
 -3.07021;-1.49628;-3.44457;,
 -3.26255;0.04691;-1.44482;,
 -3.07021;-0.40368;-2.67966;,
 -2.17234;-6.74217;-0.90852;,
 -2.30977;-7.11381;0.03035;,
 -2.48335;-5.67795;1.20626;,
 -2.17245;-4.79931;0.63966;,
 -3.79264;-1.24407;0.27186;,
 -3.39955;-2.17407;1.49534;,
 -2.58447;-1.96018;1.21396;,
 -2.54873;-0.90332;2.96094;,
 0.03694;-1.17580;3.46061;,
 0.03615;-2.20423;1.53502;,
 -3.30374;-7.36367;-3.05808;,
 -3.53902;-6.54822;-4.13087;,
 -2.82895;-6.54822;-4.13087;,
 3.26958;0.04076;-1.43564;,
 3.61063;-3.20214;-1.20037;,
 -2.49775;-4.78447;0.62014;,
 -3.07021;-4.78447;0.62014;,
 -2.49775;-6.72763;-0.92765;,
 -2.62561;-8.06888;-5.70070;,
 -2.66507;-7.06297;-4.72105;,
 2.52500;-7.81700;-3.72906;,
 2.51083;-8.75651;-4.79607;,
 -2.17241;-2.70971;2.20002;,
 -2.48330;-1.79048;4.12807;,
 -2.54369;-1.27682;3.45232;,
 -2.37185;-2.61580;2.07647;,
 3.15515;-4.94193;-6.24407;,
 3.61392;-6.54822;-4.13086;,
 2.49542;-9.84901;-5.66180;,
 2.60269;-12.34865;-7.17366;,
 0.03616;-12.43623;-7.05845;,
 -2.58267;-9.16803;-6.55769;,
 2.58384;-7.36367;-3.05807;,
 3.38868;-7.36367;-3.05807;,
 3.15515;-7.82043;-2.45717;,
 2.43190;-2.61459;2.07489;,
 3.15515;-2.69499;2.18066;,
 3.37528;-2.17407;1.49535;,
 2.38366;-7.11381;0.03035;,
 0.03695;-8.40447;-0.46181;,
 0.03615;-7.35875;-0.96415;,
 2.57005;-4.78448;0.62014;,
 3.15515;-4.78448;0.62015;,
 3.15515;-4.66082;0.71864;,
 2.57005;-4.66082;0.71864;,
 2.57005;-2.69499;2.18066;,
 2.40604;-12.77100;-6.61803;,
 2.40604;-11.07390;-4.74150;,
 2.40604;-12.68701;-4.99783;,
 2.40604;-6.81519;-1.08376;,
 3.15515;-6.72763;-0.92764;,
 2.87286;-1.24408;0.27187;,
 2.40604;-0.20449;-1.09579;,
 -3.79563;-3.32688;-1.29745;,
 -3.07021;-1.61708;-3.54683;,
 -3.07021;-3.51533;-5.15369;,
 -3.73030;-5.28707;-2.82281;,
 3.38842;-4.26116;-0.06832;,
 3.40396;-6.21043;-1.60806;,
 0.04247;-1.41461;-3.81319;,
 3.15515;-1.61708;-3.54682;,
 3.15515;-3.51533;-5.15368;,
 0.04247;-3.17264;-5.60452;,
 3.57847;-1.24408;0.27187;,
 3.38743;-4.13711;0.02966;,
 3.65439;-5.28707;-2.82281;,
 -3.37600;-6.21043;-1.60806;,
 -3.79978;-3.20214;-1.20038;,
 -3.40225;-4.13711;0.02966;,
 3.61325;-3.32688;-1.29744;,
 -3.40068;-4.26116;-0.06833;,
 2.62599;0.05783;-1.46111;,
 0.03713;0.16740;-1.62459;,
 2.24472;-2.70971;2.20002;,
 2.57005;-6.72763;-0.92764;,
 2.51201;-2.94517;1.99049;,
 2.51201;-4.66719;0.71380;,
 -2.45110;-4.66719;0.71380;,
 -2.45110;-2.94517;1.99048;,
 2.51201;-4.77511;0.62726;,
 2.51201;-6.47108;-0.73264;,
 -2.21909;-4.78995;0.64678;,
 -2.21901;-6.48562;-0.71351;,
 2.24477;-4.73750;0.68893;,
 2.55724;-4.03809;2.45544;,
 2.55724;-5.67795;1.20626;,
 2.24476;-4.79931;0.63967;,
 -2.46719;-3.82305;-0.34704;,
 -2.46719;-2.10103;0.92964;,
 2.49592;-3.93098;-0.43358;,
 2.49592;-5.62694;-1.79347;,
 -2.23515;-2.11576;0.94901;,
 -2.21906;-2.95989;2.00985;,
 -2.23518;-3.94581;-0.41406;,
 -2.23510;-5.64148;-1.77435;,
 -2.45110;-6.47108;-0.73264;,
 2.30279;-4.78995;0.64678;,
 2.30279;-4.68204;0.73335;,
 2.30272;-6.48562;-0.71351;,
 -2.21909;-4.68204;0.73334;,
 -2.17246;-4.67568;0.73818;,
 2.24477;-4.67568;0.73819;,
 2.30276;-2.95989;2.00985;,
 -2.45110;-4.77511;0.62726;,
 -2.23519;-3.83791;-0.32750;,
 2.28669;-3.94581;-0.41406;,
 2.28662;-5.64148;-1.77434;,
 -2.46719;-3.93097;-0.43358;,
 2.49592;-2.10103;0.92965;,
 2.28667;-2.11576;0.94901;,
 2.28670;-3.83791;-0.32749;,
 2.49592;-3.82305;-0.34704;,
 -2.46719;-5.62694;-1.79348;,
 -2.17246;-4.73749;0.68892;,
 2.59807;-1.27539;3.45044;,
 2.55719;-1.79048;4.12808;,
 -2.48335;-4.03809;2.45544;,
 -2.48336;-2.39823;3.70462;,
 0.03616;-12.91454;-6.42920;,
 0.03695;-7.15738;0.58097;,
 2.57364;-1.96018;1.21396;,
 0.03694;-1.96103;4.49363;,
 0.03694;-2.17208;4.62089;,
 2.55724;-2.39823;3.70462;,
 0.03694;-4.79728;2.77531;,
 2.53327;-0.90332;2.96095;,
 0.03616;-11.21274;-4.61290;,
 0.03616;-12.67854;-4.86708;,
 -2.33372;-12.68701;-4.99784;,
 -2.33372;-11.07390;-4.74150;,
 -2.55007;0.05790;-1.46122;;
 
 226;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 3;16,17,18;,
 3;19,20,21;,
 4;22,23,24,25;,
 4;26,27,28,29;,
 4;11,30,31,8;,
 4;17,32,33,18;,
 4;34,35,36,37;,
 4;38,39,40,41;,
 4;42,34,37,43;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;45,52,53,46;,
 4;23,51,50,24;,
 4;54,55,56,57;,
 3;58,59,60;,
 4;61,62,63,64;,
 3;3,65,66;,
 4;27,44,47,28;,
 4;5,42,43,6;,
 4;32,38,41,33;,
 4;30,22,25,31;,
 3;61,7,17;,
 4;67,68,69,70;,
 4;4,71,72,73;,
 3;14,74,75;,
 4;76,77,20,78;,
 4;31,25,79,80;,
 4;81,82,44,27;,
 4;83,84,85,86;,
 4;29,87,88,26;,
 4;89,90,52,45;,
 4;91,35,49,48;,
 4;82,89,45,44;,
 4;25,24,92,79;,
 4;24,50,49,92;,
 4;93,81,27,26;,
 4;73,72,9,8;,
 4;93,94,95,18;,
 4;8,31,80,73;,
 3;86,85,68;,
 3;96,97,98;,
 4;90,91,48,52;,
 4;11,10,87,29;,
 4;16,99,100,101;,
 4;28,47,22,30;,
 4;6,43,38,32;,
 4;102,103,104,105;,
 4;105,104,97,106;,
 4;107,40,108,109;,
 4;46,53,51,23;,
 4;52,48,51,53;,
 4;43,37,39,38;,
 4;7,6,32,17;,
 4;29,28,30,11;,
 4;47,46,23,22;,
 3;110,95,111;,
 3;3,2,59;,
 4;112,113,1,0;,
 4;114,115,116,117;,
 4;118,103,111,119;,
 4;120,121,122,123;,
 3;113,124,74;,
 4;98,97,104,125;,
 4;60,12,15,58;,
 4;123,10,9,116;,
 4;126,88,87,122;,
 4;127,71,21,20;,
 4;116,9,72,117;,
 4;119,94,88,126;,
 4;66,65,128,129;,
 4;121,130,126,122;,
 4;131,114,117,127;,
 4;122,87,10,123;,
 4;113,132,133,1;,
 4;115,120,123,116;,
 3;106,96,134;,
 3;83,86,54;,
 3;16,110,135;,
 3;78,19,61;,
 4;106,136,137,105;,
 4;57,138,139,54;,
 4;102,140,141,135;,
 4;64,142,143,61;,
 4;144,145,146,147;,
 4;138,148,149,139;,
 4;140,150,151,141;,
 4;139,149,152,153;,
 4;142,154,155,143;,
 4;61,143,156,78;,
 3;144,157,158;,
 4;135,141,159,16;,
 4;83,153,160,161;,
 4;162,158,163,134;,
 4;54,139,153,83;,
 4;134,163,136,106;,
 4;76,164,138,57;,
 4;148,165,152,149;,
 4;150,166,167,151;,
 4;168,154,165,148;,
 4;169,170,171,172;,
 4;136,169,172,137;,
 4;141,151,167,159;,
 4;157,166,171,158;,
 4;153,152,165,160;,
 4;158,171,170,163;,
 4;143,155,173,156;,
 4;164,168,148,138;,
 4;163,170,169,136;,
 4;156,173,168,164;,
 4;159,167,166,157;,
 4;173,155,154,168;,
 4;172,171,166,150;,
 4;78,156,164,76;,
 4;16,159,157,147;,
 4;160,165,154,142;,
 4;137,172,150,140;,
 3;174,160,142;,
 4;105,137,140,102;,
 4;58,15,120,115;,
 4;129,128,114,131;,
 4;14,75,130,121;,
 4;15,14,121,120;,
 4;125,104,103,118;,
 4;128,58,115,114;,
 4;135,111,103,102;,
 4;96,175,176,134;,
 4;57,56,77,76;,
 4;174,177,178,161;,
 4;160,165,148,138;,
 4;164,168,154,142;,
 4;142,160,138,164;,
 4;158,137,172,171;,
 4;140,157,166,150;,
 4;140,137,158,157;,
 4;55,66,129,56;,
 4;77,131,127,20;,
 4;56,129,131,77;,
 4;75,125,118,130;,
 4;111,95,94,119;,
 4;117,72,71,127;,
 4;124,98,125,75;,
 4;130,118,119,126;,
 4;107,179,91,90;,
 4;26,88,94,93;,
 4;18,33,81,93;,
 4;41,40,89,82;,
 4;179,36,35,91;,
 4;40,107,90,89;,
 4;33,41,82,81;,
 4;7,21,71,4;,
 4;80,79,42,5;,
 4;79,92,34,42;,
 4;92,49,35,34;,
 4;73,80,5,4;,
 4;146,180,100,99;,
 4;70,0,3,67;,
 4;181,112,0,70;,
 3;181,98,124;,
 3;86,55,54;,
 3;85,69,68;,
 4;176,182,183,184;,
 4;182,84,178,183;,
 4;180,63,62,100;,
 3;176,175,69;,
 4;185,177,63,180;,
 4;145,185,180,146;,
 4;70,69,186,181;,
 4;134,176,184,162;,
 4;147,146,99,16;,
 4;101,100,62,61;,
 4;161,178,84,83;,
 3;96,181,186;,
 4;184,183,185,145;,
 4;183,178,177,185;,
 4;64,63,177,174;,
 4;162,184,145,144;,
 4;39,187,108,40;,
 3;188,179,109;,
 4;37,36,189,190;,
 4;188,187,190,189;,
 4;109,108,187,188;,
 4;37,190,187,39;,
 3;60,59,191;,
 3;132,74,13;,
 4;12,133,132,13;,
 4;60,191,133,12;,
 4;1,133,191,2;,
 3;17,16,101;,
 3;16,18,110;,
 3;20,19,78;,
 3;19,21,7;,
 3;59,128,65;,
 3;128,59,58;,
 3;3,66,67;,
 3;7,61,19;,
 3;61,17,101;,
 3;124,75,74;,
 3;13,74,14;,
 3;86,68,67;,
 3;97,96,106;,
 3;96,98,181;,
 3;95,110,18;,
 3;110,111,135;,
 3;191,59,2;,
 3;3,59,65;,
 3;124,113,112;,
 3;113,74,132;,
 3;157,144,147;,
 3;144,158,162;,
 3;160,174,161;,
 3;174,142,64;,
 3;181,124,112;,
 3;55,86,66;,
 3;67,66,86;,
 3;69,84,182;,
 3;84,69,85;,
 3;186,69,175;,
 3;176,69,182;,
 3;96,186,175;,
 3;179,189,36;,
 3;189,179,188;,
 3;109,179,107;;
 
 MeshMaterialList {
  1;
  226;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  309;
  0.002826;0.807492;0.589872;,
  -0.003932;0.809944;0.586495;,
  0.973813;0.110592;-0.198638;,
  0.093450;-0.829748;0.550259;,
  -0.980339;-0.171517;0.097559;,
  -0.233276;0.652853;-0.720670;,
  0.976033;0.143082;-0.163972;,
  0.079706;-0.623792;0.777516;,
  -0.992747;-0.063224;0.102257;,
  -0.253259;0.594788;-0.762946;,
  0.969065;0.180435;-0.168393;,
  0.994377;-0.045343;0.095700;,
  -0.994377;-0.045344;0.095703;,
  0.000001;-0.796118;-0.605141;,
  -0.257460;0.594064;-0.762104;,
  0.980194;0.113555;-0.162249;,
  0.083997;-0.708591;0.700602;,
  -0.987064;-0.116049;0.110621;,
  -0.973772;-0.209422;0.088943;,
  0.000000;-0.796117;-0.605143;,
  0.967653;0.099463;-0.231851;,
  0.120003;-0.821192;0.557892;,
  -0.000000;0.796116;0.605145;,
  -0.001672;0.797044;0.603919;,
  0.996686;-0.048263;-0.065487;,
  -0.996685;-0.048262;-0.065489;,
  0.999521;0.030926;-0.001172;,
  -0.999521;0.030926;-0.001173;,
  0.997611;0.064492;0.024752;,
  0.000000;-0.796116;-0.605144;,
  0.999566;-0.004924;-0.029048;,
  -0.999566;-0.004924;-0.029051;,
  0.990996;-0.089239;-0.099815;,
  -0.000000;-0.796116;-0.605144;,
  -0.000000;-0.617954;0.786214;,
  -0.000003;0.814239;0.580530;,
  -0.000056;-0.725568;-0.688150;,
  -0.000001;-0.833395;0.552678;,
  0.000000;0.671377;-0.741116;,
  -0.000001;-0.625784;0.779997;,
  0.000001;0.614832;-0.788658;,
  0.000000;-0.989610;-0.143775;,
  0.000001;-0.796117;-0.605142;,
  -0.000943;-0.624049;0.781385;,
  -0.000001;-0.711104;0.703087;,
  0.000000;0.614789;-0.788692;,
  -0.000001;-0.863458;0.504421;,
  0.000001;-0.796117;-0.605143;,
  0.000001;-0.796116;-0.605144;,
  -0.000000;0.796116;0.605145;,
  0.002264;0.797048;0.603912;,
  0.000001;0.796112;0.605149;,
  0.000000;0.797671;0.603092;,
  0.999972;-0.002228;-0.007107;,
  -0.008799;-0.779914;0.625825;,
  -0.998995;-0.036258;-0.026356;,
  -0.976265;0.114496;-0.183841;,
  0.000001;0.644761;-0.764385;,
  0.160839;0.636366;-0.754433;,
  -0.968438;0.144615;-0.203015;,
  0.000000;-0.610004;0.792399;,
  0.000000;-0.596766;0.802416;,
  -0.000003;-0.609980;0.792417;,
  0.099516;0.632299;-0.768306;,
  0.000000;-0.796114;-0.605147;,
  0.008945;-0.779907;0.625831;,
  0.000000;0.635458;-0.772135;,
  0.999288;0.029007;-0.024114;,
  -0.998728;0.025456;-0.043536;,
  0.013241;-0.810247;0.585939;,
  0.000001;-0.796117;-0.605143;,
  0.000000;-0.796116;-0.605144;,
  0.021981;-0.600225;0.799529;,
  -0.000008;0.796104;0.605160;,
  -0.000002;0.796111;0.605151;,
  -0.543654;-0.593326;0.593637;,
  -0.017744;-0.598677;0.800794;,
  -0.966909;-0.231313;0.107612;,
  0.142213;-0.779348;0.610240;,
  0.019308;-0.740495;0.671784;,
  0.061716;-0.840929;0.537615;,
  -0.014990;-0.741338;0.670964;,
  -0.971400;0.112660;-0.209020;,
  -0.061620;-0.840929;0.537625;,
  -0.016418;-0.609677;0.792480;,
  0.009215;-0.610151;0.792232;,
  0.013214;-0.609672;0.792544;,
  -0.007423;-0.610144;0.792256;,
  0.032990;-0.601843;0.797933;,
  0.062289;-0.600961;0.796848;,
  0.002323;-0.782479;-0.622672;,
  -0.026631;-0.599562;0.799885;,
  0.079865;-0.611461;0.787234;,
  0.014346;-0.613273;0.789741;,
  -0.003770;-0.615616;0.788038;,
  -0.084910;-0.613481;0.785131;,
  -0.034770;-0.617372;0.785903;,
  -0.022799;-0.615301;0.787963;,
  0.036003;-0.617342;0.785871;,
  0.028980;-0.615200;0.787839;,
  0.116017;-0.591500;0.797915;,
  0.999929;0.007072;-0.009539;,
  0.115173;-0.621495;0.774906;,
  -0.999930;-0.007424;0.009259;,
  -0.002303;0.782478;0.622674;,
  -0.999930;-0.007455;0.009234;,
  -0.104961;-0.607336;0.787481;,
  0.999930;0.007248;-0.009399;,
  0.116245;-0.606571;0.786485;,
  0.952210;0.187726;-0.240946;,
  -0.970268;0.142333;-0.195760;,
  -0.000007;-0.623032;0.782196;,
  0.000000;-0.623057;0.782177;,
  0.123135;0.675353;-0.727142;,
  0.999911;0.013230;0.001394;,
  -0.999908;-0.011053;-0.007924;,
  0.000001;0.680536;-0.732714;,
  -0.016456;-0.623589;0.781579;,
  0.962920;0.149730;-0.224423;,
  0.020359;-0.623561;0.781510;,
  -0.963318;0.148828;-0.223313;,
  0.016932;-0.624678;0.780699;,
  0.047039;-0.624098;0.779929;,
  -0.007933;-0.624748;0.780786;,
  -0.048367;-0.624028;0.779904;,
  -0.999930;-0.007424;0.009259;,
  0.115802;-0.621461;0.774840;,
  -0.104567;-0.622238;0.775813;,
  -0.999930;-0.007456;0.009236;,
  0.976383;-0.072493;0.203522;,
  0.983561;-0.088558;0.157367;,
  0.978459;-0.188476;0.084230;,
  0.986940;-0.119648;0.107857;,
  0.968333;-0.241503;0.063310;,
  -0.978459;-0.188475;0.084231;,
  -0.983561;-0.088559;0.157368;,
  -0.976382;-0.072493;0.203524;,
  -0.986940;-0.119652;0.107856;,
  -0.968334;-0.241497;0.063314;,
  0.000001;-0.796115;-0.605145;,
  0.000001;0.796116;0.605144;,
  0.975260;-0.157688;0.154930;,
  0.980021;-0.116015;0.161556;,
  0.000000;-0.796115;-0.605146;,
  -0.000003;0.796114;0.605147;,
  -0.959347;-0.217230;0.180177;,
  -0.952783;-0.188833;0.237795;,
  0.000000;-0.796115;-0.605146;,
  -0.954430;-0.193545;0.227167;,
  0.978342;-0.122597;0.166786;,
  -0.002742;0.868434;-0.495797;,
  -0.000000;0.796116;0.605144;,
  0.161470;0.857078;-0.489229;,
  -0.000000;-0.335096;0.942184;,
  -0.000001;0.432870;0.901456;,
  0.000000;-0.644450;0.764646;,
  0.990919;-0.134449;-0.001677;,
  0.975264;-0.214240;0.054412;,
  -0.958437;-0.266465;0.101952;,
  -0.665126;-0.279741;0.692353;,
  -0.954858;0.178330;-0.237582;,
  0.284603;-0.321240;0.903220;,
  -0.166880;0.856224;-0.488909;,
  0.344774;-0.680914;-0.646132;,
  -0.952656;0.186869;-0.239846;,
  -0.000001;0.139925;0.990162;,
  -0.000000;-0.988780;0.149382;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;-0.000000;,
  -0.968905;0.141906;-0.202699;,
  -0.026174;0.990737;0.133245;,
  -0.969302;0.144278;-0.199089;,
  -0.000010;0.991183;0.132502;,
  -0.000007;0.806422;-0.591340;,
  0.030433;0.990377;0.135009;,
  0.068812;0.858632;-0.507952;,
  0.988654;0.091930;-0.118796;,
  0.000001;0.721022;-0.692912;,
  -0.215088;0.704145;-0.676696;,
  0.000002;0.618123;-0.786082;,
  -0.241898;0.599765;-0.762736;,
  -0.143872;-0.779759;0.609325;,
  -0.119995;-0.821193;0.557891;,
  0.987794;0.095566;-0.123003;,
  -0.973812;0.110593;-0.198643;,
  -0.980194;0.113555;-0.162251;,
  -0.000000;-0.796115;-0.605145;,
  -0.976033;0.143082;-0.163972;,
  -0.969065;0.180434;-0.168392;,
  -0.997611;0.064493;0.024752;,
  -0.967651;0.099466;-0.231855;,
  -0.990996;-0.089236;-0.099814;,
  -0.992105;-0.120469;0.034843;,
  0.000002;-0.796115;-0.605145;,
  0.000001;-0.796116;-0.605144;,
  0.233277;0.652855;-0.720668;,
  0.257461;0.594063;-0.762104;,
  -0.093452;-0.829747;0.550260;,
  -0.083998;-0.708591;0.700602;,
  0.000000;-0.596768;0.802414;,
  0.992747;-0.063222;0.102254;,
  0.253261;0.594787;-0.762947;,
  0.241901;0.599765;-0.762736;,
  0.000001;-0.796117;-0.605143;,
  -0.079707;-0.623794;0.777514;,
  0.215089;0.704147;-0.676693;,
  -0.013021;-0.810248;0.585942;,
  -0.008660;0.918611;0.395069;,
  0.964270;-0.162251;0.209424;,
  0.959444;-0.203271;0.195314;,
  0.017076;0.949352;0.313749;,
  0.980656;-0.112487;0.160191;,
  0.967326;-0.149555;0.204727;,
  0.966501;-0.154323;0.205088;,
  -0.063270;0.857706;-0.510233;,
  -0.099515;0.632308;-0.768299;,
  0.000001;0.607371;-0.794418;,
  -0.179068;0.597554;-0.781578;,
  -0.160836;0.636366;-0.754433;,
  0.999718;-0.016931;-0.016681;,
  0.985579;0.104093;-0.133408;,
  0.985988;0.107060;-0.127925;,
  -0.960096;-0.243760;0.137099;,
  -0.943069;-0.276560;0.184756;,
  -0.941670;-0.248291;0.227178;,
  -0.979824;0.091540;-0.177666;,
  -0.997140;-0.060505;-0.045288;,
  0.971984;-0.189536;0.139007;,
  -0.952783;-0.180710;0.244025;,
  -0.999986;-0.000619;0.005279;,
  0.986715;0.108641;-0.120793;,
  0.179070;0.597554;-0.781577;,
  -0.123133;0.675362;-0.727134;,
  -0.000004;0.796107;0.605156;,
  -0.000009;0.796119;0.605140;,
  -0.000003;0.796115;0.605146;,
  -0.000004;0.796116;0.605143;,
  -0.043611;-0.623239;0.780815;,
  0.000870;-0.624063;0.781374;,
  -0.036657;-0.595686;0.802381;,
  -0.063067;-0.598543;0.798605;,
  -0.104755;-0.592244;0.798920;,
  -0.999929;-0.007072;0.009539;,
  -0.999930;-0.007248;0.009399;,
  0.002548;0.782510;0.622632;,
  0.999930;0.007403;-0.009283;,
  0.999930;0.007241;-0.009407;,
  -0.999929;-0.007060;0.009553;,
  -0.999930;-0.007258;0.009395;,
  0.999929;0.007082;-0.009531;,
  0.999930;0.007424;-0.009259;,
  -0.002540;-0.782515;-0.622627;,
  0.999930;0.007424;-0.009259;,
  0.999929;0.007406;-0.009283;,
  -0.104006;-0.622267;0.775865;,
  0.035285;-0.623473;0.781048;,
  0.982186;-0.176307;0.065015;,
  -0.002342;0.782479;0.622672;,
  0.002338;-0.782477;-0.622675;,
  0.002620;0.782515;0.622626;,
  -0.002604;-0.782515;-0.622626;,
  -0.933802;-0.213373;0.287202;,
  -0.933837;-0.219671;0.282301;,
  -0.935831;-0.222670;0.273200;,
  0.954327;-0.240771;0.176885;,
  0.999933;0.010770;0.004324;,
  0.034347;-0.954932;-0.294830;,
  0.973770;-0.209432;0.088942;,
  0.980339;-0.171518;0.097558;,
  0.987065;-0.116042;0.110619;,
  -0.034346;-0.954933;-0.294828;,
  0.000000;-0.796114;-0.605147;,
  0.215708;-0.629279;0.746645;,
  0.000000;-0.644393;0.764695;,
  0.187976;-0.632906;0.751063;,
  0.000000;0.796116;0.605145;,
  -0.000003;0.796114;0.605147;,
  -0.000000;0.796114;0.605146;,
  -0.033062;0.794888;0.605855;,
  0.000099;0.796044;0.605238;,
  -0.033085;0.795523;0.605019;,
  0.133475;0.575176;0.807067;,
  -0.284604;-0.321237;0.903221;,
  -0.215708;-0.629279;0.746645;,
  -0.187976;-0.632906;0.751063;,
  0.033361;0.796254;0.604042;,
  -0.249681;-0.598383;0.761313;,
  0.249680;-0.598382;0.761314;,
  0.954433;0.179141;-0.238676;,
  0.971047;0.113600;-0.210151;,
  -0.344880;-0.681218;-0.645755;,
  -0.957990;0.167659;-0.232693;,
  0.991063;-0.102751;0.085062;,
  0.998612;-0.048406;-0.020758;,
  -0.000001;0.430481;0.902600;,
  0.046221;0.139775;0.989104;,
  0.030011;0.430285;0.902194;,
  0.034650;-0.994315;0.100687;,
  -0.000001;-0.163904;0.986476;,
  -0.046223;0.139776;0.989104;,
  -0.058172;-0.163627;0.984806;,
  0.058169;-0.163624;0.984806;,
  -0.030013;0.430290;0.902192;,
  -0.000002;-0.895775;0.444508;,
  0.988975;0.084928;-0.121311;,
  -0.979651;-0.162227;0.118183;,
  -0.033461;0.794954;0.605746;,
  0.033413;0.797798;0.601998;,
  -0.034650;-0.994315;0.100687;;
  226;
  4;49,35,1,23;,
  4;138,134,4,18;,
  4;19,19,47,47;,
  4;173,175,63,66;,
  3;78,46,21;,
  3;83,65,69;,
  4;45,40,9,14;,
  4;32,24,2,20;,
  4;177,38,5,178;,
  4;46,37,3,21;,
  4;135,136,12,8;,
  4;44,39,7,16;,
  4;137,135,8,17;,
  4;30,26,6,15;,
  4;48,29,13,42;,
  4;26,28,10,6;,
  4;40,179,180,9;,
  4;76,61,60,85;,
  3;59,171,169;,
  4;82,82,120,120;,
  3;23,52,144;,
  4;24,30,15,2;,
  4;134,137,17,4;,
  4;37,44,16,3;,
  4;38,45,14,5;,
  3;181,182,46;,
  4;152,152,150,150;,
  4;147,147,33,33;,
  3;183,176,67;,
  4;119,112,65,81;,
  4;184,185,31,25;,
  4;131,132,30,24;,
  4;75,159,158,77;,
  4;70,70,71,71;,
  4;130,129,28,26;,
  4;139,186,29,48;,
  4;132,130,26,30;,
  4;185,187,27,31;,
  4;187,188,189,27;,
  4;133,131,24,32;,
  4;33,33,19,19;,
  4;143,143,64,64;,
  4;190,184,25,191;,
  3;77,158,192;,
  3;74,51,140;,
  4;193,139,48,194;,
  4;47,47,70,70;,
  4;163,163,36,36;,
  4;195,196,45,38;,
  4;197,198,44,37;,
  4;117,111,62,87;,
  4;87,62,199,72;,
  4;11,200,168,168;,
  4;201,202,179,40;,
  4;194,48,42,203;,
  4;198,204,39,44;,
  4;182,197,37,46;,
  4;205,195,38,177;,
  4;196,201,40,45;,
  3;80,206,54;,
  3;23,1,207;,
  4;22,0,35,49;,
  4;115,110,56,55;,
  4;149,208,209,141;,
  4;116,113,58,57;,
  3;0,50,210;,
  4;211,212,213,142;,
  4;214,173,66,215;,
  4;57,216,217,218;,
  4;53,219,220,221;,
  4;145,222,223,224;,
  4;56,225,226,55;,
  4;141,227,219,53;,
  4;228,229,68,146;,
  4;230,114,53,221;,
  4;148,115,55,145;,
  4;58,231,216,57;,
  4;0,174,172,35;,
  4;232,116,57,218;,
  3;233,74,73;,
  3;234,235,236;,
  3;78,80,79;,
  3;81,83,181;,
  4;72,88,99,87;,
  4;85,97,91,76;,
  4;117,121,93,79;,
  4;237,124,95,181;,
  4;109,109,118,118;,
  4;107,107,101,101;,
  4;125,125,103,103;,
  4;90,90,90,90;,
  4;128,128,105,105;,
  4;181,95,94,81;,
  3;238,122,98;,
  4;79,93,92,78;,
  4;239,240,96,84;,
  4;86,98,89,89;,
  4;76,91,240,75;,
  4;89,89,88,72;,
  4;119,123,97,85;,
  4;106,106,241,241;,
  4;126,126,102,102;,
  4;127,127,106,106;,
  4;100,100,108,108;,
  4;242,242,243,243;,
  4;244,244,244,244;,
  4;245,245,246,246;,
  4;247,247,248,248;,
  4;246,246,249,249;,
  4;104,104,104,104;,
  4;250,250,107,107;,
  4;251,251,251,251;,
  4;252,252,250,250;,
  4;253,253,245,245;,
  4;254,254,127,127;,
  4;108,108,126,126;,
  4;81,94,123,119;,
  4;78,92,122,255;,
  4;248,248,128,128;,
  4;243,243,125,125;,
  3;43,96,124;,
  4;87,99,121,117;,
  4;215,66,116,232;,
  4;146,68,115,148;,
  4;183,67,114,230;,
  4;66,63,113,116;,
  4;142,213,208,149;,
  4;68,59,110,115;,
  4;79,54,111,117;,
  4;256,157,156,156;,
  4;85,60,112,119;,
  4;164,164,160,160;,
  4;257,257,257,257;,
  4;258,258,258,258;,
  4;124,96,97,123;,
  4;259,259,259,259;,
  4;260,260,260,260;,
  4;121,99,98,122;,
  4;261,228,146,262;,
  4;263,148,145,224;,
  4;262,146,148,263;,
  4;67,142,149,114;,
  4;209,264,227,141;,
  4;55,226,222,145;,
  4;265,211,142,67;,
  4;114,149,141,53;,
  4;266,41,139,193;,
  4;71,71,143,143;,
  4;267,268,131,133;,
  4;269,200,130,132;,
  4;41,270,186,139;,
  4;200,11,129,130;,
  4;268,269,132,131;,
  4;271,271,147,147;,
  4;25,31,137,134;,
  4;31,27,135,137;,
  4;27,189,136,135;,
  4;191,25,134,138;,
  4;272,155,273,274;,
  4;275,49,23,276;,
  4;151,22,49,275;,
  3;151,140,50;,
  3;235,277,236;,
  3;278,279,280;,
  4;281,154,153,161;,
  4;154,159,282,153;,
  4;155,283,284,273;,
  3;281,285,279;,
  4;34,286,283,155;,
  4;287,34,155,272;,
  4;150,150,162,162;,
  4;156,156,288,288;,
  4;118,118,289,289;,
  4;36,36,290,290;,
  4;160,160,291,291;,
  3;256,292,293;,
  4;161,153,34,287;,
  4;153,282,286,34;,
  4;120,120,164,164;,
  4;288,288,109,109;,
  4;294,165,295,296;,
  3;166,41,297;,
  4;8,12,167,167;,
  4;298,165,299,300;,
  4;301,295,165,298;,
  4;302,299,165,294;,
  3;214,207,170;,
  3;174,210,175;,
  4;173,172,174,175;,
  4;214,170,172,173;,
  4;35,172,170,1;,
  3;46,78,303;,
  3;78,21,80;,
  3;65,83,81;,
  3;83,69,182;,
  3;171,68,229;,
  3;68,171,59;,
  3;23,144,276;,
  3;182,181,83;,
  3;181,46,303;,
  3;265,67,176;,
  3;304,176,183;,
  3;77,192,305;,
  3;51,74,233;,
  3;74,140,151;,
  3;206,80,21;,
  3;80,54,79;,
  3;170,207,1;,
  3;23,207,52;,
  3;50,0,22;,
  3;0,210,174;,
  3;122,238,255;,
  3;238,98,86;,
  3;96,43,84;,
  3;43,124,237;,
  3;151,50,22;,
  3;277,235,144;,
  3;276,144,235;,
  3;279,306,154;,
  3;306,279,278;,
  3;307,279,285;,
  3;281,279,154;,
  3;256,293,157;,
  3;41,308,270;,
  3;308,41,166;,
  3;297,41,266;;
 }
 MeshTextureCoords {
  192;
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