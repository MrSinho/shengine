#ifndef SH_INPUT_H
#define SH_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



typedef struct ShWindow ShWindow;

extern void shGetCursorPosition(ShWindow* p_window);



#define SH_RELEASE	GLFW_RELEASE
#define SH_PRESS	GLFW_PRESS  
#define SH_REPEAT	GLFW_REPEAT 



/* Printable keys */
#define SH_KEY_SPACE              32
#define SH_KEY_APOSTROPHE         39  /* ' */
#define SH_KEY_COMMA              44  /* , */
#define SH_KEY_MINUS              45  /* - */
#define SH_KEY_PERIOD             46  /* . */
#define SH_KEY_SLASH              47  /* / */
#define SH_KEY_0                  48
#define SH_KEY_1                  49
#define SH_KEY_2                  50
#define SH_KEY_3                  51
#define SH_KEY_4                  52
#define SH_KEY_5                  53
#define SH_KEY_6                  54
#define SH_KEY_7                  55
#define SH_KEY_8                  56
#define SH_KEY_9                  57
#define SH_KEY_SEMICOLON          59  /* ; */
#define SH_KEY_EQUAL              61  /* = */
#define SH_KEY_A                  65
#define SH_KEY_B                  66
#define SH_KEY_C                  67
#define SH_KEY_D                  68
#define SH_KEY_E                  69
#define SH_KEY_F                  70
#define SH_KEY_G                  71
#define SH_KEY_H                  72
#define SH_KEY_I                  73
#define SH_KEY_J                  74
#define SH_KEY_K                  75
#define SH_KEY_L                  76
#define SH_KEY_M                  77
#define SH_KEY_N                  78
#define SH_KEY_O                  79
#define SH_KEY_P                  80
#define SH_KEY_Q                  81
#define SH_KEY_R                  82
#define SH_KEY_S                  83
#define SH_KEY_T                  84
#define SH_KEY_U                  85
#define SH_KEY_V                  86
#define SH_KEY_W                  87
#define SH_KEY_X                  88
#define SH_KEY_Y                  89
#define SH_KEY_Z                  90
#define SH_KEY_LEFT_BRACKET       91  /* [ */
#define SH_KEY_BACKSLASH          92  /* \ */
#define SH_KEY_RIGHT_BRACKET      93  /* ] */
#define SH_KEY_GRAVE_ACCENT       96  /* ` */
#define SH_KEY_WORLD_1            161 /* non-US #1 */
#define SH_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define SH_KEY_ESCAPE             256
#define SH_KEY_ENTER              257
#define SH_KEY_TAB                258
#define SH_KEY_BACKSPACE          259
#define SH_KEY_INSERT             260
#define SH_KEY_DELETE             261
#define SH_KEY_RIGHT              262
#define SH_KEY_LEFT               263
#define SH_KEY_DOWN               264
#define SH_KEY_UP                 265
#define SH_KEY_PAGE_UP            266
#define SH_KEY_PAGE_DOWN          267
#define SH_KEY_HOME               268
#define SH_KEY_END                269
#define SH_KEY_CAPS_LOCK          280
#define SH_KEY_SCROLL_LOCK        281
#define SH_KEY_NUM_LOCK           282
#define SH_KEY_PRINT_SCREEN       283
#define SH_KEY_PAUSE              284
#define SH_KEY_F1                 290
#define SH_KEY_F2                 291
#define SH_KEY_F3                 292
#define SH_KEY_F4                 293
#define SH_KEY_F5                 294
#define SH_KEY_F6                 295
#define SH_KEY_F7                 296
#define SH_KEY_F8                 297
#define SH_KEY_F9                 298
#define SH_KEY_F10                299
#define SH_KEY_F11                300
#define SH_KEY_F12                301
#define SH_KEY_F13                302
#define SH_KEY_F14                303
#define SH_KEY_F15                304
#define SH_KEY_F16                305
#define SH_KEY_F17                306
#define SH_KEY_F18                307
#define SH_KEY_F19                308
#define SH_KEY_F20                309
#define SH_KEY_F21                310
#define SH_KEY_F22                311
#define SH_KEY_F23                312
#define SH_KEY_F24                313
#define SH_KEY_F25                314
#define SH_KEY_KP_0               320
#define SH_KEY_KP_1               321
#define SH_KEY_KP_2               322
#define SH_KEY_KP_3               323
#define SH_KEY_KP_4               324
#define SH_KEY_KP_5               325
#define SH_KEY_KP_6               326
#define SH_KEY_KP_7               327
#define SH_KEY_KP_8               328
#define SH_KEY_KP_9               329
#define SH_KEY_KP_DECIMAL         330
#define SH_KEY_KP_DIVIDE          331
#define SH_KEY_KP_MULTIPLY        332
#define SH_KEY_KP_SUBTRACT        333
#define SH_KEY_KP_ADD             334
#define SH_KEY_KP_ENTER           335
#define SH_KEY_KP_EQUAL           336
#define SH_KEY_LEFT_SHIFT         340
#define SH_KEY_LEFT_CONTROL       341
#define SH_KEY_LEFT_ALT           342
#define SH_KEY_LEFT_SUPER         343
#define SH_KEY_RIGHT_SHIFT        344
#define SH_KEY_RIGHT_CONTROL      345
#define SH_KEY_RIGHT_ALT          346
#define SH_KEY_RIGHT_SUPER        347
#define SH_KEY_MENU               348

