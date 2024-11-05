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

#include <algorithm>
#include <cctype>
#include <charconv>
#include <optional>
#include <regex>
#include <tuple>

#include "cbase.h"
#include "config_system.h"

#include "ConCommandSystem.h"
#include "GameLibrary.h"
#include "JSONSystem.h"

bool ConfigurationSystem::Initialize()
{
	m_Logger = g_Logging.CreateLogger("config system");

#ifndef CLIENT_DLL

	g_ConCommands.CreateCommand(
		"cfg_find", [this](const auto& args)
		{
			if (args.Count() < 2)
			{
				Con_Printf("Usage: %s\n", args.Argument(0));
				return;
			}
		},
		CommandLibraryPrefix::No
	);

	g_ConCommands.CreateCommand(
		"cfg_set", [this](const auto& args)
		{
			if (args.Count() != 3)
			{
				Con_Printf("Usage: %s\n", args.Argument(0));
				return;
			}
		},
		CommandLibraryPrefix::No
	);

#endif

	return true;
}

void ConfigurationSystem::Shutdown()
{
	m_Logger.reset();
}

void ConfigurationSystem::LoadConfigFiles()
{
#ifndef CLIENT
	m_config = std::make_unique<json>();

	LoadConfigFile( "cfg/player.json" );
#endif
}

void ConfigurationSystem::LoadConfigFile( const char* name )
{
    if( std::optional<json> m_Configuration = g_JSON.LoadJSONFile( name ); m_Configuration.has_value() )
	{
		std::unique_ptr<json> customConfig = std::make_unique<json>( m_Configuration.value() );

		for( auto it = customConfig->begin(); it != customConfig->end(); ++it )
		{
			(*m_config)[ it.key() ] = it.value();
		}
		customConfig = nullptr;
    }
	else
	{
		m_Logger->error( "Failed to load \"{}\".", name );
    }
}

float ConfigurationSystem::GetValue(std::string_view name, float defaultValue, CBaseEntity* pEntity) const
{
#ifndef CLIENT
	if( pEntity != nullptr && pEntity->m_config != nullptr && pEntity->m_config->contains( name ) && (*pEntity->m_config)[ name ].is_number() )
	{
		return (*pEntity->m_config)[name].get<float>();
	}
#endif

    if( m_config->contains( name ) && (*m_config)[ name ].is_number() )
    {
        return (*m_config)[name].get<float>();
    }

	if( std::find( m_list_logged->begin(), m_list_logged->end(), name ) != m_list_logged->end() )
	{
		m_Logger->warn( "Undefined variable \"{}\". Using default value \"{}\"", name, defaultValue );
		m_list_logged->push_back(name);
	}

    return defaultValue;
}

std::string_view ConfigurationSystem::GetValue(std::string_view name, const std::string& defaultValue, CBaseEntity* pEntity) const
{
#ifndef CLIENT
	if( pEntity != nullptr && pEntity->m_config != nullptr && pEntity->m_config->contains( name ) && (*pEntity->m_config)[ name ].is_string() )
	{
		return (*pEntity->m_config)[name].get<std::string>();
	}
#endif

    if( m_config->contains( name ) && (*m_config)[ name ].is_string() )
    {
        return (*m_config)[name].get<std::string>();
    }

	if( std::find( m_list_logged->begin(), m_list_logged->end(), name ) != m_list_logged->end() )
	{
		m_Logger->warn( "Undefined variable \"{}\". Using default value \"{}\"", name, defaultValue );
		m_list_logged->push_back(name);
	}

    return defaultValue;
}

void ConfigurationSystem::SetValue(std::string_view name, float value)
{
	(*m_config)[name] = value;
}

void ConfigurationSystem::SetValue(std::string_view name, std::string_view value)
{
	(*m_config)[name] = value;
}
