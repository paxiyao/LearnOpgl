#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{    
	/*
		ע�⣬����������ı�Ե��ʱ��OpenGL��Ա�Ե��ֵ��������һ���ظ���ֵ���в�ֵ����Ϊ���ǽ����Ļ��Ʒ�ʽ����Ϊ��GL_REPEAT��
		��ͨ����û����ģ�������������ʹ����͸��ֵ������ͼ��Ķ���������ײ���Ե�Ĵ�ɫֵ���в�ֵ�������Ľ����һ����͸������ɫ
		�߿�����ܻῴ������������������ı��Ρ�Ҫ����������ÿ����alpha�����ʱ���뽫����Ļ��Ʒ�ʽ����ΪGL_CLAMP_TO_EDGE��
	*/
	vec4 texColor = texture(texture1, TexCoords);
	if(texColor.a < 0.1)
	{
		discard;
	}
    FragColor = texColor;
}