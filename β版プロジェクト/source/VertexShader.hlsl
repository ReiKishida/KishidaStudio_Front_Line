// -------------------------------------------------------------
// シャドウボリュームによる影
//
// Copyright (c) 2002 IMAGIRE Takashi. All rights reserved.
// -------------------------------------------------------------

// -------------------------------------------------------------
// グローバル変数
// -------------------------------------------------------------
float4x4 mWVP;      // ローカルから射影空間への座標変換
float4   vLightPos; // ライトの位置

// -------------------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos      : POSITION;
};

// -------------------------------------------------------------
// 頂点シェーダプログラム
// -------------------------------------------------------------
VS_OUTPUT VS (
      float4 Pos    : POSITION,          // モデルの頂点
      float3 Normal : NORMAL             // モデルの法線
){
    VS_OUTPUT Out = (VS_OUTPUT)0;        // 出力データ

    // 光の裏になっている面を後ろに引き伸ばす
    float4 dir = vLightPos - Pos;
    float LN = dot( Normal, dir );
    float scale = (0<LN) ? 0.0f : 1.0f;

    // 座標変換
    Pos.xyz -= 0.001f*Pos;// 縞がおきないように少し縮める
    Out.Pos = mul( Pos - scale * dir, mWVP );

    return Out;
}
// -------------------------------------------------------------
// テクニック
// -------------------------------------------------------------
technique TShader
{
    pass P0
    {
        // シェーダ
        VertexShader = compile vs_1_1 VS();
        PixelShader  = NULL;
    }
}