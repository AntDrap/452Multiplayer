#include "OwnershipTestActor.h"
#include "OwnershipRoles.h"
#include "OwnershipRolesCharacter.h"
#include "Kismet/GameplayStatics.h"

AOwnershipTestActor::AOwnershipTestActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AOwnershipTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugSphere(GetWorld(), GetActorLocation(), OwnershipRadius, 32, FColor::Yellow);

	if (HasAuthority())
	{
		AActor* NextOwner = nullptr;

		float MinDistance = OwnershipRadius;

		TArray<AActor*> Actors;

		UGameplayStatics::GetAllActorsOfClass(this, AOwnershipRolesCharacter::StaticClass(), Actors);

		for (AActor* Actor : Actors)
		{
			const float Distance = GetDistanceTo(Actor);

			if (Distance <= MinDistance)
			{
				MinDistance = Distance;
				NextOwner = Actor;
			}
		}

		if (GetOwner() != NextOwner)
		{
			SetOwner(NextOwner);
		}
	}

	const FString LocalRoleString = ROLE_TO_STRING(GetLocalRole());
	const FString RemoteRoleString = ROLE_TO_STRING(GetRemoteRole());
	const FString OwnerString = GetOwner() != nullptr ? GetOwner()->GetName() : TEXT("No Owner");
	const FString ConnectionString = GetNetConnection() != nullptr ? TEXT("Valid Connection") : TEXT("Invalid Connection");

	const FString Values = FString::Printf(TEXT("Anthony Drapeau\nLocalRole = %s\nRemoteRole = %s\nOwner = %s\nConnection = %s"), *LocalRoleString, *RemoteRoleString, *OwnerString, *ConnectionString);

	DrawDebugString(GetWorld(), GetActorLocation(), Values, nullptr, FColor::White, 0.0f, true);
}