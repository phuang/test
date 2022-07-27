
#include "vulkan_demo.h"
#include "vertex.h"

const int WIDTH = 800;
const int HEIGHT = 600;

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char *> kValidationLayers = {
    "VK_LAYER_KHRONOS_validation"};

const std::vector<const char *> kDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
const bool kEnableValidationLayers = false;
#else
const bool kEnableValidationLayers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pCallback)
{
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr)
  {
    return func(instance, pCreateInfo, pAllocator, pCallback);
  }
  else
  {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT callback,
                                   const VkAllocationCallbacks *pAllocator)
{
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr)
  {
    func(instance, callback, pAllocator);
  }
}

//   0------3
//   | \    |
//   |  \   |
//   |   \  |
//   |    \ |
//   2------1
const std::vector<Vertex> kVertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}},
};

struct UniformBufferObject
{
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};

const std::array<uint16_t, 6> kIndices = {0, 1, 2, 0, 3, 1};

void VulkanDemo::run()
{
  InitWindow();
  InitVulkan();
  MainLoop();
  Cleanup();
}

void VulkanDemo::InitWindow()
{
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  window_ = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
  glfwSetWindowUserPointer(window_, this);
  glfwSetFramebufferSizeCallback(window_, FramebufferResizeCallback);
}

void VulkanDemo::FramebufferResizeCallback(GLFWwindow *window, int width,
                                           int height)
{
  auto app = reinterpret_cast<VulkanDemo *>(
      glfwGetWindowUserPointer(window));
  app->framebuffer_resized_ = true;
}

void VulkanDemo::InitVulkan()
{
  CreateInstance();
  SetupDebugCallback();
  CreateSurface();
  PickPhysicalDevice();
  CreateLogicalDevice();
  CreateSwapChain();
  CreateImageViews();
  CreateRenderPass();
  CreateDescriptorSetLayout();
  CreateGraphicsPipeline();
  CreateFramebuffers();
  CreateCommandPool();
  CreateTextureImage();
  CreateTextureImageView();
  CreateVertexBuffer();
  CreateIndexBuffer();
  CreateUniformBuffers();
  CreateDescriptorPool();
  CreateDescriptorSets();
  CreateCommandBuffers();
  CreateSyncObjects();
}

void VulkanDemo::MainLoop()
{
  while (!glfwWindowShouldClose(window_))
  {
    glfwPollEvents();
    DrawFrame();
  }

  vkDeviceWaitIdle(device_);
}

void VulkanDemo::CleanupSwapChain()
{
  for (auto framebuffer : swap_chain_framebuffers_)
  {
    vkDestroyFramebuffer(device_, framebuffer, nullptr);
  }

  vkFreeCommandBuffers(device_, command_pool_,
                       static_cast<uint32_t>(command_buffers_.size()),
                       command_buffers_.data());

  vkDestroyPipeline(device_, graphics_pipeline_, nullptr);
  vkDestroyPipelineLayout(device_, pipeline_layout_, nullptr);
  vkDestroyRenderPass(device_, render_pass_, nullptr);

  for (auto image_view : swap_chain_image_views_)
  {
    vkDestroyImageView(device_, image_view, nullptr);
  }

  vkDestroySwapchainKHR(device_, swap_chain_, nullptr);

  for (size_t i = 0; i < swap_chain_images_.size(); i++)
  {
    vkDestroyBuffer(device_, uniform_buffers_[i], nullptr);
    vkFreeMemory(device_, uniform_buffers_memory_[i], nullptr);
  }

  vkDestroyDescriptorPool(device_, descriptor_pool_, nullptr);
}

void VulkanDemo::Cleanup()
{
  CleanupSwapChain();

  vkDestroyBuffer(device_, vertex_buffer_, nullptr);
  vkFreeMemory(device_, vertex_buffer_memory_, nullptr);
  vkDestroyBuffer(device_, index_buffer_, nullptr);
  vkFreeMemory(device_, index_buffer_memory_, nullptr);
  vkDestroyDescriptorSetLayout(device_, descriptor_set_layout_, nullptr);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    vkDestroySemaphore(device_, render_finished_semaphores_[i], nullptr);
    vkDestroySemaphore(device_, image_available_semaphores_[i], nullptr);
    vkDestroyFence(device_, in_flight_fences_[i], nullptr);
  }

  vkDestroyCommandPool(device_, command_pool_, nullptr);

  vkDestroyDevice(device_, nullptr);

  if (kEnableValidationLayers)
  {
    DestroyDebugUtilsMessengerEXT(instance_, callback_, nullptr);
  }

  vkDestroySurfaceKHR(instance_, surface_, nullptr);
  vkDestroyInstance(instance_, nullptr);

  glfwDestroyWindow(window_);

  glfwTerminate();
}

