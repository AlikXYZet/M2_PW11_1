#include "PW11_StudentDatabaseWidget.h"
#include "Kismet/GameplayStatics.h"



/* ---   FTask_ConsumerOfStudentData   --- */

void FTask_ConsumerOfStudentData::DReact_AddStudent(const FStudentData iStudentData)
{
    UE_LOG(LogTemp, Error, TEXT("FTask_ConsumerOfStudentData::DReact_AddStudent"));
}

void FTask_ConsumerOfStudentData::DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef &MyCompletionGraphEvent)
{
    TaskDelegate_OnNewStudentData.AddLambda([&](const FStudentData iStudentData)
        {
            UE_LOG(LogTemp, Error, TEXT("FTask_ConsumerOfStudentData::DoTask - AddLambda"));
        });
    //TaskDelegate_OnNewStudentData.AddUFunction(this, TEXT("DReact_AddStudent"));
    UE_LOG(LogTemp, Warning, TEXT("FTask_ConsumerOfStudentData::DoTask"));
}
//----------------------------------------------------------------------------------------



/* ---   UPW11_StudentDatabaseWidget   --- */

UPW11_StudentDatabaseWidget::~UPW11_StudentDatabaseWidget()
{
    rConsumerTask = nullptr;
}

void UPW11_StudentDatabaseWidget::NativeConstruct()
{
    Super::NativeConstruct();

    APW11_GameStateBase *lCurrWorld = Cast<APW11_GameStateBase>(UGameplayStatics::GetGameState(GetWorld()));

    rConsumerTask = TGraphTask<FTask_ConsumerOfStudentData>::CreateTask(nullptr, ENamedThreads::AnyThread)
        .ConstructAndHold(OnNewStudentData);

    if (rConsumerTask)
        if (!rConsumerTask->GetCompletionEvent().IsValid())
        {
            rConsumerTask->Unlock();
            UE_LOG(LogTemp, Error, TEXT("rConsumerTask->Unlock()"));
        }
}
//----------------------------------------------------------------------------------------