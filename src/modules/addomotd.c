/*
 *   IRC - Internet Relay Chat, src/modules/out.c
 *   (C) 2004 The UnrealIRCd Team
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

#include "unrealircd.h"

CMD_FUNC(cmd_addomotd);

#define MSG_ADDOMOTD 	"ADDOMOTD"	

ModuleHeader MOD_HEADER
  = {
	"addomotd",
	"5.0",
	"command /addomotd", 
	"UnrealIRCd Team",
	"unrealircd-6",
    };

MOD_INIT()
{
	CommandAdd(modinfo->handle, MSG_ADDOMOTD, cmd_addomotd, 1, CMD_USER);
	MARK_AS_OFFICIAL_MODULE(modinfo);
	return MOD_SUCCESS;
}

MOD_LOAD()
{
	return MOD_SUCCESS;
}

MOD_UNLOAD()
{
	return MOD_SUCCESS;
}

/*
** cmd_addomotd (write a line to opermotd)
**
** De-Potvinized by codemastr
*/
CMD_FUNC(cmd_addomotd)
{
	FILE *conf;
	const char *text;

	text = parc > 1 ? parv[1] : NULL;

	if (!MyConnect(client))
		return;

	if (!ValidatePermissionsForPath("server:addomotd",client,NULL,NULL,NULL))
	{
		sendnumeric(client, ERR_NOPRIVILEGES);
		return;
	}
	if (parc < 2)
	{
		sendnumeric(client, ERR_NEEDMOREPARAMS, "ADDOMOTD");
		return;
	}

	conf = fopen(conf_files->opermotd_file, "a");
	if (conf == NULL)
		return;
	fprintf(conf, "%s\n", text);
	fclose(conf);

	sendnotice(client, "*** Wrote (%s) to OperMotd", text);
}
