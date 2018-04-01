struct VS_Output {
	float4 position : SV_POSITION;
	float4 color : TEXCOORD;
};
struct PS_Input {
	float4 position : SV_POSITION;
	float4 color : TEXCOORD;
};

VS_Output vs_main(uint id : SV_VertexID) {
	VS_Output output;
	if(id == 0) {
		output.position = float4(0.0f,1.0f,0.0f,1.0f);
		output.color = float4(1.0f,0.0f,0.0f,1.0f);
	} else if(id == 1) {
		output.position = float4(1.0f,-1.0f,0.0f,1.0f);
		output.color = float4(0.0f,1.0f,0.0f,1.0f);
	} else {
		output.position = float4(-1.0f,-1.0f,0.0f,1.0f);
		output.color = float4(0.0f,0.0f,1.0f,1.0f);
	}
	return output;
}


float4 ps_main(PS_Input input) : SV_TARGET{
	return input.color;
}