// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()
	
	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void SetupGame();
	void EndGame();
	FBullCowCount GetBullsCows(const FString& Guess) const;
	void ProcessGuess(const FString& Guess);
	TArray<FString> GetValidWords(TArray<FString>& WordList) const;
	bool IsIsogram(const FString& Word)const;

	// Your declarations go below!
	private:
	FString MagicWord;
	int32 Lives;
	bool bGameOver;
	TArray<FString> Words;
	TArray<FString> Isograms;
};
