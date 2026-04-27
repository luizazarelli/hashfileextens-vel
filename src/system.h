#include "libs.h"
#include "paths.h"

#ifndef SYSTEM_H
#define SYSTEM_H

void readParam(int argc, char** argv, void* paths);

void readPrintGeo(void* paths, void* hash_quadras);
void readPm(void* paths, void* hash_hab);
void readQry(void* paths, void* hash_quadras, void* hash_hab);

#endif
