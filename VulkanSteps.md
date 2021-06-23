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

## Create a VkSwapchain

Which requires the following data: 

* `minImageCount`, `images` `width`, `height` and `transform`, accessible with `vkGetPhysicalDeviceSurfaceCapabilitiesKHR` 

* image format and bitdepth (ex `VK_FORMAT_B8G8R8A8_UNORM`) accessible from `vkGetPhysicalDeviceSurfaceFormatsKHR`

* present mode (the supported present modes can be enumerated with `VkGetPhysicalDeviceSurfacePresentModesKHR`), which means how the swapchain is going to present the images to the screen: 
* 
  * With `VK_PRESENT_MODE_IMMEDIATE_KHR` the images submitted to the application are presented on the screen the instant they're ready

  * `VK_PRESENT_MODE_FIFO_KHR` the ready-to-use image is submitted to a queue 

  * using VK_PRESENT_MODE_FIFO_RELAXED_KHR the image is not submitted to a queue when the queue is empty

  * `VK_PRESENT_MODE_MAILBOX_KHR` replaces the old images with newer ones in case the queue is full, instead of blocking the application such as in `VK_PRESENT_MODE_FIFO_KHR`

* composite alpha
* image array layers
* if clipped
* the image color space 