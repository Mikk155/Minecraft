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
		"cfg_update", [this](const auto& args)
		{
			if( args.Count() != 3 )
			{
				Con_Printf( "Usage: %s key_name value\n", args.Argument(0) );
			}
			else if( auto sv = std::string_view( args.Argument(1) ); m_config->contains( sv ) )
			{
				if( FStrEq( args.Argument(2), "false" ) || FStrEq( args.Argument(2), "true" ) )
				{
					Con_Printf( "Set key_name %s to bool value %f\n", sv, args.Argument(2) );
					SetValue( sv, FStrEq( args.Argument(2), "true" ) );
				}
				if( (*m_config)[ sv ].is_number() )
				{
					Con_Printf( "Set key_name %s to float value %f\n", sv, atof( args.Argument(2) ) );
					float flValue = atof( args.Argument(2) );
					SetValue( sv, flValue );
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

	LoadConfigFile(ConfigFileServer());

	// Include custom config for the current map
	if( auto mapCfgFileName = fmt::format("cfg/maps/{}.json", STRING( gpGlobals->mapname ) );
		g_pFileSystem->FileExists( mapCfgFileName.c_str() ) ) {
			g_Cfg.LoadConfigFile( mapCfgFileName.c_str() );
	}

	// Remove any existing files first to prevent problems if it isn't purged by the filesystem on open.
	RemoveTempData("GAMECONFIG");

	FSFile file{ ConfigFileClient(), "wb", "GAMECONFIG" };

	if( !file.IsOpen() )
	{
		m_Logger->error( fmt::format( "Failed to create \"{}\" Clients will use their default configuration.", ConfigFileClient() ) );
	}
	else
	{
		try
		{
			// The engine generally accepts invalid UTF8 text in things like filenames so ignore invalid UTF8.
			auto fileData = m_config->dump(-1, ' ', false, nlohmann::detail::error_handler_t::ignore);

			/**
			 *	@brief The minimum size that the generated file must be.
			*	See https://github.com/ValveSoftware/halflife/issues/3326 for why this is necessary.
			*/
			std::size_t MinimumFileDataSize = MAX_QPATH + "uncompressed"sv.size() + 1 + sizeof(int);

			// Pad the file with whitespace to reach the minium valid size.
			if (fileData.size() < MinimumFileDataSize)
			{
				fileData.append(MinimumFileDataSize - fileData.size(), ' ');
			}

			file.Write(fileData.data(), fileData.size());

			m_Logger->debug("Client config file saved: {} bytes", fileData.size());

			// Precache the file so clients download it.
			UTIL_PrecacheGenericDirect(ConfigFileClient());
		}
		catch(const std::exception& e)
		{
			// Remove in case it's been writted.
			RemoveTempData("GAMECONFIG");
			m_Logger->error("Error converting JSON to UTF8 text: {}", e.what());
		}
	}

#else

	auto LoadServerDefault = [](const char* message)
	{
		g_Cfg.m_Logger->error(fmt::format( "{}, Using \"{}\"", message, g_Cfg.ConfigFileServer() ) );
		g_Cfg.LoadConfigFile(g_Cfg.ConfigFileServer());
	};

	// The file can be in either of these, so check both.
	// Do NOT use a null path ID, since that allows overriding the file using alternate directories!
	// Check downloads first since we clear it no matter what.
	// If this is a listen server we'll fall back to the file saved by the server.
	FSFile file{ ConfigFileClient(), "rb", "GAMEDOWNLOAD" };

	if( !file.IsOpen() ) {
		file.Open(ConfigFileClient(), "rb", "GAMECONFIG");
	}

	if( !file.IsOpen() ) {
		LoadServerDefault("Server failed to network custom game configuration");
		return;
	}

	std::size_t sizeInBytes = file.Size();

	std::vector<std::uint8_t> fileData;

	fileData.resize(sizeInBytes);

	if( static_cast<std::size_t>( file.Read( fileData.data(), fileData.size() ) ) != fileData.size() )
	{
		LoadServerDefault("Error loading custom game configuration: read error");
		return;
	}

	json input = json::parse(fileData);

	if (!input.is_object())
	{
		LoadServerDefault("Error loading custom game configuration: data is invalid");
		return;
	}

	m_config = std::make_unique<json>(input);

	m_Logger->debug("Network custom configuration loaded: {} bytes", fileData.size());

#endif
}

void ConfigurationSystem::RemoveTempData(const char* PathID)
{
	g_pFileSystem->RemoveFile( g_Cfg.ConfigFileClient(), PathID );
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

/*
float ConfigurationSystem::GetValue(std::string_view name, float value, std::optional<CBaseEntity*> entity) const
{
#ifndef CLIENT_DLL
	if( entity.has_value() )
	{
		if( auto pEntity = entity.value(); pEntity != nullptr && pEntity->m_config->contains( name ) )
		{
			if( (*pEntity->m_config)[ name ].is_number() )
			{
				return (*pEntity->m_config)[name].get<float>();
			}
			else
			{
				m_Logger->warn( "Variable \"{}\" in entity {}:{} is not a \"numeric\" value!", name, pEntity->GetClassname(), pEntity->GetTargetname() );
			}
		}
	}
#endif

    if( m_config->contains( name ) )
	{
		if( (*m_config)[ name ].is_number() )
		{
	        return (*m_config)[name].get<float>();
		}
		else
		{
			m_Logger->warn( "Variable \"{}\" is not a \"numeric\" value!", name );
		}
    }

	_list_log_(name, value);

    return value;
}

std::string ConfigurationSystem::GetValue(std::string_view name, std::string_view value, std::optional<CBaseEntity*> entity) const
{
#ifndef CLIENT_DLL
	if( entity.has_value() )
	{
		if( auto pEntity = entity.value(); pEntity != nullptr && pEntity->m_config->contains( name ) )
		{
			if( (*pEntity->m_config)[ name ].is_string() )
			{
				return (*pEntity->m_config)[name].get<std::string>();
			}
			else
			{
				m_Logger->warn( "Variable \"{}\" in entity {}:{} is not a \"string\" value!", name, pEntity->GetClassname(), pEntity->GetTargetname() );
			}
		}
	}
#endif

    if( m_config->contains( name ) )
	{
		if( (*m_config)[ name ].is_string() )
		{
        	return (*m_config)[name].get<std::string>();
		}
		else
		{
			m_Logger->warn( "Variable \"{}\" is not a \"string\" value!", name );
		}
    }

	_list_log_(name, value);

    return std::string( value );
}

bool ConfigurationSystem::GetValue(std::string_view name, bool value, std::optional<CBaseEntity*> entity) const
{
#ifndef CLIENT_DLL
	if( entity.has_value() )
	{
		if( auto pEntity = entity.value(); pEntity != nullptr && pEntity->m_config->contains( name ) )
		{
			if( (*pEntity->m_config)[ name ].is_boolean() )
			{
				return (*pEntity->m_config)[name].get<bool>();
			}
			else
			{
				m_Logger->warn( "Variable \"{}\" in entity {}:{} is not a \"boolean\" value!", name, pEntity->GetClassname(), pEntity->GetTargetname() );
			}
		}
	}
#endif

    if( m_config->contains( name ) )
	{
		if( (*m_config)[ name ].is_boolean() )
		{
        	return (*m_config)[name].get<bool>();
		}
		else
		{
			m_Logger->warn( "Variable \"{}\" is not a \"boolean\" value!", name );
		}
    }

	_list_log_(name, value);

    return value;
}
*/

void ConfigurationSystem::_list_log_(std::string_view log, std::variant<float, std::string_view, bool> value) const
{
	if( std::find( _list_logged_->begin(), _list_logged_->end(), log ) == _list_logged_->end() )
	{
		std::visit( [&]( auto&& avalue ) {
			m_Logger->warn( "Undefined variable \"{}\". Using default value \"{}\"", log, avalue );
		}, value );

		_list_logged_->push_back(log);
	}
}

void ConfigurationSystem::GetValue(std::string_view name, std::variant<float*, int*, const char*, bool*> value, std::optional<CBaseEntity*> entity) const
{
    std::visit([&](auto&& DefaultValue)
	{
#ifndef CLIENT_DLL
		if( entity.has_value() )
		{
			if( auto pEntity = entity.value(); pEntity != nullptr && pEntity->m_config->contains( name ) )
			{
				using T = std::decay_t<decltype(DefaultValue)>;

				if( (*pEntity->m_config)[ name ].is_string() ) {
					if constexpr( std::is_same_v<T, const char*> )
						DefaultValue = (*pEntity->m_config)[name].get<std::string>().c_str();
				}
				else if( (*pEntity->m_config)[ name ].is_number() ) {
					if constexpr( std::is_same_v<T, float*> )
						*DefaultValue = (*pEntity->m_config)[name].get<float>();
					if constexpr( std::is_same_v<T, int*> )
						*DefaultValue = (*pEntity->m_config)[name].get<int>();
				}
				else if( (*pEntity->m_config)[ name ].is_boolean() ) {
					if constexpr( std::is_same_v<T, bool*> )
						*DefaultValue = (*pEntity->m_config)[name].get<bool>();
				}
			}
		}
#endif

		if( m_config->contains( name ) )
		{
			using T = std::decay_t<decltype(DefaultValue)>;

			if( (*m_config)[ name ].is_string() ) {
				if constexpr( std::is_same_v<T, const char*> )
					DefaultValue = (*m_config)[name].get<std::string>().c_str();
			}
			else if( (*m_config)[ name ].is_number() ) {
				if constexpr( std::is_same_v<T, float*> )
					*DefaultValue = (*m_config)[name].get<float>();
				if constexpr( std::is_same_v<T, int*> )
					*DefaultValue = (*m_config)[name].get<int>();
			}
			else if( (*m_config)[ name ].is_boolean() ) {
				if constexpr( std::is_same_v<T, bool*> )
					*DefaultValue = (*m_config)[name].get<bool>();
			}
		}
    }, value);
}

void ConfigurationSystem::SetValue(std::string_view name, std::variant<float, std::string_view, bool> value, std::optional<CBaseEntity*> entity) const
{
	std::visit([&](auto&& DefaultValue)
	{
#ifndef CLIENT_DLL
		if( entity.has_value() )
		{
			if( m_config->contains( name ) )
			{
				if( auto pEntity = entity.value(); pEntity != nullptr )
				{
					using T = std::decay_t<decltype(DefaultValue)>;

					if( (*pEntity->m_config)[ name ].is_string() )
					{
						if constexpr( !std::is_same_v<T, std::string> )
						{
							m_Logger->warn( "Can not set variable \"{}\" of type \"string\" to value \"{}\" for entity {}:{}:{}",
								name, DefaultValue, pEntity->entindex(), pEntity->GetClassname(), pEntity->GetTargetname() );
							return;
						}
					}
					else if( (*pEntity->m_config)[ name ].is_number() )
					{
						if constexpr( !std::is_same_v<T, float> )
						{
							m_Logger->warn( "Can not set variable \"{}\" of type \"float\" to value \"{}\" for entity {}:{}:{}",
								name, DefaultValue, pEntity->entindex(), pEntity->GetClassname(), pEntity->GetTargetname() );
							return;
						}
					}
					else if( (*pEntity->m_config)[ name ].is_boolean() )
					{
						if constexpr( !std::is_same_v<T, float> )
						{
							m_Logger->warn( "Can not set variable \"{}\" of type \"bool\" to value \"{}\" for entity {}:{}:{}",
								name, DefaultValue, pEntity->entindex(), pEntity->GetClassname(), pEntity->GetTargetname() );
							return;
						}
					}

					(*pEntity->m_config)[name] = DefaultValue;
					m_Logger->info( "Set key \"{}\" to \"{}\" for entity {}:{}:{}",
						name, DefaultValue, pEntity->entindex(), pEntity->GetClassname(), pEntity->GetTargetname() );
				}
			}
			else
			{
				m_Logger->warn( "Can't set variable \"{}\" it doesn't exists in the default configuration!", name );
			}
			return;
		}
#endif

		if( m_config->contains( name ) )
		{
			using T = std::decay_t<decltype(DefaultValue)>;

			if( (*m_config)[ name ].is_string() )
			{
				if constexpr( !std::is_same_v<T, std::string> )
				{
					m_Logger->warn( "Can not set variable \"{}\" of type \"string\" to value \"{}\"", name, DefaultValue );
					return;
				}
			}
			else if( (*m_config)[ name ].is_number() )
			{
				if constexpr( !std::is_same_v<T, float> )
				{
					m_Logger->warn( "Can not set variable \"{}\" of type \"float\" to value \"{}\"", name, DefaultValue );
					return;
				}
			}
			else if( (*m_config)[ name ].is_boolean() )
			{
				if constexpr( !std::is_same_v<T, float> )
				{
					m_Logger->warn( "Can not set variable \"{}\" of type \"bool\" to value \"{}\"", name, DefaultValue );
					return;
				}
			}

			(*m_config)[name] = DefaultValue;
			m_Logger->info( "Set key \"{}\" to \"{}\"", name, DefaultValue );
		}
		else
		{
			m_Logger->warn( "Can't set variable \"{}\" it doesn't exists in the default configuration!", name );
		}

	}, value);
}
