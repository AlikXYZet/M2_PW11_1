﻿#pragma once

#include "CoreMinimal.h"
#include "MyStruct.generated.h"


// Структура данных о студентах
USTRUCT(BlueprintType)
struct FStudentData
{
    GENERATED_BODY()

    FString Nickname;
    uint8 Age;
    uint8 Rating;
    uint32 ID = 0;
};