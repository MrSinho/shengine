/**
 * @file shExport.h
 * 
 * @brief Defines the export function attribute for the `shengine` module.
 */

#ifndef SH_ENGINE_EXPORT_H
#define SH_ENGINE_EXPORT_H

#ifdef _WIN32
/**
 * @def SH_ENGINE_EXPORT_FUNCTION
 * @brief Specifies the export function attribute for Windows platforms.
 */
#define SH_ENGINE_EXPORT_FUNCTION __declspec(dllexport)
#else
/**
 * @def SH_ENGINE_EXPORT_FUNCTION
 * @brief Specifies the export function attribute for non-Windows platforms.
 */
#define SH_ENGINE_EXPORT_FUNCTION
#endif//_WIN32



#endif//SH_ENGINE_EXPORT_H
