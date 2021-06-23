



## Create a VkSwapchain

Which requires the following data: 
 * minImageCount, images width, height and transform, accessible with vkGetPhysicalDeviceSurfaceCapabilitiesKHR 
 * image format and bitdepth (ex VK_FORMAT_B8G8R8A8_UNORM) accessible from vkGetPhysicalDeviceSurfaceFormatsKHR
 * present mode, which means how the swapchain is going to present the images to the screen: 
  * With VK_PRESENT_MODE_IMMEDIATE_KHR the images submitted to the application are presented on the screen the instant they're ready
  * VK_PRESENT_MODE_FIFO_KHR the ready-to-use image is submitted to a queue 
  * using VK_PRESENT_MODE_FIFO_RELAXED_KHR the image is not submitted to a queue when the queue is empty
  * VK_PRESENT_MODE_MAILBOX_KHR replaces the old images with newr ones in case the queue is full, instead of blocking the application such as in VK_PRESENT_MODE_FIFO_KHR

  - the supported present modes can be enumerated with VkGetPhysicalDeviceSurfacePresentModesKHR

 * composite alpha
 * image array layers
 * if clipped
 * the image color space 