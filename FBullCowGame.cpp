
/* 
Filename:
	FBullCowGame.cpp

Description:
	Everything in this file handles the game logic,
	for user interaction see main.cpp,
	for prototyping see FBullCowGame.h
*/

#pragma once

#include "FBullCowGame.h"
#include <map>
#define TMap std::map

#include <cstdlib>
#include <ctime>

// Constructor function
FBullCowGame::FBullCowGame()
{
	// Do this once only
	Difficulty = 1;
	WinStreak = 0;
	Score = 0;

	// Seed the RNG
	srand(time(NULL));

	// Do all other preparations
	Reset();
}

// Returns MyCurrentTry
int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }

// Maps a number of max tries to the hidden word length
// and returns that amount of tries
int32 FBullCowGame::GetMaxTries() const
{
	TMap<int32, int32> WordLengthToMaxTries{ {3,4}, {4,7}, {5,10}, {6,14}, {7,20} };
	return WordLengthToMaxTries[GetHiddenWordLength()];
}

// Returns length of hidden word
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }

// Returns score
int32 FBullCowGame::GetScore() const { return Score; }

// Returns whether game is won or not
bool FBullCowGame::IsGameWon() const { return BGameWon; }

// Returns difficulty
int32 FBullCowGame::GetDifficulty() const { return Difficulty; }

// Returns difficulty as text ie EASY
FString FBullCowGame::GetDifficultyAsText() const
{
	FString Output = "";
	if (Difficulty == 1) Output = "EASY";
	else if (Difficulty == 2) Output = "MODERATE";
	else if (Difficulty == 3) Output = "HARD";
	else if (Difficulty == 4) Output = "CHALLENGE";
	else if (Difficulty == 5) Output = "EXPERT";
	return Output;
}

// Picks a word depending on the difficulty with a random number
FString FBullCowGame::PickAWord() const
{
	int32 MAX = WordList.at(GetDifficulty() - 1).size();
	return WordList.at(GetDifficulty() - 1).at(GetRandomNumber(0, MAX));
}

// Checks if word is an isogram
bool FBullCowGame::IsIsogram(FString Word) const
{
	// 0 and 1 letter words are isograms by default
	if (Word.length() <= 1) return true;

	// Setup map
	TMap<char, bool> LetterSeen;
	// for each letter in word
	for (char Letter : Word)
	{
		Letter = tolower(Letter);
		if (LetterSeen.count(Letter)) return false;
		else LetterSeen.insert(std::make_pair(Letter, true));
	}

	return true;
}

// Returns a random number between MIN and MAX
int32 FBullCowGame::GetRandomNumber(int32 MIN, int32 MAX) const
{
	return rand() % MAX + MIN;
}

// Return a lower case string
FString FBullCowGame::ToLowerCase(FString Word) const
{
	// Empty string
	FString Output = "";

	// Iterate through word letters, if they are already
	// lowercase just add them to Output otherwise make them
	// lowercase first before adding them
	for (char Letter : Word)
	{
		if (!islower(Letter)) Output += tolower(Letter);
		else Output += Letter;
	}
	return Output;
}

// Checks if the guess is the correct length and if it is an isogram
EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	// Is guess correct length
	if (Guess.length() != GetHiddenWordLength())
	{
		return EGuessStatus::NOT_CORRECT_WORD_LENGTH;
	}
	// Is guess isogram
	else if (!IsIsogram(Guess))
	{
		return EGuessStatus::NOT_ISOGRAM;
	}
	else
	{
		return EGuessStatus::OK;
	}
}

// Resets score and winstreak
void FBullCowGame::Lose()
{
	WinStreak = 0;
}

// Reset the values and set a word
void FBullCowGame::Reset()
{
	BGameWon = false;
	MyCurrentTry = 1;
	MyHiddenWord = PickAWord();
	return;
}

// Submits the already validated word to the game
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BCCount;
	int32 WordLength = MyHiddenWord.length();

	// Compare words
	for (int32 i = 0; i < WordLength; i++)
	{
		for (int32 j = 0; j < WordLength; j++)
		{
			// Increase Bulls if current letter of Guess
			// equals current letter of MyHiddenWord
			if (Guess[i] == MyHiddenWord[j] && i == j) BCCount.Bulls++;
			// Increase Cows if the current letter of Guess
			// is found anywhere else in MyHiddenWord
			else if (Guess[i] == MyHiddenWord[j] && i != j) BCCount.Cows++;
		}
	}

	// If word is correct set current game to won
	if (BCCount.Bulls == WordLength)
	{
		// Calculate value to add to Score
		int32 Temp = GetMaxTries() - (GetCurrentTry() - 1);
		if (Temp < 1) Temp = 1;
		Score += Temp * Difficulty;

		// Difficulty progression
		if (++WinStreak == 3)
		{
			WinStreak = 0;
			if (++Difficulty > 5) Difficulty = 5;
		}

		// Game won
		BGameWon = true;
	}

	// Return the struct with its values
	return BCCount;
}
