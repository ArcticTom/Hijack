// Copyright Tommi Kujanen 2018.
//Opens and closes (timed) the door via rotation

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s is missing pressure plate."), *GetOwner()->GetName());
	}
	
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if player is on pressure plate and the door is closed, open the door
	if (GetTotalMassOnPressureTriggerer() > TriggerMass) {
		OnOpen.Broadcast();
	} else {
		OnClose.Broadcast();
	}
	
}

float UOpenDoor::GetTotalMassOnPressureTriggerer() {

	float TotalMass = 0.0f;
	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) {
		return TotalMass;
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s of weight on pressureplate."), *Actor->GetName());
	}

	return TotalMass;
}

