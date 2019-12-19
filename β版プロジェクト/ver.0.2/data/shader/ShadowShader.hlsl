//==========================================================================
//
// 影シェーダ	[ShadowShader.hlsl]
// Author : Ishida Takuto
//
//==========================================================================
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	float4 position		: POSITION;		// 頂点座標
	float3 normal		: NORMAL;		// 法線
	float4 uv			: TEXCOORD;		// テクスチャ座標
} InputVertex;

typedef struct
{
	float4 position		: POSITION;		// 頂点座標
	float3 normal		: NORMAL;		// 法線
	float4 uv			: TEXCOORD;		// テクスチャ座標
} OutputVertex;

// =========================================================================
// 変数
// =========================================================================
float4x4 g_matrix;						// 合成用マトリクス
float4 g_lightDir;						// ライトの方向
sampler g_mainTexture : register(s0);	// サンプラー(s)の0番を指定
sampler g_rampTexture : register(s1);	// サンプラー(s)の1番を指定

// =========================================================================
// 頂点関数
// =========================================================================
OutputVertex Vertex(InputVertex input)
{
	// 構造体の初期化
	OutputVertex output = (OutputVertex)0;

	// 座標
	output.position = mul(input.position, g_matrix);

	// 法線
	output.normal = input.normal;

	// UV
	output.uv = input.uv;

	return output;
}

// =========================================================================
// ピクセル関数
// =========================================================================
float4 Pixel(OutputVertex input) : COLOR0
{
	float x = dot(input.normal, -g_lightDir) * 0.5 + 0.5;
	float4 ramp = tex2D(g_rampTexture, float2(x, 0.0));		// ランプテクスチャの計算
	float4 color = tex2D(g_mainTexture, input.uv) * ramp;	// 元々のテクスチャに影をつける
	return color;
}

// =========================================================================
// テクニック宣言
// =========================================================================
technique StandardShader
{
	pass Pass0
	{
		vertexShader = compile vs_3_0 Vertex();	// 頂点関数
		pixelShader = compile ps_3_0 Pixel();	// ピクセル関数
	}
}