^.{0,2}\x02.+\x03
^.{0,2}(\x16\x03).*\x16\x03|^.{0,2}(\x01\x03\x01?).*\x0b
^.{0,2}(\x16\x03).*\x16\x03.*(thawte|equifax secure|rsa data security, inc|verisign, inc|gte cybertrust root|entrust\.net limited)|^.{0,2}(\x01\x03\x01?).*\x0b.*(thawte|equifax secure|rsa data security, inc|verisign, inc|gte cybertrust root|entrust\.net limited)
^.{0,4}[\x01\x02].{0,6}[\x01-?][a-z0-9][\x01-?a-z]*[\x02-\x06][a-z][a-z][fglmoprstuvz]?[aeop]?(um)?[\x01-\x10\x1c][\x01\x03\x04\xFF]
^.{1,2}(v\$\xcf)
^[1-9][0-9]{0,4}[\x09-\x0d]*,[\x09-\x0d]*[1-9][0-9]{0,4}(\x0d\x0a|[\x0d\x0a])?
^.{1}()[\x02\x06][!-~]+ [!-~]+ [0-9][0-9]?[0-9]?[0-9]?[0-9]? "[\x09-\x0d -~]+" ([0-9]|10)|^1(send|get)[!-~]+ "[\x09-\x0d -~]+"
^20[01][\x09-\x0d -~]*AUTHINFO USER|^20[01][\x09-\x0d -~]*news
^.{20}TRTPHOTL\x01\x02
^220[\x09-\x0d -~]* (E?SMTP|[Ss]imple [Mm]ail) userspace flags=REG_NOSUB REG_EXTENDED
^220[\x09-\x0d -~]*ftp
^.{2}\x02.{13}
^.{2}(\x05\x58\x0a\x1d\x03)
^ajprot\x0d\x0a
^[a-z][a-z0-9]+@[1-9][0-9]+
^[a-z][a-z0-9][a-z0-9]+/[1-9][0-9]{0,3}00
^[a-z][a-z0-9\-_]+|login: [\x09-\x0d -~]* name: [^\x00-\x08\x0e-\x1f\x7f-\xff]*( Directory: )
^BEGIN (AUTH|VERIFICATION|GSSAPI) REQUEST\x0a
^CTPv1\.[123] Kamusta[\x00-\xff]*\x0d\x0a
^\|/dev/[a-z]{3}\|[0-9a-z]*\|[0-9]{2}\|[cfk]\|
^dmdt[\x00-\xff]*\x01[\x00-\xff]*(""|\x11\x11|uu)|^tncp[\x00-\xff]*33
^([()]|get)..{1,4}(reg|get|query)|^([()]|get)..*User-Agent: (Mozilla/4\.0 \(compatible; (MSIE 6\.0; Windows NT 5\.1;? ?\)|MSIE 5\.00; Windows 98\)))|Keep-Alive\x0d\x0a\x0d\x0a[26]
^GET /docookie\.php\?uname=|^\xfd\xfd\xfd\xfd\x30\x30\x30\x30\x30
^get (/.download/[ -~]*|/.supernode[ -~]|/.status[ -~]|/.network[ -~]*|/.files|/.hash=[0-9a-f]*/[ -~]*) http/1.1|user-agent: kazaa|x-kazaa(-username|-network|-ip|-supernodeip|-xferid|-xferuid|tag)|^give [0-9]{7,10}
^get /.*icy-metadata:1|icy [1-5][0-9]{2} [^\x00-\x08\x0e-\x1f\x7f-\xff]*(content-type:audio|icy-)
^get /ivs-IVSGetFileChunk|^http/(0\.9|1\.0|1\.1) [1-5][0-9]{2} [^\x00-\x08\x0e-\x1f\x7f-\xff]*(\x23\x23\x23\x23\x23REPLAY_CHUNK_START\x23\x23\x23\x23\x23)
^GETMP3\x0d\x0aFilename|^\x01.{0,3}(\x51\x3a\+|\x51\x32\x3a)|^\x10[\x14-\x16]\x10[\x15-\x17].{0,4}
^get[^\x00-\x08\x0e-\x1f\x7f-\xff]* Accept: application/x-rtsp-tunnelled|^http/(0\.9|1\.0|1\.1) [1-5][0-9]{2} [^\x00-\x08\x0e-\x1f\x7f-\xff]*(a=control:rtsp://)
^gkrellm [23].[0-9].[0-9]\x0a
^gnd[\x01\x02]?.?.?\x01|^gnutella connect/[012]\.[0-9]\x0d\x0a|^get /uri-res/n2r\?urn:sha1:|^get /.*user-agent: (gtk-gnutella|bearshare|mactella|gnucleus|gnotella|limewire|imesh)|^get /.*content-type: application/x-gnutella-packets|^giv [0-9]*:[0-9a-f]*/|^queue [0-9a-f]* [1-9][0-9]?[0-9]?\.[1-9][0-9]?[0-9]?\.[1-9][0-9]?[0-9]?\.[1-9][0-9]?[0-9]?:[1-9][0-9]?[0-9]?[0-9]?|^gnutella[\x00-\xff]*content-type: application/x-gnutella|^.{18,19}lime
gnuclear con(nect/)[^\x00-\x08\x0e-\x1f\x7f-\xff]*(user-agent: gnucleus) [^\x00-\x08\x0e-\x1f\x7f-\xff]*(lan:)
http/(0\.9|1\.0|1\.1) [1-5][0-9]{2} [^\x00-\x08\x0e-\x1f\x7f-\xff]*(connection:|content-type:|content-length:|date:)|post [\x09-\x0d -~]* http/[01]\.[019]
^(invite|register|cancel|message|subscribe|notify) sip[\x09-\x0d -~]*sip/[0-2]\.[0-9]
ipp://
^[lB].?\x0b
^(longaccoun|qsver2auth|\x35[57]\x30|\+\x10\*)
membername.*session.*player
^(\$mynick |\$lock |\$key )
^nick[\x09-\x0d -~]*user[\x09-\x0d -~]*:|^user[\x09-\x0d -~]*:[\x02-\x0d -~]*nick[\x09-\x0d -~]*\x0d\x0a
^notify[\x09-\x0d ]\*[\x09-\x0d ]http/1\.1[\x09-\x0d -~]*ssdp:(alive|byebye)|^m-search[\x09-\x0d ]\*[\x09-\x0d ]http/1\.1[\x09-\x0d -~]*ssdp:discover
^(nq|st)
^\* ok|^a[0-9]+ noop
^(\+ok |-err )
<peerplat>|^get /getfilebyhash\.cgi\?|^get /queue_register\.cgi\?|^get /getupdowninfo\.cgi\?
^post[\x09-\x0d -~]*<PasswordHash>[\x00-\xff]{32}</PasswordHash><ClientVer>|^\x34\x80?\x0d?\xfc\xff\x04|^get[^\x00-\x08\x0e-\x1f\x7f-\xff]*Host: imsh\.download-prod\.musicnet\.com|^\x02[\x01\x02]\x83.*\x02[\x01\x02]\x83
^(Public|AES)Key: [0-9a-f]*\x0aEnd(Public|AES)Key\x0a
rdpdr.*cliprdr.*rdpsnd
^rfb 00[1-9]\.00[0-9]\x0a
rtsp/1.0 200 ok
^ssh-[12]\.[0-9]
<stream:s(tream[\x09-\x0d ])[^\x00-\x1f\x7f-\xff]*([\x09-\x0d ]xmlns=['"]jabber)
^\( success \( 1 2 \(
TOR1.*<identity>
^t\x03ni.?[\x01-\x06]?t[\x01-\x05]s[\x0a\x0b](glob|who are you|query data)
user-agent:aim/
ver [0-9]+ msnp[1-9][0-9]? [^\x00-\x08\x0e-\x1f\x7f-\xff]*cvr0\x0d\x0a|usr 1 [!-~]+ [0-9. ]+\x0d\x0a|ans 1 [!-~]+ [0-9. ]+\x0d\x0a
^ver [ -~]*msnftp\x0d\x0aver msnftp\x0d\x0ausr|^method msnmsgr:
^[^\x00-\x1e\x7f-\xff]+(\x0d\x0a)
^\x01.{4}\x11\x10.{8}\x01
^\x01\x01\x05\x0a
^\x01\x01(\x08\x08|\x1b\x1b)
^[\x01\x02].{15,16}
^[\x01\x02][ -~]*(netascii|octet|mail)
^\*[\x01\x02][\x00-\xff]*\x03\x0b|^\*\x01.?.?.?.?\x01|flapon|toc_signon.*0x
^[\x01\x02][\x01- ]\x06.*c\x82sc
^(\x01[!-~]+|\x02[!-~]+\x0a.[\x01\x02\x03][\x01-\x0a -~]*|[\x03\x04][!-~]+[\x09-\x0d]+[a-z][\x09-\x0d -~]*|\x05[!-~]+[\x09-\x0d]+([a-z][!-~]*[\x09-\x0d]+[1-9][0-9]?[0-9]?|root[\x09-\x0d]+[!-~]+)[\x00-\xff]*)\x0a
^\x01[\x08\x09][\x03\x04]
\x01\x10\x01|\)\x10\x01\x01|0\x10\x01
^\x01\x11\x10\|\xf8\x02\x10\x40\x06
^[\x01-\x13\x16-$]\x01.{0,10}[ -~]+
\x01...\xd3..*\x0c.
^\x01\xf4\x01\xf4
\x02[0-4][0-9]:[0-9]+.*\x03
^\x02\x01\x04[\x00-\xff]+([\xa0-\xa3]\x02[\x01-\x04].?.?.?.?\x02\x01.?\x02\x01.?\x30|\xa4\x06[\x00-\xff]+\x40\x04.?.?.?.?\x02\x01.?\x02\x01.?\x43)
^\x03..?\x08.{2,17}\x05
\x03\x9a\x89\x22\x31\x31\x31\.\x30\x30\x20\x42\x65\x74\x61\x20|\xe2\x3c\x69\x1e\x1c\xe9
^\x03[\]Z].?.?\x05
^[\x04\x05]\x0c.i\x01
^\x05..?|^.{1}()\x01.[ -~]+\x01F..?.?.?.?.?.?.?
\x05[\x01-\x08]*\x05[\x01-\x08]?.*\x05[\x01-\x03][\x01\x03].*\x05[\x01-\x08]?[\x01\x03]
^\x06\xec\x01
^[\x09-\x0d]*[1-9,+tgi][\x09-\x0d -~]*\x09[\x09-\x0d -~]*\x09[a-z0-9.]*\.[a-z][a-z].?.?\x09[1-9]
^\x10here=
^\x10\x03...........\x0a\x02.....\x0e
^\x11\x20\x01...?\x11|^\xfe\xfd.?.?.?.?.?.?(\x14\x01\x06|\xff\xff\xff)|[\]\x01].?battlefield2
^\x11\x20\x01\x90\x50\x64\x10|^\xfe\xfd.?.?.?\x18|^[\x01\\].?battlefield2
^(\x13bittorrent protocol|azver\x01|get /scrape\?info_hash=get /announce\?info_hash=|get /client/bitcomet/|GET /data\?fid=)|d1:ad2:id20:|\x08'7P\)[RP]
^[\x13\x1b\x23\xd3\xdb\xe3]|^[\x14\x1c$].......?.?.?.?.?.?.?.?.?[\xc6-\xff]
^\x1b\xd7\x3b\x48[\x01\x02]\x01?\x01
\x32\x26\x85\x92\x58
^\x58\x80........\xf3|^\x06\x58\x4e
^\x64.....\x70....\x50\x37|^\x65[\x00-\xff]+
^\x80[\x01-"`-\x7f\x80-\xa2\xe0-\xff]?..........*\x80
^\x80\x0c\x01quake\x03
^\x80\x94\x0a\x01....\x1f\x9e
\x81..?.?[A-P]{40}
^[\xc5\xd4\xe3-\xe5].{0,4}[\x01\x02\x05\x14-\x16\x18-\x1c\x20\x21\x32-\x36\x38\x40-\x43\x46-\x58\x60\x81\x82\x90\x91\x93\x96-\x9c\x9e\xa0-\xa4]|^[\xc5\xd4\xe3-\xe5].{0,4}\x59.{15,16}[ -~]|^[\xc5\xd4\xe3-\xe5].{0,4}\x96.{4}
^\xf4\xbe\x03[\x00-\xff]*teamspeak
\xffsmb[\x72\x25]
^\xff[\xfb-\xfe].\xff[\xfb-\xfe].\xff[\xfb-\xfe]
^\xff\xffchallenge
^\xff\xff\xff\xff.*cstrikeCounter-Strike
^\xff\xff\xff\xff.*dodDay of Defeat
^\xff\xff\xff\xffget(info|challenge)
^\xff\xff\xff\xffgetstatus\x0a
^\xff\xff\xff\xff.*hl2mpDeathmatch
^\xff\xff\xff\xff.....*tfTeam Fortress
^\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff..?\x01[\x03\x04]
x-openftalias: [-)(0-9a-z ~.]
YCLC_E|CYEL
^(ymsg|ypns|yhoo).?.?.?.?.?.?.?[lwt][\x00-\xff]*\xc0\x80
