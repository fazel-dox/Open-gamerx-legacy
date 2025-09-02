/*
    ================================================================================
    Project:    GamerX Rebirth - Spawn System(visuals) 
    File:       spawn_system.inc
    Description: Handles the visuals and the texts in spawn selection after a player joins.
    ================================================================================
*/

// ---===[ 1. VARIABLES & FORWARDS ]===---

// *** FIX: Changed all TextDraw variables to PlayerTextDraw arrays. ***
// This gives every player their own separate set of TextDraws.
new PlayerTextDraw: g_ptdWelcomeBox[MAX_PLAYERS];
new PlayerTextDraw: g_ptdWelcomeLogo[MAX_PLAYERS];
new PlayerTextDraw: g_ptdWelcomeURL[MAX_PLAYERS];
new PlayerTextDraw: g_ptdWelcomeTitle[MAX_PLAYERS];
new PlayerTextDraw: g_ptdWelcomeHelp[MAX_PLAYERS];
new PlayerTextDraw: g_ptdWelcomeCheatWarning[MAX_PLAYERS];
new PlayerTextDraw: g_ptdWelcomeVersion[MAX_PLAYERS];
new PlayerTextDraw: g_ptdWelcomeCommands[MAX_PLAYERS];
new PlayerTextDraw: g_ptdWelcomeTips[MAX_PLAYERS];

// Forward declarations for functions within this file.
forward Internal_SetPlayerSpawnScreen(playerid);
forward Internal_DestroyWelcomeTextDraws(playerid);


// ---===[ 2. CORE FUNCTIONS ]===---

// Called from OnGameModeInit
public Sys_InitSpawn()
{
    // *** FIX: No longer needed. TextDraws are now created when a player connects, not when the server starts. ***
    print("[SPAWN SYSTEM] Ready. Player TextDraws will be created on demand.");
    return 1;
}