#define SH_KEY_LAST               SH_KEY_MENU

typedef int8_t shKeyEvents[SH_KEY_LAST + 1];

#define SH_MOUSE_BUTTON_1         0
#define SH_MOUSE_BUTTON_2         1
#define SH_MOUSE_BUTTON_3         2
#define SH_MOUSE_BUTTON_4         3
#define SH_MOUSE_BUTTON_5         4
#define SH_MOUSE_BUTTON_6         5
#define SH_MOUSE_BUTTON_7         6
#define SH_MOUSE_BUTTON_8         7
#define SH_MOUSE_BUTTON_LAST      SH_MOUSE_BUTTON_8
#define SH_MOUSE_BUTTON_LEFT      SH_MOUSE_BUTTON_1
#define SH_MOUSE_BUTTON_RIGHT     SH_MOUSE_BUTTON_2
#define SH_MOUSE_BUTTON_MIDDLE    SH_MOUSE_BUTTON_3

typedef int8_t shMouseEvents[SH_MOUSE_BUTTON_8 + 1];

#define SH_JOYSTICK_1             0
#define SH_JOYSTICK_2             1
#define SH_JOYSTICK_3             2
#define SH_JOYSTICK_4             3
#define SH_JOYSTICK_5             4
#define SH_JOYSTICK_6             5
#define SH_JOYSTICK_7             6
#define SH_JOYSTICK_8             7
#define SH_JOYSTICK_9             8
#define SH_JOYSTICK_10            9
#define SH_JOYSTICK_11            10
#define SH_JOYSTICK_12            11
#define SH_JOYSTICK_13            12
#define SH_JOYSTICK_14            13
#define SH_JOYSTICK_15            14
#define SH_JOYSTICK_16            15
#define SH_JOYSTICK_LAST          SH_JOYSTICK_16

#define SH_GAMEPAD_BUTTON_A               0
#define SH_GAMEPAD_BUTTON_B               1
#define SH_GAMEPAD_BUTTON_X               2
#define SH_GAMEPAD_BUTTON_Y               3
#define SH_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define SH_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define SH_GAMEPAD_BUTTON_BACK            6
#define SH_GAMEPAD_BUTTON_START           7
#define SH_GAMEPAD_BUTTON_GUIDE           8
#define SH_GAMEPAD_BUTTON_LEFT_THUMB      9
#define SH_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define SH_GAMEPAD_BUTTON_DPAD_UP         11
#define SH_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define SH_GAMEPAD_BUTTON_DPAD_DOWN       13
#define SH_GAMEPAD_BUTTON_DPAD_LEFT       14
#define SH_GAMEPAD_BUTTON_LAST            SH_GAMEPAD_BUTTON_DPAD_LEFT

#define SH_GAMEPAD_BUTTON_CROSS       SH_GAMEPAD_BUTTON_A
#define SH_GAMEPAD_BUTTON_CIRCLE      SH_GAMEPAD_BUTTON_B
#define SH_GAMEPAD_BUTTON_SQUARE      SH_GAMEPAD_BUTTON_X
#define SH_GAMEPAD_BUTTON_TRIANGLE    SH_GAMEPAD_BUTTON_Y

#define SH_GAMEPAD_AXIS_LEFT_X        0
#define SH_GAMEPAD_AXIS_LEFT_Y        1
#define SH_GAMEPAD_AXIS_RIGHT_X       2
#define SH_GAMEPAD_AXIS_RIGHT_Y       3
#define SH_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define SH_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define SH_GAMEPAD_AXIS_LAST          SH_GAMEPAD_AXIS_RIGHT_TRIGGER



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_INPUT_H