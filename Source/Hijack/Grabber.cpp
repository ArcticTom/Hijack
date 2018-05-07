// Copyright Tommi Kujanen 2018.
//Grabs hold of an object close enough that the player is looking at

#include "Grabber.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
		//handle is found
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s is missing PhysicsHandle."), *GetOwner()->GetName());
	}

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Press", IE_Pressed, this, &UGrabber::Press);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s is missing InputComponent."), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	//get player location and rotation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

	//draw a line to visualize player reach
	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.0f, 0.0f, 10.0f);

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FHitResult TraceHit;
	GetWorld()->LineTraceSingleByObjectType(OUT TraceHit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);

	AActor* ActorHit = TraceHit.GetActor();

	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("%s is hit."), *ActorHit->GetName());
	}

}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Thing grabbed."));
}

void UGrabber::Press() {
	UE_LOG(LogTemp, Warning, TEXT("Thing pressed."));
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Thing released."));
}

