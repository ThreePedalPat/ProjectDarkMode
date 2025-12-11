// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGameManager::SetUI()
{
	if (hud)
	{
		if (!hud->IsInViewport())
		{
			hud->AddToViewport();
		}
		if (!gemsText)
		{
			gemsText = Cast<UTextBlock>(hud->GetWidgetFromName(TEXT("Gems")));
		}
		
		if (gemsText)
		{
			gemsText->SetText(FText::FromString("Gems: " + FString::FromInt(gemsCollected)));
		}

		if (!killsText)
		{
			killsText = Cast<UTextBlock>(hud->GetWidgetFromName(TEXT("Kills")));
		}
		
		if (killsText)
		{
			killsText->SetText(FText::FromString("Kills: " + FString::FromInt(enemiesKilled)));
		}

		if (!smashablesText)
		{
			smashablesText = Cast<UTextBlock>(hud->GetWidgetFromName(TEXT("Smashables")));
		}
		
		if (smashablesText)
		{
			smashablesText->SetText(FText::FromString("Smashables: " + FString::FromInt(smashablesBroke)));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "No hud widget assigned in Game Manager");
	}
}

void AGameManager::EnemyKilled()
{
	enemiesKilled++;
	SetUI();
}

void AGameManager::GemCollected()
{
	gemsCollected++;
	SetUI();
}

void AGameManager::smashableSmashed()
{
	smashablesBroke++;
	SetUI();
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	gameTime += DeltaTime;
	if (hud->IsInViewport())
	{
		if (!timerText)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, "Getting timer!");
			timerText = Cast<UTextBlock>(hud->GetWidgetFromName("Timer"));
		}

		if (timerText)
		{
			timerText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), gameTime)));
		}
	}
}

