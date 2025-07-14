// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PropellerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRONEWAR_API UPropellerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPropellerComponent();


    void SetThrust(float NewThrust);
	UFUNCTION(BlueprintCallable, Category = "Thrust")
	void ApplyThrust(float Thrust, UStaticMeshComponent* TargetMesh);



protected:
    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    UStaticMeshComponent* PropellerMesh;

		
};
