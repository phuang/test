
vulkan: vulkan.cpp shader.vert shader.frag
	g++ vulkan.cpp -std=c++17 -lvulkan -lglfw -o $@
	mkdir -p shaders
	glslc  shader.vert  -o shaders/vert.spv
	glslc  shader.frag  -o shaders/frag.spv