// Called from OnPlayerConnect
public Sys_OnPlayerConnect(playerid)
{
    // *** FIX: Create a unique set of TextDraws for the connecting player. ***
    g_ptdWelcomeBox[playerid] = CreatePlayerTextDraw(playerid, 150.0, 142.0, "_");
    PlayerTextDrawLetterSize(playerid, g_ptdWelcomeBox[playerid], 0.0, 0.0);
    PlayerTextDrawUseBox(playerid, g_ptdWelcomeBox[playerid], true);
    PlayerTextDrawBoxColor(playerid, g_ptdWelcomeBox[playerid], 0x0000AA99);
    PlayerTextDrawSetShadow(playerid, g_ptdWelcomeBox[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_ptdWelcomeBox[playerid], 0);
    PlayerTextDrawTextSize(playerid, g_ptdWelcomeBox[playerid], 490.0, 0.0);

    g_ptdWelcomeLogo[playerid] = CreatePlayerTextDraw(playerid, 320.0, 145.0, "GAMERX");
    PlayerTextDrawFont(playerid, g_ptdWelcomeLogo[playerid], 2);
    PlayerTextDrawLetterSize(playerid, g_ptdWelcomeLogo[playerid], 0.9, 3.8);
    PlayerTextDrawColor(playerid, g_ptdWelcomeLogo[playerid], 0xFFDD00AA);
    PlayerTextDrawSetProportional(playerid, g_ptdWelcomeLogo[playerid], false);
    PlayerTextDrawAlignment(playerid, g_ptdWelcomeLogo[playerid], 2);

    g_ptdWelcomeURL[playerid] = CreatePlayerTextDraw(playerid, 320.0, 175.0, "HTTP://GAMERXSERVER.COM"); PlayerTextDrawFont(playerid, g_ptdWelcomeURL[playerid], 2); PlayerTextDrawLetterSize(playerid, g_ptdWelcomeURL[playerid], 0.3, 1.2); PlayerTextDrawColor(playerid, g_ptdWelcomeURL[playerid], 0xFFDD00AA); PlayerTextDrawSetProportional(playerid, g_ptdWelcomeURL[playerid], false); PlayerTextDrawAlignment(playerid, g_ptdWelcomeURL[playerid], 2);
    g_ptdWelcomeTitle[playerid] = CreatePlayerTextDraw(playerid, 320.0, 190.0, "WELCOME TO GAMERX VERSION 4!"); PlayerTextDrawFont(playerid, g_ptdWelcomeTitle[playerid], 2); PlayerTextDrawLetterSize(playerid, g_ptdWelcomeTitle[playerid], 0.4, 1.5); PlayerTextDrawColor(playerid, g_ptdWelcomeTitle[playerid], 0x99FF33AA); PlayerTextDrawSetProportional(playerid, g_ptdWelcomeTitle[playerid], false); PlayerTextDrawAlignment(playerid, g_ptdWelcomeTitle[playerid], 2);
    g_ptdWelcomeHelp[playerid] = CreatePlayerTextDraw(playerid, 320.0, 210.0, "Type /help or /commands to learn more~n~about our server. Use the /v1 to /v17~n~commands to spawn a vehicle."); PlayerTextDrawFont(playerid, g_ptdWelcomeHelp[playerid], 1); PlayerTextDrawLetterSize(playerid, g_ptdWelcomeHelp[playerid], 0.25, 1.1); PlayerTextDrawColor(playerid, g_ptdWelcomeHelp[playerid], 0xFFFFFFAA); PlayerTextDrawSetProportional(playerid, g_ptdWelcomeHelp[playerid], true); PlayerTextDrawAlignment(playerid, g_ptdWelcomeHelp[playerid], 2);
    g_ptdWelcomeCheatWarning[playerid] = CreatePlayerTextDraw(playerid, 320.0, 245.0, "CHEAT HERE AND YOU WILL BE KICKED OR BANNED."); PlayerTextDrawFont(playerid, g_ptdWelcomeCheatWarning[playerid], 2); PlayerTextDrawLetterSize(playerid, g_ptdWelcomeCheatWarning[playerid], 0.35, 1.3); PlayerTextDrawColor(playerid, g_ptdWelcomeCheatWarning[playerid], 0xFF0033AA); PlayerTextDrawSetProportional(playerid, g_ptdWelcomeCheatWarning[playerid], false); PlayerTextDrawAlignment(playerid, g_ptdWelcomeCheatWarning[playerid], 2);
    g_ptdWelcomeVersion[playerid] = CreatePlayerTextDraw(playerid, 320.0, 260.0, "V4.12.2 - MAY 2022~N~BY THE GAMERX CREW"); PlayerTextDrawFont(playerid, g_ptdWelcomeVersion[playerid], 2); PlayerTextDrawLetterSize(playerid, g_ptdWelcomeVersion[playerid], 0.35, 1.3); PlayerTextDrawColor(playerid, g_ptdWelcomeVersion[playerid], 0xFF9900AA); PlayerTextDrawSetProportional(playerid, g_ptdWelcomeVersion[playerid], false); PlayerTextDrawAlignment(playerid, g_ptdWelcomeVersion[playerid], 2);
    g_ptdWelcomeCommands[playerid] = CreatePlayerTextDraw(playerid, 320.0, 425.0, "-=- /c for Commands -=- Type /t for Teleports -=- /v1 to /v17 or /car to Spawn Vehicles -=-"); PlayerTextDrawFont(playerid, g_ptdWelcomeCommands[playerid], 0); PlayerTextDrawLetterSize(playerid, g_ptdWelcomeCommands[playerid], 0.22, 1.0); PlayerTextDrawColor(playerid, g_ptdWelcomeCommands[playerid], 0xE1E1E1FF); PlayerTextDrawSetProportional(playerid, g_ptdWelcomeCommands[playerid], true); PlayerTextDrawAlignment(playerid, g_ptdWelcomeCommands[playerid], 2);
    g_ptdWelcomeTips[playerid] = CreatePlayerTextDraw(playerid, 320.0, 435.0, "Use the /drinkbeer or /wine commands to hold a beer or wine bottle."); PlayerTextDrawFont(playerid, g_ptdWelcomeTips[playerid], 0); PlayerTextDrawLetterSize(playerid, g_ptdWelcomeTips[playerid], 0.22, 1.0); PlayerTextDrawColor(playerid, g_ptdWelcomeTips[playerid], 0xE1E1E1FF); PlayerTextDrawSetProportional(playerid, g_ptdWelcomeTips[playerid], true); PlayerTextDrawAlignment(playerid, g_ptdWelcomeTips[playerid], 2);

    // Now, show the unique TextDraws to the player
    PlayerTextDrawShow(playerid, g_ptdWelcomeBox[playerid]);
    PlayerTextDrawShow(playerid, g_ptdWelcomeLogo[playerid]);
    PlayerTextDrawShow(playerid, g_ptdWelcomeURL[playerid]);
    PlayerTextDrawShow(playerid, g_ptdWelcomeTitle[playerid]);
    PlayerTextDrawShow(playerid, g_ptdWelcomeHelp[playerid]);
    PlayerTextDrawShow(playerid, g_ptdWelcomeCheatWarning[playerid]);
    PlayerTextDrawShow(playerid, g_ptdWelcomeVersion[playerid]);
    PlayerTextDrawShow(playerid, g_ptdWelcomeCommands[playerid]);
    PlayerTextDrawShow(playerid, g_ptdWelcomeTips[playerid]);
    return 1;
}

// Called from OnPlayerRequestClass
public Sys_OnPlayerRequestClass(playerid, classid)
{
    // This timer is delayed to ensure the player has fully connected before we teleport them.
    SetTimer("Internal_SetPlayerSpawnScreen", 100, false, "i", playerid);
    return 1;
}

public Internal_SetPlayerSpawnScreen(playerid)
{
    // Your original logic here was correct.
    SendClientMessage(playerid, 0xFFFF00AA, "Anim System by [VLA]Wooz1e Loaded: type /animhelp to view the list."); SendClientMessage(playerid, 0xFFDD00AA, "* Your client is outdated. Please update your SA-MP client to to 0.3.7-R5 as soon as possible!"); SendClientMessage(playerid, 0xFFDD00AA, "* You can download San Andreas Multiplayer client at {FFFFFF}www.sa-mp.com");
    new oldName[MAX_PLAYER_NAME], newName[MAX_PLAYER_NAME], string[128]; GetPlayerName(playerid, oldName, sizeof(oldName));
    new bool:db_connection_failed = true; if (db_connection_failed) { format(newName, sizeof(newName), "GXTempName%d", playerid); SetPlayerName(playerid, newName); format(string, sizeof(string), "** Your player name %s is not currently registered...", oldName); SendClientMessage(playerid, 0x33CC33FF, string); SendClientMessage(playerid, 0x33CC33FF, "*	you can type the /register command once you spawn to register it."); }
    SetPlayerInterior(playerid, 17); SetPlayerPos(playerid, 487.7120, -2.5619, 1002.3828); SetPlayerFacingAngle(playerid, -180.0); SetPlayerCameraPos(playerid, 487.5785, -12.2041, 1000.6797); SetPlayerCameraLookAt(playerid, 487.7120, -2.5619, 1002.3828, CAMERA_CUT);
    ApplyAnimation(playerid, "STRIP", "strip_D", 4.1, 1, 0, 0, 0, 0); TogglePlayerControllable(playerid, 0); SetPlayerDrunkLevel(playerid, 2000);
    
    // Set a timer to destroy the welcome textdraws after a delay
    SetTimer("Internal_DestroyWelcomeTextDraws", 12000, false, "i", playerid);
    return 1;
}

public Internal_DestroyWelcomeTextDraws(playerid)
{
    // *** FIX: Instead of hiding, we now DESTROY the player's unique TextDraws to free up memory. ***
    // This is crucial for preventing client-side lag and memory leaks.
    if(IsPlayerConnected(playerid))
    {
        PlayerTextDrawDestroy(playerid, g_ptdWelcomeBox[playerid]);
        PlayerTextDrawDestroy(playerid, g_ptdWelcomeLogo[playerid]);
        PlayerTextDrawDestroy(playerid, g_ptdWelcomeURL[playerid]);
        PlayerTextDrawDestroy(playerid, g_ptdWelcomeTitle[playerid]);
        PlayerTextDrawDestroy(playerid, g_ptdWelcomeHelp[playerid]);
        PlayerTextDrawDestroy(playerid, g_ptdWelcomeCheatWarning[playerid]);
        PlayerTextDrawDestroy(playerid, g_ptdWelcomeVersion[playerid]);
    }
    return 1;
}

// Called from OnPlayerSpawn
public Sys_OnPlayerSpawn(playerid)
{
    // Restore player state
    TogglePlayerControllable(playerid, 1);
    SetPlayerDrunkLevel(playerid, 0);
    SetCameraBehindPlayer(playerid);

    // *** FIX: Hide the remaining TextDraws before they are destroyed. ***
    // This provides a clean transition for the player.
    Internal_DestroyWelcomeTextDraws(playerid);
    PlayerTextDrawHide(playerid, g_ptdWelcomeCommands[playerid]);
    PlayerTextDrawHide(playerid, g_ptdWelcomeTips[playerid]);

    // We can also destroy these now to clean up.
    PlayerTextDrawDestroy(playerid, g_ptdWelcomeCommands[playerid]);
    PlayerTextDrawDestroy(playerid, g_ptdWelcomeTips[playerid]);
    return 1;
}