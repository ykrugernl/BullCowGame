/* 
Filename:
	FBullCowGame.h

Description:
	Prototyping for FBullCowGame.cpp
*/

#pragma once

#include <string>
#include <vector>

// Unreal Engine friendly variables
using FString = std::string;
using int32 = int;

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

// Used in main.cpp->GetValidGuess() and FBullCowGame.cpp->CheckGuessValidity()
enum class EGuessStatus
{
	OK,
	NO_STATUS,
	NOT_CORRECT_WORD_LENGTH,
	NOT_ISOGRAM,
};

class FBullCowGame
{
public:
	FBullCowGame();

	int32 GetCurrentTry() const;
	int32 GetMaxTries() const;
	int32 GetHiddenWordLength() const;
	int32 GetScore() const;
	bool IsGameWon() const;
	EGuessStatus CheckGuessValidity(FString) const;
	FString ToLowerCase(FString) const;
	int32 GetDifficulty() const;
	FString GetDifficultyAsText() const;

	void Lose();
	void Reset();
	FBullCowCount SubmitValidGuess(FString);

private:
	int32 MyCurrentTry;
	std::vector<std::vector<FString>> WordList { 
		{ "ape", "bot", "top", "zip", "war", "jaw", "lid", "dip" },
		{ "drop", "knot", "shot", "grow", "yoga" },
		{ "field", "forge", "trial", "plane", "cargo", "major" },
		{ "planet", "planes", "travel", "coding", "pocket", "mosaic", "stroke" },
		{ "isogram", "roaming", "soaring" }
	};
	FString MyHiddenWord;
	bool BGameWon;
	int32 Score;
	int32 Difficulty;
	int32 WinStreak;

	FString PickAWord() const;
	bool IsIsogram(FString) const;
	int32 GetRandomNumber(int32, int32) const;

};