void VulkanDemo::RecreateSwapChain()
{
  int width = 0, height = 0;
  while (width == 0 || height == 0)
  {
    glfwGetFramebufferSize(window_, &width, &height);
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(device_);

  CleanupSwapChain();

  CreateSwapChain();
  CreateImageViews();
  CreateRenderPass();
  CreateGraphicsPipeline();
  CreateFramebuffers();
  CreateUniformBuffers();
  CreateDescriptorPool();
  CreateDescriptorSets();
  CreateCommandBuffers();
}

void VulkanDemo::CreateInstance()
{
  if (kEnableValidationLayers && !CheckValidationLayerSupport())
  {
    throw std::runtime_error(
        "validation layers requested, but not available!");
  }

  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_2;

  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  auto extensions = GetRequiredExtensions();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  if (kEnableValidationLayers)
  {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(kValidationLayers.size());
    createInfo.ppEnabledLayerNames = kValidationLayers.data();
  }
  else
  {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateInstance(&createInfo, nullptr, &instance_) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create instance!");
  }
}

void VulkanDemo::SetupDebugCallback()
{
  if (!kEnableValidationLayers)
    return;

  VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = DebugCallback;

  if (CreateDebugUtilsMessengerEXT(instance_, &createInfo, nullptr,
                                   &callback_) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to set up debug callback!");
  }
}

void VulkanDemo::CreateSurface()
{
  if (glfwCreateWindowSurface(instance_, window_, nullptr, &surface_) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to create window surface!");
  }
}

void VulkanDemo::PickPhysicalDevice()
{
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);

  if (deviceCount == 0)
  {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());

  for (const auto &device : devices)
  {
    if (IsDeviceSuitable(device))
    {
      physical_device_ = device;
      break;
    }
  }

  if (physical_device_ == VK_NULL_HANDLE)
  {
    throw std::runtime_error("failed to find a suitable GPU!");
  }
}

