/* 
Filename:
	main.cpp

Description:
	This file handles all the user interaction and output,
	for all the game logic see FBullCowGame.h and FBullCowGame.cpp
*/

#pragma once

#include <iostream>
#include <string>
#include "FBullCowGame.h"

// Unreal Engine friendly variables
using FText = std::string;
using int32 = int;

// Function prototypes
void PrintIntroduction();
void PlayGame();
FText GetValidGuess();
bool AskToPlayAgain();
void PrintGameSummary();
void PrintEndScreen();
void PrintHelp();
void PrintBullsAndCows(FBullCowCount);
void PrintScore();
void ConfirmHint();

// Instantiate game object as a global variable
FBullCowGame BCGame;

int main()
{
	// Loop as long as AskToPlayAgain() returns true
	do
	{
		BCGame.Reset();
		PrintIntroduction();
		PlayGame();
	} while (AskToPlayAgain());

	// Display score
	PrintEndScreen();

	// End application
	std::cout << "\tPress any key to exit...";
	std::cin.get(); // <- Keypress needed
	return 0;
}

void PrintIntroduction()
{
	// Introduction
	std::cout
		<< "====================================================================================="
		<< std::endl << std::endl
		<< "\t\tWelcome to Bulls & Cows, a fun word game!"
		<< std::endl << std::endl
		<< "\t\t\t         }   {         ___ " << std::endl
		<< "\t\t\t         (o o)        (o o) " << std::endl
		<< "\t\t\t  /-------\\ /          \\ /-------\\ " << std::endl
		<< "\t\t\t / | BULL |O            O| COW  | \\ " << std::endl
		<< "\t\t\t*  |-,--- |              |------|  * " << std::endl
		<< "\t\t\t   ^      ^              ^      ^ " << std::endl
		<< std::endl
		<< "\t- Type 'help' for more information - Type 'hint' for a hint -"
		<< std::endl << std::endl
		<< "====================================================================================="
		<< std::endl << std::endl
		<< "\tCURRENT DIFFICULTY - " << BCGame.GetDifficultyAsText() << ":"
		<< std::endl
		<< "\tCan you guess the "
		<< BCGame.GetHiddenWordLength()
		<< " letter isogram I'm thinking of in "
		<< BCGame.GetMaxTries()
		<< " turns?"
		<< std::endl << std::endl
		<< "====================================================================================="
		<< std::endl << std::endl;
	return;
}

void PrintGameSummary()
{
	// If game won congratulate player
	if (BCGame.IsGameWon())
	{
		int32 Tries = BCGame.GetCurrentTry() - 1;
		std::cout << "\tYou won! Congratulations!"
			<< std::endl;
	}
	// Otherwise tell them they lost
	else
	{
		BCGame.Lose(); // <- Resets winstreak
		std::cout << "\tYou lost! Better luck next time..."
			<< std::endl;
	}
}

void PrintEndScreen()
{
	// Display score
	std::cout
		<< "====================================================================================="
		<< std::endl
		<< "\tThanks for playing!"
		<< std::endl
		<< "\tYou scored " << BCGame.GetScore() << " points!"
		<< std::endl
		<< "====================================================================================="
		<< std::endl;
}

void PrintHelp()
{
	// Display help section
	std::cout
		<< std::endl << "\tWelcome to the help section of Bulls & Cows!"
		<< std::endl << "\t - Type in an isogram (a word with no repeating letters) and press enter."
		<< std::endl << "\t   Note that the length of the entered word should be equal to the level"
		<< std::endl << "\t   specific word length, which you can see at the begin of each level."
		<< std::endl << "\t - You can offer a number of tries (depending on difficulty level) for a"
		<< std::endl << "\t   hint and can be done by typing 'hint' followed by pressing enter."
		<< std::endl << "\t - To increase in difficulty you need to guess a word in the current"
		<< std::endl << "\t   difficulty level correct 3 times in a row. If you guess a word wrong"
		<< std::endl << "\t   you lose your win streak, but NOT your score and difficulty level."
		<< std::endl << "\t - To view your current score, type 'score'."
		<< std::endl << std::endl;
}

void PrintScore()
{
	// Display the score
	std::cout << "\tYou have score " << BCGame.GetScore() << " points so far!"
		<< std::endl << std::endl;
}

