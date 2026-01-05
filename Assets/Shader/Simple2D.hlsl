//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0);	// テクスチャー
SamplerState g_sampler : register(s0); // テクスチャーサンプラー

//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
	matrix g_matWorld;		// 頂点座標変換行列
	matrix g_matTexture;	// テクスチャ座標変換行列
	float4 g_vecColor;		// テクスチャ合成色
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUTPUT
{
	float4 pos	: SV_POSITION;	// 位置
	float2 uv	: TEXCOORD;		// UV座標
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUTPUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)
{
	VS_OUTPUT output;
	
	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけてスクリーン座標に変換
	output.pos = mul(pos, g_matWorld);
	
	//テクスチャ座標に変換行列をかける(テクスチャの表示位置・拡大・回転等を制御)
	output.uv = mul(uv, g_matTexture);
	
	//まとめて出力
	return output;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUTPUT input) : SV_Target
{
	//UV座標の位置からテクスチャの色を1ピクセル分取り出し(g_texture.Sample)
	//その色に合成色(g_vecColor)をかけて最終的なピクセルの色を計算して返す
	return g_vecColor * g_texture.Sample(g_sampler, input.uv);
}