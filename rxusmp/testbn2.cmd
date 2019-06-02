/* rexx */
parse source ?
say 'Entering' ?

do vb=3 to 7
  scala.vb = random()
end

do i=300 to 310
  v.i = random()
end

v.307 = ''
v.303 = ''
r = 'funky new value!'

call rxreturnbyname 'scala.'

say 'Exiting' ?
exit date('s') time('c')
