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

/*
	-MC Should we really waste resources on this? Seems pointless

	g_ConCommands.CreateCommand(
		"cfg_find", [this](const auto& args)
		{
			if( args.Count() < 2 )
			{
				Con_Printf("Usage: %s key_name_prefix\n", args.Argument(0) );
				return;
			}
		},
		CommandLibraryPrefix::No
	);
*/

	g_ConCommands.CreateCommand(
		"cfg_update", [this](const auto& args)
		{
			if( args.Count() != 3 )
			{
				Con_Printf( "Usage: %s key_name value\n", args.Argument(0) );
			}
			else if( auto sv = std::string_view( args.Argument(1) ); m_config->contains( sv ) )
			{
				if( (*m_config)[ sv ].is_number() )
				{
					Con_Printf( "Set key_name %s to float value %f\n", sv, atof( args.Argument(2) ) );
					SetValue( sv, atof( args.Argument(2) ) );
				}
				else
				{
					Con_Printf( "Set key_name %s to string value %f\n", sv, args.Argument(2) );
					SetValue( sv, args.Argument(2) );
				}
			}
			else
			{
				Con_Printf( "Unsuported key_name %s\n", args.Argument(1) );
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
	if( m_config != nullptr )
		m_config->clear();
	else
		m_config = std::make_unique<json>();

	if( _list_logged_ != nullptr )
		_list_logged_->clear();
	else
		_list_logged_ = std::make_unique<std::vector<std::string_view>>();

#ifndef CLIENT_DLL
	LoadConfigFile("cfg/server/default_configuration.json");

	if( auto mapCfgFileName = fmt::format("cfg/maps/{}.json", STRING( gpGlobals->mapname ) );
		g_pFileSystem->FileExists( mapCfgFileName.c_str() ) ) {
			g_Cfg.LoadConfigFile( mapCfgFileName.c_str() );
	}
#else
	LoadConfigFile("cfg/networking/default_configuration.json");
#endif
}

void ConfigurationSystem::LoadConfigFile(const char* name)
{
    if( std::optional<json> opt_cfg = g_JSON.LoadJSONFile( name ); opt_cfg.has_value() )
	{
		json new_cfg = opt_cfg.value();

		for( auto okv_cfg = new_cfg.begin(); okv_cfg != new_cfg.end(); okv_cfg++ )
		{
			if( json obj_cfg = okv_cfg.value(); obj_cfg && obj_cfg.is_object() )
			{
				for( auto okv_obj = obj_cfg.begin(); okv_obj != obj_cfg.end(); okv_obj++ )
				{
					(*m_config)[ fmt::format( "{}_{}", okv_cfg.key(), okv_obj.key() ) ] = okv_obj.value();
				}
			}
			else
			{
				(*m_config)[ okv_cfg.key() ] = okv_cfg.value();
			}
		}
    }
	else
	{
		m_Logger->error( "Failed to load \"{}\".", name );
    }
}

float ConfigurationSystem::GetValue(std::string_view name, float defaultValue, CBaseEntity* pEntity) const
{
#ifndef CLIENT_DLL
	if( pEntity != nullptr && pEntity->m_config != nullptr && pEntity->m_config->contains( name ) && (*pEntity->m_config)[ name ].is_number() )
	{
		return (*pEntity->m_config)[name].get<float>();
	}
#endif

    if( m_config->contains( name ) && (*m_config)[ name ].is_number() )
    {
        return (*m_config)[name].get<float>();
    }

	_list_log_(name, defaultValue);

    return defaultValue;
}

std::string ConfigurationSystem::GetValue(std::string_view name, std::string_view defaultValue, CBaseEntity* pEntity) const
{
#ifndef CLIENT_DLL
	if( pEntity != nullptr && pEntity->m_config != nullptr
		&& pEntity->m_config->contains( name ) && (*pEntity->m_config)[ name ].is_string() ) {
			return (*pEntity->m_config)[name].get<std::string>();
	}
#endif

    if( m_config->contains( name ) && (*m_config)[ name ].is_string() ) {
        return (*m_config)[name].get<std::string>();
    }

	_list_log_(name, defaultValue);

    return std::string( defaultValue );
}

void ConfigurationSystem::SetValue(std::string_view name, float value, CBaseEntity* pEntity)
{
#ifndef CLIENT_DLL
	if( pEntity != nullptr && pEntity->m_config != nullptr
		&& pEntity->m_config->contains( name ) && (*pEntity->m_config)[ name ].is_string() ) {
			(*pEntity->m_config)[name] = value;	
				return;
	}
#endif

	(*m_config)[name] = value;
}

void ConfigurationSystem::SetValue(std::string_view name, std::string_view value, CBaseEntity* pEntity)
{
#ifndef CLIENT_DLL
	if( pEntity != nullptr && pEntity->m_config != nullptr
		&& pEntity->m_config->contains( name ) && (*pEntity->m_config)[ name ].is_number() ) {
			(*pEntity->m_config)[name] = value;	
				return;
	}
#endif

	(*m_config)[name] = value;
}

void ConfigurationSystem::_list_log_(std::string_view log, std::variant<float, std::string_view> defaultValue) const
{
	if( std::find( _list_logged_->begin(), _list_logged_->end(), log ) == _list_logged_->end() )
	{
		std::visit( [&]( auto&& value ) {
			m_Logger->warn( "Undefined variable \"{}\". Using default value \"{}\"", log, value );
		}, defaultValue );

		_list_logged_->push_back(log);
	}
}
