// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BountyDashGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BOUNTYDASH_API ABountyDashGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY()
	float gameSpeed;

	UPROPERTY()
	int32 gameLevel;

public:
	ABountyDashGameModeBase();

	void CharScoreUp(unsigned int charScore);

	UFUNCTION()
	float GetInvGameSpeed();

	UFUNCTION()
	float GetGameSpeed();

	UFUNCTION()
	int32 GetGameLevel();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 numCoinsForSpeedIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float gameSpeedIncrease;
};
