// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CustomMovementComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"


void UCustomMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TraceClimbableSurfaces();
	TraceEyeHeightSurface();
}

TArray<FHitResult> UCustomMovementComponent::DoClimbTrace(const FVector& Start, const FVector& End, bool bShowDebugShape) const
{
	auto Scale = UpdatedComponent->GetComponentScale().GetMax();

	TArray<FHitResult> OutHitResults;
	UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		this,
		Start,
		End,
		ClimbCapsuleTraceRadius * Scale,
		ClimbCapsuleTraceHalfHeight * Scale,
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
	const FVector End = UpdatedComponent->GetComponentTransform().TransformPosition(ClimbCapsuleTraceOffset + FVector::ForwardVector * ClimbCapsuleTraceDistance);

	DoClimbTrace(Start, End, true);
}

void UCustomMovementComponent::TraceEyeHeightSurface()
{
	const FVector Start = UpdatedComponent->GetComponentTransform().TransformPosition(EyeHeightTraceOffset);
	const FVector End = UpdatedComponent->GetComponentTransform().TransformPosition(EyeHeightTraceOffset + FVector::ForwardVector * EyeHeightTraceDistance);

	DoEyeHeightTrace(Start, End, true);
}
