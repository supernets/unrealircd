/************************************************************************
 *   Unreal Internet Relay Chat Daemon, include/numeric.h
 *   Copyright (C) 1990 Jarkko Oikarinen
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
 *
 *   $Id$
 */

/*
 * Added numerics 600-799 as numeric_replies2[], we ran out
 */

/*
 * Reserve numerics 000-099 for server-client connections where the client
 * is local to the server. If any server is passed a numeric in this range
 * from another server then it is remapped to 100-199.
 */

#define	RPL_WELCOME        001
#define	RPL_YOURHOST       002
#define	RPL_CREATED        003
#define	RPL_MYINFO         004
#define RPL_ISUPPORT	   005
#define RPL_REDIR	       10
#define RPL_MAPUSERS	   18
#define RPL_REMOTEISUPPORT 105

/*
 * Errors are in the range from 400-599 currently and are grouped by what
 * commands they come from.
 */
#define ERR_NOSUCHNICK       401
#define ERR_NOSUCHSERVER     402
#define ERR_NOSUCHCHANNEL    403
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_TOOMANYCHANNELS  405
#define ERR_WASNOSUCHNICK    406
#define ERR_TOOMANYTARGETS   407
#define	ERR_NOORIGIN         409
#define ERR_INVALIDCAPCMD    410
#define ERR_NORECIPIENT      411
#define ERR_NOTEXTTOSEND     412
#define ERR_TOOMANYMATCHES   416
#define ERR_INPUTTOOLONG     417
#define ERR_UNKNOWNCOMMAND   421
#define	ERR_NOMOTD           422
#define	ERR_NOADMININFO      423
#define ERR_NOOPERMOTD	     425
#define ERR_TOOMANYAWAY	     429
#define ERR_NONICKNAMEGIVEN  431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE    433
#define ERR_NORULES          434
#define ERR_BANNICKCHANGE    437
#define ERR_NCHANGETOOFAST   438
#define ERR_TARGETTOOFAST    439
#define ERR_SERVICESDOWN     440
#define ERR_USERNOTINCHANNEL 441
#define ERR_NOTONCHANNEL     442
#define	ERR_USERONCHANNEL    443
#define ERR_NONICKCHANGE     447
#define ERR_FORBIDDENCHANNEL 448
#define ERR_NOTREGISTERED    451
#define ERR_NOTFORHALFOPS	 460
#define ERR_NEEDMOREPARAMS   461
#define ERR_ALREADYREGISTRED 462
#define ERR_PASSWDMISMATCH   464
#define ERR_YOUREBANNEDCREEP 465
#define ERR_ONLYSERVERSCANCHANGE 468
#define ERR_LINKCHANNEL	     470
#define ERR_CHANNELISFULL    471
#define ERR_UNKNOWNMODE      472
#define ERR_INVITEONLYCHAN   473
#define ERR_BANNEDFROMCHAN   474
#define	ERR_BADCHANNELKEY    475
#define ERR_NEEDREGGEDNICK   477
#define ERR_BANLISTFULL      478
#define ERR_CANNOTKNOCK		 480
#define ERR_NOPRIVILEGES     481
#define ERR_CHANOPRIVSNEEDED 482
#define ERR_KILLDENY	     485
#define ERR_NOTFORUSERS	     487
#define ERR_SECUREONLYCHAN   489
#define ERR_NOOPERHOST       491
#define ERR_CHANOWNPRIVNEEDED 499
#define ERR_TOOMANYJOINS     500
#define ERR_UMODEUNKNOWNFLAG 501
#define ERR_USERSDONTMATCH   502
#define ERR_SILELISTFULL     511
#define ERR_TOOMANYWATCH     512
#define ERR_TOOMANYDCC       514
#define ERR_DISABLED         517
#define ERR_NOINVITE		 518
#define ERR_OPERONLY		 520
#define ERR_LISTSYNTAX       521
#define ERR_CANTSENDTOUSER	 531
/*
 * Numberic replies from server commands.
 * These are currently in the range 200-399.
 */
#define	RPL_NONE             300
#define RPL_AWAY             301
#define RPL_USERHOST         302
#define RPL_ISON             303
#define RPL_TEXT             304
#define	RPL_UNAWAY           305
#define	RPL_NOWAWAY          306
#define RPL_WHOISREGNICK     307
#define RPL_RULESSTART       308
#define RPL_ENDOFRULES       309

#define RPL_WHOISUSER        311
#define RPL_WHOISSERVER      312
#define RPL_WHOISOPERATOR    313

#define RPL_WHOWASUSER       314
/* rpl_endofwho below (315) */
#define	RPL_ENDOFWHOWAS      369

