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

void UPropellerComponent::ApplyThrust(float Thrust)
{
	if (!PropellerMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("[PropellerComponent] PropellerMesh is nullptr"));
		return;
	}

	USceneComponent* AttachTarget = GetAttachParent();
	if (!AttachTarget)
	{
		UE_LOG(LogTemp, Error, TEXT("[PropellerComponent] AttachParent is nullptr"));
		return;
	}

	UStaticMeshComponent* Parent = Cast<UStaticMeshComponent>(AttachTarget);
	if (!Parent)
	{
		UE_LOG(LogTemp, Error, TEXT("[PropellerComponent] AttachParent is not UStaticMeshComponent"));
		return;
	}

	if (!Parent->IsSimulatingPhysics())
	{
		UE_LOG(LogTemp, Warning, TEXT("[PropellerComponent] Parent is not simulating physics"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("UPropellerComponent::ApplyThrust"));
}





