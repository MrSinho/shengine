# Steps in a sintetic way

For some computer graphics definitions check this [wikipedia link](https://en.wikipedia.org/wiki/Glossary_of_computer_graphics). 
The best resource for reading about vulkan specific functions and structure definitions is [this](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/).

## VkApplicationInfo: 
Defines stuff like your application name, the application version, the engine name with its version and the Vulkan API version.

## Validation layers and message callbacks
Vulkan libraries have no debug tools by default, which could lead to unexpected errors and bugs for the developer. That's why you can use validation layers, which consist in some checks and conditions before a function is called. You can develop your own validation layer, or you can ask Vulkan to use some you've already installed with the Vulkan SDK,, such as `"VK_LAYER_KHRONOS_validation"`. With `vkEnumerateInstanceLayerProperties` you get an array of the installed validation layers, which properties are stored in the `VkLayerProperties` structure. 

Validation layers work only when you're manipulating CPU data, that's why for GPU debugging you should setup a debug messenger callback: --

## VkInstance and instance extensions

A VkInstance is what is the intermediate component between yout program and Vulkan. To create an instance you first need to setup a `VkInstanceCreateInfo` structure which holds the `VkApplicationInfo`, the name and number of the validation layers you want to use, and the instance extensions. In the future you're going to create a swapchain, which you'll be able to use only by using the extension `"VK_KHR_swapchain"`

## Create a native window surface

Cross platform window handlers are great. GLFW is one of them. It allows creating windows, handling events and inputs for every platform. GLFW was first meant to be linked with OpenGL, but later it supported many other APIs. Surfaces in Vulkan have to be platform spe

## Physical devices and queue families 

Vulkan needs to know wich graphics card it's going to use, so we need to check if the installed hardware does support specific features that we can find in queue families, and create a simple scoring system for choosing only one of the suitable devices.

So we need check for some specific queue families support and store the `indices`. 
In your GPU there are different queue families, each one is made for a differente purpose, for instance in our case we want to know if our device does support graphics, which means we want to store the index of the queue family enumerated from the physical device. Then we want to check if the selected queue family is able to present images to the screen, if yes, than it's all fine, if not then we just need to find another queue family that is able to do that job, and add the index of that queue family.

## Create a queue

Before creating a logical device we need to create a queue for each queue family index. To do that you have to fill the `VkDeviceQueueCreateInfo`, which requires:
* A `queueCount`, generally set to 1
* A `queuePriorities`: the higher is the value, the higher is the priority.
* A `queueFamilyIndex`: as I said before you have to create a queue for each required queue family, which ID in your gpu is the queue family index

## Create a logical device

In Vulkan a logical device is what allows you to comunicate directly to the gpu hardware. Every graphics settings will require a logical device. 

Create a `VkDeviceCreateInfo` and fill the following items:

* The `queueCreateInfoCount`: the number of `VkQueueCreateInfo` you've created
  
* The `pQueueCreateInfos`, that is the pointer of the array containing the `VkQueueCreateInfo`s
  
* The `enabledLayerCount`, older versions of Vulkan differenciate between `VkInstance` validation layers and `VkDevice` validation layers, but today when you define the validation layers in the `VkInstanceCreateInfo` they will automatically work for the physical device. It's still a good practice to repeat the process with `VkDeviceCreateInfo`. The same goes for `ppEnabledLayerNames`
  
* `ppEnabledLayerNames`: the pointer of the array that stores the layer names

Use `VkCreateLogicalDevice` to create the logical device.

## Create command pools

Now that the logical device is set up, you need to create a command pool for each queue index. The `VkCommandPoolCreateInfo` needs:

* `queueFamilyIndex`: makes sense

* `flags`: defines the behaviour of the command pool

Use `VkCreateCommandPool` to create a command pool.

## Create a command buffer

For each command pool a command buffer is required. Fill the `VkCommandBufferCreateInfo` with:

* `commandPool`

* `commandBufferCount`

* `level`

## Create a swapchain

Which requires the following data: 

* `minImageCount`: which is the minimum number of required images to present. A standard way is to use the same number of minImageCount the surface capabilities (`vkGetPhysicalDeviceSurfaceCapabilitiesKHR`).

* (accessible with `vkGetPhysicalDeviceSurfaceCapabilitiesKHR`) image `width`, `height` and `preTransform`, which consist in some [transformation](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSurfaceTransformFlagBitsKHR.html) you can make to images before the presentation, it's an easy to understand operation.

* [image format](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkFormat.html) (specifies color space, bitdepth, components type and the way data is stored) (the supported formats can be found with `vkGetPhysicalDeviceSurfaceFormatsKHR`). A common implementation of the format in the swapchain is to look for a specific format (for instance `VK_FORMAT_B8G8R8A8_UNORM`), and in case these formats have not been found you set it to `VK_FORMAT_UNDEFINED`, which means that you don't want to specify which format you're going to use. The way used to define formats is the following: 
`VK_FORMAT` + `COLOR_COMPONENT_WITH_BIT_SIZE` * `N_COMPONENTS` + `VALUES_TYPE`. Some common types are: `UNORM` (the color vector with unsigned components is normalized), `SNORM` (the color vector with signed components is normalized), `USCALED` (the components are unsigned scaled integers), `SSCALED` (the components are signed scaled integers), `UINT` (the color components are unsigned integers), `SINT` (the color components are signed integers), `UFLOAT` (unsigned floats), `SFLOAT` (signed floats), `SRGB` (unsigned and normalized vector that is handled with sRGB nonlinear encoding).

Example: 
> `VK_FORMAT_B8G8R8A8_UNORM` specifies a four-component, 32-bit unsigned normalized format that has an 8-bit B component in byte 0, an 8-bit G component in byte 1, an 8-bit R component in byte 2, and an 8-bit A component in byte 3.

* present mode (the supported present modes can be enumerated with `VkGetPhysicalDeviceSurfacePresentModesKHR`), which means how the swapchain is going to present the images to the screen: 

  * With `VK_PRESENT_MODE_IMMEDIATE_KHR` the images submitted to the application are presented on the screen the instant they're ready

  * `VK_PRESENT_MODE_FIFO_KHR` the ready-to-use image is submitted to a queue 

  * using VK_PRESENT_MODE_FIFO_RELAXED_KHR the image is not submitted to a queue when the queue is empty

  * `VK_PRESENT_MODE_MAILBOX_KHR` replaces the old images with newer ones in case the queue is full, instead of blocking the application such as in `VK_PRESENT_MODE_FIFO_KHR`

* composite alpha: consists on how the alpha value is handled by the swapchain:
  * VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR: ignore alpha value, the image is always opaque
  * VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR: 
  * VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR: 
  * VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR: 

* image array layers
* if clipped
* the image color space 

