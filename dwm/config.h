/* See LICENSE file for copyright and license details. */

/* appearance */
static const char *fonts[] = {
	"monospace:size=12"
};
static const char dmenufont[]       = "monospace:size=14";
static const char normbordercolor[] = "#444444";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#bbbbbb";
static const char selbordercolor[]  = "#606060";
/* static const char selbgcolor[]      = "#005577"; */
static const char selbgcolor[]      = "#606060";
static const char selfgcolor[]      = "#eeeeee";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

/* tagging */
#define MAX_TAGLEN 16
/* static char tags[][MAX_TAGLEN] = { "1",   "2",  "3", "4", "5", "6",   "7",   "8",   "9" }; */
  static char tags[][MAX_TAGLEN] = { "sys", "dev", "3", "4", "5", "6",  "vmx", "snd", "net" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class              instance            title       tags mask     isfloating   monitor */
	{ "Gimp",             NULL,               NULL,       0,            1,           -1 },
	{ "Firefox",          NULL,               NULL,       1 << 8,       0,           -1 },
	{ "Vlc",              "vlc",              NULL,       1 << 7,       1,           -1 },
	{ "Pavucontrol",      "pavucontrol",      NULL,       1 << 7,       1,           -1 },
	{ "Thunderbird",      NULL,               NULL,       1 << 8,       0,            1 },
	{ "VirtualBox",       NULL,               NULL,       1 << 6,       1,            1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
 	{ "###",      horizgrid },
  	{ "[@]",      spiral },
  	{ "[\\]",     dwindle },
    { "TTT",      htile }, 
    { "[C]",      centeredmaster }, 
};

/* key definitions */
#define WINKEY Mod1Mask
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|WINKEY,                KEY,      nview,          {.ui = 1 << TAG} }, \
	{ MODKEY|WINKEY|ControlMask,    KEY,      ntoggleview,    {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key                  function        argument */
	{ MODKEY,                       XK_p,                spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,           spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,                togglebar,      {0} },
	{ MODKEY,                       XK_j,                focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_bracketleft,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_bracketright,     incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                setmfact,       {.f = +0.05} },
 	{ MODKEY|ShiftMask,             XK_h,                setcfact,       {.f = +0.25} },
 	{ MODKEY|ShiftMask,             XK_l,                setcfact,       {.f = -0.25} },
 	{ MODKEY|ShiftMask,             XK_o,                setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_Return,           zoom,           {0} },
	{ MODKEY,                       XK_Tab,              view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,                killclient,     {0} },
	{ MODKEY,                       XK_t,                setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,                setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,                setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_e,                setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_s,                setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_a,                setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_w,                setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                       XK_q,                setlayout,      {.v = &layouts[7]} },
	{ MODKEY,                       XK_space,            setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,            togglefloating, {0} },
	{ MODKEY,                       XK_0,                view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,            focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,           focusmon,       {.i = +1 } },
 	{ MODKEY,                       XK_n,                nametag,        {0} },
	{ MODKEY|ShiftMask,             XK_comma,            tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,           tagmon,         {.i = +1 } },
    { MODKEY,                       XK_bracketright,     view_adjacent,  {.i = +1 } },
    { MODKEY,                       XK_bracketleft,      view_adjacent,  {.i = -1 } },
	TAGKEYS(                        XK_1,                0)
	TAGKEYS(                        XK_2,                1)
	TAGKEYS(                        XK_3,                2)
	TAGKEYS(                        XK_4,                3)
	TAGKEYS(                        XK_5,                4)
	TAGKEYS(                        XK_6,                5)
	TAGKEYS(                        XK_7,                6)
	TAGKEYS(                        XK_8,                7)
	TAGKEYS(                        XK_9,                8)
	{ MODKEY|ShiftMask,             XK_q,                quit,           {0} },

    { MODKEY,                       XK_Down,             moveresize,     {.v = "0x 25y 0w 0h"}},
    { MODKEY,                       XK_Up,               moveresize,     {.v = "0x -25y 0w 0h"}},
    { MODKEY,                       XK_Right,            moveresize,     {.v = "25x 0y 0w 0h"}},
    { MODKEY,                       XK_Left,             moveresize,     {.v = "-25x 0y 0w 0h"}},
    { MODKEY|ShiftMask,             XK_Down,             moveresize,     {.v = "0x 0y 0w 25h"}},
	{ MODKEY|ShiftMask,             XK_Up,               moveresize,     {.v = "0x 0y 0w -25h"}},
	{ MODKEY|ShiftMask,             XK_Right,            moveresize,     {.v = "0x 0y 25w 0h"}},
	{ MODKEY|ShiftMask,             XK_Left, 			 moveresize,     {.v = "0x 0y -25w 0h"}},


};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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
    { ClkTagBar,            0,              Button4,        view_adjacent,  { .i = +1 } },
    { ClkTagBar,            0,              Button5,        view_adjacent,  { .i = -1 } },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY|WINKEY,  Button1,        nview,          {0} },
	{ ClkTagBar,            MODKEY|WINKEY,  Button3,        ntoggleview,    {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