void VulkanDemo::CreateLogicalDevice()
{
  QueueFamilyIndices indices = FindQueueFamilies(physical_device_);

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<int> uniqueQueueFamilies = {indices.graphics_family,
                                       indices.present_family};

  float queuePriority = 1.0f;
  for (int queueFamily : uniqueQueueFamilies)
  {
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures = {};

  VkDeviceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

  createInfo.queueCreateInfoCount =
      static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();

  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(kDeviceExtensions.size());
  createInfo.ppEnabledExtensionNames = kDeviceExtensions.data();

  if (kEnableValidationLayers)
  {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(kValidationLayers.size());
    createInfo.ppEnabledLayerNames = kValidationLayers.data();
  }
  else
  {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(physical_device_, &createInfo, nullptr, &device_) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to create logical device!");
  }

  vkGetDeviceQueue(device_, indices.graphics_family, 0, &graphics_queue_);
  vkGetDeviceQueue(device_, indices.present_family, 0, &present_queue_);
}

void VulkanDemo::CreateSwapChain()
{
  SwapChainSupportDetails swapChainSupport =
      QuerySwapChainSupport(physical_device_);

  VkSurfaceFormatKHR surfaceFormat =
      ChooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode =
      ChooseSwapPresentMode(swapChainSupport.present_modes);
  VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount)
  {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface_;

  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices = FindQueueFamilies(physical_device_);
  uint32_t queueFamilyIndices[] = {(uint32_t)indices.graphics_family,
                                   (uint32_t)indices.present_family};

  if (indices.graphics_family != indices.present_family)
  {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  }
  else
  {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }

  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;

  if (vkCreateSwapchainKHR(device_, &createInfo, nullptr, &swap_chain_) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to create swap chain!");
  }

  vkGetSwapchainImagesKHR(device_, swap_chain_, &imageCount, nullptr);
  swap_chain_images_.resize(imageCount);
  vkGetSwapchainImagesKHR(device_, swap_chain_, &imageCount,
                          swap_chain_images_.data());

  swap_chain_image_format_ = surfaceFormat.format;
  swap_chain_extent_ = extent;
}

void VulkanDemo::CreateImageViews()
{
  swap_chain_image_views_.resize(swap_chain_images_.size());

  for (size_t i = 0; i < swap_chain_images_.size(); i++)
  {
    swap_chain_image_views_[i] = createImageView(swap_chain_images_[i], swap_chain_image_format_);
  }
}

void VulkanDemo::CreateRenderPass()
{
  VkAttachmentDescription color_attachment = {};
  color_attachment.format = swap_chain_image_format_;
  color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference color_attachment_ref = {};
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass = {};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_attachment_ref;

  VkSubpassDependency dependency = {};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                             VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkRenderPassCreateInfo render_pass_info = {};
  render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  render_pass_info.attachmentCount = 1;
  render_pass_info.pAttachments = &color_attachment;
  render_pass_info.subpassCount = 1;
  render_pass_info.pSubpasses = &subpass;
  render_pass_info.dependencyCount = 1;
  render_pass_info.pDependencies = &dependency;

  if (vkCreateRenderPass(device_, &render_pass_info, nullptr,
                         &render_pass_) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create render pass!");
  }
}

void VulkanDemo::CreateDescriptorSetLayout()
{
  VkDescriptorSetLayoutBinding ubo_layout_binding{};
  ubo_layout_binding.binding = 0;
  ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  ubo_layout_binding.descriptorCount = 1;
  ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  ubo_layout_binding.pImmutableSamplers = nullptr; // Optional

  VkDescriptorSetLayoutCreateInfo layout_info{};
  layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layout_info.bindingCount = 1;
  layout_info.pBindings = &ubo_layout_binding;

  if (vkCreateDescriptorSetLayout(device_, &layout_info, nullptr,
                                  &descriptor_set_layout_) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create descriptor set layout!");
  }
}

void VulkanDemo::CreateGraphicsPipeline()
{
  auto vert_shader_code = ReadFile("shaders/vert.spv");
  auto frag_shader_code = ReadFile("shaders/frag.spv");

  VkShaderModule vert_shader_module = CreateShaderModule(vert_shader_code);
  VkShaderModule frag_shader_module = CreateShaderModule(frag_shader_code);

  VkPipelineShaderStageCreateInfo vert_shader_stage_info = {};
  vert_shader_stage_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vert_shader_stage_info.module = vert_shader_module;
  vert_shader_stage_info.pName = "main";

  VkPipelineShaderStageCreateInfo frag_shader_stage_info = {};
  frag_shader_stage_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  frag_shader_stage_info.module = frag_shader_module;
  frag_shader_stage_info.pName = "main";

  VkPipelineShaderStageCreateInfo shader_stages[] = {vert_shader_stage_info,
                                                     frag_shader_stage_info};

  VkPipelineVertexInputStateCreateInfo vertex_input_info = {};
  vertex_input_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

  auto binding_description = Vertex::GetBindingDescription();
  vertex_input_info.vertexBindingDescriptionCount = 1;
  vertex_input_info.pVertexBindingDescriptions = &binding_description;

  auto attribute_descriptions = Vertex::GetAttributeDescriptions();
  vertex_input_info.vertexAttributeDescriptionCount =
      attribute_descriptions.size();
  vertex_input_info.pVertexAttributeDescriptions =
      attribute_descriptions.data();

  VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
  inputAssembly.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  VkViewport viewport = {};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)swap_chain_extent_.width;
  viewport.height = (float)swap_chain_extent_.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor = {};
  scissor.offset = {0, 0};
  scissor.extent = swap_chain_extent_;

  VkPipelineViewportStateCreateInfo viewport_state = {};
  viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state.viewportCount = 1;
  viewport_state.pViewports = &viewport;
  viewport_state.scissorCount = 1;
  viewport_state.pScissors = &scissor;

  VkPipelineRasterizationStateCreateInfo rasterizer = {};
  rasterizer.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;

  VkPipelineMultisampleStateCreateInfo multisampling = {};
  multisampling.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

  VkPipelineColorBlendAttachmentState color_blend_attachment = {};
  color_blend_attachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  color_blend_attachment.blendEnable = VK_FALSE;

  VkPipelineColorBlendStateCreateInfo color_blending = {};
  color_blending.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blending.logicOpEnable = VK_FALSE;
  color_blending.logicOp = VK_LOGIC_OP_COPY;
  color_blending.attachmentCount = 1;
  color_blending.pAttachments = &color_blend_attachment;
  color_blending.blendConstants[0] = 0.0f;
  color_blending.blendConstants[1] = 0.0f;
  color_blending.blendConstants[2] = 0.0f;
  color_blending.blendConstants[3] = 0.0f;

  VkPipelineLayoutCreateInfo pipeline_layout_info = {};
  pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipeline_layout_info.setLayoutCount = 1;
  pipeline_layout_info.pSetLayouts = &descriptor_set_layout_;
  pipeline_layout_info.pushConstantRangeCount = 0;

  if (vkCreatePipelineLayout(device_, &pipeline_layout_info, nullptr,
                             &pipeline_layout_) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create pipeline layout!");
  }

  VkGraphicsPipelineCreateInfo pipeline_info = {};
  pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_info.stageCount = 2;
  pipeline_info.pStages = shader_stages;
  pipeline_info.pVertexInputState = &vertex_input_info;
  pipeline_info.pInputAssemblyState = &inputAssembly;
  pipeline_info.pViewportState = &viewport_state;
  pipeline_info.pRasterizationState = &rasterizer;
  pipeline_info.pMultisampleState = &multisampling;
  pipeline_info.pColorBlendState = &color_blending;
  pipeline_info.layout = pipeline_layout_;
  pipeline_info.renderPass = render_pass_;
  pipeline_info.subpass = 0;
  pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

  if (vkCreateGraphicsPipelines(device_, VK_NULL_HANDLE, 1, &pipeline_info,
                                nullptr, &graphics_pipeline_) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create graphics pipeline!");
  }

  vkDestroyShaderModule(device_, frag_shader_module, nullptr);
  vkDestroyShaderModule(device_, vert_shader_module, nullptr);
}

