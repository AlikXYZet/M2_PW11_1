#include "PW11_StudentDatabaseWidget.h"

#include "MessageEndpointBuilder.h"

#include "M2PW11_1/Core/PW11_GameStateBase.h"



/* ---   Threads   --- */
bool FConsumer_Runnable::Init()
{
    ME_StudentDataReceiver = FMessageEndpoint::Builder(TEXT("Receiver_StudentData_Runnable"))
        .Handling<FStudentData>(this, &FConsumer_Runnable::BM_StudentDataHandler);

    if (ME_StudentDataReceiver.IsValid())
        ME_StudentDataReceiver->Subscribe<FStudentData>();

    return true;
}

uint32 FConsumer_Runnable::Run()
{
    while (!bIsStopThread)
    {
        FPlatformProcess::Sleep(0.01f);
    }

    return 1;
}

void FConsumer_Runnable::Stop()
{
    bIsStopThread = true;
}

void FConsumer_Runnable::Exit()
{
    if (ME_StudentDataReceiver.IsValid())
        ME_StudentDataReceiver.Reset();
}

void FConsumer_Runnable::BM_StudentDataHandler(const FStudentData &Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe> &Context)
{
    UE_LOG(LogTemp, Error, TEXT("FConsumer_Runnable::BM_StudentDataHandler NickName - %s"), *Message.Nickname);
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
            rConsumer_Class = new FConsumer_Runnable();
        }

        rConsumer_Thread = FRunnableThread::Create(
            rConsumer_Class,
            TEXT("ConsumerThread"),
            0,
            EThreadPriority::TPri_Normal);
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
    }
}
//----------------------------------------------------------------------------------------