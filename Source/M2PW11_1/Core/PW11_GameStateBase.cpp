#include "PW11_GameStateBase.h"
#include "Async/TaskGraphInterfaces.h"
#include "HAL/ThreadManager.h"
#include "Kismet/GameplayStatics.h"

#include "M2PW11_1/Tools/MyRandom.h"



/* ---   FTask_ProducerOfStudentData   --- */

void FTask_ProducerOfStudentData::DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef &MyCompletionGraphEvent)
{
    if (TaskDelegate_OnNewStudentData.IsBound())
    {
        FStudentData lStudentData;
        for (int8 i = 0; i < 20; ++i)
        {
            lStudentData = {
            GetRandomNickname(),
            GetRandomAge(),
            GetRandomRating(),
            GetRandomID() };

            FPlatformProcess::Sleep(GetRandomFloat(0.5f, 3.f));

            TaskDelegate_OnNewStudentData.Broadcast(lStudentData);
        }
    }
}
//----------------------------------------------------------------------------------------



/* ---   APW11_GameStateBase : Base   --- */

APW11_GameStateBase::APW11_GameStateBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void APW11_GameStateBase::BeginPlay()
{
    Super::BeginPlay();

    // Подписка на делегат. 
    OnNewStudentData.AddUObject(this, &APW11_GameStateBase::DReact_AddStudent);

    rProducerTask = TGraphTask<FTask_ProducerOfStudentData>::CreateTask(nullptr, ENamedThreads::AnyThread)
        .ConstructAndHold(OnNewStudentData);
}

void APW11_GameStateBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APW11_GameStateBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}
//----------------------------------------------------------------------------------------



/* ---   Student Database   --- */

TMap<FString, FStudentData> APW11_GameStateBase::GetStudentsDatabase()
{
    return StudentsDatabase;
}

void APW11_GameStateBase::EmptyStudentDatabase()
{
    StudentsDatabase.Empty();
}

void APW11_GameStateBase::DReact_AddStudent(const FStudentData iStudentData)
{
    StudentsDatabase.Add(iStudentData.Nickname, iStudentData);
}
//----------------------------------------------------------------------------------------