void VulkanDemo::CreateFramebuffers()
{
  swap_chain_framebuffers_.resize(swap_chain_image_views_.size());

  for (size_t i = 0; i < swap_chain_image_views_.size(); i++)
  {
    VkImageView attachments[] = {swap_chain_image_views_[i]};

    VkFramebufferCreateInfo framebuffer_info = {};
    framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_info.renderPass = render_pass_;
    framebuffer_info.attachmentCount = 1;
    framebuffer_info.pAttachments = attachments;
    framebuffer_info.width = swap_chain_extent_.width;
    framebuffer_info.height = swap_chain_extent_.height;
    framebuffer_info.layers = 1;

    if (vkCreateFramebuffer(device_, &framebuffer_info, nullptr,
                            &swap_chain_framebuffers_[i]) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create framebuffer!");
    }
  }
}

void VulkanDemo::CreateBuffer(size_t buffer_size, VkBufferUsageFlags buffer_usage,
                              VkMemoryPropertyFlags properties, VkBuffer *buffer,
                              VkDeviceMemory *memory)
{
  VkBufferCreateInfo buffer_info{};
  buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffer_info.size = buffer_size;
  buffer_info.usage = buffer_usage;
  buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  if (vkCreateBuffer(device_, &buffer_info, nullptr, buffer) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create buffer!");
  }

  VkMemoryRequirements mem_requirements;
  vkGetBufferMemoryRequirements(device_, *buffer, &mem_requirements);

  VkMemoryAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.allocationSize = mem_requirements.size;
  alloc_info.memoryTypeIndex =
      FindMemoryType(mem_requirements.memoryTypeBits, properties);
  if (vkAllocateMemory(device_, &alloc_info, nullptr, memory) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to allocate buffer memory!");
  }

  vkBindBufferMemory(device_, *buffer, *memory, 0);
}

void VulkanDemo::CreateImage(uint32_t width, uint32_t height, VkFormat format,
                             VkImageTiling tiling, VkImageUsageFlags usage,
                             VkMemoryPropertyFlags properties, VkImage &image,
                             VkDeviceMemory &imageMemory)
{
  VkImageCreateInfo imageInfo{};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = width;
  imageInfo.extent.height = height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = format;
  imageInfo.tiling = tiling;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = usage;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateImage(device_, &imageInfo, nullptr, &image) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create image!");
  }

  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(device_, image, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
      FindMemoryType(memRequirements.memoryTypeBits, properties);

  if (vkAllocateMemory(device_, &allocInfo, nullptr, &imageMemory) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to allocate image memory!");
  }

  vkBindImageMemory(device_, image, imageMemory, 0);
}

