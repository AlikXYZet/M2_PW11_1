#include "PW11_StudentDatabaseWidget.h"

#include "MessageEndpointBuilder.h"

#include "M2PW11_1/Core/PW11_GameStateBase.h"



/* ---   Threads   --- */

FConsumer_Runnable::FConsumer_Runnable(
	UPW11_StudentDatabaseWidget *irSDWidget,
	const TMap<FString, FStudentData> iCurrentStudentDatabase)
{
	// in GameThread

	rSDWidget = irSDWidget;

	for (auto &Data : iCurrentStudentDatabase)
	{
		LocalStudentDatabase.Add(Data.Value);
	}

	//UE_LOG(LogTemp, Error, TEXT("GameThreadId - %d"), FPlatformTLS::GetCurrentThreadId());
}

bool FConsumer_Runnable::Init()
{
	// Инициализация получения и обработки данных
	ME_StudentDataReceiver = FMessageEndpoint::Builder(TEXT("Receiver_StudentData_Runnable"))
		.Handling<FStudentData>(this, &FConsumer_Runnable::BM_StudentDataHandler);

	// Подписка на получение данных
	if (ME_StudentDataReceiver.IsValid())
		ME_StudentDataReceiver->Subscribe<FStudentData>();

	// Подписка функции на делегат
	rSDWidget->OnReSort.BindRaw(this, &FConsumer_Runnable::ReSortArray);

	//UE_LOG(LogTemp, Error, TEXT("FConsumer_Runnable::Init   CurrentThreadId - %d"), FPlatformTLS::GetCurrentThreadId());

	return true;
}

uint32 FConsumer_Runnable::Run()
{
	while (!bIsStopThread)
	{
		FPlatformProcess::Sleep(1.01f);
	}

	return 1;
}

void FConsumer_Runnable::Stop()
{
	bIsStopThread = true;
}

void FConsumer_Runnable::Exit()
{
	rSDWidget->OnReSort.Unbind();

	if (ME_StudentDataReceiver.IsValid())
		ME_StudentDataReceiver.Reset();

	LocalStudentDatabase.Empty();
}

void FConsumer_Runnable::BM_StudentDataHandler(const FStudentData &Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe> &Context)
{
	LocalStudentDatabase.Add(Message);

	SendDataToWidget();
}

void FConsumer_Runnable::ReSortArray()
{
	switch (rSDWidget->CurrentSortType)
	{
	case ESortType::NicknameUp:
		CurrentSortingPredicate = [](const FStudentData &first, const FStudentData &second)
			{
				return first.Nickname < second.Nickname;
			};
		break;

	case ESortType::NicknameDown:
		CurrentSortingPredicate = [](const FStudentData &first, const FStudentData &second)
			{
				return first.Nickname > second.Nickname;
			};
		break;

	case ESortType::AgeUp:
		CurrentSortingPredicate = [](const FStudentData &first, const FStudentData &second)
			{
				return first.Age < second.Age;
			};
		break;

	case ESortType::AgeDown:
		CurrentSortingPredicate = [](const FStudentData &first, const FStudentData &second)
			{
				return first.Age > second.Age;
			};
		break;

	case ESortType::RatingUp:
		CurrentSortingPredicate = [](const FStudentData &first, const FStudentData &second)
			{
				return first.Rating < second.Rating;
			};
		break;

	case ESortType::RatingDown:
		CurrentSortingPredicate = [](const FStudentData &first, const FStudentData &second)
			{
				return first.Rating > second.Rating;
			};
		break;

	case ESortType::IDUp:
		CurrentSortingPredicate = [](const FStudentData &first, const FStudentData &second)
			{
				return first.ID < second.ID;
			};
		break;

	case ESortType::IDDown:
		CurrentSortingPredicate = [](const FStudentData &first, const FStudentData &second)
			{
				return first.ID > second.ID;
			};
		break;

	default:
		break;
	}

	SendDataToWidget();
}

void FConsumer_Runnable::SendDataToWidget()
{
	LocalStudentDatabase.Sort(CurrentSortingPredicate);

	rSDWidget->ArrayStudentData = LocalStudentDatabase;

	rSDWidget->bIsNewData = true;
}
//----------------------------------------------------------------------------------------



/* ---   Base   --- */

UPW11_StudentDatabaseWidget::~UPW11_StudentDatabaseWidget()
{
	StopConsumerThread();
}

void UPW11_StudentDatabaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CreateConsumerThread();
}

void UPW11_StudentDatabaseWidget::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsNewData)
	{
		UpdateWidgetStudentData(ArrayStudentData);
		bIsNewData = false;
	}
}
//----------------------------------------------------------------------------------------



/* ---   Database in Widget   --- */

void UPW11_StudentDatabaseWidget::SetSortType(const ESortType InSortType)
{
	if (CurrentSortType == InSortType)
		CurrentSortType = ESortType(uint8(InSortType) + 0x80);
	else
		CurrentSortType = InSortType;

	OnReSort.ExecuteIfBound();
}
//----------------------------------------------------------------------------------------



/* ---   Threads   --- */

void UPW11_StudentDatabaseWidget::CreateConsumerThread()
{
	if (!rConsumer_Thread)
	{
		if (!rConsumer_Class)
		{
			APW11_GameStateBase *lGameStateBase = Cast<APW11_GameStateBase>(GetWorld()->GetGameState());
			rConsumer_Class = new FConsumer_Runnable(
				this,
				lGameStateBase->GetStudentsDatabase());
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
