// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CustomMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void UCustomMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TraceClimbableSurfaces();
}

TArray<FHitResult> UCustomMovementComponent::DoCapsuleTraceMultiByObject(const FVector& Start, const FVector& End, bool bShowDebugShape)
{
	TArray<FHitResult> OutHitResults;
	UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		this,
		Start,
		End,
		ClimbCapsuleTraceRadius,
		ClimbCapsuleTraceHalfHeight,
		ClimbableSurfaceTraceTypes,
		false,
		TArray<AActor*>(),
		bShowDebugShape ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		OutHitResults,
		false
		);

	return OutHitResults;
}

void UCustomMovementComponent::TraceClimbableSurfaces()
{
	const FVector Start = UpdatedComponent->GetComponentTransform().TransformPosition(ClimbCapsuleTraceOffset);
	const FVector End = Start + UpdatedComponent->GetForwardVector() * ClimbCapsuleTraceDistance;
	DoCapsuleTraceMultiByObject(Start, End, true);
}
