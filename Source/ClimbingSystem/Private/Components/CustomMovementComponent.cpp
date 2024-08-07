// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CustomMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void UCustomMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TraceClimbableSurfaces();
	TraceEyeHeightSurface();
}

TArray<FHitResult> UCustomMovementComponent::DoClimbTrace(const FVector& Start, const FVector& End, bool bShowDebugShape) const
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

FHitResult UCustomMovementComponent::DoEyeHeightTrace(const FVector& Start, const FVector& End, bool bShowDebugShape) const
{
	FHitResult OutHitResult;
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ClimbableSurfaceTraceTypes,
		false,
		TArray<AActor*>(),
		bShowDebugShape ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		OutHitResult,
		false
		);

	return OutHitResult;
}

void UCustomMovementComponent::TraceClimbableSurfaces()
{
	const FVector Start = UpdatedComponent->GetComponentTransform().TransformPosition(ClimbCapsuleTraceOffset);
	const FVector End = Start + UpdatedComponent->GetForwardVector() * ClimbCapsuleTraceDistance;

	DoClimbTrace(Start, End, true);
}

void UCustomMovementComponent::TraceEyeHeightSurface()
{
	const FVector Start = UpdatedComponent->GetComponentTransform().TransformPosition(EyeHeightTraceOffset);
	const FVector End = Start + UpdatedComponent->GetForwardVector() * EyeHeightTraceDistance;

	DoEyeHeightTrace(Start, End, true);
}
