#pragma once

#include "CoreMinimal.h"
#include "MyStruct.generated.h"



/* ---   Structs   --- */

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
//----------------------------------------------------------------------------------------



/* ---   Delegates   --- */

// Делегат передачи данных о новом студенте
DECLARE_DELEGATE_OneParam(FOnNewStudentDataDelegate, FStudentData NewStudentData);
//----------------------------------------------------------------------------------------