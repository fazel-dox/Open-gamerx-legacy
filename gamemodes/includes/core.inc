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

// ---===[ 1. CORE LIBRARIES ]===---
// These are needed by nearly every file in the project.
#include <open.mp>
#include <sscanf2>

// CORRECT YSI INITIALIZATION:
// This single include handles the entire YSI library suite, including
// y_commands, y_hooks, and all the core utilities that were causing errors.
// It must be placed AFTER sscanf2.
#include <YSI_Coding\y_hooks>

#include <samp_bcrypt>
// a_mysql is included via mysql.inc below

// ---===[ 2. CORE SYSTEM INCLUDES ]===---
#include "includes/mysql.inc" // Must be included to define g_dbConnection for other files.

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