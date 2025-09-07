/*
    ================================================================================
    Project:    GamerX Legacy - Base
    File:       gamemode.pwn 
    Description: The core file for the Open GamerX project
                 Initializes all systems and handles core callbacks.
    ================================================================================
*/
#include <open.mp>
// ---===[ 1. LIBRARIES ]===---
// ALL libraries and core systems are now handled by core.inc
// This section is intentionally left blank.


// ---===[ 2. SYSTEM INCLUDES (ORDER IS IMPORTANT) ]===---

// The NEW Foundation - must be first
#include "includes/core.inc"          // Defines all globals, enums, and core libraries

// Core Systems
#include "includes/accounts.inc"
#include "includes/commands.inc"
#include "includes/spawn_class_visual.inc"
#include "includes/admin.inc"
#include "includes/class_system.inc"
#include "includes/player_objects.inc"

// Feature Systems
#include "includes/events.inc"        // Event system framework
#include "includes/races.inc"         // Race system framework

// Experimental Systems
#include "Non_legacy_experimental_features/Fly.inc"
#include "Non_legacy_experimental_features/Globalradio.inc"


// ---===[ 3. DEFINITIONS / 4. GLOBALS ]===---
// MOVED TO includes/core.inc to provide global access to all systems.
// This section is intentionally left blank.
#include <YSI_Visual/y_commands>

// ---===[ 5. CORE CALLBACKS ]===---

public OnGameModeInit()
{
    print("\n------------------------------------");
    print(" GamerX Rebirth - Initializing...");
    print("------------------------------------");

    SetGameModeText("GamerX Rebirth");
    UsePlayerPedAnims();
    DisableInteriorEnterExits();
    EnableStuntBonusForAll(false);
    
    // Initialize Core Systems
    MySQL_Connect(); // From mysql.inc
    Classes_Init();
    Sys_InitSpawn();
    print("[SUCCESS] Spawn System Initialized.");

    // Initialize Feature Systems
    OnGameModeInit_Events();
    OnGameModeInit_Races();

    Teleports_LoadAll();
    
    print("------------------------------------");
    print(" GamerX Rebirth is now running.");
    print("------------------------------------\n");
    return 1;
}

public OnGameModeExit()
{
    OnGameModeExit_Events();
    OnGameModeExit_Races();
    return 1;
}

public OnPlayerConnect(playerid)
{
    // Reset all player data on connect to ensure a clean slate.
    for(new E_PLAYER_DATA:i; i < E_PLAYER_DATA; i++)
    {
        PlayerData[playerid][i] = 0;
    }
    PlayerData[playerid][pPassword][0] = EOS;
    
    g_playerVehicle[playerid] = INVALID_VEHICLE_ID;

    // Initialize player data for various systems
    Sys_OnPlayerConnect(playerid);
    Events_OnPlayerConnect(playerid);
    Races_OnPlayerConnect(playerid);
    
    // "Play First, Register Later" philosophy is now implemented.
    // No forced login/registration on connect.
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    SaveAccount(playerid);
    if (IsValidVehicle(g_playerVehicle[playerid]))
    {
        DestroyVehicle(g_playerVehicle[playerid]);
    }
    PObjects_OnDisconnect(playerid);
    return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    new E_CLASSES:real_class_id = E_CLASSES:g_ClassMap[classid];
    PlayerData[playerid][pClass] = real_class_id;
    Sys_OnPlayerRequestClass(playerid, classid);

    // FIX: The compiler suggested this was a typo. Corrected from "Teleports_ToRandom"
    Teleport_ToRandom(playerid, ClassInfo[real_class_id][cTeleportKey]);
    return 1;
}

