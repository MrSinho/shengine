#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shengine/shEngine.h>
#include <shengine/shExport.h>

#include <shserial/shSerial.h>

#define CAMERA_ENTITY 0
#define PICO_ENTITY 1
#define SKYSPHERE_ENTITY 5

#include <shecs/shTransform.h>

//Make serial handle as entity component
SH_ECS_MAKE_COMPONENT_DEFINITIONS(ShSerialHandle, 11);

#define p_scene &p_engine->scene

void SH_ENGINE_EXPORT_FUNCTION serial_start(ShEngine* p_engine, uint32_t entity) {
	
	if (entity == PICO_ENTITY) {
	
		//PORT IS DIFFERENT FOR EACH MACHINE
		ShSerialHandle* p_serial = shAddShSerialHandle(p_scene, entity);

#ifdef _WIN32
		shSerialOpen("COM3", 9600, 500, SH_SERIAL_READ_WRITE, p_serial);
#else
		shSerialOpen(shSerialOpen("/dev/ttyACM0", 9600, 500, SH_SERIAL_READ_WRITE, &handle);
#endif//_WIN32
	}
}

#include <stdio.h>
#include <stdlib.h>

void read_serial_data(ShEngine* p_engine, uint32_t entity) {
	ShSerialHandle* p_serial = shGetShSerialHandle(p_scene, entity);

	float serial_buffer[2] = { 0 };
	float* p_uniform = (float*)&((char*)p_engine->p_materials[0].material_clients[SKYSPHERE_ENTITY].p_uniform_parameters)[80];

	uint32_t bytes_read = 0;
	if (shSerialReadBuffer(8, serial_buffer, &bytes_read, p_serial)) {
		if (serial_buffer[0] >= 0.01f) {
			
			p_uniform[4] = serial_buffer[1];//light intensity;
			p_uniform[5] = serial_buffer[1];
			p_uniform[6] = serial_buffer[1];

			printf("temperature: %f\nlight intensity: %f\n\n", serial_buffer[0], serial_buffer[1]);
		}

	}
}

void SH_ENGINE_EXPORT_FUNCTION serial_update(ShEngine* p_engine, uint32_t entity) {
	
	switch (entity) {
	case PICO_ENTITY:
		read_serial_data(p_engine, entity);
		break;
	//case CAMERA_ENTITY:
	//	break;
	//case SKYSPHERE_ENTITY:
	//	break;
	//default:
	//	break;
	}
}

void SH_ENGINE_EXPORT_FUNCTION serial_close(ShEngine* p_engine, uint32_t entity) {

	if (entity == PICO_ENTITY) {
		ShSerialHandle* p_serial = shGetShSerialHandle(p_scene, entity);
	
		printf("closing serial port...\n");
		shSerialSleep(10);
		shSerialClose(p_serial);
	}
}

#ifdef __cplusplus
}
#endif//__cplusplus
