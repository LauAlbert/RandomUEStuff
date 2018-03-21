// Fill out your copyright notice in the Description page of Project Settings.

#include "Barrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Component/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABarrel::ABarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bIgnoreOwningActor = false;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->Radius = 300.0f;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthDamage.AddDynamic(this, &ABarrel::HealthChange);

	bExploded = false;

}

// Called when the game starts or when spawned
void ABarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UGameplayStatics::ApplyDamage(this, 10.0f, GetInstigatorController(), this, DummyDamageType);
}

void ABarrel::HealthChange(UHealthComponent* HealthComponent, float Health, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Warning, TEXT("hurt"));
	if (bExploded || IsPendingKill())
	{
		return;
	}
	else if (!bExploded && Health <= 0.0f)
	{
		bExploded = true;

		if (ExplosionEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
		}
		if (ExplodedMaterial)
		{
			MeshComp->SetMaterial(0, ExplodedMaterial);
		}

		RadialForceComp->FireImpulse();

		TArray<AActor*> ActorToIgnore;
		ActorToIgnore.Add(this);

		UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), 80.0f, 20.0f, GetActorLocation(), 100.0f, 300.0f, 0.2, DummyDamageType, ActorToIgnore);

		SetLifeSpan(5);

	}
}