/*
 *   Unreal Internet Relay Chat Daemon, src/misc.c
 *   Copyright (C) 1990 Jarkko Oikarinen and
 *                      University of Oulu, Computing Center
 *   Copyright (C) 1999-present UnrealIRCd team
 *
 *   See file AUTHORS in IRC package for additional names of
 *   the programmers.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 1, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/** @file
 * @brief Miscellaneous functions that don't fit in other files.
 * Generally these are either simple helper functions or larger
 * functions that don't fit in either user.c, channel.c.
 */

#include "unrealircd.h"

static const char *months[] = {
	"January", "February", "March", "April",
	"May", "June", "July", "August",
	"September", "October", "November", "December"
};

static const char *weekdays[] = {
	"Sunday", "Monday", "Tuesday", "Wednesday",
	"Thursday", "Friday", "Saturday"
};

static const char *short_months[12] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
};

static const char *short_weekdays[7] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
};

typedef struct {
	int value;			/** Unique integer value of item */
	char character;		/** Unique character assigned to item */
	char *name;			/** Name of item */
	char config_only;
} BanActTable;

static BanActTable banacttable[] = {
	{ BAN_ACT_KILL,		'K',	"kill",			0 },
	{ BAN_ACT_SOFT_KILL,	'i',	"soft-kill",		0 },
	{ BAN_ACT_TEMPSHUN,	'S',	"tempshun",		0 },
	{ BAN_ACT_SOFT_TEMPSHUN,'T',	"soft-tempshun",	0 },
	{ BAN_ACT_SHUN,		's',	"shun",			0 },
	{ BAN_ACT_SOFT_SHUN,	'H',	"soft-shun",		0 },
	{ BAN_ACT_KLINE,	'k',	"kline",		0 },
	{ BAN_ACT_SOFT_KLINE,	'I',	"soft-kline",		0 },
	{ BAN_ACT_ZLINE,	'z',	"zline",		0 },
	{ BAN_ACT_GLINE,	'g',	"gline",		0 },
	{ BAN_ACT_SOFT_GLINE,	'G',	"soft-gline",		0 },
	{ BAN_ACT_GZLINE,	'Z',	"gzline",		0 },
	{ BAN_ACT_BLOCK,	'b',	"block",		0 },
	{ BAN_ACT_SOFT_BLOCK,	'B',	"soft-block",		0 },
	{ BAN_ACT_DCCBLOCK,	'd',	"dccblock",		0 },
	{ BAN_ACT_SOFT_DCCBLOCK,'D',	"soft-dccblock",	0 },
	{ BAN_ACT_VIRUSCHAN,	'v',	"viruschan",		0 },
	{ BAN_ACT_SOFT_VIRUSCHAN,'V',	"soft-viruschan",	0 },
	{ BAN_ACT_WARN,		'w',	"warn",			0 },
	{ BAN_ACT_SOFT_WARN,	'W',	"soft-warn",		0 },
	{ BAN_ACT_SET,		'1',	"set",			1 },
	{ BAN_ACT_REPORT,	'r',	"report",		1 },
	{ BAN_ACT_STOP,		'0',	"stop",			1 },
	{ 0, 0, 0, 0 }
};

typedef struct {
	int value;			/** Unique integer value of item */
	char character;		/** Unique character assigned to item */
	char *name;			/** Name of item */
	char *irccommand;	/** Raw IRC command of item (not unique!) */
} SpamfilterTargetTable;

SpamfilterTargetTable spamfiltertargettable[] = {
	{ SPAMF_CHANMSG,	'c',	"channel",		"PRIVMSG" },
	{ SPAMF_USERMSG,	'p',	"private",		"PRIVMSG" },
	{ SPAMF_USERNOTICE,	'n',	"private-notice",	"NOTICE" },
	{ SPAMF_CHANNOTICE,	'N',	"channel-notice",	"NOTICE" },
	{ SPAMF_PART,		'P',	"part",			"PART" },
	{ SPAMF_QUIT,		'q',	"quit",			"QUIT" },
	{ SPAMF_DCC,		'd',	"dcc",			"PRIVMSG" },
	{ SPAMF_USER,		'u',	"user",			"NICK" },
	{ SPAMF_AWAY,		'a',	"away",			"AWAY" },
	{ SPAMF_TOPIC,		't',	"topic",		"TOPIC" },
	{ SPAMF_MTAG,		'T',	"message-tag",		"message-tag" },
	{ SPAMF_RAW,		'R',	"raw",			"cmd" },
	{ 0, 0, 0, 0 }
};

/** IRC Statistics (quite useless?) */
struct IRCStatistics ircstats;

/** Returns the date in rather long string */
const char *long_date(time_t clock)
{
	static char buf[80], plus;
	struct tm *lt, *gm;
	struct tm gmbuf;
	int  minswest;

	if (!clock)
		time(&clock);
	gm = gmtime(&clock);
	memcpy(&gmbuf, gm, sizeof(gmbuf));
	gm = &gmbuf;
	lt = localtime(&clock);
#ifndef _WIN32
	if (lt->tm_yday == gm->tm_yday)
		minswest = (gm->tm_hour - lt->tm_hour) * 60 +
		    (gm->tm_min - lt->tm_min);
	else if (lt->tm_yday > gm->tm_yday)
		minswest = (gm->tm_hour - (lt->tm_hour + 24)) * 60;
	else
		minswest = ((gm->tm_hour + 24) - lt->tm_hour) * 60;
#else
	minswest = (_timezone / 60);
#endif
	plus = (minswest > 0) ? '-' : '+';
	if (minswest < 0)
		minswest = -minswest;
	ircsnprintf(buf, sizeof(buf), "%s %s %d %d -- %02d:%02d %c%02d:%02d",
	    weekdays[lt->tm_wday], months[lt->tm_mon], lt->tm_mday,
	    1900 + lt->tm_year,
	    lt->tm_hour, lt->tm_min, plus, minswest / 60, minswest % 60);

	return buf;
}

/** Convert timestamp to a short date, a la: Wed Jun 30 21:49:08 1993
 * @returns A short date string, or NULL if the timestamp is invalid
 * (out of range)
 * @param ts   The timestamp
 * @param buf  The buffer to store the string (minimum size: 128 bytes),
 *             or NULL to use temporary static storage.
 */
const char *short_date(time_t ts, char *buf)
{
	struct tm *t = gmtime(&ts);
	static char retbuf[128];

	if (!buf)
		buf = retbuf;

	*buf = '\0';
	if (!t)
		return NULL;

	if (!strftime(buf, 128, "%a %b %d %H:%M:%S %Y", t))
		return NULL;

	return buf;
}

/** Return a string with the "pretty date" - yeah, another variant */
const char *pretty_date(time_t t)
{
	static char buf[128];
	struct tm *tm;

	if (!t)
		time(&t);
	tm = gmtime(&t);
	snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d GMT",
	         1900 + tm->tm_year,
	         tm->tm_mon + 1,
	         tm->tm_mday,
	         tm->tm_hour,
	         tm->tm_min,
	         tm->tm_sec);

	return buf;
}

/** Helper function for make_user_host() and friends.
 * Fixes a string so that the first white space found becomes an end of
 * string marker (`\-`).  returns the 'fixed' string or "*" if the string
 * was NULL length or a NULL pointer.
 */
const char *check_string(const char *s)
{
	static char buf[512];
	static char star[2] = "*";
	const char *str = s;

	if (BadPtr(s))
		return star;

	for (; *s; s++)
	{
		if (isspace(*s))
		{
			/* Because this is an unlikely scenario, we have
			 * delayed the copy until here:
			 */
			strlncpy(buf, s, sizeof(buf), s - str);
			str = buf;
			break;
		}
	}

	return (BadPtr(str)) ? star : str;
}

/** Create a user@host based on the provided name and host */
char *make_user_host(const char *name, const char *host)
{
	static char namebuf[USERLEN + HOSTLEN + 6];

	strlncpy(namebuf, check_string(name), sizeof(namebuf), USERLEN+1);
	strlcat(namebuf, "@", sizeof(namebuf));
	strlncat(namebuf, check_string(host), sizeof(namebuf), HOSTLEN+1);
	return namebuf;
}

/** Create a nick!user@host string based on the provided variables.
 * If any of the variables are NULL, it becomes * (asterisk)
 * This is the reentrant safe version.
 */
char *make_nick_user_host_r(char *namebuf, size_t namebuflen, const char *nick, const char *name, const char *host)
{
	strlncpy(namebuf, check_string(nick), namebuflen, NICKLEN+1);
	strlcat(namebuf, "!", namebuflen);
	strlncat(namebuf, check_string(name), namebuflen, USERLEN+1);
	strlcat(namebuf, "@", namebuflen);
	strlncat(namebuf, check_string(host), namebuflen, HOSTLEN+1);
	return namebuf;
}

/** Create a nick!user@host string based on the provided variables.
 * If any of the variables are NULL, it becomes * (asterisk)
 * This version uses static storage.
 */
char *make_nick_user_host(const char *nick, const char *name, const char *host)
{
	static char namebuf[NICKLEN + USERLEN + HOSTLEN + 24];

	return make_nick_user_host_r(namebuf, sizeof(namebuf), nick, name, host);
}


/** Similar to ctime() but without a potential newline and
 * also takes a time_t value rather than a pointer.
 */
const char *myctime(time_t value)
{
	static char buf[28];
	char *p;

	strlcpy(buf, ctime(&value), sizeof buf);
	if ((p = strchr(buf, '\n')) != NULL)
		*p = '\0';

	return buf;
}

/*
** get_client_name
**      Return the name of the client for various tracking and
**      admin purposes. The main purpose of this function is to
**      return the "socket host" name of the client, if that
**	differs from the advertised name (other than case).
**	But, this can be used to any client structure.
**
**	Returns:
**	  "name[user@ip#.port]" if 'showip' is true;
**	  "name[sockethost]", if name and sockhost are different and
**	  showip is false; else
**	  "name".
**
** NOTE 1:
**	Watch out the allocation of "nbuf", if either client->name
**	or client->local->sockhost gets changed into pointers instead of
**	directly allocated within the structure...
**
** NOTE 2:
**	Function return either a pointer to the structure (client) or
**	to internal buffer (nbuf). *NEVER* use the returned pointer
**	to modify what it points!!!
*/
const char *get_client_name(Client *client, int showip)
{
	static char nbuf[HOSTLEN * 2 + USERLEN + 5];

	if (MyConnect(client))
	{
		if (showip)
			ircsnprintf(nbuf, sizeof(nbuf), "%s[%s@%s.%u]",
			    client->name,
			    IsIdentSuccess(client) ? client->ident : "",
			    client->ip ? client->ip : "???",
			    (unsigned int)client->local->port);
		else
		{
			if (mycmp(client->name, client->local->sockhost))
				ircsnprintf(nbuf, sizeof(nbuf), "%s[%s]",
				    client->name, client->local->sockhost);
			else
				return client->name;
		}
		return nbuf;
	}
	return client->name;
}

