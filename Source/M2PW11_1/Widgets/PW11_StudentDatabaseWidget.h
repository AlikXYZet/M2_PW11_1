#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MessageEndpoint.h"

#include "M2PW11_1/Tools/MyStruct.h"

#include "PW11_StudentDatabaseWidget.generated.h"



/* ---   Threads   --- */

//����� ������-�����������
class M2PW11_1_API FConsumer_Runnable : public FRunnable
{
public:

    FConsumer_Runnable() {};

    virtual ~FConsumer_Runnable() override {};

    /* ---   in FRunnable   --- */

    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
    virtual void Exit() override;
    //--------------------------------------------

private:

    // �������� ������ ������
    //FThreadSafeBool bIsStopThread = false;
    bool bIsStopThread = false;

    // "����������" ������
    TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> ME_StudentDataReceiver;

    // "����������" ������
    void BM_StudentDataHandler(
        const struct FStudentData &Message,
        const TSharedRef<IMessageContext, ESPMode::ThreadSafe> &Context);
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

    /* ---   Threads   --- */

    FConsumer_Runnable *rConsumer_Class = nullptr;
    FRunnableThread *rConsumer_Thread = nullptr;

    // �������� ������-����������
    void CreateConsumerThread();

    // ������� ������-����������
    void StopConsumerThread();
    //--------------------------------------------
};
