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
 32;
 0.10367;2.07723;-3.97512;,
 0.10369;-1.98297;-7.09085;,
 0.19479;-5.81167;-2.10287;,
 0.19477;-1.75146;1.01286;,
 6.06471;2.07895;-3.97732;,
 5.93499;0.53943;-2.21532;,
 5.93499;-3.28487;-5.14991;,
 6.06472;-1.98126;-7.09305;,
 6.15581;-1.74975;1.01066;,
 5.54501;0.28911;1.88172;,
 0.78718;0.28775;1.88347;,
 5.78584;-4.88451;-7.22559;,
 0.28601;-4.86156;-7.25553;,
 6.15583;-5.80996;-2.10507;,
 5.47229;2.94013;-1.57182;,
 0.71447;2.93877;-1.57007;,
 6.28555;-4.77769;-3.20601;,
 7.65780;-4.36105;-2.91322;,
 7.65779;-1.34387;-0.59795;,
 6.28554;-0.95339;-0.27142;,
 7.30724;0.14878;-2.54198;,
 7.30724;-2.86840;-4.85724;,
 0.47368;-7.19812;-4.21214;,
 1.20996;-6.97199;-4.50667;,
 5.15654;-6.98190;-4.49373;,
 5.97351;-7.22107;-4.18220;,
 5.04956;-5.11064;-6.93106;,
 5.04957;-5.64599;-7.34207;,
 1.10299;-5.63608;-7.35502;,
 1.10298;-5.10073;-6.94400;,
 5.15655;-7.51725;-4.90474;,
 1.20997;-7.50734;-4.91768;;
 
 30;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;0,4,7,1;,
 4;8,9,10,3;,
 4;7,11,12,1;,
 4;2,13,8,3;,
 4;10,9,14,15;,
 4;4,14,9,8;,
 4;3,10,15,0;,
 4;0,15,14,4;,
 4;16,17,18,19;,
 4;8,19,5,4;,
 4;7,6,16,13;,
 4;13,16,19,8;,
 4;20,18,17,21;,
 4;5,20,21,6;,
 4;19,18,20,5;,
 4;6,21,17,16;,
 4;22,23,24,25;,
 4;2,22,25,13;,
 4;13,25,11,7;,
 4;1,12,22,2;,
 4;26,27,28,29;,
 4;25,24,26,11;,
 4;11,26,29,12;,
 4;12,29,23,22;,
 4;28,27,30,31;,
 4;29,28,31,23;,
 4;23,31,30,24;,
 4;24,30,27,26;;
 
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
   0.640000;0.640000;0.640000;1.000000;;
   0.500000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  38;
  -0.990740;0.097984;0.093985;,
  -0.998490;-0.053569;-0.012126;,
  0.989311;0.119763;0.083194;,
  0.000463;-0.504741;0.863271;,
  -0.001301;-0.729762;0.683700;,
  0.996210;-0.060685;-0.062320;,
  -0.000226;0.987949;0.154777;,
  0.000451;-0.392760;0.919641;,
  0.996688;0.066193;0.047246;,
  0.015555;-0.608703;0.793246;,
  0.996586;-0.063703;-0.052527;,
  0.989926;0.086196;-0.112325;,
  0.000986;-0.569272;-0.822149;,
  0.000005;-0.793192;-0.608971;,
  0.000986;-0.569272;-0.822149;,
  0.000006;-0.793192;-0.608972;,
  0.004131;0.608967;-0.793184;,
  0.000006;-0.793192;-0.608972;,
  -0.000450;0.807985;-0.589203;,
  0.001141;0.345768;-0.938319;,
  -0.000003;0.793240;0.608910;,
  0.962194;0.224861;0.153686;,
  0.989313;0.113593;0.091412;,
  -0.963458;0.202997;0.174761;,
  0.996606;0.056894;0.059500;,
  0.996505;-0.073135;-0.040372;,
  0.996279;-0.066967;-0.054246;,
  -0.015551;0.608702;-0.793246;,
  0.346480;0.773154;0.531211;,
  0.346462;-0.712797;-0.609824;,
  -0.003035;-0.831283;0.555842;,
  0.995150;-0.054661;-0.081785;,
  -0.994528;-0.098179;-0.035710;,
  0.000006;-0.793192;-0.608972;,
  0.000005;-0.793192;-0.608972;,
  -0.999395;-0.021200;0.027589;,
  -0.004130;-0.608963;0.793188;,
  0.999394;0.021200;-0.027589;;
  30;
  4;0,1,1,0;,
  4;2,8,10,5;,
  4;18,18,19,19;,
  4;3,7,7,3;,
  4;19,14,12,19;,
  4;4,4,3,3;,
  4;20,20,6,6;,
  4;2,21,21,22;,
  4;0,23,23,0;,
  4;18,6,6,18;,
  4;9,9,9,9;,
  4;22,24,8,2;,
  4;5,10,25,26;,
  4;26,25,24,22;,
  4;11,11,11,11;,
  4;27,27,27,27;,
  4;28,28,28,28;,
  4;29,29,29,29;,
  4;13,13,15,15;,
  4;4,30,30,4;,
  4;26,31,31,5;,
  4;1,32,32,1;,
  4;16,16,16,16;,
  4;15,15,33,14;,
  4;14,33,34,12;,
  4;12,34,13,13;,
  4;17,17,17,17;,
  4;35,35,35,35;,
  4;36,36,36,36;,
  4;37,37,37,37;;
 }
 MeshTextureCoords {
  32;
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
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