void VulkanDemo::CreateTextureImage()
{
  int texWidth, texHeight, texChannels;
  stbi_uc *pixels = stbi_load("textures/texture.jpg", &texWidth, &texHeight,
                              &texChannels, STBI_rgb_alpha);
  VkDeviceSize imageSize = texWidth * texHeight * 4;

  if (!pixels)
  {
    throw std::runtime_error("failed to load texture image!");
  }

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
               &stagingBuffer, &stagingBufferMemory);
  void *data;
  vkMapMemory(device_, stagingBufferMemory, 0, imageSize, 0, &data);
  memcpy(data, pixels, static_cast<size_t>(imageSize));
  vkUnmapMemory(device_, stagingBufferMemory);
  stbi_image_free(pixels);

  CreateImage(
      texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

  transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB,
                        VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  copyBufferToImage(stagingBuffer, textureImage,
                    static_cast<uint32_t>(texWidth),
                    static_cast<uint32_t>(texHeight));
  transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

VkImageView VulkanDemo::createImageView(VkImage image, VkFormat format)
{
  VkImageViewCreateInfo viewInfo{};
  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.image = image;
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = format;
  viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;

  VkImageView imageView;
  if (vkCreateImageView(device_, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create texture image view!");
  }

  return imageView;
}

void VulkanDemo::CreateTextureImageView()
{
  textureImageView = createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB);
}

void VulkanDemo::createTextureSampler()
{
  VkSamplerCreateInfo samplerInfo{};
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;

  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

  samplerInfo.anisotropyEnable = VK_TRUE;
  samplerInfo.maxAnisotropy = 16;

  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

  samplerInfo.unnormalizedCoordinates = VK_FALSE;

  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  samplerInfo.mipLodBias = 0.0f;
  samplerInfo.minLod = 0.0f;
  samplerInfo.maxLod = 0.0f;

  // if (vkCreateSampler(device_, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS)
  // {
  //   throw std::runtime_error("failed to create texture sampler!");
  // }
}

void VulkanDemo::CreateVertexBuffer()
{
  auto buffer_size = sizeof(kVertices[0]) * kVertices.size();
  CreateBuffer(buffer_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
               &vertex_buffer_, &vertex_buffer_memory_);
  void *data;
  vkMapMemory(device_, vertex_buffer_memory_, 0, buffer_size, 0, &data);
  memcpy(data, kVertices.data(), buffer_size);
  vkUnmapMemory(device_, vertex_buffer_memory_);
}

void VulkanDemo::CreateIndexBuffer()
{
  auto buffer_size = sizeof(kIndices[0]) * kIndices.size();
  CreateBuffer(buffer_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
               &index_buffer_, &index_buffer_memory_);
  void *data;
  vkMapMemory(device_, index_buffer_memory_, 0, buffer_size, 0, &data);
  memcpy(data, kIndices.data(), buffer_size);
  vkUnmapMemory(device_, index_buffer_memory_);
}

void VulkanDemo::CreateUniformBuffers()
{
  VkDeviceSize buffer_size = sizeof(UniformBufferObject);

  uniform_buffers_.resize(swap_chain_images_.size());
  uniform_buffers_memory_.resize(swap_chain_images_.size());

  for (size_t i = 0; i < swap_chain_images_.size(); i++)
  {
    CreateBuffer(buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 &uniform_buffers_[i], &uniform_buffers_memory_[i]);
  }
}

void VulkanDemo::CreateDescriptorPool()
{
  VkDescriptorPoolSize poolSize{};
  poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSize.descriptorCount = static_cast<uint32_t>(swap_chain_images_.size());

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = 1;
  poolInfo.pPoolSizes = &poolSize;

  poolInfo.maxSets = static_cast<uint32_t>(swap_chain_images_.size());

  if (vkCreateDescriptorPool(device_, &poolInfo, nullptr,
                             &descriptor_pool_) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create descriptor pool!");
  }
}

void VulkanDemo::CreateDescriptorSets()
{
  std::vector<VkDescriptorSetLayout> layouts(swap_chain_images_.size(),
                                             descriptor_set_layout_);
  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = descriptor_pool_;
  allocInfo.descriptorSetCount =
      static_cast<uint32_t>(swap_chain_images_.size());
  allocInfo.pSetLayouts = layouts.data();

  descriptor_sets_.resize(swap_chain_images_.size());
  if (vkAllocateDescriptorSets(device_, &allocInfo,
                               descriptor_sets_.data()) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to allocate descriptor sets!");
  }

  for (size_t i = 0; i < swap_chain_images_.size(); i++)
  {
    VkDescriptorBufferInfo buffer_info{};
    buffer_info.buffer = uniform_buffers_[i];
    buffer_info.offset = 0;
    buffer_info.range = sizeof(UniformBufferObject);

    VkWriteDescriptorSet descriptor_write{};
    descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_write.dstSet = descriptor_sets_[i];
    descriptor_write.dstBinding = 0;
    descriptor_write.dstArrayElement = 0;

    descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_write.descriptorCount = 1;

    descriptor_write.pBufferInfo = &buffer_info;
    descriptor_write.pImageInfo = nullptr;       // Optional
    descriptor_write.pTexelBufferView = nullptr; // Optional
    vkUpdateDescriptorSets(device_, 1, &descriptor_write, 0, nullptr);
  }
}

void VulkanDemo::CreateCommandPool()
{
  QueueFamilyIndices queue_family_indices =
      FindQueueFamilies(physical_device_);

  VkCommandPoolCreateInfo pool_info = {};
  pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  pool_info.queueFamilyIndex = queue_family_indices.graphics_family;

  if (vkCreateCommandPool(device_, &pool_info, nullptr, &command_pool_) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to create command pool!");
  }
}

void VulkanDemo::CreateCommandBuffers()
{
  command_buffers_.resize(swap_chain_framebuffers_.size());

  VkCommandBufferAllocateInfo alloc_info = {};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.commandPool = command_pool_;
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandBufferCount = (uint32_t)command_buffers_.size();

  if (vkAllocateCommandBuffers(device_, &alloc_info,
                               command_buffers_.data()) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to allocate command buffers!");
  }

  for (size_t i = 0; i < command_buffers_.size(); i++)
  {
    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    if (vkBeginCommandBuffer(command_buffers_[i], &begin_info) !=
        VK_SUCCESS)
    {
      throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo render_pass_info = {};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = render_pass_;
    render_pass_info.framebuffer = swap_chain_framebuffers_[i];
    render_pass_info.renderArea.offset = {0, 0};
    render_pass_info.renderArea.extent = swap_chain_extent_;

    VkClearValue clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
    render_pass_info.clearValueCount = 1;
    render_pass_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(command_buffers_[i], &render_pass_info,
                         VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(command_buffers_[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
                      graphics_pipeline_);

    VkBuffer vertex_buffers[] = {vertex_buffer_};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(command_buffers_[i], 0, 1, vertex_buffers,
                           offsets);
    vkCmdBindIndexBuffer(command_buffers_[i], index_buffer_, 0,
                         VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(command_buffers_[i],
                            VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout_,
                            0, 1, &descriptor_sets_[i], 0, nullptr);
    vkCmdDrawIndexed(command_buffers_[i],
                     static_cast<uint32_t>(kIndices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(command_buffers_[i]);

    if (vkEndCommandBuffer(command_buffers_[i]) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to record command buffer!");
    }
  }
}

void VulkanDemo::CreateSyncObjects()
{
  image_available_semaphores_.resize(MAX_FRAMES_IN_FLIGHT);
  render_finished_semaphores_.resize(MAX_FRAMES_IN_FLIGHT);
  in_flight_fences_.resize(MAX_FRAMES_IN_FLIGHT);

  VkSemaphoreCreateInfo semaphoreInfo = {};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fence_info = {};
  fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    if (vkCreateSemaphore(device_, &semaphoreInfo, nullptr,
                          &image_available_semaphores_[i]) != VK_SUCCESS ||
        vkCreateSemaphore(device_, &semaphoreInfo, nullptr,
                          &render_finished_semaphores_[i]) != VK_SUCCESS ||
        vkCreateFence(device_, &fence_info, nullptr, &in_flight_fences_[i]) !=
            VK_SUCCESS)
    {
      throw std::runtime_error(
          "failed to create synchronization objects for a frame!");
    }
  }
}

void VulkanDemo::UpdateUniformBuffer(uint32_t current_image)
{
  static auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(
                   currentTime - startTime)
                   .count();

  UniformBufferObject ubo{};
  ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),
                          glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.view =
      glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.proj = glm::perspective(glm::radians(45.0f),
                              swap_chain_extent_.width /
                                  (float)swap_chain_extent_.height,
                              0.1f, 10.0f);
  ubo.proj[1][1] *= -1;
  void *data;
  vkMapMemory(device_, uniform_buffers_memory_[current_image], 0, sizeof(ubo),
              0, &data);
  memcpy(data, &ubo, sizeof(ubo));
  vkUnmapMemory(device_, uniform_buffers_memory_[current_image]);
}

void VulkanDemo::DrawFrame()
{
  vkWaitForFences(device_, 1, &in_flight_fences_[current_frame_], VK_TRUE,
                  std::numeric_limits<uint64_t>::max());

  uint32_t image_index;
  VkResult result = vkAcquireNextImageKHR(
      device_, swap_chain_, std::numeric_limits<uint64_t>::max(),
      image_available_semaphores_[current_frame_], VK_NULL_HANDLE,
      &image_index);

  if (result == VK_ERROR_OUT_OF_DATE_KHR)
  {
    RecreateSwapChain();
    return;
  }
  else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
  {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  UpdateUniformBuffer(image_index);

  VkSubmitInfo submit_info = {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore wait_semaphores[] = {
      image_available_semaphores_[current_frame_]};
  VkPipelineStageFlags wait_stages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = wait_semaphores;
  submit_info.pWaitDstStageMask = wait_stages;

  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &command_buffers_[image_index];

  VkSemaphore signal_semaphores[] = {
      render_finished_semaphores_[current_frame_]};
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = signal_semaphores;

  vkResetFences(device_, 1, &in_flight_fences_[current_frame_]);

  if (vkQueueSubmit(graphics_queue_, 1, &submit_info,
                    in_flight_fences_[current_frame_]) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  VkPresentInfoKHR present_info = {};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = signal_semaphores;

  VkSwapchainKHR swapChains[] = {swap_chain_};
  present_info.swapchainCount = 1;
  present_info.pSwapchains = swapChains;

  present_info.pImageIndices = &image_index;

  result = vkQueuePresentKHR(present_queue_, &present_info);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      framebuffer_resized_)
  {
    framebuffer_resized_ = false;
    RecreateSwapChain();
  }
  else if (result != VK_SUCCESS)
  {
    throw std::runtime_error("failed to present swap chain image!");
  }

  current_frame_ = (current_frame_ + 1) % MAX_FRAMES_IN_FLIGHT;
}

VkShaderModule VulkanDemo::CreateShaderModule(const std::vector<char> &code)
{
  VkShaderModuleCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = code.size();
  create_info.pCode = reinterpret_cast<const uint32_t *>(code.data());

  VkShaderModule shader_module;
  if (vkCreateShaderModule(device_, &create_info, nullptr, &shader_module) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to create shader module!");
  }

  return shader_module;
}

VkSurfaceFormatKHR VulkanDemo::ChooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &available_formats)
{
  if (available_formats.size() == 1 &&
      available_formats[0].format == VK_FORMAT_UNDEFINED)
  {
    return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
  }

  for (const auto &available_format : available_formats)
  {
    if (available_format.format == VK_FORMAT_B8G8R8A8_UNORM &&
        available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
    {
      return available_format;
    }
  }

  return available_formats[0];
}

VkPresentModeKHR VulkanDemo::ChooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> available_present_modes)
{
  VkPresentModeKHR best_mode = VK_PRESENT_MODE_FIFO_KHR;

  for (const auto &available_present_mode : available_present_modes)
  {
    if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
    {
      return available_present_mode;
    }
    else if (available_present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
    {
      best_mode = available_present_mode;
    }
  }

  return best_mode;
}

VkExtent2D VulkanDemo::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max())
  {
    return capabilities.currentExtent;
  }
  else
  {
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);

    VkExtent2D actual_extent = {static_cast<uint32_t>(width),
                                static_cast<uint32_t>(height)};

    actual_extent.width = std::max(
        capabilities.minImageExtent.width,
        std::min(capabilities.maxImageExtent.width, actual_extent.width));
    actual_extent.height = std::max(
        capabilities.minImageExtent.height,
        std::min(capabilities.maxImageExtent.height, actual_extent.height));

    return actual_extent;
  }
}

SwapChainSupportDetails VulkanDemo::QuerySwapChainSupport(VkPhysicalDevice device)
{
  SwapChainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface_,
                                            &details.capabilities);

  uint32_t format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &format_count,
                                       nullptr);

  if (format_count != 0)
  {
    details.formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &format_count,
                                         details.formats.data());
  }

  uint32_t present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_,
                                            &present_mode_count, nullptr);

  if (present_mode_count != 0)
  {
    details.present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface_, &present_mode_count, details.present_modes.data());
  }

  return details;
}

