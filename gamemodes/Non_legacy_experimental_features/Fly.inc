#if defined NON_LEGACY_CMDS_INCLUDED
    #endinput
#endif
#define NON_LEGACY_CMDS_INCLUDED


// --- Configuration ---
#define FLY_MOVE_SPEED      15.0 // How fast the player moves forward.
#define FLY_ALTITUDE_SPEED  0.8  // How fast the player moves up/down.
#define FLY_HOVER_FORCE     0.07 // A small upward force to counteract gravity while hovering.

// --- Global Variables ---
static bool:IsPlayerFlying[MAX_PLAYERS];
static Timer:gFlyTimer[MAX_PLAYERS];

// --- Command Definition ---
YCMD:fly(playerid, params[], help)
{
    // Toggle the flying state for the player
    if (IsPlayerFlying[playerid])
    {
        StopFlying(playerid);
    }
    else
    {
        StartFlying(playerid);
    }
    return 1;
}

// --- Core Functions ---

stock StartFlying(playerid)
{
    if (!IsPlayerConnected(playerid)) return;

    IsPlayerFlying[playerid] = true;

    // Apply a "falling" animation that looks like flying.
    // This animation also has the benefit of preventing fall damage.
    ApplyAnimation(playerid, "PARACHUTE", "FALL_skyDive", 4.1, 1, 0, 0, 0, 0);

    // Start the timer to handle movement updates
    gFlyTimer[playerid] = SetTimerEx("UpdateFlyState", 100, true, "i", playerid);

    SendClientMessage(playerid, 0x26B0F5AA, "Flying enabled. [W] for forward, [Space] for up, [C] for down.");
    
    // REMOVED: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USEJETPACK);
    // This was the cause of the jetpack bug. It is no longer needed.
}

stock StopFlying(playerid)
{
    if (!IsPlayerConnected(playerid)) return;
    
    // Kill the movement timer
    if (Timer:gFlyTimer[playerid] != Timer:-1)
    {
        KillTimer(gFlyTimer[playerid]);
        gFlyTimer[playerid] = Timer:-1;
    }

    if(IsPlayerFlying[playerid]) // Check if they were actually flying
    {
        IsPlayerFlying[playerid] = false;
        ClearAnimations(playerid);

        // Reset velocity to stop them from drifting
        SetPlayerVelocity(playerid, 0.0, 0.0, 0.0);
        
        // ADDED: Give player a parachute for a safe landing.
        // This is a much safer way to handle being high in the air.
        SetPlayerArmedWeapon(playerid, 46); // 46 is the parachute weapon ID

        SendClientMessage(playerid, 0xFF6347AA, "Flying disabled. You have been given a parachute to land safely.");
    }
}

// This function is called repeatedly by the timer to update the player's velocity
forward UpdateFlyState(playerid);
public UpdateFlyState(playerid)
{
    if (!IsPlayerConnected(playerid) || !IsPlayerFlying[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT)
    {
        return StopFlying(playerid);
    }

    new keys, updown, leftright;
    new Float:velX = 0.0, Float:velY = 0.0, Float:velZ = 0.0;
    
    GetPlayerKeys(playerid, keys, updown, leftright);

    // Altitude control (Up/Down)
    if (keys & KEY_JUMP)
    {
        velZ = FLY_ALTITUDE_SPEED;
    }
    else if (keys & KEY_CROUCH)
    {
        velZ = -FLY_ALTITUDE_SPEED;
    }
    else
    {
        // Apply a small upward force to hover and fight gravity
        velZ = FLY_HOVER_FORCE;
    }

    // Forward movement
    if (keys & KEY_WALK || keys & KEY_SPRINT)
    {
        new Float:camX, Float:camY, Float:camZ;
        GetPlayerCameraFrontVector(playerid, camX, camY, camZ);
        
        velX = camX * FLY_MOVE_SPEED;
        velY = camY * FLY_MOVE_SPEED;
        // The Z-axis is now fully controlled by Space/C for a better feel
    }
    
    // Apply all movement calculations in one single command
    SetPlayerVelocity(playerid, velX, velY, velZ);
    
    return 1;
}

// --- Hooks for Robustness ---

hook OnPlayerDisconnect(playerid, reason)
{
    if (IsPlayerFlying[playerid])
    {
        KillTimer(gFlyTimer[playerid]);
        gFlyTimer[playerid] = Timer:-1;
        IsPlayerFlying[playerid] = false;
    }
    return 1;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    if (IsPlayerFlying[playerid])
    {
        // We only need to kill the timer and reset the state.
        // No need to call StopFlying() which gives a parachute to a dead player.
        KillTimer(gFlyTimer[playerid]);
        gFlyTimer[playerid] = Timer:-1;
        IsPlayerFlying[playerid] = false;
    }
    return 1;
}

hook OnPlayerStateChange(playerid, newstate, oldstate)
{
    if (IsPlayerFlying[playerid] && (newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER))
    {
        StopFlying(playerid);
    }
    return 1;
}