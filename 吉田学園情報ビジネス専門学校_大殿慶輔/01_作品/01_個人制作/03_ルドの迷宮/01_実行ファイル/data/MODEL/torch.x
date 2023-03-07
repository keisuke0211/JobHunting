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
 112;
 -0.51955;14.59740;-6.45470;,
 -0.96140;17.16765;-7.54555;,
 -2.89070;14.91035;-8.78650;,
 -2.09005;14.95785;-10.22365;,
 -2.48930;17.59955;-9.29295;,
 -0.61280;18.82665;-10.31175;,
 0.17450;20.50850;-9.81345;,
 1.96310;16.97660;-8.18330;,
 1.83490;15.94545;-10.37405;,
 2.07525;13.04805;-7.46410;,
 2.51180;11.27100;-9.11055;,
 -0.59855;14.08350;-11.86735;,
 0.68705;11.08735;-6.98340;,
 2.26980;13.86110;-10.92820;,
 -1.99860;11.13580;-7.79420;,
 0.01230;9.15690;-9.18440;,
 0.87200;11.29530;-11.56640;,
 -2.22285;11.25285;-10.67385;,
 5.29050;-6.77730;-2.58965;,
 5.29050;6.77730;-2.58965;,
 5.29050;-6.77730;-0.00310;,
 5.29050;6.77730;-0.00310;,
 -5.29050;-6.77730;-0.00310;,
 -5.29050;6.77730;-0.00310;,
 -5.29050;-6.77730;-2.58965;,
 -5.29050;6.77730;-2.58965;,
 0.74455;-0.78345;-2.58965;,
 -0.74455;-0.78345;-2.58965;,
 0.74455;-0.78345;-7.63350;,
 -0.74455;-0.78345;-7.63350;,
 0.74455;0.78345;-2.58965;,
 -0.74455;0.78345;-2.58965;,
 -0.74455;0.78345;-7.63350;,
 0.74455;0.78345;-7.63350;,
 -3.33550;0.00000;-9.27650;,
 -1.66775;0.00000;-6.38785;,
 -3.00195;0.57770;-9.27650;,
 -1.50095;0.57770;-6.67675;,
 -2.33485;0.57770;-9.27650;,
 -1.16740;0.57770;-7.25445;,
 -2.00130;0.00000;-9.27650;,
 -1.00065;0.00000;-7.54330;,
 -2.33485;-0.57775;-9.27650;,
 -1.16740;-0.57775;-7.25445;,
 -3.00195;-0.57775;-9.27650;,
 -1.50095;-0.57770;-6.67675;,
 1.66775;0.00000;-6.38785;,
 1.50095;0.57770;-6.67675;,
 1.16740;0.57770;-7.25445;,
 1.00065;0.00000;-7.54330;,
 1.16740;-0.57775;-7.25445;,
 1.50095;-0.57770;-6.67675;,
 3.33550;0.00000;-9.27650;,
 3.00195;0.57770;-9.27650;,
 2.33485;0.57770;-9.27650;,
 2.00130;0.00000;-9.27650;,
 2.33485;-0.57775;-9.27650;,
 3.00195;-0.57775;-9.27650;,
 1.66775;0.00000;-12.16510;,
 1.50095;0.57770;-11.87625;,
 1.16740;0.57770;-11.29855;,
 1.00065;0.00000;-11.00965;,
 1.16740;-0.57775;-11.29855;,
 1.50095;-0.57775;-11.87625;,
 -1.66775;0.00000;-12.16510;,
 -1.50095;0.57770;-11.87625;,
 -1.16740;0.57770;-11.29855;,
 -1.00065;0.00000;-11.00965;,
 -1.16740;-0.57775;-11.29855;,
 -1.50095;-0.57775;-11.87625;,
 0.84815;-11.92515;-7.74735;,
 1.38910;9.01825;-6.81040;,
 -0.84815;-11.92515;-7.74735;,
 -1.38910;9.01825;-6.81040;,
 -1.69630;-11.92515;-9.21635;,
 -2.77815;9.01825;-9.21640;,
 -0.84815;-11.92515;-10.68540;,
 -1.38910;9.01825;-11.62235;,
 0.84815;-11.92515;-10.68540;,
 1.38910;9.01825;-11.62235;,
 1.92860;9.01825;-12.55680;,
 -1.92860;9.01825;-12.55680;,
 1.69630;-11.92515;-9.21635;,
 2.77815;9.01825;-9.21640;,
 1.38910;12.84775;-6.81040;,
 2.77815;12.84775;-9.21640;,
 1.38910;11.46660;-6.81040;,
 2.77815;11.46660;-9.21640;,
 -1.38910;12.84775;-11.62235;,
 1.38910;12.84775;-11.62235;,
 -1.92860;12.84775;-12.55680;,
 1.92860;12.84775;-12.55680;,
 3.85720;9.01825;-9.21640;,
 1.92860;9.01825;-5.87595;,
 -2.77815;12.84775;-9.21640;,
 -3.85720;12.84775;-9.21640;,
 3.85720;12.84775;-9.21640;,
 1.92860;12.84775;-5.87595;,
 -4.10405;10.93315;-9.21640;,
 -2.05205;10.93315;-5.66215;,
 -1.92860;12.84775;-5.87595;,
 4.10405;10.93315;-9.21640;,
 2.05205;10.93315;-12.77060;,
 2.05205;10.93315;-5.66215;,
 -2.05205;10.93315;-12.77060;,
 -1.38910;12.84775;-6.81040;,
 -1.92860;9.01825;-5.87595;,
 -3.85720;9.01825;-9.21640;,
 -1.38910;11.46660;-6.81040;,
 -2.77815;11.46660;-9.21640;,
 1.38910;11.46660;-11.62235;,
 -1.38910;11.46660;-11.62235;;
 
 212;
 3;0,1,2;,
 3;3,4,5;,
 3;2,4,3;,
 3;6,7,8;,
 3;9,10,7;,
 3;1,6,5;,
 3;5,11,3;,
 3;1,7,6;,
 3;11,5,8;,
 3;9,1,0;,
 3;5,6,8;,
 3;5,4,1;,
 3;9,7,1;,
 3;0,12,9;,
 3;1,4,2;,
 3;10,13,7;,
 3;13,8,7;,
 3;12,10,9;,
 3;12,14,15;,
 3;11,8,13;,
 3;13,16,11;,
 3;17,2,3;,
 3;12,15,10;,
 3;15,14,17;,
 3;10,15,16;,
 3;14,2,17;,
 3;0,14,12;,
 3;14,0,2;,
 3;15,17,16;,
 3;10,16,13;,
 3;3,11,17;,
 3;17,11,16;,
 3;18,19,20;,
 3;20,19,21;,
 3;20,21,22;,
 3;22,21,23;,
 3;22,23,24;,
 3;24,23,25;,
 3;26,27,28;,
 3;28,27,29;,
 3;20,22,18;,
 3;18,22,24;,
 3;23,21,25;,
 3;25,21,19;,
 3;19,18,30;,
 3;26,30,18;,
 3;24,25,27;,
 3;31,27,25;,
 3;18,24,26;,
 3;27,26,24;,
 3;25,19,31;,
 3;30,31,19;,
 3;29,32,28;,
 3;28,32,33;,
 3;30,26,33;,
 3;33,26,28;,
 3;31,30,32;,
 3;32,30,33;,
 3;27,31,29;,
 3;29,31,32;,
 3;34,35,36;,
 3;36,35,37;,
 3;36,37,38;,
 3;38,37,39;,
 3;38,39,40;,
 3;40,39,41;,
 3;40,41,42;,
 3;42,41,43;,
 3;42,43,44;,
 3;44,43,45;,
 3;44,45,34;,
 3;34,45,35;,
 3;35,46,37;,
 3;37,46,47;,
 3;37,47,39;,
 3;39,47,48;,
 3;39,48,41;,
 3;41,48,49;,
 3;41,49,43;,
 3;43,49,50;,
 3;43,50,45;,
 3;45,50,51;,
 3;45,51,35;,
 3;35,51,46;,
 3;46,52,47;,
 3;47,52,53;,
 3;47,53,48;,
 3;48,53,54;,
 3;48,54,49;,
 3;49,54,55;,
 3;49,55,50;,
 3;50,55,56;,
 3;50,56,51;,
 3;51,56,57;,
 3;51,57,46;,
 3;46,57,52;,
 3;52,58,53;,
 3;53,58,59;,
 3;53,59,54;,
 3;54,59,60;,
 3;54,60,55;,
 3;55,60,61;,
 3;55,61,56;,
 3;56,61,62;,
 3;56,62,57;,
 3;57,62,63;,
 3;57,63,52;,
 3;52,63,58;,
 3;58,64,59;,
 3;59,64,65;,
 3;59,65,60;,
 3;60,65,66;,
 3;60,66,61;,
 3;61,66,67;,
 3;61,67,62;,
 3;62,67,68;,
 3;62,68,63;,
 3;63,68,69;,
 3;63,69,58;,
 3;58,69,64;,
 3;64,34,65;,
 3;65,34,36;,
 3;65,36,66;,
 3;66,36,38;,
 3;66,38,67;,
 3;67,38,40;,
 3;67,40,68;,
 3;68,40,42;,
 3;68,42,69;,
 3;69,42,44;,
 3;69,44,64;,
 3;64,44,34;,
 3;70,71,72;,
 3;72,71,73;,
 3;72,73,74;,
 3;74,73,75;,
 3;74,75,76;,
 3;76,75,77;,
 3;76,77,78;,
 3;78,77,79;,
 3;79,77,80;,
 3;80,77,81;,
 3;78,79,82;,
 3;82,79,83;,
 3;82,83,70;,
 3;70,83,71;,
 3;70,72,82;,
 3;72,74,82;,
 3;74,76,82;,
 3;76,78,82;,
 3;84,85,86;,
 3;86,85,87;,
 3;88,89,90;,
 3;90,89,91;,
 3;83,79,92;,
 3;92,79,80;,
 3;71,83,93;,
 3;93,83,92;,
 3;94,88,95;,
 3;95,88,90;,
 3;85,84,96;,
 3;96,84,97;,
 3;98,99,95;,
 3;95,99,100;,
 3;101,102,96;,
 3;96,102,91;,
 3;99,103,100;,
 3;100,103,97;,
 3;104,98,90;,
 3;90,98,95;,
 3;103,101,97;,
 3;97,101,96;,
 3;102,104,91;,
 3;91,104,90;,
 3;84,105,97;,
 3;97,105,100;,
 3;73,71,106;,
 3;106,71,93;,
 3;77,75,81;,
 3;81,75,107;,
 3;89,85,91;,
 3;91,85,96;,
 3;75,73,107;,
 3;107,73,106;,
 3;105,94,100;,
 3;100,94,95;,
 3;108,86,109;,
 3;86,87,109;,
 3;87,110,109;,
 3;110,111,109;,
 3;89,88,110;,
 3;110,88,111;,
 3;94,105,109;,
 3;109,105,108;,
 3;85,89,87;,
 3;87,89,110;,
 3;88,94,111;,
 3;111,94,109;,
 3;105,84,108;,
 3;108,84,86;,
 3;80,81,102;,
 3;102,81,104;,
 3;93,92,103;,
 3;103,92,101;,
 3;81,107,104;,
 3;104,107,98;,
 3;106,93,99;,
 3;99,93,103;,
 3;92,80,101;,
 3;101,80,102;,
 3;107,106,98;,
 3;98,106,99;;
 
 MeshMaterialList {
  2;
  212;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.621176;0.385882;0.250980;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.155294;0.096471;0.062745;;
  }
  Material {
   0.800000;0.329600;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.350000;0.144200;0.000000;;
  }
 }
 MeshNormals {
  152;
  -0.576098;-0.010080;0.817318;,
  -0.747923;0.136545;-0.649589;,
  0.979681;0.194120;-0.050427;,
  -0.739542;0.121480;-0.662057;,
  -0.229211;0.108787;-0.967279;,
  -0.194340;0.256987;-0.946673;,
  0.907369;0.357361;-0.221302;,
  0.924991;-0.375976;0.055078;,
  0.269283;-0.474511;0.838049;,
  -0.686234;-0.442084;0.577620;,
  0.633596;0.059778;0.771351;,
  0.299603;-0.447327;-0.842696;,
  0.617665;0.318211;-0.719188;,
  0.968849;0.014332;-0.247236;,
  -0.709976;0.451521;0.540428;,
  -0.814108;0.060657;0.577537;,
  -0.842377;-0.233839;-0.485511;,
  0.195980;-0.766224;0.611958;,
  0.000000;-1.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  -0.925819;-0.267267;0.267259;,
  -0.854223;0.493233;0.164391;,
  0.000000;1.000000;0.000000;,
  0.925819;0.267267;-0.267261;,
  0.854257;-0.493170;-0.164402;,
  -0.000019;-1.000000;0.000018;,
  -0.694359;0.267291;0.668147;,
  -0.284732;0.493263;0.821961;,
  0.694370;-0.267237;-0.668157;,
  0.284755;-0.493158;-0.822015;,
  -0.000019;-1.000000;0.000075;,
  0.231452;0.267306;0.935402;,
  0.569477;0.493246;0.657575;,
  -0.231462;-0.267227;-0.935423;,
  -0.569507;-0.493163;-0.657611;,
  0.000025;-1.000000;0.000050;,
  0.925811;0.267294;0.267258;,
  0.854229;0.493221;-0.164397;,
  -0.925829;-0.267231;-0.267261;,
  -0.854251;-0.493180;0.164403;,
  0.000000;-1.000000;0.000006;,
  0.694357;0.267276;-0.668155;,
  0.284739;0.493208;-0.821991;,
  -0.694365;-0.267256;0.668155;,
  -0.284748;-0.493212;0.821986;,
  -0.231448;0.267279;-0.935411;,
  -0.569483;0.493214;-0.657594;,
  0.231460;-0.267260;0.935413;,
  0.569497;-0.493194;0.657596;,
  0.000000;-0.044693;0.999001;,
  0.000000;-0.044693;0.999001;,
  -0.865161;-0.044691;0.499498;,
  -0.865169;-0.044690;0.499486;,
  -0.865163;-0.044692;-0.499496;,
  -0.865164;-0.044692;-0.499493;,
  0.000000;-0.044693;-0.999001;,
  0.865163;-0.044692;-0.499496;,
  0.865164;-0.044692;-0.499493;,
  -0.866034;0.000000;-0.499985;,
  -0.866034;0.000000;-0.499985;,
  -0.860676;0.110984;0.496909;,
  -0.865430;0.037199;0.499648;,
  -0.865430;-0.037187;0.499648;,
  0.865427;0.037199;-0.499653;,
  0.865427;-0.037187;-0.499653;,
  0.000000;0.037203;0.999308;,
  -0.865427;-0.037182;-0.499653;,
  -0.872669;0.064436;-0.484042;,
  -0.912314;0.391556;0.119861;,
  -0.664024;0.675181;0.321252;,
  0.330691;0.370708;0.867882;,
  0.221453;0.344355;0.912348;,
  0.215748;0.497151;0.840413;,
  0.325078;0.295591;-0.898304;,
  0.456549;0.199776;0.866979;,
  0.200304;0.190945;-0.960947;,
  -0.539125;0.576586;0.613916;,
  0.318728;0.185861;-0.929445;,
  -0.491042;-0.787832;-0.371750;,
  0.597233;-0.691939;-0.405626;,
  -0.190312;-0.695397;-0.692967;,
  0.854233;0.493213;0.164400;,
  0.569496;0.493195;-0.657597;,
  -0.000019;-1.000000;0.000046;,
  -0.854247;-0.493189;-0.164398;,
  -0.569481;-0.493234;0.657580;,
  -0.284749;0.493191;-0.821998;,
  0.000000;0.447214;-0.894427;,
  0.000025;-1.000000;0.000072;,
  0.284730;-0.493263;0.821961;,
  0.000000;-0.447276;0.894396;,
  -0.854239;0.493202;-0.164399;,
  -0.774596;0.447213;-0.447214;,
  0.000025;-1.000000;0.000021;,
  0.854234;-0.493212;0.164394;,
  0.774581;-0.447241;0.447212;,
  -0.569485;0.493227;0.657582;,
  -0.774588;0.447227;0.447215;,
  0.569495;-0.493181;-0.657607;,
  0.774596;-0.447207;-0.447221;,
  0.284743;0.493244;0.821968;,
  0.000000;0.447275;0.894397;,
  -0.284745;-0.493176;-0.822008;,
  0.000000;-0.447182;-0.894443;,
  -0.774583;0.447238;-0.447213;,
  0.774588;0.447227;0.447215;,
  0.774601;-0.447196;0.447223;,
  -0.774596;-0.447208;-0.447220;,
  -0.865154;-0.044693;0.499510;,
  -0.865162;-0.044692;-0.499498;,
  0.000000;-0.044693;-0.999001;,
  0.865162;-0.044693;-0.499497;,
  0.865154;-0.044689;0.499511;,
  0.865162;-0.044691;0.499498;,
  0.865169;-0.044692;0.499486;,
  -0.860679;0.110976;0.496906;,
  0.860676;0.110976;-0.496912;,
  0.860673;0.110984;-0.496915;,
  0.000000;-0.037185;0.999308;,
  0.000000;0.110978;0.993823;,
  0.000000;0.110978;0.993823;,
  -0.865426;0.037204;-0.499653;,
  -0.860676;0.110976;-0.496912;,
  -0.860675;0.110967;-0.496916;,
  0.865430;-0.037182;0.499648;,
  0.865429;0.037204;0.499647;,
  0.860679;0.110976;0.496906;,
  0.860678;0.110968;0.496910;,
  0.000000;-0.037186;-0.999308;,
  0.000000;0.037203;-0.999308;,
  0.000000;0.110979;-0.993823;,
  0.000000;0.000000;1.000000;,
  0.866034;0.000000;-0.499985;,
  0.866034;0.000000;-0.499985;,
  -0.866029;0.000000;0.499993;,
  -0.866029;0.000000;0.499993;,
  0.866029;0.000000;0.499993;,
  0.866029;0.000000;0.499993;,
  0.000000;0.000000;-1.000000;,
  0.000000;-0.110961;-0.993825;,
  0.860678;-0.110967;0.496910;,
  0.860681;-0.110959;0.496907;,
  -0.860675;-0.110967;-0.496916;,
  -0.860678;-0.110959;-0.496913;,
  -0.000000;-0.110961;0.993825;,
  0.860676;-0.110950;-0.496917;,
  0.860678;-0.110958;-0.496913;,
  -0.860680;-0.110950;0.496911;,
  -0.860681;-0.110958;0.496907;;
  212;
  3;0,14,15;,
  3;1,3,5;,
  3;70,3,1;,
  3;6,2,12;,
  3;10,7,2;,
  3;14,71,72;,
  3;5,4,1;,
  3;73,74,75;,
  3;76,5,12;,
  3;10,73,77;,
  3;5,78,12;,
  3;72,79,14;,
  3;10,74,73;,
  3;77,8,10;,
  3;14,79,15;,
  3;7,13,2;,
  3;13,12,2;,
  3;8,7,10;,
  3;8,9,17;,
  3;4,12,80;,
  3;80,11,4;,
  3;16,70,1;,
  3;8,17,7;,
  3;81,9,16;,
  3;7,82,11;,
  3;9,15,16;,
  3;0,9,8;,
  3;9,0,15;,
  3;81,83,11;,
  3;7,11,13;,
  3;1,4,16;,
  3;16,4,11;,
  3;20,20,20;,
  3;20,20,20;,
  3;21,21,21;,
  3;21,21,21;,
  3;22,22,22;,
  3;22,22,22;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;25,25,25;,
  3;25,25,25;,
  3;19,19,19;,
  3;19,19,19;,
  3;19,19,19;,
  3;19,19,19;,
  3;19,19,19;,
  3;19,19,19;,
  3;19,19,19;,
  3;19,19,19;,
  3;19,19,19;,
  3;19,19,19;,
  3;20,20,20;,
  3;20,20,20;,
  3;25,25,25;,
  3;25,25,25;,
  3;22,22,22;,
  3;22,22,22;,
  3;23,29,24;,
  3;24,29,30;,
  3;25,25,25;,
  3;25,25,25;,
  3;84,85,26;,
  3;26,85,31;,
  3;26,31,27;,
  3;27,31,32;,
  3;18,86,28;,
  3;28,86,33;,
  3;87,88,23;,
  3;23,88,29;,
  3;29,34,30;,
  3;30,34,35;,
  3;25,25,25;,
  3;25,25,25;,
  3;85,89,90;,
  3;90,89,36;,
  3;31,36,32;,
  3;32,36,37;,
  3;86,91,33;,
  3;33,91,38;,
  3;88,92,93;,
  3;93,92,34;,
  3;34,39,35;,
  3;35,39,40;,
  3;25,25,25;,
  3;25,25,25;,
  3;89,94,95;,
  3;95,94,41;,
  3;36,41,37;,
  3;37,41,42;,
  3;91,96,38;,
  3;38,96,43;,
  3;92,97,98;,
  3;98,97,39;,
  3;39,44,40;,
  3;40,44,45;,
  3;25,25,25;,
  3;25,25,25;,
  3;94,99,100;,
  3;100,99,46;,
  3;41,46,42;,
  3;42,46,47;,
  3;96,18,43;,
  3;43,18,18;,
  3;97,101,102;,
  3;102,101,44;,
  3;44,48,45;,
  3;45,48,49;,
  3;25,25,25;,
  3;25,25,25;,
  3;99,103,104;,
  3;104,103,50;,
  3;46,50,47;,
  3;47,50,51;,
  3;18,18,18;,
  3;18,18,18;,
  3;101,105,106;,
  3;106,105,48;,
  3;48,107,49;,
  3;49,107,24;,
  3;25,25,25;,
  3;25,25,25;,
  3;103,84,108;,
  3;108,84,26;,
  3;50,109,51;,
  3;51,109,27;,
  3;18,18,18;,
  3;18,18,28;,
  3;105,87,110;,
  3;110,87,23;,
  3;52,53,53;,
  3;53,53,53;,
  3;111,54,54;,
  3;54,54,55;,
  3;112,56,56;,
  3;56,56,57;,
  3;113,58,58;,
  3;58,58,58;,
  3;18,18,18;,
  3;18,18,18;,
  3;114,59,59;,
  3;59,59,60;,
  3;115,116,116;,
  3;116,116,117;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;61,62,62;,
  3;62,62,61;,
  3;25,25,25;,
  3;25,25,25;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;65,64,118;,
  3;118,64,63;,
  3;67,66,119;,
  3;119,66,120;,
  3;121,68,122;,
  3;122,68,123;,
  3;69,124,125;,
  3;125,124,126;,
  3;127,128,129;,
  3;129,128,130;,
  3;131,132,133;,
  3;133,132,133;,
  3;25,25,25;,
  3;25,25,25;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;25,25,25;,
  3;25,25,25;,
  3;18,18,18;,
  3;18,18,18;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;134,21,21;,
  3;21,21,134;,
  3;135,136,136;,
  3;136,136,135;,
  3;137,138,138;,
  3;138,138,137;,
  3;139,140,140;,
  3;140,140,139;,
  3;141,19,19;,
  3;19,19,141;,
  3;142,142,131;,
  3;131,142,132;,
  3;143,144,127;,
  3;127,144,128;,
  3;145,146,69;,
  3;69,146,124;,
  3;147,147,121;,
  3;121,147,68;,
  3;148,149,67;,
  3;67,149,66;,
  3;150,151,65;,
  3;65,151,64;;
 }
 MeshTextureCoords {
  112;
  0.298110;0.509940;,
  0.303530;0.478430;,
  0.327190;0.506100;,
  0.317370;0.505520;,
  0.322260;0.473130;,
  0.299260;0.458090;,
  0.289610;0.437470;,
  0.267680;0.480770;,
  0.269250;0.493410;,
  0.266300;0.528940;,
  0.260950;0.550720;,
  0.299080;0.516240;,
  0.283320;0.552970;,
  0.263920;0.518970;,
  0.316250;0.552380;,
  0.291590;0.576640;,
  0.281050;0.550420;,
  0.319000;0.550950;,
  0.711620;0.771092;,
  0.711620;0.228908;,
  0.711620;0.771092;,
  0.711620;0.228908;,
  0.288380;0.771092;,
  0.288380;0.228908;,
  0.288380;0.771092;,
  0.288380;0.228908;,
  0.529782;0.531338;,
  0.470218;0.531338;,
  0.529782;0.531338;,
  0.470218;0.531338;,
  0.529782;0.468662;,
  0.470218;0.468662;,
  0.470218;0.468662;,
  0.529782;0.468662;,
  0.366580;0.500000;,
  0.433290;0.500000;,
  0.379922;0.476892;,
  0.439962;0.476892;,
  0.406606;0.476892;,
  0.453304;0.476892;,
  0.419948;0.500000;,
  0.459974;0.500000;,
  0.406606;0.523110;,
  0.453304;0.523110;,
  0.379922;0.523110;,
  0.439962;0.523108;,
  0.566710;0.500000;,
  0.560038;0.476892;,
  0.546696;0.476892;,
  0.540026;0.500000;,
  0.546696;0.523110;,
  0.560038;0.523108;,
  0.633420;0.500000;,
  0.620078;0.476892;,
  0.593394;0.476892;,
  0.580052;0.500000;,
  0.593394;0.523110;,
  0.620078;0.523110;,
  0.566710;0.500000;,
  0.560038;0.476892;,
  0.546696;0.476892;,
  0.540026;0.500000;,
  0.546696;0.523110;,
  0.560038;0.523110;,
  0.433290;0.500000;,
  0.439962;0.476892;,
  0.453304;0.476892;,
  0.459974;0.500000;,
  0.453304;0.523110;,
  0.439962;0.523110;,
  0.533926;0.977006;,
  0.555564;0.139270;,
  0.466074;0.977006;,
  0.444436;0.139270;,
  0.432148;0.977006;,
  0.388874;0.139270;,
  0.466074;0.977006;,
  0.444436;0.139270;,
  0.533926;0.977006;,
  0.555564;0.139270;,
  0.577144;0.139270;,
  0.422856;0.139270;,
  0.567852;0.977006;,
  0.611126;0.139270;,
  0.555564;-0.013910;,
  0.611126;-0.013910;,
  0.555564;0.041336;,
  0.611126;0.041336;,
  0.444436;-0.013910;,
  0.555564;-0.013910;,
  0.422856;-0.013910;,
  0.577144;-0.013910;,
  0.654288;0.139270;,
  0.577144;0.139270;,
  0.388874;-0.013910;,
  0.345712;-0.013910;,
  0.654288;-0.013910;,
  0.577144;-0.013910;,
  0.335838;0.062674;,
  0.417918;0.062674;,
  0.422856;-0.013910;,
  0.664162;0.062674;,
  0.582082;0.062674;,
  0.582082;0.062674;,
  0.417918;0.062674;,
  0.444436;-0.013910;,
  0.422856;0.139270;,
  0.345712;0.139270;,
  0.444436;0.041336;,
  0.388874;0.041336;,
  0.555564;0.041336;,
  0.444436;0.041336;;
 }
}
