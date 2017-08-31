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
		<< "\t\tWelcome to Bulls and Cows, a fun word game!"
		<< std::endl << std::endl
		<< "\t\t\t         }   {         ___ " << std::endl
		<< "\t\t\t         (o o)        (o o) " << std::endl
		<< "\t\t\t  /-------\\ /          \\ /-------\\ " << std::endl
		<< "\t\t\t / | BULL |O            O| COW  | \\ " << std::endl
		<< "\t\t\t*  |-,--- |              |------|  * " << std::endl
		<< "\t\t\t   ^      ^              ^      ^ " << std::endl
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
			<< std::endl
			<< "\tIt took you " << Tries << " ";
		if (Tries == 1) std::cout << "guess.";
		else std::cout << "guesses.";
		std::cout << std::endl << std::endl;
	}
	// Otherwise tell them they lost
	else
	{
		BCGame.Lose(); // <- Resets winstreak
		std::cout << "\tYou lost! Better luck next time..."
			<< std::endl << std::endl;
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

void PlayGame()
{
	// Loop through the amount of tries
	while(!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= BCGame.GetMaxTries()) {
		// Get the guess and pass it into SubmitGuess()
		FText Guess = GetValidGuess();
		FBullCowCount BCCount = BCGame.SubmitValidGuess(Guess);

		// Display the amount of Bulls and Cows
		std::cout << "\tBulls = " << BCCount.Bulls
			<< ". Cows = " << BCCount.Cows
			<< std::endl << std::endl;
	}

	PrintGameSummary();
	return;
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
		GuessStatus = BCGame.CheckGuessValidity(Guess);
		switch (GuessStatus)
		{
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
	return BCGame.ToLowerCase(Guess);
}

bool AskToPlayAgain()
{
	std::cout << "\tDo you want to play again? (y/n): ";
	FText Response = "";
	std::getline(std::cin, Response);
	std::cout << std::endl;

	// Following always returns false except if player enters 'y' or 'Y'
	if (Response[0] == 'y' || Response[0] == 'Y') return true;
	else if (Response[0] == 'n' || Response[0] == 'N') return false;
	return false;
}
