/*
    ================================================================================
    Project:    Open GamerX - (Legacy)
    File:       class_system.inc 
    Description: Manages player classes, skins, weapon loadouts, and the
                 server-wide random teleport point system.
    ================================================================================
*/
#if defined _class_system_included
    #endinput
#endif
#define _class_system_included

#include <open.mp>
#include <sscanf2>

// ---===[ 0. DEFINITIONS ]===---
#define MAX_CLASSES 300

// ---===[ 1. TELEPORT SYSTEM ]===---

enum E_TELEPORT_DATA { Float:tpX, Float:tpY, Float:tpZ, Float:tpAngle }
new Float:g_TeleportPoints[64][30][E_TELEPORT_DATA];
new g_TeleportPointCount[64];
new g_TeleportKeys[64][32];
new g_TeleportKeyCount = 0;


// Replace the existing Teleports_LoadAll with this one.
stock Teleports_LoadAll()
{
    new File:file = fopen("teleports.ini", io_read);
    if (!file)
    {
        print("[FATAL ERROR] Could not load 'scriptfiles/teleports.ini'. Teleports will not work.");
        return;
    }

    new line[128];
    new currentKeyIndex = -1;

    while(fread(file, line))
    {
        // This key parsing logic is correct and robust.
        new endPos = strfind(line, "]");
        if (line[0] == '[' && endPos != -1)
        {
            currentKeyIndex = g_TeleportKeyCount;
            strmid(g_TeleportKeys[currentKeyIndex], line, 1, endPos, 32);
            g_TeleportPointCount[currentKeyIndex] = 0;
            g_TeleportKeyCount++;
        }
        else if (currentKeyIndex != -1)
        {
            new pointIndex = g_TeleportPointCount[currentKeyIndex];
            if (pointIndex < 30)
            {
                new eqPos = strfind(line, "=");
                if (eqPos != -1)
                {
                    // *** THE FIX: The sscanf format now correctly looks for four floats separated by commas. ***
                    if (sscanf(line[eqPos + 1], "%f,%f,%f,%f",
                        g_TeleportPoints[currentKeyIndex][pointIndex][tpX],
                        g_TeleportPoints[currentKeyIndex][pointIndex][tpY],
                        g_TeleportPoints[currentKeyIndex][pointIndex][tpZ],
                        g_TeleportPoints[currentKeyIndex][pointIndex][tpAngle]
                    ))
                    {
                        g_TeleportPointCount[currentKeyIndex]++;
                    }
                }
            }
        }
    }
    fclose(file);

    // This will now print the correct number of loaded locations.
    for (new i = 0; i < g_TeleportKeyCount; i++)
    {
        printf("[TELEPORTS] Loaded %d locations for key '%s'.", g_TeleportPointCount[i], g_TeleportKeys[i]);
    }
}

stock Teleport_ToRandom(playerid, const key[])
{
    for (new i = 0; i < g_TeleportKeyCount; i++)
    {
        if (strcmp(g_TeleportKeys[i], key, true) == 0)
        {
            if (g_TeleportPointCount[i] == 0)
            {
                printf("[TELEPORT WARNING] No points loaded for key '%s', cannot teleport playerid %d.", key, playerid);
                return 0; // No points found for this key
            }
            new rand_pos = random(g_TeleportPointCount[i]);

            // This correctly forces the player back to the main world (Interior 0).
            SetPlayerInterior(playerid, 0);

            SetPlayerPos(playerid, g_TeleportPoints[i][rand_pos][tpX], g_TeleportPoints[i][rand_pos][tpY], g_TeleportPoints[i][rand_pos][tpZ]);
            SetPlayerFacingAngle(playerid, g_TeleportPoints[i][rand_pos][tpAngle]);
            SetCameraBehindPlayer(playerid);
            return 1; // Success
        }
    }
    SendClientMessagef(playerid,0xFF0033AA , "A unexpected error has been occured, 34 error code");
    printf("[TELEPORT WARNING] Could not find teleport key '%s' for playerid %d.", key, playerid);
    return 0; // Key not found
}


// ---===[ 2. CLASS SYSTEM ]===---

enum E_CLASS_INFO
{
    cName[32],
    cTeleportKey[32],
    cSkin1, cSkin2, cSkin3,
    cWeap1, cAmmo1,
    cWeap2, cAmmo2,
    cWeap3, cAmmo3
}

enum E_CLASSES { CLASS_MEDIC, CLASS_BARGIRL, MAX_GAME_CLASSES }

new ClassInfo[MAX_GAME_CLASSES][E_CLASS_INFO] =
{
    // CLASS_MEDIC
    {"Medic", "Class_Medic", 274, 275, 276, 24, 50, 29, 100, 6, 1},
    // CLASS_BARGIRL
    {"Bargirl", "Class_Bargirl", 140, 0, 0, 0, 0, 0, 0, 0, 0}
};

new g_ClassMap[MAX_CLASSES];
new g_TotalClassesAdded = 0;

stock Classes_Init()
{
    g_TotalClassesAdded = 0;
    for (new E_CLASSES:c; c < MAX_GAME_CLASSES; c++) {
        if(ClassInfo[c][cSkin1] > 0) { AddPlayerClass(ClassInfo[c][cSkin1], 1958.3, 1343.1, 15.3, 270.0, 0, 0, 0, 0, 0, 0); g_ClassMap[g_TotalClassesAdded++] = c; }
        if(ClassInfo[c][cSkin2] > 0) { AddPlayerClass(ClassInfo[c][cSkin2], 1958.3, 1343.1, 15.3, 270.0, 0, 0, 0, 0, 0, 0); g_ClassMap[g_TotalClassesAdded++] = c; }
        if(ClassInfo[c][cSkin3] > 0) { AddPlayerClass(ClassInfo[c][cSkin3], 1958.3, 1343.1, 15.3, 270.0, 0, 0, 0, 0, 0, 0); g_ClassMap[g_TotalClassesAdded++] = c; }
    }
}

stock Sys_GiveClassLoadout(playerid, E_CLASSES:class_id)
{
    if(ClassInfo[class_id][cWeap1] > 0) GivePlayerWeapon(playerid, ClassInfo[class_id][cWeap1], ClassInfo[class_id][cAmmo1]);
    if(ClassInfo[class_id][cWeap2] > 0) GivePlayerWeapon(playerid, ClassInfo[class_id][cWeap2], ClassInfo[class_id][cAmmo2]);
    if(ClassInfo[class_id][cWeap3] > 0) GivePlayerWeapon(playerid, ClassInfo[class_id][cWeap3], ClassInfo[class_id][cAmmo3]);
}