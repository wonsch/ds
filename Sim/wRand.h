///////////////////////////////////////////////////////////////
// 작성자 : 원순철 (wonsch6207@hanmail.net)
// 날  짜 : 최초작성 2011-??-??, 최종수정 2011-09-21
///////////////////////////////////////////////////////////////

#pragma once

class CwRand
{
public:
	CwRand(unsigned long Seed = 0)
	{
		SetSeed(Seed);
	}

	// Seed 값 설정
	void SetSeed(unsigned long Seed)
	{
		Rand = Seed;
	}

	unsigned long operator()()
	{
		return GetULong();
	}

	// [0, 4294967295]
	unsigned long GetULong()
	{
		return Rand = Rand * 1103515245 + 12345;
	}

	// [0.0, 1.0]
	double GetDouble()
	{
		return (double)GetULong() / 0xFFFFFFFF;
	}

private:
	unsigned long Rand;
};