const char *get_client_host(Client *client)
{
	static char nbuf[HOSTLEN * 2 + USERLEN + 5];

	if (!MyConnect(client))
		return client->name;
	if (!client->local->hostp)
		return get_client_name(client, FALSE);
	ircsnprintf(nbuf, sizeof(nbuf), "%s[%-.*s@%-.*s]",
	    client->name, USERLEN,
  	    IsIdentSuccess(client) ? client->ident : "",
	    HOSTLEN, client->local->hostp->h_name);
	return nbuf;
}

/*
 * Set sockhost to 'host'. Skip the user@ part of 'host' if necessary.
 */
void set_sockhost(Client *client, const char *host)
{
	const char *s;
	if ((s = strchr(host, '@')))
		s++;
	else
		s = host;
	strlcpy(client->local->sockhost, s, sizeof(client->local->sockhost));
}

/** Returns 1 if 'from' is on the allow list of 'to' */
int on_dccallow_list(Client *to, Client *from)
{
	Link *lp;

	for(lp = to->user->dccallow; lp; lp = lp->next)
		if (lp->flags == DCC_LINK_ME && lp->value.client == from)
			return 1;
	return 0;
}

/** Initialize the (quite useless) IRC statistics */
void initstats(void)
{
	memset(&ircstats, 0, sizeof(ircstats));
}

/** Verify operator count, to catch bugs introduced by flawed services */
void verify_opercount(Client *orig, const char *tag)
{
	int counted = 0;
	Client *client;
	char text[2048];

	list_for_each_entry(client, &client_list, client_node)
	{
		if (IsOper(client) && !IsHideOper(client))
			counted++;
	}
	if (counted == irccounts.operators)
		return;
	unreal_log(ULOG_WARNING, "main", "BUG_LUSERS_OPERS", orig,
	           "[BUG] Operator count bug at $where! Value in /LUSERS is $opers, "
	           "we counted $counted_opers, "
	           "triggered by $client.details on $client.user.servername",
	           log_data_integer("opers", irccounts.operators),
	           log_data_integer("counted_opers", counted),
	           log_data_string("where", tag));
	irccounts.operators = counted;
}

/** Check if the specified hostname does not contain forbidden characters.
 * @param host		The host name to check
 * @param strict	If set to 1 then we also check if the hostname
 *                      resembles an IP address (eg contains ':') and
 *                      some other stuff that we don't consider valid
 *                      in actual DNS names (eg '/').
 * @returns 1 if valid, 0 if not.
 */
int valid_host(const char *host, int strict)
{
	const char *p;

	if (!*host)
		return 0; /* must at least contain something */

	if (strlen(host) > HOSTLEN)
		return 0; /* too long hosts are invalid too */

	if (strict)
	{
		for (p=host; *p; p++)
			if (!isalnum(*p) && !strchr("_-.", *p))
				return 0;
	} else {
		for (p=host; *p; p++)
			if (!isalnum(*p) && !strchr("_-.:/", *p))
				return 0;
	}

	return 1;
}

/** Check if the specified ident / user name does not contain forbidden characters.
 * @param username	The username / ident to check
 * @returns 1 if valid, 0 if not.
 */
int valid_username(const char *username)
{
	const char *s;

	if (strlen(username) > USERLEN)
		return 0; /* Too long */

	for (s = username; *s; s++)
	{
		if ((*s == '~') && (s == username))
			continue;
		if (!isallowed(*s))
			return 0;
	}

	return 1;
}

/** Check validity of a vhost which can be both in 'host' or 'user@host' format.
 * This will call valid_username() and valid_host(xxx, 0) accordingly.
 * @param userhost the "host" or "user@host"
 * @returns 1 if valid, 0 if not.
 */
int valid_vhost(const char *userhost)
{
	char uhost[512], *p;
	const char *host = userhost;

        strlcpy(uhost, userhost, sizeof(uhost));

	if ((p = strchr(uhost, '@')))
	{
		*p++ = '\0';
		if (!valid_username(uhost))
			return 0;
		host = p;
	}

	if (!valid_host(host, 0))
		return 0;

	return 1;
}

/** Weird variant of valid_vhost() that allows $varnames through.
 * So should only be used for checking things like vhost::vhost,
 * oper::vhost and such in the CONFIG TEST routines.
 * At runtime you should use valid_vhost() to check the final host
 * again after variable expansion (by unreal_expand_string usually).
 */
int potentially_valid_vhost(const char *userhost)
{
	char uhost[512], *p;
	strlcpy(uhost, userhost, sizeof(uhost));
	for (p = uhost; *p; p++)
		if (*p == '$')
			*p = '_';
	return valid_vhost(uhost);
}

/*|| BAN ACTION ROUTINES FOLLOW ||*/

int parse_ban_action_set(const char *str, char **var, VarActionValue *op, int *value, char **error)
{
	/* SCORE=1
	 * SCORE++
	 * SCORE+=5
	 * SCORE--
	 * SCORE-=5
	 */
	static char buf[512];
	char *p;

	*error = NULL;
	*var = NULL;
	*op = 0;
	*value = 0;

	strlcpy(buf, str, sizeof(buf));
	for (p = buf; *p; p++)
		if (!isupper(*p) && !isdigit(*p) && !strchr("_", *p))
			break;
	if (!*p)
	{
		*error = "Missing value to set";
		return 0;
	}

	*var = buf;
	if (!strncmp(p, "++", 2))
	{
		*op = VAR_ACT_INCREASE;
		*p = '\0';
		p+=2;
		*value = 1;
	} else
	if (!strncmp(p, "--", 2))
	{
		*op = VAR_ACT_DECREASE;
		*p = '\0';
		p+=2;
		*value = 1;
	} else
	if (!strncmp(p, "+=", 2))
	{
		*op = VAR_ACT_INCREASE;
		*p = '\0';
		p+=2;
	} else
	if (!strncmp(p, "-=", 2))
	{
		*op = VAR_ACT_DECREASE;
		*p = '\0';
		p+=2;
	} else
	if (!strncmp(p, "=", 1))
	{
		*op = VAR_ACT_SET;
		*p = '\0';
		p+=1;
	} else
	{
		*error = "Unknown set action, should be one of: ++, --, +=, -= or =";
		return 0;
	}
	if (*value != 0)
	{
		if (*p)
		{
			*error = "Set action has trailing data after a ++ or --. This is not allowed.";
			return 0;
		}
		return 1;
	}
	if (!*p)
	{
		*error = "Missing value to set or increase";
		return 0;
	}
	*value = atoi(p);
	return 1;
}

int test_ban_action_config_helper(ConfigEntry *ce, const char *name, const char *value)
{
	int errors = 0;
	BanActionValue action;

	action = banact_stringtoval(name);
	if (!action)
	{
		config_error("%s:%d: unknown action: %s",
			     ce->file->filename, ce->line_number, name);
		errors++;
	} else if (action == BAN_ACT_SET)
	{
		if (!value)
		{
			config_error("%s:%d: action set is missing a value",
				ce->file->filename, ce->line_number);
			errors++;
		} else
		{
			char *var;
			VarActionValue op;
			int varvalue;
			char *error;
			if (!parse_ban_action_set(value, &var, &op, &varvalue, &error))
			{
				config_error("%s:%d: action: %s",
					ce->file->filename, ce->line_number, error);
				errors++;
			}
		}
	} else if (action == BAN_ACT_REPORT)
	{
	}

	return errors;
}

/** Test an action item in the config.
 * @param ce	The config entry to parse
 * @returns	Number of errors, so 0 means OK.
 */
int test_ban_action_config(ConfigEntry *ce)
{
	ConfigEntry *cep;
	int errors = 0;

	if (ce->items && !ce->value)
	{
		/* action { xxx; } */
		for (cep = ce->items; cep; cep = cep->next)
			errors += test_ban_action_config_helper(cep, cep->name, cep->value);
	} else
	if (!ce->value)
	{
		config_error("%s:%d: action has no value", ce->file->filename, ce->line_number);
		errors++;
	} else
	{
		/* action xxx; */
		errors += test_ban_action_config_helper(ce, ce->value, NULL);
	}

	return errors;
}

BanAction *parse_ban_action_config_helper(const char *name, const char *value)
{
	int errors = 0;
	BanAction *action = safe_alloc(sizeof(BanAction));

	action->action = banact_stringtoval(name);
	if (!action->action)
		abort(); /* impossible, is config tested earlier */

	if (action->action == BAN_ACT_SET)
	{
		char *var;
		VarActionValue op;
		int varvalue;
		char *error;
		if (!parse_ban_action_set(value, &var, &op, &varvalue, &error))
			abort();
		safe_strdup(action->var, var);
		action->value = varvalue;
		action->var_action = op;
	} else
	if (action->action == BAN_ACT_REPORT)
	{
		safe_strdup(action->var, value); // can be NULL, means all
	}

	return action;
}

/** Parse an action item in the config.
 * @param ce		The config entry to parse
 * @param store_actions	Where to store the linked list in, any previously stored actions are freed.
 * @notes Be sure to initialize *store_actions before calling (eg to NULL).
 * The reason this function does not return a BanAction but uses a parameter is because
 * it is common to have a default config setting and then parsing of config items later,
 * which would have made it too easy to create memory leaks.
 */
void parse_ban_action_config(ConfigEntry *ce, BanAction **store_actions)
{
	ConfigEntry *cep;
	BanAction *action;

	if (*store_actions)
	{
		free_all_ban_actions(*store_actions);
		*store_actions = NULL;
	}

	if (ce->items && !ce->value)
	{
		/* action { xxx; } */
		for (cep = ce->items; cep; cep = cep->next)
		{
			action = parse_ban_action_config_helper(cep->name, cep->value);
			if (action)
				append_ListItem((ListStruct *)action, (ListStruct **)store_actions);
		}
	} else
	if (ce->value)
	{
		/* action xxx; */
		action = parse_ban_action_config_helper(ce->value, NULL);
		if (action)
			append_ListItem((ListStruct *)action, (ListStruct **)store_actions);
	}
}

