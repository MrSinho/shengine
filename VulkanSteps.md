# Steps in a sintetic way

For some computer graphics definitions check this [wikipedia link](https://en.wikipedia.org/wiki/Glossary_of_computer_graphics). 
The best resource for reading about vulkan specific functions and structure definitions is [this](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/).

## VkApplicationInfo: obvious
## Validation layers and message callbacks
## VkInstance and instance extensions
## Create a native window surface
## Physical devices and queue families 
enumerate physical devices and check for queue families support and store the `indices`

## Create queues, a logical device, command pools and command buffers
create a `queue` for each queue family index
create a `logical` device and add swapchain extension
create a `command pool` for each queue
create  a `command buffer` for each command pool

## Create a swapchain

Which requires the following data: 

* `minImageCount`: which is the minimum number of required images to present. A standard way is to use the same number of minImageCount the surface capabilities (`vkGetPhysicalDeviceSurfaceCapabilitiesKHR`).

* images `width`, `height` and `transform`, accessible with `vkGetPhysicalDeviceSurfaceCapabilitiesKHR`.

* [image format](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkFormat.html) (specifies color space, bitdepth, components type and the way data is stored) (the supported formats can be found with `vkGetPhysicalDeviceSurfaceFormatsKHR`). A common implementation of the format in the swapchain is to look for a specific format (for instance `VK_FORMAT_B8G8R8A8_UNORM`), and in case these formats have not been found you set it to `VK_FORMAT_UNDEFINED`, which means that you don't want to specify which format you're going to use. The way used to define formats is the following: 
`VK_FORMAT` + `COLOR_COMPONENT_WITH_BIT_SIZE` * `N_COMPONENTS` + `VALUES_TYPE`. Some common types are: `UNORM` (the color vector with unsigned components is normalized), `SNORM` (the color vector with signed components is normalized), `USCALED` (the components are unsigned scaled integers), `SSCALED` (the components are signed scaled integers), `UINT` (the color components are unsigned integers), `SINT` (the color components are signed integers), `UFLOAT` (unsigned floats), `SFLOAT` (signed floats), `SRGB` (unsigned and normalized vector that is handled with sRGB nonlinear encoding).

This is means that according to Khronos: 
> `VK_FORMAT_B8G8R8A8_UNORM` specifies a four-component, 32-bit unsigned normalized format that has an 8-bit B component in byte 0, an 8-bit G component in byte 1, an 8-bit R component in byte 2, and an 8-bit A component in byte 3.

* present mode (the supported present modes can be enumerated with `VkGetPhysicalDeviceSurfacePresentModesKHR`), which means how the swapchain is going to present the images to the screen: 

  * With `VK_PRESENT_MODE_IMMEDIATE_KHR` the images submitted to the application are presented on the screen the instant they're ready

  * `VK_PRESENT_MODE_FIFO_KHR` the ready-to-use image is submitted to a queue 

  * using VK_PRESENT_MODE_FIFO_RELAXED_KHR the image is not submitted to a queue when the queue is empty

  * `VK_PRESENT_MODE_MAILBOX_KHR` replaces the old images with newer ones in case the queue is full, instead of blocking the application such as in `VK_PRESENT_MODE_FIFO_KHR`

* composite alpha
* image array layers
* if clipped
* the image color space 

