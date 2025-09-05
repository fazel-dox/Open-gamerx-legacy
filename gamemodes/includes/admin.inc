/*
    ================================================================================
    Project:    Open GamerX - Base (Legacy)
    File:       admin.inc (Corrected)
    Author:     Paniz & AI Assistant
    Description: Handles all core administrative commands and systems.
    ================================================================================
*/

#if defined _admin_included
    #endinput
#endif
#define _admin_included

#include "core.inc" // Include the new foundation

// ---===[ 1. DEFINITIONS & ENUMS ]===---
// All ADMIN_ level definitions have been moved to core.inc

#define DIALOG_BAN_MESSAGE      102

#define COLOR_ADMIN_ACTION      0xC2A2DAFF
#define COLOR_PUBLIC_BAN        0xFF8C00FF
#define COLOR_SERVER_ALERT      0xFF0000FF

// ---===[ 2. FORWARDS & VARIABLES ]===---

forward Kick_OnPlayerKicked(playerid, reason[], adminname[]);
forward Ban_PerformBan(adminid, targetid, reason[]);

// ---===[ 3. HELPER FUNCTIONS ]===---

stock LogAdminAction(adminid, action[])
{
    new adminName[MAX_PLAYER_NAME], escapedAction[1024], query[1200];
    GetPlayerName(adminid, adminName, sizeof(adminName));
    mysql_escape_string(action, escapedAction, g_dbConnection);
    format(query, sizeof(query), "INSERT INTO `admin_logs` (`admin_name`, `action`) VALUES ('%s', '%s')", adminName, escapedAction);
    mysql_tquery(g_dbConnection, query);
}

// ---===[ 4. CORE ADMIN COMMANDS ]===---

CMD:kick(playerid, params[])
{
    if(PlayerData[playerid][pAdminLevel] < ADMIN_TRIAL)
    {
        SendClientMessage(playerid, COLOR_SERVER_ALERT, "ERROR: You are not authorized to use this command.");
        return 1;
    }

    new targetid, reason[128];
    if(sscanf(params, "us[128]", targetid, reason))
    {
        SendClientMessage(playerid, 0xFF9900FF, "USAGE: /kick [playerid/name] [reason]");
        return 1;
    }

    if(!IsPlayerConnected(targetid))
    {
        SendClientMessage(playerid, COLOR_SERVER_ALERT, "ERROR: The specified player is not online.");
        return 1;
    }
    
    if(PlayerData[targetid][pAdminLevel] >= PlayerData[playerid][pAdminLevel] && playerid != targetid)
    {
        SendClientMessage(playerid, COLOR_SERVER_ALERT, "ERROR: You cannot kick an admin of equal or higher level.");
        return 1;
    }

    new message[256], adminName[MAX_PLAYER_NAME], targetName[MAX_PLAYER_NAME];
    GetPlayerName(playerid, adminName, sizeof(adminName));
    GetPlayerName(targetid, targetName, sizeof(targetName));

    format(message, sizeof(message), "Kicked '%s' (ID: %d). Reason: %s", targetName, targetid, reason);
    LogAdminAction(playerid, message);

    format(message, sizeof(message), "* %s has been kicked by Admin %s. (Reason: %s)", targetName, adminName, reason);
    SendClientMessageToAll(COLOR_SERVER_ALERT, message);
    
    SetTimerEx("Kick_OnPlayerKicked", 100, false, "iss", targetid, reason, adminName);
    return 1;
}

CMD:ban(playerid, params[])
{
    if(PlayerData[playerid][pAdminLevel] < ADMIN_STANDARD)
    {
        SendClientMessage(playerid, COLOR_SERVER_ALERT, "ERROR: You are not authorized to use this command.");
        return 1;
    }
    
    new targetid, reason[128];
    if(sscanf(params, "us[128]", targetid, reason))
    {
        SendClientMessage(playerid, 0xFF9900FF, "USAGE: /ban [playerid/name] [reason]");
        return 1;
    }

    if(!IsPlayerConnected(targetid))
    {
        SendClientMessage(playerid, COLOR_SERVER_ALERT, "ERROR: The specified player is not online.");
        return 1;
    }
    
    if(PlayerData[targetid][pAdminLevel] >= PlayerData[playerid][pAdminLevel] && playerid != targetid)
    {
        SendClientMessage(playerid, COLOR_SERVER_ALERT, "ERROR: You cannot ban an admin of equal or higher level.");
        return 1;
    }
    
    Ban_PerformBan(playerid, targetid, reason);
    return 1;
}

// ---===[ 5. CALLBACKS & WORKERS ]===---

public Kick_OnPlayerKicked(playerid, reason[], adminname[])
{
    if(IsPlayerConnected(playerid))
    {
        new message[256];
        format(message, sizeof(message), "You have been kicked by %s. Reason: %s", adminname, reason);
        SendClientMessage(playerid, COLOR_SERVER_ALERT, message);
        Kick(playerid);
    }
    return 1;
}

stock Ban_PerformBan(adminid, targetid, reason[])
{
    new adminName[MAX_PLAYER_NAME], targetName[MAX_PLAYER_NAME], targetIP[16];
    new escapedTargetName[MAX_PLAYER_NAME*2+1], escapedAdminName[MAX_PLAYER_NAME*2+1], escapedReason[128*2+1];
    
    GetPlayerName(adminid, adminName, sizeof(adminName));
    GetPlayerName(targetid, targetName, sizeof(targetName));
    GetPlayerIp(targetid, targetIP, sizeof(targetIP));

    new publicMsg[256];
    format(publicMsg, sizeof(publicMsg), "* GamerX: %s (ID:%d) has been banned by Admin %s for %s.", targetName, targetid, adminName, reason);
    SendClientMessageToAll(COLOR_PUBLIC_BAN, publicMsg);

    format(publicMsg, sizeof(publicMsg), "Banned '%s' (ID: %d). Reason: %s", targetName, targetid, reason);
    LogAdminAction(adminid, publicMsg);

    mysql_escape_string(targetName, escapedTargetName, g_dbConnection);
    mysql_escape_string(adminName, escapedAdminName, g_dbConnection);
    mysql_escape_string(reason, escapedReason, g_dbConnection);
    
    new query[512];
    format(query, sizeof(query), "INSERT INTO `bans` (banned_name, banned_ip, admin_name, reason) VALUES ('%s', '%s', '%s', '%s')",
        escapedTargetName,
        targetIP,
        escapedAdminName,
        escapedReason
    );
    mysql_tquery(g_dbConnection, query);

    new banDialogText[512];
    new year, month, day, hour, minute, second;
    getdate(year, month, day);
    gettime(hour, minute, second);
    
    // THIS IS THE CORRECTED PART
    format(banDialogText, sizeof(banDialogText),
        "YOU HAVE BEEN BANNED - READ THE FOLLOWING DETAILS:\n\n" \
        "Your Player Name: %s\n" \
        "Admin Name: %s (ID:%d)\n" \
        "Date and Time: %02d-%02d-%d %02d:%02d\n" \
        "Reason: %s\n\n" \
        "You can press the F8 key to take a screenshot of this screen. If you wish to make\n" \
        "an appeal then go to https://gamerxserver.com and post in the Ban Appeals forum.",
        targetName, adminName, adminid, day, month, year, hour, minute, reason
    );
    ShowPlayerDialog(targetid, DIALOG_BAN_MESSAGE, DIALOG_STYLE_MSGBOX, "You Have Been Banned...", banDialogText, "OK", "");

    SetTimer("Kick", 2000, false, "i", targetid);
    return 1;
}