/** Return 1 if this is "config only" ban action, like BAN_ACT_SET is */
int banact_config_only(BanActionValue action)
{
	BanActTable *b;

	for (b = &banacttable[0]; b->value; b++)
		if (b->value == action)
			return b->config_only;
	return 0;
}

/** Converts a banaction string (eg: "kill") to an integer value (eg: BAN_ACT_KILL) */
BanActionValue banact_stringtoval(const char *s)
{
	BanActTable *b;

	for (b = &banacttable[0]; b->value; b++)
		if (!strcasecmp(s, b->name))
			return b->value;
	return 0;
}

/** Converts a banaction character (eg: 'K') to an integer value (eg: BAN_ACT_KILL) */
BanActionValue banact_chartoval(char c)
{
	BanActTable *b;

	for (b = &banacttable[0]; b->value; b++)
		if (b->character == c)
			return b->value;
	return 0;
}

/** Converts a banaction value (eg: BAN_ACT_KILL) to a character value (eg: 'k') */
char banact_valtochar(BanActionValue val)
{
	BanActTable *b;

	for (b = &banacttable[0]; b->value; b++)
		if (b->value == val)
			return b->character;
	return '\0';
}

/** Converts a banaction value (eg: BAN_ACT_KLINE) to a string (eg: "kline") */
const char *banact_valtostring(BanActionValue val)
{
	BanActTable *b;

	for (b = &banacttable[0]; b->value; b++)
		if (b->value == val)
			return b->name;
	return "UNKNOWN";
}

BanAction *banact_value_to_struct(BanActionValue val)
{
	BanAction *action = safe_alloc(sizeof(BanAction));
	action->action = val;
	return action;
}

/** Check if action is only of type 'what', eg if they are all BAN_ACT_WARN */
int only_actions_of_type(BanAction *actions, BanActionValue what)
{
	for (; actions; actions = actions->next)
		if (actions->action != what)
			return 0;
	return 1;
}

/** Check if action 'what' is in any of 'actions' */
int has_actions_of_type(BanAction *actions, BanActionValue what)
{
	for (; actions; actions = actions->next)
		if (actions->action == what)
			return 1;
	return 0;
}

/** Check if only soft ban actions */
int only_soft_actions(BanAction *actions)
{
	for (; actions; actions = actions->next)
		if (!IsSoftBanAction(actions->action))
			return 0;
	return 1;
}

/** Turn a linked list BanAction * into a string of actions.
 * Returns something like "gline" or "set, gline"
 */
const char *ban_actions_to_string(BanAction *actions)
{
	static char buf[512];
	const char *s;
	BanAction *action;
	*buf = '\0';

	for (action = actions; action; action = action->next)
	{
		s = banact_valtostring(action->action);
		if (!s)
			s = "???";
		strlcat(buf, s, sizeof(buf));
		strlcat(buf, ",", sizeof(buf));
	}

	/* Cut off trailing "," */
	if (*buf)
		buf[strlen(buf)-1] = '\0';

	return buf;
}

/* Find the highest value in a BanAction linked list (the strongest action, eg gline>block) */
int highest_ban_action(BanAction *action)
{
	int highest = 0;

	for (; action; action = action->next)
		if (action->action > highest)
			highest = action->action;

	return highest;
}

/**  Lower any 'actions' to a maximum of 'limit_action'.
 * See the BanActionValue table for what is considered higher/lower.
 */
void lower_ban_action_to_maximum(BanAction *actions, BanActionValue limit_action)
{
	for (; actions; actions = actions->next)
		if (actions->action > limit_action)
			actions->action = limit_action;
}

void free_single_ban_action(BanAction *action)
{
	safe_free(action->var);
	safe_free(action);
}

void free_all_ban_actions(BanAction *actions)
{
	BanAction *next;
	for (; actions; actions = next)
	{
		next = actions->next;
		free_single_ban_action(actions);
	}
}

/** Duplicate an entire BanAction linked list (deep copy) */
BanAction *duplicate_ban_actions(BanAction *actions)
{
	BanAction *newlist = NULL;
	BanAction *action, *n;

	for (action = actions; action; action = action->next)
	{
		n = safe_alloc(sizeof(BanAction));
		// This matches the struct.h order:
		n->action = action->action;
		safe_strdup(n->var, action->var);
		n->value = action->value;
		n->var_action = action->var_action;
		/* And add to the list, maintain action ordering */
		AppendListItem(n, newlist);
	}
	return newlist;
}

/*|| BAN TARGET ROUTINES FOLLOW ||*/

/** Extract target flags from string 's'. */
int spamfilter_gettargets(const char *s, Client *client)
{
SpamfilterTargetTable *e;
int flags = 0;

	for (; *s; s++)
	{
		for (e = &spamfiltertargettable[0]; e->value; e++)
			if (e->character == *s)
			{
				flags |= e->value;
				break;
			}
		if (!e->value && client)
		{
			sendnotice(client, "Unknown target type '%c'", *s);
			return 0;
		}
	}
	return flags;
}

/** Convert a string with a targetname to an integer value */
int spamfilter_getconftargets(const char *s)
{
SpamfilterTargetTable *e;

	for (e = &spamfiltertargettable[0]; e->value; e++)
		if (!strcmp(s, e->name))
			return e->value;
	return 0;
}

/** Create a string with (multiple) targets from an integer mask */
char *spamfilter_target_inttostring(int v)
{
	static char buf[128];
	SpamfilterTargetTable *e;
	char *p = buf;

	for (e = &spamfiltertargettable[0]; e->value; e++)
		if (v & e->value)
			*p++ = e->character;
	*p = '\0';
	return *buf ? buf : "-";
}

/** Replace underscores back to the space character.
 * This is used for the spamfilter reason.
 */
char *unreal_decodespace(const char *s)
{
	const char *i;
	static char buf[512], *o;

	for (i = s, o = buf; (*i) && (o < buf+510); i++)
		if (*i == '_')
		{
			if (i[1] != '_')
				*o++ = ' ';
			else {
				*o++ = '_';
				i++;
			}
		}
		else
			*o++ = *i;
	*o = '\0';
	return buf;
}

/** Replace spaces to underscore characters.
 * This is used for the spamfilter reason.
 */
char *unreal_encodespace(const char *s)
{
	const char *i;
	static char buf[512], *o;

	if (!s)
		return NULL; /* NULL in = NULL out */

	for (i = s, o = buf; (*i) && (o < buf+509); i++)
	{
		if (*i == ' ')
			*o++ = '_';
		else if (*i == '_')
		{
			*o++ = '_';
			*o++ = '_';
		}
		else
			*o++ = *i;
	}
	*o = '\0';
	return buf;
}

/** This is basically only used internally by match_spamfilter()... */
const char *cmdname_by_spamftarget(int target)
{
	SpamfilterTargetTable *e;

	for (e = &spamfiltertargettable[0]; e->value; e++)
		if (e->value == target)
			return e->irccommand;
	return "???";
}

/** Convert the value for set::spamfilter::show-message-content-on-hit
 * and spamfilter::show-message-content-on-hit.
 */
SpamfilterShowMessageContentOnHit spamfilter_show_message_content_on_hit_strtoval(const char *s)
{
	if (!strcmp(s, "always"))
		return SPAMFILTER_SHOW_MESSAGE_CONTENT_ON_HIT_ALWAYS;
	if (!strcmp(s, "channel-only"))
		return SPAMFILTER_SHOW_MESSAGE_CONTENT_ON_HIT_CHANNEL_ONLY;
	if (!strcmp(s, "never"))
		return SPAMFILTER_SHOW_MESSAGE_CONTENT_ON_HIT_NEVER;
	return 0;
}

/** Add name entries from config */
void unreal_add_names(NameList **n, ConfigEntry *ce)
{
	if (ce->items)
	{
		ConfigEntry *cep;
		for (cep = ce->items; cep; cep = cep->next)
			_add_name_list(n, cep->value ? cep->value : cep->name);
	} else
	if (ce->value)
	{
		_add_name_list(n, ce->value);
	}
}

/** Add name/value entries from config */
void unreal_add_name_values(NameValuePrioList **n, const char *name, ConfigEntry *ce)
{
	if (ce->items)
	{
		ConfigEntry *cep;
		for (cep = ce->items; cep; cep = cep->next)
			add_nvplist(n, 0, name, cep->value ? cep->value : cep->name);
	} else
	if (ce->value)
	{
		add_nvplist(n, 0, name, ce->value);
	}
}

/** Prints the name:value pair of a NameValuePrioList */
const char *namevalue(NameValuePrioList *n)
{
	static char buf[512];

	if (!n->name)
		return "";

	if (!n->value)
		return n->name;

	snprintf(buf, sizeof(buf), "%s:%s", n->name, n->value);
	return buf;
}

/** Version of namevalue() but replaces spaces with underscores.
 * Used in for example numeric sending routines where a field
 * may not contain any spaces.
 */
const char *namevalue_nospaces(NameValuePrioList *n)
{
	static char buf[512];
	char *p;

	if (!n->name)
		return "";

	if (!n->value)
		strlcpy(buf, n->name, sizeof(buf));

	snprintf(buf, sizeof(buf), "%s:%s", n->name, n->value);

	/* Replace spaces with underscores */
	for (p=buf; *p; p++)
		if (*p == ' ')
			*p = '_';

	return buf;
}

/** Our own strcasestr implementation because strcasestr is
 * often not available or is not working correctly.
 */
char *our_strcasestr(const char *haystack, const char *needle)
{
	int i;
	int nlength = strlen(needle);
	int hlength = strlen(haystack);

	if (nlength > hlength)
		return NULL;

	if (hlength <= 0)
		return NULL;

	if (nlength <= 0)
		return (char *)haystack;

	for (i = 0; i <= (hlength - nlength); i++)
	{
		if (strncasecmp (haystack + i, needle, nlength) == 0)
			return (char *)(haystack + i);
	}

	return NULL; /* not found */
}

/** Add a title to the users' WHOIS ("special whois"). Broadcast change to servers.
 * @param client	The client
 * @param tag		A tag used internally and for server-to-server traffic,
 *			not visible to end-users.
 * @param priority	Priority - for ordering multiple swhois entries
 *                      (lower number = further up in the swhoises list in WHOIS)
 * @param swhois	The actual special whois title (string) you want to add to the user
 * @param from		Who added this entry
 * @param skip		Which server(-side) to skip broadcasting this entry to.
 */
