function(add_game_shared_sources target)
	target_sources(${target}
		PRIVATE
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/GameLibrary.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/GameLibrary.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/config_system.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/config_system.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/palette.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ProjectInfoSystem.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ProjectInfoSystem.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/voice_common.h

			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Minecraft/CMinecraft.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Minecraft/CMinecraft.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Minecraft/MC_ConstDefs.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Minecraft/MC_Effects.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Minecraft/MC_Effects.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Minecraft/MC_Enchants.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Minecraft/MC_Enchants.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Minecraft/MC_Utils.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Minecraft/MC_Utils.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/ehandle.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/ehandle.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/EntityClassificationSystem.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/EntityClassificationSystem.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/EntityDictionary.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/entity_shared.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/entity_utils.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/entity_utils.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/player_shared.cpp
			
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/items/CBaseItem.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/items/CBaseItem.h
			
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/items/weapons/AmmoTypeSystem.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/items/weapons/AmmoTypeSystem.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/items/weapons/weapons_shared.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/items/weapons/weapons.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/items/weapons/WeaponDataSystem.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/entities/items/weapons/WeaponDataSystem.h
			
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/models/BspLoader.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/models/BspLoader.h
			
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/networking/NetworkDataSystem.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/networking/NetworkDataSystem.h
			
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/player_movement/pm_debug.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/player_movement/pm_debug.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/player_movement/pm_defs.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/player_movement/pm_info.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/player_movement/pm_materials.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/player_movement/pm_movevars.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/player_movement/pm_shared.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/player_movement/pm_shared.h
			
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/saverestore/DataFieldSerializers.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/saverestore/DataFieldSerializers.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/saverestore/DataMap.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/saverestore/DataMap.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/saverestore/saverestore.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/saverestore/saverestore.h
			
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/scripting/AS/as_addons.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/scripting/AS/as_utils.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/scripting/AS/as_utils.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/scripting/AS/ASManager.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/scripting/AS/ASManager.h
			
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/sound/MaterialSystem.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/sound/MaterialSystem.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/sound/sentence_utils.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/sound/sentence_utils.h
			
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ui/hud/HudReplacementSystem.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ui/hud/HudReplacementSystem.h
			
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/ConCommandSystem.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/ConCommandSystem.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/filesystem_utils.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/filesystem_utils.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/GameSystem.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/GameSystem.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/heterogeneous_lookup.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/json_fwd.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/JSONSystem.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/JSONSystem.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/LogSystem.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/LogSystem.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/PrecacheList.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/PrecacheList.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/ReplacementMaps.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/ReplacementMaps.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/shared_utils.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/shared_utils.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/string_utils.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/string_utils.h
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/StringPool.cpp
			${CMAKE_CURRENT_FUNCTION_LIST_DIR}/utils/StringPool.h)
endfunction()
