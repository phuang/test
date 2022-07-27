
vulkan_demo: vulkan_demo.cpp shader.vert shader.frag
	g++ vulkan_demo.cpp -lvulkan -lglfw -o $@
	mkdir -p shaders
	glslangValidator -V100 shader.vert  -o shaders/vert.spv
	glslangValidator -V100 shader.frag  -o shaders/frag.spv