int swhois_add(Client *client, const char *tag, int priority, const char *swhois, Client *from, Client *skip)
{
	SWhois *s;

	/* Make sure the line isn't added yet. If so, then bail out silently. */
	for (s = client->user->swhois; s; s = s->next)
		if (!strcmp(s->line, swhois))
			return -1; /* exists */

	s = safe_alloc(sizeof(SWhois));
	safe_strdup(s->line, swhois);
	safe_strdup(s->setby, tag);
	s->priority = priority;
	AddListItemPrio(s, client->user->swhois, s->priority);

	sendto_server(skip, 0, PROTO_EXTSWHOIS, NULL, ":%s SWHOIS %s :%s",
		from->id, client->id, swhois);

	sendto_server(skip, PROTO_EXTSWHOIS, 0, NULL, ":%s SWHOIS %s + %s %d :%s",
		from->id, client->id, tag, priority, swhois);

	return 0;
}

/** Delete swhois title(s).
 * Delete swhois by tag and swhois. Then broadcast this change to all other servers.
 * @param client	The client
 * @param tag		A tag used internally and for server-to-server traffic,
 *			not visible to end-users.
 * @param swhois	The actual special whois title (string) you are removing
 * @param from		Who added this entry earlier on
 * @param skip		Which server(-side) to skip broadcasting this entry to.
 * @note If you use swhois "*" then it will remove all swhois titles for that tag
 */
int swhois_delete(Client *client, const char *tag, const char *swhois, Client *from, Client *skip)
{
	SWhois *s, *s_next;
	int ret = -1; /* default to 'not found' */

	for (s = client->user->swhois; s; s = s_next)
	{
		s_next = s->next;

		/* If ( same swhois or "*" ) AND same tag */
		if ( ((!strcmp(s->line, swhois) || !strcmp(swhois, "*")) &&
		    !strcmp(s->setby, tag)))
		{
			DelListItem(s, client->user->swhois);
			safe_free(s->line);
			safe_free(s->setby);
			safe_free(s);

			sendto_server(skip, 0, PROTO_EXTSWHOIS, NULL, ":%s SWHOIS %s :",
				from->id, client->id);

			sendto_server(skip, PROTO_EXTSWHOIS, 0, NULL, ":%s SWHOIS %s - %s %d :%s",
				from->id, client->id, tag, 0, swhois);

			ret = 0;
		}
	}

	return ret;
}

/** Is this user using a websocket? (LOCAL USERS ONLY) */
int IsWebsocket(Client *client)
{
	ModDataInfo *md = findmoddata_byname("websocket", MODDATATYPE_CLIENT);
	if (!md)
		return 0; /* websocket module not loaded */
	return (MyConnect(client) && moddata_client(client, md).ptr) ? 1 : 0;
}

const char *compressed_ip(const char *ip)
{
	char scratch[64];
	static char ret[64];

	if (ip && strchr(ip, ':') && (inet_pton(AF_INET6, ip, scratch) == 1))
		if (inet_ntop(AF_INET6, scratch, ret, sizeof(ret)))
			return ret;

	return ip;
}

/** Our stpcpy implementation - discouraged due to lack of bounds checking */
char *mystpcpy(char *dst, const char *src)
{
	for (; *src; src++)
		*dst++ = *src;
	*dst = '\0';
	return dst;
}

/** Helper function for send_channel_modes_sjoin3() and cmd_sjoin()
 * to build the SJSBY prefix which is <seton,setby> to
 * communicate when the ban was set and by whom.
 * @param buf   The buffer to write to
 * @param setby The setter of the "ban"
 * @param seton The time the "ban" was set
 * @retval The number of bytes written EXCLUDING the NUL byte,
 *         so similar to what strlen() would have returned.
 * @note Caller must ensure that the buffer 'buf' is of sufficient size.
 */
size_t add_sjsby(char *buf, const char *setby, time_t seton)
{
	char tbuf[32];
	char *p = buf;

	snprintf(tbuf, sizeof(tbuf), "%ld", (long)seton);

	*p++ = '<';
	p = mystpcpy(p, tbuf);
	*p++ = ',';
	p = mystpcpy(p, setby);
	*p++ = '>';
	*p = '\0';

	return p - buf;
}

/** Concatenate the entire parameter string.
 * The function will take care of spaces in the final parameter (if any).
 * @param buf   The buffer to output in.
 * @param len   Length of the buffer.
 * @param parc  Parameter count, ircd style.
 * @param parv  Parameters, ircd style, so we will start at parv[1].
 * @section ex1 Example
 * @code
 * char buf[512];
 * concat_params(buf, sizeof(buf), parc, parv);
 * sendto_server(client, 0, 0, recv_mtags, ":%s SOMECOMMAND %s", client->name, buf);
 * @endcode
 */
void concat_params(char *buf, int len, int parc, const char *parv[])
{
	int i;

	*buf = '\0';
	for (i = 1; i < parc; i++)
	{
		const char *param = parv[i];

		if (!param)
			break;

		if (*buf)
			strlcat(buf, " ", len);

		if (strchr(param, ' ') || (*param == ':'))
		{
			/* Last parameter, with : */
			strlcat(buf, ":", len);
			strlcat(buf, parv[i], len);
			break;
		}
		strlcat(buf, parv[i], len);
	}
}

/** Find a particular message-tag in the 'mtags' list */
MessageTag *find_mtag(MessageTag *mtags, const char *token)
{
	for (; mtags; mtags = mtags->next)
		if (!strcmp(mtags->name, token))
			return mtags;
	return NULL;
}

/** Free all message tags in the list 'm' */
void free_message_tags(MessageTag *m)
{
	MessageTag *m_next;

	for (; m; m = m_next)
	{
		m_next = m->next;
		safe_free(m->name);
		safe_free(m->value);
		safe_free(m);
	}
}

/** Duplicate a MessageTag structure.
 * @note  This duplicate a single MessageTag.
 *        It does not duplicate an entire linked list.
 */
MessageTag *duplicate_mtag(MessageTag *mtag)
{
	MessageTag *m = safe_alloc(sizeof(MessageTag));
	safe_strdup(m->name, mtag->name);
	safe_strdup(m->value, mtag->value);
	return m;
}

/** New message. Either really brand new, or inherited from other servers.
 * This function calls modules so they can add tags, such as:
 * msgid, time and account.
 */
void new_message(Client *sender, MessageTag *recv_mtags, MessageTag **mtag_list)
{
	Hook *h;
	for (h = Hooks[HOOKTYPE_NEW_MESSAGE]; h; h = h->next)
		(*(h->func.voidfunc))(sender, recv_mtags, mtag_list, NULL);
}

/** New message - SPECIAL edition. Either really brand new, or inherited
 * from other servers.
 * This function calls modules so they can add tags, such as:
 * msgid, time and account.
 * This special version deals in a special way with msgid in particular.
 * The pattern and vararg create a 'signature', this is normally
 * identical to the message that is sent to clients (end-users).
 * For example ":xyz JOIN #chan".
 */
void new_message_special(Client *sender, MessageTag *recv_mtags, MessageTag **mtag_list, FORMAT_STRING(const char *pattern), ...)
{
	Hook *h;
	va_list vl;
	char buf[512];

	va_start(vl, pattern);
	ircvsnprintf(buf, sizeof(buf), pattern, vl);
	va_end(vl);

	for (h = Hooks[HOOKTYPE_NEW_MESSAGE]; h; h = h->next)
		(*(h->func.voidfunc))(sender, recv_mtags, mtag_list, buf);
}

/** Default handler for parse_message_tags().
 * This is only used if the 'mtags' module is NOT loaded,
 * which would be quite unusual, but possible.
 */
void parse_message_tags_default_handler(Client *client, char **str, MessageTag **mtag_list)
{
	/* Just skip everything until the space character */
	for (; **str && **str != ' '; *str = *str + 1);
}

/** Default handler for mtags_to_string().
 * This is only used if the 'mtags' module is NOT loaded,
 * which would be quite unusual, but possible.
 */
const char *mtags_to_string_default_handler(MessageTag *m, Client *client)
{
	return NULL;
}

/** Default handler for add_silence().
 * This is only used if the 'silence' module is NOT loaded,
 * which would be unusual, but possible.
 */
int add_silence_default_handler(Client *client, const char *mask, int senderr)
{
	return 0;
}

/** Default handler for del_silence().
 * This is only used if the 'silence' module is NOT loaded,
 * which would be unusual, but possible.
 */
int del_silence_default_handler(Client *client, const char *mask)
{
	return 0;
}

/** Default handler for is_silenced().
 * This is only used if the 'silence' module is NOT loaded,
 * which would be unusual, but possible.
 */
int is_silenced_default_handler(Client *client, Client *acptr)
{
	return 0;
}

int spamreport_default_handler(Client *client, const char *ip, NameValuePrioList *details, const char *spamreport_block, Client *by)
{
	return -1;
}

/** Generate a BATCH id.
 * This can be used in a :serv BATCH +%s ... message
 */
void generate_batch_id(char *str)
{
	gen_random_alnum(str, BATCHLEN);
}

/** A default handler if labeled-response module is not loaded.
 * Normally a NOOP, but since caller will safe_free it
 * later we do actually allocate something.
 */
void *labeled_response_save_context_default_handler(void)
{
	return safe_alloc(8);
}

/** A default handler for if labeled-response module is not loaded */
void labeled_response_set_context_default_handler(void *ctx)
{
}

/** A default handler for if labeled-response module is not loaded */
void labeled_response_force_end_default_handler(void)
{
}

/** Ad default handler for if the slog module is not loaded */
void do_unreal_log_remote_deliver_default_handler(LogLevel loglevel, const char *subsystem, const char *event_id, MultiLine *msg, const char *json_serialized)
{
}

int make_oper_default_handler(Client *client, const char *operblock_name, const char *operclass,
                              ConfigItem_class *clientclass, long modes, const char *snomask,
                              const char *vhost, const char *autojoin_channels)
{
	return 0;
}

void webserver_send_response_default_handler(Client *client, int status, char *msg)
{
}

void webserver_close_client_default_handler(Client *client)
{
}

int webserver_handle_body_default_handler(Client *client, WebRequest *web, const char *readbuf, int length)
{
	return 0;
}

void rpc_response_default_handler(Client *client, json_t *request, json_t *result)
{
}

