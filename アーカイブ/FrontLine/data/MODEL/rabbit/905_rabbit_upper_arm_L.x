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
 356;
 5.06905;-0.84445;-1.28598;,
 4.81397;-1.99481;-2.43195;,
 3.35942;0.36270;-2.63442;,
 3.34898;1.51368;-1.55171;,
 0.02379;-3.58876;-2.68693;,
 -0.85768;-4.52279;-1.79521;,
 -1.68310;-1.88486;-2.06831;,
 -0.90475;-0.80009;-2.73287;,
 -1.65796;-2.85177;3.40395;,
 -0.85768;-2.85163;4.83962;,
 -1.68310;-0.39908;3.83056;,
 -2.32266;-0.17872;2.40244;,
 3.38730;-0.10746;4.80489;,
 4.81397;-0.32366;4.20289;,
 3.35942;1.84848;3.26445;,
 1.93107;2.13505;3.58360;,
 4.65238;-10.97352;4.82038;,
 4.43929;-9.77552;5.52253;,
 3.35748;-9.69349;5.22824;,
 3.36539;-10.54632;4.48326;,
 1.64971;3.06282;1.91421;,
 1.93107;2.13505;3.58360;,
 3.34898;2.34925;1.76571;,
 -2.32266;-0.17872;2.40244;,
 -1.68310;-0.39908;3.83056;,
 -0.90475;0.87106;3.90197;,
 -1.71380;1.56363;2.29181;,
 -1.71380;1.56363;2.29181;,
 -0.90475;0.87106;3.90197;,
 0.36600;1.85547;3.83120;,
 -0.31562;2.95192;2.11286;,
 -0.31562;2.95192;2.11286;,
 0.36600;1.85547;3.83120;,
 1.93107;2.13505;3.58360;,
 1.64971;3.06282;1.91421;,
 7.66099;-8.63168;4.00101;,
 7.17659;-7.99123;4.79948;,
 6.58709;-8.81820;5.28141;,
 7.19984;-9.83806;4.53438;,
 7.19984;-9.83806;4.53438;,
 6.58709;-8.81820;5.28141;,
 5.62464;-9.50265;5.57770;,
 6.14089;-10.83063;4.90376;,
 6.14089;-10.83063;4.90376;,
 5.62464;-9.50265;5.57770;,
 4.43929;-9.77552;5.52253;,
 4.65238;-10.97352;4.82038;,
 -0.85768;-2.85163;4.83962;,
 0.02379;-1.60665;5.18250;,
 -1.68310;-0.39908;3.83056;,
 0.02379;-1.60665;5.18250;,
 1.56376;-0.47705;5.23942;,
 -0.90475;0.87106;3.90197;,
 1.56376;-0.47705;5.23942;,
 3.38730;-0.10746;4.80489;,
 0.36600;1.85547;3.83120;,
 4.81397;-1.99481;-2.43195;,
 3.38730;-2.08958;-3.06454;,
 1.93107;0.46389;-3.05124;,
 3.35942;0.36270;-2.63442;,
 3.38730;-2.08958;-3.06454;,
 1.56376;-2.62091;-3.27215;,
 0.36600;0.10039;-3.13686;,
 1.93107;0.46389;-3.05124;,
 1.56376;-2.62091;-3.27215;,
 0.02379;-3.58876;-2.68693;,
 -0.90475;-0.80009;-2.73287;,
 0.36600;0.10039;-3.13686;,
 5.62464;-10.72987;0.70536;,
 6.14089;-11.58017;1.92795;,
 4.65239;-11.66651;2.06907;,
 4.43929;-10.94406;0.88320;,
 6.14089;-11.58017;1.92795;,
 6.43184;-11.81923;3.57047;,
 4.76322;-11.81945;3.57052;,
 4.65239;-11.66651;2.06907;,
 6.43184;-11.81923;3.57047;,
 6.14089;-10.83063;4.90376;,
 4.65238;-10.97352;4.82038;,
 4.76322;-11.81945;3.57052;,
 6.58709;-9.98674;0.64208;,
 7.19984;-10.53105;1.78308;,
 6.14089;-11.58017;1.92795;,
 5.62464;-10.72987;0.70536;,
 7.19984;-10.53105;1.78308;,
 7.51855;-10.59134;3.26119;,
 6.43184;-11.81923;3.57047;,
 6.14089;-11.58017;1.92795;,
 7.51855;-10.59134;3.26119;,
 7.19984;-9.83806;4.53438;,
 6.14089;-10.83063;4.90376;,
 6.43184;-11.81923;3.57047;,
 7.17659;-9.03015;0.67477;,
 7.66099;-9.21594;1.68135;,
 7.19984;-10.53105;1.78308;,
 6.58709;-9.98674;0.64208;,
 7.66099;-9.21594;1.68135;,
 7.88031;-9.11086;2.88829;,
 7.51855;-10.59134;3.26119;,
 7.19984;-10.53105;1.78308;,
 7.88031;-9.11086;2.88829;,
 7.66099;-8.63168;4.00101;,
 7.19984;-9.83806;4.53438;,
 7.51855;-10.59134;3.26119;,
 0.36600;0.10039;-3.13686;,
 -0.31562;1.87999;-2.14293;,
 1.64971;2.07176;-2.02051;,
 -0.31562;1.87999;-2.14293;,
 -0.69978;3.22643;-0.21917;,
 1.50336;3.22672;-0.21925;,
 1.64971;2.07176;-2.02051;,
 -0.69978;3.22643;-0.21917;,
 -0.31562;2.95192;2.11286;,
 1.64971;3.06282;1.91421;,
 1.50336;3.22672;-0.21925;,
 -0.90475;-0.80009;-2.73287;,
 -1.71380;0.57258;-1.64290;,
 -0.31562;1.87999;-2.14293;,
 -1.71380;0.57258;-1.64290;,
 -2.13460;1.60520;0.18918;,
 -0.69978;3.22643;-0.21917;,
 -0.31562;1.87999;-2.14293;,
 -2.13460;1.60520;0.18918;,
 -1.71380;1.56363;2.29181;,
 -0.31562;2.95192;2.11286;,
 -0.69978;3.22643;-0.21917;,
 -1.68310;-1.88486;-2.06831;,
 -2.32266;-1.01430;-0.91498;,
 -1.71380;0.57258;-1.64290;,
 -2.32266;-1.01430;-0.91498;,
 -2.61224;-0.34954;0.68153;,
 -2.13460;1.60520;0.18918;,
 -1.71380;0.57258;-1.64290;,
 -2.61224;-0.34954;0.68153;,
 -2.32266;-0.17872;2.40244;,
 -1.71380;1.56363;2.29181;,
 -2.13460;1.60520;0.18918;,
 1.93107;0.46389;-3.05124;,
 1.64971;2.07176;-2.02051;,
 3.34898;1.51368;-1.55171;,
 1.64971;2.07176;-2.02051;,
 1.50336;3.22672;-0.21925;,
 3.34425;2.30540;0.01281;,
 1.50336;3.22672;-0.21925;,
 1.64971;3.06282;1.91421;,
 3.34898;2.34925;1.76571;,
 3.34425;2.30540;0.01281;,
 4.43929;-10.94406;0.88320;,
 4.65239;-11.66651;2.06907;,
 3.36539;-11.13058;2.16360;,
 3.35748;-10.73241;1.10352;,
 4.65239;-11.66651;2.06907;,
 4.76322;-11.81945;3.57052;,
 3.36897;-11.12166;3.39476;,
 3.36539;-11.13058;2.16360;,
 4.76322;-11.81945;3.57052;,
 4.65238;-10.97352;4.82038;,
 3.36539;-10.54632;4.48326;,
 3.36897;-11.12166;3.39476;,
 -0.85768;-4.52279;-1.79521;,
 -1.65796;-3.84283;-0.53077;,
 -2.32266;-1.01430;-0.91498;,
 -1.68310;-1.88486;-2.06831;,
 -1.65796;-3.84283;-0.53077;,
 -2.07421;-3.17050;1.39206;,
 -2.61224;-0.34954;0.68153;,
 -2.32266;-1.01430;-0.91498;,
 -2.07421;-3.17050;1.39206;,
 -1.65796;-2.85177;3.40395;,
 -2.32266;-0.17872;2.40244;,
 -2.61224;-0.34954;0.68153;,
 4.81397;-0.32366;4.20289;,
 5.06905;0.14660;2.64873;,
 3.35942;1.84848;3.26445;,
 5.06905;0.14660;2.64873;,
 5.20172;0.07254;0.57522;,
 5.20172;0.07254;0.57522;,
 5.06905;-0.84445;-1.28598;,
 7.15755;-7.18691;0.94077;,
 7.47281;-6.97431;2.35015;,
 7.47281;-6.97431;2.35015;,
 7.15755;-7.18691;0.94077;,
 6.14572;-3.76902;-0.38066;,
 6.46176;-3.17257;1.39258;,
 6.46176;-3.17257;1.39258;,
 6.14572;-3.76902;-0.38066;,
 5.06905;-0.84445;-1.28598;,
 5.20172;0.07254;0.57522;,
 7.47281;-6.97431;2.35015;,
 7.15755;-6.49392;3.69207;,
 7.15755;-6.49392;3.69207;,
 7.47281;-6.97431;2.35015;,
 6.46176;-3.17257;1.39258;,
 6.14571;-2.85787;3.23679;,
 6.14571;-2.85787;3.23679;,
 6.46176;-3.17257;1.39258;,
 5.20172;0.07254;0.57522;,
 5.06905;0.14660;2.64873;,
 7.17659;-7.99123;4.79948;,
 7.15755;-6.49392;3.69207;,
 6.55143;-5.99870;4.57124;,
 6.55143;-5.99870;4.57124;,
 7.15755;-6.49392;3.69207;,
 6.14571;-2.85787;3.23679;,
 5.58495;-2.81747;4.45246;,
 5.58495;-2.81747;4.45246;,
 6.14571;-2.85787;3.23679;,
 5.06905;0.14660;2.64873;,
 4.81397;-0.32366;4.20289;,
 3.36897;-11.12166;3.39476;,
 3.36539;-10.54632;4.48326;,
 2.06264;-8.76483;4.26406;,
 1.96215;-9.43054;2.96881;,
 1.96215;-9.43054;2.96881;,
 2.06264;-8.76483;4.26406;,
 -0.03887;-5.61447;3.93111;,
 -0.35492;-6.21091;2.15786;,
 -0.35492;-6.21091;2.15786;,
 -0.03887;-5.61447;3.93111;,
 -1.65796;-2.85177;3.40395;,
 -2.07421;-3.17050;1.39206;,
 3.36539;-11.13058;2.16360;,
 3.36897;-11.12166;3.39476;,
 1.96215;-9.43054;2.96881;,
 2.06264;-9.45782;1.51276;,
 2.06264;-9.45782;1.51276;,
 1.96215;-9.43054;2.96881;,
 -0.35492;-6.21091;2.15786;,
 -0.03887;-6.52562;0.31366;,
 -0.03887;-6.52562;0.31366;,
 -0.35492;-6.21091;2.15786;,
 -2.07421;-3.17050;1.39206;,
 -1.65796;-3.84283;-0.53077;,
 3.35748;-10.73241;1.10352;,
 3.36539;-11.13058;2.16360;,
 2.06264;-9.45782;1.51276;,
 2.25583;-9.08188;0.41416;,
 2.25583;-9.08188;0.41416;,
 2.06264;-9.45782;1.51276;,
 -0.03887;-6.52562;0.31366;,
 0.52190;-6.56602;-0.90202;,
 0.52190;-6.56602;-0.90202;,
 -0.03887;-6.52562;0.31366;,
 -1.65796;-3.84283;-0.53077;,
 -0.85768;-4.52279;-1.79521;,
 4.43929;-10.94406;0.88320;,
 3.33637;-9.23658;-0.00589;,
 4.71748;-8.95196;-0.31633;,
 4.71748;-8.95196;-0.31633;,
 3.33637;-9.23658;-0.00589;,
 1.50728;-6.29204;-1.66864;,
 3.05342;-5.69601;-2.21194;,
 3.05342;-5.69601;-2.21194;,
 1.50728;-6.29204;-1.66864;,
 0.02379;-3.58876;-2.68693;,
 1.56376;-2.62091;-3.27215;,
 5.62464;-10.72987;0.70536;,
 4.71748;-8.95196;-0.31633;,
 5.88382;-8.10113;-0.29188;,
 5.88382;-8.10113;-0.29188;,
 4.71748;-8.95196;-0.31633;,
 3.05342;-5.69601;-2.21194;,
 4.59957;-4.91374;-2.01580;,
 4.59957;-4.91374;-2.01580;,
 3.05342;-5.69601;-2.21194;,
 1.56376;-2.62091;-3.27215;,
 3.38730;-2.08958;-3.06454;,
 6.58709;-9.98674;0.64208;,
 5.88382;-8.10113;-0.29188;,
 6.55143;-7.16724;-0.06809;,
 6.55143;-7.16724;-0.06809;,
 5.88382;-8.10113;-0.29188;,
 4.59957;-4.91374;-2.01580;,
 5.58495;-4.30930;-1.47043;,
 5.58495;-4.30930;-1.47043;,
 4.59957;-4.91374;-2.01580;,
 3.38730;-2.08958;-3.06454;,
 4.81397;-1.99481;-2.43195;,
 5.62464;-9.50265;5.57770;,
 5.88382;-6.71515;5.21072;,
 4.71748;-7.45289;5.63529;,
 4.71748;-7.45289;5.63529;,
 5.88382;-6.71515;5.21072;,
 4.59957;-3.09145;5.21909;,
 3.05342;-3.68747;5.76238;,
 3.05342;-3.68747;5.76238;,
 4.59957;-3.09145;5.21909;,
 3.38730;-0.10746;4.80489;,
 1.56376;-0.47705;5.23942;,
 4.43929;-9.77552;5.52253;,
 4.71748;-7.45289;5.63529;,
 3.33637;-7.85061;5.49671;,
 3.33637;-7.85061;5.49671;,
 4.71748;-7.45289;5.63529;,
 3.05342;-3.68747;5.76238;,
 1.50728;-4.46975;5.56625;,
 1.50728;-4.46975;5.56625;,
 3.05342;-3.68747;5.76238;,
 1.56376;-0.47705;5.23942;,
 0.02379;-1.60665;5.18250;,
 3.35748;-9.69349;5.22824;,
 3.33637;-7.85061;5.49671;,
 2.25583;-7.91334;5.05349;,
 2.25583;-7.91334;5.05349;,
 3.33637;-7.85061;5.49671;,
 1.50728;-4.46975;5.56625;,
 0.52190;-5.07418;5.02087;,
 0.52190;-5.07418;5.02087;,
 1.50728;-4.46975;5.56625;,
 0.02379;-1.60665;5.18250;,
 -0.85768;-2.85163;4.83962;,
 6.58709;-8.81820;5.28141;,
 6.55143;-5.99870;4.57124;,
 5.88382;-6.71515;5.21072;,
 5.88382;-6.71515;5.21072;,
 6.55143;-5.99870;4.57124;,
 5.58495;-2.81747;4.45246;,
 4.59957;-3.09145;5.21909;,
 4.59957;-3.09145;5.21909;,
 5.58495;-2.81747;4.45246;,
 4.81397;-0.32366;4.20289;,
 3.38730;-0.10746;4.80489;,
 3.36539;-10.54632;4.48326;,
 3.35748;-9.69349;5.22824;,
 2.25583;-7.91334;5.05349;,
 2.06264;-8.76483;4.26406;,
 2.06264;-8.76483;4.26406;,
 2.25583;-7.91334;5.05349;,
 0.52190;-5.07418;5.02087;,
 -0.03887;-5.61447;3.93111;,
 -0.03887;-5.61447;3.93111;,
 0.52190;-5.07418;5.02087;,
 -0.85768;-2.85163;4.83962;,
 -1.65796;-2.85177;3.40395;,
 3.35748;-10.73241;1.10352;,
 2.25583;-9.08188;0.41416;,
 3.33637;-9.23658;-0.00589;,
 3.33637;-9.23658;-0.00589;,
 2.25583;-9.08188;0.41416;,
 0.52190;-6.56602;-0.90202;,
 1.50728;-6.29204;-1.66864;,
 1.50728;-6.29204;-1.66864;,
 0.52190;-6.56602;-0.90202;,
 -0.85768;-4.52279;-1.79521;,
 0.02379;-3.58876;-2.68693;,
 7.17659;-9.03015;0.67477;,
 6.55143;-7.16724;-0.06809;,
 7.15755;-7.18691;0.94077;,
 7.15755;-7.18691;0.94077;,
 6.55143;-7.16724;-0.06809;,
 5.58495;-4.30930;-1.47043;,
 6.14572;-3.76902;-0.38066;,
 6.14572;-3.76902;-0.38066;,
 5.58495;-4.30930;-1.47043;,
 4.81397;-1.99481;-2.43195;,
 5.06905;-0.84445;-1.28598;;
 
 96;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,14,22;,
 4;23,24,25,26;,
 4;27,28,29,30;,
 4;31,32,33,34;,
 4;35,36,37,38;,
 4;39,40,41,42;,
 4;43,44,45,46;,
 4;47,48,25,49;,
 4;50,51,29,52;,
 4;53,54,33,55;,
 4;56,57,58,59;,
 4;60,61,62,63;,
 4;64,65,66,67;,
 4;68,69,70,71;,
 4;72,73,74,75;,
 4;76,77,78,79;,
 4;80,81,82,83;,
 4;84,85,86,87;,
 4;88,89,90,91;,
 4;92,93,94,95;,
 4;96,97,98,99;,
 4;100,101,102,103;,
 4;104,105,106,63;,
 4;107,108,109,110;,
 4;111,112,113,114;,
 4;115,116,117,67;,
 4;118,119,120,121;,
 4;122,123,124,125;,
 4;126,127,128,7;,
 4;129,130,131,132;,
 4;133,134,135,136;,
 4;137,138,139,59;,
 4;140,141,142,3;,
 4;143,144,145,146;,
 4;147,148,149,150;,
 4;151,152,153,154;,
 4;155,156,157,158;,
 4;159,160,161,162;,
 4;163,164,165,166;,
 4;167,168,169,170;,
 4;171,172,145,173;,
 4;174,175,142,22;,
 4;176,177,139,146;,
 4;100,93,178,179;,
 4;180,181,182,183;,
 4;184,185,186,187;,
 4;35,97,188,189;,
 4;190,191,192,193;,
 4;194,195,196,197;,
 4;198,101,199,200;,
 4;201,202,203,204;,
 4;205,206,207,208;,
 4;209,210,211,212;,
 4;213,214,215,216;,
 4;217,218,219,220;,
 4;221,222,223,224;,
 4;225,226,227,228;,
 4;229,230,231,232;,
 4;233,234,235,236;,
 4;237,238,239,240;,
 4;241,242,243,244;,
 4;68,245,246,247;,
 4;248,249,250,251;,
 4;252,253,254,255;,
 4;80,256,257,258;,
 4;259,260,261,262;,
 4;263,264,265,266;,
 4;92,267,268,269;,
 4;270,271,272,273;,
 4;274,275,276,277;,
 4;278,40,279,280;,
 4;281,282,283,284;,
 4;285,286,287,288;,
 4;289,44,290,291;,
 4;292,293,294,295;,
 4;296,297,298,299;,
 4;300,17,301,302;,
 4;303,304,305,306;,
 4;307,308,309,310;,
 4;311,36,312,313;,
 4;314,315,316,317;,
 4;318,319,320,321;,
 4;322,323,324,325;,
 4;326,327,328,329;,
 4;330,331,332,333;,
 4;147,334,335,336;,
 4;337,338,339,340;,
 4;341,342,343,344;,
 4;96,345,346,347;,
 4;348,349,350,351;,
 4;352,353,354,355;;
 
 MeshMaterialList {
  1;
  96;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   60.000000;
   0.580000;0.580000;0.580000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/ENEMY/rabbit.png";
   }
  }
 }
 MeshNormals {
  98;
  0.747444;0.143086;0.648733;,
  0.485398;0.735553;0.472600;,
  0.747443;-0.181292;-0.639111;,
  0.485397;0.423921;-0.764644;,
  -0.471752;-0.400335;0.785609;,
  -0.769799;0.176085;0.613517;,
  -0.471752;-0.724715;-0.502234;,
  -0.769799;-0.135547;-0.623727;,
  -0.172306;-0.725106;-0.666732;,
  0.188892;-0.616342;-0.764488;,
  0.522255;-0.415525;-0.744707;,
  0.181008;0.352318;-0.918209;,
  -0.199640;0.206648;-0.957831;,
  -0.547363;0.027666;-0.836438;,
  0.522256;-0.013179;0.852687;,
  0.188890;-0.180668;0.965235;,
  -0.172309;-0.322760;0.930664;,
  -0.547363;0.420586;0.723534;,
  -0.199641;0.635716;0.745660;,
  0.181008;0.745236;0.641762;,
  0.926092;-0.123832;-0.356398;,
  0.999413;-0.033225;0.008369;,
  0.926092;0.059769;0.372534;,
  0.583115;0.795909;0.162805;,
  0.624573;0.757314;-0.190748;,
  0.583114;0.623827;-0.520402;,
  -0.577963;-0.610621;0.541388;,
  -0.624772;-0.757160;0.190709;,
  -0.577963;-0.794221;-0.187541;,
  -0.935275;-0.052952;-0.349937;,
  -0.999404;0.033472;-0.008430;,
  -0.935275;0.119131;0.333268;,
  -0.767481;0.296126;-0.568580;,
  -0.878611;0.463075;-0.116636;,
  -0.767481;0.530130;0.360466;,
  -0.343823;0.586715;-0.733179;,
  -0.417638;0.881095;-0.221926;,
  -0.343824;0.864018;0.367775;,
  0.176417;0.716841;-0.674548;,
  0.209689;0.948155;-0.238817;,
  0.176417;0.950846;0.254499;,
  0.754663;-0.528706;-0.388527;,
  0.878894;-0.462569;0.116511;,
  0.754663;-0.281580;0.592618;,
  0.335564;-0.853349;-0.398988;,
  0.417638;-0.881094;0.221928;,
  0.335564;-0.562533;0.755615;,
  -0.174322;-0.942774;-0.284233;,
  -0.210273;-0.948033;0.238786;,
  -0.174322;-0.695647;0.696912;,
  0.728299;0.258780;0.634518;,
  0.730303;0.286144;0.620306;,
  0.651744;0.490779;0.578244;,
  0.651744;0.158309;-0.741733;,
  0.730301;-0.041837;-0.681843;,
  0.728297;-0.072669;-0.681397;,
  -0.678391;-0.434561;-0.592404;,
  -0.571144;-0.621919;-0.535734;,
  -0.547066;-0.641126;-0.538216;,
  -0.678391;-0.102092;0.727574;,
  -0.571145;-0.293940;0.766416;,
  -0.547066;-0.309678;0.777700;,
  -0.328962;0.142527;0.933526;,
  -0.238519;-0.077979;0.968002;,
  -0.210665;-0.117554;0.970465;,
  -0.033718;0.314585;0.948630;,
  0.072324;0.087973;0.993494;,
  0.101511;0.022760;0.994574;,
  0.282359;0.415007;0.864894;,
  0.392140;0.203119;0.897201;,
  0.403585;0.156231;0.901505;,
  0.282360;-0.044209;-0.958289;,
  0.392140;-0.246119;-0.886370;,
  0.403584;-0.289453;-0.867950;,
  -0.033719;-0.172315;-0.984465;,
  0.072325;-0.393142;-0.916629;,
  0.101513;-0.451085;-0.886689;,
  -0.328963;-0.316688;-0.889659;,
  -0.238517;-0.527216;-0.815569;,
  -0.210662;-0.563236;-0.798991;,
  -0.890678;-0.411954;-0.192321;,
  -0.790360;-0.589523;-0.166717;,
  -0.756982;-0.631251;-0.168823;,
  -0.937040;-0.338644;0.085296;,
  -0.845280;-0.518141;0.130507;,
  -0.803321;-0.577509;0.145460;,
  -0.890677;-0.271701;0.364517;,
  -0.790359;-0.440210;0.426084;,
  -0.756982;-0.475961;0.447705;,
  0.847501;0.503043;0.169378;,
  0.917607;0.321067;0.234335;,
  0.926192;0.274267;0.258739;,
  0.875200;0.469110;-0.118156;,
  0.955931;0.284698;-0.071707;,
  0.975073;0.215162;-0.054193;,
  0.847501;0.362790;-0.387459;,
  0.917607;0.171755;-0.358464;,
  0.926192;0.118978;-0.357788;;
  96;
  4;95,53,3,25;,
  4;77,56,7,13;,
  4;86,59,5,31;,
  4;68,52,1,19;,
  4;49,16,4,26;,
  4;40,19,1,23;,
  4;31,5,17,34;,
  4;34,17,18,37;,
  4;37,18,19,40;,
  4;22,0,14,43;,
  4;43,14,15,46;,
  4;46,15,16,49;,
  4;59,62,17,5;,
  4;62,65,18,17;,
  4;65,68,19,18;,
  4;53,71,11,3;,
  4;71,74,12,11;,
  4;74,77,13,12;,
  4;9,44,47,8;,
  4;44,45,48,47;,
  4;45,46,49,48;,
  4;10,41,44,9;,
  4;41,42,45,44;,
  4;42,43,46,45;,
  4;2,20,41,10;,
  4;20,21,42,41;,
  4;21,22,43,42;,
  4;12,35,38,11;,
  4;35,36,39,38;,
  4;36,37,40,39;,
  4;13,32,35,12;,
  4;32,33,36,35;,
  4;33,34,37,36;,
  4;7,29,32,13;,
  4;29,30,33,32;,
  4;30,31,34,33;,
  4;11,38,25,3;,
  4;38,39,24,25;,
  4;39,40,23,24;,
  4;8,47,28,6;,
  4;47,48,27,28;,
  4;48,49,26,27;,
  4;56,80,29,7;,
  4;80,83,30,29;,
  4;83,86,31,30;,
  4;52,89,23,1;,
  4;89,92,24,23;,
  4;92,95,25,24;,
  4;21,20,97,94;,
  4;94,97,96,93;,
  4;93,96,95,92;,
  4;22,21,94,91;,
  4;91,94,93,90;,
  4;90,93,92,89;,
  4;0,22,91,50;,
  4;50,91,90,51;,
  4;51,90,89,52;,
  4;27,26,88,85;,
  4;85,88,87,84;,
  4;84,87,86,83;,
  4;28,27,85,82;,
  4;82,85,84,81;,
  4;81,84,83,80;,
  4;6,28,82,58;,
  4;58,82,81,57;,
  4;57,81,80,56;,
  4;9,8,79,76;,
  4;76,79,78,75;,
  4;75,78,77,74;,
  4;10,9,76,73;,
  4;73,76,75,72;,
  4;72,75,74,71;,
  4;2,10,73,55;,
  4;55,73,72,54;,
  4;54,72,71,53;,
  4;15,14,70,67;,
  4;67,70,69,66;,
  4;66,69,68,65;,
  4;16,15,67,64;,
  4;64,67,66,63;,
  4;63,66,65,62;,
  4;4,16,64,61;,
  4;61,64,63,60;,
  4;60,63,62,59;,
  4;14,0,50,70;,
  4;70,50,51,69;,
  4;69,51,52,68;,
  4;26,4,61,88;,
  4;88,61,60,87;,
  4;87,60,59,86;,
  4;8,6,58,79;,
  4;79,58,57,78;,
  4;78,57,56,77;,
  4;20,2,55,97;,
  4;97,55,54,96;,
  4;96,54,53,95;;
 }
 MeshTextureCoords {
  356;
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;,
  0.373870;0.626130;,
  0.373870;0.373870;,
  0.626130;0.373870;,
  0.626130;0.626130;;
 }
}
