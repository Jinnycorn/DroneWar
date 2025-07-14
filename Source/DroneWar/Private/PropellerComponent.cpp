// Fill out your copyright notice in the Description page of Project Settings.


#include "PropellerComponent.h"

// Sets default values for this component's properties
UPropellerComponent::UPropellerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// PropellerMesh 생성 및 부착
	PropellerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PropellerMesh"));
	check(PropellerMesh); // 죽더라도 확실하게 실패 포인트 찍음

	PropellerMesh->SetupAttachment(this);

	// 충돌, 물리 해제 (회전용 비주얼일 뿐)
	PropellerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PropellerMesh->SetSimulatePhysics(false);
}

void UPropellerComponent::ApplyThrust(float Thrust, UStaticMeshComponent* TargetMesh)
{
	if (!TargetMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("[PropellerComponent] TargetMesh is NULL"));
		return;
	}

	if (!TargetMesh->IsSimulatingPhysics())
	{
		UE_LOG(LogTemp, Warning, TEXT("[PropellerComponent] TargetMesh is not simulating physics"));
		return;
	}

	FVector Force = FVector(0.f, 0.f, Thrust);
	FVector Location = GetComponentLocation();

	TargetMesh->AddForceAtLocation(Force, Location);
}





