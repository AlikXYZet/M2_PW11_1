#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MessageEndpoint.h"

#include "M2PW11_1/Tools/MyStruct.h"

#include "PW11_StudentDatabaseWidget.generated.h"



// Тип предиката сортировки
typedef bool (*Predicate)(const FStudentData &first, const FStudentData &second);
//----------------------------------------------------------------------------------------



/* ---   Delegates   --- */

DECLARE_DELEGATE(FReSort);
//----------------------------------------------------------------------------------------



/* ---   ENums   --- */

// Типы сортировки
UENUM(BlueprintType)
enum struct ESortType : uint8
{
	NONE,

	// Возрастающий список
	NicknameUp UMETA(DisplayName = "Nickname Up"),
	AgeUp      UMETA(DisplayName = "Age Up"),
	RatingUp   UMETA(DisplayName = "Rating Up"),
	IDUp       UMETA(DisplayName = "ID Up"),

	// Спадающий список
	NicknameDown = NicknameUp + 0x80 UMETA(DisplayName = "Nickname Down"),
	AgeDown    UMETA(DisplayName = "Age Down"),
	RatingDown UMETA(DisplayName = "Rating Down"),
	IDDown     UMETA(DisplayName = "ID Down"),
};

//----------------------------------------------------------------------------------------



/* ---   Threads   --- */

//Класс потока-Потребителя
class M2PW11_1_API FConsumer_Runnable : public FRunnable
{
public:

	/** Конструктор потока получения данных
	*
	* @param irSDWidget - Указатель на виджет.
	* Необходим для использования делегатов и передачи отсортированного массива в TQueue
	*
	* @param iCurrentStudentDatabase - Текущая база данных из GameStateBase
	*/
	FConsumer_Runnable(
		UPW11_StudentDatabaseWidget *irSDWidget,
		const TMap<FString, FStudentData> iCurrentStudentDatabase);

	virtual ~FConsumer_Runnable() override {};

	/* ---   in FRunnable   --- */

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	//--------------------------------------------

private:

	// Контроль работы потока
    //FThreadSafeBool bIsStopThread = false;
	bool bIsStopThread = false;

	// Указатель на виджет
	UPW11_StudentDatabaseWidget *rSDWidget;

	// Локальная база данных потока. В нём данные сортируются и передаются виджету
	TArray<FStudentData> LocalStudentDatabase;

	// Текущий предикат сортировки
	Predicate CurrentSortingPredicate = [](const FStudentData &first, const FStudentData &second)
		{
			return first.Nickname < second.Nickname;
		};

	// Месседж-"Получатель" данных из потока-"Продюсера"
	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> ME_StudentDataReceiver;

	// Функция Месседжа: Обрабодка данных из потока-"Продюсера"
	void BM_StudentDataHandler(
		const struct FStudentData &Message,
		const TSharedRef<IMessageContext, ESPMode::ThreadSafe> &Context);

	// Функция делегата: Смена сортировки
	void ReSortArray();

	// Передача данных в Виджет с предварительной сортировкой
	void SendDataToWidget();
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

	/* ---   Base   --- */

	~UPW11_StudentDatabaseWidget();

	// Аналогичен простому Construct()
	virtual void NativeConstruct() override;

	// Аналогичен простому Tick(")
	virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;
	//--------------------------------------------



	/* ---   Delegates   --- */

	// Делегат смены сортировки
	FReSort OnReSort;
	//--------------------------------------------



	/* ---   Database in Widget   --- */

	// Локальная база данных о студентах, заполняется из потока-"Получателя"
	TArray< FStudentData> WidgetStudentDatabase;

	// Контроль обработки данных: если true, то сбрасывается и 
	// запускается Event обновления данных в Виджете [ UpdateWidgetStudentData(") ]
	bool bIsNewData = false;

	// Перемменная типа сортировки
	std::atomic<ESortType> CurrentSortType = ESortType::NicknameUp;

	// Event обновления данных в Виджете
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "StudentData")
	void UpdateWidgetStudentData(const TArray<FStudentData> &StudentDatabase);

	// Функция изменения типа сортировки
	UFUNCTION(BlueprintCallable, Category = "StudentData")
	void SetSortType(const ESortType InSortType);
	//--------------------------------------------



private:

	/* ---   Threads   --- */

	FConsumer_Runnable *rConsumer_Class = nullptr;
	FRunnableThread *rConsumer_Thread = nullptr;

	// Создание потока-"Получателя"
	void CreateConsumerThread();

	// Останов потока-"Получателя"
	void StopConsumerThread();
	//--------------------------------------------
};
