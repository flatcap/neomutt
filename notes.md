warning on muttrc synonyms
	this will be removed in x months
	this will be removed after DATE (better)
	suggest correct command

warn on option synonym usage
	{ "pgp_autoencrypt",		DT_SYN,  R_NONE, UL "crypt_autoencrypt", 0 },
	{ "crypt_autoencrypt",	DT_BOOL, R_NONE, OPTCRYPTAUTOENCRYPT, 0 },
	crypt_autoencrypt is the visible option
	warn if pgp_autoencrypt is used

two (or more) stage deprecation

list of old options for
    grep -f LIST myconfig

# Warnings

## Install

Help file for upgrade
    What has changed?
    Why?
    What should I do?
sed script

## Quiet

Read config quietly
Before startup hook
    mutt_message (blue) or mutt_error (red)
    Config Warning: Please Read: /usr/share/doc/mutt/synonyms.txt

## Noisy

Print warning message on every synonym
Config Warning: Please Read: /usr/share/doc/mutt/synonyms.txt
Pause for the user to read

# Stage 1 - Informing (1 month)

The Changes Guide lists all the differences between Mutt and NeoMutt:
    https://www.neomutt.org/changes/

To do:

* Write a short guide to fixing your config files (with scripts)
  These will be installed with NeoMutt

* Create a "New to NeoMutt?" guide
  Link to it from everywhere possible.

* Blanket coverage of all forums alerting users to the change

# Stage 2 - Warning (2 month)

NeoMutt will work as usual.

This might happen in one of two ways:

* Verbosely - warn the user about every old variable name in their config
  Urge them to read the guide to fixing their config
  Wait for a keypress

* Quietly - After startup, display a error message urging the user to
  read the guide.
  No keypress required, so it won't interrupt the user.

# Stage 3 - Removal

NeoMutt will not accept the old config.

NeoMutt may, for a while, recognise the old options and tell the user to
read the upgrade guide.
