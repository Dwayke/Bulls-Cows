// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordList/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    Isograms = GetValidWords(Words);
    SetupGame(); 
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    ClearScreen();
    if (bGameOver == false)
    {
        ProcessGuess(PlayerInput);
    }
    else
    {
        SetupGame();
    }
}

void UBullCowCartridge::SetupGame() // Sets up the game
{
    MagicWord = Isograms[FMath::RandRange(0, Isograms.Num())-1];
    Lives = MagicWord.Len() * 2;
    bGameOver = false;
    PrintLine(TEXT("~~Welcome to Bulls & Cows!~~"));
    PrintLine(TEXT("~~Guess the %i letter Magic Word!~~"), MagicWord.Len());
    PrintLine(TEXT("~~You have %i Lives!~~\n"), Lives);
    PrintLine(TEXT("\"When you get a right letter in the right\norder it's a bull!\nAnd when you get a right letter in the\nwrong order it's a cow!\"\n"));
    PrintLine(TEXT("~~Press \"Tab\" to start!~~"));
    //PrintLine(TEXT("%s\n"), *MagicWord);//debug line
}

void UBullCowCartridge::EndGame() // When the player wins or loses
{
    bGameOver = true;
    PrintLine(TEXT("Press \"Enter\" to play again,\nor \"Alt+F4\" to quit the game.\n"));
}

TArray<FString> UBullCowCartridge::GetValidWords(TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word); 
        }
    }   
    return ValidWords;
}

void UBullCowCartridge::ProcessGuess(const FString& Guess) // Processes the guess
{
    if (MagicWord.Len() != Guess.Len())
    {
        PrintLine(TEXT("Please enter a %i letters word.\n"), MagicWord.Len());
        return;
    }
    if (Guess == MagicWord)
    {
        PrintLine(TEXT("You win!\n"));
        PrintLine(TEXT("The Magic Word was : %s.\n"), *MagicWord);
        EndGame();
        return;
    }
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters please!\n"));
        return;
    }
    FBullCowCount Score =  GetBullsCows(Guess);
    PrintLine(TEXT("Wrong!\n"));
    PrintLine(TEXT("Lives : %i.\n"), --Lives);
    PrintLine(TEXT("Your guess was : %s. \n"), *Guess);
    PrintLine(TEXT("Bulls : %i.\n"), Score.Bulls);
    PrintLine(TEXT("Cows : %i.\n"), Score.Cows);
    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You lose...\n"));
        PrintLine(TEXT("The Magic Word is : %s.\n"), *MagicWord);
        EndGame();
    } 
}

bool UBullCowCartridge::IsIsogram(const FString& Word)const // checks if the guess IsIsogram
{
    for (int32 i = 0; i < Word.Len(); i++)
    {
        for (int32 I = i + 1; I < Word.Len(); I++)
        {
            if (Word[i] == Word[I])
            {
                return false;
            }
        }
    }
    return true;
}

FBullCowCount UBullCowCartridge::GetBullsCows(const FString& Guess) const
{
    FBullCowCount Count;
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex]==MagicWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for (int32 MagicIndex = 0; MagicIndex < MagicWord.Len(); MagicIndex++)
        {
            if (Guess[GuessIndex] == MagicWord[MagicIndex])
            {
                Count.Cows++;
                break;
            }
        }        
    }
    return Count;
}