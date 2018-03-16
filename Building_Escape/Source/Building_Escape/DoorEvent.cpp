// Copyright Dorian Signargout

#include "DoorEvent.h"

#define OUT

// Sets default values for this component's properties
UDoorEvent::UDoorEvent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorEvent::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = GetOwner();
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Uh oh, no owner"));
	}
	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *GetOwner()->GetName());
	}
}

// Called every frame
void UDoorEvent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UDoorEvent::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return 0.f; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through actors to sum their mass
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName())
	}

	return TotalMass;
}