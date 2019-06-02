/* rexx */
parse source ?

arg it .
if it = '' then
  it = 10

if rxfuncquery('rxpassbyname') then
  call rxfuncadd 'rxpassbyname','rxu','rxpassbyname'

a = date()
t = time()
r = random()

sfrank.jim = date('o')

do i=1 to it
  s.i = time('l')
end

call rxpassbyname 'testbn2','&v. &a &t &s. &r sfrank. btime'
say 'Back inside' ?
call rxvlist

exit
