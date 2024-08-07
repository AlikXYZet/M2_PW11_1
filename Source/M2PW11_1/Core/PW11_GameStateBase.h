#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "M2PW11_1/Tools/MyStruct.h"

#include "PW11_GameStateBase.generated.h"



/* ---   TGraphTask   --- */

/**Класс таска потока-Продюсера
 * @param Делегат для FOnNewStudentData
 */
class FTask_ProducerOfStudentData
{
    FOnNewStudentDataDelegate TaskDelegate_OnNewStudentData;

public:
    FTask_ProducerOfStudentData(FOnNewStudentDataDelegate iDelegate)
        : TaskDelegate_OnNewStudentData(iDelegate) {}

    ~FTask_ProducerOfStudentData() {}

    FORCEINLINE TStatId GetStatId() const
    {
        RETURN_QUICK_DECLARE_CYCLE_STAT(FTask_ProducerOfStudentData, STATGROUP_TaskGraphTasks);
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

    void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef &MyCompletionGraphEvent);
};
//----------------------------------------------------------------------------------------



/**
 *
 */
UCLASS()
class M2PW11_1_API APW11_GameStateBase : public AGameStateBase
{
    GENERATED_BODY()

public:

    APW11_GameStateBase();

protected:

    virtual void BeginPlay() override;

public:

    virtual void Tick(float DeltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;



    /* ---   FTask_ProducerOfStudentData   --- */

    // Делегат передачи данных о новом студенте
    FOnNewStudentDataDelegate OnNewStudentDataDelegate;

    // Таск потока-Продюсера
    TGraphTask<FTask_ProducerOfStudentData> *rProducerTask;
    //--------------------------------------------



    /* ---   Student Database   --- */

    /** Получить информацию о всех студентах
    * @return Копия контейнера TMap с базой данных студентов
    */
    TMap<FString, FStudentData> GetStudentsDatabase();

    // Сбросить базу данных студентов
    void EmptyStudentDatabase();
    //--------------------------------------------

private:

    /* ---   Student Database   --- */

    // База данных студентов
    TMap<FString, FStudentData> StudentsDatabase;

    // Реакция на делегат: Добавить одного студента
    void DReact_AddStudent(const FStudentData iStudentData);
    //--------------------------------------------
};
