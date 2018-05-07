// Copyright Tommi Kujanen 2018.
//Opens and closes (timed) the door via rotation

#include "OpenDoor.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	PressureTriggerer = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}

void UOpenDoor::OpenDoor() {

	//calculate new rotation
	FRotator NewRotation = FRotator(0.0f, (Owner->GetActorRotation().Yaw + OpenAngle), 0.0f);

	Owner->SetActorRotation(NewRotation);

	IsOpen = true;
}

void UOpenDoor::CloseDoor() {

	//calculate new rotation
	FRotator NewRotation = FRotator(0.0f, (Owner->GetActorRotation().Yaw + -OpenAngle), 0.0f);

	Owner->SetActorRotation(NewRotation);

	IsOpen = false;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if player is on pressure plate and the door is closed, open the door
	if (PressurePlate->IsOverlappingActor(PressureTriggerer) && !IsOpen) {
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	
	//close the door after a delay
	if ((GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) && IsOpen) {
		CloseDoor();
	}
	
}

