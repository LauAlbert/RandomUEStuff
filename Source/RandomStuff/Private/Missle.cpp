// Fill out your copyright notice in the Description page of Project Settings.

#include "Missle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Component/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "RandomStuffCharacter.h"

// Sets default values
AMissle::AMissle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetSimulatePhysics(false);


	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bIgnoreOwningActor = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->Radius = 300.0f;
	
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthDamage.AddDynamic(this, &AMissle::HealthDamage);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	//ProjectileMovementComp->bShouldBounce = true;
	//ProjectileMovementComp->InitialSpeed = 1500.0f;
	//ProjectileMovementComp->MaxSpeed = 1500.0f;
	//ProjectileMovementComp->bRotationFollowsVelocity = true;
	//ProjectileMovementComp->bIsHomingProjectile = true;
	//ProjectileMovementComp->HomingAccelerationMagnitude = 2000.0f;

	


	bExploded = false;
}

// Called when the game starts or when spawned
void AMissle::BeginPlay()
{
	Super::BeginPlay();

	// If Target is set make that the homing target
	ARandomStuffCharacter* player = Cast<ARandomStuffCharacter>(TargetActor);
	if (player != NULL)
	{
		ProjectileMovementComp->bShouldBounce = true;
		ProjectileMovementComp->InitialSpeed = 1500.0f;
		ProjectileMovementComp->MaxSpeed = 1500.0f;
		ProjectileMovementComp->bRotationFollowsVelocity = true;
		ProjectileMovementComp->bIsHomingProjectile = true;
		ProjectileMovementComp->HomingAccelerationMagnitude = 2000.0f;
		
		ProjectileMovementComp->HomingTargetComponent = player->GetRootComponent();

		ProjectileMovementComp->SetVelocityInLocalSpace(GetActorForwardVector()*ProjectileMovementComp->InitialSpeed);

	}
	else {
		MeshComp->SetSimulatePhysics(true);
	}
	
}

// Called every frame
void AMissle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMissle::HealthDamage(UHealthComponent* HealthComponent, float Health, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (bExploded)
	{
		return;
	}
	if (!bExploded && Health <= 0.0f)
	{
		bExploded = true;

		TArray<AActor*> ActorToIgnore;
		ActorToIgnore.Add(this);

		UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), 80.0f, 20.0f, GetActorLocation(), 100.0f, 300.0f, 0.2, DummyDamageType, ActorToIgnore);

		RadialForceComp->FireImpulse();

	}
}


void AMissle::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	if (!bExploded && !MeshComp->IsSimulatingPhysics())
	{
		UE_LOG(LogTemp, Warning, TEXT("hit obj"));
		bExploded = true;

		if (ExplosionEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
		}

		TArray<AActor*> ActorToIgnore;
		ActorToIgnore.Add(this);

		UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), 80.0f, 20.0f, GetActorLocation(), 100.0f, 300.0f, 0.2, DummyDamageType, ActorToIgnore);

		RadialForceComp->FireImpulse();

		Destroy();
	}
}