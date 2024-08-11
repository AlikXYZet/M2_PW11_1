#pragma once

#include "CoreMinimal.h"
#include "MyStruct.generated.h"



/* ---   Structs   --- */

// Структура данных о студентах
USTRUCT(BlueprintType)
struct FStudentData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Nickname;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Age;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Rating;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID = 0;
};
//----------------------------------------------------------------------------------------