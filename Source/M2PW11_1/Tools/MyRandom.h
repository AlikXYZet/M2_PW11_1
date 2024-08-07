#pragma once

#include "CoreMinimal.h"
#include <random>

// Получение рандомного значения [min, max]
int32 GetRandom(const int32 &min, const int32 &max);

// Получение рандомного bool значения
bool GetRandom();

// Получение рандомного bool значения
float GetRandomFloat(const float &min = 0.f, const float &max = 1.f);



/* ---   Получение рандомных данных студента   --- */

// Получение рандомного Имени (Псевдонима)
FString GetRandomNickname();

// Получение рандомного значения [0, 60]
uint8 GetRandomAge();

// Получение рандомного значения [0, 100]
uint8 GetRandomRating();

// Получение рандомного значения [0, UINT32_MAX]
uint32 GetRandomID();
//--------------------------------------------