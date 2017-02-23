

//--------------------------------------------------------------------------------------
// RAY TRACING 
//--------------------------------------------------------------------------------------

struct VS_QUAD
{
	float4 Position : SV_POSITION; // vertex position
	float2 TextureUV : TEXCOORD0; // vertex texture coords
};

VS_QUAD RenderSceneVS_RT(float4 vPos : POSITION)
{
	VS_QUAD Output;
	Output.Position = float4(sign(vPos.xy), 0, 1);
	Output.TextureUV = sign(vPos.xy);
	return Output;
}



//this is raycasting
//float4 RayCasting(VS_QUAD input)
//{
//	//___________________________________
//	// 1. specify canvas size
//	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	float WinWidth = 800, WinHeight = 800;
//	float2 xy = 0.02*input.TextureUV*float2(WinWidth, WinHeight);
//	float distEye2Canvas = 2.0;
//	float3 PixelPos = float3(xy, distEye2Canvas);
//	//___________________________________
//	//2. for each pixel location (x,y), fire a ray
//	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//	Ray eyeray;
//	eyeray.o = E.xyz; //eye position specified in world space
//	eyeray.d = normalize(PixelPos - E.xyz); //view direction in world space
//											//___________________________________
//											//3. Calculate ray-sphere hit position
//											//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	bool hit = false;
//	float t = SphereIntersect(eyeray, hit);
//	float3 interP = eyeray.o + t*normalize(eyeray.d);
//	//___________________________________
//	//4. Render
//	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	float4 RTColor = (float4)0;
//	if (!hit)
//		RTColor = backgroundColor;
//	else
//	{
//		float3 c = LightColor.rgb;
//		float3 N = normalize(interP);
//		N = normalize(N);
//		float3 L = normalize(LightPos - interP);
//		float3 V = normalize(E.xyz - interP);
//		float3 R = reflect(-L, N);
//		float r = max(dot(N, L), 0.2);
//		r += 0.6*pow(max(0.1, dot(R, V)), 50);
//		RTColor = float4(r*c, 1.0);
//	}
//	return RTColor;
//}




