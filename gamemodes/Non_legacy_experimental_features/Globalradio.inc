/*
 *  AutoRadio Include Script for open.mp (YCMD Version)
 *  - Provides a simple server-wide radio system.
 *  - Allows RCON admins to set or stop a radio stream for all players.
 *  - New players will automatically hear the stream if it's active.
 *
 *  Commands:
 *  /setradio [URL]     - Sets the radio stream for everyone.
 *  /setradio off       - Stops the radio stream for everyone.
 *
 *  Dependencies:
 *  - open.mp
 *  - YSI_Commands (part of the YSI library)
 *  - sscanf
*/

#if defined _AUTORADIO_INC_
    #endinput
#endif
#define _AUTORADIO_INC_

#include <open.mp>

// --- Configuration ---
#define MAX_RADIO_URL 256 // Maximum length for a stream URL. 256 is safe.

// --- Global Variables ---
static g_szRadioURL[MAX_RADIO_URL]; // This stores the current radio URL globally.

/**
 * <summary>
 *  This hook is called when a player connects to the server.
 *  We use it to start the radio stream for them if one is active.
 * </summary>
 */
hook OnPlayerConnect(playerid)
{
    if (g_szRadioURL[0] != '\0')
    {
        PlayAudioStreamForPlayer(playerid, g_szRadioURL);
        new message[MAX_RADIO_URL + 40];
        format(message, sizeof(message), "SERVER RADIO: Now playing a stream. Use /stopradio to mute.");
        SendClientMessage(playerid, 0x2596beFF, message);
    }
    return 1;
}

/**
 * <summary>
 *  Command for RCON admins to set or stop the server-wide radio.
 * </summary>
 */
YCMD:setradio(playerid, params[], help) // Changed from ocmd:setradio
{
    #pragma unused help // Tell the compiler we know 'help' isn't used.
    if (!IsPlayerAdmin(playerid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "ERROR: You are not logged in as an RCON admin.");
        return 1;
    }

    new param[MAX_RADIO_URL];
    if (sscanf(params, "s", param))
    {
        SendClientMessage(playerid, 0xFFFFFFAA, "USAGE: /setradio [url] OR /setradio [off]");
        return 1;
    }

    if (strcmp(param, "off", true) == 0)
    {
        g_szRadioURL[0] = '\0';
        foreach (new i : Player)
        {
            StopAudioStreamForPlayer(i);
        }
        SendClientMessageToAll(0xFFFF00AA, "Admin has turned off the server radio.");
        return 1;
    }

    format(g_szRadioURL, sizeof(g_szRadioURL), "%s", param);
    new message[MAX_RADIO_URL + 50];
    format(message, sizeof(message), "SERVER RADIO: Admin has set the stream. Now playing for everyone.");
    SendClientMessageToAll(0x2596beFF, message);

    foreach (new i : Player)
    {
        PlayAudioStreamForPlayer(i, g_szRadioURL);
    }
    return 1;
}

/**
 * <summary>
 *  A simple command for players to stop the radio for themselves.
 * </summary>
 */
YCMD:stopradio(playerid, params[], help) // Changed from ocmd:stopradio
{
    #pragma unused help
    #pragma unused params
    StopAudioStreamForPlayer(playerid);
    SendClientMessage(playerid, 0x2596beFF, "Radio stream stopped for you. The stream is still active for other players.");
    return 1;
}