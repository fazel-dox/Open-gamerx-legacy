/*
    ================================================================================
    Project:    GamerX Legacy - Race System
    File:       races.inc
    Description: Handles all logic for the GamerX Race System.
    ================================================================================
*/

#if defined _races_included
    #endinput
#endif
#define _races_included

// ---===[ 1. DEFINITIONS & ENUMS ]===---
#define COLOR_RACE_ANNOUNCE  0xC3C436AA
#define COLOR_RACE_INFO      0xDCDE3DAA
#define COLOR_RACE_JOIN      0xC3C436AA
#define COLOR_RACE_WARNING   0xFF0033AA
#define COLOR_RACE_FINISH    0x7FAAFF

enum E_RACE_STATE
{
    RACE_STATE_NONE,      // No race is active
    RACE_STATE_WAITING,   // A race has been loaded and is waiting for players
    RACE_STATE_COUNTDOWN, // The race countdown has begun
    RACE_STATE_RUNNING    // The race is in progress
}

enum E_PLAYER_RACE_DATA
{
    pInRace,
    pCurrentCheckpoint,
    Float:pRaceStartTime
}

// ---===[ 2. GLOBAL VARIABLES ]===---

new g_RaceState = RACE_STATE_NONE;
new PlayerRaceData[MAX_PLAYERS][E_PLAYER_RACE_DATA];
new Text:g_JoinRaceTD;

// ---===[ 3. CORE COMMANDS (PLAYER) ]===---

YCMD:join(playerid, params[], help)
{
    // Placeholder logic until a race is loaded by an admin
    if (g_RaceState != RACE_STATE_WAITING)
    {
        return SendClientMessage(playerid, COLOR_RACE_WARNING, "* There is no race to join right now.");
    }

    // Actual join logic will be implemented here.
    // This will check for the required vehicle, set player state, teleport, etc.
    SendClientMessage(playerid, COLOR_RACE_JOIN, "** You joined the race..."); // Placeholder message
    PlayerRaceData[playerid][pInRace] = true;

    // From RPC Log: Display a GameText message confirming the join
    GameTextForPlayer(playerid, "~g~~h~You Joined the Race~n~~w~", 8000, 3);
    return 1;
}

YCMD:leave(playerid, params[], help)
{
    if (!PlayerRaceData[playerid][pInRace])
    {
        return SendClientMessage(playerid, COLOR_RACE_WARNING, "* You are not currently in a race.");
    }

    // Logic to remove player from race, reset their state, and teleport them back.
    PlayerRaceData[playerid][pInRace] = false;
    DisablePlayerRaceCheckpoint(playerid);
    SendClientMessage(playerid, COLOR_RACE_INFO, "* You have left the race.");
    return 1;
}

YCMD:raceparticipants(playerid, params[], help)
{
    // Aliases: /racers, /rp, /rps
    if (g_RaceState == RACE_STATE_NONE)
    {
        return SendClientMessage(playerid, COLOR_RACE_INFO, "* There is no active race.");
    }
    // Logic to build and show a dialog with all players where PlayerRaceData[playerid][pInRace] is true.
    SendClientMessage(playerid, 0xFFFFFFFF, "COMMAND PREVIEW: This will show a list of race participants.");
    return 1;
}
// FIX: Using the correct 'CMD_ALIAS' macro for YSI aliases.
CMD_ALIAS:racers(raceparticipants)(playerid, params[], help);
CMD_ALIAS:rp(raceparticipants)(playerid, params[], help);
CMD_ALIAS:rps(raceparticipants)(playerid, params[], help);


YCMD:raceinfo(playerid, params[], help)
{
    // Alias: /ri
    if (g_RaceState == RACE_STATE_NONE)
    {
        return SendClientMessage(playerid, COLOR_RACE_INFO, "* There is no active race.");
    }
    // Logic to show information about the currently loaded race.
    SendClientMessage(playerid, 0xFFFFFFFF, "COMMAND PREVIEW: This will show info on the current race.");
    return 1;
}
// FIX: Using the correct 'CMD_ALIAS' macro for YSI aliases.
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
    // Alias: /raceadminhelp
    if (PlayerData[playerid][pAdminLevel] < ADMIN_STANDARD) // Level 4+
    {
        return SendClientMessage(playerid, COLOR_SERVER_ALERT, "ERROR: You are not authorized to use this command.");
    }
    // Logic to show a detailed admin help dialog.
    new helpText[] =
        "{FFFFFF}GamerX Race Admin Help:\n\n" \
        "This dialog will contain commands for loading, starting,\n" \
        "and managing races, such as /loadrace, /startrace etc.";
    ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, "Race Admin Help", helpText, "Close", "");
    return 1;
}
// FIX: Using the correct 'CMD_ALIAS' macro for YSI aliases.
CMD_ALIAS:raceadminhelp(aracehelp)(playerid, params[], help);


YCMD:buildhelp(playerid, params[], help)
{
    // Alias: /buildracehelp
    if (PlayerData[playerid][pAdminLevel] < ADMIN_STANDARD) // Level 4+
    {
        return SendClientMessage(playerid, COLOR_SERVER_ALERT, "ERROR: You are not authorized to use this command.");
    }
    // Logic to show a detailed race building help dialog.
    new helpText[] =
        "{FFFFFF}GamerX Race Building Help:\n\n" \
        "1. Use /cpo to create player objects for checkpoints.\n" \
        "2. The first object placed is the starting grid.\n" \
        "3. The last object is the finish line.\n" \
        "4. Use /saverace [Name] [RequiredVehicle] to save it.";
    ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, "Race Building Help", helpText, "Close", "");
    return 1;
}
// FIX: Using the correct 'CMD_ALIAS' macro for YSI aliases.
CMD_ALIAS:buildracehelp(buildhelp)(playerid, params[], help);


// ---===[ 5. SYSTEM FUNCTIONS & CALLBACKS ]===---

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
    // From RPC Log: Create the global "Type /join" TextDraw, but keep it hidden initially.
    g_JoinRaceTD = TextDrawCreate(400.0, 15.0, "~h~~h~Type ~g~/join ~w~~h~~h~to join the race!");
    TextDrawUseBox(g_JoinRaceTD, true);
    TextDrawBoxColor(g_JoinRaceTD, 0x95542400);
    TextDrawBackgroundColor(g_JoinRaceTD, 0xFF000000);
    TextDrawFont(g_JoinRaceTD, 2);
    TextDrawLetterSize(g_JoinRaceTD, 0.5, 2.0);
    TextDrawColor(g_JoinRaceTD, 0xFFE1E1E1);
    TextDrawSetOutline(g_JoinRaceTD, 1);
    TextDrawTextSize(g_JoinRaceTD, 630.0, 0.0); // Set size of the box
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
        return 0; // Not a race system checkpoint, let other scripts handle it.
    }

    // This is where the core race logic will go.
    // - Check if it's the finish line.
    // - If not, set the next checkpoint in the sequence.
    // - Play sound from RPC log (ID: 1138)
    PlayerPlaySound(playerid, 1138, 0.0, 0.0, 0.0);

    PlayerRaceData[playerid][pCurrentCheckpoint]++;

    // Placeholder for setting the next checkpoint
    // SetPlayerRaceCheckpoint(...);

    return 1; // We handled this callback.
}