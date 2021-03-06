#version 330 core

in vec3 vFragPosition;
in vec3 vFragNormal;
in vec2 vTexCoords;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;

uniform sampler2D uTexture;

out vec4 fFragColor;


vec3 blinnPhong() {
    float d = distance(vFragPosition, uLightPos_vs);
    return (uLightIntensity*pow(exp(-d),2)+0.1)*(vec3(texture(uTexture, vTexCoords))*dot(normalize(uLightPos_vs - vFragPosition), vFragNormal)
            + uKs * pow(dot(normalize((-vFragPosition+uLightPos_vs - vFragPosition)/2), vFragNormal),uShininess));
}

void main() {
//   fFragColor = vec4(vFragNormal+vec3(0.2,0.2,0.2),1);
  fFragColor = vec4(blinnPhong().xyz,1);
//   fFragColor = texture(uTexture, vTexCoords);
//   fFragColor = vec4(vTexCoords,1,1);
//   fFragColor = vec4(uKd,1);
}
