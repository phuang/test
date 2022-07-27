vulkan_demo: vulkan_demo.cpp shader.vert shader.frag
	g++ -DVULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 vulkan_demo.cpp -ldl -lvulkan -lglfw -o $@
	mkdir -p shaders
	glslangValidator -V100 shader.vert  -o shaders/vert.spv
	glslangValidator -V100 shader.frag  -o shaders/frag.spv

clean:
	rm -f vulkan_demo vulkan_demo.o