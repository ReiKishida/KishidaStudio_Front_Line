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
 62;
 -57.63991;0.50175;801.45343;,
 -57.63991;243.64954;801.45343;,
 -30.85226;243.64954;807.40656;,
 -30.85226;0.50175;807.40651;,
 -30.95078;0.50175;861.40089;,
 -30.95078;243.64954;861.40099;,
 -57.75695;243.64954;867.35402;,
 -57.75695;0.50175;867.35402;,
 -24.63540;0.50175;834.40372;,
 -24.63540;243.64954;834.40372;,
 -84.44396;0.50175;807.40651;,
 -84.44396;243.64954;807.40656;,
 -90.75701;0.50175;834.40372;,
 -90.75701;243.64954;834.40372;,
 -24.63540;243.64954;834.40372;,
 -24.63540;0.50175;834.40372;,
 -24.63540;243.64954;834.40372;,
 -15.07080;253.02351;834.40372;,
 -23.21320;253.02351;869.21115;,
 -30.95078;243.64954;861.40099;,
 -57.77432;253.02351;876.88644;,
 -57.75695;243.64954;867.35402;,
 -100.32122;253.02351;834.40372;,
 -100.32122;259.28282;834.40372;,
 -92.18182;259.28282;799.59630;,
 -92.18182;253.02351;799.59630;,
 -84.53873;0.50175;861.40089;,
 -84.53873;243.64954;861.40099;,
 -57.63991;243.64954;801.45343;,
 -57.62344;253.02351;791.92101;,
 -23.08618;253.02351;799.59630;,
 -30.85226;243.64954;807.40656;,
 -84.44396;243.64954;807.40656;,
 -92.18182;253.02351;799.59630;,
 -15.07080;253.02351;834.40372;,
 -24.63540;243.64954;834.40372;,
 -84.53873;243.64954;861.40099;,
 -92.30402;253.02351;869.21115;,
 -100.32122;253.02351;834.40372;,
 -90.75701;243.64954;834.40372;,
 -90.75701;243.64954;834.40372;,
 -100.32122;253.02351;834.40372;,
 -57.62344;253.02351;791.92101;,
 -57.62344;259.28282;791.92101;,
 -23.08618;259.28282;799.59630;,
 -23.08618;253.02351;799.59630;,
 -15.07080;253.02351;834.40372;,
 -15.07080;259.28282;834.40372;,
 -23.21320;259.28282;869.21115;,
 -23.21320;253.02351;869.21115;,
 -57.77432;253.02351;876.88644;,
 -57.77432;259.28282;876.88644;,
 -92.30402;259.28282;869.21115;,
 -92.30402;253.02351;869.21115;,
 -100.32122;259.28282;834.40372;,
 -15.07080;259.28282;834.40372;,
 -57.62344;259.28282;791.92101;,
 -23.08618;259.28282;799.59630;,
 -57.77432;259.28282;876.88644;,
 -23.21320;259.28282;869.21115;,
 -92.30402;259.28282;869.21115;,
 -92.18182;259.28282;799.59630;;
 
 30;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,5,4;,
 4;10,11,1,0;,
 4;12,13,11,10;,
 4;3,2,14,15;,
 4;16,17,18,19;,
 4;19,18,20,21;,
 4;22,23,24,25;,
 4;26,27,13,12;,
 4;28,29,30,31;,
 4;7,6,27,26;,
 4;32,33,29,28;,
 4;31,30,34,35;,
 4;36,37,38,39;,
 4;40,41,33,32;,
 4;21,20,37,36;,
 4;42,43,44,45;,
 4;46,47,48,49;,
 4;50,51,52,53;,
 4;53,52,23,22;,
 4;49,48,51,50;,
 4;45,44,47,46;,
 4;25,24,43,42;,
 3;54,55,56;,
 3;57,56,55;,
 3;55,58,59;,
 3;54,60,58;,
 3;55,54,58;,
 3;54,56,61;;
 
 MeshMaterialList {
  1;
  30;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.800000;0.800000;0.800000;1.000000;;
   0.000000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MAP\\wall_1.png";
   }
  }
 }
 MeshNormals {
  44;
  0.920862;-0.389884;0.001612;,
  -0.716080;-0.554807;0.423579;,
  -0.000073;-0.388889;0.921285;,
  0.200454;-0.380901;0.902626;,
  0.000065;0.000000;-1.000000;,
  0.920862;-0.389884;0.001612;,
  0.417915;-0.554802;-0.719404;,
  -0.000096;-0.000000;1.000000;,
  -0.920867;-0.389874;-0.001551;,
  -0.417510;-0.554264;-0.720053;,
  -0.999999;-0.000000;-0.001666;,
  -0.216814;0.000000;-0.976213;,
  -0.974485;-0.000000;0.224453;,
  0.000044;-0.388890;-0.921284;,
  0.999999;-0.000000;0.001731;,
  0.216941;0.000000;-0.976185;,
  0.216797;-0.000000;0.976217;,
  0.632134;-0.437700;0.639394;,
  0.633091;-0.438267;-0.638059;,
  -0.633097;-0.438250;0.638063;,
  -0.000073;-0.388889;0.921285;,
  -0.900218;-0.381175;-0.210508;,
  0.000044;-0.388890;-0.921284;,
  -0.920867;-0.389874;-0.001551;,
  -0.973732;0.000000;-0.227698;,
  0.000065;0.000000;-1.000000;,
  -0.216985;0.000000;0.976175;,
  -0.000096;0.000000;1.000000;,
  0.216940;0.000000;-0.976185;,
  0.999999;0.000000;0.001731;,
  0.973713;0.000000;0.227778;,
  -0.999999;0.000000;-0.001666;,
  0.900199;-0.381180;0.210581;,
  0.973713;-0.000000;0.227778;,
  -0.973732;0.000000;-0.227699;,
  0.974496;0.000000;-0.224405;,
  -0.216985;-0.000000;0.976175;,
  -0.200469;-0.380906;-0.902620;,
  -0.974485;0.000000;0.224453;,
  0.216797;0.000000;0.976217;,
  0.974496;0.000000;-0.224405;,
  -0.216813;0.000000;-0.976213;,
  0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;;
  30;
  4;4,13,6,15;,
  4;16,3,2,7;,
  4;14,0,32,33;,
  4;11,9,13,4;,
  4;10,8,34,34;,
  4;35,35,0,14;,
  4;0,5,17,32;,
  4;3,17,20,2;,
  4;23,31,24,21;,
  4;12,1,8,10;,
  4;13,22,18,6;,
  4;7,2,36,36;,
  4;9,37,22,13;,
  4;6,18,5,0;,
  4;1,19,23,8;,
  4;8,23,21,9;,
  4;2,20,19,1;,
  4;22,25,28,18;,
  4;5,29,30,17;,
  4;20,27,26,19;,
  4;19,38,31,23;,
  4;17,39,27,20;,
  4;18,40,29,5;,
  4;37,41,25,22;,
  3;42,42,42;,
  3;42,42,42;,
  3;42,42,42;,
  3;42,43,42;,
  3;42,42,42;,
  3;42,42,43;;
 }
 MeshTextureCoords {
  62;
  0.665310;0.999930;,
  0.665310;0.008050;,
  0.726710;0.008050;,
  0.726720;0.999930;,
  0.289190;0.999930;,
  0.289190;0.008050;,
  0.350630;0.008050;,
  0.350630;0.999930;,
  0.193240;0.999930;,
  0.193240;0.008050;,
  0.603880;0.999930;,
  0.603880;0.008050;,
  0.507930;0.999930;,
  0.507930;0.008050;,
  0.822630;0.008050;,
  0.822630;0.999930;,
  0.629410;0.308030;,
  0.664070;0.316670;,
  0.608310;0.468730;,
  0.586160;0.425980;,
  0.509410;0.499950;,
  0.509450;0.450190;,
  0.406550;0.552830;,
  0.394140;0.552840;,
  0.421230;0.442810;,
  0.430500;0.450820;,
  0.412020;0.999930;,
  0.412020;0.008050;,
  0.509700;0.549810;,
  0.509730;0.500050;,
  0.608580;0.531220;,
  0.586370;0.573990;,
  0.432990;0.574020;,
  0.410840;0.531260;,
  0.664080;0.683330;,
  0.629410;0.691970;,
  0.432790;0.426010;,
  0.410580;0.468780;,
  0.355080;0.316670;,
  0.389750;0.308030;,
  0.389750;0.691970;,
  0.355080;0.683330;,
  0.488830;0.419960;,
  0.488180;0.407500;,
  0.560210;0.428560;,
  0.550780;0.436870;,
  0.576580;0.544980;,
  0.588940;0.545270;,
  0.561220;0.655170;,
  0.551970;0.647450;,
  0.496000;0.678920;,
  0.496570;0.691100;,
  0.428120;0.668400;,
  0.437220;0.660430;,
  0.448320;0.387540;,
  0.512000;0.839030;,
  0.112870;0.665480;,
  0.205030;0.839030;,
  0.847450;0.561060;,
  0.806900;0.753460;,
  0.755290;0.387550;,
  0.153420;0.473100;;
 }
}
