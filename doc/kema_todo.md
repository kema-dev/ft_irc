# KEMA'S TODOLIST

## Ongoing

- [ ] join and welcome

## Commands

- [x] JOIN
- [x] PART
- [x] QUIT
- [x] NICK
- [x] USER ***Removed***
- [x] PONG ***Removed***
- [ ] PRIVMSG (+ Send to all users of current channel) -> dOD
- [ ] TOPIC -> dOD
- [x] OPER
- [x] KICK
- [x] MODE ***Removed***
- [x] AWAY
- [x] NAMES
- [ ] LIST -> dOD
- [ ] NOTICE -> dOD

## Check

- [ ] < realname > may contain space characters.
- [ ] permaban les "username[x]"
- [ ] leaks
- [ ] no "to_string()" and "nullptr"
- [ ] full tests for all errors
- [ ] connect with nc
- [ ] test multiple connections
- [ ] test avec meme nom dans limechat
- [ ] test avec plusieurs espaces dans les noms donnes par limechat
- [ ] free params
- [ ] test irc client + nc (multi client connections)
- [ ] join channel = update status of all users connected to the channel

PASS password
NICK dOD
USER dOD 0 * :dOD Lecon
JOIN #ratio
PRIVMSG #ratio message to ratio