void ConfirmHint()
{
	// Setup temporary variables
	int32 Cost, Difficulty = BCGame.GetDifficulty();
	FText Temp = " tries";

	// TODO Find correct ratio
	// Determine cost of hint
	if (Difficulty == 1)
	{
		Temp = " try";
		Cost = 1;
	}
	else if (Difficulty == 2 || Difficulty == 3)	Cost = 2;
	else if (Difficulty == 4)						Cost = 3;
	else if (Difficulty == 5)						Cost = 5;
	else if (Difficulty == 6)						Cost = 10;
	else if (Difficulty == 7)						Cost = 20;

	// Check if player has enough tries left
	if (BCGame.GetMaxTries() - BCGame.GetCurrentTry() >= Cost) 
	{
		do
		{
			// Confirm hint
			std::cout << "\tAre you sure you want to spend " << Cost << Temp << " for a hint? (y/n): ";
			FText Response = "";
			std::getline(std::cin, Response);

			if (Response[0] == 'y' || Response[0] == 'Y')
			{
				BCGame.PayHint(Cost);
				FHint Hint = BCGame.GetHint();
				std::cout << std::endl;
				std::cout << " *HINT* The isogram starts with the letter '"
					<< Hint.FirstLetter
					<< "' and ends with the letter '"
					<< Hint.LastLetter
					<< "'!" << std::endl << std::endl;
				break;
			}
		} while (true);
	}
	else std::cout << "\tYou don't have enough tries left!" << std::endl << std::endl;
}

void PlayGame()
{
	// Loop through the amount of tries
	while(!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= BCGame.GetMaxTries()) {
		// Get the guess and pass it into SubmitGuess()
		FText Guess = GetValidGuess();
		FBullCowCount BCCount = BCGame.SubmitValidGuess(Guess);
		PrintBullsAndCows(BCCount);
	}

	PrintGameSummary();
	return;
}

void PrintBullsAndCows(FBullCowCount BCCount)
{
	// Display the amount of Bulls and Cows
	// TODO Not sure if I want to keep this
	/*
	std::cout << "\tBulls = " << BCCount.Bulls
		<< ". Cows = " << BCCount.Cows
		<< std::endl; */

	// Loop through every letter position of the guessed word
	std::cout << "\t";
	int BullsCounter = 0, CowsCounter = 0;
	for (int i = 0; i < BCGame.GetHiddenWordLength(); i++)
	{
		bool Neither = true;

		// Bulls should be bigger than zero and BullsCounter less than Bulls
		if (BCCount.Bulls != 0 && BullsCounter < BCCount.Bulls)
		{
			// If the current letter position is equal to the bull position stored
			// Visualize the position of Bull
			if (i == BCCount.LetterPositionsBulls.at(BullsCounter))
			{
				std::cout << "[ B ]";
				BullsCounter++;
				Neither = false;
			}
		}

		// Cows should be bigger than zero and CowsCounter less than Cows
		if (BCCount.Cows != 0 && CowsCounter < BCCount.Cows)
		{
			// If the current letter position is equal to the cow position stored
			// Visualize the position of Cow
			if (i == BCCount.LetterPositionsCows.at(CowsCounter))
			{
				std::cout << "[ C ]";
				CowsCounter++;
				Neither = false;
			}
		}

		// If neither are the case
		// Visualize empty spot
		if (Neither) std::cout << "[   ]";
	}
	std::cout << std::endl << std::endl;
}

FText GetValidGuess()
{
	FText Guess = "";
	EGuessStatus GuessStatus = EGuessStatus::NO_STATUS;

	// Do until GuessStatus is OK
	do {
		// Get input from player
		std::cout << "\tTry " 
			<< BCGame.GetCurrentTry() 
			<< " of " 
			<< BCGame.GetMaxTries()
			<< ". Enter your guess: ";
		std::getline(std::cin, Guess);

		// Check if guess is valid
		Guess = BCGame.ToLowerCase(Guess);
		GuessStatus = BCGame.CheckGuessValidity(Guess);
		switch (GuessStatus)
		{
		case EGuessStatus::HELP:
			PrintHelp();
			break;
		case EGuessStatus::HINT:
			if(!BCGame.IsHintUsed()) ConfirmHint();
			else std::cout << "\tYou have used your hint for this level already!" << std::endl << std::endl;
			break;
		case EGuessStatus::SCORE:
			PrintScore();
			break;
		case EGuessStatus::NOT_ISOGRAM: // If not isogram
			std::cout << "\tPlease enter an isogram, a word with no recurring letters."
				<< std::endl << std::endl;
			break;
		case EGuessStatus::NOT_CORRECT_WORD_LENGTH: // If not correct word length
			std::cout << "\tPlease enter a "
				<< BCGame.GetHiddenWordLength()
				<< " letter word."
				<< std::endl << std::endl;
			break;
		case EGuessStatus::OK: // If OK
		default:
			break;
		}
	} while (GuessStatus != EGuessStatus::OK);

	// Return guess as a lower case string
	return Guess;
}

bool AskToPlayAgain()
{
	// Looping this because player might unintentionally quit the game at a high difficulty level
	FText Response = "";
	do
	{
		std::cout << "\tDo you want to play again? (y/n): ";
		std::getline(std::cin, Response);

		// Following always returns false except if player enters 'y' or 'Y'
		if (Response[0] == 'y' || Response[0] == 'Y')
		{
			std::cout << std::endl;
			return true;
		}
		else if (Response[0] == 'n' || Response[0] == 'N')
		{
			std::cout << std::endl;
			return false;
		}
	} while (true);
}
