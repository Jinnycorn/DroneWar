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
	/*void HoverDown();
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// 드론 메시 및 물리
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DroneMesh;

	// 프로펠러들
	UPROPERTY()
	UPropellerComponent* FrontLeftPropeller;
	UPROPERTY()
	UPropellerComponent* FrontRightPropeller;
	UPROPERTY()
	UPropellerComponent* BackLeftPropeller;
	UPROPERTY()
	UPropellerComponent* BackRightPropeller;

	// 추력 계산
	float HoverThrust = 0.f;
	float CurrentThrust = 0.f;
	float TargetThrust = 0.f;

	UPROPERTY(EditAnywhere)
	float InterpSpeed = 3.0f;

	UPROPERTY(EditAnywhere)
	float MaxThrust = 2000.0f;

	UPROPERTY(EditAnywhere)
	float MaxAltitude = 300.0f;

	void ApplyAllThrust();

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
