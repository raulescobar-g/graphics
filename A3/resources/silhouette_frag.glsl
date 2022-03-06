#version 120


varying vec3 normal;
varying vec3 position;

void main()
{
	vec3 n = normalize(normal);
	vec3 eye = -1 * normalize(position);

	if (dot(n,eye) >= 0.3) {
		gl_FragColor = vec4(1.0,1.0,1.0, 1.0);
	}
	else {
		gl_FragColor = vec4(0.0,0.0,0.0, 1.0);
	}
	
	
}

