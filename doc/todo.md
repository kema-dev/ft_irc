# KEMA'S TODOLIST

## Check

- [ ] < realname > may contain space characters.
- [ ] join chan + other user join chan, leave then rejoin chan = segv
- [ ] full tests for all errors
- [ ] test avec meme nom dans limechat
- [ ] test avec plusieurs espaces dans les noms donnes par limechat
- [ ] free : server / user in kqueue / ServerManip / Command / empty chandb with frees
- [ ] leaks

PASS password
NICK dod
USER dburgun 0 * :Dorian Burgun
JOIN #ratio

lime connect
lime join
nc connect
nc join
lime leave
lime join
-> next command = SEGV
-> try to privmsg = user doesnt exist
