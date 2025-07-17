
#include "DronePawn.h"

// Sets default values
ADronePawn::ADronePawn()
{
 	
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

	GravityZ = -980.f;
	TotalMass = 1.f;
	HoverThrust = 0.f;
	PerPropellerThrust = 20000.f;
	CurrentAltitude = 0.f;
	CurrentHoverPower = 0.f;
}

// Called when the game starts or when spawned
void ADronePawn::BeginPlay()
{
	Super::BeginPlay();

	if (DroneMesh)
	{
		GravityZ = GetWorld()->GetGravityZ();
		TotalMass = DroneMesh->GetMass();
		HoverThrust = -GravityZ * TotalMass;

		PerPropellerThrust = HoverThrust * 1.05f / 4.f; // 마진을 살짝 더해 안정적인 호버링 유도
		UE_LOG(LogTemp, Warning, TEXT("[Thrust Init] Gravity: %.2f, Mass: %.2f, HoverThrust: %.2f, PerPropeller: %.2f"), GravityZ, TotalMass, HoverThrust, PerPropellerThrust);
	
		DroneMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
		DroneMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		DroneMesh->SetAngularDamping(10.f); // 회전 억제값 설정

		DroneMesh->BodyInstance.bLockXRotation = true;
		DroneMesh->BodyInstance.bLockYRotation = true;
		DroneMesh->BodyInstance.bLockZRotation = true;
	}


	UE_LOG(LogTemp, Warning, TEXT("Drone mass: %f"), TotalMass);
	UE_LOG(LogTemp, Warning, TEXT("Gravity: %f"), GravityZ);
	UE_LOG(LogTemp, Warning, TEXT("HoverThrust: %f"), HoverThrust);
	UE_LOG(LogTemp, Warning, TEXT("PerPropellerThrust: %f"), PerPropellerThrust);


	APlayerController* PC = Cast<APlayerController>(GetController());

	if (PC)
	{
		PC->Possess(this);
		UE_LOG(LogTemp, Warning, TEXT("drone Possess succeed: %s"), *PC->GetName());

		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (Subsystem && IMC_DroneControls)  // IMC_DroneControls은 UPROPERTY로 선언된 매핑 컨텍스트
		{
			Subsystem->AddMappingContext(IMC_DroneControls, 0);
	
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("drone Possess 실패. GetController() = nullptr"));
	}


}

// Called every frame
void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!DroneMesh) return;
	CurrentAltitude = DroneMesh->GetComponentLocation().Z;

	if (CurrentAltitude >= MaxAltitude)
	{
		bIsHoveringAllowed = false;
	}
	else
	{
		bIsHoveringAllowed = true;
	}


	// 점진적 호버링 구현
	if (bIsHoveringAllowed && bHoverInputHeld)
	{
		CurrentHoverPower = FMath::FInterpTo(CurrentHoverPower, 1.f, DeltaTime, InterpSpeed); 
	}
	else
	{
		CurrentHoverPower = FMath::FInterpTo(CurrentHoverPower, 0.f, DeltaTime, InterpSpeed); 
	}

	float HoverThrustScaled = PerPropellerThrust * CurrentHoverPower; 

	UE_LOG(LogTemp, Warning, TEXT("[Tick] Alt: %.2f, Hovering: %s, HoverPower: %.2f, Thrust: %.2f"),
		CurrentAltitude,
		bIsHoveringAllowed ? TEXT("True") : TEXT("False"),
		CurrentHoverPower,
		HoverThrustScaled);

	FrontLeftPropeller->ApplyThrust(HoverThrustScaled * FrontLeftScale, DroneMesh);
	FrontRightPropeller->ApplyThrust(HoverThrustScaled * FrontRightScale, DroneMesh);
	BackLeftPropeller->ApplyThrust(HoverThrustScaled * BackLeftScale, DroneMesh);
	BackRightPropeller->ApplyThrust(HoverThrustScaled * BackRightScale, DroneMesh);


}

// Called to bind functionality to input
void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(IA_HoverUp, ETriggerEvent::Triggered, this, &ADronePawn::HoverUp);
		EnhancedInput->BindAction(IA_HoverUp, ETriggerEvent::Completed, this, &ADronePawn::HoverUpReleased);
		//EnhancedInput->BindAction(IA_HoverDown, ETriggerEvent::Triggered, this, &ADronePawn::HoverDown);
		EnhancedInput->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &ADronePawn::MoveForward);
		EnhancedInput->BindAction(IA_MoveForward, ETriggerEvent::Completed, this, &ADronePawn::MoveForwardReleased);
		/*	EnhancedInput->BindAction(IA_MoveBackward, ETriggerEvent::Triggered, this, &ADronePawn::MoveBackward);
		EnhancedInput->BindAction(IA_MoveLeft, ETriggerEvent::Triggered, this, &ADronePawn::MoveLeft);
		EnhancedInput->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &ADronePawn::MoveRight);*/
	}


}

void ADronePawn::ApplyAllThrust()
{
	float Thrust = FMath::Clamp(PerPropellerThrust, 0.f, MaxThrust);
	FrontLeftPropeller->ApplyThrust(Thrust, DroneMesh);
	FrontRightPropeller->ApplyThrust(Thrust, DroneMesh);
	BackLeftPropeller->ApplyThrust(Thrust, DroneMesh);
	BackRightPropeller->ApplyThrust(Thrust, DroneMesh);

}


void ADronePawn::HoverUp(const FInputActionInstance& Instance)
{
	
	bHoverInputHeld = true;
	
}

void ADronePawn::HoverUpReleased(const FInputActionInstance& Instance)
{
	bHoverInputHeld = false; 
	DroneMesh->SetPhysicsLinearVelocity(DroneMesh->GetPhysicsLinearVelocity() * 0.5f); // 감속
	DroneMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector); // 회전 제거
}

void ADronePawn::MoveForward(const FInputActionInstance& Instance)
{
	float InputValue = Instance.GetValue().Get<float>(); // 0 ~ 1
	FrontLeftScale = FMath::Clamp(1.f - InputValue * 0.2f, 0.5f, 1.f); // 줄이기
	FrontRightScale = FrontLeftScale;

	BackLeftScale = FMath::Clamp(1.f + InputValue * 0.2f, 1.f, 1.5f); // 늘리기
	BackRightScale = BackLeftScale;
}

void ADronePawn::MoveForwardReleased(const FInputActionInstance& Instance)
{
	FrontLeftScale = 1.f;
	FrontRightScale = 1.f;
	BackLeftScale = 1.f;
	BackRightScale = 1.f;
}
