/*
    ================================================================================
    Project:    GamerX Rebirth - Player Object System (FINAL)
    File:       player_objects.inc
    Author:     Paniz & AI Assistant
    Description: Handles all player-created objects (/cpo, /epo, /spoma etc.)
    ================================================================================
*/
#if defined _player_objects_included
    #endinput
#endif
#define _player_objects_included

// ---===[ 1. FORWARDS & DEFINITIONS ]===---

// The timer callback MUST be forwarded and declared as public so the timer system can find it.
forward public ApplyPlayerObjectMaterial(playerid);

#define MAX_PLAYER_OBJECTS 350
#define OBJECT_SPAWN_DISTANCE 10.0
#define COLOR_POB_INFO 0x20B2AAAA
#define COLOR_POB_SUCCESS 0xFFDD00AA

enum E_POB_DATA
{
    pobExists,
    pobID,
    pobModel
}
new PlayerObjects[MAX_PLAYERS][MAX_PLAYER_OBJECTS][E_POB_DATA];

new g_pobLastCreated[MAX_PLAYERS];

// Global variables to temporarily hold data for the spoma timer
new g_spomaData[MAX_PLAYERS][5]; // 0:slot, 1:mat_idx, 2:src_mdl, 3:mat_clr, 4:temp_obj_id
new g_spomaTxd[MAX_PLAYERS][64];
new g_spomaTex[MAX_PLAYERS][64];


// ---===[ 2. CORE COMMANDS ]===---

YCMD:cpo(playerid, params[], help)
{
    new modelid;
    if (sscanf(params, "d", modelid)) return SendClientMessage(playerid, 0xFF9900FF, "USAGE: /cpo [model_id]");
    new objectSlot = -1, objectCount = 0;
    for (new i = 0; i < MAX_PLAYER_OBJECTS; i++)
    {
        if (PlayerObjects[playerid][i][pobExists]) objectCount++;
        else if (objectSlot == -1) objectSlot = i;
    }
    if (objectSlot == -1) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: You have reached your player object limit (350).");
    new Float:x, Float:y, Float:z, Float:angle;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, angle);
    new Float:spawnX = x + (OBJECT_SPAWN_DISTANCE * floatsin(-angle, degrees));
    new Float:spawnY = y + (OBJECT_SPAWN_DISTANCE * floatcos(-angle, degrees));
    new PlayerObject:objectId = CreatePlayerObject(playerid, modelid, spawnX, spawnY, z, 0.0, 0.0, angle);
    if (objectId == INVALID_OBJECT_ID) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: Invalid model ID. The object could not be created.");
    PlayerObjects[playerid][objectSlot][pobExists] = true;
    PlayerObjects[playerid][objectSlot][pobID] = objectId;
    PlayerObjects[playerid][objectSlot][pobModel] = modelid;
    g_pobLastCreated[playerid] = objectSlot;
    new message[256];
    format(message, sizeof(message), "~g~~h~Player Object Created...~n~Number %d (ID:%d)", objectSlot, modelid);
    GameTextForPlayer(playerid, message, 4000, 3);
    format(message, sizeof(message), "** You created player object number %d (ID:%d) %.2fm in front of you... total player objects %d of %d.", objectSlot, modelid, OBJECT_SPAWN_DISTANCE, objectCount + 1, MAX_PLAYER_OBJECTS);
    SendClientMessage(playerid, COLOR_POB_INFO, message);
    SendClientMessagef(playerid, COLOR_POB_INFO, "*  To edit this object's position with your mouse type: /epo %d", objectSlot);
    SendClientMessagef(playerid, COLOR_POB_INFO, "*  To put this object in front of you type: /ppo %d", objectSlot);
    SendClientMessagef(playerid, COLOR_POB_INFO, "*  To teleport to this player object type: /gpo %d", objectSlot);
    SendClientMessagef(playerid, COLOR_POB_INFO, "*  To delete this player object type: /dpo %d", objectSlot);
    SendClientMessage(playerid, COLOR_POB_INFO, "*	Remember: only YOU can see the player objects that you create. Any objects created are destroyed when you leave the server.");
    SendClientMessage(playerid, COLOR_POB_INFO, "*	Read /pobhelp for lots more information and help.");
    return 1;
}

