
vulkan: vulkan.cpp shader.vert shader.frag
	g++ vulkan.cpp -g -lvulkan -lglfw -o $@
	mkdir -p shaders
	glslangValidator -V100 shader.vert  -o shaders/vert.spv
	glslangValidator -V100 shader.frag  -o shaders/frag.spv
