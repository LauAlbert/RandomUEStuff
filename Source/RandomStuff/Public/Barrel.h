// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Barrel.generated.h"

UCLASS()
class RANDOMSTUFF_API ABarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Barrel")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Barrel")
	class URadialForceComponent* RadialForceComp;

	UPROPERTY(VisibleAnywhere, Category = "Barrel")
	class UHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UMaterialInterface* ExplodedMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "DamageType")
	TSubclassOf<UDamageType> DummyDamageType;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HealthChange(UHealthComponent* HealthComponent, float Health, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:
	bool bExploded;
};
