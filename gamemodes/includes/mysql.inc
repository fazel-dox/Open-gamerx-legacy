/*================================================================================
    === START OF FILE: gamemodes\includes\mysql.inc ===
================================================================================*/

/*
    ================================================================================
    Project:    GamerX Rebirth - MySQL Core
    File:       mysql.inc
    Author:     Paniz & AI Assistant
    Description: Centralizes all database connection logic for the server.
    ================================================================================
*/

#if defined _mysql_included
    #endinput
#endif // FIX: Added the missing #endif. This was a critical preprocessor bug.
#define _mysql_included

// FIX: This line is required for any of the mysql_* functions to be recognized.
#include <a_mysql>

// The one and only global variable for our database connection.
new MySQL:g_dbConnection;


// The single function to connect to the database.
stock MySQL_Connect()
{
    // Connection details.
    new host[] = "localhost";
    new user[] = "root";
    new pass[] = "";
    new db[]   = "gamerx_db";

    g_dbConnection = mysql_connect(host, user, pass, db);

    // This check is vital. If it fails, the server stops with a clear error.
    if (!g_dbConnection)
    {
        print("[FATAL ERROR] Database connection failed. The server cannot continue.");
        print("[FATAL ERROR] Please check your mysql.inc settings and that the database server is running.");
        SendRconCommand("exit");
        return 0;
    }
    
    print("[SUCCESS] Database connection established.");
    return 1;
}

/*================================================================================
    === END OF FILE: gamemodes\includes\mysql.inc ===
================================================================================*/