#pragma once

class CWorkBase
{
public:
	CWorkBase();
	virtual ~CWorkBase();

	CWorkBase*									NextWork;
};