bool VulkanDemo::IsDeviceSuitable(VkPhysicalDevice device)
{
  QueueFamilyIndices indices = FindQueueFamilies(device);

  bool extensions_supported = CheckDeviceExtensionSupport(device);

  bool swap_chain_adequate = false;
  if (extensions_supported)
  {
    SwapChainSupportDetails swap_chain_support =
        QuerySwapChainSupport(device);
    swap_chain_adequate = !swap_chain_support.formats.empty() &&
                          !swap_chain_support.present_modes.empty();
  }

  return indices.is_complete() && extensions_supported && swap_chain_adequate;
}

bool VulkanDemo::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
  uint32_t extension_count;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count,
                                       nullptr);

  std::vector<VkExtensionProperties> available_extensions(extension_count);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count,
                                       available_extensions.data());

  std::set<std::string> required_extensions(kDeviceExtensions.begin(),
                                            kDeviceExtensions.end());

  for (const auto &extension : available_extensions)
  {
    required_extensions.erase(extension.extensionName);
  }

  return required_extensions.empty();
}

QueueFamilyIndices VulkanDemo::FindQueueFamilies(VkPhysicalDevice device)
{
  QueueFamilyIndices indices;

  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count,
                                           nullptr);

  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count,
                                           queue_families.data());

  int i = 0;
  for (const auto &queue_family : queue_families)
  {
    if (queue_family.queueCount > 0 &&
        queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      indices.graphics_family = i;
    }

    VkBool32 present_support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface_,
                                         &present_support);

    if (queue_family.queueCount > 0 && present_support)
    {
      indices.present_family = i;
    }

    if (indices.is_complete())
    {
      break;
    }

    i++;
  }

  return indices;
}

