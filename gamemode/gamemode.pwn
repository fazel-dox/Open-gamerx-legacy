/*
    ================================================================================
    Project:    Open GamerX - Base (Legacy)
    File:       gamemode.pwn (CORRECTED)
    Author:     Paniz & AI Assistant
    Description: The core file for the GamerX Rebirth project.
                 Initializes all systems and handles core callbacks.
    ================================================================================
*/

// ---===[ 1. LIBRARIES ]===---
#include <open.mp>
#include <a_mysql>
#include <sscanf2>

// *** FIX: We only need the y_commands hook for YCMD:y_unknown. ***
// We will not try to call non-existent functions.
#include <YSI/YSI_Visual/y_commands>

#include <samp_bcrypt>

// ---===[ 2. DEFINITIONS ]===---
#define DIALOG_LOGIN                1
#define DIALOG_REGISTER             2
#define DIALOG_RULES                3
#define DIALOG_UNREGISTERED_HELP    4
// *** ADDED: The actual dialog to show the list of common commands. ***
#define DIALOG_COMMANDS_LIST        5


// ---===[ 3. GLOBAL ENUMS & VARIABLES ]===---
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
new MySQL:g_dbConnection;


// ---===[ 4. SYSTEM INCLUDES ]===---
#include <accounts.inc>
#include <commands.inc>
#include <spawn_class_visual.inc>
#include <admin.inc>
#include <class_system.inc>
#include <player_objects.inc>

// Gamerx improved(the non legacy version)'s features, They will be moved to the non legacy branch after this project finishes/reaches to the public state. they're here for early testing.
#include <Non_legacy_experimental_features/Fly.inc>
#include <Non_legacy_experimental_features/Globalradio.inc>

public e_COMMAND_ERRORS:OnPlayerCommandReceived(playerid, cmdtext[], e_COMMAND_ERRORS:success)
{
    // Check if the reason this was called is specifically because the command was not found.
    if (success == COMMAND_UNDEFINED)
    {
        // The command was not found. Display our custom message.
        GameTextForPlayer(playerid, "~r~~h~Unknown Command!", 3000, 3);
        new message[128];
        
        switch(PlayerData[playerid][pAdminLevel])
        {
            case ADMIN_TRUSTED_PLAYER: // Level 2
            {
                // CORRECTED: Removed the random number
                format(message, sizeof(message), "* Sorry TP... you entered an unknown command!");
            }
            default: // Handles Level 0 (Unregistered), Level 1 (Player), and all others.
            {
                // CORRECTED: Removed the random number
                format(message, sizeof(message), "* Sorry... you entered an unknown command!");
            }
        }
        SendClientMessage(playerid, 0xFF0033AA, message);
        
        // IMPORTANT: Tell the YSI system to stop processing and say nothing further.
        return COMMAND_SILENT;
    }
    
    // For any other case, let the YSI system handle it as normal.
    return success;
}


// ---===[ 5. MAIN GAMEMODE LOGIC ]===---

public OnGameModeInit()
{
    print("\n------------------------------------");
    print(" GamerX Rebirth - Initializing...");
    print("------------------------------------");


    SetGameModeText("GamerX Rebirth");
    UsePlayerPedAnims();
    DisableInteriorEnterExits();
    EnableStuntBonusForAll(false);
    
    Classes_Init();

    g_dbConnection = mysql_connect("localhost", "root", "", "gamerx_db");
    if (!g_dbConnection)
    {
        print("[FATAL ERROR] Database connection failed. Shutting down.");
        SendRconCommand("exit");
        return 0;
    }
    print("[SUCCESS] Database connection established.");

    Sys_InitSpawn();
    print("[SUCCESS] Spawn System Initialized.");

    Teleports_LoadAll();
    
    print("------------------------------------");
    print(" GamerX Rebirth is now running.");
    print("------------------------------------\n");
    return 1;
}

public OnPlayerConnect(playerid)
{
    // Reset all player data on connect to ensure a clean slate.
    // pAdminLevel and pLoggedIn will correctly default to 0.
    for(new E_PLAYER_DATA:i; i < E_PLAYER_DATA; i++)
    {
        PlayerData[playerid][i] = 0;
    }
    PlayerData[playerid][pPassword][0] = EOS;
    
    g_playerVehicle[playerid] = INVALID_VEHICLE_ID;

    // Show the welcome TextDraws
    Sys_OnPlayerConnect(playerid);
    
    // The forced LoadAccount(playerid); call has been REMOVED.
    // Players can now join and play immediately.
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
    Dialog_OnAccounts(playerid, dialogid, response, listitem, inputtext);
    switch(dialogid)
    {
        case DIALOG_UNREGISTERED_HELP:
        {
            if (response)
            {
                // *** THE FIX: Instead of calling a command, we now show the commands dialog directly. ***
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