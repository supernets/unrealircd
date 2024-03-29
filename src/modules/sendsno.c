/*
 *   Unreal Internet Relay Chat Daemon, src/modules/sendsno.c
 *   (C) 2000-2001 Carsten V. Munk and the UnrealIRCd Team
 *   Moved to modules by Fish (Justin Hammond)
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

#include "unrealircd.h"

CMD_FUNC(cmd_sendsno);

#define MSG_SENDSNO   "SENDSNO"

ModuleHeader MOD_HEADER
  = {
	"sendsno",	/* Name of module */
	"5.0", /* Version */
	"command /sendsno", /* Short description of module */
	"UnrealIRCd Team",
	"unrealircd-6",
    };

/* This is called on module init, before Server Ready */
MOD_INIT()
{
	CommandAdd(modinfo->handle, MSG_SENDSNO, cmd_sendsno, MAXPARA, CMD_SERVER);
	MARK_AS_OFFICIAL_MODULE(modinfo);
	return MOD_SUCCESS;
}

/* Is first run when server is 100% ready */
MOD_LOAD()
{
	return MOD_SUCCESS;
}

/* Called when module is unloaded */
MOD_UNLOAD()
{
	return MOD_SUCCESS;
}

/*
** cmd_sendsno - Written by Syzop, bit based on SENDUMODE from Stskeeps
**      parv[1] = target snomask
**      parv[2] = message text
** Servers can use this to:
**   :server.unreal.net SENDSNO e :Hiiiii
*/
CMD_FUNC(cmd_sendsno)
{
	MessageTag *mtags = NULL;
	const char *sno, *msg, *p;
	Client *acptr;

	if ((parc < 3) || BadPtr(parv[2]))
	{
		sendnumeric(client, ERR_NEEDMOREPARAMS, "SENDSNO");
		return;
	}
	sno = parv[1];
	msg = parv[2];

	new_message(client, recv_mtags, &mtags);

	/* Forward to others... */
	sendto_server(client, 0, 0, mtags, ":%s SENDSNO %s :%s", client->id, parv[1], parv[2]);

	list_for_each_entry(acptr, &oper_list, special_node)
	{
		if (acptr->user->snomask)
		{
			char found = 0;
			for (p = sno; *p; p++)
			{
				if (strchr(acptr->user->snomask, *p))
				{
					found = 1;
					break;
				}
			}
			if (found)
				sendto_one(acptr, mtags, ":%s NOTICE %s :%s", client->name, acptr->name, msg);
		}
	}

	free_message_tags(mtags);
}