void rpc_error_default_handler(Client *client, json_t *request, JsonRpcError error_code, const char *error_message)
{
}

void rpc_error_fmt_default_handler(Client *client, json_t *request, JsonRpcError error_code, const char *fmt, ...)
{
}

void rpc_send_request_to_remote_default_handler(Client *source, Client *target, json_t *request)
{
}

void rpc_send_response_to_remote_default_handler(Client *source, Client *target, json_t *response)
{
}

int rrpc_supported_simple_default_handler(Client *target, char **problem_server)
{
	if (problem_server)
		*problem_server = me.name;
	return 0;
}

int rrpc_supported_default_handler(Client *target, const char *module, const char *minimum_version, char **problem_server)
{
	if (problem_server)
		*problem_server = me.name;
	return 0;
}

int websocket_handle_websocket_default_handler(Client *client, WebRequest *web, const char *readbuf2, int length2, int callback(Client *client, char *buf, int len))
{
	return -1;
}

int websocket_create_packet_default_handler(int opcode, char **buf, int *len)
{
	return -1;
}

int websocket_create_packet_ex_default_handler(int opcode, char **buf, int *len, char *sendbuf, size_t sendbufsize)
{
	return -1;
}

int websocket_create_packet_simple_default_handler(int opcode, const char **buf, int *len)
{
	return -1;
}

void mtag_add_issued_by_default_handler(MessageTag **mtags, Client *client, MessageTag *recv_mtags)
{
}

void cancel_ident_lookup_default_handler(Client *client)
{
}

void ban_act_set_reputation_default_handler(Client *client, BanAction *action)
{
}

const char *get_central_api_key_default_handler(void)
{
	return NULL;
}

int central_spamreport_default_handler(Client *target, Client *by, const char *url)
{
	return 0;
}

int central_spamreport_enabled_default_handler(void)
{
	return 0;
}

void sasl_succeeded_default_handler(Client *client)
{
}

void sasl_failed_default_handler(Client *client)
{
}

int decode_authenticate_plain_default_handler(const char *param, char **authorization_id, char **authentication_id, char **passwd)
{
	return 0;
}


/** my_timegm: mktime()-like function which will use GMT/UTC.
 * Strangely enough there is no standard function for this.
 * On some *NIX OS's timegm() may be available, sometimes only
 * with the help of certain #define's which we may or may
 * not do.
 * Windows provides _mkgmtime().
 * In the other cases the man pages and basically everyone
 * suggests to set TZ to empty prior to calling mktime and
 * restoring it after the call. Whut? How ridiculous is that?
 */
time_t my_timegm(struct tm *tm)
{
#if HAVE_TIMEGM
	return timegm(tm);
#elif defined(_WIN32)
	return _mkgmtime(tm);
#else
	time_t ret;
	char *tz = NULL;

	safe_strdup(tz, getenv("TZ"));
	setenv("TZ", "", 1);
	ret = mktime(tm);
	if (tz)
	{
		setenv("TZ", tz, 1);
		safe_free(tz);
	} else {
		unsetenv("TZ");
	}
	tzset();

	return ret;
#endif
}

/** Convert an ISO 8601 timestamp ('server-time') to UNIX time */
time_t server_time_to_unix_time(const char *tbuf)
{
	struct tm tm;
	int dontcare = 0;
	time_t ret;

	if (!tbuf)
		return 0;

	if (strlen(tbuf) < 20)
		return 0;

	memset(&tm, 0, sizeof(tm));
	ret = sscanf(tbuf, "%d-%d-%dT%d:%d:%d.%dZ",
		&tm.tm_year,
		&tm.tm_mon,
		&tm.tm_mday,
		&tm.tm_hour,
		&tm.tm_min,
		&tm.tm_sec,
		&dontcare);

	if (ret != 7)
		return 0;

	tm.tm_year -= 1900;
	tm.tm_mon -= 1;

	ret = my_timegm(&tm);
	return ret;
}

/** Convert an RFC 2616 timestamp (used in HTTP headers) to UNIX time */
time_t rfc2616_time_to_unix_time(const char *tbuf)
{
	struct tm tm;
	int dontcare = 0;
	time_t ret;
	char month[8];
	int i;

	if (!tbuf)
		return 0;

	if (strlen(tbuf) < 20)
		return 0;

	memset(&tm, 0, sizeof(tm));
	*month = '\0';
	ret = sscanf(tbuf, "%*[a-zA-Z,] %d %3s %d %d:%d:%d",
	             &tm.tm_mday, month, &tm.tm_year,
	             &tm.tm_hour, &tm.tm_min, &tm.tm_sec);

	if (ret < 6)
		return 0;

	for (i=0; i < 12; i++)
	{
		if (!strcmp(short_months[i], month))
		{
			tm.tm_mon = i;
			break;
		}
	}
	if (i == 12)
		return 0; /* Month not found */
	if (tm.tm_year < 1900)
		return 0;

	tm.tm_year -= 1900;
	ret = my_timegm(&tm);
	return ret; /* can still be 0 */
}

/** Returns an RFC 2616 timestamp (used in HTTP headers) */
const char *rfc2616_time(time_t clock)
{
	static char buf[80], plus;
	struct tm *lt, *gm;
	struct tm gmbuf;
	int  minswest;

	if (!clock)
		time(&clock);
	gm = gmtime(&clock);

	snprintf(buf, sizeof(buf),
	         "%s, %02d %.3s %4d %02d:%02d:%02d GMT",
	         short_weekdays[gm->tm_wday], gm->tm_mday, short_months[gm->tm_mon],
	         gm->tm_year + 1900, gm->tm_hour, gm->tm_min, gm->tm_sec);

	return buf;
}

/** Write a 64 bit integer to a file.
 * @param fd   File descriptor
 * @param t    The value to write
 * @returns 1 on success, 0 on failure.
 */
int write_int64(FILE *fd, uint64_t t)
{
	if (fwrite(&t, 1, sizeof(t), fd) < sizeof(t))
		return 0;
	return 1;
}

/** Write a 32 bit integer to a file.
 * @param fd   File descriptor
 * @param t    The value to write
 * @returns 1 on success, 0 on failure.
 */
int write_int32(FILE *fd, uint32_t t)
{
	if (fwrite(&t, 1, sizeof(t), fd) < sizeof(t))
		return 0;
	return 1;
}

/** Read a 64 bit integer from a file.
 * @param fd   File descriptor
 * @param t    The value to write
 * @returns 1 on success, 0 on failure.
 */
int read_int64(FILE *fd, uint64_t *t)
{
	if (fread(t, 1, sizeof(uint64_t), fd) < sizeof(uint64_t))
		return 0;
	return 1;
}

/** Read a 32 bit integer from a file.
 * @param fd   File descriptor
 * @param t    The value to write
 * @returns 1 on success, 0 on failure.
 */
int read_int32(FILE *fd, uint32_t *t)
{
	if (fread(t, 1, sizeof(uint32_t), fd) < sizeof(uint32_t))
		return 0;
	return 1;
}

/** Read binary data from a file.
 * @param fd   File descriptor
 * @param buf  Pointer to buffer
 * @param len  Size of buffer
 * @note  This function is not used much, in most cases
 *        you should use read_str(), read_int32() or
 *        read_int64() instead.
 * @returns 1 on success, 0 on failure.
 */
int read_data(FILE *fd, void *buf, size_t len)
{
	if (fread(buf, 1, len, fd) < len)
		return 0;
	return 1;
}

/** Write binary data to a file.
 * @param fd   File descriptor
 * @param buf  Pointer to buffer
 * @param len  Size of buffer
 * @note  This function is not used much, in most cases
 *        you should use write_str(), write_int32() or
 *        write_int64() instead.
 * @returns 1 on success, 0 on failure.
 */
int write_data(FILE *fd, const void *buf, size_t len)
{
	if (fwrite(buf, 1, len, fd) < len)
		return 0;
	return 1;
}

/** Write a string to a file.
 * @param fd   File descriptor
 * @param x    Pointer to string
 * @note  This function can write a string up to 65534
 *        characters, which should be plenty for usage
 *        in UnrealIRCd.
 *        Note that 'x' can safely be NULL.
 * @returns 1 on success, 0 on failure.
 */
int write_str(FILE *fd, const char *x)
{
	uint16_t len;

	len = x ? strlen(x) : 0xffff;
	if (!write_data(fd, &len, sizeof(len)))
		return 0;
	if ((len > 0) && (len < 0xffff))
	{
		if (!write_data(fd, x, len))
			return 0;
	}
	return 1;
}

/** Read a string from a file.
 * @param fd   File descriptor
 * @param x    Pointer to string pointer
 * @note  This function will allocate memory for the data
 *        and set the string pointer to this value.
 *        If a NULL pointer was written via write_str()
 *        then read_str() may also return a NULL pointer.
 * @returns 1 on success, 0 on failure.
 */
int read_str(FILE *fd, char **x)
{
	uint16_t len;
	size_t size;

	*x = NULL;

	if (!read_data(fd, &len, sizeof(len)))
		return 0;

	if (len == 0xffff)
	{
		/* Magic value meaning NULL */
		*x = NULL;
		return 1;
	}

	if (len == 0)
	{
		/* 0 means empty string */
		safe_strdup(*x, "");
		return 1;
	}

	if (len > 10000)
		return 0;

	size = len;
	*x = safe_alloc(size + 1);
	if (!read_data(fd, *x, size))
	{
		safe_free(*x);
		return 0;
	}
	(*x)[len] = 0;
	return 1;
}

/** Convert binary 'data' of size 'len' to a hexadecimal string 'str'.
 * The caller is responsible to ensure that 'str' is sufficiently large.
 */
void binarytohex(void *data, size_t len, char *str)
{
	const char hexchars[16] = "0123456789abcdef";
	char *datastr = (char *)data;
	int i, n = 0;

	for (i=0; i<len; i++)
	{
		str[n++] = hexchars[(datastr[i] >> 4) & 0xF];
		str[n++] = hexchars[datastr[i] & 0xF];
	}
	str[n] = '\0';
}

/** Generates an MD5 checksum - binary version.
 * @param mdout[out] Buffer to store result in, the result will be 16 bytes in binary
 *                   (not ascii printable!).
 * @param src[in]    The input data used to generate the checksum.
 * @param n[in]      Length of data.
 * @deprecated       The MD5 algorithm is deprecated and insecure,
 *                   so only use this if absolutely needed.
 */
