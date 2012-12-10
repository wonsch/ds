///////////////////////////////////////////////////////////////
// �ۼ��� : ����ö (wonsch6207@hanmail.net)
// ��  ¥ : �����ۼ� 2011-??-??, �������� 2011-09-21
///////////////////////////////////////////////////////////////

#pragma once

class CwRand
{
public:
	CwRand(unsigned long Seed = 0)
	{
		SetSeed(Seed);
	}

	// Seed �� ����
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
