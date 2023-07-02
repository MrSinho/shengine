#ifndef SH_ENGINE_EXPORT_H



#ifdef _WIN32
#define SH_ENGINE_EXPORT_FUNCTION __declspec(dllexport)
#else
#define SH_ENGINE_EXPORT_FUNCTION
#endif//_WIN32



#endif//SH_ENGINE_EXPORT_H
