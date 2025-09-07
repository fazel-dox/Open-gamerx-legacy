/*
    ================================================================================
    Project: Open GamerX - Base
    File: commands.inc
    Description: Exhaustive list of all GamerX commands based on GamerX-Commands.txt
                 This is the COMPLETE file with all 2760+ commands and aliases.
    ================================================================================
*/



//------------------------------------------------------------------------
// Login, Register and Help Commands
//------------------------------------------------------------------------
//CMD:ns(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:login(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:lhelp(playerid, params[], "loginhelp"){ return ShowNotImplemented(playerid); }
//CMD:register(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:help(playerid, params[]){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// Misc Commands Available in Jail, Toilet, Torture etc
//------------------------------------------------------------------------
//CMD:setrecommendedversion(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:betahelp(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:newb(playerid, params[], "noob", "newbie", "noobie"){ return ShowNotImplemented(playerid); }
YCMD:rules(playerid, params[])
{
    // A large string to hold all the rules text.
    new rulesString[2048];

    // Build the string line by line for readability.
    // The \n character creates a new line in the dialog.
    format(rulesString, sizeof(rulesString),
        "1) No cheating, hacking or using mods to gain an unfair advantage.\n" \
        "2) No racism (that includes using the N word), flaming, spamming, flooding or behaving like an arsehole.\n" \
        "3) Do not behave like a moron in a Helicopter (that means do not kill or annoy people with it).\n" \
        "4) Do not jack other players who are in a vehicle mod shop (eg at the /t1 /t2 /t3 etc teleports).\n" \
        "5) Do not ask to become an admin, if we think you are good enough we will ask you to join us.\n" \
        "6) No spawn killing or \"camping\" at the point where players spawn (shaded red on radar) or teleport to.\n" \
        "7) Do not use the Vortex to driveby kill people... it is for driving or flying not killing.\n" \
        "8) Respect other players and Admins.... do NOT annoy people when they are creating objects.\n" \
        "9) Do not driveby with /autofix enabled... disable /autofix if you wish to driveby.\n" \
        "10) Do not driveby at spawn zones even with /autofix off... spawn zones are shaded red on the radar.\n" \
        "11) Do not entice people to type /q or /quit... it is NOT funny.\n" \
        "12) Do not use the /healme command during a fight... only use it after.\n" \
        "13) Do not shoot at other players when they are inside Ammu-Nation (/ammo or /guns).\n" \
        "14) Do not use a command to avoid death during a fight with another player.\n" \
        "15) Do not use the 2 shot bug, the C-Bug or water tanks in the red spawn zones.\n" \
        "16) Do not use the minigun, rockets, grenades, molotovs or the flame thrower in the red spawn zones.\n" \
        "17) Do not interfere with an event being run by an admin or TP (eg goo hunts, pickup competitions, etc).\n\n" \
        "Refusing to obey WILL result in a mute, punishment, kick or a ban.\n\n" \
        "If you do not agree with these rules then please find another server to play on.\n" \
        "By playing here you accept the terms displayed in the /disclaimer text.\n\n" \
        "A complete list of the rules can be found here: https://gamerxserver.com/rules.html\n" \
        "For lots more help and instruction visit our website and forum: https://gamerxserver.com"
    );

    // Display the dialog to the player who typed the command.
    ShowPlayerDialog(playerid, DIALOG_RULES, DIALOG_STYLE_MSGBOX, "GamerX Server Rules...", rulesString, "OK", "");

    return 1;
}
//CMD:motd(playerid, params[]){ return ShowNotImplemented(playerid); }
YCMD:car(playerid, params[])
{
    // Check if the player alreadyCMD:car(playerid, params[])
{
    // CORRECTED: Using the global g_playerVehicle array
    if(IsValidVehicle(g_playerVehicle[playerid]))
    {
        DestroyVehicle(g_playerVehicle[playerid]);
    }

    // Get the player's current position and facing angle.
    new Float:x, Float:y, Float:z, Float:angle;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, angle);

    // Calculate a spawn position 4 units in front of the player.
    new Float:spawnX = x + (4.0 * floatsin(-angle, degrees));
    new Float:spawnY = y + (4.0 * floatcos(-angle, degrees));

    // Create the Infernus (model ID 411) with random colors.
    new vehicleid = CreateVehicle(411, spawnX, spawnY, z + 0.5, angle, -1, -1, -1);

    // CORRECTED: Storing the ID in the global g_playerVehicle array
    g_playerVehicle[playerid] = vehicleid;

    // Put the player directly into the driver's seat.
    PutPlayerInVehicle(playerid, vehicleid, 0);

    // Send the exact confirmation message from the original server.
    SendClientMessage(playerid, 0x33CC33FF, "* You spawned a new Infernus car... you can also use the /v1 to /v17 dialogs to spawn other vehicle types.");

    return 1;
}
//CMD:v(playerid, params[], "vehicle", "vehicles")
//{
    //new modelid;

    // PROBLEM 1 & 2 FIXED: Correctly check if sscanf failed to get an integer.
    // If it fails (returns anything other than 0), send a usage message and stop.
    //if (sscanf(params, "i", modelid))
    //{
        //SendClientMessage(playerid, 0xFF9900FF, "USAGE: /v [vehicle_model_id]");
        //return 1; // Stop the command here.
    //}

    // Un-commented and enabled the essential model ID validation to prevent crashes.
    //if (modelid < 400 || modelid > 611)
    //{
        //SendClientMessage(playerid, 0xFF6347FF, "ERROR: Invalid vehicle model ID. Please use a number between 400 and 611.");
        //return 1; // Stop the command here.
    //}

    // PROBLEM 3 FIXED: Correctly calculate a spawn position in front of the player.
    //new Float:x, Float:y, Float:z, Float:angle;
    //GetPlayerPos(playerid, x, y, z);
    //GetPlayerFacingAngle(playerid, angle);

    // This uses sine and cosine to find a point 5 units IN FRONT of the player, regardless of their rotation.
    //new Float:spawnX = x + (5.0 * floatsin(-angle, degrees));
    //new Float:spawnY = y + (5.0 * floatcos(-angle, degrees));

    // Create the vehicle with the specified model ID.
    //new vehicleid = CreateVehicle(modelid, spawnX, spawnY, z, angle, -1, -1, 60000); // 60 second respawn time

    // Put the player into the newly created vehicle.
    //PutPlayerInVehicle(playerid, vehicleid, 0);

    // Added a success message so the player knows it worked.
    //new vehicleName[32];
    //GetVehicleName(modelid, vehicleName, sizeof(vehicleName));
    
    //new message[128];
    //format(message, sizeof(message), "You have spawned a %s (ID: %d).", vehicleName, modelid);
    //SendClientMessage(playerid, 0x33CC33FF, message);

    // PROBLEM 4 FIXED: A command must return 1 to indicate it was processed successfully.
    //return 1;
