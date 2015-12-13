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
	"Calamari",
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
	MISSION_FUNDS,
	NUM_MISSIONS
} mission_index;

static const mission_mission kMissions[NUM_MISSIONS] =
{
	{ // MISSION_INTRO
		{"","","",""},
		{"Well howdy youngster. You want in on the ol' asteroid farming business do ya?\n\nWell suuure, I can help, but first, you wouldn't mind popping across the way to grab me some Calamari, would ya?\n\nThe diner is at Hutov, y'know, the nice place", "No problem", "Ugh, fine!", ""},
		{"","","","launch"},
		{"","","","launch"},
		{GOOD_NONE, 0},
		{GOOD_NONE, 0},
		MISSION_LAUNCH,
		true,
	},
	{ // MISSION_LAUNCH
		{"I hope someones taught you how to steer that old boat?\n\nNo?\n\nWell it's simple enough, left is left, and the other one is, uhh, y'know.. it's.. the other one\n\nOh, and yeah, you launch from a station by holding both directions at once. That one is pretty important too.\n\nStop dilly dallying, get yourself to Hutov", "", "", "launch"},
		{"Do I smell Calamari? It's about time too!\n\nHere I am, all alone, hungry, and with a business to run\n\nHand it over then","here gran","get your own","launch"},
		{"Okay, first you'll need to hire a big 'ol tractor beam to snag you an asteroid, but they cost big bucks\n\n$50 or so should be enough\n\nTry selling Calamari at Yutis, or if you're feeling adventurous run some space weed from Alclov to Seue\n\nI'll lend you $10 to get started. Come back when you have enough","","","launch"},
		{"Not ever in my life. Come back when you got some manners girl.","","","launch"},
		{GOOD_SNACK, 0},
		{GOOD_NONE, 10},
		MISSION_FUNDS,
		false,
	},
	{ // MISSION_SNACK
		{"Darling, I'd really love to help, but you know how it is. I've got bills to pay, and Tracy isn't getting any better.\n\nSo it's 4 creds for calamari or nothing for nothing I'm afraid.\n\nI hope you can get that together somehow","","","launch"},
		{"Helloooo! It's been too long xxx\n\nCalamari eh? Your gran sure does love that stuff\n\nThat'll be $4 for you darling.\n\n", "thanks kate", "oh! brb", "launch"},
		{"Come back soon girl, we need a proper catch up sometime soon xxx\n\n","","","launch"},
		{"Ok, laterz","","","launch"},
		{GOOD_NONE, 4},
		{GOOD_SNACK, 0},
		NUM_MISSIONS,
		false,
	},
	{ // MISSION_FUNDS
		{"That's not enough!","","","launch"},
		{"Okay, so", "", "", "launch"},
		{"","","","launch"},
		{"","","","launch"},
		{GOOD_NONE, 50},
		{GOOD_NONE, 40},
		NUM_MISSIONS,
		false,
	},
	// { // MISSION_
	// 	{"","","","launch"},
	// 	{"", "", "", "launch"},
	// 	{"","","","launch"},
	// 	{"","","","launch"},
	// 	{GOOD_NONE, 0},
	// 	{GOOD_NONE, 0},
	// 	NUM_MISSIONS,
	// 	false,
	// },
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

