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
 13;
 2001.17503;0.00000;-93.92869;,
 1269.88889;0.00000;-93.92868;,
 1352.17121;0.00001;-152.16697;,
 2083.45745;0.00000;-152.16697;,
 1352.17121;0.00001;-354.55511;,
 1352.17121;199.20625;-354.55511;,
 1352.17121;199.20624;-152.16696;,
 2083.45745;199.20624;-152.16696;,
 1269.88889;0.00000;-93.92868;,
 1288.92062;0.00001;-354.55511;,
 1352.17121;199.20624;-152.16696;,
 1352.17121;199.20624;-152.16696;,
 1352.17121;199.20625;-354.55511;;
 
 9;
 4;0,1,2,3;,
 4;4,5,6,2;,
 4;3,2,6,7;,
 3;6,1,2;,
 4;4,2,8,9;,
 4;7,10,1,0;,
 3;3,7,0;,
 4;9,8,11,12;,
 3;4,9,5;;
 
 MeshMaterialList {
  1;
  9;
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
  12;
  1.000000;0.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.000000;-1.000000;-0.000000;,
  0.888178;-0.000000;0.459500;,
  0.000000;0.000000;-1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.305895;0.148577;0.940400;,
  0.305895;0.148577;0.940400;,
  0.000000;0.280606;0.959823;,
  0.577719;-0.000000;0.816235;,
  -0.942094;0.333626;-0.033956;,
  0.000000;0.000000;-1.000000;;
  9;
  4;2,1,1,2;,
  4;0,0,3,3;,
  4;4,4,4,4;,
  3;3,6,3;,
  4;5,1,1,5;,
  4;7,8,6,7;,
  3;9,7,7;,
  4;10,10,10,10;,
  3;11,11,11;;
 }
 MeshTextureCoords {
  13;
  0.010000;1.000000;,
  0.170000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.010000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.170000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;;
 }
}
