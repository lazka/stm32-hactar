# Copyright 2012 Christoph Reiter
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.

# Generates a calltree file which uses ELF symbols as functions
# and the size of each symbol as call cost. It can be opened by
# kcachegrind.
#
# MEMORY calls RAM and FLASH, which each have the cost of the available
# memory. RAM calls RAM_USED and FLASH calls FLASH_USED which both have no
# cost but call all ELF sections contained in RAM or FLASH which have
# the their size in bytes as cost.

from subprocess import Popen, PIPE
from os.path import basename
import sys

FLASH_ORIGIN = 0x08000000
FLASH_LENGTH = 128 * 1024

RAM_ORIGIN = 0x20000000
RAM_LENGTH = 20 * 1024

NM_COMMANDS = ["nm", "arm-none-eabi-nm.exe"]

###############################################################################

if len(sys.argv) != 2:
    print >> sys.stderr, "Usage: %s objfile.elf > foo.out" % __file__
    raise SystemExit

# Now get all values for symbols with size values
symbols = {}
for cmd in NM_COMMANDS:
    try:
        output = Popen([cmd, "-P", "-a", "--size-sort", "-l", "-td",
                        sys.argv[1]], stdout=PIPE).communicate()[0]
    except OSError: pass
    else: break
else:
    print >> sys.stderr, "Could not find %s." % (" or ".join(NM_COMMANDS))
    raise SystemExit

for line in output.splitlines():
    s = line.split()

    # some have no file
    if len(s) != 5:
        s.append("")

    name, type_, pos, size, source = s
    symbols[name] = {"position": int(pos), "size": int(size),
                     "source": basename(source).split(":")[0],
                     "type": type_}

# Now move them in the right memory sections
sections = {"FLASH": {}, "RAM": {}}
for name, info in symbols.iteritems():
    # All non BSS is in FLASH (It has to be loaded into RAM on each reset)
    if info["type"].lower() != "b":
        sections["FLASH"][name] = info
    if RAM_ORIGIN + RAM_LENGTH >= info["position"] >= RAM_ORIGIN:
        sections["RAM"][name] = info

###############################################################################

# We have one event: size
print "events: Size"

# Now print the dummy main section
print "fl=" # Unkown file
print "fn=MEMORY" # call it memory

# Memory "calls" RAM and FLASH
sizes = {"FLASH": FLASH_LENGTH, "RAM": RAM_LENGTH}
for section, symbols in sections.iteritems():
    print "cfl=" # also unknown file
    print "cfn=%s" % section # the function names
    print "calls=1 0"
    print "0 %d" % sizes[section]

# Each section hast the cost of their possible memory size
# and calls the used memory section
for section, symbols in sections.iteritems():
    print "fl=" # unkown file
    print "fn=%s" % section # section
    print "0 %d" % sizes[section] # line 0, ram/flash size

    size = 0
    for name, info in symbols.iteritems():
        size += info["size"]

    print "cfl="
    print "cfn=%s_USED" % section
    print "calls=1 0" # called one time, line 0
    print "0 %d" % size

# Each section calls their symbols
for section, symbols in sections.iteritems():
    print "fl=" # unkown file
    print "fn=%s_USED" % section # section

    for name, info in symbols.iteritems():
        print "cfl=%(source)s" % info
        print "cfn=%s" % name
        print "calls=1 0" # called one time, line 0
        print "0 %(size)d" % info

# define each symbol once
for name, info in symbols.iteritems():
    print "fl=%(source)s" % info
    print "fn=%s" % name
    print "0 %(size)d" % info
