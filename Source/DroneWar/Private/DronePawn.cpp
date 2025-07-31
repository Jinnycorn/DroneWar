
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

	bForwardInputHeld = false;

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



	if (bHoverInputHeld)
		CurrentHoverPower = FMath::FInterpTo(CurrentHoverPower, 1.f, DeltaTime, InterpSpeed);
	else
		CurrentHoverPower = FMath::FInterpTo(CurrentHoverPower, 0.f, DeltaTime, InterpSpeed);

	float HoverThrustScaled = PerPropellerThrust * CurrentHoverPower;

	float ForwardBias = bForwardInputHeld ? 0.5f : 0.f;
	float FrontScale = 1.f - ForwardBias;
	float BackScale = 1.f + ForwardBias;

	
	FVector ForwardVector = GetActorForwardVector();    // ← 추가됨**
	FVector UpVector = FVector::UpVector;               // ← 추가됨**
	FVector ForceDir_FLBR = (UpVector + ForwardVector * ForwardBias).GetSafeNormal(); // ← 추가됨**
	FVector ForceDir_FRBL = UpVector.GetSafeNormal();   // ← 추가됨**

	FrontLeftPropeller->ApplyThrust(HoverThrustScaled * FrontScale, DroneMesh, ForceDir_FRBL); // ← 변경됨**
	FrontRightPropeller->ApplyThrust(HoverThrustScaled * FrontScale, DroneMesh, ForceDir_FRBL); // ← 변경됨**
	BackLeftPropeller->ApplyThrust(HoverThrustScaled * BackScale, DroneMesh, ForceDir_FLBR);    // ← 변경됨**
	BackRightPropeller->ApplyThrust(HoverThrustScaled * BackScale, DroneMesh, ForceDir_FLBR);   // ← 변경됨**


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
	float ForwardBias = bForwardInputHeld ? 0.5f : 0.f; 
	float FrontScale = 1.f - ForwardBias;               
	float BackScale = 1.f + ForwardBias;                

	FVector ForwardVector = GetActorForwardVector();    
	FVector UpVector = FVector::UpVector;               
	FVector ForceDir_FLBR = (UpVector + ForwardVector * ForwardBias).GetSafeNormal(); 
	FVector ForceDir_FRBL = UpVector.GetSafeNormal();   

	float Thrust = FMath::Clamp(PerPropellerThrust, 0.f, MaxThrust); 
	FrontLeftPropeller->ApplyThrust(Thrust * FrontScale, DroneMesh, ForceDir_FRBL);  
	FrontRightPropeller->ApplyThrust(Thrust * FrontScale, DroneMesh, ForceDir_FRBL);
	BackLeftPropeller->ApplyThrust(Thrust * BackScale, DroneMesh, ForceDir_FLBR);    
	BackRightPropeller->ApplyThrust(Thrust * BackScale, DroneMesh, ForceDir_FLBR);  

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
	bForwardInputHeld = true;
}

void ADronePawn::MoveForwardReleased(const FInputActionInstance& Instance)
{
	bForwardInputHeld = false;
}


float ADronePawn::ComputePID(float Target, float Current, float DeltaTime)
{
	float Error = Target - Current;
	Integral += Error * DeltaTime;
	Integral = FMath::Clamp(Integral, -1000.f, 1000.f);
	float Derivative = (Error - PreviousError) / DeltaTime;
	PreviousError = Error;
	return Kp * Error + Ki * Integral + Kd * Derivative;
}