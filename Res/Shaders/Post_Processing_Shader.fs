uniform vec2 resolution;
uniform float time;
uniform float vignette;
uniform float doublerating;
uniform float coef;
uniform sampler2D tex0;

void main(void)
{
	vec2 pos = gl_FragCoord.xy / resolution.xy;
	vec3 col;
	float a;

	if(pos.x > 1.0 - (doublerating * 2) || pos.x < doublerating * 2) 
		{ col.r = texture2D(tex0, vec2(pos.x * coef, pos.y * coef)).x; } 
	else 
		{ col.r = texture2D(tex0, vec2((pos.x + doublerating) * coef, pos.y * coef)).x; }

	if(pos.x < doublerating * 2 || pos.x > 1.0 - (doublerating * 2)) 
		{ col.b = texture2D(tex0, vec2(pos.x * coef, pos.y * coef)).z; } 
	else 
		{ col.b = texture2D(tex0, vec2((pos.x - doublerating) * coef, pos.y * coef)).z; }

	col.g = texture2D(tex0,vec2(pos.x, pos.y)).y;
	a = texture2D(tex0,vec2(pos.x, pos.y)).w;

	col = clamp(col*0.5+0.5*col*col*1.2,0.0,1.0);

	if(vignette == 1)
	{
		col *= 0.7 + 0.5*16.0*pos.x*pos.y*(1.0-pos.x)*(1.0-pos.y);
	}

	col *= vec3(0.8,1.0,0.7);

	//col *= 0.9+0.1*sin(7.0*time+pos.y*700.0);              

	col *= 0.97+0.03*sin(210.0*time);

	gl_FragColor = vec4(col, a);
}