YCMD:epo(playerid, params[], help)
{
    new objectSlot;
    if (sscanf(params, "d", objectSlot)) objectSlot = g_pobLastCreated[playerid];
    if (objectSlot < 0 || objectSlot >= MAX_PLAYER_OBJECTS || !PlayerObjects[playerid][objectSlot][pobExists]) return SendClientMessage(playerid, 0xFF0000AA, "ERROR: You have not created that player object slot.");
    new message[128];
    new modelid = PlayerObjects[playerid][objectSlot][pobModel];
    new PlayerObject:objectid = PlayerObjects[playerid][objectSlot][pobID];
    format(message, sizeof(message), "~g~~h~Editing Player Object...~n~Number %d (ID:%d)", objectSlot, modelid);
    GameTextForPlayer(playerid, message, 4000, 3);
    format(message, sizeof(message), "* You are now editing player object number %d (ID:%d)", objectSlot, modelid);
    SendClientMessage(playerid, COLOR_POB_SUCCESS, message);
    EditPlayerObject(playerid, objectid);
    return 1;
}

YCMD:spoma(playerid, params[], help)
{
    new objectSlot, materialIndex, sourceModel, materialColor = 0;
    new txdFile[64], textureName[64];

    // This single, clean sscanf call correctly handles the optional color parameter
    // without generating warnings, and correctly parses unquoted strings.
    if (sscanf(params, "dddS[64]S[64]I", objectSlot, materialIndex, sourceModel, txdFile, textureName, materialColor) < 5)
    {
        return SendClientMessage(playerid, 0xFF9900FF, "USAGE: /spoma [object slot] [material index] [source model] [txd file] [texture name] <color_hex>");
    }

    if (objectSlot < 0 || objectSlot >= MAX_PLAYER_OBJECTS || !PlayerObjects[playerid][objectSlot][pobExists])
    {
        return SendClientMessage(playerid, 0xFF0000AA, "ERROR: You have not created that player object slot.");
    }
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    new PlayerObject:tempObject = CreatePlayerObject(playerid, sourceModel, x, y, z + 200.0, 0.0, 0.0, 0.0, 0.0);
    g_spomaData[playerid][0] = objectSlot;
    g_spomaData[playerid][1] = materialIndex;
    g_spomaData[playerid][2] = sourceModel;
    g_spomaData[playerid][3] = materialColor;
    g_spomaData[playerid][4] = _:tempObject;
    strunpack(g_spomaTxd[playerid], txdFile, 64);
    strunpack(g_spomaTex[playerid], textureName, 64);
    SetTimer("ApplyPlayerObjectMaterial", 100, false);
    return 1;
}

// ---===[ 3. SYSTEM FUNCTIONS & CALLBACKS ]===---

stock PObjects_OnDisconnect(playerid)
{
    for (new i = 0; i < MAX_PLAYER_OBJECTS; i++)
    {
        if (PlayerObjects[playerid][i][pobExists])
        {
            DestroyPlayerObject(playerid, PlayerObjects[playerid][i][pobID]);
            PlayerObjects[playerid][i][pobExists] = false;
            PlayerObjects[playerid][i][pobID] = INVALID_OBJECT_ID;
            PlayerObjects[playerid][i][pobModel] = 0;
        }
    }
}

public ApplyPlayerObjectMaterial(playerid)
{
    new objectSlot    = g_spomaData[playerid][0];
    new materialIndex = g_spomaData[playerid][1];
    new sourceModel   = g_spomaData[playerid][2];
    new materialColor = g_spomaData[playerid][3];
    new PlayerObject:temp_object = PlayerObject:g_spomaData[playerid][4];
    new txdFile[64], textureName[64];
    strpack(txdFile, g_spomaTxd[playerid], 64);
    strpack(textureName, g_spomaTex[playerid], 64);

    DestroyPlayerObject(playerid, temp_object);

    new PlayerObject:objectid = PlayerObjects[playerid][objectSlot][pobID];
    new modelid = PlayerObjects[playerid][objectSlot][pobModel];

    SetPlayerObjectMaterial(playerid, objectid, materialIndex, sourceModel, txdFile, textureName, materialColor);

    new message[256];
    format(message, sizeof(message), "~g~~h~Player Object Material Set...~n~Number %d (ID:%d)", objectSlot, modelid);
    GameTextForPlayer(playerid, message, 5000, 3);
    format(message, sizeof(message), "** You set the material on player object number %d (ID:%d)...", objectSlot, modelid);
    SendClientMessage(playerid, COLOR_POB_INFO, message);
    format(message, sizeof(message), "*  Material Index = %d ~ Source Object ID = %d | TXD File Name = \"%s\" | Texture Name = \"%s\" | Material Colour = 0x%X", materialIndex, sourceModel, txdFile, textureName, materialColor);
    SendClientMessage(playerid, COLOR_POB_INFO, message);

    // Clean up the global arrays to prevent using old data
    g_spomaTxd[playerid][0] = EOS;
    g_spomaTex[playerid][0] = EOS;

    return 1;
}