/*
    ================================================================================
    Project:    GamerX Rebirth - Event System (Core Framework)
    File:       events.inc
    Author:     Paniz & AI Assistant
    Description: The central manager for all server events. This script loads the
                 correct event module (CTF, TDM, etc.) based on DB info.
    ================================================================================
*/

#if defined _events_included
    #endinput
#endif
#define _events_included

#include "core.inc" // Include the new foundation

// ---===[ 1. MODULE INCLUDES ]===---
// MOVED TO THE END OF THE FILE to ensure definitions are loaded first.


// ---===[ 2. DEFINITIONS & ENUMS ]===---

#define COLOR_EVENT_ANNOUNCE 0xFF6699AA
#define COLOR_EVENT_CANCEL   0xFF0033AA
#define COLOR_ADMIN_ACTION   0xC2A2DAFF

#define COLOR_PLACEHOLDER    0x9E9E9EAA
#define MSG_PLACEHOLDER      "[Placeholder message. Details to be confirmed by community contribution.]"

enum E_EVENT_STATE
{
    EVENT_STATE_NONE,
    EVENT_STATE_WAITING,
    EVENT_STATE_RUNNING
}

enum E_PLAYER_EVENT_DATA
{
    pInEvent,
    pEventTeam
}

// ---===[ 3. GLOBAL VARIABLES & FORWARDS ]===---

new g_EventState = EVENT_STATE_NONE;
new PlayerEventData[MAX_PLAYERS][E_PLAYER_EVENT_DATA];
new Text:g_JoinEventTD;

new g_LoadedEventMapName[64];
new g_LoadedEventType[16];

// The master callback for the 'events' table
forward OnEventLoad_Master(loadedByAdminID, Cache:cache_id, const mapName[]);

// These functions MUST be defined in the module files (e.g., ctf.inc)
forward Events_LoadModule(eventType[], mapName[], loadedByAdminID);


// ---===[ 4. CORE LOGIC & COMMANDS ]===---

// This is the new core function. It can be called by an admin command OR an auto-timer.
// The 'loadedByAdminID' tells us who (or what) started the event.
stock Events_LoadMap(const mapName[], loadedByAdminID)
{
    static query[256];
    mysql_format(g_dbConnection, query, sizeof(query), "SELECT `EventType`, `CreatorName` FROM `events` WHERE `MapName` = '%e' LIMIT 1", mapName);
    
    // Pass the admin ID and map name to the callback
    mysql_tquery(g_dbConnection, query, "OnEventLoad_Master", "is", loadedByAdminID, mapName);
    
    if (loadedByAdminID != 1111) // Don't spam the console for the autorotation
    {
        SendClientMessagef(loadedByAdminID, COLOR_ADMIN_ACTION, "* Looking up event map '%s'...", mapName);
    }
}

// The admin command is now just a simple wrapper around the core function.
YCMD:loadevent(playerid, params[], help)
{
    if (PlayerData[playerid][pAdminLevel] < ADMIN_STANDARD)
    {
        return SendClientMessage(playerid, COLOR_SERVER_ALERT, "ERROR: You are not authorized to use this command.");
    }

    new mapName[64];
    if (sscanf(params, "s[64]", mapName))
    {
        return SendClientMessage(playerid, 0xFF9900FF, "USAGE: /loadevent [MapName]");
    }
    
    Events_LoadMap(mapName, playerid);
    return 1;
}

YCMD:event(playerid, params[], help)
{
    if (g_EventState != EVENT_STATE_WAITING)
    {
        return SendClientMessage(playerid, COLOR_EVENT_CANCEL, "* There is no event to join right now.");
    }
    
    SendClientMessage(playerid, COLOR_EVENT_ANNOUNCE, "* You have joined the event!");
    PlayerEventData[playerid][pInEvent] = true;
    return 1;
}

// ---===[ 5. CALLBACKS & SYSTEM FUNCTIONS ]===---

// The master callback. Its only job is to route the request to the correct module.
public OnEventLoad_Master(loadedByAdminID, Cache:cache_id, const mapName[])
{
    if (!cache_get_row_count())
    {
        if(loadedByAdminID != 1111) SendClientMessagef(loadedByAdminID, COLOR_EVENT_CANCEL, "* Event map '%s' not found.", mapName);
        return 1;
    }

    new eventType[16], creatorName[24];
    cache_get_value_name(0, "EventType", eventType, sizeof(eventType));
    cache_get_value_name(0, "CreatorName", creatorName, sizeof(creatorName));

    if(loadedByAdminID != 1111) SendClientMessagef(loadedByAdminID, COLOR_ADMIN_ACTION, "* Map found. Type: %s. Forwarding to the correct event module...", eventType);
    
    // Call the forwarded function which will exist in one of the module includes
    Events_LoadModule(eventType, mapName, loadedByAdminID);
    return 1;
}

stock Events_OnPlayerConnect(playerid)
{
    for(new E_PLAYER_EVENT_DATA:i; i < E_PLAYER_EVENT_DATA; i++) PlayerEventData[playerid][i] = 0;
    return 1;
}

public OnGameModeInit_Events()
{
    g_JoinEventTD = TextDrawCreate(400.0, 1.0, "Type ~y~/event ~w~to join the event");
    // ... TextDraw settings ...
    print("[SUCCESS] Event System Initialized.");
    return 1;
}

public OnGameModeExit_Events()
{
    TextDrawDestroy(g_JoinEventTD);
    return 1;
}

// ---===[ 6. LATE-LOAD MODULE INCLUDES ]===---
// Each specific event type gets its own module file.
// This is included LAST to ensure the module has access to all framework definitions.
#include "includes/events/ctf.inc"