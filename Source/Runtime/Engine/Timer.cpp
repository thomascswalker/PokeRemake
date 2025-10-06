#include "Timer.h"

PTimerManager* GTimerManager = nullptr;

size_t PTimerManager::sNextHandle = 0;