std::vector<const char *> VulkanDemo::GetRequiredExtensions()
{
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);

  if (kEnableValidationLayers)
  {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

bool VulkanDemo::CheckValidationLayerSupport()
{
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  for (const char *layer_name : kValidationLayers)
  {
    bool layer_found = false;

    for (const auto &layerProperties : available_layers)
    {
      if (strcmp(layer_name, layerProperties.layerName) == 0)
      {
        layer_found = true;
        break;
      }
    }

    if (!layer_found)
    {
      return false;
    }
  }

  return true;
}

std::vector<char> VulkanDemo::ReadFile(const std::string &filename)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open())
  {
    throw std::runtime_error("failed to open file!");
  }

  size_t file_size = (size_t)file.tellg();
  std::vector<char> buffer(file_size);

  file.seekg(0);
  file.read(buffer.data(), file_size);

  file.close();

  return buffer;
}

VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanDemo::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                          VkDebugUtilsMessageTypeFlagsEXT messageType,
                          const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                          void *pUserData)
{
  std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

  return VK_FALSE;
}

uint32_t VulkanDemo::FindMemoryType(uint32_t type_filter,
                                    VkMemoryPropertyFlags properties)
{
  VkPhysicalDeviceMemoryProperties mem_properties;
  vkGetPhysicalDeviceMemoryProperties(physical_device_, &mem_properties);
  for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
  {
    if (type_filter & (1 << i) &&
        (mem_properties.memoryTypes[i].propertyFlags & properties) ==
            properties)
    {
      return i;
    }
  }

  throw std::runtime_error("failed to find suitable memory type!");
}

