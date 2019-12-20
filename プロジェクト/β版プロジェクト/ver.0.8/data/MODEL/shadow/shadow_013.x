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
 24;
 262.88721;686.58497;-1407.22072;,
 262.88721;686.58497;-724.98473;,
 100.08857;0.00002;-412.58677;,
 100.08857;0.00005;-1094.82267;,
 100.08857;0.00002;-412.58677;,
 100.08857;0.00005;-1094.82267;,
 -552.06358;0.00005;-1094.82267;,
 -552.06358;0.00002;-412.58677;,
 262.88721;686.58497;-724.98473;,
 -389.26495;686.58497;-724.98473;,
 -552.06358;0.00002;-412.58677;,
 100.08857;0.00002;-412.58677;,
 -389.26495;686.58497;-724.98473;,
 -389.26495;686.58497;-1407.22072;,
 -552.06358;0.00005;-1094.82267;,
 -552.06358;0.00002;-412.58677;,
 -389.26495;686.58497;-1407.22072;,
 262.88721;686.58497;-1407.22072;,
 100.08857;0.00005;-1094.82267;,
 -552.06358;0.00005;-1094.82267;,
 -389.26495;686.58497;-1407.22072;,
 -389.26495;686.58497;-724.98473;,
 262.88721;686.58497;-724.98473;,
 262.88721;686.58497;-1407.22072;;
 
 6;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;;
 
 MeshMaterialList {
  1;
  6;
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
  6;
  0.973021;-0.230717;-0.000000;,
  -0.973021;0.230717;0.000000;,
  0.000000;0.414148;0.910210;,
  0.000000;1.000000;0.000000;,
  0.000000;-0.414148;-0.910210;,
  0.000000;1.000000;0.000000;;
  6;
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;4,4,4,4;,
  4;5,5,5,5;;
 }
 MeshTextureCoords {
  24;
  0.426540;1.499420;,
  0.426540;-0.252390;,
  0.426540;-0.252390;,
  0.426540;1.499420;,
  0.975310;0.999910;,
  0.454430;0.999910;,
  0.454430;0.000090;,
  0.975310;0.000090;,
  0.430290;1.200260;,
  0.430130;-0.253800;,
  0.430130;-0.253800;,
  0.430290;1.200260;,
  0.422860;1.378780;,
  0.422860;-0.373040;,
  0.422860;-0.373040;,
  0.422860;1.378780;,
  0.450240;1.400190;,
  0.450240;-0.351580;,
  0.450240;-0.351580;,
  0.450240;1.400190;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;;
 }
}
