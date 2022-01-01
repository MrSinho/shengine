#ifndef SH_THERMODYNAMICS_HANDLE_H
#define SH_THERMODYNAMICS_HANDLE_H

typedef struct ShThermodynamicsHandle {
	float mass;
	float density;
	float thermal_expansion_coefficient;
	float specific_heat;
} ShThermodynamicsHandle;

#endif//SH_THERMODYNAMICS_HANDLE_H