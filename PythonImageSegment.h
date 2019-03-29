#pragma once
#include <stdio.h>
#include "limits.h"
#include <direct.h>
#include <Python.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

//#define PATH_MAX 1024

wchar_t *GetWC(const char *c);

void PythonImageSegment(const char* fileSrc, const char* fileGrowing, const char* fileEdge);