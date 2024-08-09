// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CustomMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	MOVE_Climb UMETA(DisplayName = "Climb Mode")
};

UCLASS()
class CLIMBINGSYSTEM_API UCustomMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxAcceleration() const override;

	void ToggleClimbing();
	bool IsClimbing() const;
	FVector GetForwardClimbingDirection() const { return FVector::CrossProduct(-CurrentClimbableSurfaceNormal, UpdatedComponent->GetRightVector()); }
	FVector GetRightwardClimbingDirection() const { return FVector::CrossProduct(-CurrentClimbableSurfaceNormal, -UpdatedComponent->GetUpVector()); }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> ClimbableSurfaceTraceTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	float ClimbCapsuleTraceRadius = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	float ClimbCapsuleTraceHalfHeight = 72.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	FVector ClimbCapsuleTraceOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	float ClimbCapsuleTraceDistance = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	FVector EyeHeightTraceOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	float EyeHeightTraceDistance = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	float ClimbingMaxBrakingDeceleration = 400.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	float ClimbingMaxSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	float ClimbingMaxAcceleration = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
	float ClimbingRotationInterpSpeed = 5.f;

	TArray<FHitResult> ClimbableSurfacesTraced;
	FVector CurrentClimbableSurfaceLocation;
	FVector CurrentClimbableSurfaceNormal;

	void PhysClimb(float deltaTime, int32 Iterations);

	bool CanStartClimbing();
	static bool IsClimbingMovementMode(EMovementMode InMovementMode, uint8 InCustomMode);

	void StartClimbing();
	void StopClimbing();

	void ProcessClimbableSurfaceInfo();
	TArray<FHitResult> DoClimbTrace(const FVector& Start, const FVector& End, const EDrawDebugTrace::Type DebugTraceType) const;
	FHitResult DoEyeHeightTrace(const FVector& Start, const FVector& End, const EDrawDebugTrace::Type DebugTraceType) const;

	bool DetectClimbableSurfaces();
	bool DetectEyeHeightSurface();

	FQuat GetClimbRotation(float DeltaTime) const;
	void SnapMovementToClimbableSurfaces(float DeltaTime);
};