void DoMD5(char *mdout, const char *src, unsigned long n)
{
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	unsigned int md_len;
	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
	if (EVP_DigestInit_ex(mdctx, md5_function, NULL) != 1)
		abort();
	EVP_DigestUpdate(mdctx, src, n);
	EVP_DigestFinal_ex(mdctx, mdout, &md_len);
	EVP_MD_CTX_free(mdctx);
#else
	MD5_CTX hash;

	MD5_Init(&hash);
	MD5_Update(&hash, src, n);
	MD5_Final(mdout, &hash);
#endif
}

/** Generates an MD5 checksum - ASCII printable string (0011223344..etc..).
 * @param dst[out]  Buffer to store result in, this will be the result will be
 *                  32 characters + nul terminator, so needs to be at least 33 characters.
 * @param src[in]   The input data used to generate the checksum.
 * @param n[in]     Length of data.
 * @deprecated      The MD5 algorithm is deprecated and insecure,
 *                  so only use this if absolutely needed.
 */
char *md5hash(char *dst, const char *src, unsigned long n)
{
	char tmp[16];

	DoMD5(tmp, src, n);
	binarytohex(tmp, sizeof(tmp), dst);
	return dst;
}

/** Generates a SHA256 checksum - binary version.
 * Most people will want to use sha256hash() instead which outputs hex.
 * @param dst[out]  Buffer to store result in, which needs to be 32 bytes in length
 *                  (SHA256_DIGEST_LENGTH).
 * @param src[in]   The input data used to generate the checksum.
 * @param n[in]     Length of data.
 */
void sha256hash_binary(char *dst, const char *src, unsigned long n)
{
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	unsigned int md_len;
	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
	if (EVP_DigestInit_ex(mdctx, sha256_function, NULL) != 1)
		abort();
	EVP_DigestUpdate(mdctx, src, n);
	EVP_DigestFinal_ex(mdctx, dst, &md_len);
	EVP_MD_CTX_free(mdctx);
#else
	SHA256_CTX hash;

	SHA256_Init(&hash);
	SHA256_Update(&hash, src, n);
	SHA256_Final(dst, &hash);
#endif
}

/** Generates a SHA256 checksum - ASCII printable string (0011223344..etc..).
 * @param dst[out]  Buffer to store result in, which needs to be 65 bytes minimum.
 * @param src[in]   The input data used to generate the checksum.
 * @param n[in]     Length of data.
 */
char *sha256hash(char *dst, const char *src, unsigned long n)
{
	char binaryhash[SHA256_DIGEST_LENGTH];

	sha256hash_binary(binaryhash, src, n);
	binarytohex(binaryhash, sizeof(binaryhash), dst);
	return dst;
}

/** Calculate the SHA256 checksum of a file */
const char *sha256sum_file(const char *fname)
{
	FILE *fd;
	char buf[2048];
	SHA256_CTX hash;
	char binaryhash[SHA256_DIGEST_LENGTH];
	static char hexhash[SHA256_DIGEST_LENGTH*2+1];
	int n;
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	unsigned int md_len;
	EVP_MD_CTX *mdctx;

	mdctx = EVP_MD_CTX_new();
	if (EVP_DigestInit_ex(mdctx, sha256_function, NULL) != 1)
		abort();
#else
	SHA256_Init(&hash);
#endif

	fd = fopen(fname, "rb");
	if (!fd)
		return NULL;

	while ((n = fread(buf, 1, sizeof(buf), fd)) > 0)
	{
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
		EVP_DigestUpdate(mdctx, buf, n);
#else
		SHA256_Update(&hash, buf, n);
#endif
	}
	fclose(fd);

#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	EVP_DigestFinal_ex(mdctx, binaryhash, &md_len);
	EVP_MD_CTX_free(mdctx);
#else
	SHA256_Final(binaryhash, &hash);
#endif
	binarytohex(binaryhash, sizeof(binaryhash), hexhash);
	return hexhash;
}

/** Generates a SHA1 checksum - binary version.
 * @param dst[out]  Buffer to store result in, which needs to be 32 bytes in length
 *                  (SHA1_DIGEST_LENGTH).
 * @param src[in]   The input data used to generate the checksum.
 * @param n[in]     Length of data.
 * @deprecated      The SHA1 algorithm is deprecated and insecure,
 *                  so only use this if absolutely needed.
 */
void sha1hash_binary(char *dst, const char *src, unsigned long n)
{
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	unsigned int md_len;
	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
	if (EVP_DigestInit_ex(mdctx, sha1_function, NULL) != 1)
		abort();
	EVP_DigestUpdate(mdctx, src, n);
	EVP_DigestFinal_ex(mdctx, dst, &md_len);
	EVP_MD_CTX_free(mdctx);
#else
	SHA_CTX hash;

	SHA1_Init(&hash);
	SHA1_Update(&hash, src, n);
	SHA1_Final(dst, &hash);
#endif
}

/** Remove a suffix from a filename, eg ".c" (if it is present) */
char *filename_strip_suffix(const char *fname, const char *suffix)
{
	static char buf[512];

	strlcpy(buf, fname, sizeof(buf));

	if (suffix)
	{
		int buf_len = strlen(buf);
		int suffix_len = strlen(suffix);
		if (buf_len >= suffix_len)
		{
			if (!strncmp(buf+buf_len-suffix_len, suffix, suffix_len))
				buf[buf_len-suffix_len] = '\0';
		}
	} else {
		char *p = strrchr(buf, '.');
		if (p)
			*p = '\0';
	}
	return buf;
}

/** Add a suffix to a filename, eg ".c" */
char *filename_add_suffix(const char *fname, const char *suffix)
{
	static char buf[512];
	snprintf(buf, sizeof(buf), "%s%s", fname, suffix);
	return buf;
}

/* Returns 1 if the filename has the suffix, eg ".c" */
int filename_has_suffix(const char *fname, const char *suffix)
{
	char buf[256];
	char *p;
	strlcpy(buf, fname, sizeof(buf));
	p = strrchr(buf, '.');
	if (!p)
		return 0;
	if (!strcmp(p, suffix))
		return 1;
	return 0;
}

/** Check if the specified file or directory exists */
int file_exists(const char *file)
{
#ifdef _WIN32
	if (_access(file, 0) == 0)
#else
	if (access(file, 0) == 0)
#endif
		return 1;
	return 0;
}

/** Get the file creation time */
time_t get_file_time(const char *fname)
{
	struct stat st;

	if (stat(fname, &st) != 0)
		return 0;

	return (time_t)st.st_ctime;
}

/** Get the size of a file */
long get_file_size(const char *fname)
{
	struct stat st;

	if (stat(fname, &st) != 0)
		return -1;

	return (long)st.st_size;
}

/** Add a line to a MultiLine list */
void addmultiline(MultiLine **l, const char *line)
{
	MultiLine *m = safe_alloc(sizeof(MultiLine));
	safe_strdup(m->line, line);
	append_ListItem((ListStruct *)m, (ListStruct **)l);
}

/** Free an entire MultiLine list */
void freemultiline(MultiLine *l)
{
	MultiLine *l_next;
	for (; l; l = l_next)
	{
		l_next = l->next;
		safe_free(l->line);
		safe_free(l);
	}
}

/** Convert a line regular string containing \n's to a MultiLine linked list */
MultiLine *line2multiline(const char *str)
{
	static char buf[8192];
	char *p, *p2;
	MultiLine *ml = NULL;

	strlcpy(buf, str, sizeof(buf));
	p = buf;
	do {
		p2 = strchr(p, '\n');
		if (p2)
			*p2++ = '\0';
		addmultiline(&ml, p);
		p = p2;
	} while(p2 && *p2);
	return ml;
}

/** Convert a sendtype to a command string */
const char *sendtype_to_cmd(SendType sendtype)
{
	if (sendtype == SEND_TYPE_PRIVMSG)
		return "PRIVMSG";
	if (sendtype == SEND_TYPE_NOTICE)
		return "NOTICE";
	if (sendtype == SEND_TYPE_TAGMSG)
		return "TAGMSG";
	return NULL;
}

/** Check password strength.
 * @param pass		The password to check
 * @param min_length	The minimum length of the password
 * @param strict	Whether to require UPPER+lower+digits
 * @returns 1 if good, 0 if not.
 */
int check_password_strength(const char *pass, int min_length, int strict, char **err)
{
	static char buf[256];
	char has_lowercase=0, has_uppercase=0, has_digit=0;
	const char *p;

	if (err)
		*err = NULL;

	if (strlen(pass) < min_length)
	{
		if (err)
		{
			snprintf(buf, sizeof(buf), "Password must be at least %d characters", min_length);
			*err = buf;
		}
		return 0;
	}

	for (p=pass; *p; p++)
	{
		if (islower(*p))
			has_lowercase = 1;
		else if (isupper(*p))
			has_uppercase = 1;
		else if (isdigit(*p))
			has_digit = 1;
	}

	if (strict)
	{
		if (!has_lowercase)
		{
			if (err)
				*err = "Password must contain at least 1 lowercase character";
			return 0;
		} else
		if (!has_uppercase)
		{
			if (err)
				*err = "Password must contain at least 1 UPPERcase character";
			return 0;
		} else
		if (!has_digit)
		{
			if (err)
				*err = "Password must contain at least 1 digit (number)";
			return 0;
		}
	}

	return 1;
}

int valid_secret_password(const char *pass, char **err)
{
	return check_password_strength(pass, 10, 1, err);
}

int running_interactively(void)
{
#ifndef _WIN32
	char *s;

	if (!isatty(0))
		return 0;

	s = getenv("TERM");
	if (!s || !strcasecmp(s, "dumb") || !strcasecmp(s, "none"))
		return 0;

	return 1;
#else
	return IsService ? 0 : 1;
#endif
}

int terminal_supports_color(void)
{
#ifndef _WIN32
	char *s;

	/* Support NO_COLOR as per https://no-color.org */
	s = getenv("NO_COLOR");
	if (s != NULL && s[0] != '\0')
		return 0;

	/* Yeah we check all of stdin, stdout, stderr, because one
	 * or more may be redirected (bin/unrealircd >log 2>&1),
	 * and then we want to say no to color support.
	 */
	if (!isatty(0) || !isatty(1) || !isatty(2))
		return 0;

	s = getenv("TERM");
	/* Yeah this is a lazy way to detect color-capable terminals
	 * but it is good enough for me.
	 */
	if (s)
	{
		if (strstr(s, "color") || strstr(s, "ansi"))
			return 1;
	}

	return 0;
#else
	return 0;
#endif
}

