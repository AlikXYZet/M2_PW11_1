#include "MyRandom.h"

int64 GetRandom(const int64 &min, const int64 &max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(min, max);

	return distrib(gen);
}

bool GetRandom()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::bernoulli_distribution d(0.5);

	return d(gen);
}

float GetRandomFloat(const float &min, const float &max)
{
	return GetRandom(min * 256, max * 256) / static_cast<float>(256);
}

FString GetRandomNickname()
{
	TArray<FString> VowelLetter { "a", "e", "i", "o", "u", "y" };
	TArray<FString> ConsonantLetter { "b", "c", "d", "f", "g", "h", "j", "k", "l", "m", "n", "p", "q", "r", "s", "t", "v", "w", "x", "z" };

	FString Result;

	bool bIsStartWithVowel = GetRandom();

	for (int8 j = 0; j < GetRandom(3, 8); j++)
	{
		bool FlipFlop = false;
		if (j % 2)
			FlipFlop = true;

		if (bIsStartWithVowel)
		{
			if (FlipFlop)
				Result.Append(VowelLetter[GetRandom(0, VowelLetter.Num() - 1)]);
			else
				Result.Append(ConsonantLetter[GetRandom(0, ConsonantLetter.Num() - 1)]);
		}
		else
		{
			if (FlipFlop)
				Result.Append(ConsonantLetter[GetRandom(0, ConsonantLetter.Num() - 1)]);
			else
				Result.Append(VowelLetter[GetRandom(0, VowelLetter.Num() - 1)]);
		}
	}

	return Result;
}

int GetRandomAge()
{
	return GetRandom(18, 60);
}

int GetRandomRating()
{
	return GetRandom(25, 100);
}

int GetRandomID()
{
	return GetRandom(0, INT_MAX);
}