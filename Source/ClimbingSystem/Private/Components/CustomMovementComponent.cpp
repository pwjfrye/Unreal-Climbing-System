// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CustomMovementComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

#include "ClimbingSystem/DebugHelper.h"
#include "Components/CapsuleComponent.h"

void UCustomMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCustomMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	if (IsClimbing())
	{
		// The climbed surface will control the rotation of the character
		bOrientRotationToMovement = false;

		// The character takes up less space in the climb animation
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(48.f); // TODO Don't hard-code
	}

	// Restore settings when leaving climbing
	if (IsClimbingMovementMode(PreviousMovementMode, PreviousCustomMode))
	{
		bOrientRotationToMovement = true;
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(96.f); // TODO Don't hard-code

		// Zero out velocity from climbing
		StopMovementImmediately();
	}

	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
}

void UCustomMovementComponent::ToggleClimbing()
{
	if (IsClimbing())
	{
		// Exit climb state
		StopClimbing();
	}
	else
	{
		if (CanStartClimbing())
		{
			// Enter climb state
			StartClimbing();
		}
		else
		{
			Debug::Print(TEXT("CANNOT CLIMB"));
		}
	}
}

bool UCustomMovementComponent::CanStartClimbing()
{
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
	return IsClimbingMovementMode(MovementMode, CustomMovementMode);
}

bool UCustomMovementComponent::IsClimbingMovementMode(EMovementMode InMovementMode, uint8 InCustomMode)
{
	return InMovementMode == MOVE_Custom && InCustomMode == static_cast<uint8>(ECustomMovementMode::MOVE_Climb);
}

void UCustomMovementComponent::StartClimbing()
{
	SetMovementMode(MOVE_Custom, static_cast<uint8>(ECustomMovementMode::MOVE_Climb));
}

void UCustomMovementComponent::StopClimbing()
{
	SetMovementMode(MOVE_Falling);
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
