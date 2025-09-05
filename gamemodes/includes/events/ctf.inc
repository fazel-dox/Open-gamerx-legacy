/*
    ================================================================================
    Project:    GamerX Rebirth - Event System (CTF Module)
    File:       ctf.inc
    Author:     Paniz & AI Assistant
    Description: Handles all logic for Capture The Flag events. Loaded by events.inc.
    ================================================================================
*/

#if defined _events_ctf_included
    #endinput
#endif
#define _events_ctf_included

// Forward for the CTF-specific MySQL callback
forward OnEventLoad_CTF(loadedByAdminID, Cache:cache_id, const mapName[]);

// This is the implementation of the function forwarded in events.inc
// It gets called by the master callback and decides which sub-module to use.
public Events_LoadModule(eventType[], mapName[], loadedByAdminID)
{
    if (strcmp(eventType, "CTF", true) == 0)
    {
        static query[256];
        mysql_format(g_dbConnection, query, sizeof(query), "SELECT * FROM `events_ctf` WHERE `MapName` = '%e' LIMIT 1", mapName);
        mysql_tquery(g_dbConnection, query, "OnEventLoad_CTF", "is", loadedByAdminID, mapName);
    }
    // else if (strcmp(eventType, "TDM", true) == 0) { ... load TDM module ... }
    
    return 1;
}


// The callback for when the CTF map data has been fetched from the DB
public OnEventLoad_CTF(loadedByAdminID, Cache:cache_id, const mapName[])
{
    if (!cache_get_row_count())
    {
        if(loadedByAdminID != 1111) SendClientMessagef(loadedByAdminID, COLOR_EVENT_CANCEL, "* CRITICAL ERROR: Map '%s' desynced! Found in 'events' but not 'events_ctf'.", mapName);
        return 1;
    }

    // Set global event state
    g_EventState = EVENT_STATE_WAITING;
    strcopy(g_LoadedEventMapName, mapName, 64);
    strcopy(g_LoadedEventType, "CTF", 16);

    // TODO: Load all the CTF coords (flag positions, spawns) from the cache into global variables.
    // e.g., g_CTF_RedFlagPos[0] = cache_get_value_name_float(0, "RedFlagX");

    // Announce the event based on the RPC logs
    new announceMsg[128];
    format(announceMsg, sizeof(announceMsg), "** A Capture The Flag (CTF) Event is about to start, type {FFFFFF}/E{CCCCCC}VENT{FF6699} to join.");
    SendClientMessageToAll(COLOR_EVENT_ANNOUNCE, announceMsg);
    
    TextDrawShowForAll(g_JoinEventTD);
    
    if (loadedByAdminID != 1111)
    {
        SendClientMessagef(loadedByAdminID, COLOR_ADMIN_ACTION, "* CTF event '%s' loaded successfully.", mapName);
        SendClientMessage(loadedByAdminID, COLOR_PLACEHOLDER, MSG_PLACEHOLDER);
    }
    // TODO: A log message to admins/IRC would go here, using the 'loadedByAdminID'
    return 1;
}