VkCommandBuffer VulkanDemo::beginSingleTimeCommands()
{
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = command_pool_;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(device_, &allocInfo, &commandBuffer);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  return commandBuffer;
}

void VulkanDemo::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
  vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkQueueSubmit(graphics_queue_, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(graphics_queue_);

  vkFreeCommandBuffers(device_, command_pool_, 1, &commandBuffer);
}

void VulkanDemo::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
  VkCommandBuffer commandBuffer = beginSingleTimeCommands();

  VkBufferCopy copyRegion{};
  copyRegion.size = size;
  vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

  endSingleTimeCommands(commandBuffer);
}

void VulkanDemo::transitionImageLayout(VkImage image, VkFormat format,
                                       VkImageLayout oldLayout, VkImageLayout newLayout)
{
  VkCommandBuffer commandBuffer = beginSingleTimeCommands();
  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.image = image;
  barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;
  barrier.srcAccessMask = 0; // TODO
  barrier.dstAccessMask = 0; // TODO

  VkPipelineStageFlags sourceStage;
  VkPipelineStageFlags destinationStage;
  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
  {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  }
  else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
  {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  }
  else
  {
    throw std::invalid_argument("unsupported layout transition!");
  }

  vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0,
                       nullptr, 0, nullptr, 1, &barrier);
  endSingleTimeCommands(commandBuffer);
}

void VulkanDemo::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                                   uint32_t height)
{
  VkCommandBuffer commandBuffer = beginSingleTimeCommands();
  VkBufferImageCopy region{};
  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;

  region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;

  region.imageOffset = {0, 0, 0};
  region.imageExtent = {width, height, 1};
  vkCmdCopyBufferToImage(commandBuffer, buffer, image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
  endSingleTimeCommands(commandBuffer);
}

int main()
{
  VulkanDemo app;

  try
  {
    app.run();
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
