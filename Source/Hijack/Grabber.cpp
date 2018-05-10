// Copyright Tommi Kujanen 2018.
// Grabs hold of an object close enough that the player is looking at

#include "Grabber.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();

	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) {
		return;
	}
	if (PhysicsHandle->GrabbedComponent) {
		//move grabbed object
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}

}

//grab physics object when grab key is hit
void UGrabber::Grab() {

	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit) {
		if (!PhysicsHandle) {
			return;
		}
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation());
	}

}

void UGrabber::Press() {
	UE_LOG(LogTemp, Warning, TEXT("Thing pressed."));
}

//release grabbed object when key is hit
void UGrabber::Release() {
	if (!PhysicsHandle) {
		return;
	}
	PhysicsHandle->ReleaseComponent();
}

//check physics component is found
void UGrabber::FindPhysicsHandleComponent() {

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s is missing PhysicsHandle."), *GetOwner()->GetName());
	}
}

//setup input handling
void UGrabber::SetupInputComponent() {

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {

		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Press", IE_Pressed, this, &UGrabber::Press);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s is missing InputComponent."), *GetOwner()->GetName());
	}
}

//check if physics object hit between the player and linetraceend
const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FHitResult TraceHit;
	GetWorld()->LineTraceSingleByObjectType(OUT TraceHit, GetPlayerLocation(), GetLineTraceEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);

	return TraceHit;
}

FVector UGrabber::GetLineTraceEnd() {

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	//get player location and rotation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}

FVector UGrabber::GetPlayerLocation() {

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	//get player location and rotation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}

