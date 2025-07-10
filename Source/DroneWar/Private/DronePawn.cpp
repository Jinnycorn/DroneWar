
#include "DronePawn.h"


// 추력 계산용 상수
float GravityZ;
float TotalMass;
float HoverThrust;
float PerPropellerThrust;

// Sets default values
ADronePawn::ADronePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시 생성 및 루트 컴포넌트 설정
	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
	RootComponent = DroneMesh;

	DroneMesh->SetSimulatePhysics(true);   // 물리 시뮬레이션
	DroneMesh->SetEnableGravity(true);     // 중력 적용


	FrontLeftPropeller = CreateDefaultSubobject<UPropellerComponent>(TEXT("FrontLeft"));
	FrontLeftPropeller->SetupAttachment(DroneMesh);

	FrontRightPropeller = CreateDefaultSubobject<UPropellerComponent>(TEXT("FrontRight"));
	FrontRightPropeller->SetupAttachment(DroneMesh);

	BackLeftPropeller = CreateDefaultSubobject<UPropellerComponent>(TEXT("BackLeft"));
	BackLeftPropeller->SetupAttachment(DroneMesh);

	BackRightPropeller = CreateDefaultSubobject<UPropellerComponent>(TEXT("BackRight"));
	BackRightPropeller->SetupAttachment(DroneMesh);
}

// Called when the game starts or when spawned
void ADronePawn::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(GetController());

	if (PC)
	{
		PC->Possess(this);
		UE_LOG(LogTemp, Warning, TEXT("drone Possess succeed: %s"), *PC->GetName());

		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (Subsystem && IMC_DroneControls)  // IMC_DroneControls은 UPROPERTY로 선언된 매핑 컨텍스트
		{
			Subsystem->AddMappingContext(IMC_DroneControls, 0);
			
			//UE_LOG(LogTemp, Warning, TEXT("IMC OK"));
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("drone Possess 실패. GetController() = nullptr"));
	}


	if (DroneMesh)
	{
		TotalMass = DroneMesh->GetMass()
			+ FrontLeftPropeller->GetMass()
			+ FrontRightPropeller->GetMass()
			+ BackLeftPropeller->GetMass()
			+ BackRightPropeller->GetMass();

		HoverThrust = -GetWorld()->GetGravityZ() * TotalMass;

		// 필요하면 PerPropellerThrust 미리 계산
		PerPropellerThrust = HoverThrust / 4.0f;

		UE_LOG(LogTemp, Warning, TEXT("TotalMass: %f / HoverThrust: %f / PerPropeller: %f"),
			TotalMass, HoverThrust, PerPropellerThrust);
	}


}

// Called every frame
void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// 1. 추력 점진적 변화 (CurrentThrust → TargetThrust로 부드럽게)
	CurrentThrust = FMath::FInterpTo(CurrentThrust, TargetThrust, DeltaTime, InterpSpeed);

	// 2. 고도 제한 적용: 너무 높이 안 뜨게 막기
	if (GetActorLocation().Z < MaxAltitude)
	{
		// 3. 추력 분산해서 각 날개에 적용
		float ForceZ = CurrentThrust / 4.0f;
		FVector UpForce = FVector(0.f, 0.f, ForceZ);

		FrontLeftPropeller->ApplyThrust(PerPropellerThrust);
		FrontRightPropeller->ApplyThrust(PerPropellerThrust);
		BackLeftPropeller->ApplyThrust(PerPropellerThrust);
		BackRightPropeller->ApplyThrust(PerPropellerThrust);
	}
	else
	{
		// 고도 초과 시 추력 감소 (빠르게 하강하도록)
		TargetThrust = 0.f;
	}
}

// Called to bind functionality to input
void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(IA_HoverUp, ETriggerEvent::Triggered, this, &ADronePawn::HoverUp);
	/*	EnhancedInput->BindAction(IA_HoverDown, ETriggerEvent::Triggered, this, &ADronePawn::HoverDown);
		EnhancedInput->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &ADronePawn::MoveForward);
		EnhancedInput->BindAction(IA_MoveBackward, ETriggerEvent::Triggered, this, &ADronePawn::MoveBackward);
		EnhancedInput->BindAction(IA_MoveLeft, ETriggerEvent::Triggered, this, &ADronePawn::MoveLeft);
		EnhancedInput->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &ADronePawn::MoveRight);*/
	}

	// 단순 키 입력만 바인딩
	/*UE_LOG(LogTemp, Warning, TEXT("InputComponent Setting"));

	PlayerInputComponent->BindAction("HoverUp", IE_Pressed, this, &ADronePawn::OnHoverUp);*/
}

void ADronePawn::ApplyAllThrust()
{
	if (!DroneMesh || !DroneMesh->IsSimulatingPhysics()) return;

	PerPropellerThrust = CurrentThrust / 4.0f;

	// 총합 추력 Z 방향으로 적용 (드론 전체)
	FVector UpwardForce = FVector(0.f, 0.f, PerPropellerThrust * 4.f);  // = CurrentThrust
	DroneMesh->AddForce(UpwardForce);

	// 각각의 프로펠러 비주얼 회전도 처리할 수 있음 (선택적)
	FrontLeftPropeller->ApplyThrust(PerPropellerThrust);
	FrontRightPropeller->ApplyThrust(PerPropellerThrust);
	BackLeftPropeller->ApplyThrust(PerPropellerThrust);
	BackRightPropeller->ApplyThrust(PerPropellerThrust);
}


void ADronePawn::HoverUp(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("HoverUp Triggered"));

	// 입력값 (0.0 ~ 1.0)
	float InputValue = Instance.GetValue().Get<float>();

	// 힘 증폭 (1.2배까지 허용)
	float ScaledThrust = PerPropellerThrust * FMath::Clamp(InputValue * 1.2f, 0.f, 1.5f);

	// 각 프로펠러에 AddForce 적용 (Z+ 방향으로)
	FVector Force = FVector(0.f, 0.f, ScaledThrust);

	FrontLeftPropeller->ApplyThrust(PerPropellerThrust);
	FrontRightPropeller->ApplyThrust(PerPropellerThrust);
	BackLeftPropeller->ApplyThrust(PerPropellerThrust);
	BackRightPropeller->ApplyThrust(PerPropellerThrust);

	UE_LOG(LogTemp, Warning, TEXT("Input: %f / Scaled Thrust: %f"), InputValue, ScaledThrust);
}