/** Skip whitespace (if any) */
void skip_whitespace(char **p)
{
	for (; **p == ' ' || **p == '\t'; *p = *p + 1);
}

/** Keep reading '*p' until we hit any of the 'stopchars'.
 * Actually behaves like strstr() but then hit the end
 * of the string (\0) i guess?
 */
void read_until(char **p, char *stopchars)
{
	for (; **p && !strchr(stopchars, **p); *p = *p + 1);
}

void write_pidfile_failed(void)
{
	char *errstr = strerror(errno);
	unreal_log(ULOG_WARNING, "config", "WRITE_PID_FILE_FAILED", NULL,
		   "Unable to write to pid file '$filename': $system_error",
		   log_data_string("filename", conf_files->pid_file),
		   log_data_string("system_error", errstr));
}

/** Write PID file */
void write_pidfile(void)
{
#ifdef IRCD_PIDFILE
	int fd;
	char buff[20];
	if ((fd = open(conf_files->pid_file, O_CREAT | O_WRONLY, 0600)) < 0)
	{
		write_pidfile_failed();
		return;
	}
	ircsnprintf(buff, sizeof(buff), "%5d\n", (int)getpid());
	if (write(fd, buff, strlen(buff)) < 0)
		write_pidfile_failed();
	if (close(fd) < 0)
		write_pidfile_failed();
#endif
}

/*
 * Determines if the given string is a valid URL. Since libcurl
 * supports telnet, ldap, and dict such strings are treated as
 * invalid URLs here since we don't want them supported in
 * unreal.
 */
int url_is_valid(const char *string)
{
	if (strstr(string, " ") || strstr(string, "\t"))
		return 0;

	if (strstr(string, "telnet://") == string ||
	    strstr(string, "ldap://") == string ||
	    strstr(string, "dict://") == string)
	{
		return 0;
	}
	return (strstr(string, "://") != NULL);
}

/** A displayable URL for in error messages and such.
 * This leaves out any authentication information (user:pass)
 * the URL may contain.
 */
const char *displayurl(const char *url)
{
	static char buf[512];
	char *proto, *rest;

	/* protocol://user:pass@host/etc.. */
	rest = strchr(url, '@');

	if (!rest)
		return url; /* contains no auth information */

	rest++; /* now points to the rest (remainder) of the URL */

	proto = strstr(url, "://");
	if (!proto || (proto > rest) || (proto == url))
		return url; /* incorrectly formatted, just show entire URL. */

	strlncpy(buf, url, sizeof(buf), proto - url);
	strlcat(buf, "://***:***@", sizeof(buf));
	strlcat(buf, rest, sizeof(buf));

	return buf;
}

/*
 * Returns the filename portion of the URL. The returned string
 * is malloc()'ed and must be freed by the caller. If the specified
 * URL does not contain a filename, a '-' is allocated and returned.
 */
char *url_getfilename(const char *url)
{
	const char *c, *start;

	if ((c = strstr(url, "://")))
		c += 3;
	else
		c = url;

	while (*c && *c != '/')
		c++;

	if (*c == '/')
	{
		c++;
		if (!*c || *c == '?')
			return raw_strdup("-");
		start = c;
		while (*c && *c != '?')
			c++;
		if (!*c)
			return raw_strdup(start);
		else
			return raw_strldup(start, c-start+1);

	}
	return raw_strdup("-");
}

#ifdef _WIN32
 // https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/access-waccess
 // mode value   Checks file for
 // 04 	         Read-only
 #define R_OK 04
#endif

/*
 * Checks whether a file can be opened for reading.
 */
int is_file_readable(const char *file, const char *dir)
{
	char *filename = strdup(file);
	convert_to_absolute_path(&filename, dir);
	if (access(filename, R_OK)){
		safe_free(filename);
		return 0;
	}
	safe_free(filename);
	return 1;
}

void delletterfromstring(char *s, char letter)
{
	if (s == NULL)
		return;
	for (; *s; s++)
	{
		if (*s == letter)
		{
			for (; *s; s++)
				*s = s[1];
			break;
		}
	}
}

int sort_character_lowercase_before_uppercase(char x, char y)
{
	/* Lower before upper */
	if (islower(x) && isupper(y))
		return 1;
	if (isupper(x) && islower(y))
		return 0;
	/* Other than that, easy */
	return x < y ? 1 : 0;
}

/* Helper function, mainly used by snomask code */
void addlettertodynamicstringsorted(char **str, char letter)
{
	char *i, *o;
	char *newbuf;
	size_t newbuflen;

	/* NULL string? Easy! */
	if (*str == NULL)
	{
		*str = safe_alloc(2);
		**str = letter;
		return;
	}

	/* Exists? Then nothing to do */
	if (strchr(*str, letter))
		return;

	/* Ok, we really need to add it */
	newbuflen = strlen(*str) + 2;
	newbuf = safe_alloc(newbuflen);
	for (i = *str, o = newbuf; *i; i++)
	{
		/* Insert before a higher letter */
		if (letter && sort_character_lowercase_before_uppercase(letter, *i))
		{
			*o++ = letter;
			letter = '\0';
		}
		*o++ = *i;
	}
	/* Or maybe we should be at the final spot? */
	if (letter)
		*o++ = letter;
	*o = '\0';
	safe_free_raw(*str);
	*str = newbuf;
}

void s_die()
{
#ifdef _WIN32
	Client *client;
	if (!IsService)
	{
		loop.terminating = 1;
		unload_all_modules();

		list_for_each_entry(client, &lclient_list, lclient_node)
			(void) send_queued(client);

		exit(-1);
	}
	else {
		SERVICE_STATUS status;
		SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		SC_HANDLE hService = OpenService(hSCManager, "UnrealIRCd", SERVICE_STOP);
		ControlService(hService, SERVICE_CONTROL_STOP, &status);
	}
#else
	loop.terminating = 1;
	unload_all_modules();
	unlink(conf_files ? conf_files->pid_file : IRCD_PIDFILE);
	exit(0);
#endif
}

#ifndef _WIN32
void s_rehash()
{
	struct sigaction act;
	dorehash = 1;
	act.sa_handler = s_rehash;
	act.sa_flags = 0;
	(void)sigemptyset(&act.sa_mask);
	(void)sigaddset(&act.sa_mask, SIGHUP);
	(void)sigaction(SIGHUP, &act, NULL);
}

void s_reloadcert()
{
	struct sigaction act;
	doreloadcert = 1;
	act.sa_handler = s_reloadcert;
	act.sa_flags = 0;
	(void)sigemptyset(&act.sa_mask);
	(void)sigaddset(&act.sa_mask, SIGUSR1);
	(void)sigaction(SIGUSR1, &act, NULL);
}
#endif // #ifndef _WIN32

void restart(const char *mesg)
{
	server_reboot(mesg);
}

void s_restart()
{
	dorestart = 1;
}

#ifndef _WIN32
/** Signal handler for signals which we ignore,
 * like SIGPIPE ("Broken pipe") and SIGWINCH (terminal window changed) etc.
 */
void ignore_this_signal()
{
	struct sigaction act;

	act.sa_handler = ignore_this_signal;
	act.sa_flags = 0;
	(void)sigemptyset(&act.sa_mask);
	(void)sigaddset(&act.sa_mask, SIGALRM);
	(void)sigaddset(&act.sa_mask, SIGPIPE);
	(void)sigaction(SIGALRM, &act, (struct sigaction *)NULL);
	(void)sigaction(SIGPIPE, &act, (struct sigaction *)NULL);
#ifdef SIGWINCH
	(void)sigaddset(&act.sa_mask, SIGWINCH);
	(void)sigaction(SIGWINCH, &act, (struct sigaction *)NULL);
#endif
}
#endif /* #ifndef _WIN32 */


void server_reboot(const char *mesg)
{
	int i;
	Client *client;
	unreal_log(ULOG_INFO, "main", "UNREALIRCD_RESTARTING", NULL,
	           "Restarting server: $reason",
	           log_data_string("reason", mesg));

	list_for_each_entry(client, &lclient_list, lclient_node)
		(void) send_queued(client);

	/*
	 * ** fd 0 must be 'preserved' if either the -d or -i options have
	 * ** been passed to us before restarting.
	 */
#ifdef HAVE_SYSLOG
	(void)closelog();
#endif
#ifndef _WIN32
	for (i = 3; i < MAXCONNECTIONS; i++)
		(void)close(i);
	if (!(bootopt & (BOOT_TTY | BOOT_DEBUG)))
		(void)close(2);
	(void)close(1);
	(void)close(0);
	close_std_descriptors();
	(void)execv(MYNAME, myargv);
#else
	close_connections();
	if (!IsService)
	{
		CleanUp();
		WinExec(cmdLine, SW_SHOWDEFAULT);
	}
#endif
	loop.terminating = 1;
	unload_all_modules();
#ifdef _WIN32
	if (IsService)
	{
		SERVICE_STATUS status;
		PROCESS_INFORMATION pi;
		STARTUPINFO si;
		char fname[MAX_PATH];
		memset(&status, 0, sizeof(status));
		memset(&si, 0, sizeof(si));
		IRCDStatus.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(IRCDStatusHandle, &IRCDStatus);
		GetModuleFileName(GetModuleHandle(NULL), fname, MAX_PATH);
		CreateProcess(fname, "restartsvc", NULL, NULL, FALSE,
			0, NULL, NULL, &si, &pi);
		IRCDStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(IRCDStatusHandle, &IRCDStatus);
		ExitProcess(0);
	}
	else
#endif
	exit(-1);
}

/** Check if at least 'minimum' seconds passed by since last run.
 * @param tv_old   Pointer to a timeval struct to keep track of things.
 * @param minimum  The time specified in milliseconds (eg: 1000 for 1 second)
 * @returns When 'minimum' msec passed 1 is returned and the time is reset, otherwise 0 is returned.
 */
int minimum_msec_since_last_run(struct timeval *tv_old, long minimum)
{
	long v;

	if (tv_old->tv_sec == 0)
	{
		/* First call ever */
		tv_old->tv_sec = timeofday_tv.tv_sec;
		tv_old->tv_usec = timeofday_tv.tv_usec;
		return 0;
	}
	v = ((timeofday_tv.tv_sec - tv_old->tv_sec) * 1000) + ((timeofday_tv.tv_usec - tv_old->tv_usec)/1000);
	if (v >= minimum)
	{
		tv_old->tv_sec = timeofday_tv.tv_sec;
		tv_old->tv_usec = timeofday_tv.tv_usec;
		return 1;
	}
	return 0;
}

