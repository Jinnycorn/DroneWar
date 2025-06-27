#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PropellerComponent.h"
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

	UPROPERTY(VisibleAnywhere)
	UPropellerComponent* PropellerFL;
	UPROPERTY(VisibleAnywhere)
	UPropellerComponent* PropellerFR;
	UPROPERTY(VisibleAnywhere)
	UPropellerComponent* PropellerBL;
	UPROPERTY(VisibleAnywhere)
	UPropellerComponent* PropellerBR;

	void HoverUp();
	void HoverDown();
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();


};
