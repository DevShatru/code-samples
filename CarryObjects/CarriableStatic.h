// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CarriableInterface.h"
#include "Props/InteractableStatic.h"
#include "CarriableStatic.generated.h"

/**
 * 
 */
UCLASS()
class TENMINUTESTOBREATHE_API ACarriableStatic : public AInteractableStatic, public ICarriableInterface
{
	GENERATED_BODY()
public:
	ACarriableStatic();

	// Called when a character begins an interaction
	virtual void InteractStart_Implementation(const ACharacter* Interactor) override;
	// Called when a character ends an interaction
	virtual void InteractEnd_Implementation(const ACharacter* Interactor) override;

	// Report noise when it pitches on the ground
	UFUNCTION()
	virtual void MakeNoiseWhenDropped(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void BeginPlay() override;

	// Expose mass param
	virtual float GetMass() const override;

	// Fire delegate when thrown
	virtual void BroadcastThrown() override;

	UPROPERTY(BlueprintAssignable)
	FItemThrown OnItemThrown;
	
	UPROPERTY(BlueprintAssignable)
	FItemDropped OnItemDropped;
	
	UPROPERTY(BlueprintAssignable)
	FItemPickUp OnItemPickedUp;

protected:
	// Debug option to measure throwing metrics
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMeasureThrowDistance = false;

	// Whether the object has been picked up and not yet dropped
	UPROPERTY(BlueprintReadOnly)
	bool bIsCarried = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Properties)
	float Mass = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Properties)
	float SoundRange = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAIPerceptionStimuliSourceComponent* PerceptionStimuli;

private:
	// Variables to cache reference to current interactor, position when last dropped, and default collision response to pawn
	APlayerCharacter* CurrentInteractor;
	FVector ReleasePosition;
	ECollisionResponse InitialResponse;

	UFUNCTION()
	void OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};