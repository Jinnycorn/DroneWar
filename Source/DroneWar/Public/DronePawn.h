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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// 드론 메시 및 물리
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DroneMesh;

	// InputActions
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DroneMappingContext;

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

	UFUNCTION()
	void HoverUp(const FInputActionInstance& Instance);
	/*void HoverDown();
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();*/


};