#define RPL_WHOISIDLE        317

#define RPL_ENDOFWHOIS       318
#define RPL_WHOISCHANNELS    319
#define RPL_WHOISSPECIAL     320
#define RPL_LISTSTART        321
#define RPL_LIST             322
#define RPL_LISTEND          323
#define RPL_CHANNELMODEIS    324
#define RPL_CREATIONTIME     329

#define RPL_WHOISLOGGEDIN    330	/* ircu/charybdis-family --nenolod */

#define RPL_NOTOPIC          331
#define RPL_TOPIC            332
#define RPL_TOPICWHOTIME     333

#define RPL_INVITELIST       336
#define RPL_ENDOFINVITELIST  337

#define RPL_LISTSYNTAX       334
#define RPL_WHOISBOT	     335
#define RPL_USERIP	         340
#define RPL_INVITING         341
#define RPL_WHOISCOUNTRY     344

#define RPL_VERSION          351

#define RPL_WHOREPLY         352
#define RPL_ENDOFWHO         315
#define RPL_NAMREPLY         353
#define RPL_ENDOFNAMES       366
#define RPL_INVEXLIST	     346
#define RPL_ENDOFINVEXLIST   347

#define RPL_EXLIST	         348
#define RPL_ENDOFEXLIST      349
#define	RPL_CLOSING          362
#define RPL_CLOSEEND         363
#define RPL_LINKS            364
#define RPL_ENDOFLINKS       365
/* rpl_endofnames above (366) */
#define RPL_BANLIST          367
#define RPL_ENDOFBANLIST     368
/* rpl_endofwhowas above (369) */

#define	RPL_INFO             371
#define	RPL_MOTD             372
#define	RPL_ENDOFINFO        374
#define	RPL_MOTDSTART        375
#define	RPL_ENDOFMOTD        376

#define RPL_WHOISHOST        378
#define RPL_WHOISMODES       379
#define RPL_YOUREOPER        381
#define RPL_REHASHING        382
#define RPL_QLIST			 386
#define	RPL_ENDOFQLIST		 387
#define	RPL_ALIST			 388
#define	RPL_ENDOFALIST		 389

#define RPL_TIME             391
#define	RPL_USERSSTART       392
#define	RPL_USERS            393
#define	RPL_ENDOFUSERS       394
#define	RPL_NOUSERS          395
#define RPL_HOSTHIDDEN       396

#define RPL_TRACELINK        200
#define RPL_TRACECONNECTING  201
#define RPL_TRACEHANDSHAKE   202
#define RPL_TRACEUNKNOWN     203

#define RPL_TRACEOPERATOR    204
#define RPL_TRACEUSER        205
#define RPL_TRACESERVER      206
#define RPL_TRACENEWTYPE     208
#define RPL_TRACECLASS       209

#define RPL_STATSHELP	     210
#define RPL_STATSLINKINFO    211
#define RPL_STATSCOMMANDS    212
#define RPL_STATSCLINE       213

#define RPL_STATSILINE       215
#define RPL_STATSQLINE       217
#define RPL_STATSYLINE       218
#define RPL_ENDOFSTATS       219


#define RPL_UMODEIS          221
#define RPL_STATSGLINE		 223
#define RPL_STATSTLINE		 224
#define RPL_STATSNLINE	     226
#define RPL_STATSVLINE	     227
#define RPL_STATSBANVER	     228
#define RPL_STATSSPAMF       229
#define RPL_STATSEXCEPTTKL   230
#define RPL_RULES            232
#define	RPL_SERVICE          233

#define	RPL_STATSLLINE       241
#define	RPL_STATSUPTIME      242
#define	RPL_STATSOLINE       243
#define	RPL_STATSHLINE       244
#define RPL_STATSXLINE	     247
#define RPL_STATSULINE       248
#define	RPL_STATSDEBUG	     249
#define RPL_STATSCONN        250

#define	RPL_LUSERCLIENT      251
#define RPL_LUSEROP          252
#define	RPL_LUSERUNKNOWN     253
#define	RPL_LUSERCHANNELS    254
#define	RPL_LUSERME          255
#define	RPL_ADMINME          256
#define	RPL_ADMINLOC1        257
#define	RPL_ADMINLOC2        258
#define	RPL_ADMINEMAIL       259

#define	RPL_TRACELOG         261
#define RPL_TRYAGAIN         263
#define RPL_LOCALUSERS       265
#define RPL_GLOBALUSERS      266

#define RPL_SILELIST         271
#define RPL_ENDOFSILELIST    272
#define RPL_STATSDLINE       275
#define RPL_WHOISCERTFP      276

/*
 * New /MAP format.
 */
