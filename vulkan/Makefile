
vulkan: vulkan.cpp shader.vert shader.frag
	g++ vulkan.cpp -lvulkan -lglfw -o $@
	glslc  shader.vert  -o shaders/vert.spv
	glslc  shader.frag  -o shaders/frag.spv