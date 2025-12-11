// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameManager.generated.h"

UCLASS()
class PROJECTDARKMODE_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

	float gameTime;
	UPROPERTY(BlueprintReadWrite)
	int gemsTotal;
	UPROPERTY(BlueprintReadWrite)
	int gemsCollected;
	UPROPERTY(BlueprintReadWrite)
	int enemiesTotal;
	UPROPERTY(BlueprintReadWrite)
	int enemiesKilled;
	UPROPERTY(BlueprintReadWrite)
	int smashablesTotal;
	UPROPERTY(BlueprintReadWrite)
	int smashablesBroke;

	void EnemyKilled();
	void GemCollected();
	void smashableSmashed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* hud;

	UPROPERTY(BlueprintReadWrite)
	UTextBlock* smashablesText;
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* gemsText;
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* killsText;
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* timerText;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetUI();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
