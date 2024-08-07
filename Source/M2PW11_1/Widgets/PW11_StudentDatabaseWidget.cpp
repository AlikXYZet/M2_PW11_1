#include "PW11_StudentDatabaseWidget.h"

#include "M2PW11_1/Core/PW11_GameStateBase.h"



/* ---   Threads   --- */

FConsumer_Runnable::FConsumer_Runnable(FOnNewStudentDataDelegate &iDelegate)
    : ThreadDelegate(iDelegate)
{
    //ThreadDelegate = iDelegate;
}

FConsumer_Runnable::~FConsumer_Runnable()
{
}

bool FConsumer_Runnable::Init()
{
    ThreadDelegate.BindRaw(this, &FConsumer_Runnable::DReact_AddStudent);

    UE_LOG(LogTemp, Error, TEXT("FConsumer_Runnable::Init"));

    return true;
}

uint32 FConsumer_Runnable::Run()
{
    UE_LOG(LogTemp, Error, TEXT("FConsumer_Runnable::Run"));

    while (!bIsStopThread)
    {
        FPlatformProcess::Sleep(0.01f);
    }

    return 1;
}

void FConsumer_Runnable::Stop()
{
    UE_LOG(LogTemp, Error, TEXT("FConsumer_Runnable::Stop"));

    bIsStopThread = true;
}

void FConsumer_Runnable::Exit()
{
    UE_LOG(LogTemp, Error, TEXT("FConsumer_Runnable::Exit"));
}

void FConsumer_Runnable::DReact_AddStudent(const FStudentData iStudentData)
{
    UE_LOG(LogTemp, Error, TEXT("FConsumer_Runnable::DReact_AddStudent:   Nickname - %s"), *iStudentData.Nickname);
}
//----------------------------------------------------------------------------------------



/* ---   UPW11_StudentDatabaseWidget   --- */

UPW11_StudentDatabaseWidget::~UPW11_StudentDatabaseWidget()
{
    StopConsumerThread();
}

void UPW11_StudentDatabaseWidget::NativeConstruct()
{
    Super::NativeConstruct();

    CreateConsumerThread();
}

void UPW11_StudentDatabaseWidget::CreateConsumerThread()
{
    if (!rConsumer_Thread)
    {
        if (!rConsumer_Class)
        {
            APW11_GameStateBase *CurrentGameState = Cast<APW11_GameStateBase>(GetWorld()->GetGameState());
            rConsumer_Class = new FConsumer_Runnable(CurrentGameState->OnNewStudentDataDelegate);

            //rConsumer_Class = new FConsumer_Runnable(OnNewStudentDataDelegate);
        }


        rConsumer_Thread = FRunnableThread::Create(
            rConsumer_Class,
            TEXT("ConsumerThread"),
            0,
            EThreadPriority::TPri_Normal);

        UE_LOG(LogTemp, Warning, TEXT("Create ConsumerThread"));
    }
}

void UPW11_StudentDatabaseWidget::StopConsumerThread()
{
    if (rConsumer_Thread)
    {
        rConsumer_Thread->Suspend(false);
        rConsumer_Thread->Kill(true);

        rConsumer_Thread = nullptr;
        rConsumer_Class = nullptr;

        UE_LOG(LogTemp, Warning, TEXT("Stop ConsumerThread"));
    }
}
//----------------------------------------------------------------------------------------