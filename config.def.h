/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

#define COL_GRAY1 "#282a36"
#define COL_GRAY3 "#96b5b4"
#define COL_GRAY5 "#44475a"
#define COL_CYAN  "#00ffff"


/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10", "fontawesome:size=12", "Firacode:size=13" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = COL_GRAY1;
static const char col_gray2[]       = "#111111";
static const char col_gray3[]       = COL_GRAY3;
static const char col_gray4[]       = "#d7d7d7";
static const char col_gray5[]       = COL_GRAY5;
static const char col_cyan []       =  COL_CYAN;
static const char col_white[]       = "#ffffff";
static const char col_black[]       = "#000000";
static const char col_lime []       = "#bfff00";
static const char col_red  []       = "#df2331";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
        [SchemeNorm]      = { col_gray3, col_gray1,    col_red  },
        [SchemeSel]       = { col_gray4,  col_cyan,   col_cyan  },
        [SchemeStatus]    = { col_gray3, col_gray1,   "#000000" }, // Statusbar right {text,background,not used but cannot be empty}
        [SchemeTagsSel]   = { col_gray1, col_gray3,   "#000000" }, // Tagbar left selected {text,background,not used but cannot be empty}
        [SchemeTagsNorm]  = { col_gray3, col_gray1,   "#000000" }, // Tagbar left unselected {text,background,not used but cannot be empty}
        [SchemeInfoSel]   = {  col_cyan, col_gray1,   "#000000" }, // infobar middle  selected {text,background,not used but cannot be empty}
        [SchemeInfoNorm]  = { col_gray3, col_gray1,   "#000000" }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", ""};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	// if tag and swallow both are given --> then tag is used ( swallow would not work,
	//   even when spawning terminal is on spawn tag of app )
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,        0,          -1,        -1 },
	{ "st-256color",  NULL,   NULL,       0,            0,        1,          -1,        -1 },
	{ NULL,       NULL,   "Event Tester", 0,            0,        0,           1,        -1 }, /* xev */
	{ "firefox",  NULL,       NULL,       1 << 1,       0,        0,          -1,        -1 },
	{ "Google-chrome",  NULL, NULL,       1 << 1,       0,        0,          -1,        -1 },
	{ "Chromium", NULL,       NULL,       1 << 1,       0,        0,          -1,        -1 },
	{ "Steam",    NULL,       NULL,       1 << 3,       0,        0,          -1,        -1 },
	{ "VirtualBox Manager",   NULL, NULL, 1 << 4,       0,        0,          -1,        -1 },
	{ "TelegramDesktop", NULL,NULL,       1 << 5,       0,        0,          -1,        -1 },
	{ "discord",  NULL,       NULL,       1 << 5,       0,        0,           0,        -1 },
	{ NULL,  NULL,  "Spotify Free",       1 << 6,       0,        0,          -1,        -1 },
	{ "obs",      NULL,       NULL,       1 << 7,       0,        0,          -1,        -1 },
	{ "Thunar",   NULL,       NULL,       1 << 8,       0,        0,          -1,        -1 },
	{ "Anaconda-Navigator",   NULL, NULL, 1 << 8,       0,        0,          -1,        -1 },
	{ "Spyder",   NULL,       NULL,       1 << 2,       0,        0,          -1,        -1 },
	{ "Code",     NULL,       NULL,       1 << 2,       0,        0,          -1,        -1 },
	{ "Eclipse",  NULL,       NULL,       1 << 2,       0,        0,          -1,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* audio control */
static const char *upvol  [] = { "/usr/bin/pactl", "set-sink-volume", "0",    "+5%", NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0",    "-5%", NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute"  , "0", "toggle", NULL };

/* media control */
static const char *playerplaypause[] = {"playerctl", "play-pause", NULL };
static const char *playernext     [] = {"playerctl", "next", NULL };
static const char *playerprev     [] = {"playerctl", "previous", NULL };
static const char *playerstop     [] = {"playerctl", "stop", NULL };

/* screen lock */
static const char *screenlock[] = { "/usr/bin/xlock", NULL } ;

/* brightness */
static const char *screenbrightness_up[]   = { "sudo", "/usr/bin/light", "-A", "4", NULL};
static const char *screenbrightness_down[] = { "sudo", "/usr/bin/light", "-U", "4", NULL};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_gray5, "-sf", col_cyan, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *vim[]  = { "st", "-e", "vim" };
static const char *filemanager[] = { "st", "-e",  "ranger", NULL };
static const char *screenshot [] = { "xfce4-screenshooter", NULL };
static const char *emojiboard [] = { "rofimoji", "-a", "type", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_w,      spawn,          SHCMD("\
			case $(echo -e 'firefox\nchromium\nchrome' | dmenu -i -l 5 -p 'browser' -nb '" COL_GRAY1 "' -nf '" COL_GRAY3 "' -sb '" COL_GRAY5 "' -sf '" COL_CYAN "') in \
				chrome)   google-chrome-stable ;;\
				firefox)  firefox ;;\
				chromium) chromium ;;\
			esac") },
	{ MODKEY|ShiftMask,             XK_f,      spawn,          {.v = filemanager } },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD("env LD_PRELOAD=/usr/lib/spotify-adblock.so spotify %U") },
	{ MODKEY|ShiftMask,             XK_v,      spawn,          {.v = vim } },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          {.v = emojiboard } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = screenshot } },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	// volume control
	{ 0,                XF86XK_AudioMute,      spawn,          {.v = mutevol} },
	{ 0,         XF86XK_AudioLowerVolume,      spawn,          {.v = downvol} },
	{ 0,         XF86XK_AudioRaiseVolume,      spawn,          {.v = upvol  } },
	// media
	{ 0,                XF86XK_AudioPlay,      spawn,          {.v = playerplaypause   } },
	{ 0,                XF86XK_AudioPrev,      spawn,          {.v = playerprev        } },
	{ 0,                XF86XK_AudioNext,      spawn,          {.v = playernext        } },
	{ 0,                XF86XK_AudioStop,      spawn,          {.v = playerstop        } },
	// screen
	{ ShiftMask, XF86XK_AudioLowerVolume,      spawn,          {.v = screenbrightness_down } },
	{ ShiftMask, XF86XK_AudioRaiseVolume,      spawn,          {.v = screenbrightness_up   } },
	{ ShiftMask,        XF86XK_AudioPlay,      spawn,          {.v = screenlock            } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

