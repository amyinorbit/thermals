#version 410 core

out vec4 outColor;
uniform vec2 resolution;


struct ray {
    vec3 pos;
    vec3 dir;
};

ray createRay(vec2 uv, vec3 camPos, vec3 lookAt, float zoom) {
    vec3 f = normalize(lookAt - camPos);
    vec3 r = cross(vec3(0.0,1.0,0.0),f);
    vec3 u = cross(f,r);
    vec3 c=camPos+f*zoom;
    vec3 i=c+uv.x*r+uv.y*u;
    vec3 dir=i-camPos;
    return ray(camPos,normalize(dir));
}

float distToScene(vec3 p){
    return length(p-vec3(0.3,0.0,0.4))-0.3;
}
//Estimate normal based on distToScene function
const float EPS=0.001;
vec3 estimateNormal(vec3 p){
    float xPl=distToScene(vec3(p.x+EPS,p.y,p.z));
    float xMi=distToScene(vec3(p.x-EPS,p.y,p.z));
    float yPl=distToScene(vec3(p.x,p.y+EPS,p.z));
    float yMi=distToScene(vec3(p.x,p.y-EPS,p.z));
    float zPl=distToScene(vec3(p.x,p.y,p.z+EPS));
    float zMi=distToScene(vec3(p.x,p.y,p.z-EPS));
    float xDiff=xPl-xMi;
    float yDiff=yPl-yMi;
    float zDiff=zPl-zMi;
    return normalize(vec3(xDiff,yDiff,zDiff));
}

void main() {
    vec2 uv=gl_FragCoord.xy/resolution.xy;
    uv-=vec2(0.5);//offset, so center of screen is origin
    uv.x*=resolution.x/resolution.y;//scale, so there is no rectangular distortion

    vec3 camPos=vec3(2.0,1.0,0.5);
    vec3 lookAt=vec3(0.0);
    float zoom=1.0;

    ray camRay=createRay(uv,camPos,lookAt,zoom);

    float totalDist=0.0;
    float finalDist=distToScene(camRay.pos);
    int iters=0;
    int maxIters=100;
    for(iters=0;iters<maxIters&&finalDist>0.01;iters++){
        camRay.pos+=finalDist*camRay.dir;
        totalDist+=finalDist;
        finalDist=distToScene(camRay.pos);
    }
    vec3 normal=estimateNormal(camRay.pos);

    vec3 lightPos=vec3(2.0,1.0,1.0);

    float dotSN=dot(normal,normalize(lightPos-camRay.pos));

    outColor=vec4(0.5+0.5*normal,1.0)*dotSN;
}
