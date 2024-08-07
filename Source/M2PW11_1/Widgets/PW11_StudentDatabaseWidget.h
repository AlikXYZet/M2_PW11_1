#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "M2PW11_1/Tools/MyStruct.h"

#include "PW11_StudentDatabaseWidget.generated.h"



class APW11_GameStateBase;



/* ---   TGraphTask   --- */

/**Класс таска потока-Потребителя
 * @param Делегат для FOnNewStudentData
 */
class FTask_ConsumerOfStudentData
{
    FOnNewStudentData TaskDelegate_OnNewStudentData;

public:
    FTask_ConsumerOfStudentData(FOnNewStudentData iDelegate)
        : TaskDelegate_OnNewStudentData(iDelegate) {}

    ~FTask_ConsumerOfStudentData() {}

    FORCEINLINE TStatId GetStatId() const
    {
        RETURN_QUICK_DECLARE_CYCLE_STAT(FTask_ConsumerOfStudentData, STATGROUP_TaskGraphTasks);
    }

    static ENamedThreads::Type GetDesiredThread()
    {

        FAutoConsoleTaskPriority myTaskPriority(
            TEXT("TaskGraph.TaskPriorities.LoadFileToString"),
            TEXT("Task and thread priority for file loading."),
            ENamedThreads::BackgroundThreadPriority,
            ENamedThreads::NormalTaskPriority,
            ENamedThreads::NormalTaskPriority);

        return myTaskPriority.Get();
    }

    static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::FireAndForget; }

    // Реакция на делегат: Добавить одного студента
    void DReact_AddStudent(const FStudentData iStudentData);

    void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef &MyCompletionGraphEvent);
};
//----------------------------------------------------------------------------------------



/**
 *
 */
UCLASS()
class M2PW11_1_API UPW11_StudentDatabaseWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    ~UPW11_StudentDatabaseWidget();

    virtual void NativeConstruct() override;

private:

    /* ---   FTask_ConsumerOfStudentData   --- */

    // Делегат передачи данных о новом студенте
    FOnNewStudentData OnNewStudentData;

    // Таск потока-Продюсера
    TGraphTask<FTask_ConsumerOfStudentData> *rConsumerTask;
    //--------------------------------------------
};
