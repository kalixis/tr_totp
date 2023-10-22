/*

	About: TOTP v2.1
	Author: kalixis

*/

#if !defined _samp_included
	#error "Please include a_samp or a_npc before tr_totp"
#endif

#if !defined HTTP_ERROR_MALFORMED_RESPONSE
	#error "You must have a_http include in order to use this one."
#endif

#if defined _tr_totp_included_
	#endinput
#endif
#define _tr_totp_included_

/*

	Define const

*/

#if !defined TR_TOTP_URL
	#error Please define the URL of the TOTP script. Example: #define TR_TOTP_URL "example.com/totp.php"
#endif

#if !defined MAX_FUNCTION_NAME
	#define MAX_FUNCTION_NAME 32
#endif

#if !defined MAX_PLAYER_TOTP
	#define MAX_PLAYER_TOTP 16
#endif

#if !defined MAX_TOTP_SIZE
	#define MAX_TOTP_SIZE 128
#endif

/*

	Define functions

*/

#define TOTP_Create:%0(%1) \
	forward TC_%0(%1); \
	public TC_%0(%1)

#define TOTP_Show::%0(%1) \
	TC_%0(%1)

#define TOTP_Response:%0(%1) \
	forward TR_%0(%1); \
	public TR_%0(%1)

#define TOTP: #

#if !defined isnull
    #define isnull(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

/*

	Forwards

*/

forward OnPlayerTOTPScriptResponse(playerid, response_code, const data[]);

/*

	Vars

*/

static
	totp_player_function[MAX_PLAYERS][MAX_FUNCTION_NAME + 1 char];

/*

	Public functions

*/

stock TOTP_Send(playerid, const function[], const secret[])
{
	new
		info[MAX_TOTP_SIZE + 1];

	totp_player_function[playerid] = !"TR_";
	strcat(totp_player_function[playerid], function);
	format(info, sizeof(info), "secret=%s", secret);
	HTTP(playerid, HTTP_POST, TR_TOTP_URL, info, !"OnPlayerTOTPScriptResponse");
}

/*

	OnPlayerTOTPScriptResponse

*/

public OnPlayerTOTPScriptResponse(playerid, response_code, const data[])
{
	if (funcidx(totp_player_function[playerid]) != -1)
		CallLocalFunction(totp_player_function[playerid], !"iis", playerid, response_code, isnull(data) ? !"\1" : data);
	return 1;
}