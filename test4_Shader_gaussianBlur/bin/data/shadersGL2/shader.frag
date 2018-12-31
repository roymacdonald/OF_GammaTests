#version 120

uniform sampler2DRect tex0;

uniform vec2 mousePos;

varying vec2 texCoordVarying;



void main()
{


	gl_FragColor = texture2DRect(tex0, mousePos);
}
