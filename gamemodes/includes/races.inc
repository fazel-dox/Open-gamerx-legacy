/*
    ================================================================================
    Project:    GamerX Rebirth - Race System
    File:       races.inc
    Author:     Paniz & AI Assistant
    Description: Handles all logic for the GamerX Race System.
    ================================================================================
*/

#if defined _races_included
    #endinput
#endif
#define _races_included

// We need these for the YSI commands and other functions
#include <open.mp>
#include <YSI_Visual/y_commands>


// ---===[ 1. DEFINITIONS & ENUMS ]===---

// Some colors for the race messages
#define COLOR_RACE_ANNOUNCE  0xC3C436AA
#define COLOR_RACE_INFO      0xDCDE3DAA
#define COLOR_RACE_JOIN      0xC3C436AA
#define COLOR_RACE_WARNING   0xFF0033AA
#define COLOR_RACE_FINISH    0x7FAAFF
#define COLOR_ADMIN_ACTION   0xC2A2DAFF

// for stuff we haven't confirmed from the original GX yet
#define COLOR_PLACEHOLDER    0x9E9E9EAA
#define MSG_PLACEHOLDER      "[Placeholder message. Details to be confirmed by community contribution.]"

// Dont let admins go crazy with checkpoints
#define MAX_RACE_CHECKPOINTS 100


// keeps track of where the race is at globally
enum E_RACE_STATE
{
    RACE_STATE_NONE,
    RACE_STATE_WAITING,
    RACE_STATE_COUNTDOWN,
    RACE_STATE_RUNNING
}

// player-specific race data
enum E_PLAYER_RACE_DATA
{
    pInRace,
    pCurrentCheckpoint,
    Float:pRaceStartTime
}

// for storing the loaded chekcpoint coords
enum E_CHECKPOINT_DATA
{
    Float:cpX,
    Float:cpY,
    Float:cpZ
}


// ---===[ 2. GLOBAL VARIABLES ]===---

new g_RaceState = RACE_STATE_NONE;
new PlayerRaceData[MAX_PLAYERS][E_PLAYER_RACE_DATA];
new Text:g_JoinRaceTD;

// Holds the currently active race's info
new g_LoadedRaceName[64];
new g_LoadedRaceVehicleID;
new g_LoadedRaceCheckpointCount;
new g_LoadedRaceCheckpoints[MAX_RACE_CHECKPOINTS][E_CHECKPOINT_DATA];

// Forward for our mysql callback
forward OnRaceLoad(playerid, Cache:cache_id);


// ---===[ 3. CORE COMMANDS (PLAYER) ]===---

YCMD:join(playerid, params[], help)
{
    if (g_RaceState != RACE_STATE_WAITING)
    {
        return SendClientMessage(playerid, COLOR_RACE_WARNING, "* There is no race to join right now.");
    }

    // TODO: Add vehicle checks later on
    SendClientMessage(playerid, COLOR_RACE_JOIN, "** You joined the race...");
    PlayerRaceData[playerid][pInRace] = true;

    GameTextForPlayer(playerid, "~g~~h~You Joined the Race~n~~w~", 8000, 3);
    return 1;
}

YCMD:leave(playerid, params[], help)
{
    if (!PlayerRaceData[playerid][pInRace])
    {
        return SendClientMessage(playerid, COLOR_RACE_WARNING, "* You are not currently in a race.");
    }

    PlayerRaceData[playerid][pInRace] = false;
    DisablePlayerRaceCheckpoint(playerid);
    SendClientMessage(playerid, COLOR_RACE_INFO, "* You have left the race.");
    return 1;
}

YCMD:raceparticipants(playerid, params[], help)
{
    if (g_RaceState == RACE_STATE_NONE)
    {
        return SendClientMessage(playerid, COLOR_RACE_INFO, "* There is no active race.");
    }
    SendClientMessage(playerid, 0xFFFFFFFF, "COMMAND PREVIEW: This will show a list of race participants.");
    return 1;
}
// Set up the command aliases
CMD_ALIAS:racers(raceparticipants)(playerid, params[], help);
CMD_ALIAS:rp(raceparticipants)(playerid, params[], help);
CMD_ALIAS:rps(raceparticipants)(playerid, params[], help);


YCMD:raceinfo(playerid, params[], help)
{
    if (g_RaceState == RACE_STATE_NONE)
    {
        return SendClientMessage(playerid, COLOR_RACE_INFO, "* There is no active race.");
    }
    SendClientMessage(playerid, 0xFFFFFFFF, "COMMAND PREVIEW: This will show info on the current race.");
    return 1;
}
CMD_ALIAS:ri(raceinfo)(playerid, params[], help);

