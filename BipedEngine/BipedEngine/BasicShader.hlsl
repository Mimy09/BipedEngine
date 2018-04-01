cbuffer ConstantBuffer : register(b0) {
	matrix World;
	matrix View;
	matrix Projection;
}

struct VS_Output{
	float4 position : SV_POSITION;
	//float4 color : COLOR;
};

struct PS_Input {
	float4 position : POSITION;
	//float4 color : COLOR;
};

VS_Output vs_main(PS_Input input)
{
	VS_Output output = (VS_Output)0;
	output.position = input.position;
	//output.color = input.color;

	return output;
}


float4 ps_main(PS_Input input) : SV_TARGET
{
	return float4(0.0f, 1.0f, 0.0f, 1.0f);
	//return input.color;
}