/** Strip color, bold, underline, and reverse codes from a string.
 * @param text			The input text
 * @param output		The buffer for the output text
 * @param outputlen		The length of the output buffer
 * @param strip_flags		Zero or (a combination of) UNRL_STRIP_LOW_ASCII / UNRL_STRIP_KEEP_LF.
 * @returns The new string, which will be 'output', or in unusual cases (outputlen==0) will be NULL.
 */
const char *StripControlCodesEx(const char *text, char *output, size_t outputlen, int strip_flags)
{
	int i = 0, len = strlen(text), save_len=0;
	char nc = 0, col = 0, rgb = 0;
	char *o = output;
	const char *save_text=NULL;

	/* Handle special cases first.. */

	if (outputlen == 0)
		return NULL;

	if (outputlen == 1)
	{
		*output = '\0';
		return output;
	}

	/* Reserve room for the NUL byte */
	outputlen--;

	while (len > 0) 
	{
		if ((col && isdigit(*text) && nc < 2) ||
		    ((col == 1) && (*text == ',') && isdigit(text[1]) && (nc > 0) && (nc < 3)))
		{
			nc++;
			if (*text == ',')
			{
				nc = 0;
				col++;
			}
		}
		/* Syntax for RGB is ^DHHHHHH where H is a hex digit.
		 * If < 6 hex digits are specified, the code is displayed
		 * as text
		 */
		else if ((rgb && isxdigit(*text) && nc < 6) || (rgb && *text == ',' && nc < 7))
		{
			nc++;
			if (*text == ',')
				nc = 0;
		}
		else 
		{
			if (col)
				col = 0;
			if (rgb)
			{
				if (nc != 6)
				{
					text = save_text+1;
					len = save_len-1;
					rgb = 0;
					continue;
				}
				rgb = 0;
			}
			switch (*text)
			{
			case 3:
				/* color */
				col = 1;
				nc = 0;
				break;
			case 4:
				/* RGB */
				save_text = text;
				save_len = len;
				rgb = 1;
				nc = 0;
				break;
			case 2:
				/* bold */
				break;
			case 31:
				/* underline */
				break;
			case 22:
				/* reverse */
				break;
			case 15:
				/* plain */
				break;
			case 29:
				/* italic */
				break;
			case 30:
				/* strikethrough */
				break;
			case 17:
				/* monospace */
				break;
			case 0xe2:
				if (!strncmp(text+1, "\x80\x8b", 2))
				{
					/* +2 means we skip 3 */
					text += 2;
					len  -= 2;
					break;
				}
				/*fallthrough*/
			default:
				if ((*text >= ' ') ||
				    !(strip_flags & UNRL_STRIP_LOW_ASCII) ||
				    ((strip_flags & UNRL_STRIP_KEEP_LF) && (*text == '\n'))
				    )
				{
					*o++ = *text;
					outputlen--;
					if (outputlen == 0)
					{
						*o = '\0';
						return output;
					}
				}
				break;
			}
		}
		text++;
		len--;
	}

	*o = '\0';
	return output;
}

/* strip color, bold, underline, and reverse codes from a string */
const char *StripControlCodes(const char *text)
{
	static unsigned char new_str[4096];

	return StripControlCodesEx(text, new_str, sizeof(new_str), 0);
}

const char *command_issued_by_rpc(MessageTag *mtags)
{
	MessageTag *m = find_mtag(mtags, "unrealircd.org/issued-by");
	if (m && m->value && !strncmp(m->value, "RPC:", 4))
		return m->value;
	return NULL;
}

/** Is 's' a valid spamfilter id? A-Z, 0-9 and _ and with a max length. */
int valid_spamfilter_id(const char *s)
{
	if (strlen(s) > MAXSPAMFILTERIDLEN)
		return 0;
	return 1;
}

void download_complete_dontcare(OutgoingWebRequest *request, OutgoingWebResponse *response)
{
#ifdef DEBUGMODE
	if (response->memory)
	{
		unreal_log(ULOG_DEBUG, "url", "DEBUG_URL_RESPONSE", NULL,
		           "Response for '$url': $response",
		           log_data_string("url", request->url),
		           log_data_string("response", response->memory));
	}
#endif
}

int valid_operclass_character(char c)
{
	/* allow alpha, numeric, -, _ */
	if (!strchr("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-_", c))
		return 0;
	return 1;
}

int valid_operclass_name(const char *str)
{
	const char *p;

	if (strlen(str) > OPERCLASSLEN)
		return 0;

	for (p = str; *p; p++)
		if (!valid_operclass_character(*p))
			return 0;

	return 1;
}

/** Free an OutgoingWebRequest struct - note: use safe_free_outgoingwebrequest() instead (which calls us). */
void free_outgoingwebrequest(OutgoingWebRequest *r)
{
	safe_free(r->apicallback);
	safe_free(r->url);
	safe_free(r->actual_url);
        safe_free(r->body);
        safe_free_nvplist(r->headers);
        safe_free(r);
}

/** Safely duplicate an OutgoingWebRequest struct (eg for https redirects) */
OutgoingWebRequest *duplicate_outgoingwebrequest(OutgoingWebRequest *orig)
{
	OutgoingWebRequest *e = safe_alloc(sizeof(OutgoingWebRequest));

	e->callback = orig->callback;
	safe_strdup(e->apicallback, orig->apicallback);
	e->callback_data = orig->callback_data;
	safe_strdup(e->url, orig->url);
	safe_strdup(e->actual_url, orig->actual_url);
	e->http_method = orig->http_method;
	safe_strdup(e->body, orig->body);
	e->headers = duplicate_nvplist(orig->headers);
	e->store_in_file = orig->store_in_file;
	e->cachetime = orig->cachetime;
	e->max_redirects = orig->max_redirects;
	e->keep_file = orig->keep_file;
	e->connect_timeout = orig->connect_timeout;
	e->transfer_timeout = orig->transfer_timeout;
	return e;
}

/*
 * Handles asynchronous downloading of a file (simple non-flexible version).
 * NOTE: url_start_async() is the more advanced one.
 *
 * This function allows a download to be made transparently without
 * the caller having any knowledge of how libcurl works. The specified
 * callback function is called when the download completes, or the
 * download fails. The callback function is defined as:
 *
 * void callback(const char *url, const char *filename, const char *memory_data, int memory_data_len, char *errorbuf, int cached, void *data);
 *  - url will contain the original URL used to download the file.
 *  - filename will contain the name of the file (if successful, NULL on error or if cached).
 *    This file will be cleaned up after the callback returns, so save a copy to support caching.
 *  - errorbuf will contain the error message (if failed, NULL otherwise).
 *  - cached 1 if the specified cachetime is >= the current file on the server,
 *    if so, errorbuf will be NULL, filename will contain the path to the file.
 *  - data will be the value of callback_data, allowing you to figure
 *    out how to use the data contained in the downloaded file ;-).
 *    Make sure that if you access the contents of this pointer, you
 *    know that this pointer will persist. A download could take more
 *    than 10 seconds to happen and the config file can be rehashed
 *    multiple times during that time.
 */
void download_file_async(const char *url,
                         time_t cachetime,
                         void (*callback)(OutgoingWebRequest *request, OutgoingWebResponse *response),
                         void *callback_data,
                         int maxredirects)
{
	OutgoingWebRequest *request = safe_alloc(sizeof(OutgoingWebRequest));
	safe_strdup(request->url, url);
	request->http_method = HTTP_METHOD_GET;
	request->cachetime = cachetime;
	request->callback = callback;
	request->callback_data = callback_data;
	request->max_redirects = maxredirects;
	request->store_in_file = 1;
	url_start_async(request);
}

void url_callback(OutgoingWebRequest *r, const char *file, const char *memory, int memory_len, const char *errorbuf, int cached, void *ptr)
{
	OutgoingWebResponse *response;

	if (!r->callback && !r->apicallback)
		return; /* Nothing to do */

	response = safe_alloc(sizeof(OutgoingWebResponse));
	response->file = file;
	response->memory = memory;
	response->memory_len = memory_len;
	response->errorbuf = errorbuf;
	response->cached = cached;
	response->ptr = ptr;

	if (r->callback)
	{
		r->callback(r, response);
	} else if (r->apicallback)
	{
		APICallback *cb = APICallbackFind(r->apicallback, API_CALLBACK_WEB_RESPONSE);
		if (cb && !cb->unloaded)
			cb->callback.web_response(r, response);
	}

	safe_free(response);
}

static int synchronous_http_request_in_progress;
static char synchronous_http_request_tmpfile[512];

void synchronous_http_request_handle_response(OutgoingWebRequest *request, OutgoingWebResponse *response)
{
	if (response->errorbuf)
	{
		config_error("%s: %s", request->url, response->errorbuf);
		synchronous_http_request_in_progress = -1;
		return;
	}
	if (response->file)
	{
		strlcpy(synchronous_http_request_tmpfile, response->file, sizeof(synchronous_http_request_tmpfile));
		synchronous_http_request_in_progress = 0;
	} else {
		config_error("%s: Unexpected error, no error but no file", request->url);
		synchronous_http_request_in_progress = -1;
	}
	return;
}

const char *synchronous_http_request(const char *url, int max_redirects, int connect_timeout, int transfer_timeout)
{
	OutgoingWebRequest *request;

	if (loop.booted)
		abort(); /* this function is NOT for modules or the like */

	request = safe_alloc(sizeof(OutgoingWebRequest));
	safe_strdup(request->url, url);
	request->http_method = HTTP_METHOD_GET;
	request->callback = synchronous_http_request_handle_response;
	request->max_redirects = max_redirects;
	request->store_in_file = 1;
	request->keep_file = 1;
	request->connect_timeout = connect_timeout;
	request->transfer_timeout = transfer_timeout;
	url_start_async(request);
	synchronous_http_request_in_progress = 1;

	while (synchronous_http_request_in_progress == 1)
	{
		gettimeofday(&timeofday_tv, NULL);
		timeofday = timeofday_tv.tv_sec;
		url_socket_timeout(NULL);
		unrealdns_timeout(NULL);
		fd_select(500);
	}

	if (synchronous_http_request_in_progress == 0)
		return synchronous_http_request_tmpfile;
	return NULL; /* failure */
}

// TODO: move all that url shit to url_generic.c ? ;)
