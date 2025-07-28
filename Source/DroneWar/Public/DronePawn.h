#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PropellerComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

#include "DronePawn.generated.h"


UCLASS()
class DRONEWAR_API ADronePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADronePawn();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//입력 함수
	UFUNCTION()
	void HoverUp(const FInputActionInstance& Instance);

	UFUNCTION()
	void HoverUpReleased(const FInputActionInstance& Instance);
	//void HoverDown();

	UFUNCTION()
	void MoveForward(const FInputActionInstance& Instance);

	UFUNCTION()
	void MoveForwardReleased(const FInputActionInstance& Instance);

	/*void MoveBackward();
	void MoveLeft();
	void MoveRight();*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 각 프로펠러별 힘 스케일
	UPROPERTY(EditAnywhere, Category = "Thrust Scaling")
	float FrontLeftScale = 1.f;

	UPROPERTY(EditAnywhere, Category = "Thrust Scaling")
	float FrontRightScale = 1.f;

	UPROPERTY(EditAnywhere, Category = "Thrust Scaling")
	float BackLeftScale = 1.f;

	UPROPERTY(EditAnywhere, Category = "Thrust Scaling")
	float BackRightScale = 1.f;



protected:
	// 드론 메시 및 물리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DroneMesh;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Propeller", meta = (AllowPrivateAccess = "true"))
	UPropellerComponent* FrontLeftPropeller;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Propeller", meta = (AllowPrivateAccess = "true"))
	UPropellerComponent* FrontRightPropeller;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Propeller", meta = (AllowPrivateAccess = "true"))
	UPropellerComponent* BackLeftPropeller;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Propeller", meta = (AllowPrivateAccess = "true"))
	UPropellerComponent* BackRightPropeller;

	// PID 제어 변수 추가
	UPROPERTY(EditAnywhere, Category = "PID") float Kp = 9.8f; 
	UPROPERTY(EditAnywhere, Category = "PID") float Ki = 0.98f;
	UPROPERTY(EditAnywhere, Category = "PID") float Kd = 3.5f; 

	UPROPERTY(EditAnywhere, Category = "PID") float TargetAltitude = 150.f; 

	float PreviousError = 0.f; 
	float Integral = 0.f; 

	float ComputePID(float Target, float Current, float DeltaTime); 

	// 추력 계산
	float GravityZ = -980.f;
	float TotalMass = 1.f;
	float HoverThrust = 0.f;
	float PerPropellerThrust = 0.f;
	float CurrentAltitude = 0.f;
	float CurrentHoverPower = 0.f;

	bool bHoverInputHeld = false;
	bool bIsHoveringAllowed = true;

	UPROPERTY(EditAnywhere)
	float InterpSpeed = 10.0f;

	UPROPERTY(EditAnywhere)
	float MaxThrust = 5000.0f;

	UPROPERTY(EditAnywhere)
	float MaxAltitude = 300.0f;

	void ApplyAllThrust();
	void OnHoverUp();

	// InputActions
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* IMC_DroneControls;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_HoverUp;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_HoverDown;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_MoveForward;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_MoveBackward;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_MoveLeft;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_MoveRight;



};
