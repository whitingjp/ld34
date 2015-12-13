#ifndef MISSION_H_
#define MISSION_H_

typedef enum
{
	GOOD_NONE,
	GOOD_SNACK,
	GOOD_TRADE2,
	GOOD_TRACTOR,
	GOOD_SEED,
	GOOD_PLANT,
} mission_good;

typedef struct
{
	mission_good good;
	whitgl_int creds;
} mission_trade;

#define MAX_TEXT_LEN (512)
typedef struct
{
	char text[MAX_TEXT_LEN];
	char left[MAX_TEXT_LEN];
	char right[MAX_TEXT_LEN];
	char launch[MAX_TEXT_LEN];
} mission_page;

typedef struct
{
	mission_page need_page;
	mission_page have_page;
	mission_trade need;
	mission_trade have;
	whitgl_int replacement;
	whitgl_bool always_yes;
} mission_mission;

typedef enum
{
	MISSION_INTRO,
	MISSION_LAUNCH,
	MISSION_SNACK,
	NUM_MISSIONS
} mission_index;

static const mission_mission kMissions[NUM_MISSIONS] =
{
	{ // MISSION_INTRO
		{"","","",""},
		{"Hey, I want to get started, but first can you nip to xyz and get me a snack.", "Sure thing", "Ugh fine!", ""},
		{GOOD_NONE, 0},
		{GOOD_NONE, 0},
		MISSION_LAUNCH,
		true,
	},
	{ // MISSION_LAUNCH
		{"you need to get yourself to xyz and pick me up a snack\n\nto leave the station hold left and right", "", "", "launch"},
		{"great, thanks, so what we're going to do now is...","","","launch"},
		{GOOD_SNACK, 0},
		{GOOD_NONE, 10},
		NUM_MISSIONS,
		true,
	},
	{ // MISSION_SNACK
		{"not even 4 creds to your name, youre gonna need some more","","","launch"},
		{"get your snacks, only 4 creds!", "yes please", "bit steep", "launch"},
		{GOOD_NONE, 4},
		{GOOD_SNACK, 0},
		NUM_MISSIONS,
		false,
	},
};


		// "the quick brown.\n"
		// "fox stole all,\n"
		// "of my cargo!\n"
		// "\n"
		// "dammit$\n"
		// "\n"
		// "hey\n"
		// "\n"
		// "i dont suppose\n"
		// "you could help\n"
		// "out\n"
		// "\n"
		// "i will' make it\n"
		// "worthwhile\n";

#endif // MISSION_H_

