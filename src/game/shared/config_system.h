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

#pragma once

#include <array>
#include <limits>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>
#include <variant>

#include <fmt/core.h>

#include <spdlog/logger.h>

#include "utils/json_fwd.h"
#include "utils/GameSystem.h"

/**
 *	@brief Loads configuration for a server, map and entity instances
 */
class ConfigurationSystem final : public IGameSystem
{
	public:

		const char* GetName() const override { return "Configuration"; }

		bool Initialize() override;
		void PostInitialize() override {}
		void Shutdown() override;

		void LoadConfigFiles();

		void LoadConfigFile(const char* name);

		const char* ConfigFileServer()
		{
			return "cfg/server/default_configuration.json";
		}

		const char* ConfigFileClient()
		{
			return "cfg/client/configuration.json";
		}

		void RemoveTempData(const char* PathID);

		void SetValue(std::string_view name, std::variant<float, std::string_view, bool> value, std::optional<CBaseEntity*> entity = std::nullopt) const;
		void GetValue(std::string_view name, std::variant<float*, int*, const char*, bool*> value, std::optional<CBaseEntity*> entity = std::nullopt) const;

		std::shared_ptr<spdlog::logger> m_Logger;

	private:

		std::unique_ptr<json> m_config = std::make_unique<json>();

		std::unique_ptr<std::vector<std::string_view>> _list_logged_;

		void _list_log_(std::string_view log, std::variant<float, std::string_view, bool> value) const;
};

inline ConfigurationSystem g_Cfg;
