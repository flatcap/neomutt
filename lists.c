ADD ONE
	mailbox_email_add(M, E)
	emaillist_email_add(M->EL, E)
		INSERT(EL, E)
	set_parent(E->notify, M->notify)
	notify_send(M->notify, EventE, NEW) // EventE contains EL

DEL ONE
	mailbox_email_del(M, E)
	notify_send(E->notify, EventE, DEL) // EventE contains EL(E)
	emaillist_email_del(M->EL, E)
		REMOVE(EL, E)

ADD MANY
	mailbox_emaillist_add(M, EL)
	emaillist_emaillist_add(M->EL, EL)
		FOREACH(EL2)
			INSERT(EL1, E)
	FOREACH(EL)
		set_parent(E->notify, M->notify)
	notify_send(M->notify, EventE, NEW) // EventE contains EL

DEL MANY
	mailbox_emaillist_del(M, EL)
	notify_send(M->notify, EventE, DEL) // EventE contains EL, notification propagated
	FOREACH(EL)
		notify_send(E->notify, EventE, DEL) // EventE contains E, notification NOT propagated
	emaillist_emaillist_del(M->EL, EL)
		FOREACH(EL2)
			REMOVE(EL1, E)