#define RPL_MAP              006
#define RPL_MAPMORE          610
#define RPL_MAPEND           007


#define ERR_WHOLIMEXCEED 523
#define ERR_OPERSPVERIFY 524

#define RPL_SNOMASK	     8

/*
 * Numberic replies from server commands.
 * These are also in the range 600-799.
 */

#define RPL_WHOISASN         569
#define RPL_REAWAY           597
#define RPL_GONEAWAY         598
#define RPL_NOTAWAY          599
#define RPL_LOGON            600
#define RPL_LOGOFF           601
#define RPL_WATCHOFF         602
#define RPL_WATCHSTAT        603
#define RPL_NOWON            604
#define RPL_NOWOFF           605
#define RPL_WATCHLIST        606
#define RPL_ENDOFWATCHLIST   607
#define RPL_NOWISAWAY        609

#define RPL_DCCSTATUS        617
#define RPL_DCCLIST          618
#define RPL_ENDOFDCCLIST     619
#define RPL_DCCINFO          620

#define RPL_SPAMCMDFWD       659

#define RPL_STARTTLS         670

#define RPL_WHOISSECURE      671

#define RPL_MONONLINE		730
#define RPL_MONOFFLINE		731
#define RPL_MONLIST			732
#define RPL_ENDOFMONLIST	733
#define ERR_MONLISTFULL		734

#define ERR_MLOCKRESTRICTED	742

#define ERR_CANNOTDOCOMMAND 972
#define ERR_CANNOTCHANGECHANMODE 974

#define ERR_STARTTLS            691

#define ERR_INVALIDMODEPARAM	696

#define RPL_LOGGEDIN            900
#define RPL_LOGGEDOUT           901

#define RPL_SASLSUCCESS         903
#define ERR_SASLFAIL            904
#define ERR_SASLTOOLONG         905
#define ERR_SASLABORTED         906
#define RPL_SASLMECHS           908

/* Numeric texts */

