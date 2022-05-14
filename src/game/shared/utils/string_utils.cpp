/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#include <charconv>

#include "Platform.h"

#include "string_utils.h"

std::string_view TrimStart(std::string_view text)
{
	std::size_t index = 0;

	while (index < text.size() && 0 != std::isspace(text[index]))
	{
		++index;
	}

	if (index < text.size())
	{
		return text.substr(index);
	}

	//text was empty or consisted only of whitespace
	return {};
}

std::string_view TrimEnd(std::string_view text)
{
	//Avoid relying on overflow
	std::size_t index = text.size();

	while (index > 0 && std::isspace(text[index - 1]))
	{
		--index;
	}

	//index is the number of characters left in the string
	if (index > 0)
	{
		return text.substr(0, index);
	}

	//text was empty or consisted only of whitespace
	return {};
}

std::string_view Trim(std::string_view text)
{
	return TrimEnd(TrimStart(text));
}

//TODO: need to use ICU for Unicode-compliant conversion

void ToLower(std::string& text)
{
	std::transform(text.begin(), text.end(), text.begin(), [](auto c)
		{ return std::tolower(c); });
}

void ToUpper(std::string& text)
{
	std::transform(text.begin(), text.end(), text.begin(), [](auto c)
		{ return std::toupper(c); });
}

std::string ToLower(std::string_view text)
{
	std::string result{text};

	ToLower(result);

	return result;
}

std::string ToUpper(std::string_view text)
{
	std::string result{text};

	ToUpper(result);

	return result;
}

void UTIL_StringToVector(float* pVector, std::string_view pString)
{
	std::size_t index = 0;

	int j;

	const char* const end = pString.data() + pString.size();

	for (j = 0; j < 3; j++) // lifted from pr_edict.c
	{
		const auto result = std::from_chars(pString.data() + index, end, pVector[j]);

		if (result.ec != std::errc())
		{
			break;
		}

		//Continue after parsed value.
		index = result.ptr - pString.data();

		//Skip all whitespace.
		while (index < pString.size() && std::isspace(pString[index]))
			++index;
		if (index >= pString.size())
			break;
	}
	if (j < 2)
	{
		/*
		ALERT( at_error, "Bad field in entity!! %s:%s == \"%s\"\n",
			pkvd->szClassName, pkvd->szKeyName, pkvd->szValue );
		*/
		for (j = j + 1; j < 3; j++)
			pVector[j] = 0;
	}
}
