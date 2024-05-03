#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout(binding=1) uniform sampler2D PrimaryTex;
layout(binding=2) uniform sampler2D SecondaryTex;

layout (location = 0) out vec4 FragColor;

uniform struct SpotLightInfo {
	vec3 Position; //light position
	vec3 La; // ambient
	vec3 L; // intensity
	vec3 Direction; // direction
	float Exponent; // exponent
	float Cutoff;	// cutoff
} Spot;

uniform struct LightInfo {
	vec4 Position; //light position
	vec3 La; // light ambient
	vec3 L; // light intensity
} lights[3];

uniform struct MaterialInfo {
	vec3 Ka; //material ambient
	vec3 Kd; //material diffuse
	vec3 Ks; //material specular
	float Shininess;
	int TexDetail;
} Material;

uniform struct FogInfo 
{
	float MaxDist; // maximum distance
	float MinDist; // minimum distance
	vec3 Color; // Colour of course 
} Fog;

const int levels=4; //shader levels for toon shading
const float scaleFactor=1.0/float(levels); //scale factor for toon shading

//SpotLight shader
vec3 phongSpot( vec3 position, vec3 n){
	vec3 diffuse=vec3(0.0), spec=vec3(0);
	vec3 ambient=Spot.La*Material.Ka;

	vec3 s=normalize(vec3(Spot.Position.xyz) - position);

	float cosAng=dot(-s,normalize(Spot.Direction));
	float angle=acos(cosAng);

	float spotScale;

	if (angle>=0.0&&angle<Spot.Cutoff)
	{
		spotScale=pow(cosAng,Spot.Exponent);
		float sDotN=max(dot(s,n),0.0);
		diffuse=Material.Kd*floor(sDotN*levels)*scaleFactor;
		if(sDotN>0.0){
			vec3 v=normalize(-position.xyz);
			vec3 h=normalize(s+v);
			spec=Material.Ks*pow(max(dot(h,n),0.0),Material.Shininess);
		}
	}
	return ambient+spotScale*(diffuse+spec)*Spot.L;
}

//Shading
vec3 phongAmbient(int light, vec3 position, vec3 n){
	//define diffuse, spec, ambient
	vec3 diffuse = vec3(0.0), spec = vec3(0.0);

	vec3 MatColor;
	//choose the texture
	switch (Material.TexDetail)
	{
		case 0:
			MatColor = Material.Ka;
			break;
		case 1:
			vec4 PrimaryTexColor = texture(PrimaryTex, TexCoord);
			MatColor = PrimaryTexColor.rgb;
			break;
		case 2:
			PrimaryTexColor = texture(PrimaryTex, TexCoord);
			vec4 SecondaryTexColor = texture(SecondaryTex, TexCoord);
			MatColor = mix(PrimaryTexColor.rgb, SecondaryTexColor.rgb, SecondaryTexColor.a);
			break;
		case 3:
			SecondaryTexColor = texture(SecondaryTex, TexCoord);
			MatColor = SecondaryTexColor.rgb;
			break;
	}

	//calculate the ambient
	vec3 ambient=lights[light].La*MatColor;
	//calculate the light direction
    vec3 s=normalize(vec3(lights[light].Position.xyz) - position);
	//calculate the diffuse and specular
    float sDotN=max(dot(s,n),0.0);
	//calculate the diffuse using toon shading
	diffuse=MatColor*floor(sDotN*levels)*scaleFactor;

	if(sDotN>0.0){
		vec3 v=normalize(-position.xyz);
		vec3 r=reflect(-s,n);
		spec=Material.Ks*pow(max(dot(r,v),0.0),Material.Shininess);
	}

    return ambient+(diffuse+spec)*lights[light].L;
}

void main() {

	float dist = abs(Position.z);
	float fogFactor=(Fog.MaxDist-dist)/(Fog.MaxDist-Fog.MinDist);
	fogFactor=clamp(fogFactor,0.0,1.0);

	vec3 shadeColor = vec3(0.0);
	for(int i=0;i<3;i++)
	{
		shadeColor+=phongAmbient(i,Position,normalize(Normal));	
	}	 
	shadeColor+=phongSpot(Position,normalize(Normal));

	vec3 Color=mix(Fog.Color,shadeColor,fogFactor);
	FragColor = vec4(Color, 1.0);
}
