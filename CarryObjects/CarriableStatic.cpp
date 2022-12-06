// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/CarriableStatic.h"

#include "DrawDebugHelpers.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Player/PlayerCharacter.h"

ACarriableStatic::ACarriableStatic()
{
	PerceptionStimuli = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimuli Source"));
}

void ACarriableStatic::BeginPlay()
{
	Super::BeginPlay();

	// Assign defaults on begin play
	PerceptionStimuli->bAutoRegister = true;
	PerceptionStimuli->RegisterForSense(UAISense_Hearing::StaticClass());

	Mesh->SetMassOverrideInKg(NAME_None, Mass);
	Mesh->SetNotifyRigidBodyCollision(true);
	ReleasePosition = FVector::ZeroVector;

	// Bind delegate to report noise events when this object hits something
	Mesh->OnComponentHit.AddDynamic(this, &ACarriableStatic::MakeNoiseWhenDropped);

	// Cache the default response to the pawn
	InitialResponse = Mesh->GetCollisionResponseToChannel(ECC_Pawn);
}

// Called when a character begins an interaction
void ACarriableStatic::InteractStart_Implementation(const ACharacter* Interactor)
{
	// Cache interactor reference
	CurrentInteractor = Cast<APlayerCharacter>(const_cast<ACharacter*>(Interactor));

	// Reset any physics affecting the object
	Mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	Mesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

	// Disable gravity and set pawn collision to ignore
	Mesh->SetEnableGravity(false);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// Bind delegate to drop object on hit, only while being carried
	Mesh->OnComponentHit.AddDynamic(this, &ACarriableStatic::OnCollision);

	// Set bool, used to determine whether to make noise when dropped
	bIsCarried = true;

	// Broadcast delegate for pick up event
	if(OnItemPickedUp.IsBound()) OnItemPickedUp.Broadcast();
}

// Called when a character ends an interaction
void ACarriableStatic::InteractEnd_Implementation(const ACharacter* Interactor)
{
	// Re-enable gravity and reset collision response to pawn to default
	Mesh->SetEnableGravity(true);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, InitialResponse);

	// Remove delegate to drop object
	Mesh->OnComponentHit.RemoveDynamic(this, &ACarriableStatic::OnCollision);

	// Cache position when released, used to measure metrics
	ReleasePosition = GetActorLocation();
}

void ACarriableStatic::OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// On collision while being carried, force interactor to drop the object
	CurrentInteractor->TryReleaseInteractable();
}

// Report noise when it pitches on the ground
void ACarriableStatic::MakeNoiseWhenDropped(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// If it hasn't been picked up, don't report a noise event
	if(!bIsCarried) return;

	// Debug to measure throwing metrics, draws a line and logs the distance to the screen
	if(bMeasureThrowDistance)
	{
		DrawDebugLine(GetWorld(), ReleasePosition, GetActorLocation(), FColor::Purple, true, -1.0f, 0, 5.0f);
		const float Distance = FVector::Distance(GetActorLocation(), ReleasePosition);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.0f, FColor::Purple, FString::SanitizeFloat(Distance));
	}

	// Broadcast delegate for on drop event
	if(OnItemDropped.IsBound()) OnItemDropped.Broadcast();

	// Only want to report event on first pitch so reset this bool
	bIsCarried = false;

	// Report noise event for Perception system
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), Hit.Location, 1.0f, this);
}

// Expose mass param
float ACarriableStatic::GetMass() const
{
	return Mass;
}

// Fire delegate when thrown
void ACarriableStatic::BroadcastThrown()
{
	if(OnItemThrown.IsBound()) OnItemThrown.Broadcast();
}
