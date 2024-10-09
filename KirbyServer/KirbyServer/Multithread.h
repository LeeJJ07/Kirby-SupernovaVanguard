#pragma once

#include <iostream>
#include <Windows.h>
#include <process.h>

static CRITICAL_SECTION criticalSection;

static bool isChecked;