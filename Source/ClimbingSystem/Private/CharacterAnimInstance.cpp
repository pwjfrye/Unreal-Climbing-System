
#include "CharacterAnimInstance.h"

#include "ClimbingSystem/ClimbingSystemCharacter.h"
#include "Components/CustomMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ClimbingSystemCharacter = Cast<AClimbingSystemCharacter>(TryGetPawnOwner());
	if (ClimbingSystemCharacter)
	{
		CustomMovementComponent = ClimbingSystemCharacter->GetCustomMovementComponent();
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!ClimbingSystemCharacter || !CustomMovementComponent)
	{
		return;
	}

	UpdateHorizontalSpeed();
	UpdateVerticalVelocity();
	UpdateShouldMove();
	UpdateIsFalling();
	UpdateWasJumping();
	UpdateCurrentJumpCount();
	UpdateIsClimbing();
	UpdateClimbingVelocity();
}

void UCharacterAnimInstance::UpdateHorizontalSpeed()
{
	HorizontalSpeed = UKismetMathLibrary::VSizeXY(ClimbingSystemCharacter->GetVelocity());
}

void UCharacterAnimInstance::UpdateVerticalVelocity()
{
	VerticalVelocity = ClimbingSystemCharacter->GetVelocity().Z;
}

void UCharacterAnimInstance::UpdateShouldMove()
{
	bShouldMove = CustomMovementComponent->GetCurrentAcceleration().SizeSquared() > 0.f
		&& HorizontalSpeed > MinHorizontalSpeedToMove
		&& !bIsFalling;
}

void UCharacterAnimInstance::UpdateIsFalling()
{
	bIsFalling = CustomMovementComponent->IsFalling();
}

void UCharacterAnimInstance::UpdateWasJumping()
{
	bWasJumping = ClimbingSystemCharacter->bWasJumping;
}

void UCharacterAnimInstance::UpdateCurrentJumpCount()
{
	CurrentJumpCount = ClimbingSystemCharacter->JumpCurrentCount;
}

void UCharacterAnimInstance::UpdateIsClimbing()
{
	bIsClimbing = CustomMovementComponent->IsClimbing();
}

void UCharacterAnimInstance::UpdateClimbingVelocity()
{
	ClimbingVelocity = CustomMovementComponent->GetUnrotatedClimbingVelocity();
}
