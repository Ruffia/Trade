#pragma once

#ifdef DATABASE_EXPORT
#define EXPORT_CALSS __declspec(dllexport)
#else
#define EXPORT_CALSS __declspec(dllimport)
#endif