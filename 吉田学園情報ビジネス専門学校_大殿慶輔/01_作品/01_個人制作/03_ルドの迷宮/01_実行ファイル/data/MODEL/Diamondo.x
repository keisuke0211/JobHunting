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
 10;
 0.00000;16.00000;0.00000;,
 -8.00000;0.00000;8.00000;,
 0.00000;0.00000;10.83562;,
 -10.69305;0.00000;0.00000;,
 -8.00000;0.00000;-8.00000;,
 0.00000;-16.00000;-0.00000;,
 0.00000;0.00000;-10.83562;,
 8.00000;0.00000;-8.00000;,
 8.00000;-0.00000;8.00000;,
 10.69305;0.00000;0.00000;;
 
 16;
 3;0,1,2;,
 3;3,0,4;,
 3;1,0,3;,
 3;5,4,6;,
 3;1,3,5;,
 3;1,5,2;,
 3;5,3,4;,
 3;7,6,0;,
 3;2,8,0;,
 3;9,7,0;,
 3;8,9,0;,
 3;6,7,5;,
 3;5,9,8;,
 3;8,2,5;,
 3;5,7,9;,
 3;4,0,6;;
 
 MeshMaterialList {
  1;
  16;
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.494118;0.729412;1.000000;1.000000;;
   5.000000;
   0.100000;0.100000;0.100000;;
   0.024706;0.036471;0.050000;;
  }
 }
 MeshNormals {
  20;
  -0.582245;0.577336;-0.572428;,
  -0.831417;0.555649;0.000000;,
  -0.582245;0.577336;0.572428;,
  0.000000;-0.560739;-0.827993;,
  0.000000;0.560739;0.827993;,
  0.582245;0.577336;-0.572428;,
  -0.309528;-0.622874;-0.718485;,
  0.831417;0.555649;0.000000;,
  0.582245;0.577336;0.572428;,
  -0.309528;0.622874;0.718485;,
  -0.582245;-0.577336;-0.572428;,
  -0.582245;-0.577336;0.572428;,
  -0.831417;-0.555649;0.000000;,
  -0.309528;-0.622874;0.718485;,
  0.000000;-0.560739;0.827993;,
  0.000000;0.560739;-0.827993;,
  0.724039;0.618413;-0.305505;,
  0.582245;-0.577336;-0.572428;,
  0.831417;-0.555649;-0.000000;,
  0.582245;-0.577336;0.572428;;
  16;
  3;9,2,4;,
  3;1,0,0;,
  3;2,9,1;,
  3;6,10,3;,
  3;11,12,13;,
  3;11,13,14;,
  3;6,12,10;,
  3;5,15,16;,
  3;4,8,9;,
  3;7,5,16;,
  3;8,7,16;,
  3;3,17,6;,
  3;18,18,19;,
  3;19,14,13;,
  3;18,17,18;,
  3;0,0,15;;
 }
 MeshTextureCoords {
  10;
  0.500000;0.492000;,
  0.496000;0.500000;,
  0.500000;0.500000;,
  0.494653;0.500000;,
  0.496000;0.500000;,
  0.500000;0.508000;,
  0.500000;0.500000;,
  0.504000;0.500000;,
  0.504000;0.500000;,
  0.505347;0.500000;;
 }
}
