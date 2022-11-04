#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shengine/shEngine.h>
#include <shengine/shExport.h>
#include <shegui/shEgui.h>

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
			(shSerialOpen("COM3", 9600, 500, SH_SERIAL_READ_WRITE, p_serial) == 0)
				&& (p_serial->_handle = NULL);
#else
			shSerialOpen(shSerialOpen("/dev/ttyACM0", 9600, 500, SH_SERIAL_READ_WRITE, &handle);
#endif//_WIN32
			break;
		}
	}
	
	shEngineGuiSetup(p_engine, 256, SH_GUI_THEME_EXTRA_DARK);

	return 1;
}

#include <stdio.h>
#include <stdlib.h>

void read_serial_data(ShEngine* p_engine, ShSerialHandle* p_serial) {

	ShMaterialHost* p_material = &p_engine->p_materials[0];

	float serial_buffer[2] = { 0 };
	float* p_descriptor = (float*)&((char*)p_material->p_material_clients[SKYSPHERE_ENTITY].p_uniform_parameters)[p_material->pipeline.descriptor_buffer_infos[0].range + 16];

	uint32_t bytes_read = 0;
	char temperature_light[128] = { '\0'};
	if (shSerialReadBuffer(8, serial_buffer, &bytes_read, p_serial)) {
		if (serial_buffer[0] >= 0.2f) {

			p_descriptor[4] = serial_buffer[1];//light intensity;
			p_descriptor[5] = serial_buffer[1];
			p_descriptor[6] = serial_buffer[1];
		}
	}

	sprintf(
		temperature_light,
		"temperature: %.3f",
		serial_buffer[0]
	);
	
	shGuiText(
		p_engine->p_gui,
		SH_GUI_WINDOW_TEXT_SIZE * 3.0f,
		20.0f,
		-20.0f,
		temperature_light,
		SH_GUI_EDGE_LEFT | SH_GUI_EDGE_TOP
	);
}

uint64_t SH_ENGINE_EXPORT_FUNCTION serial_thread(void* p_ext) {
	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION serial_update_pending(ShEngine* p_engine) {
	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION serial_after_thread(ShEngine* p_engine) {
	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION serial_update(ShEngine* p_engine) {
	ShSerialHandle* p_serial = NULL;
	p_serial = shGetShSerialHandle(&p_engine->scene, PICO_ENTITY);

	if (p_serial != NULL) {
		if (p_serial->_handle != NULL) {
			read_serial_data(p_engine, p_serial);
		}
		else {
			shGuiText(
				p_engine->p_gui, 
				SH_GUI_WINDOW_TEXT_SIZE * 1.5f, 
				0.0f, -20.0f, 
				"Failed connecting to serial port",
				SH_GUI_CENTER_WIDTH | SH_GUI_EDGE_TOP
			);
		}
	}

	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION serial_frame_update(ShEngine* p_engine) {
	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION serial_frame_resize(ShEngine* p_engine) {
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION serial_close(ShEngine* p_engine) {
	ShSerialHandle* p_serial = shGetShSerialHandle(&p_engine->scene, PICO_ENTITY);
	
	printf("closing serial port...\n");
	shSerialSleep(10);
	if (p_serial != NULL) {
		shSerialClose(p_serial);
	}

	return 1;
}



#ifdef SH_APPLICATION_TARGET_TYPE_EXECUTABLE

#include <sheditor/shEditor.h>

int main() {
    ShEngine engine = { 0 };
    engine.application_host.p_start          = &serial_start;
    engine.application_host.p_thread         = &serial_thread;
    engine.application_host.p_update_pending = &serial_update_pending;
    engine.application_host.p_after_thread   = &serial_after_thread;
    engine.application_host.p_update         = &serial_update;
    engine.application_host.p_frame_update   = &serial_frame_update;
    engine.application_host.p_frame_resize   = &serial_frame_resize;
    engine.application_host.p_close          = &serial_close;
    engine.window.title                      = "serial demo";
    return shEditorMain(&engine);
}
#endif//SH_APPLICATION_TARGET_TYPE_EXECUTABLE

#ifdef __cplusplus
}
#endif//__cplusplus