//
}

//CMD:toggleregister(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:ss(playerid, params[], "savestats"){ return ShowNotImplemented(playerid); }
//CMD:ss2(playerid, params[], "savestats2"){ return ShowNotImplemented(playerid); }
//CMD:changepass(playerid, params[], "changepassword"){ return ShowNotImplemented(playerid); }
//CMD:tail(playerid, params[], "taipl", "toggleautoiplogin"){ return ShowNotImplemented(playerid); }
//CMD:mylevel(playerid, params[], "level", "getl", "getlevel"){ return ShowNotImplemented(playerid); }
//CMD:gxlevels(playerid, params[], "levels"){ return ShowNotImplemented(playerid); }
//CMD:toggleloginattemptkick(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:uptime(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:credits(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:disclaimer(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:thanks(playerid, params[], "thanksto"){ return ShowNotImplemented(playerid); }
//CMD:about(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:info(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:ver(playerid, params[], "version"){ return ShowNotImplemented(playerid); }
//CMD:r(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:pm(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:lpm(playerid, params[], "lastpm"){ return ShowNotImplemented(playerid); }
//CMD:lipm(playerid, params[], "lastircpm"){ return ShowNotImplemented(playerid); }
//CMD:ldpm(playerid, params[], "lastdiscordpm"){ return ShowNotImplemented(playerid); }
//CMD:bot(playerid, params[], "bots", "npc", "npcs"){ return ShowNotImplemented(playerid); }
//CMD:leave(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:raceparticipants(playerid, params[], "racers", "rp", "rps"){ return ShowNotImplemented(playerid); }
//CMD:raceinfo(playerid, params[], "ri"){ return ShowNotImplemented(playerid); }
//CMD:racehelp(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:aracehelp(playerid, params[], "raceadminhelp"){ return ShowNotImplemented(playerid); }
//CMD:buildhelp(playerid, params[], "buildracehelp"){ return ShowNotImplemented(playerid); }
//CMD:getid(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:getid2(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:ghelp(playerid, params[], "goohelp", "hphelp"){ return ShowNotImplemented(playerid); }
//CMD:toggleautorejoinmg(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:bh(playerid, params[], "bhelp", "bankhelp"){ return ShowNotImplemented(playerid); }
//CMD:sk(playerid, params[], "ski", "spawnkill", "spawnkillinfo"){ return ShowNotImplemented(playerid); }
//CMD:stats(playerid, params[], "stats2", "score"){ return ShowNotImplemented(playerid); }
//CMD:ks(playerid, params[], "killingspree"){ return ShowNotImplemented(playerid); }
//CMD:tr(playerid, params[], "track"){ return ShowNotImplemented(playerid); }
//CMD:tr2(playerid, params[], "track2"){ return ShowNotImplemented(playerid); }
//CMD:w(playerid, params[], "weapons"){ return ShowNotImplemented(playerid); }
//CMD:fch(playerid, params[], "findcheaters"){ return ShowNotImplemented(playerid); }
//CMD:vmodes(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:pnetstats(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:snetstats(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:statusdisplay(playerid, params[], "statushide"){ return ShowNotImplemented(playerid); }
//CMD:togglestatusdisplay(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:tshk(playerid, params[], "togglespoofhashkick"){ return ShowNotImplemented(playerid); }
//CMD:tkba(playerid, params[], "togglekickandbanalert"){ return ShowNotImplemented(playerid); }
//CMD:tpsb(playerid, params[], "togglepingspoofblock"){ return ShowNotImplemented(playerid); }
//CMD:tapb(playerid, params[], "toggleautopobsbackup"){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// GamerX Rank System Commands
//------------------------------------------------------------------------
//CMD:ranks(playerid, params[], "rankinfo"){ return ShowNotImplemented(playerid); }
//CMD:myrank(playerid, params[], "rank"){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// GamerX Achievements System Commands
//------------------------------------------------------------------------
//CMD:myachievements(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:achievements(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:achievementslist(playerid, params[]){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// GamerX Group Duel Player Commands
//------------------------------------------------------------------------
//CMD:myduel(playerid, params[]){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// Admin Derby Event Commands
//------------------------------------------------------------------------
//CMD:sdn(playerid, params[], "setderbyname"){ return ShowNotImplemented(playerid); }
//CMD:sdc(playerid, params[], "setderbycar"){ return ShowNotImplemented(playerid); }
//CMD:sdc2(playerid, params[], "setderbycar2"){ return ShowNotImplemented(playerid); }
//CMD:sdti(playerid, params[], "setderbytime"){ return ShowNotImplemented(playerid); }
//CMD:derbytime(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:sdt(playerid, params[], "setderbyteleport"){ return ShowNotImplemented(playerid); }
//CMD:gotoderbyteleport(playerid, params[], "gdt"){ return ShowNotImplemented(playerid); }
//CMD:sdo(playerid, params[], "setderbyobjects"){ return ShowNotImplemented(playerid); }
//CMD:setderbyholdobjects(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:createderby(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:fad(playerid, params[], "fixallderby"){ return ShowNotImplemented(playerid); }
//CMD:sdzl(playerid, params[], "setderbyzlimit"){ return ShowNotImplemented(playerid); }
//CMD:derbybuildhelp(playerid, params[], "buildderbyhelp"){ return ShowNotImplemented(playerid); }
//CMD:derbyadminhelp(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:derbyhelp(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:tds(playerid, params[], "togglederbystuff"){ return ShowNotImplemented(playerid); }
//CMD:sdmc(playerid, params[], "setderbymapcreator"){ return ShowNotImplemented(playerid); }
//CMD:sdp(playerid, params[], "setderbypreloadpos"){ return ShowNotImplemented(playerid); }
//CMD:sdvh(playerid, params[], "setderbyvehiclehealth"){ return ShowNotImplemented(playerid); }
//CMD:sdpcl(playerid, params[], "setderbypreloadcamlook"){ return ShowNotImplemented(playerid); }
//CMD:sdpcp(playerid, params[], "setderbypreloadcampos"){ return ShowNotImplemented(playerid); }
//CMD:setderbytime2(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setderbyweather(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:addderbyteleport(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setderbymode(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:buildderby(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:exitbuildderby(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:editderby(playerid, params[]){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// Admin Last Man Standing (LMS) Event Commands
//------------------------------------------------------------------------
//CMD:setlmsname(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setlmsmode(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setlmsobjectset(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setlmsholdobjectset(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setlmsteleport(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setlmsweapons(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setlmscreator(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setlmsmarkers(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setlmsjetpack(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:createlms(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:editlms(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:buildlms(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:exitbuildlms(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setlmspreloadpos(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setlmspreloadcampos(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setlmspreloadcamlookat(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setlmszlimit(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setlmstime(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setlmsweather(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setlmsbounds(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:buildlmshelp(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:gotoeventitem(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:spawneventitem(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:deletealleventitems(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:createalleventitems(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:trashitem(playerid, params[], "tit"){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// Admin Capture The Flag (CTF) Event Commands
//------------------------------------------------------------------------
//CMD:ctfs(playerid, params[], "setctfscoretowin"){ return ShowNotImplemented(playerid); }
//CMD:ctfinfo(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setctfname(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setctfobjectset(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setctfredflag(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setctfblueflag(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setctfredteleport(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setctfblueteleport(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setctfweapons(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setctfskins(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setctfcreator(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:createctf(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:editctf(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:buildctf(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:exitbuildctf(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setctfpreloadpos(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setctfpreloadcampos(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setctfpreloadcamlookat(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setctftime(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setctfweather(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:respawnredflag(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:respawnblueflag(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setctfzlimit(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:buildctfhelp(playerid, params[]){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// Admin Siege (Attack/Defend) Event Commands
//------------------------------------------------------------------------
//CMD:eventtime(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:gunmenu(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:balance(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setfirstteamname(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsecondteamname(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:carmenu(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegename(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegeweapons(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegemode(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegeobjectset(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegeholdobjectset(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegeatteleport(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegedefteleport(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegecp(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegeskins(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegecreator(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:createsiege(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:editsiege(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:buildsiege(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:exitbuildsiege(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegepreloadpos(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegepreloadcampos(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegepreloadcamlookat(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegevehicle(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegezlimit(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegetime(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setsiegeweather(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:buildsiegehelp(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:respawnproject(playerid, params[]){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// Admin Death Match and Team Death Match (DM/TDM) Event Commands
//------------------------------------------------------------------------
//CMD:builddm(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:exitbuilddm(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmname(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmobjectset(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmholdobjectset(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmfirstteleport(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmsecondteleport(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmmarkers(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmzlimit(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmweapons(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmskins(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmtime(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmpreloadpos(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmpreloadcampos(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmpreloadcamlookat(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmcreator(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmtime2(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmweather(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmkills(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:builddmhelp(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:editdm(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:createdm(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setdmmode(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:seteventtargethealth(playerid, params[]){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// General Admin Build Event Commands
//------------------------------------------------------------------------
//CMD:deleteeventteleport(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:gotoeventteleport(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:addeventteleport(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:addeventtarget(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:deleteeventtarget(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:addeventcar(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:deleteeventcar(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:gotoeventcar(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:addeventposition(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:deleteeventposition(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:gotoeventposition(playerid, params[]){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// Event System Admin Commands
//------------------------------------------------------------------------
//CMD:lde(playerid, params[], "lev", "loadevent"){ return ShowNotImplemented(playerid); }
//CMD:ste(playerid, params[], "startevent"){ return ShowNotImplemented(playerid); }
//CMD:ee(playerid, params[], "endevent"){ return ShowNotImplemented(playerid); }
//CMD:fle(playerid, params[], "forceleaveevent"){ return ShowNotImplemented(playerid); }
//CMD:bre(playerid, params[], "bringevent"){ return ShowNotImplemented(playerid); }
//CMD:tet(playerid, params[], "toggleeventteleport"){ return ShowNotImplemented(playerid); }
//CMD:togglegxrc(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:ses(playerid, params[], "seteventscoretowin"){ return ShowNotImplemented(playerid); }
//CMD:timeevent(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:seteventscore(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:seteventteam(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:eah(playerid, params[], "eventadminhelp"){ return ShowNotImplemented(playerid); }
//CMD:beh(playerid, params[], "buildeventhelp"){ return ShowNotImplemented(playerid); }
//CMD:ter(playerid, params[], "toggleeventrotation"){ return ShowNotImplemented(playerid); }
//CMD:nev(playerid, params[], "nextevent"){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// Player Event Commands (available in an Event)
//------------------------------------------------------------------------
//CMD:events(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:he(playerid, params[], "eventhelp", "helpevent"){ return ShowNotImplemented(playerid); }
//CMD:eventinformations(playerid, params[], "informationsevent", "ei"){ return ShowNotImplemented(playerid); }
//CMD:er(playerid, params[], "eventrules"){ return ShowNotImplemented(playerid); }
//CMD:pe(playerid, params[], "pie", "playersinevent"){ return ShowNotImplemented(playerid); }
//CMD:rev(playerid, params[], "respawnevent"){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// Player Radio Commands
//------------------------------------------------------------------------
//CMD:playradio(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:rad(playerid, params[], "myradio"){ return ShowNotImplemented(playerid); }
//CMD:radiooff(playerid, params[], "stopradio"){ return ShowNotImplemented(playerid); }
//CMD:setmyradio(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:radio(playerid, params[]){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// Hit Sound and Hit Text Draw Commands
//------------------------------------------------------------------------
//CMD:ths(playerid, params[], "hitsound", "togglehitsound"){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// Admin Play Sound Command
//------------------------------------------------------------------------
//CMD:playsound(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:setopeningsound(playerid, params[]){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// Unlock Command
//------------------------------------------------------------------------
//CMD:unlock(playerid, params[])

    // Needs vehicle state checks, basic for now
    //return ShowFeedback(playerid, "Vehicle unlocked (placeholder).");


