/*================================================================================
    === START OF FILE: gamemodes\includes\core.inc ===
================================================================================*/

/*
    ================================================================================
    Project:    GamerX Rebirth - Core Definitions
    File:       core.inc
    Description: The single source of truth for all global variables, enums,
                 and definitions used across the entire gamemode.
                 EVERY other system include file should include this file first.
    ================================================================================
*/

#if defined _core_included
    #endinput
#endif
#define _core_included

// ---===[ 1. CORE LIBRARIES (STRICT ORDER) ]===---

// STEP 1: The absolute core of the server. Must be first.
#include <open.mp>

// STEP 2: Foundational libraries required by other systems.
#include <sscanf2>
#include <samp_bcrypt>

// FIX: Replaced the single 'default.inc' with specific includes for reliability.
// This ensures that all required functions like random(), min(), max(), funcidx(),
// getproperty(), etc., are correctly loaded before YSI needs them. This single
// change resolves the vast majority of the "undefined symbol" errors.
// #include <pawn-stdlib/default.inc> // Original line
#include <pawn-stdlib/core>
#include <pawn-stdlib/string>
#include <pawn-stdlib/amx>


// STEP 3: The master YSI include. It depends on the above files.
// This single include handles all sub-modules like y_commands, y_iterate, etc.
#include <YSI_Coding\y_hooks>


// ---===[ 2. CORE SYSTEM INCLUDES ]===---
#include "includes/mysql.inc" // Defines g_dbConnection for other files.


// ---===[ 3. GLOBAL DEFINITIONS ]===---
// Dialogs
#define DIALOG_LOGIN                1
#define DIALOG_REGISTER             2
#define DIALOG_RULES                3
#define DIALOG_UNREGISTERED_HELP    4
#define DIALOG_COMMANDS_LIST        5

// Admin Levels
#define ADMIN_NONE                  0
#define ADMIN_PLAYER                1
#define ADMIN_TRUSTED_PLAYER        2
#define ADMIN_TRIAL                 3
#define ADMIN_STANDARD              4
#define ADMIN_MANAGEMENT            9
#define ADMIN_OWNER                 10


// ---===[ 4. GLOBAL ENUMS & VARIABLES ]===---
enum E_PLAYER_DATA
{
    pLoggedIn,
    pPassword[129],
    pAdminLevel,
    pClass,
    pLevel,
    pMoney,
    pKills,
    pDeaths
}
new PlayerData[MAX_PLAYERS][E_PLAYER_DATA];

new g_playerVehicle[MAX_PLAYERS];

/*================================================================================
    === END OF FILE: gamemodes\includes\core.inc ===
================================================================================*/