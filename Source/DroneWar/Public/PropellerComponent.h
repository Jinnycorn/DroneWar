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
    void ApplyThrust(UPrimitiveComponent* ParentBody);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    UStaticMeshComponent* PropellerMesh;

    UPROPERTY(EditAnywhere, Category = "Physics")
    float ThrustPower = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Physics")
    float MaxThrust = 200.0f;

		
};
