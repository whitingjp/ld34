#ifndef MISSION_H_
#define MISSION_H_

#define NUM_STATIONS (6)

typedef enum
{
	GOOD_NONE,
	GOOD_SNACK,
	GOOD_CRAB,
	GOOD_TRACTOR,
	NUM_GOODS,
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


typedef enum
{
	NO_REPLACE,
	REPLACE_ON_ANY_ANSWER,
	REPLACE_ON_ACCEPTED_LAUNCH,
	REPLACE_ON_MET_NEED,
} mission_replacement_type;

typedef struct
{
	mission_page need_page;
	mission_page have_page;
	mission_page accepted;
	mission_page rejected;
	mission_trade need;
	mission_trade have;
	whitgl_int replacement;
	mission_replacement_type replacement_type;
	whitgl_int known_stations[NUM_STATIONS];
} mission_mission;

typedef enum
{
	MISSION_INTRO,
	MISSION_LAUNCH,
	MISSION_SNACK,
	MISSION_FUNDS,
	MISSION_RESTORED,
	MISSION_YUCTIS,
	MISSION_TRACTOR,
	MISSION_ALCLOV,
	MISSION_OSKAO,
	MISSION_TAGAWA,
	NUM_MISSIONS,
	MISSION_RESUME
} mission_index;


static const mission_mission kMissions[NUM_MISSIONS] =
{
	{ // MISSION_INTRO
		{"","","",""},
		{"Well howdy youngster. You want in on the ol' asteroid mining business do ya?\n\nWell suuure, I can help, but first, you wouldn't mind popping across the way to grab me some Calamari, would ya?\n\nThe diner is at Hutov, y'know, the nice place", "No problem", "Ugh, fine!", ""},
		{"","","","launch"},
		{"","","","launch"},
		{GOOD_NONE, 0},
		{GOOD_NONE, 0},
		MISSION_LAUNCH,
		REPLACE_ON_ANY_ANSWER,
		{0,1,-1},
	},
	{ // MISSION_LAUNCH
		{"I hope someones taught you how to steer that old boat?\n\nNo?\n\nWell it's simple enough, left is left, and the other one is, uhh, y'know.. it's.. the other one\n\nOh, and yeah, you launch from a station by holding both directions at once. That one is pretty important too.\n\nStop dilly dallying, get yourself to Hutov", "", "", "launch"},
		{"Do I smell Calamari? It's about time too!\n\nHere I am, all alone, hungry, and with a business to run\n\nHand it over then","here gran","get your own","launch"},
		{"Okay, first you'll need to hire a big 'ol tractor beam to start snagging asteroids, but they cost big bucks\n\n$50 or so should be enough\n\nTry selling Calamari at Yutis, or if you're feeling adventurous run some red crabs from Alclov to Oskao\n\nI'll lend you $10 to get started. Come back when you have enough","","","launch"},
		{"Not ever in my life. Come back when you got some manners girl.","","","launch"},
		{GOOD_SNACK, 0},
		{GOOD_NONE, 10},
		MISSION_FUNDS,
		REPLACE_ON_ACCEPTED_LAUNCH,
		{-1},
	},
	{ // MISSION_SNACK
		{"Darling, I'd really love to help, but you know how it is. I've got bills to pay, and Tracy isn't getting any better.\n\nSo it's 4 creds for calamari or nothing for nothing I'm afraid.\n\nI hope you can get that together somehow","","","launch"},
		{"Helloooo! It's been too long xxx\n\nCalamari eh? Your gran sure does love that stuff\n\nThat'll be $4 for you darling.\n\n", "thanks kate", "oh! brb", "just leave"},
		{"Come back soon girl, we need a proper catch up sometime soon xxx\n\n","","","launch"},
		{"Ok, laterz","","","launch"},
		{GOOD_NONE, 4},
		{GOOD_SNACK, 0},
		NUM_MISSIONS,
		NO_REPLACE,
		{-1},
	},
	{ // MISSION_FUNDS
		{"You'll need more than that to hire yourself a tractor beam!\n\n$50 or so should be enough\n\nTry selling Calamari at Yutis, or if you're feeling adventurous run some red crabs from Alclov to Oskao\n\nCome back when you have enough","","","launch"},
		{"","","","launch"},
		{"","","","launch"},
		{"","","","launch"},
		{GOOD_NONE, 50},
		{GOOD_NONE, 50},
		MISSION_TRACTOR,
		REPLACE_ON_MET_NEED,
		{2,3,4,-1},
	},
	{ // MISSION_RESTORED
		{"","","","launch"},
		{"Oh jeez. Why'd you have to go and smear your dead body all over the system? Well why'd'ya?\n\nI know I know, you don't remember nothing. It wasn't -you- that did it\n\nGood thing we have a clone-o-mat! Take care o'yourself this time whydontcha?", "", "", "launch"},
		{"","","","launch"},
		{"","","","launch"},
		{GOOD_NONE, 0},
		{GOOD_NONE, 0},
		MISSION_RESUME,
		REPLACE_ON_ANY_ANSWER,
		{-1},
	},
	{ // MISSION_YUCTIS
		{"Yo! You live at Zunus station, near Hutov right?\n\nThe food here is super terrible. Hook us up with some of that good Hutov calamari and we'll pay well\n\nHow does $20 for a hold full sound to ya?","","","launch"},
		{"Oh, wow, you got some of that Hutov calamari? That's just swell.\n\n$20 big ones for the hold full sound alright?\n\n", "sure", "no thanks", "launch"},
		{"Great, thanks sister.\n\nI'm sure we'd happily take more if you need to get rid of it.\n\n","","","launch"},
		{"Ah shame, come back if you ever feel like sharing the wealth.","","","launch"},
		{GOOD_SNACK, 0},
		{GOOD_NONE, 20},
		NUM_MISSIONS,
		NO_REPLACE,
		{-1},
	},
	{ // MISSION_TRACTOR
		{"Okay, so, the best heavy machinery place I knows is down in the old country.\n\nTry Tagawa\n\nThey know one end of a tractor from the other down there\n\nyeah, I know they're robots, but robots aint bad people","","","launch"},
		{"Well done missy, we'll soon make an asteroid miner out of you!\n\nsadly jonathan didn't get a chance to work on any further plot points, sorry!\n\nthe tractor beam sure is fun and dangerous though, right?", "", "", "play"},
		{"","","","launch"},
		{"","","","launch"},
		{GOOD_TRACTOR, 0},
		{GOOD_NONE, 0},
		NUM_MISSIONS,
		NO_REPLACE,
		{5,-1},
	},
	{ // MISSION_ALCLOV
		{"Yeah cuz, we might have red crabs, or we might not. Show us some holobacked $$$ and we can talk\n\nIf you don't have ooh, at least $20, it's best you just leave now.\n\nif Pol finds me selling for less.. well.. she won't be happy\n\n","","","get out"},
		{"You got $20? Sure, that'll get you some of the red stuff.\n\nJust if anybody asks, you didn't get it from me. Right?\n\n", "here's $20", "maybe later", "get out"},
		{"Right, pleasure doing business with you\n\nI wouldn't stick around too long\n\nPol said she'd be getting back pretty soon, and she sounded pissed.","","","flee"},
		{"Stop wasting my time cuz","","","launch"},
		{GOOD_NONE, 20},
		{GOOD_CRAB, 0},
		NUM_MISSIONS,
		NO_REPLACE,
		{-1},
	},
	{ // MISSION_OSKAO
		{"Heyyyy, don't suppose you could hook me up with some of the red stuff\n\nCan't get enough these days\n\nYou can't?\n\nWell, if you ever can I can maybe pay $50 if it's good stuff","","","launch"},
		{"You got crabs?\n\nYES!\n\nI can give you $50 now, or, like, if you're happy for me to owe you, maybe $70 next week", "$50 now", "nah", "launch"},
		{"Hey, I owe you one. Come round any time, we can get cosy, watch some holos, share a claw?","","","ugh, no!"},
		{"Hey, why'd you even come here?\n\nYou're no fun at all.\n\nDon't bother coming back","","","launch"},
		{GOOD_CRAB, 0},
		{GOOD_NONE, 50},
		NUM_MISSIONS,
		NO_REPLACE,
		{-1},
	},
	{ // MISSION_TAGAWA
		{"hello flesh creature\n\ni sense that you would like to hire a tractor beam\n\nyou are currently unable to pay me\n\nit costs $50\n\ndo you have a fault?\n\n does your cpu need replacing?","","","back away"},
		{"greetings fleshling\n\nif you are about to give us $50 we will have already installed a tractor beam to your ship", "ok..?", "umm, no", "launch"},
		{"excellent\n\ni am certain you will find it satisfactory","","","launch"},
		{"you will be back soon enough","","","launch"},
		{GOOD_NONE, 50},
		{GOOD_TRACTOR, 0},
		NUM_MISSIONS,
		NO_REPLACE,
		{-1},
	},
	// { // MISSION_
	// 	{"","","","launch"},
	// 	{"", "", "", "launch"},
	// 	{"","","","launch"},
	// 	{"","","","launch"},
	// 	{GOOD_NONE, 0},
	// 	{GOOD_NONE, 0},
	// 	NUM_MISSIONS,
	// 	NO_REPLACE,
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

