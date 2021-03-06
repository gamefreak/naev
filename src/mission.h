/*
 * See Licensing and Copyright notice in naev.h
 */


#ifndef MISSION_H
#  define MISSION_H


#include "nlua_misn.h"

#include "opengl.h"


/* availability by location */
#define  MIS_AVAIL_NONE       0 /**< Mission isn't available. */
#define  MIS_AVAIL_COMPUTER   1 /**< Mission is available at mission computer. */
#define  MIS_AVAIL_BAR        2 /**< Mission is available at bar. */
#define  MIS_AVAIL_OUTFIT     3 /**< Mission is available at outfitter. */
#define  MIS_AVAIL_SHIPYARD   4 /**< Mission is available at shipyard. */
#define  MIS_AVAIL_LAND       5 /**< Mission is available on landing. */
#define  MIS_AVAIL_COMMODITY  6 /**< Mission is available at commodity exchange. */


/* flag functions */
#define mis_isFlag(m,f)    ((m)->flags & (f))
#define mis_setFlag(m,f)   ((m)->flags |= (f))
#define mis_rmFlag(m,f)    ((m)->flags &= ~(f))
/* actual flags */
#define MISSION_UNIQUE        (1<<0) /**< Unique missions can't be repeated */

#define MISSION_TIMER_MAX     10 /**< Maximum amount of timers in a mission. */


/**
 * @brief Different type of system markers.
 */
typedef enum SysMarker_ {
   SYSMARKER_MISC, /**< Miscellaneous marker. */
   SYSMARKER_RUSH, /**< Rush mission marker. */
   SYSMARKER_CARGO /**< Cargo mission marker. */
} SysMarker;


/**
 * @brief Defines the availability of a mission.
 */
typedef struct MissionAvail_s {
   int loc; /**< Location of the mission. */
   int chance; /**< Chance of it appearing, last two digits represent %, first digit represents times it can appear (if 0 it behaves like once). */

   /* for specific cases */
   char *planet; /**< Planet name. */
   char *system; /**< System name. */

   /* for generic cases */
   int* factions; /**< To certain factions. */
   int nfactions; /**< Number of factions in factions. */

   char* cond; /**< Condition that must be met (Lua). */
   char* done; /**< Previous mission that must have been done. */

   int priority; /**< Mission priority: 0 = main plot, 5 = default, 10 = insignificant. */
} MissionAvail_t;


/**
 * @struct MissionData
 *
 * @brief Static mission data.
 */
typedef struct MissionData_ {
   char *name; /**< The name of the mission. */

   MissionAvail_t avail; /**< Mission availability. */

   unsigned int flags; /**< Flags to store binary properties */
   char* lua; /**< Lua file to use. */
} MissionData;


/**
 * @struct Mission
 *
 * @brief Represents an active mission.
 */
typedef struct Mission_ {
   MissionData *data; /**< Data to use. */
   unsigned int id; /**< Unique mission identifier, used for keeping track of hooks. */
   int accepted; /**< Mission is a player mission. */

   char *title; /**< Not to be confused with name */
   char *desc; /**< Description of the mission */
   char *reward; /**< Rewards in text */
   glTexture *portrait; /**< Portrait of the mission giver if applicable. */
   char *npc; /**< Name of the NPC giving the mission. */

   /* mission cargo given to the player - need to cleanup */
   unsigned int *cargo; /**< Cargos given to player. */
   int ncargo; /**< Number of cargos given to player. */

   char *sys_marker; /**< System to mark. */
   SysMarker sys_markerType; /**< Type of the marker. */

   /* Timers. */
   double timer[MISSION_TIMER_MAX]; /**< Mission timers. */
   char *tfunc[MISSION_TIMER_MAX]; /**< Functions assosciated to the timers. */

   /* OSD. */
   unsigned int osd; /**< On-Screen Display ID. */
   int osd_set; /**< OSD was set explicitly. */

   lua_State *L; /**< The state of the running lua code. */
} Mission;


/*
 * current player missions
 */
#define MISSION_MAX  12 /**< No sense in allowing the player have infinite missions. */
extern Mission player_missions[MISSION_MAX]; /**< Player's active missions. */


/*
 * creates missions for a planet and such
 */
Mission* missions_genList( int *n, int faction,
      const char* planet, const char* sysname, int loc );
int mission_accept( Mission* mission ); /* player accepted mission for computer/bar */
void missions_run( int loc, int faction, const char* planet, const char* sysname );
int mission_start( const char *name );

/*
 * misc
 */
void missions_update( const double dt );
int mission_getID( const char* name );
MissionData* mission_get( int id );
void mission_sysMark (void);
void mission_sysComputerMark( Mission* misn );


/*
 * cargo stuff
 */
int mission_linkCargo( Mission* misn, unsigned int cargo_id );
int mission_unlinkCargo( Mission* misn, unsigned int cargo_id );


/*
 * load/quit
 */
int missions_load (void);
void mission_cleanup( Mission* misn );
void missions_free (void);
void missions_cleanup (void);

/*
 * Actually in nlua_misn.h
 */
int misn_tryRun( Mission *misn, const char *func );
lua_State *misn_runStart( Mission *misn, const char *func );
int misn_runFunc( Mission *misn, const char *func, int nargs );
int misn_run( Mission *misn, const char *func );


#endif /* MISSION_H */


