
#include "DronePawn.h"

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

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DroneMappingContext, 0);
		}
	}

	if (DroneMesh)
	{
		float Mass = DroneMesh->GetMass();            // 드론 메쉬의 물리 질량
		HoverThrust = -GetWorld()->GetGravityZ() * Mass;
	}
}

// Called every frame
void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 추력 점진적 증가
	CurrentThrust = FMath::FInterpTo(CurrentThrust, TargetThrust, DeltaTime, InterpSpeed);

	if (GetActorLocation().Z < MaxAltitude)
	{
		ApplyAllThrust();
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


}

void ADronePawn::ApplyAllThrust()
{
	float PerPropellerThrust = CurrentThrust / 4.0f;
	FrontLeftPropeller->ApplyThrust(PerPropellerThrust);
	FrontRightPropeller->ApplyThrust(PerPropellerThrust);
	BackLeftPropeller->ApplyThrust(PerPropellerThrust);
	BackRightPropeller->ApplyThrust(PerPropellerThrust);
}


void ADronePawn::HoverUp(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("HoverUp Triggered"));

	float InputValue = Instance.GetValue().Get<float>();
	float AdditionalThrust = InputValue * (MaxThrust - HoverThrust);
	TargetThrust = FMath::Clamp(HoverThrust + AdditionalThrust, 0.f, MaxThrust);

	UE_LOG(LogTemp, Warning, TEXT("Input Value: %f"), InputValue);
}

