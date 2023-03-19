// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomFileHelper.generated.h"

UCLASS()
class COMBATGAME_API ACustomFileHelper : public AActor
{
	GENERATED_BODY()
	
public:	
	ACustomFileHelper();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "File")
	FString loadFile(FString fileName);
	UFUNCTION(BlueprintCallable, Category = "File")
	void saveFile(FString fileName, FString content);
};
