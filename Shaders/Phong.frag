#version 330

in vec3 fragNormal;
in vec3 fragWorldPos;
out vec4 outColor;

struct DirectionalLight {
	vec3 lDirection;
	vec3 lDiffuseColor;
	vec3 lSpecColor;
};

uniform vec3 uCameraPos;
uniform float uSpecPower;
uniform vec3 uAmbientLight;
uniform DirectionalLight uDirLight;

void main() {
	vec3 N = normalize(fragNormal);
	vec3 L = normalize(-uDirLight.lDirection);
	vec3 V = normalize(uCameraPos - fragWorldPos);
	vec3 R = normalize(reflect(-L, N));

	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);

	if (NdotL > 0) {
		vec3 Diffuse = uDirLight.lDiffuseColor * NdotL;
		vec3 Specular = uDirLight.lSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

	outColor = vec4(0.0, 0.0, 1.0, 1.0) * vec4(Phong, 1.0f);
}
