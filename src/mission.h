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
} mission_page;

typedef struct
{
	mission_page need_page;
	mission_page have_page;
	mission_trade need;
	mission_trade have;
	whitgl_int yes_replacement;
	whitgl_int no_replacement;
} mission_mission;

typedef enum
{
	INTRO,
	NUM_MISSIONS
} mission_index;

static const mission_mission missions[NUM_MISSIONS] =
{
	{ // INTRO
		{"Hey, I want to get started, but first can you nip to xyz and get me a snack", "Ok", "No Way"},
		{"Great, hand it over", "Ok", "No Way"},
		{GOOD_SNACK, 0},
		{GOOD_NONE, 10},
		-1,
		-1,
	},
};

#endif // MISSION_H_
