#include "PW11_GameStateBase.h"

#include "MessageEndpointBuilder.h"

#include "M2PW11_1/Tools/MyRandom.h"



/* ---   FTask_ProducerOfStudentData   --- */

FTask_ProducerOfStudentData::FTask_ProducerOfStudentData()
{
    ME_StudentDataSender = FMessageEndpoint::Builder("Sender_Producer_Task").Build();;
}

void FTask_ProducerOfStudentData::DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef &MyCompletionGraphEvent)
{
    /**
    * Инициализация рандомных данных и их отправка получателям
    */

    if (ME_StudentDataSender.IsValid())
    {
        FStudentData lStudentData;

        for (int8 i = 0; i < 20; ++i)
        {
            lStudentData = {
            GetRandomNickname(),
            GetRandomAge(),
            GetRandomRating(),
            GetRandomID() };

            // Имитация задержки данных
            FPlatformProcess::Sleep(GetRandomFloat(0.1f, 1.f));

            ME_StudentDataSender->Publish<FStudentData>(new FStudentData(lStudentData));
        }

        ME_StudentDataSender.Reset();
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

    // Инициализация Таска
    rProducerTask = TGraphTask<FTask_ProducerOfStudentData>::CreateTask(nullptr, ENamedThreads::AnyThread)
        .ConstructAndHold();

    // Запуск Таска
    if (rProducerTask)
        if (!rProducerTask->GetCompletionEvent().IsValid())
        {
            rProducerTask->Unlock();
        }
}

void APW11_GameStateBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APW11_GameStateBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    EmptyStudentDatabase();

    rProducerTask = nullptr;
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
    UE_LOG(LogTemp, Warning, TEXT("APW11_GameStateBase::DReact_AddStudent:   Nickname - %s"), *iStudentData.Nickname);
}
//----------------------------------------------------------------------------------------
