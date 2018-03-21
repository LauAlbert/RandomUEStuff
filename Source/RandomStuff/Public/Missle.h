// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missle.generated.h"

UCLASS()
class RANDOMSTUFF_API AMissle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Missle)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = Missle)
	class URadialForceComponent* RadialForceComp;

	UPROPERTY(VisibleAnywhere, Category = Missle)
	class UHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, Category = Missle)
	class UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	class UParticleSystem* ExplosionEffect;

	UPROPERTY(EditInstanceOnly, Category = "DamageType")
	AActor* TargetActor;

	UPROPERTY(EditDefaultsOnly, Category = DamageType)
	TSubclassOf<UDamageType> DummyDamageType;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HealthDamage(UHealthComponent* HealthComponent, float Health, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);


	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

private:
	bool bExploded;
	
};
