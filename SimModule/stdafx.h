#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define WINVER									0x0500 // Windows 2000

#include <afx.h>
#include <atlcoll.h>
#include <atlstr.h>
#include <assert.h>
#include <stdio.h>
#include <conio.h>
#include "wLock.h"
#include "wRand.h"

#ifdef DEBUG_NEW
#define new DEBUG_NEW
#endif
