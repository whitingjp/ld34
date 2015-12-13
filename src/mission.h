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
	NUM_GOODS,
} mission_good;

static const char* mission_good_names[NUM_GOODS] =
{
	"Empty",
	"Snack",
	"Trade2",
	"Tractor",
	"Seed",
	"Plant",
};

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
	mission_page accepted;
	mission_page rejected;
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
		{"Hey, I want to get started, but first can you nip to Zunus and get me a snack.", "Sure thing", "Ugh fine!", ""},
		{"","","","launch"},
		{"","","","launch"},
		{GOOD_NONE, 0},
		{GOOD_NONE, 0},
		MISSION_LAUNCH,
		true,
	},
	{ // MISSION_LAUNCH
		{"you need to get yourself to Zunus and pick me up a snack\n\nto leave the station hold left and right", "", "", "launch"},
		{"great, thanks, so what we're going to do now is...","","","launch"},
		{"","","","launch"},
		{"","","","launch"},
		{GOOD_SNACK, 0},
		{GOOD_NONE, 10},
		NUM_MISSIONS,
		true,
	},
	{ // MISSION_SNACK
		{"not even 4 creds to your name, youre gonna need some more","","","launch"},
		{"get your snacks, only 4 creds!", "yes please", "bit steep", "launch"},
		{"pleasure doing buisness with you","","","launch"},
		{"well, come back later if you scrapes some pennies together","","","launch"},
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

