#version 410

layout (location=0) in vec3 vp;
layout (location=1) in vec2 vt;

uniform vec3 pos;
uniform vec3 camPos;
uniform vec3 camScale;
uniform vec3 scale;

out vec2 texture_coordinates;

void main(){
    vec3 finalPos;
    // Calculation of Local Position....
    finalPos.x = vp.x * scale.x * camScale.x;
    finalPos.y = vp.y * scale.y * camScale.y;
    finalPos.z = vp.z * scale.z * camScale.z;

    // Transformation into World Position....
    finalPos.x += (pos.x - camPos.x) * camScale.x;
    finalPos.y += (pos.y - camPos.y) * camScale.y;
    finalPos.z += (pos.z - camPos.z) * camScale.z;

    gl_Position = vec4(finalPos.x, finalPos.y, finalPos.z, 1.0f);
    texture_coordinates = vt;
}
