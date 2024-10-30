#include "minecraft.h"

std::string minecraft::effect::level( int num )
{
	if( num == 0 )
	{
		return "";
	}

	std::string result = " ";

	for( int i = 0; i < 5 && num > 0; ++i )
	{
		while( num >= roman_numbs_integers[i] )
		{
			result += roman_numbs_literal[i];
			num -= roman_numbs_integers[i];
		}
	}

	return result;
}
