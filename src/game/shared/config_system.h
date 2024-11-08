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

#include <fmt/core.h>

#include <spdlog/logger.h>

#include "utils/json_fwd.h"
#include "utils/GameSystem.h"

/**
 *	@brief Loads a global configuration and a per-entity-basis
 */
class ConfigurationSystem final : public IGameSystem
{
	public:

		const char* GetName() const override { return "Configuration"; }

		bool Initialize() override;
		void PostInitialize() override {}
		void Shutdown() override;

		void LoadConfigFile(const char* name);
		void LoadConfigFiles();

		float GetValue(std::string_view name, float defaultValue = 0.f, CBaseEntity* pEntity = nullptr) const;
		std::string_view GetValue(std::string_view name, const std::string&  defaultValue = nullptr, CBaseEntity* pEntity = nullptr) const;
		void SetValue(std::string_view name, float value);
		void SetValue(std::string_view name, std::string_view value);

	private:

		std::shared_ptr<spdlog::logger> m_Logger;

		std::unique_ptr<json> m_config = std::make_unique<json>();

		std::unique_ptr<std::vector<std::string_view>> m_list_logged = std::make_unique<std::vector<std::string_view>>();
};

inline ConfigurationSystem g_Cfg;