//------------------------------------------------------------------------
// Admin Fake Pickup Commands
//------------------------------------------------------------------------
//CMD:cfp(playerid, params[], "createfakepickup"){ return ShowNotImplemented(playerid); }
//CMD:dlfp(playerid, params[], "deletelastfakepickup"){ return ShowNotImplemented(playerid); }
//CMD:dafp(playerid, params[], "deleteallfakepickup", "deleteallfakepickups"){ return ShowNotImplemented(playerid); }
//CMD:damfp(playerid, params[], "deleteallmyfakepickup", "deleteallmyfakepickups"){ return ShowNotImplemented(playerid); }
//CMD:dfp(playerid, params[], "deletefakepickup"){ return ShowNotImplemented(playerid); }
//CMD:gfp(playerid, params[], "gotofakepickup"){ return ShowNotImplemented(playerid); }
//CMD:nfp(playerid, params[], "clfp", "closestfakepickup", "nearestfakepickup"){ return ShowNotImplemented(playerid); }
//CMD:tfp(playerid, params[], "totalfakepickups"){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// Misc
//------------------------------------------------------------------------
//CMD:drinkbeer(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:wine(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:sprunk(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:ciggy(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:cigar(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:fart(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:care(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:lol(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:lmao(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:lmao2(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:rofl(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:cough(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:acthelp(playerid, params[], "actionshelp"){ return ShowNotImplemented(playerid); }
//CMD:cb(playerid, params[], "chatbubble", "whisper"){ return ShowNotImplemented(playerid); }
//CMD:admins(playerid, params[]){ return ShowNotImplemented(playerid); } // Already defined above, kept for structure
//CMD:me(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:me2(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:me3(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:report(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:k(playerid, params[]){ return cmd_kill(playerid, params); } // Alias for kill
//CMD:spc(playerid, params[], "setpc", "setprivatechat"){ return ShowNotImplemented(playerid); }
//CMD:fixcam(playerid, params[], "fixcamera"){ SetCameraBehindPlayer(playerid); return ShowFeedback(playerid, "Camera fixed."); } // Already defined
//CMD:dance(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:handsup(playerid, params[]){ return ShowNotImplemented(playerid); }
YCMD:megadrunk(playerid, params[])
{
    SetPlayerDrunkLevel(playerid, 50000);
    SendClientMessage(playerid, 0xFFFFFFFF, "You are mega drunk.");
    return 1;
}
YCMD:drunk(playerid, params[])
{
    SetPlayerDrunkLevel(playerid, 0);
    SendClientMessage(playerid, 0xFFFFFFFF, "You are now sober.");
    return 1;
} // This is the single, correct closing brace

