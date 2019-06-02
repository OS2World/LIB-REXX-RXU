/* rexx */

arg pid type .

rci. = ''
rci.0   = 'NO_ERROR'
rci.13  = 'ERROR_INVALID_DATA'
rci.217 = 'ERROR_ZOMBIE_PROCESS'
rci.303 = 'ERROR_INVALID_PROCID'
rci.305 = 'ERROR_NOT_DESCENDANT'
lit.t = 'those process(es)'
lit.p = 'that process'
tab = '09'x
crlf = '0d0a'x
explist = 'tab crlf s. lvl'

if pid = '' then
  do
  say 'Syntax:';say
  say tab || 'KILL  pid  type'
  say;say 'where:';say
  say tab || 'pid' || tab || '- process-id or .EXE name prefix'
  say tab || tab || '  (process-id can be hex like "xnnn", decimal like "nnn")'
  say tab || 'type' || tab || '- P (just the process)'
  say tab || tab || '  T (tree, i.e. process and its decendants)'
  exit
  end

if type = '' then
  type = 'P'
else
  do
  type = left(type,1)
  if pos(type,'PT') = 0 then
    do
    say 'Second argument, if supplied, must be either P (for process) or T (for tree)'
    exit
    end
  end

procname = ''

if left(pid,1) = 'X' then
  do
  pid = substr(pid,2)
  if datatype(pid,'x') then
    do
    xpid = pid
    pid = x2d(pid)
    end
  else
    procname = 'X'||pid
  end
else
  do
  if datatype(pid,'w') then
    xpid = d2x(pid)
  else
    procname = pid
  end

if rxfuncquery('rxkillprocess') then
  do
  call rxfuncadd 'rxuinit','rxu','rxuinit'
  call rxuinit
  end

say
dosrc = rxqprocstatus('s.')
do i=1 to s.0p.0
  if procname = '' then
    do
    if x2d(s.0p.i.1) = pid then
      do
      procname = s.0p.i.6
      call killProcess
      exit
      end
    end
  else
    do
    if abbrev(translate(filespec('n',s.0p.i.6)),translate(procname)) then
      do
      procname = s.0p.i.6
      xpid = strip(s.0p.i.1,'l','0')
      pid = x2d(xpid)
      call killProcess
      exit
      end
    end
end

if procname = '' then
  say 'Process (pid=0x'||d2x(pid)'='pid') not found'
else
  say 'Process named "'procname'" not found'

exit

killProcess:
say 'Kill process(es):';say
select
  when type = 'T' then
    do
    call charout ,'(pid=0x'||xpid'='pid') "'procname'"'
    lvl = 0
    call getTree pid
    end
  when type = 'P' then
    call charout ,'(pid=0x'||xpid'='pid') "'procname'"'
  otherwise
    nop
end
call charout ,crlf||'Ok (y/n) [n]? '
resp = translate(charin())
call rxpmprintf ,'>'resp'<'l
call charin ,,1 /* to pull extra crlf from stdin */
if left(resp,1) = 'Y' then
  do
  dosrc = rxkillprocess(pid,type)
  say
  if dosrc = 0 then
    say 'Ok, process (pid=0x'||xpid'='pid') "'procname'"' ||crlf || tab || 'killed with rc='dosrc '('rci.dosrc')'
  else
    say 'Process (pid=0x'||xpid'='pid') "'procname'"' ||crlf || tab || 'NOT killed because rc='dosrc '('rci.dosrc')'
  end
else
  do
  say;say 'Ok, did NOT kill' lit.type
  end
return

getTree: procedure expose (explist)
arg inpid
lvl = lvl + 1
do i=1 to s.0p.0
  if x2d(s.0p.i.2) = inpid then
    do
    xpid = strip(s.0p.i.1,'l','0')
    pid = x2d(xpid)
    procname = s.0p.i.6
    say crlf||copies(tab,lvl) || '(pid=0x'||xpid'='pid') "'procname'"'
    call getTree x2d(s.0p.i.1)
    end
end
lvl = lvl - 1
return
