#pragma once

#include "CoreMinimal.h"
#include <random>

// Получение рандомного значения [min, max]
int64 GetRandom(const int64 &min, const int64 &max);

// Получение рандомного bool значения
bool GetRandom();

// Получение рандомного bool значения
float GetRandomFloat(const float &min = 0.f, const float &max = 1.f);



/* ---   Получение рандомных данных студента   --- */

// Получение рандомного Имени (Псевдонима)
FString GetRandomNickname();

// Получение рандомного значения [0, 60]
int GetRandomAge();

// Получение рандомного значения [0, 100]
int GetRandomRating();

// Получение рандомного значения [0, UINT32_MAX]
int GetRandomID();
//--------------------------------------------