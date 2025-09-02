/*
    ================================================================================
    Project:    Open GamerX - Legacy
    Description: Handles player registration, login, and data persistence
                 using modern, non-blocking password hashing(much parts of this code were created by ai, including this description)
    ================================================================================
*/

#if defined _accounts_included
    #endinput
#endif
#define _accounts_included

// ---===[ Forwards for this system ]===---
forward OnAccountLoad(playerid, Cache:cache_id);
forward OnAccountRegister(playerid, Cache:cache_id);
forward OnPasswordHash(playerid, const pass_hash[]);
forward OnPasswordVerify(playerid, bool:success);

// ---===[ Core Functions ]===---

stock LoadAccount(playerid)
{
    new playerName[MAX_PLAYER_NAME], escapedName[MAX_PLAYER_NAME * 2 + 1], query[300];
    GetPlayerName(playerid, playerName, sizeof(playerName));
    mysql_escape_string(playerName, escapedName, g_dbConnection);

    // MODIFIED: Added `AdminLevel` to the fields we retrieve.
    format(query, sizeof(query), "SELECT `Password`, `Level`, `Money`, `Kills`, `Deaths`, `AdminLevel` FROM `players` WHERE `Name` = '%s' LIMIT 1", escapedName);
    mysql_tquery(g_dbConnection, query, "OnAccountLoad", "i", playerid);
    return 1;
}

stock SaveAccount(playerid)
{
    if (!PlayerData[playerid][pLoggedIn]) return 1;

    new playerName[MAX_PLAYER_NAME], escapedName[MAX_PLAYER_NAME * 2 + 1], query[512];
    new playerIP[16], escapedIP[32];

    GetPlayerName(playerid, playerName, sizeof(playerName));
    mysql_escape_string(playerName, escapedName, g_dbConnection);
    
    GetPlayerIp(playerid, playerIP, sizeof(playerIP));
    mysql_escape_string(playerIP, escapedIP, g_dbConnection);

    // MODIFIED: Added `AdminLevel` and `LastIP` to the fields we save.
    format(query, sizeof(query), "UPDATE `players` SET `Level` = %d, `Money` = %d, `Kills` = %d, `Deaths` = %d, `AdminLevel` = %d, `LastIP` = '%s' WHERE `Name` = '%s'",
        PlayerData[playerid][pLevel],
        PlayerData[playerid][pMoney],
        PlayerData[playerid][pKills],
        PlayerData[playerid][pDeaths],
        PlayerData[playerid][pAdminLevel],
        escapedIP,
        escapedName
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
        cache_get_value_name(0, "Password", PlayerData[playerid][pPassword], 129);
        PlayerData[playerid][pLevel]      = cache_get_value_name_int(0, "Level");
        PlayerData[playerid][pMoney]      = cache_get_value_name_int(0, "Money");
        PlayerData[playerid][pKills]      = cache_get_value_name_int(0, "Kills");
        PlayerData[playerid][pDeaths]     = cache_get_value_name_int(0, "Deaths");
        PlayerData[playerid][pAdminLevel] = cache_get_value_name_int(0, "AdminLevel"); // ADDED
        
        ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Account Login", "This name is registered.\nPlease enter your password:", "Login", "Cancel");
    }
    else
    {
        ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Account Registration", "This name is not registered.\nPlease choose a password to register:", "Register", "Cancel");
    }
    return 1;
}

public OnAccountRegister(playerid, Cache:cache_id)
{
    if (!IsPlayerConnected(playerid)) return 1;
    PlayerData[playerid][pLoggedIn] = true;
    SendClientMessage(playerid, 0x33CC33FF, "Registration successful! You are now logged in.");
    SpawnPlayer(playerid);
    return 1;
}

// ---===[ BCrypt Callbacks ]===---

public OnPasswordHash(playerid, const pass_hash[])
{
    if(!IsPlayerConnected(playerid)) return 1;

    new playerName[MAX_PLAYER_NAME], escapedName[MAX_PLAYER_NAME * 2 + 1], query[512];
    GetPlayerName(playerid, playerName, sizeof(playerName));
    mysql_escape_string(playerName, escapedName, g_dbConnection);

    // Set default stats in the PlayerData array for the new player.
    PlayerData[playerid][pLevel]      = 1;
    PlayerData[playerid][pMoney]      = 5000;
    PlayerData[playerid][pKills]      = 0;
    PlayerData[playerid][pDeaths]     = 0;
    PlayerData[playerid][pAdminLevel] = 1; // ADDED: Default admin level is 1 (Player)
    strcopy(PlayerData[playerid][pPassword], pass_hash, 129);

    // MODIFIED: Added `AdminLevel` to the fields we insert for a new registration.
    format(query, sizeof(query), "INSERT INTO `players` (`Name`, `Password`, `Level`, `Money`, `Kills`, `Deaths`, `AdminLevel`) VALUES ('%s', '%s', 1, 5000, 0, 0, 1)", escapedName, pass_hash);
    mysql_tquery(g_dbConnection, query, "OnAccountRegister", "i", playerid);
    return 1;
}

public OnPasswordVerify(playerid, bool:success)
{
    if(!IsPlayerConnected(playerid)) return 1;
    if (success)
    {
        PlayerData[playerid][pLoggedIn] = true;
        SendClientMessage(playerid, 0x33CC33FF, "Login successful! Welcome back.");
        SpawnPlayer(playerid);
    }
    else
    {
        SendClientMessage(playerid, 0xFF0000FF, "ERROR: Incorrect password.");
        ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Account Login", "Incorrect password. Please try again:", "Login", "Cancel");
    }
    return 1;
}

// ---===[ Dialog Handler ]===---
stock Dialog_OnAccounts(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        case DIALOG_LOGIN:
        {
            if (!response) return Kick(playerid);
            SendClientMessage(playerid, 0xFFFFFFAA, "Authenticating...");
            bcrypt_verify(playerid, "OnPasswordVerify", inputtext, PlayerData[playerid][pPassword]);
            return 1;
        }
        case DIALOG_REGISTER:
        {
            if (!response) return Kick(playerid);

            if (strlen(inputtext) < 5 || strlen(inputtext) > 24)
            {
                SendClientMessage(playerid, 0xFF0000FF, "ERROR: Your password must be between 5 and 24 characters.");
                ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Account Registration", "Password invalid. Please choose a password\n(5-24 characters):", "Register", "Cancel");
                return 1;
            }
            SendClientMessage(playerid, 0xFFFFFFAA, "Registering account, please wait...");
            bcrypt_hash(playerid, "OnPasswordHash", inputtext, BCRYPT_COST);
            return 1;
        }
    }
    return 0;
}