YCMD:racehelp(playerid, params[], help)
{
    new helpText[] =
        "{FFFFFF}GamerX Race Help:\n\n" \
        "/join - Joins the current race if one is starting.\n" \
        "/leave - Leaves the race you are currently in.\n" \
        "/lcp - Teleports you to your last checkpoint if you get stuck.\n" \
        "/racers - Shows who is participating in the current race.\n" \
        "/raceinfo - Displays information about the current race track.";
    ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, "Race Commands Help", helpText, "Close", "");
    return 1;
}

// ---===[ 4. CORE COMMANDS (ADMIN) ]===---

YCMD:aracehelp(playerid, params[], help)
{
    if (PlayerData[playerid][pAdminLevel] < ADMIN_STANDARD)
    {
        return SendClientMessage(playerid, COLOR_SERVER_ALERT, "ERROR: You are not authorized to use this command.");
    }

    new helpText[] =
        "{FFFFFF}GamerX Race Admin Help:\n\n" \
        "This dialog will contain commands for loading, starting,\n" \
        "and managing races, such as /loadrace, /startrace etc.";
    ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, "Race Admin Help", helpText, "Close", "");
    return 1;
}
CMD_ALIAS:raceadminhelp(aracehelp)(playerid, params[], help);


YCMD:buildhelp(playerid, params[], help)
{
    if (PlayerData[playerid][pAdminLevel] < ADMIN_STANDARD)
    {
        return SendClientMessage(playerid, COLOR_SERVER_ALERT, "ERROR: You are not authorized to use this command.");
    }

    new helpText[] =
        "{FFFFFF}GamerX Race Building Help:\n\n" \
        "1. Use /cpo to create player objects for checkpoints.\n" \
        "2. The first object placed is the starting grid.\n" \
        "3. The last object is the finish line.\n" \
        "4. Use /saverace [Name] [RequiredVehicle] to save it.";
    ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, "Race Building Help", helpText, "Close", "");
    return 1;
}
CMD_ALIAS:buildracehelp(buildhelp)(playerid, params[], help);


YCMD:saverace(playerid, params[], help)
{
    //if (PlayerData[playerid][pAdminLevel] < ADMIN_STANDARD)
    //{
        //return SendClientMessage(playerid, COLOR_SERVER_ALERT, "ERROR: You are not authorized to use this command.");
    //}

    new raceName[64], vehicleModelID;
    if (sscanf(params, "s[64]d", raceName, vehicleModelID) < 2)
    {
        return SendClientMessage(playerid, 0xFF9900FF, "USAGE: /saverace [RaceName] [VehicleModelID]");
    }

    static checkpointData[4096];
    checkpointData[0] = EOS;
    new checkpointCount = 0;
    new Float:x, Float:y, Float:z;

    for (new i = 0; i < MAX_PLAYER_OBJECTS; i++)
    {
        if (PlayerObjects[playerid][i][pobExists])
        {
            if (checkpointCount >= MAX_RACE_CHECKPOINTS) break;

            GetPlayerObjectPos(playerid, PlayerObjects[playerid][i][pobID], x, y, z);
            
            new temp[64];
            format(temp, sizeof(temp), "%.4f,%.4f,%.4f;", x, y, z);
            strcat(checkpointData, temp);
            checkpointCount++;
        }
    }

    if (checkpointCount == 0)
    {
        return SendClientMessage(playerid, COLOR_RACE_WARNING, "* You have not placed any player objects to save as a race.");
    }
    
    new adminName[MAX_PLAYER_NAME];
    GetPlayerName(playerid, adminName, sizeof(adminName));

    // *** FIX: Use mysql_format for a single, safe query operation ***
    static query[9000];
    mysql_format(g_dbConnection, query, sizeof(query), "INSERT INTO `races` (`Name`, `CreatorName`, `VehicleModelID`, `CheckpointCount`, `CheckpointsData`) VALUES ('%e', '%e', %d, %d, '%e')",
        raceName,
        adminName,
        vehicleModelID,
        checkpointCount,
        checkpointData
    );

    mysql_tquery(g_dbConnection, query);

    SendClientMessagef(playerid, COLOR_ADMIN_ACTION, "* Race '%s' has been saved to the database with %d checkpoints.", raceName, checkpointCount);
    SendClientMessage(playerid, COLOR_PLACEHOLDER, MSG_PLACEHOLDER);
    return 1;
}