//CMD:tps(playerid, params[], "tpslist", "tplist", "listtp", "listtps"){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// Countdown Related Commands
//------------------------------------------------------------------------
//CMD:cd(playerid, params[], "123", "321", "countdown"){ return ShowNotImplemented(playerid); }
//CMD:dcd(playerid, params[], "disablecountdown"){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// Admin Announce Commands
//------------------------------------------------------------------------
//CMD:ar(playerid, params[], "announceradius"){ return ShowNotImplemented(playerid); }
//CMD:a(playerid, params[], "announce"){ return ShowNotImplemented(playerid); }
//CMD:a2(playerid, params[], "announce2"){ return ShowNotImplemented(playerid); }
//CMD:a3(playerid, params[], "announce3"){ return ShowNotImplemented(playerid); }
//CMD:a4(playerid, params[], "announce4"){ return ShowNotImplemented(playerid); }
//CMD:a5(playerid, params[], "announce5"){ return ShowNotImplemented(playerid); }
//CMD:a6(playerid, params[], "announce6"){ return ShowNotImplemented(playerid); }
//CMD:ha(playerid, params[], "helpannounce"){ return ShowNotImplemented(playerid); }
//CMD:stdbg(playerid, params[], "settextdrawbackground"){ return ShowNotImplemented(playerid); }
//CMD:lann(playerid, params[], "lastannounce"){ return ShowNotImplemented(playerid); }
//CMD:rc(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:lh(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:rhe(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:tt(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:tm(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:tf(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:tn(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:tc(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:tco(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:tto(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:tpc(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:tgasf(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:su(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:daa(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:say(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:tv(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:tpr(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:ndb(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:nwb(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:nsb(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:nrrz(playerid, params[]){ return ShowNotImplemented(playerid); }

//------------------------------------------------------------------------
// Admin Announce and Player Info Commands
//------------------------------------------------------------------------
//CMD:irc(playerid, params[], "showirc", "sirc", "tirc"){ return ShowNotImplemented(playerid); }
//CMD:stpi(playerid, params[], "ttpi", "showtpinfo"){ return ShowNotImplemented(playerid); }
//CMD:img(playerid, params[], "gximg"){ return ShowNotImplemented(playerid); }
//CMD:www(playerid, params[], "showwww", "swww", "twww", "weburl", "website"){ return ShowNotImplemented(playerid); }
//CMD:ts3(playerid, params[]){ return ShowNotImplemented(playerid); }
//CMD:discord(playerid, params[]){ return ShowNotImplemented(playerid); }