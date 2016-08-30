
s,e = string.find (mailbox, "/")

-- print (s, e)

if (s) then
  base = string.sub (mailbox, 1, s - 1)
  set_mailbox_color (base, "blue")
else
  set_mailbox_color (mailbox, "red")
end


return 42