YCMD:loadrace(playerid, params[], help)
{
    //if (PlayerData[playerid][pAdminLevel] < ADMIN_STANDARD)
    //{
        //return SendClientMessage(playerid, COLOR_SERVER_ALERT, "ERROR: You are not authorized to use this command.");
    //}
    
    new raceName[64];
    if (sscanf(params, "s[64]", raceName))
    {
        return SendClientMessage(playerid, 0xFF9900FF, "USAGE: /loadrace [RaceName]");
    }
    
    // *** FIX: Use mysql_format for a simpler, safer query ***
    static query[256];
    mysql_format(g_dbConnection, query, sizeof(query), "SELECT * FROM `races` WHERE `Name` = '%e' LIMIT 1", raceName);
    
    mysql_tquery(g_dbConnection, query, "OnRaceLoad", "i", playerid);

    SendClientMessagef(playerid, COLOR_ADMIN_ACTION, "* Attempting to load race '%s' from the database...", raceName);
    SendClientMessage(playerid, COLOR_PLACEHOLDER, MSG_PLACEHOLDER);
    return 1;
}

// ---===[ 5. SYSTEM FUNCTIONS & CALLBACKS ]===---

public OnRaceLoad(playerid, Cache:cache_id)
{
    if (!cache_get_row_count())
    {
        SendClientMessage(playerid, COLOR_RACE_WARNING, "* Race not found in the database.");
        return 1;
    }

    cache_get_value_name(0, "Name", g_LoadedRaceName, 64);
    g_LoadedRaceVehicleID = cache_get_value_name_int(0, "VehicleModelID");
    g_LoadedRaceCheckpointCount = cache_get_value_name_int(0, "CheckpointCount");

    static checkpointData[4096];
    cache_get_value_name(0, "CheckpointsData", checkpointData, sizeof(checkpointData));
    
    new currentPos = 0;
    for(new i = 0; i < g_LoadedRaceCheckpointCount; i++)
    {
        new nextPos = strfind(checkpointData, ";", false, currentPos);
        if (nextPos == -1) break;

        new checkpoint[64];
        strmid(checkpoint, checkpointData, currentPos, nextPos, sizeof(checkpoint));
        
        sscanf(checkpoint, "f,f,f", 
            g_LoadedRaceCheckpoints[i][cpX], 
            g_LoadedRaceCheckpoints[i][cpY], 
            g_LoadedRaceCheckpoints[i][cpZ]
        );

        currentPos = nextPos + 1;
    }

    g_RaceState = RACE_STATE_WAITING;

    new announceMsg[256];
    // Minor typo fix in color code
    format(announceMsg, sizeof(announceMsg), "** Race \"{FFFF00}%s{C3C436}\" is about to start, type {FFFFFF}/JOIN{C3C436} to join the race!", g_LoadedRaceName);
    SendClientMessageToAll(COLOR_RACE_ANNOUNCE, announceMsg);
    
    TextDrawShowForAll(g_JoinRaceTD);

    SendClientMessagef(playerid, COLOR_ADMIN_ACTION, "* Race '%s' loaded successfully with %d checkpoints.", g_LoadedRaceName, g_LoadedRaceCheckpointCount);
    SendClientMessage(playerid, COLOR_PLACEHOLDER, MSG_PLACEHOLDER);
    return 1;
}

stock Races_OnPlayerConnect(playerid)
{
    for(new E_PLAYER_RACE_DATA:i; i < E_PLAYER_RACE_DATA; i++)
    {
        PlayerRaceData[playerid][i] = 0;
    }
    PlayerRaceData[playerid][pRaceStartTime] = 0.0;
    return 1;
}

public OnGameModeInit_Races()
{
    g_JoinRaceTD = TextDrawCreate(400.0, 15.0, "~h~~h~Type ~g~/join ~w~~h~~h~to join the race!");
    TextDrawUseBox(g_JoinRaceTD, true);
    TextDrawBoxColor(g_JoinRaceTD, 0x95542400);
    TextDrawBackgroundColor(g_JoinRaceTD, 0xFF000000);
    TextDrawFont(g_JoinRaceTD, 2);
    TextDrawLetterSize(g_JoinRaceTD, 0.5, 2.0);
    TextDrawColor(g_JoinRaceTD, 0xFFE1E1E1);
    TextDrawSetOutline(g_JoinRaceTD, 1);
    TextDrawTextSize(g_JoinRaceTD, 630.0, 0.0);
    print("[SUCCESS] Race System Initialized.");
    return 1;
}

public OnGameModeExit_Races()
{
    TextDrawDestroy(g_JoinRaceTD);
    return 1;
}

hook OnPlayerEnterRaceCheckpoint(playerid)
{
    if (!PlayerRaceData[playerid][pInRace])
    {
        return 0;
    }

    PlayerPlaySound(playerid, 1138, 0.0, 0.0, 0.0);

    PlayerRaceData[playerid][pCurrentCheckpoint]++;
    return 1;
}