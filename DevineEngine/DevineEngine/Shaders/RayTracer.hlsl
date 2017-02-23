float4 backgroundColor = float4(0.0, 0.4, 0.4, 1);
float farPlane = 1000.0;
float4 LightColor = float4(1, 1, 1, 1);
float3 LightPos = float3(0, 100, 0);
float4 E = float4(0, 0, 50, 1); //eye position

#define NOBJECTS 4
struct Sphere
{
	float3 centre;
	float rad2; // radius* radius
	float4 color;
	float Kd, Ks, Kr, shininess;
};



struct VS_QUAD
{
	float4 Position : SV_POSITION; // vertex position
	float2 TextureUV : TEXCOORD0; // vertex texture coords
};

float4 sphereColor_1 = float4(1, 0, 0, 1); //sphere1 color
float4 sphereColor_2 = float4(0, 1, 0, 1); //sphere2 color
float4 sphereColor_3 = float4(0, 0, 1, 1); //sphere3 color
float4 sphereColor_4 = float4(1, 1, 1, 1);
float shininess = 40;

static Sphere object[NOBJECTS] =
{
//sphere 1
	{ 0.0, 0.0, 0.0, 1.0, sphereColor_1, 0.3, 0.5, 0.7, shininess },
//sphere 2
	{ 2.0, -1.0, 0.0, 0.25, sphereColor_2, 0.5, 0.7, 0.4, shininess },
//sphere 3
	{ -2.0, -2.0, 1.0, 2, backgroundColor, 0.5, 0.3, 0.3, shininess },
	//sphere 4
	{ 2.0, -8.0, 1.0, 20, sphereColor_4, 0.5, 0.3, 0.3, shininess }
};

struct Ray
{
	float3 o; // origin
	float3 d; // direction
};

float3 SphereNormal(Sphere s, float3 pos)
{
	return normalize(pos - s.centre);
}


float SphereIntersect(Sphere s, Ray ray, out bool hit)
{
	float t;
	float3 v = s.centre - ray.o;
	float A = dot(v, ray.d);
	float B = dot(v, v) - A * A;
	float R = sqrt(s.rad2);

	if (B > R * R)
	{
		hit = false;
		t = farPlane;
	}
	else
	{
		float disc = sqrt(R * R - B);
		t = A - disc;
		if (t < 0.0)
		{
			hit = false;
		}
		else
			hit = true;
	}
	return t;
}

bool AnyHit(Ray ray)
{
	bool anyhit = false;
	for (int i = 0; i < NOBJECTS; i++)
	{
		bool hit;
		float t = SphereIntersect(object[i], ray, hit);
		if (hit)
		{
			anyhit = true;
			
		}
	}
	return anyhit;
}

float3 NearestHit(Ray ray, out int hitobj, out bool anyhit)
{
	float mint = farPlane;
	hitobj = -1;
	anyhit = false;
	for (int i = 0; i < NOBJECTS; i++)
	{
		bool hit;
		float t = SphereIntersect(object[i], ray, hit);
		if (hit)
		{
			if (t < mint)
			{
				hitobj = i;
				mint = t;
				anyhit = true;
			}
		}
		//AnyHit(ray);
	}
	return ray.o + ray.d * mint;
}

float4 Phong(float3 n, float3 l, float3 v, float shininess, float4 diffuseColor, float4
specularColor)
{
	float NdotL = dot(n, l);
	float diff = saturate(NdotL);
	float3 r = reflect(l, n);
	float spec = pow(saturate(dot(v, r)), shininess) * (NdotL > 0.0);
	return diff * diffuseColor + spec * specularColor;
}

float4 Shade(float3 hitPos, float3 normal,
float3 viewDir, int hitobj, float lightIntensity)
{
	float3 lightDir = normalize(LightPos - hitPos);
	float4 diff = object[hitobj].color * object[hitobj].Kd;
	float4 spec = object[hitobj].color * object[hitobj].Ks;

	float shadowFactor = 1.0;
//fire shadow ray:
	Ray ShadowRay;
	ShadowRay.d = normalize(lightDir);
	ShadowRay.o = hitPos;
	if (AnyHit(ShadowRay))
		shadowFactor = 0.0;

	return shadowFactor * LightColor * lightIntensity * Phong(normal, lightDir, viewDir,
object[hitobj].shininess, diff, spec);
}

float4 RayTracing(VS_QUAD input) :SV_Target
{
	float g_pfWinWidth = 800;
	float g_pfWinHeight = 800;
	float2 xy = 0.01 * input.TextureUV * float2(g_pfWinWidth, g_pfWinHeight);
	float distEye2Canvas = 5.0;
	float3 PixelPos = float3(xy, distEye2Canvas); //pixel position in view space
//For each pixel, fire a ray from eye positon:
	Ray eyeray;;
	eyeray.o = E.xyz;
	eyeray.d = normalize(PixelPos - E.xyz);
	int hitobj;
	bool hit = false;
	float3 n;
	float4 c = (float4) 0;
	float lightInensity = 1.0;
//Specify the eyeray as the current ray:
	Ray currRay = eyeray;
	float3 i = NearestHit(eyeray, hitobj, hit);
	for (int depth = 1; depth < 4; depth++)
	{
		if (hit)
		{
 ////shade surface
			n = SphereNormal(object[hitobj], i);
			c += Shade(i, n, currRay.d, hitobj, lightInensity);
//// shoot refleced ray
			lightInensity *= object[hitobj].Kr;
			currRay.o = i;
			currRay.d = reflect(currRay.d, n);
			i = NearestHit(currRay, hitobj, hit);
			//
		}
		else
		{
			c += backgroundColor / depth / depth;
			
		}
	}
	float4 RTColor = c; //(float4)0;

	return RTColor;
}