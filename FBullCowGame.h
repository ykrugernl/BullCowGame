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

struct FHint
{
	char FirstLetter;
	char LastLetter;
};

// Used in main.cpp->GetValidGuess() and FBullCowGame.cpp->CheckGuessValidity()
enum class EGuessStatus
{
	OK,
	NO_STATUS,
	NOT_CORRECT_WORD_LENGTH,
	NOT_ISOGRAM,
	HELP,
	HINT,
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

	void PayHint(int32);
	FHint GetHint();
	void Lose();
	void Reset();
	FBullCowCount SubmitValidGuess(FString);

private:
	int32 MyCurrentTry;
	std::vector<std::vector<FString>> WordList { 
		{ "ape", "bot", "top", "zip", "war", "jaw", "lid", "dip", "eat", "tea", "bed", "old", "shy", "hut", "fun", "mob", "pot" },
		{ "drop", "knot", "shot", "grow", "yoga", "lobe", "pelt", "mean", "join", "push", "wake", "trip", "coin", "flow",  },
		{ "field", "forge", "trial", "plane", "cargo", "major", "axiom", "faith", "hated", "dough", "grove", "claim", "yours", "strap", },
		{ "planet", "planes", "travel", "coding", "pocket", "mosaic", "stroke", "fandom", "winces", "punish", "bought", "shrimp", "trains", },
		{ "isogram", "roaming", "soaring", "laundry", "spawned", "tackled", "dashing", "largest", "patched", "metrics", "bunches", "cashier", },
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
