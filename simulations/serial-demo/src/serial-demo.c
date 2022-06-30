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

uint8_t SH_ENGINE_EXPORT_FUNCTION serial_start(ShEngine* p_engine) {
	
	for (uint32_t entity = 0; entity < p_engine->scene.entity_count; entity++) {
		if (entity == PICO_ENTITY) {

			//PORT IS DIFFERENT FOR EACH MACHINE
			ShSerialHandle* p_serial = shAddShSerialHandle(&p_engine->scene, entity);

			printf("setting up serial port\n");

#ifdef _WIN32
			shSerialOpen("COM3", 9600, 500, SH_SERIAL_READ_WRITE, p_serial);
#else
			shSerialOpen(shSerialOpen("/dev/ttyACM0", 9600, 500, SH_SERIAL_READ_WRITE, &handle);
#endif//_WIN32
		}
		break;
	}
	
}

#include <stdio.h>
#include <stdlib.h>

void read_serial_data(ShEngine* p_engine, ShSerialHandle* p_serial) {

	ShMaterialHost* p_material = &p_engine->p_materials[0];

	float serial_buffer[2] = { 0 };
	float* p_descriptor = (float*)&((char*)p_material->material_clients[SKYSPHERE_ENTITY].p_uniform_parameters)[p_material->pipeline.descriptor_buffer_infos[0].range + 16];

	uint32_t bytes_read = 0;
	if (shSerialReadBuffer(8, serial_buffer, &bytes_read, p_serial)) {
		if (serial_buffer[0] >= 0.01f) {

			p_descriptor[4] = serial_buffer[1];//light intensity;
			p_descriptor[5] = serial_buffer[1];
			p_descriptor[6] = serial_buffer[1];

			printf("tmperature: %f\nlight intensity: %f\n\n", serial_buffer[0], serial_buffer[1]);
		}
	}
}

uint8_t SH_ENGINE_EXPORT_FUNCTION serial_update(ShEngine* p_engine) {
	for (uint32_t entity = 0; entity < p_engine->scene.entity_count; entity++) {
		ShSerialHandle* p_serial = shGetShSerialHandle(&p_engine->scene, entity);
		if (p_serial != NULL && entity == PICO_ENTITY) {
			if (p_serial->_handle != NULL) {
				read_serial_data(p_engine, p_serial);
			}
			break;
		}
	}
}

uint8_t SH_ENGINE_EXPORT_FUNCTION serial_close(ShEngine* p_engine) {
	for (uint32_t entity = 0; entity < p_engine->scene.entity_count; entity++) {
		if (entity == PICO_ENTITY) {
			ShSerialHandle* p_serial = shGetShSerialHandle(&p_engine->scene, entity);

			printf("closing serial port...\n");
			shSerialSleep(10);
			if (p_serial != NULL) {
				shSerialClose(p_serial);
			}
		}
	}
}

#ifdef __cplusplus
}
#endif//__cplusplus