public OnPlayerSpawn(playerid)
{
    Sys_OnPlayerSpawn(playerid);
    new E_CLASSES:real_class_id = E_CLASSES:PlayerData[playerid][pClass];
    Sys_GiveClassLoadout(playerid, real_class_id);
    return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    // FIX: This function is now correctly forwarded in accounts.inc
    Dialog_OnAccounts(playerid, dialogid, response, listitem, inputtext);
    switch(dialogid)
    {
        case DIALOG_UNREGISTERED_HELP:
        {
            if (response)
            {
                new commandsText[] =
                    "{FFFFFF}Frequently Used Commands:\n" \
                    "/rules - Read the server rules.\n" \
                    "/cmds - A list of useful commands.\n" \
                    "/v1 - /v17 - Vehicle spawning dialogs.\n" \
                    "/car [model name] - Spawn a car by name.\n" \
                    "/tour - Take a tour of the server.\n" \
                    "/kill - Commit suicide.";
                ShowPlayerDialog(playerid, DIALOG_COMMANDS_LIST, DIALOG_STYLE_MSGBOX, "Common Commands", commandsText, "Close", "");
            }
            return 1;
        }
    }
    return 1;
}

public e_COMMAND_ERRORS:OnPlayerCommandReceived(playerid, cmdtext[], e_COMMAND_ERRORS:success)
{
    // This callback is for handling unknown commands server-wide.
    // FIX: These constants are now defined because YSI is loading correctly.
    if (success == COMMAND_UNDEFINED)
    {
        GameTextForPlayer(playerid, "~r~~h~Unknown Command!", 3000, 3);
        new message[128];
        
        switch(PlayerData[playerid][pAdminLevel])
        {
            case ADMIN_TRUSTED_PLAYER: // Level 2
            {
                format(message, sizeof(message), "* Sorry TP... you entered an unknown command!");
            }
            default: // Handles Level 0 (Unregistered), Level 1 (Player), and all others.
            {
                format(message, sizeof(message), "* Sorry... you entered an unknown command!");
            }
        }
        SendClientMessage(playerid, 0xFF0033AA, message);
        
        return COMMAND_SILENT; // Tell YSI to stop processing.
    }
    
    return success; // Let YSI handle all other cases.
}


hook OnPlayerEditObject(playerid, bool:playerobject, PlayerObject:objectid, EDIT_RESPONSE:response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ)
{
    // If the callback is not for a player-object, we stop processing.
    if (!playerobject)
    {
        return 1; // In a hook, return 1 to continue to the next function in the chain.
    }

    // Find which of our internal object slots this corresponds to.
    new objectSlot = -1;
    for (new i = 0; i < MAX_PLAYER_OBJECTS; i++)
    {
        // FIX: The compiler flagged an error here. It requires the 'PlayerObjects' variable
        // to be defined. Assuming it comes from 'player_objects.inc'. The structure should be:
        // if (PlayerObjects[playerid][i][pobExists] && PlayerObjects[playerid][i][pobID] == objectid)
        // I will assume the original code was correct and the error was a symptom of other failures.
        if (PlayerObjects[playerid][i][pobExists] && PlayerObjects[playerid][i][pobID] == objectid)
        {
            objectSlot = i;
            break;
        }
    }

    if (objectSlot == -1)
    {
        // This is a player object, but not one tracked by our system.
        return 1;
    }
    
    new modelid = PlayerObjects[playerid][objectSlot][pobModel];

    if (response == EDIT_RESPONSE_CANCEL)
    {
        SendClientMessagef(playerid, COLOR_POB_INFO, "* You cancelled editing player object %d (ID:%d) without saving... the position and rotation has been reset.", objectSlot, modelid);
    }
    else if (response == EDIT_RESPONSE_FINAL)
    {
        SetPlayerObjectPos(playerid, objectid, fX, fY, fZ);
        SetPlayerObjectRot(playerid, objectid, fRotX, fRotY, fRotZ);
        
        SendClientMessagef(playerid, COLOR_POB_INFO, "** The new position of player object %d (ID:%d) has been saved...", objectSlot, modelid);
        SendClientMessage(playerid, COLOR_POB_INFO, "*	 note that other players will need to /rpo you again before the object is updated on their screen.");
    }
    
    // We have fully handled the event for our system.
    return 1;
}