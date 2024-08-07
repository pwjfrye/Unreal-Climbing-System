// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CustomMovementComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

#include "ClimbingSystem/DebugHelper.h"

void UCustomMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCustomMovementComponent::ToggleClimbing()
{
	if (IsClimbing())
	{
		// Exit climb state
		Debug::Print(TEXT("END CLIMB"));
	}
	else
	{
		if (CanStartClimbing())
		{
			// Enter climb state
			Debug::Print(TEXT("START CLIMB"));
		}
		else
		{
			Debug::Print(TEXT("CANNOT CLIMB"));
		}
	}
}

bool UCustomMovementComponent::CanStartClimbing()
{
	if (IsFalling())
	{
		return false;
	}

	if (!DetectClimbableSurfaces())
	{
		return false;
	}

	if (!DetectEyeHeightSurface())
	{
		return false;
	}

	return true;
}

bool UCustomMovementComponent::IsClimbing() const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == static_cast<uint8>(ECustomMovementMode::MOVE_Climb);
}

TArray<FHitResult> UCustomMovementComponent::DoClimbTrace(const FVector& Start, const FVector& End, const EDrawDebugTrace::Type DebugTraceType) const
{
	const auto Scale = UpdatedComponent->GetComponentScale().GetMax();

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
		DebugTraceType,
		OutHitResults,
		false
		);

	return OutHitResults;
}

FHitResult UCustomMovementComponent::DoEyeHeightTrace(const FVector& Start, const FVector& End, const EDrawDebugTrace::Type DebugTraceType) const
{
	FHitResult OutHitResult;
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ClimbableSurfaceTraceTypes,
		false,
		TArray<AActor*>(),
		DebugTraceType,
		OutHitResult,
		false
		);

	return OutHitResult;
}

bool UCustomMovementComponent::DetectClimbableSurfaces()
{
	const FVector Start = UpdatedComponent->GetComponentTransform().TransformPosition(ClimbCapsuleTraceOffset);
	const FVector End = UpdatedComponent->GetComponentTransform().TransformPosition(ClimbCapsuleTraceOffset + FVector::ForwardVector * ClimbCapsuleTraceDistance);

	ClimbableSurfacesTraced = DoClimbTrace(Start, End, EDrawDebugTrace::Persistent);
	return !ClimbableSurfacesTraced.IsEmpty();
}

bool UCustomMovementComponent::DetectEyeHeightSurface()
{
	const FVector Start = UpdatedComponent->GetComponentTransform().TransformPosition(EyeHeightTraceOffset);
	const FVector End = UpdatedComponent->GetComponentTransform().TransformPosition(EyeHeightTraceOffset + FVector::ForwardVector * EyeHeightTraceDistance);

	const auto HitResult = DoEyeHeightTrace(Start, End, EDrawDebugTrace::Persistent);
	return HitResult.bBlockingHit > 0;
}