#define STR_RPL_WELCOME				/* 001 */	":%s - holy smokes its %s!%s@%s <- (FAGGOT)"
//#define STR_RPL_YOURHOST			/* 002 */	":Your host is %s, running version %s"
#define STR_RPL_YOURHOST			/* 002 */	":Your host is hidden, we are running version DangerousIRCd-6.6.6"
//#define STR_RPL_CREATED			/* 003 */	":This server was created %s"
#define STR_RPL_CREATED				/* 003 */	":This network was created Fri Apr 1 1990 at 04:20:00 UTC"
#define STR_RPL_MYINFO				/* 004 */	"%s DangerousIRCd-6.6.6 %s %s"
#define STR_RPL_ISUPPORT			/* 005 */	"%s SEMTEX C4=9 DYNAMITE=2 NAILBOMB=3 MOLOTOV=1 RICIN URANIUM REFINERY COCAINE=10 HOOKS=30, FUCKYOU=99 PCP=2 NERVEGAS=10000 IRC_OVER_VOIP=1 VOIP_OVER_IRC=2 lp=34 MUNGO_SMOKE=2 PLANETS=99 DICKS=30 BALLS=60 BOOZE=12 SMOKES=24"
#define STR_RPL_MAP			        /* 006 */	":%s%s %s | Bozos: %*ld (%*.2f%%)%s"
#define STR_RPL_MAPEND				/* 007 */	":KING KONG IS COMING"
#define STR_RPL_SNOMASK				/* 008 */	"+%s :Server notice mask"
#define STR_RPL_REDIR				/* 010 */	"%s %d :Please use this Server/Port instead"
//#define STR_RPL_MAPUSERS			/* 018 */	":%d server%s and %d user%s, average %.2f users per server"
#define STR_RPL_MAPUSERS			/* 018 */	":5023 servers and 1283746 users, average 1874.5 users per server"
#define STR_RPL_REMOTEISUPPORT		/* 105 */	"%s SEMTEX C4=9 DYNAMITE=2 NAILBOMB=3 MOLOTOV=1 RICIN URANIUM REFINERY COCAINE=10 HOOKS=30, FUCKYOU=99 PCP=2 NERVEGAS=10000 IRC_OVER_VOIP=1 VOIP_OVER_IRC=2 lp=34 MUNGO_SMOKE=2 PLANETS=99 DICKS=30 BALLS=60 BOOZE=12 SMOKES=24"
#define STR_RPL_TRACELINK			/* 200 */	"Link %s%s %s %s"
#define STR_RPL_TRACECONNECTING		/* 201 */	"Attempt %s %s"
#define STR_RPL_TRACEHANDSHAKE		/* 202 */	"Handshaking %s %s"
#define STR_RPL_TRACEUNKNOWN		/* 203 */	"???? %s %s"
#define STR_RPL_TRACEOPERATOR		/* 204 */	"Operator %s %s [%s] %lld"
#define STR_RPL_TRACEUSER			/* 205 */	"User %s %s [%s] %lld"
#define STR_RPL_TRACESERVER			/* 206 */	"Server %s %dS %dC %s %s!%s@%s %lld"
#define STR_RPL_TRACENEWTYPE		/* 208 */	"%s 0 %s"
#define STR_RPL_TRACECLASS			/* 209 */	"Class %s %d"
#define STR_RPL_STATSHELP			/* 210 */	":%s"
#define STR_RPL_STATSCOMMANDS		/* 212 */	"%s %u %lu"
#define STR_RPL_STATSCLINE			/* 213 */	"%c %s * %s %d %d %s"
#define STR_RPL_STATSILINE			/* 215 */	"I %s %s %d %d %s %s %d"
#define STR_RPL_STATSQLINE			/* 217 */	"%c %s %lld %lld %s :%s"
#define STR_RPL_STATSYLINE			/* 218 */	"Y %s %d %d %d %d %d"
#define STR_RPL_ENDOFSTATS			/* 219 */	"%c :End of /STATS report"
#define STR_RPL_UMODEIS				/* 221 */	"%s"
#define STR_RPL_STATSGLINE			/* 223 */	"%c %s %lld %lld %s :%s"
#define STR_RPL_STATSTLINE			/* 224 */	"T %s %s %s"
#define STR_RPL_STATSNLINE			/* 226 */	"n %s %s"
#define STR_RPL_STATSVLINE			/* 227 */	"v %s %s %s"
#define STR_RPL_STATSBANVER			/* 228 */	"%s %s"
#define STR_RPL_STATSSPAMF			/* 229 */	"%c %s %s %s %lld %lld %lld %s %s :%s"
#define STR_RPL_STATSEXCEPTTKL		/* 230 */	"%s %s %lld %lld %s :%s"
#define STR_RPL_RULES				/* 232 */	":- %s"
#define STR_RPL_STATSLLINE			/* 241 */	"%c %s * %s %d %d"
#define STR_RPL_STATSUPTIME			/* 242 */	":Server Up %lld days, %lld:%02lld:%02lld"
#define STR_RPL_STATSOLINE			/* 243 */	"%c %s * %s %s %s"
#define STR_RPL_STATSHLINE			/* 244 */	"%c %s * %s %d %d"
#define STR_RPL_STATSXLINE			/* 247 */	"X %s %d"
#define STR_RPL_STATSULINE			/* 248 */	"U %s"
#define STR_RPL_STATSDEBUG			/* 249 */	":%s"
//#define STR_RPL_STATSCONN			/* 250 */	":Highest connection count: %d (%d clients)"
#define STR_RPL_STATSCONN			/* 250 */	":Highest connection count: 1384726 (1384712 clients)"
//#define STR_RPL_LUSERCLIENT	    /* 251 */	":There are %d users and %d invisible on %d servers"
#define STR_RPL_LUSERCLIENT	    	/* 251 */	":There are 1283746 users and 458 invisible on 5918 servers"
//#define STR_RPL_LUSEROP	        /* 252 */	"%d :operator(s) online"
#define STR_RPL_LUSEROP			    /* 252 */	"420 :operator(s) online"
#define STR_RPL_LUSERUNKNOWN		/* 253 */	"%d :niggered connection(s)"
//#define STR_RPL_LUSERCHANNELS		/* 254 */	"%d :channels formed"
#define STR_RPL_LUSERCHANNELS		/* 254 */	"5023 :tents formed"
//#define STR_RPL_LUSERME		        /* 255 */	":I have %d clients and %d servers"
#define STR_RPL_LUSERME			/* 255 */	":I have 7384 clients and 391 servers"
#define STR_RPL_ADMINME				/* 256 */	":Looks like somebody is upset..."
#define STR_RPL_ADMINLOC1			/* 257 */	":%s"
#define STR_RPL_ADMINLOC2			/* 258 */	":%s"
#define STR_RPL_ADMINEMAIL			/* 259 */	":%s"
#define STR_RPL_TRACELOG			/* 261 */	"File %s %d"
//#define STR_RPL_TRYAGAIN			/* 263 */	"%s :Flooding detected. Please wait a while and try again."
#define STR_RPL_TRYAGAIN			/* 263 */	"%s :04slow down nerd"
//#define STR_RPL_LOCALUSERS		    /* 265 */	"%d %d :Current local users %d, max %d"
#define STR_RPL_LOCALUSERS		/* 265 */	"1283746 18267367 :Current local users 1283746, max 18267367"
//#define STR_RPL_GLOBALUSERS		    /* 266 */	"%d %d :Current global users %d, max %d"
#define STR_RPL_GLOBALUSERS		/* 266 */	"2389474 2938475 :Current global users 2389474, max 2938475"
#define STR_RPL_SILELIST			/* 271 */	"%s"
#define STR_RPL_ENDOFSILELIST		/* 272 */	":End of STFU List"
#define STR_RPL_STATSDLINE			/* 275 */	"%c %s %s"
#define STR_RPL_WHOISCERTFP			/* 276 */	"%s :has the secret %s"
#define STR_RPL_AWAY				/* 301 */	"%s :%s"
#define STR_RPL_USERHOST			/* 302 */	":%s %s %s %s %s"
#define STR_RPL_ISON				/* 303 */	":"
//#define STR_RPL_UNAWAY				/* 305 */	":You are no longer marked as being away"
#define STR_RPL_UNAWAY				/* 305 */	":wb"
//#define STR_RPL_NOWAWAY				/* 306 */	":You have been marked as being away"
#define STR_RPL_NOWAWAY				/* 306 */	":be right black"
//#define STR_RPL_WHOISREGNICK		/* 307 */	"%s :is identified for this nick"
#define STR_RPL_WHOISREGNICK		/* 307 */	"%s :is keepin it 💯"
#define STR_RPL_RULESSTART			/* 308 */	":- %s Server Rules - "
#define STR_RPL_ENDOFRULES			/* 309 */	":End of RULES command."
#define STR_RPL_WHOISUSER			/* 311 */	"%s %s %s * :%s"
#define STR_RPL_WHOISSERVER			/* 312 */	"%s %s :%s"
#define STR_RPL_WHOISOPERATOR		/* 313 */	"%s :is %s"
#define STR_RPL_WHOWASUSER			/* 314 */	"%s %s %s * :%s"
#define STR_RPL_ENDOFWHO			/* 315 */	"%s :End of /WHO list."
#define STR_RPL_WHOISIDLE			/* 317 */	"%s %lld %lld :seconds idle, signon time"
#define STR_RPL_ENDOFWHOIS			/* 318 */	"%s :End of /WHOIS list."
#define STR_RPL_WHOISCHANNELS		/* 319 */	"%s :%s"
#define STR_RPL_WHOISSPECIAL		/* 320 */	"%s :%s"
//#define STR_RPL_LISTSTART			/* 321 */	"Channel :Users  Name"
#define STR_RPL_LISTSTART			/* 321 */	"Tent :Mates  Title"
#define STR_RPL_LIST				/* 322 */	"%s %d :%s %s"
//#define STR_RPL_LISTEND				/* 323 */	":End of /LIST"
#define STR_RPL_LISTEND				/* 323 */	":)"
#define STR_RPL_CHANNELMODEIS		/* 324 */	"%s %s %s"
#define STR_RPL_CREATIONTIME		/* 329 */	"%s %lld"
#define STR_RPL_WHOISLOGGEDIN		/* 330 */	"%s %s :is pretending to be "
#define STR_RPL_NOTOPIC			    /* 331 */	"%s :No topic is set."
#define STR_RPL_TOPIC			    /* 332 */	"%s :%s"
#define STR_RPL_TOPICWHOTIME		/* 333 */	"%s %s %lld"
#define STR_RPL_LISTSYNTAX		    /* 334 */	":%s"
#define STR_RPL_WHOISBOT		    /* 335 */	"%s :is a \2FUCKING ROBOT\2 on %s"
#define STR_RPL_INVITELIST		    /* 336 */	":%s"
#define STR_RPL_ENDOFINVITELIST		/* 337 */	":End of /INVITE list."
#define STR_RPL_USERIP			    /* 340 */	":%s %s %s %s %s"
#define STR_RPL_INVITING		    /* 341 */	"%s %s"
#define STR_RPL_WHOISCOUNTRY		/* 344 */	"%s %s :is connecting from %s"
#define STR_RPL_INVEXLIST		    /* 346 */	"%s %s %s %lld"
#define STR_RPL_ENDOFINVEXLIST		/* 347 */	"%s :End of Channel Invite List"
#define STR_RPL_EXLIST			    /* 348 */	"%s %s %s %lld"
#define STR_RPL_ENDOFEXLIST		    /* 349 */	"%s :End of Channel Exception List"
#define STR_RPL_VERSION			    /* 351 */	"GET MOTHER FUCKED IN THE ASS YOU FED FAGGOT WHAT THE FUCK YOU THINK YOURE DOIN UP IN MY /VERSION GET THE RETARD OFF MY NETWORK"
#define STR_RPL_WHOREPLY		    /* 352 */	"%s %s %s %s %s %s :%d %s"
#define STR_RPL_NAMREPLY		    /* 353 */	"%s"
#define STR_RPL_CLOSING			    /* 362 */	"%s :Closed. Status = %d"
#define STR_RPL_CLOSEEND	    	/* 363 */	"%d: Connections Closed"
#define STR_RPL_LINKS		    	/* 364 */	"%s %s :%d %s"
#define STR_RPL_ENDOFLINKS	    	/* 365 */	"%s :End of /LINKS list."
//#define STR_RPL_ENDOFNAMES	    	/* 366 */	"%s :End of /NAMES list."
#define STR_RPL_ENDOFNAMES	    	/* 366 */	"%s :)"
#define STR_RPL_BANLIST		    	/* 367 */	"%s %s %s %lld"
#define STR_RPL_ENDOFBANLIST		/* 368 */	"%s :End of Channel Ban List"
#define STR_RPL_ENDOFWHOWAS		    /* 369 */	"%s :End of WHOWAS"
#define STR_RPL_INFO    			/* 371 */	":Looking for something?"
#define STR_RPL_MOTD	    		/* 372 */	":- %s"
#define STR_RPL_ENDOFINFO	    	/* 374 */	":^ ASS FAGGOTS"
#define STR_RPL_MOTDSTART   		/* 375 */	":- %s Message of the Day - "
#define STR_RPL_ENDOFMOTD	    	/* 376 */	"ERROR :Closing Link:"
//#define STR_RPL_WHOISHOST	    	/* 378 */	"%s :is connecting from %s@%s %s"
#define STR_RPL_WHOISHOST	    	/* 378 */	"%s :has entered the void from %s@%s %s"
#define STR_RPL_WHOISMODES	    	/* 379 */	"%s :is using IRC buffs %s %s"
#define STR_RPL_YOUREOPER   		/* 381 */	":SUP OPER (No Oper Abuse)"
#define STR_RPL_REHASHING   		/* 382 */	"%s :Rehashing moustache mass ass cash"
#define STR_RPL_QLIST	    		/* 386 */	"%s %s"
#define STR_RPL_ENDOFQLIST	    	/* 387 */	"%s :End of Channel Owner List"
#define STR_RPL_ALIST		    	/* 388 */	"%s %s"
#define STR_RPL_ENDOFALIST	    	/* 389 */	"%s :End of Protected User List"
#define STR_RPL_TIME		    	/* 391 */	"%s :SMOKE:30"
#define STR_RPL_HOSTHIDDEN	    	/* 396 */	"%s :is now your displayed host"
#define STR_ERR_NOSUCHNICK	    	/* 401 */	"%s :No such nick/channel"
#define STR_ERR_NOSUCHSERVER		/* 402 */	"%s :No such server"
#define STR_ERR_NOSUCHCHANNEL		/* 403 */	"%s :No such channel"
#define STR_ERR_CANNOTSENDTOCHAN	/* 404 */	"%s :%s (%s)"
#define STR_ERR_TOOMANYCHANNELS		/* 405 */	"%s :04slow down nerd"
#define STR_ERR_WASNOSUCHNICK		/* 406 */	"%s :There was no such nickname"
#define STR_ERR_TOOMANYTARGETS		/* 407 */	"%s :04slow down nerd"
#define STR_ERR_NOORIGIN	    	/* 409 */	":No origin specified"
#define STR_ERR_INVALIDCAPCMD		/* 410 */	"%s :Invalid CAP subcommand"
#define STR_ERR_NORECIPIENT	    	/* 411 */	":No recipient given (%s)"
#define STR_ERR_NOTEXTTOSEND		/* 412 */	":No text to send"
#define STR_ERR_TOOMANYMATCHES		/* 416 */	"%s :%s"
#define STR_ERR_INPUTTOOLONG		/* 417 */	":Input line was too long"
#define STR_ERR_UNKNOWNCOMMAND		/* 421 */	"%s :Unknown command"
#define STR_ERR_NOMOTD		    	/* 422 */	":MOTD File is missing"
#define STR_ERR_NOADMININFO	    	/* 423 */	"%s :No administrative info available"
#define STR_ERR_NOOPERMOTD	    	/* 425 */	":OPERMOTD File is missing"
#define STR_ERR_TOOMANYAWAY	    	/* 429 */	":04slow down nerd"
#define STR_ERR_NONICKNAMEGIVEN		/* 431 */	":No nickname given"
#define STR_ERR_ERRONEUSNICKNAME	/* 432 */	"%s :Nickname is unavailable: %s"
#define STR_ERR_NICKNAMEINUSE		/* 433 */	"%s :JUPED LOL OWNED FAG"
#define STR_ERR_NORULES			    /* 434 */	":RULES File is missing"
#define STR_ERR_BANNICKCHANGE		/* 437 */	"%s :8,4   E N T E R   T H E   V O I D   "
#define STR_ERR_NCHANGETOOFAST		/* 438 */	"%s :04slow down nerd"
#define STR_ERR_TARGETTOOFAST		/* 439 */	"%s :04slow down nerd"
#define STR_ERR_SERVICESDOWN		/* 440 */	"%s :Services are currently down. Please try again later."
#define STR_ERR_USERNOTINCHANNEL	/* 441 */	"%s %s :They aren't on that channel"
#define STR_ERR_NOTONCHANNEL		/* 442 */	"%s :8,4   E N T E R   T H E   V O I D   "
#define STR_ERR_USERONCHANNEL		/* 443 */	"%s %s :is already on channel"
#define STR_ERR_NONICKCHANGE		/* 447 */	":04   E N T E R   T H E   V O I D   "
#define STR_ERR_FORBIDDENCHANNEL	/* 448 */	"%s :04   E N T E R   T H E   V O I D   "
#define STR_ERR_NOTREGISTERED		/* 451 */	":You have not registered"
#define STR_ERR_NOTFORHALFOPS		/* 460 */	":8,4   E N T E R   T H E   V O I D   "
#define STR_ERR_NEEDMOREPARAMS		/* 461 */	"%s :Not enough parameters"
#define STR_ERR_ALREADYREGISTRED	/* 462 */	":You may not reregister"
#define STR_ERR_PASSWDMISMATCH		/* 464 */	":04   E N T E R   T H E   V O I D   "
#define STR_ERR_YOUREBANNEDCREEP	/* 465 */	":8,4   E N T E R   T H E   V O I D   "
#define STR_ERR_ONLYSERVERSCANCHANGE	/* 468 */	"%s :04   E N T E R   T H E   V O I D   "
#define STR_ERR_LINKCHANNEL				/* 470 */	"%s %s :[Link] %s has become full, so you are automatically being transferred to the linked channel %s"
#define STR_ERR_CHANNELISFULL			/* 471 */	"%s :8,4   E N T E R   T H E   V O I D   "
#define STR_ERR_UNKNOWNMODE				/* 472 */	"%c :8,4   E N T E R   T H E   V O I D   "
#define STR_ERR_INVITEONLYCHAN			/* 473 */	"%s :8,4   E N T E R   T H E   V O I D   "
#define STR_ERR_BANNEDFROMCHAN			/* 474 */	"%s :04   E N T E R   T H E   V O I D   "
#define STR_ERR_BADCHANNELKEY			/* 475 */	"%s :04   E N T E R   T H E   V O I D   "
#define STR_ERR_NEEDREGGEDNICK			/* 477 */	":8,4   E N T E R   T H E   V O I D   "
#define STR_ERR_BANLISTFULL				/* 478 */	"%s %s :Channel ban/ignore list is full"
#define STR_ERR_CANNOTKNOCK				/* 480 */	":8,4   E N T E R   T H E   V O I D   "
#define STR_ERR_NOPRIVILEGES			/* 481 */	":04   E N T E R   T H E   V O I D   "
#define STR_ERR_CHANOPRIVSNEEDED		/* 482 */	"%s :04   E N T E R   T H E   V O I D   "
#define STR_ERR_KILLDENY				/* 485 */	":04   E N T E R   T H E   V O I D   "
#define STR_ERR_NOTFORUSERS				/* 487 */	":04   E N T E R   T H E   V O I D   "
#define STR_ERR_SECUREONLYCHAN			/* 489 */	"%s :04   E N T E R   T H E   V O I D   "
//#define STR_ERR_NOOPERHOST				/* 491 */	":No O-line for your host"
#define STR_ERR_NOOPERHOST				/* 491 */	":You are now an IRC Operator"
#define STR_ERR_CHANOWNPRIVNEEDED		/* 499 */	"%s :04   E N T E R   T H E   V O I D   "
#define STR_ERR_TOOMANYJOINS			/* 500 */	"%s :04slow down nerd"
#define STR_ERR_UMODEUNKNOWNFLAG		/* 501 */	":Unknown MODE flag"
#define STR_ERR_USERSDONTMATCH			/* 502 */	":04   E N T E R   T H E   V O I D   "
#define STR_ERR_SILELISTFULL			/* 511 */	"%s :Your STFU list is full"
#define STR_ERR_TOOMANYWATCH			/* 512 */	"%s :Maximum size for WATCH-list is 128 entries"
#define STR_ERR_TOOMANYDCC				/* 514 */	"%s :04   E N T E R   T H E   V O I D   "
#define STR_ERR_DISABLED				/* 517 */	"%s :%s" /* ircu */
#define STR_ERR_NOINVITE				/* 518 */	":04   E N T E R   T H E   V O I D   "
#define STR_ERR_OPERONLY				/* 520 */	":04   E N T E R   T H E   V O I D   "
#define STR_ERR_LISTSYNTAX				/* 521 */	":Bad list syntax, type /quote list ? or /raw list ?"
#define STR_ERR_WHOLIMEXCEED			/* 523 */	":Error, /who limit of %d exceeded. Please narrow your search down and try again"
#define STR_ERR_OPERSPVERIFY			/* 524 */	":Trying to join +s or +p channel as an oper. Please invite yourself first."
#define STR_ERR_CANTSENDTOUSER			/* 531 */	"%s :%s"
#define STR_RPL_REAWAY			/* 597 */	"%s %s %s %lld :%s"
#define STR_RPL_GONEAWAY		/* 598 */	"%s %s %s %lld :%s"
#define STR_RPL_NOTAWAY			/* 599 */	"%s %s %s %lld :is no longer away"
#define STR_RPL_LOGON			/* 600 */	"%s %s %s %lld :logged online"
#define STR_RPL_LOGOFF			/* 601 */	"%s %s %s %lld :logged offline"
#define STR_RPL_WATCHOFF		/* 602 */	"%s %s %s %lld :stopped watching"
#define STR_RPL_WATCHSTAT		/* 603 */	":You have %d and are on %d WATCH entries"
#define STR_RPL_NOWON			/* 604 */	"%s %s %s %lld :is online"
#define STR_RPL_NOWOFF			/* 605 */	"%s %s %s %lld :is offline"
#define STR_RPL_WATCHLIST		/* 606 */	":%s"
#define STR_RPL_ENDOFWATCHLIST		/* 607 */	":End of WATCH %c"
#define STR_RPL_NOWISAWAY		/* 609 */	"%s %s %s %lld :is away"
#define STR_RPL_MAPMORE			/* 610 */	":%s%-*s --> *more*"
#define STR_RPL_DCCSTATUS		/* 617 */	":%s has been %s your DCC allow list"
#define STR_RPL_DCCLIST			/* 618 */	":%s"
#define STR_RPL_ENDOFDCCLIST		/* 619 */	":End of DCCALLOW %s"
#define STR_RPL_DCCINFO			/* 620 */	":%s"
#define STR_RPL_SPAMCMDFWD		/* 659 */	"%s :Command processed, but a copy has been sent to ircops for evaluation (anti-spam) purposes. [%s]"
#define STR_RPL_STARTTLS		/* 670 */	":STARTTLS successful, go ahead with TLS handshake" /* kineircd */
#define STR_RPL_WHOISSECURE		/* 671 */	"%s :%s" /* our variation on the kineircd numeric */
#define STR_ERR_STARTTLS		/* 691 */	":%s"
#define STR_ERR_INVALIDMODEPARAM	/* 696 */	"%s %c %s :%s"
#define STR_RPL_MONONLINE		/* 730 */	":%s!%s@%s"
#define STR_RPL_MONOFFLINE		/* 731 */	":%s"
#define STR_RPL_MONLIST			/* 732 */	":%s"
#define STR_RPL_ENDOFMONLIST		/* 733 */	":End of MONITOR list"
#define STR_ERR_MONLISTFULL		/* 734 */	"%d %s :Monitor list is full."
#define STR_ERR_MLOCKRESTRICTED		/* 742 */	":04   E N T E R   T H E   V O I D   "
#define STR_RPL_LOGGEDIN		/* 900 */	"%s!%s@%s %s :You are now in the matrix as %s."
#define STR_RPL_LOGGEDOUT		/* 901 */	"%s!%s@%s :EXITED THE MATRIX"
#define STR_RPL_SASLSUCCESS		/* 903 */	":SASL authentication successful"
#define STR_ERR_SASLFAIL		/* 904 */	":SASL authentication failed"
#define STR_ERR_SASLTOOLONG		        /* 905 */	":SASL message too long"
#define STR_ERR_SASLABORTED		        /* 906 */	":SASL authentication aborted"
#define STR_RPL_SASLMECHS		        /* 908 */	"%s :are available SASL mechanisms"
#define STR_ERR_CANNOTDOCOMMAND	    	/* 972 */	"%s :04slow down nerd"
#define STR_ERR_CANNOTCHANGECHANMODE	/* 974 */	"%c :%s"
