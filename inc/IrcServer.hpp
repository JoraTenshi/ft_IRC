#pragma once
enum RPL {
    // ─────────── REGISTRATION / CONNECTION ───────────
    RPL_WELCOME = 001,         // :<server> 001 <nick> :Welcome to IRC
    RPL_YOURHOST = 002,        // :<server> 002 <nick> :Your host is ...
    RPL_CREATED = 003,         // :<server> 003 <nick> :This server was created ...
    RPL_MYINFO = 004,          // :<server> 004 <nick> <server> <version> <user_modes> <chan_modes>
    //TO DO APARTE HASTA 461
    ERR_ALREADYREGISTERED = 462,   // :<server> 462 <nick> :You may not reregister
    ERR_PASSWDMISMATCH = 464,      // :<server> 464 <nick> :Password incorrect
    ERR_ERRONEUSNICKNAME = 432,    // :<server> 432 * <nick> :Erroneous nickname
    ERR_NICKNAMEINUSE = 433,       // :<server> 433 * <nick> :Nickname is already in use
    ERR_NEEDMOREPARAMS = 461,      // :<server> 461 <nick> <command> :Not enough parameters
    // ─────────── CHANNEL MANAGEMENT ───────────
    RPL_CHANNELMODEIS = 324,   // :<server> 324 <nick> #channel +modes
    RPL_NOTOPIC = 331,         // :<server> 331 <nick> #channel :No topic is set
    RPL_TOPIC = 332,           // :<server> 332 <nick> #channel :<topic>
    RPL_NAMREPLY = 353,        // :<server> 353 <nick> = #channel :@nick1 nick2
    RPL_ENDOFNAMES = 366,      // :<server> 366 <nick> #channel :End of NAMES list
    RPL_INVITING = 341,        // :<server> 341 <nick> <target> #channel  //TODO
    ERR_NOSUCHCHANNEL = 403,     // :<server> 403 <nick> #channel :No such channel
    ERR_CANNOTSENDTOCHAN = 404,  // :<server> 404 <nick> #channel :Cannot send to channel
    ERR_USERNOTINCHANNEL = 441,  // :<server> 441 <nick> <target> #channel :They aren't on that channel TODO
    ERR_NOTONCHANNEL = 442,      // :<server> 442 <nick> #channel :You're not on that channel
    ERR_USERONCHANNEL = 443,     // :<server> 443 <nick> <target> #channel :is already on channel //TODO
    ERR_CHANOPRIVSNEEDED = 482,  // :<server> 482 <nick> #channel :You're not channel operator
    ERR_INVITEONLYCHAN = 473,    // :<server> 473 <nick> #channel :Cannot join channel (+i)
    ERR_BADCHANNELKEY = 475,     // :<server> 475 <nick> #channel :Cannot join channel (+k)
    ERR_CHANNELISFULL = 471,     // :<server> 471 <nick> #channel :Cannot join channel (+l)
    ERR_UNKNOWNMODE = 472,       // :<server> 472 <nick> <char> :is unknown mode char to me //TODOO
    ERR_KEYSET = 467,            // :<server> 467 <nick> #channel :Channel key already set
    ERR_INVALIDMODEPARAM = 696,  // :<server> 696 <nick> #channel <char> :Invalid mode parameter  //TODOO
    // ─────────── MESSAGING ───────────
    ERR_NOSUCHNICK = 401,        // :<server> 401 <nick> <target> :No such nick/channel //TODO
    ERR_NOTEXTTOSEND = 412,      // :<server> 412 <nick> :No text to send
    CMD_JOIN = 1000,        // :<nick>!<user>@<host> JOIN #channel
    ERR_NORECIPIENT = 411,      // :<server> 411 <nick> :No recipient given (PRIVMSG)
    ERR_NOTEXTTOSEND = 412,      // :<server> 412 <nick> :No text to send (PRIVMSG)
    /// APART
    CMD_NOTICE,      // :<nick>!<user>@<host> NOTICE <target> :<message>
    CMD_MODE,        // :<nick>!<user>@<host> MODE #channel +mode [params]
    CMD_TOPIC,       // :<nick>!<user>@<host> TOPIC #channel :<new topic>
    CMD_INVITE,      // :<nick>!<user>@<host> INVITE <otherNick> :#channel+
    CMD_KICK        // :<nick>!<user>@<host> KICK #channel <nick> :<reason>
};