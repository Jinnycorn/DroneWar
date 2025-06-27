
#include "DronePawn.h"

// Sets default values
ADronePawn::ADronePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �޽� ���� �� ��Ʈ ������Ʈ ����
	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
	RootComponent = DroneMesh;

	DroneMesh->SetSimulatePhysics(true);   // ���� �ùķ��̼�
	DroneMesh->SetEnableGravity(true);     // �߷� ����
}

// Called when the game starts or when spawned
void ADronePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/*PlayerInputComponent->BindAction("HoverUp", IE_Pressed, this, &ADronePawn::HoverUp);
	PlayerInputComponent->BindAction("HoverDown", IE_Pressed, this, &ADronePawn::HoverDown);
	PlayerInputComponent->BindAction("Forward", IE_Pressed, this, &ADronePawn::MoveForward);
	PlayerInputComponent->BindAction("Backward", IE_Pressed, this, &ADronePawn::MoveBackward);
	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &ADronePawn::MoveLeft);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &ADronePawn::MoveRight);*/


}

