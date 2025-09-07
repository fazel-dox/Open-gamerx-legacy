/*
    ================================================================================
    Project:    Open GamerX - Legacy(ACCOUNTS.INC)
    Description: Handles player registration, login, and data persistence.
                 Refactored for the "Play First, Register Later" philosophy.
    ================================================================================
*/

#if defined _accounts_included
    #endinput
#endif
#define _accounts_included

#include "core.inc" // Include the new foundation

// ---===[ Forwards for this system ]===---
forward OnAccountLoad(playerid, Cache:cache_id);
forward OnAccountRegister(playerid, Cache:cache_id);
forward OnPasswordHash(playerid, const pass_hash[]);
forward OnPasswordVerify(playerid, bool:success);
forward Dialog_OnAccounts(playerid, dialogid, response, listitem, inputtext[]);
// ---===[ NEW: On-Demand Commands ]===---

// Player types /register to start the registration process
YCMD:register(playerid, params[], help)
{
    if(PlayerData[playerid][pLoggedIn])
    {
        return SendClientMessage(playerid, 0xFF0000AA, "ERROR: You are already logged in.");
    }
    // This command now triggers the account check.
    LoadAccount(playerid);
    return 1;
}

// Player types /login to start the login process
YCMD:login(playerid, params[], help)
{
    if(PlayerData[playerid][pLoggedIn])
    {
        return SendClientMessage(playerid, 0xFF0000AA, "ERROR: You are already logged in.");
    }
    // This command also triggers the account check.
    LoadAccount(playerid);
    return 1;
}


// ---===[ Core Functions ]===---

stock LoadAccount(playerid)
{
    // This function now uses the safer mysql_format.
    static query[300];
    mysql_format(g_dbConnection, query, sizeof(query), "SELECT `Password`, `Level`, `Money`, `Kills`, `Deaths`, `AdminLevel` FROM `players` WHERE `Name` = '%e' LIMIT 1", GetPlayerName(playerid));
    mysql_tquery(g_dbConnection, query, "OnAccountLoad", "i", playerid);
    return 1;
}

stock SaveAccount(playerid)
{
    if (!PlayerData[playerid][pLoggedIn]) return 1;

    // This function also updated to use the safer mysql_format.
    static query[512];
    mysql_format(g_dbConnection, query, sizeof(query), "UPDATE `players` SET `Level` = %d, `Money` = %d, `Kills` = %d, `Deaths` = %d, `AdminLevel` = %d, `LastIP` = '%e' WHERE `Name` = '%e'",
        PlayerData[playerid][pLevel],
        PlayerData[playerid][pMoney],
        PlayerData[playerid][pKills],
        PlayerData[playerid][pDeaths],
        PlayerData[playerid][pAdminLevel],
        GetPlayerIp(playerid),
        GetPlayerName(playerid)
    );
    mysql_tquery(g_dbConnection, query);
    return 1;
}

// ---===[ Callback Handlers ]===---

public OnAccountLoad(playerid, Cache:cache_id)
{
    if (!IsPlayerConnected(playerid)) return 1;

    if (cache_get_row_count())
    {
        // Name is registered, prompt for login.
        cache_get_value_name(0, "Password", PlayerData[playerid][pPassword], 129);
        PlayerData[playerid][pLevel]      = cache_get_value_name_int(0, "Level");
        PlayerData[playerid][pMoney]      = cache_get_value_name_int(0, "Money");
        // ... etc
        PlayerData[playerid][pAdminLevel] = cache_get_value_name_int(0, "AdminLevel");
        
        ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Account Login", "This name is registered.\nPlease enter your password:", "Login", "Cancel");
    }
    else
    {
        // Name is not registered, prompt for registration.
        ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Account Registration", "This name is not registered.\nPlease choose a password to register:", "Register", "Cancel");
    }
    return 1;
}
// ... (OnAccountRegister is fine) ...
// ---===[ BCrypt Callbacks ]===---

public OnPasswordHash(playerid, const pass_hash[])
{
    if(!IsPlayerConnected(playerid)) return 1;

    // Set default stats...
    PlayerData[playerid][pLevel]      = 1;
    // ... etc
    PlayerData[playerid][pAdminLevel] = ADMIN_PLAYER; // Set to default player level
    strcopy(PlayerData[playerid][pPassword], pass_hash, 129);

    // Using mysql_format for the INSERT query.
    static query[512];
    mysql_format(g_dbConnection, query, sizeof(query), "INSERT INTO `players` (`Name`, `Password`, `Level`, `Money`, `Kills`, `Deaths`, `AdminLevel`) VALUES ('%e', '%s', 1, 5000, 0, 0, %d)", GetPlayerName(playerid), pass_hash, ADMIN_PLAYER);
    mysql_tquery(g_dbConnection, query, "OnAccountRegister", "i", playerid);
    return 1;
}
public Dialog_OnAccounts(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        case DIALOG_LOGIN:
        {
            if (!response) return 1; // Player clicked "Cancel"
            if (!strlen(inputtext)) return ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Account Login", "{FF0000}Password cannot be empty.\n{FFFFFF}Please enter your password:", "Login", "Cancel");

            bcrypt_verify(playerid, inputtext, PlayerData[playerid][pPassword], "OnPasswordVerify");
            return 1;
        }
        case DIALOG_REGISTER:
        {
            if (!response) return 1; // Player clicked "Cancel"
            if (strlen(inputtext) < 4) return ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Account Registration", "{FF0000}Password must be at least 4 characters.\n{FFFFFF}Please choose a new password:", "Register", "Cancel");

            bcrypt_hash(playerid, inputtext, "OnPasswordHash");
            return 1;
        }
    }
    return 0;
}

// ... (OnPasswordVerify is fine) ...
// ... (Dialog_OnAccounts is fine) ...