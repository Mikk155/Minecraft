#include "minecraft.h"

std::string_view minecraft::level( int num )
{
	if( num == 0 )
	{
		return ""sv;
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

	return std::string_view( result );
}
